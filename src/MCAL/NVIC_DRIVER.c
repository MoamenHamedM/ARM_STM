/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "MCAL/NVIC_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define NVIC_BASE_ADDRESS 0xE000E100
#define NVIC_REG_THRESHOLD 32
#define NVIC_PRI_REG_THRESHOLD 4

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    volatile u32_t NVIC_ISER[8];
    volatile u32_t Reserved1[24];
    volatile u32_t NVIC_ICER[8];
    volatile u32_t Reserved2[24];
    volatile u32_t NVIC_ISPR[8];
    volatile u32_t Reserved3[24];
    volatile u32_t NVIC_ICPR[8];
    volatile u32_t Reserved4[24];
    volatile u32_t NVIC_IABR[8];
    volatile u32_t Reserved5[56];
    volatile u32_t NVIC_IPR[60];
    volatile u32_t Reserved6[580];
    volatile u32_t NVIC_STIR;
} NVIC_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
NVIC_t *const NVIC = (NVIC_t *)NVIC_BASE_ADDRESS;
u32_t IRQ_BIT_Position[32] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80,
    0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
    0x10000, 0x20000, 0x40000, 0x80000, 0x100000, 0x200000, 0x400000, 0x800000,
    0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000};
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status NVIC_CTRL_EnableIRQ(IRQ_ID_t NVIC_IQR)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IQR / NVIC_REG_THRESHOLD;

    if (NVIC_IQR >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->NVIC_ISER[REG_Index] = IRQ_BIT_Position[(NVIC_IQR - (REG_Index * NVIC_REG_THRESHOLD))];
    }
    return LOC_Status;
}

Error_Status NVIC_CTRL_DisableIRQ(IRQ_ID_t NVIC_IQR)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IQR / NVIC_REG_THRESHOLD;

    if (NVIC_IQR >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->NVIC_ICER[REG_Index] = IRQ_BIT_Position[(NVIC_IQR - (REG_Index * NVIC_REG_THRESHOLD))];
    }
    return LOC_Status;
}

Error_Status NVIC_CTRL_SetIRQPending(IRQ_ID_t NVIC_IQR)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IQR / NVIC_REG_THRESHOLD;

    if (NVIC_IQR >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->NVIC_ISPR[REG_Index] = IRQ_BIT_Position[(NVIC_IQR - (REG_Index * NVIC_REG_THRESHOLD))];
    }
    return LOC_Status;
}

Error_Status NVIC_CTRL_SetIRQPending(IRQ_ID_t NVIC_IQR)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IQR / NVIC_REG_THRESHOLD;

    if (NVIC_IQR >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->NVIC_ICPR[REG_Index] = IRQ_BIT_Position[(NVIC_IQR - (REG_Index * NVIC_REG_THRESHOLD))];
    }
    return LOC_Status;
}

Error_Status NVIC_CTRL_GenerateSWI(IRQ_ID_t NVIC_IQR)
{
    Error_Status LOC_Status = Status_NOK;

    if (NVIC_IQR >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        NVIC->NVIC_STIR = NVIC_IQR;
    }
    return LOC_Status;
}

Error_Status NVIC_Get_ActiveStatus(IRQ_ID_t NVIC_IQR, u32_t *Status)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t REG_Index = NVIC_IQR / NVIC_REG_THRESHOLD;

    if (NVIC_IQR >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (Status == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = Status_OK;
        *Status = (NVIC->NVIC_IABR[REG_Index] >> (NVIC_IQR - (REG_Index * NVIC_REG_THRESHOLD))) & 1;
    }
    return LOC_Status;
}

Error_Status NVIC_CFG_SetPriority(IRQ_ID_t NVIC_IQR, u8_t PreemptPri, u8_t SubGroupPri)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t REG_Index = NVIC_IQR / NVIC_PRI_REG_THRESHOLD;
    u8_t Priority_Index = NVIC_IQR % NVIC_PRI_REG_THRESHOLD;
    u8_t LOC_TempPRiority = 0;

    if (NVIC_IQR >= _NVIC_IRQ_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempPRiority = (PreemptPri << NVIC_SUBGROUB_NUM) | SubGroupPri;
        NVIC->NVIC_IPR[REG_Index] = LOC_TempPRiority << Priority_Index;
    }
    return LOC_Status;
}