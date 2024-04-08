
#include "STD_Types.h"
#include "MCAL/GPIO_DRIVER.h"
#include "MCAL/USART_DRIVER.h"
#include "HAL/LIN_Types.h"
#include "HAL/LIN_MASTER/LIN_Master.h"
#include "HAL/LIN_MASTER/LIN_Master_config.h"

#define LIN_SYNC_BYTE 0x55
#define LIN_TEMP_DATA_SIZE 9
#define LIN_CHK_SUM_INVERSION_MASK 0xFF

typedef struct
{
    u8_t Sync_Byte;
    u8_t PID;
} Header_t;

extern const LIN_SchedTableEntry_t SchedTable[SCHED_TABLE_MESSAGES_NUM];
extern const LIN_Message_t Master_Messages[_MASTER_MSG_NUM];

Header_t LIN_Header;
USART_Req_t Header_Buffer;
USART_Req_t Send_Buffer;
USART_Req_t Recieve_Buffer;
static LIN_Message_t *CurrMSG = NULL;
static u8_t Temp_DataReceive[LIN_TEMP_DATA_SIZE];
static u8_t USART_Peri;

static void LIN_MasterSlave_Task(void);
static void SendData(void);
static Error_Status ReceiveData(void);
static void SendHeader(LIN_Message_t *Message);
static u8_t Calculate_Checksum(u8_t PID, u8_t *Data, u8_t Size);

Error_Status LIN_MasterInit(LIN_cfg_t LIN_CfgArr)
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
    Header_Buffer.CB = LIN_MasterSlave_Task;

    LOC_Status = GPIO_Init(LIN_Pins, 2);
    LOC_Status = USART_Init(USART_LIN_CFG);
    LOC_Status = USART_LIN_Init(LIN_CFG);

    return LOC_Status;
}

void LIN_MasterTask(void)
{
    static u16_t TimeStamp = 0;
    static u16_t MSG_Index = 0;

    if (TimeStamp == 0)
    {
        SendHeader(SchedTable[MSG_Index].Message);
    }

    TimeStamp += MASTER_CALL_PERIOD;

    if (TimeStamp == SchedTable[MSG_Index].TimeSlotMs)
    {
        TimeStamp = 0;
        MSG_Index++;

        if (MSG_Index == SCHED_TABLE_MESSAGES_NUM)
        {
            MSG_Index = 0;
        }
    }
}

static void LIN_MasterSlave_Task(void)
{
    u8_t Index;
    CurrMSG = NULL;

    for (Index = 0; Index < _MASTER_MSG_NUM; Index++)
    {
        if (Master_Messages[Index].PID == LIN_Header.PID)
        {
            CurrMSG = &Master_Messages[Index];
        }

        if (CurrMSG)
        {
            switch (Master_Messages[Index].Relation)
            {
            case Sender:
                SendData();
                break;

            case Receiver:
                Recieve_Buffer.USART_Peri = USART_Peri;
                Recieve_Buffer.buffer = Temp_DataReceive;
                Recieve_Buffer.length = CurrMSG->Data_Length + 1;
                Recieve_Buffer.CB = ReceiveData;
                USART_RXBufferAsyncZC(Recieve_Buffer);
                break;

            default:
                break;
            }
        }
    }
}
static void SendHeader(LIN_Message_t *Message)
{
    LIN_Header.Sync_Byte = LIN_SYNC_BYTE;
    LIN_Header.PID = Message->PID;

    Header_Buffer.USART_Peri = USART_Peri;
    Header_Buffer.buffer = (u8_t *)&LIN_Header;
    Header_Buffer.length = 2;
    Header_Buffer.CB = LIN_MasterSlave_Task;

    USART_GenerateBreak(Header_Buffer.USART_Peri);
    USART_TXBufferAsyncZC(Header_Buffer);
}

void SendData(void)
{
    u8_t CheckSum;

    CheckSum = Calculate_Checksum(CurrMSG->PID, CurrMSG->Data, CurrMSG->Data_Length);

    Send_Buffer.USART_Peri = USART_Peri;
    Send_Buffer.buffer = CurrMSG->Data;
    Send_Buffer.buffer[CurrMSG->Data_Length] = CheckSum;
    Send_Buffer.length = CurrMSG->Data_Length + 1;

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
