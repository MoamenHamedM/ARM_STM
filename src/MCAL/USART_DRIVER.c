

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/USART_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
// #define USART_BUADRATE_MASK 0x0000FFFF
#define USART_PERI_ENABLE 0x00002000
#define USART_SYNC_TXE_FLAG 0x00000080
#define USART_SYNC_RXNE_FLAG 0x00000020
#define USART_TX_ENABLE_FLAG 0x00000008
#define USART_TXEIE_ENABLE_FLAG 0x00000080
#define USART_RX_ENABLE_FLAG 0x00000004
#define USART_RXNEIE_ENABLE_FLAG 0x00000020
#define USART_REQ_STATE_BUSY 1
#define USART_REQ_STATE_READY 0
#define USART_PERI_INDEX_FLAG 0x00000003
#define USART_TX_DONE_IRQ 0x00000080
#define USART_RX_DONE_IRQ 0x00000020
#define USART_REG_NUM 3
#define USART_FRACTION_OVERFLOW_LIMIT 0x10
#define USART_4_BIT_OFFSET 4
#define USART_FIRST_BYTE_MASK 0x0F
#define USART_SECOND_BYTE_MASK 0xF0
#define USART_SR_CLEAR_MASK 0xFFFFFFFF

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    volatile u32_t SR;
    volatile u32_t DR;
    volatile u32_t BRR;
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
    CallBack_t CallBack;
} TX_Req_t;

typedef struct
{
    buffer_t buffer;
    u8_t state;
    CallBack_t CallBack;
} RX_Req_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern const USART_cfg_t USART_CFG[_USART_Num];
TX_Req_t TX_Request[_USART_Num];
RX_Req_t RX_Request[_USART_Num];
void *USART_ADD[USART_REG_NUM] = {(void *)0x40011000, (void *)0x40004400, (void *)0x40011400};

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status
USART_Init()
{
    u8_t Index;
    Error_Status LOC_Status = Status_OK;
    f32_t LOC_BRRValue = 0;
    u32_t LOC_CR1Value = 0;
    u32_t LOC_CR2Value = 0;
    u16_t LOC_Mantissa;
    u16_t LOC_Fraction;
    u32_t LOC_USARTDIV;
    u32_t LOC_OverSampling;

    for (Index = 0; Index < _USART_Num; Index++)
    {
        // get the over sampling status 1 or 0 using the USART_OVERSAMPLING_8 value
        LOC_OverSampling = USART_CFG[Index].OverSampling / USART_OVERSAMPLING_8;
        // calculate the USARTDIV value and multiply by 1000 to get the fraction as integer
        LOC_USARTDIV = (((u64_t)USART_CLK * 1000) / (USART_CFG[Index].BaudRate * (8 * (2 - LOC_OverSampling))));

        LOC_Mantissa = LOC_USARTDIV / 1000;
        LOC_Fraction = (LOC_USARTDIV % 1000) * (8 * (2 - LOC_OverSampling));

        // Check if the fraction part needs rounding
        if (LOC_Fraction % 1000 >= 500)
        {
            // Round up the fraction part
            LOC_Fraction = (LOC_Fraction / 1000) + 1;
        }
        else
        {
            LOC_Fraction = LOC_Fraction / 1000;
        }
        // check if there is any carry from the fraction
        if (LOC_Fraction >= USART_FRACTION_OVERFLOW_LIMIT)
        {
            LOC_Mantissa += LOC_Fraction & USART_SECOND_BYTE_MASK;
        }

        LOC_BRRValue = (LOC_Mantissa << USART_4_BIT_OFFSET) | (LOC_Fraction & USART_FIRST_BYTE_MASK);
        LOC_CR1Value = USART_PERI_ENABLE | USART_CFG[Index].WordLength | USART_CFG[Index].OverSampling;
        LOC_CR1Value |= USART_CFG[Index].ParityControl | USART_CFG[Index].ParitySelect;
        LOC_CR2Value = USART_CFG[Index].StopBits;

        ((USART_Peri_t *)USART_ADD[USART_CFG[Index].address])->BRR = LOC_BRRValue;
        ((USART_Peri_t *)USART_ADD[USART_CFG[Index].address])->CR1 = LOC_CR1Value;
        ((USART_Peri_t *)USART_ADD[USART_CFG[Index].address])->CR2 = LOC_CR2Value;
    }

    return LOC_Status;
}

