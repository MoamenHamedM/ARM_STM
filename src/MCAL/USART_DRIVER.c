

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/USART_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
// #define USART_BUADRATE_MASK 0x0000FFFF
#define USART_PERI_ENABLE 0x000020A0
#define USART_SYNC_TC_FLAG 0x00000040
#define USART_SYNC_RXNE_FLAG 0x00000020
#define USART_TX_ENABLE_FLAG 0x00000008
#define USART_TXEIE_ENABLE_FLAG 0x00000080
#define USART_RX_ENABLE_FLAG 0x00000004
#define USART_RXNEIE_ENABLE_FLAG 0x00000020
#define USART_ASYNC_REQ_STATE_BUSY 0
#define USART_ASYNC_REQ_STATE_READY 1
#define USART_PERI_INDEX_FLAG 0x00000003
#define USART_TX_DONE_IRQ 0x00000080
#define USART_RX_DONE_IRQ 0x00000020
#define USART_1_INDEX 0
#define USART_2_INDEX 1
#define USART_6_INDEX 2

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    volatile u32_t SR;
    volatile u32_t DR;
    volatile u32_t BRR;
    volatile u32_t PUPDR;
    volatile u32_t CR1;
    volatile u32_t CR2;
    volatile u32_t CR3;
    volatile u32_t GTPR;
} USART_Peri_t;

typedef struct
{
    u8_t *data;
    u32_t pos;
    u32_t size;
} buffer_t;

typedef struct
{
    buffer_t buffer;
    u8_t state;
    TXCB CallBack;
} TX_Req_t;

typedef struct
{
    buffer_t buffer;
    u8_t state;
    RXCB CallBack;
} RX_Req_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern const USART_cfg_t USARTs[_USART_Num];
TX_Req_t TX_Request[_USART_Num];
RX_Req_t RX_Request[_USART_Num];

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status USART_Init()
{
    u8_t Index;
    Error_Status LOC_Status = Status_OK;
    f32_t LOC_BRRValue = 0;
    u32_t LOC_CR1Value = 0;
    u32_t LOC_CR2Value = 0;
    u32_t LOC_USART_ADDRESS;

    for (Index = 0; Index < _USART_Num; Index++)
    {
        LOC_BRRValue = USARTs[Index].BaudRate;
        LOC_CR1Value = USART_PERI_ENABLE | USARTs[Index].WordLength | USARTs[Index].OverSampling;
        LOC_CR1Value |= USARTs[Index].ParityControl | USARTs[Index].ParitySelect;
        LOC_CR2Value = USARTs[Index].StopBits;
        LOC_USART_ADDRESS = (u32_t)USARTs[Index].address & ~USART_PERI_INDEX_FLAG;

        ((USART_Peri_t *)LOC_USART_ADDRESS)->BRR = LOC_BRRValue;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 = LOC_CR1Value;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR2 = LOC_CR2Value;
    }

    return LOC_Status;
}

Error_Status USART_SendByte(void *USART_Peri, u8_t byte)
{
    Error_Status LOC_Status = Status_NOK;
    u16_t TimeOut = 60000;
    u32_t LOC_USART_ADDRESS;

    if (USART_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_USART_ADDRESS = (u32_t)USART_Peri & ~USART_PERI_INDEX_FLAG;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 |= USART_RX_ENABLE_FLAG;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 &= ~USART_TXEIE_ENABLE_FLAG;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->DR = byte;

        while (!(((USART_Peri_t *)LOC_USART_ADDRESS)->SR & USART_SYNC_TC_FLAG) && TimeOut)
        {
            TimeOut--;
        }

        if (!TimeOut)
        {
            LOC_Status = Status_USART_TimeOut;
        }

        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 &= ~USART_TX_ENABLE_FLAG;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 |= USART_TXEIE_ENABLE_FLAG;
    }
    return LOC_Status;
}

Error_Status USART_GetByte(void *USART_Peri, u8_t *byte)
{
    Error_Status LOC_Status = Status_NOK;
    u16_t TimeOut = 60000;
    u32_t LOC_USART_ADDRESS;

    if (USART_Peri == NULL || byte == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_USART_ADDRESS = (u32_t)USART_Peri & ~USART_PERI_INDEX_FLAG;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 |= USART_RX_ENABLE_FLAG;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 &= ~USART_RXNEIE_ENABLE_FLAG;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->DR = byte;
        while (!(((USART_Peri_t *)LOC_USART_ADDRESS)->SR & USART_SYNC_RXNE_FLAG) && TimeOut)
        {
            TimeOut--;
        }

        if (!TimeOut)
        {
            LOC_Status = Status_USART_TimeOut;
        }
        else
        {
            *byte = ((USART_Peri_t *)LOC_USART_ADDRESS)->DR;
        }

        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 &= ~USART_RX_ENABLE_FLAG;
        ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 |= USART_RXNEIE_ENABLE_FLAG;
    }
    return LOC_Status;
}

