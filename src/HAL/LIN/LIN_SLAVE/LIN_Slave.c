/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "HAL/LIN_Types.h"
#include "MCAL/GPIO_DRIVER.h"
#include "MCAL/USART_DRIVER.h"

#include "HAL/LIN_SLAVE/LIN_Slave.h"
#include "HAL/LIN_SLAVE/LIN_Slave_config.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define LIN_SYNC_BYTE 0x55
#define LIN_GET_ID_MASK 0x3F
#define LIN_GET_1ST_BIT_MASK 0x01
#define LIN_CHK_SUM_INVERSION_MASK 0xFF
#define LIN_MAX_ID 0x3F
#define LIN_TEMP_DATA_SIZE 9

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    u8_t Sync_Byte;
    u8_t PID;
} Header_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

static Header_t LIN_Header;
static USART_Req_t Header_Buffer;
static USART_Req_t Send_Buffer;
static USART_Req_t Recieve_Buffer;
static u8_t USART_Peri;
static u8_t Temp_DataReceive[LIN_TEMP_DATA_SIZE];
static LIN_Message_t *CurrMSG;
extern const LIN_Message_t Slave_1MessageArr[_SLAVE1_MSG_NUM];

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static Error_Status LIN_SlaveTask();
static void Receive_Header(void);
static void RecieveTempData(void);
static Error_Status ReceiveData(void);
static void SendData(void);
static u8_t Calculate_Checksum(u8_t PID, u8_t *Data, u8_t size);
static u8_t CalculatePid(u8_t ID);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status LIN_SlaveInit(LIN_cfg_t LIN_CfgArr)
{
    Error_Status LOC_Status = Status_NOK;
    GPIO_Pin_t LIN_Pins[2] = {[0] = {.Pin = LIN_CfgArr.TX_Pin.Pin, .Port = LIN_CfgArr.TX_Pin.Port, .Mode = GPIO_MODE_AF_PP, .Speed = GPIO_SPEED_VHIGH},
                              [1] = {.Pin = LIN_CfgArr.RX_Pin.Pin, .Port = LIN_CfgArr.RX_Pin.Port, .Mode = GPIO_MODE_AF_PP, .Speed = GPIO_SPEED_VHIGH}};
    u8_t LOC_Alternatefunc = 0;

    switch (LIN_CfgArr.address)
    {
    case USART_Peri_1:
    case USART_Peri_2:
        LOC_Alternatefunc = GPIO_FUNC_AF7;
        break;
    case USART_Peri_6:
        LOC_Alternatefunc = GPIO_FUNC_AF8;
        break;
    default:
        break;
    }

    GPIO_CFG_AlternateFunction(LIN_Pins[0].Port, LIN_Pins[0].Pin, LOC_Alternatefunc);
    GPIO_CFG_AlternateFunction(LIN_Pins[1].Port, LIN_Pins[1].Pin, LOC_Alternatefunc);

    USART_cfg_t USART_LIN_CFG =
        {
            .address = LIN_CfgArr.address,
            .BaudRate = LIN_CfgArr.BaudRate,
            .WordLength = LIN_CfgArr.WordLength,
            .ParityControl = LIN_CfgArr.ParityControl,
            .ParitySelect = LIN_CfgArr.ParityControl,
            .StopBits = LIN_CfgArr.StopBits,
            .OverSampling = LIN_CfgArr.OverSampling};

    USART_LIN_cfg_t LIN_CFG = {
        .USART_Peri = LIN_CfgArr.address,
        .LIN_Mode = LIN_CfgArr.LIN_Mode,
        .LIN_IRQ = LIN_CfgArr.LIN_IRQ,
        .LIN_BreakLength = LIN_CfgArr.LIN_BreakLength};

    USART_Peri = LIN_CfgArr.address;
    Header_Buffer.USART_Peri = USART_LIN_CFG.address;
    Header_Buffer.buffer = (u8_t *)&LIN_Header;
    Header_Buffer.length = 2;
    Header_Buffer.CB = LIN_SlaveTask;

    LOC_Status = GPIO_Init(LIN_Pins, 2);
    LOC_Status = USART_Init(USART_LIN_CFG);
    LOC_Status = USART_LIN_Init(LIN_CFG);
    LOC_Status = USART_Set_BreakCallBack(USART_LIN_CFG.address, Receive_Header);

    return LOC_Status;
}

void Receive_Header(void)
{
    USART_RXBufferAsyncZC(Header_Buffer);
}