Error_Status USART_SendByte(USART_Req_t USART_Req)
{
    Error_Status LOC_Status = Status_NOK;
    volatile u16_t TimeOut = 3000;

    if (USART_Req.USART_Peri > USART_Peri_6)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (USART_Req.length > 1)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (TX_Request[USART_Req.USART_Peri].state == USART_REQ_STATE_BUSY)
    {
        LOC_Status = Status_USART_Busy_Buffer;
    }
    else
    {
        LOC_Status = Status_OK;
        TX_Request[USART_Req.USART_Peri].state = USART_REQ_STATE_BUSY;
        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->DR = *(USART_Req.buffer);
        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->CR1 |= USART_TX_ENABLE_FLAG;

        // while (!(((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->SR & USART_SYNC_TXE_FLAG) && TimeOut)
        while (TimeOut)
        {
            TimeOut--;
        }

      /* if (!TimeOut)
        {
            LOC_Status = Status_USART_TimeOut;
        }*/

        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->CR1 &= ~USART_TX_ENABLE_FLAG;
        TX_Request[USART_Req.USART_Peri].state = USART_REQ_STATE_READY;
    }
    return LOC_Status;
}

Error_Status USART_GetByte(USART_Req_t USART_Req)
{
    Error_Status LOC_Status = Status_NOK;
    u16_t TimeOut = 60000;

    if (USART_Req.buffer == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (USART_Req.USART_Peri > USART_Peri_6 || USART_Req.length > 1)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (RX_Request[USART_Req.USART_Peri].state == USART_REQ_STATE_BUSY)
    {
        LOC_Status = Status_USART_Busy_Buffer;
    }
    {
        LOC_Status = Status_OK;
        RX_Request[USART_Req.USART_Peri].state = USART_REQ_STATE_BUSY;
        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->CR1 |= USART_RX_ENABLE_FLAG;
        while (!(((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->SR & USART_SYNC_RXNE_FLAG) && TimeOut)
        {
            TimeOut--;
        }

        if (!TimeOut)
        {
            LOC_Status = Status_USART_TimeOut;
        }
        else
        {
            *(USART_Req.buffer) = ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->DR;
        }

        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->CR1 &= ~USART_RX_ENABLE_FLAG;
        RX_Request[USART_Req.USART_Peri].state = USART_REQ_STATE_BUSY;
    }
    return LOC_Status;
}

Error_Status USART_TXBufferAsyncZC(USART_Req_t USART_Req)
{

    Error_Status LOC_Status = Status_NOK;

    if (USART_Req.buffer == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (TX_Request[USART_Req.USART_Peri].state == USART_REQ_STATE_BUSY)
    {
        LOC_Status = Status_USART_Busy_Buffer;
    }
    else if (USART_Req.USART_Peri > USART_Peri_6)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        TX_Request[USART_Req.USART_Peri].buffer.data = USART_Req.buffer;
        TX_Request[USART_Req.USART_Peri].buffer.size = USART_Req.length;
        TX_Request[USART_Req.USART_Peri].buffer.pos = 0;
        TX_Request[USART_Req.USART_Peri].CallBack = USART_Req.CB;
        TX_Request[USART_Req.USART_Peri].state = USART_REQ_STATE_BUSY;

        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->CR1 |= USART_TX_ENABLE_FLAG;
        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->CR1 |= USART_TXEIE_ENABLE_FLAG;
        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->DR = TX_Request[USART_Req.USART_Peri].buffer.data[0];
        TX_Request[USART_Req.USART_Peri].buffer.pos++;
    }

    return LOC_Status;
}

Error_Status USART_RXBufferAsyncZC(USART_Req_t USART_Req)
{
    Error_Status LOC_Status = Status_NOK;

    if (USART_Req.buffer == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (TX_Request[USART_Req.USART_Peri].state == USART_REQ_STATE_BUSY)
    {
        LOC_Status = Status_USART_Busy_Buffer;
    }
    else if (USART_Req.USART_Peri > USART_Peri_6)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        RX_Request[USART_Req.USART_Peri].buffer.data = USART_Req.buffer;
        RX_Request[USART_Req.USART_Peri].buffer.size = USART_Req.length;
        RX_Request[USART_Req.USART_Peri].buffer.pos = 0;
        RX_Request[USART_Req.USART_Peri].CallBack = USART_Req.CB;
        RX_Request[USART_Req.USART_Peri].state = USART_REQ_STATE_BUSY;

        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->CR1 |= USART_RX_ENABLE_FLAG;
        ((USART_Peri_t *)USART_ADD[USART_Req.USART_Peri])->CR1 |= USART_RXNEIE_ENABLE_FLAG;
    }
    return LOC_Status;
}

void USART1_IRQHandler(void)
{
    if (((USART_Peri_t *)USART_ADD[USART_Peri_1])->SR & USART_TX_DONE_IRQ)
    {
        if (TX_Request[USART_Peri_1].buffer.pos <= TX_Request[USART_Peri_1].buffer.size)
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_1])->DR = TX_Request[USART_Peri_1].buffer.data[TX_Request[USART_Peri_1].buffer.pos];
            TX_Request[USART_Peri_1].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_1])->CR1 &= ~USART_TXEIE_ENABLE_FLAG;
            ((USART_Peri_t *)USART_ADD[USART_Peri_1])->CR1 &= ~USART_TX_ENABLE_FLAG;
            TX_Request[USART_Peri_1].state = USART_REQ_STATE_READY;
            if (TX_Request[USART_Peri_1].CallBack)
            {
                TX_Request[USART_Peri_1].CallBack();
            }
        }
    }

    if (((USART_Peri_t *)USART_ADD[USART_Peri_1])->SR & USART_RX_DONE_IRQ)
    {
        if (RX_Request[USART_Peri_1].buffer.pos < RX_Request[USART_Peri_1].buffer.size)
        {
            RX_Request[USART_Peri_1].buffer.data[RX_Request[USART_Peri_1].buffer.pos] = ((USART_Peri_t *)USART_ADD[USART_Peri_1])->DR;
            RX_Request[USART_Peri_1].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_1])->CR1 &= ~USART_RXNEIE_ENABLE_FLAG;
            ((USART_Peri_t *)USART_ADD[USART_Peri_1])->CR1 &= ~USART_RX_ENABLE_FLAG;
            RX_Request[USART_Peri_1].state = USART_REQ_STATE_READY;
            if (RX_Request[USART_Peri_1].CallBack)
            {
                RX_Request[USART_Peri_1].CallBack();
            }
        }
    }
}