Error_Status USART_TXBufferAsync(void *USART_Peri, u8_t *buffer, u16_t length, TXCB CB)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_USART_ADDRESS;
    u8_t LOC_Peri_Index;

    if (USART_Peri == NULL || buffer == NULL || CB == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_USART_ADDRESS = (u32_t)USART_Peri & ~USART_PERI_INDEX_FLAG;
        LOC_Peri_Index = (u32_t)USART_Peri & USART_PERI_INDEX_FLAG;

        if (TX_Request[LOC_Peri_Index].state == USART_ASYNC_REQ_STATE_BUSY)
        {
            LOC_Status = Status_USART_Busy_Buffer;
        }

        else
        {
            TX_Request[LOC_Peri_Index].buffer.data = buffer;
            TX_Request[LOC_Peri_Index].buffer.size = length;
            TX_Request[LOC_Peri_Index].buffer.pos = 0;
            TX_Request[LOC_Peri_Index].CallBack = CB;
            TX_Request[LOC_Peri_Index].state = USART_ASYNC_REQ_STATE_BUSY;

            ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 |= USART_TX_ENABLE_FLAG;
            ((USART_Peri_t *)LOC_USART_ADDRESS)->DR = TX_Request[LOC_Peri_Index].buffer.data[0];
            TX_Request[LOC_Peri_Index].buffer.pos++;
        }
    }
    return LOC_Status;
}

Error_Status USART_RXBufferAsync(void *USART_Peri, u8_t *buffer, u16_t length, RXCB CB)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_USART_ADDRESS;
    u8_t LOC_Peri_Index;

    if (USART_Peri == NULL || buffer == NULL || CB == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_USART_ADDRESS = (u32_t)USART_Peri & ~USART_PERI_INDEX_FLAG;
        LOC_Peri_Index = (u32_t)USART_Peri & USART_PERI_INDEX_FLAG;

        if (RX_Request[LOC_Peri_Index].state == USART_ASYNC_REQ_STATE_BUSY)
        {
            LOC_Status = Status_USART_Busy_Buffer;
        }

        else
        {
            RX_Request[LOC_Peri_Index].buffer.data = buffer;
            RX_Request[LOC_Peri_Index].buffer.size = length;
            RX_Request[LOC_Peri_Index].buffer.pos = 0;
            RX_Request[LOC_Peri_Index].CallBack = CB;
            RX_Request[LOC_Peri_Index].state = USART_ASYNC_REQ_STATE_BUSY;

            ((USART_Peri_t *)LOC_USART_ADDRESS)->CR1 |= USART_RX_ENABLE_FLAG;
        }
    }
    return LOC_Status;
}

void USART1_IRQHandler(void)
{
    if (((USART_Peri_t *)USART_Peri_1)->SR & USART_TX_DONE_IRQ)
    {
        if (TX_Request[USART_1_INDEX].buffer.pos < TX_Request[USART_1_INDEX].buffer.size)
        {
            ((USART_Peri_t *)USART_Peri_1)->DR = TX_Request[USART_1_INDEX].buffer.data[TX_Request[USART_1_INDEX].buffer.pos];
            TX_Request[USART_1_INDEX].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)USART_Peri_1)->CR1 &= ~USART_TX_ENABLE_FLAG;
            TX_Request[USART_1_INDEX].state = USART_ASYNC_REQ_STATE_READY;
            if (TX_Request[USART_1_INDEX].CallBack)
            {
                TX_Request[USART_1_INDEX].CallBack();
            }
        }
    }

    if (((USART_Peri_t *)USART_Peri_1)->SR & USART_RX_DONE_IRQ)
    {
        if (RX_Request[USART_1_INDEX].buffer.pos < RX_Request[USART_1_INDEX].buffer.size)
        {
            RX_Request[USART_1_INDEX].buffer.data[RX_Request[USART_1_INDEX].buffer.pos] = ((USART_Peri_t *)((u32_t)USART_Peri_1))->DR;
            RX_Request[USART_1_INDEX].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)USART_Peri_1)->CR1 &= ~USART_RX_ENABLE_FLAG;
            RX_Request[USART_1_INDEX].state = USART_ASYNC_REQ_STATE_READY;
            if (RX_Request[USART_1_INDEX].CallBack)
            {
                RX_Request[USART_1_INDEX].CallBack();
            }
        }
    }
}