Error_Status LIN_SlaveTask()
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Index;
    u8_t PID;
    CurrMSG = NULL;

    if (LIN_Header.Sync_Byte != LIN_SYNC_BYTE)
    {
        LOC_Status = Status_LIN_OUT_SYNC;
    }
    else
    {
        LOC_Status = Status_OK;
        PID = CalculatePid(LIN_Header.PID & LIN_GET_ID_MASK);
        if (LIN_Header.PID == PID)
        {
            for (Index = 0; Index < _SLAVE1_MSG_NUM; Index++)
            {
                if (PID == Slave_1MessageArr[Index].PID)
                {
                    CurrMSG = &Slave_1MessageArr[Index];
                }
            }
            if (CurrMSG) /* if a Message with the same ID as received found */
            {
                switch (CurrMSG->Relation)
                {
                case Sender:
                    SendData();
                    break;

                case Receiver:
                    RecieveTempData();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return LOC_Status;
}

static void RecieveTempData(void)
{
    Recieve_Buffer.USART_Peri = USART_Peri;
    Recieve_Buffer.buffer = Temp_DataReceive; // ####
    Recieve_Buffer.length = CurrMSG->Data_Length + 1;
    Recieve_Buffer.CB = ReceiveData;
    USART_RXBufferAsyncZC(Recieve_Buffer);
}

static void SendData(void)
{
    u8_t CheckSum;

    CheckSum = Calculate_Checksum(CurrMSG->PID, CurrMSG->Data, CurrMSG->Data_Length);
    Send_Buffer.USART_Peri = USART_Peri;
    Send_Buffer.buffer = CurrMSG->Data;
    Send_Buffer.length = CurrMSG->Data_Length + 1;
    Send_Buffer.buffer[CurrMSG->Data_Length] = CheckSum;
    USART_TXBufferAsyncZC(Send_Buffer);
}

static Error_Status ReceiveData(void)
{
    u8_t CheckSum;
    u8_t Index;
    Error_Status LOC_Status = Status_NOK;

    CheckSum = Calculate_Checksum(CurrMSG->PID, Temp_DataReceive, CurrMSG->Data_Length);

    if (CheckSum == Temp_DataReceive[CurrMSG->Data_Length])
    {
        LOC_Status = Status_OK;
        for (Index = 0; Index < CurrMSG->Data_Length; Index++)
        {
            CurrMSG->Data[Index] = Temp_DataReceive[Index];
        }
    }
    else
    {
        LOC_Status = Status_LIN_WRONG_CHK_SUM;
    }

    return LOC_Status;
}

static u8_t CalculatePid(u8_t ID)
{
    u8_t Index;
    u8_t PID;

    if (ID > LIN_MAX_ID)
    {
        /* invalid ID */
    }
    else
    {
        u8_t ID_Bits[6];
        for (Index = 0; Index < 6; Index++)
        {
            ID_Bits[Index] = (ID >> Index) & 0x01;
        }

        u8_t P0 = (ID_Bits[0] ^ ID_Bits[1] ^ ID_Bits[2] ^ ID_Bits[4]) & LIN_GET_1ST_BIT_MASK;
        u8_t P1 = ~((ID_Bits[1] ^ ID_Bits[3] ^ ID_Bits[4] ^ ID_Bits[5]) & LIN_GET_1ST_BIT_MASK);

        PID = ID | (P0 << 6) | (P1 << 7);
    }
    return PID;
}

static u8_t Calculate_Checksum(u8_t PID, u8_t *Data, u8_t Size)
{
    u8_t CheckSum = PID;
    u16_t Index;

    for (Index = 0; Index < Size; Index++)
    {
        CheckSum += Data[Index];
    }

    CheckSum = LIN_CHK_SUM_INVERSION_MASK - CheckSum;
    return CheckSum;
}

Error_Status LIN_Assign_DatatoMSGSignal(LIN_Message_t *MSG, u8_t *Values, u8_t Values_Num)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Index;
    if (MSG == NULL || Values == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Values_Num > MSG->Signals_Num)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        for (Index = 0; Index < Values_Num; Index++)
        {
            /*is the user required to know the sequence of the signal?*/
            MSG->Signals[Index]->Value = Values[Index];
        }
    }
    return LOC_Status;
}

Error_Status LIN_Publish_DataToMSG(LIN_Message_t *MSG)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Index;
    u8_t value;
    u8_t Byte_Index;

    if (MSG == NULL || MSG->Data == NULL || MSG->Signals == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = Status_OK;
        for (Index = 0; Index < MSG->Signals_Num; Index++)
        {
            /*
            value = *(MSG->Signals[Index]->Value);

            // Get the desired bit length
            value &= ((1 << MSG->Signals[Index]->Length) - 1);

            // Clear the desired bits in the data array
            *(MSG->Data) &= ~(((1 << MSG->Signals[Index]->Length) - 1) << (8 * (MSG->Data_Length - 1) - MSG->Signals[Index]->Start_Index - MSG->Signals[Index]->Length + 1));

            // Update the data array with the value
            *(MSG->Data) |= (value << (8 * (MSG->Data_Length - 1) - MSG->Signals[Index]->Start_Index - MSG->Signals[Index]->Length + 1));
            */
            Byte_Index = (MSG->Signals[Index]->Start_Index / 8);

            MSG->Data[Byte_Index] = 0;

            MSG->Data[Byte_Index] = *(MSG->Signals[Index]->Value);
        }
    }
    return LOC_Status;
}

Error_Status LIN_Collect_DatafromMSG(LIN_Message_t *MSG)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Index;
    u8_t value;
    u8_t Byte_Index;

    if (MSG == NULL || MSG->Data == NULL || MSG->Signals == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = Status_OK;
        for (Index = 0; Index < MSG->Signals_Num; Index++)
        {
            /*
            value = *(MSG->Data);
            // Extract the relevant bits from the data array
            value >>= (8 * (MSG->Data_Length - 1) - MSG->Signals[Index]->Start_Index - MSG->Signals[Index]->Length + 1);
            value &= ((1 << MSG->Signals[Index]->Length) - 1);

            // Update the variable pointed by Value with the extracted value
            *((u8_t *)MSG->Signals[Index]->Value) = value;
            */
            Byte_Index = (MSG->Signals[Index]->Start_Index / 8);

            MSG->Signals[Index]->Value = MSG->Data[Byte_Index];
        }
    }
    return LOC_Status;
}