void USART2_IRQHandler(void)
{
    if (((USART_Peri_t *)USART_ADD[USART_Peri_2])->SR & USART_TX_DONE_IRQ)
    {
        if (TX_Request[USART_Peri_2].buffer.pos <= TX_Request[USART_Peri_2].buffer.size)
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_2])->DR = TX_Request[USART_Peri_2].buffer.data[TX_Request[USART_Peri_2].buffer.pos];
            TX_Request[USART_Peri_2].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_2])->CR1 &= ~USART_TXEIE_ENABLE_FLAG;
            ((USART_Peri_t *)USART_ADD[USART_Peri_2])->CR1 &= ~USART_TX_ENABLE_FLAG;
            TX_Request[USART_Peri_2].state = USART_REQ_STATE_READY;
            if (TX_Request[USART_Peri_2].CallBack)
            {
                TX_Request[USART_Peri_2].CallBack();
            }
        }
    }

    if (((USART_Peri_t *)USART_ADD[USART_Peri_2])->SR & USART_RX_DONE_IRQ)
    {
        if (RX_Request[USART_Peri_2].buffer.pos < RX_Request[USART_Peri_2].buffer.size)
        {
            RX_Request[USART_Peri_2].buffer.data[RX_Request[USART_Peri_2].buffer.pos] = ((USART_Peri_t *)USART_ADD[USART_Peri_2])->DR;
            RX_Request[USART_Peri_2].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_2])->CR1 &= ~USART_RXNEIE_ENABLE_FLAG;
            ((USART_Peri_t *)USART_ADD[USART_Peri_2])->CR1 &= ~USART_RX_ENABLE_FLAG;
            RX_Request[USART_Peri_2].state = USART_REQ_STATE_READY;
            if (RX_Request[USART_Peri_2].CallBack)
            {
                RX_Request[USART_Peri_2].CallBack();
            }
        }
    }
}

void USART6_IRQHandler(void)
{
    if (((USART_Peri_t *)USART_ADD[USART_Peri_6])->SR & USART_TX_DONE_IRQ)
    {
        if (TX_Request[USART_Peri_6].buffer.pos <= TX_Request[USART_Peri_6].buffer.size)
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_6])->DR = TX_Request[USART_Peri_6].buffer.data[TX_Request[USART_Peri_6].buffer.pos];
            TX_Request[USART_Peri_6].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_6])->CR1 &= ~USART_TXEIE_ENABLE_FLAG;
            ((USART_Peri_t *)USART_ADD[USART_Peri_6])->CR1 &= ~USART_TX_ENABLE_FLAG;
            TX_Request[USART_Peri_6].state = USART_REQ_STATE_READY;
            if (TX_Request[USART_Peri_6].CallBack)
            {
                TX_Request[USART_Peri_6].CallBack();
            }
        }
    }

    if (((USART_Peri_t *)USART_ADD[USART_Peri_6])->SR & USART_RX_DONE_IRQ)
    {
        if (RX_Request[USART_Peri_6].buffer.pos < RX_Request[USART_Peri_6].buffer.size)
        {
            RX_Request[USART_Peri_6].buffer.data[RX_Request[USART_Peri_6].buffer.pos] = ((USART_Peri_t *)USART_ADD[USART_Peri_6])->DR;
            RX_Request[USART_Peri_6].buffer.pos++;
        }
        else
        {
            ((USART_Peri_t *)USART_ADD[USART_Peri_6])->CR1 &= ~USART_RXNEIE_ENABLE_FLAG;
            ((USART_Peri_t *)USART_ADD[USART_Peri_6])->CR1 &= ~USART_RX_ENABLE_FLAG;
            RX_Request[USART_Peri_6].state = USART_REQ_STATE_READY;
            if (RX_Request[USART_Peri_6].CallBack)
            {
                RX_Request[USART_Peri_6].CallBack();
            }
        }
    }
}