void USART2_IRQHandler(void)
{
    if (((USART_Peri_t *)((u32_t)USART_Peri_2 & USART_PERI_INDEX_FLAG))->SR & USART_TX_DONE_IRQ)
    {
        if (TX_Request[USART_2_INDEX].buffer.pos < TX_Request[USART_2_INDEX].buffer.size)
        {
            ((USART_Peri_t *)((u32_t)USART_Peri_2 & USART_PERI_INDEX_FLAG))->DR = TX_Request[USART_2_INDEX].buffer.data[TX_Request[USART_2_INDEX].buffer.pos];
            TX_Request[USART_2_INDEX].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)((u32_t)USART_Peri_2 & USART_PERI_INDEX_FLAG))->CR1 &= ~USART_TX_ENABLE_FLAG;
            TX_Request[USART_2_INDEX].state = USART_ASYNC_REQ_STATE_READY;
            if (TX_Request[USART_2_INDEX].CallBack)
            {
                TX_Request[USART_2_INDEX].CallBack();
            }
        }
    }

    if (((USART_Peri_t *)((u32_t)USART_Peri_2 & USART_PERI_INDEX_FLAG))->SR & USART_RX_DONE_IRQ)
    {
        if (RX_Request[USART_2_INDEX].buffer.pos < RX_Request[USART_2_INDEX].buffer.size)
        {
            RX_Request[USART_2_INDEX].buffer.data[RX_Request[USART_2_INDEX].buffer.pos] = ((USART_Peri_t *)((u32_t)USART_Peri_2 & USART_PERI_INDEX_FLAG))->DR;
            RX_Request[USART_2_INDEX].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)((u32_t)USART_Peri_2 & USART_PERI_INDEX_FLAG))->CR1 &= ~USART_RX_ENABLE_FLAG;
            RX_Request[USART_2_INDEX].state = USART_ASYNC_REQ_STATE_READY;
            if (RX_Request[USART_2_INDEX].CallBack)
            {
                RX_Request[USART_2_INDEX].CallBack();
            }
        }
    }
}

void USART6_IRQHandler(void)
{
    if (((USART_Peri_t *)((u32_t)USART_Peri_6 & USART_PERI_INDEX_FLAG))->SR & USART_TX_DONE_IRQ)
    {
        if (TX_Request[USART_6_INDEX].buffer.pos < TX_Request[USART_6_INDEX].buffer.size)
        {
            ((USART_Peri_t *)((u32_t)USART_Peri_6 & USART_PERI_INDEX_FLAG))->DR = TX_Request[USART_6_INDEX].buffer.data[TX_Request[USART_6_INDEX].buffer.pos];
            TX_Request[USART_6_INDEX].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)((u32_t)USART_Peri_6 & USART_PERI_INDEX_FLAG))->CR1 &= ~USART_TX_ENABLE_FLAG;
            TX_Request[USART_6_INDEX].state = USART_ASYNC_REQ_STATE_READY;
            if (TX_Request[USART_6_INDEX].CallBack)
            {
                TX_Request[USART_6_INDEX].CallBack();
            }
        }
    }

    if (((USART_Peri_t *)((u32_t)USART_Peri_6 & USART_PERI_INDEX_FLAG))->SR & USART_RX_DONE_IRQ)
    {
        if (RX_Request[USART_6_INDEX].buffer.pos < RX_Request[USART_6_INDEX].buffer.size)
        {
            RX_Request[USART_6_INDEX].buffer.data[RX_Request[USART_6_INDEX].buffer.pos] = ((USART_Peri_t *)((u32_t)USART_Peri_6 & USART_PERI_INDEX_FLAG))->DR;
            RX_Request[USART_6_INDEX].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)((u32_t)USART_Peri_6 & USART_PERI_INDEX_FLAG))->CR1 &= ~USART_RX_ENABLE_FLAG;
            RX_Request[USART_6_INDEX].state = USART_ASYNC_REQ_STATE_READY;
            if (RX_Request[USART_6_INDEX].CallBack)
            {
                RX_Request[USART_6_INDEX].CallBack();
            }
        }
    }
}