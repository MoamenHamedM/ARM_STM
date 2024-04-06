/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "MCAL/DMA_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define DMA_INPUT_ASSERT_OFFSET 32
#define DMA_MAX_STRM_NUM 7
#define DMA_CRTL_STR_EN_DIS_ASSERT_MASK 0x40
#define DMA_GET_INT_STS_CLR_ASSERT_L_MASK 0x01
#define DMA_GET_INT_STS_CLR_ASSERT_H_MASK 0x02
#define DMA_INT_EN_DIS_ASSERT_L_MASK 0x20
#define DMA_INT_EN_DIS_ASSERT_H_MASK 0x30
#define DMA_CHN_SEL_ASSERT_MASK 0x03
#define DMA_MEM_B_ASSERT_MASK 0x04
#define DMA_PERI_B_ASSERT_MASK 0x05
#define DMA_CT_ASSERT_MASK 0x06
#define DMA_DB_ASSERT_MASK 0x07
#define DMA_PRIORITY_ASSERT_MASK 0x08
#define DMA_PINCOS_ASSERT_MASK 0x09
#define DMA_MEM_SIZE_ASSERT_MASK 0x0A
#define DMA_PERI_SIZE_ASSERT_MASK 0x0B
#define DMA_MEM_INCR_ASSERT_MASK 0x0C
#define DMA_PERI_INCR_ASSERT_MASK 0x0D
#define DMA_CIRC_ASSERT_MASK 0x0E
#define DMA_TRANS_DIR_ASSERT_MASK 0x0F
#define DMA_PERO_FLOW_ASSERT_MASK 0x10
#define DMA_DIR_MODE_ASSERT_MASK 0x50
#define DMA_FIFO_THRES_ASSERT_MASK 0x60

#define DMA_STREMSTATE_CLEAR_FLAG 0x00000001
#define DMA_INT_STATE_CLEAR_FLAG 0x00000001
#define DMA_CHN_SEL_CLEARFLAG 0x0E000000
#define DMA_MEM_B_CLEARFLAG 0x03000000
#define DMA_PERI_B_CLEARFLAG 0x00600000
#define DMA_CT_CLEAR_FLAG 0x00080000
#define DMA_DB_CLEAR_FLAG 0x00040000
#define DMA_PRIORITY_CLEAR_FLAG 0x00030000
#define DMA_PINCOS_CLEAR_FLAG 0x00008000
#define DMA_MEM_SIZE_CLEAR_FLAG 0x00006000
#define DMA_PERI_SIZE_CLEAR_FLAG 0x00001800
#define DMA_MEM_INCR_CLEAR_FLAG 0x00000400
#define DMA_PERI_INCR_CLEAR_FLAG 0x00000200
#define DMA_CIRC_CLEAR_FLAG 0x00000100
#define DMA_TRANS_DIR_CLEAR_FLAG 0x000000C0
#define DMA_PERI_FLOW_CLEAR_FLAG 0x00000020
#define DMA_DIR_MODE_CLEAR_FLAG 0x00000002
#define DMA_FIFO_THRES_CLEAR_FLAG 0x00000003

#define DMA_CT_GET_FLAG 0x00080000
#define DMA_FIFO_GET_FLAG 0x00000038
#define DMA_IRQ_GET_1ST_BYTE_MASK 0x00FF
#define DMA_IRQ_GET_2ND_BYTE_MASK 0xFF00
#define DMA_IRQ_GET_LOW_NIB_MASK 0x00F
#define DMA_2_GET_MASK 0x400
#define DMA_4_BIT_OFFSET_MASK 4
#define DMA_5_BIT_OFFSET_MASK 5
#define DMA_10_BIT_OFFSET_MASK 10
#define DMA_HIGH_REG 1
#define DMA_LOW_REG 0
#define DMA_MAX_IRQ_IDX 28
#define DMA_LISR_THRES 3

#define DMA_STRM_0_IRQ 0x00
#define DMA_STRM_1_IRQ 0x61
#define DMA_STRM_2_IRQ 0x102
#define DMA_STRM_3_IRQ 0x163
#define DMA_STRM_4_IRQ 0x04
#define DMA_STRM_5_IRQ 0x65
#define DMA_STRM_6_IRQ 0x106
#define DMA_STRM_7_IRQ 0x167

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct
{
    volatile u32_t CR;
    volatile u32_t NDTR;
    volatile u32_t PAR;
    volatile u32_t M0AR;
    volatile u32_t M1AR;
    volatile u32_t FCR;
} DMA_STRM_REG_t;

typedef struct
{
    volatile u32_t LISR;
    volatile u32_t HISR;
    volatile u32_t LIFCR;
    volatile u32_t HIFCR;
    DMA_STRM_REG_t DMA_STRM_REG[7];
} DMA_Peri_t;
/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
Handler_t IRQ_CallBackFunction[2][56];
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void General_IRQ_Handler(void *DMA_Peri, u16_t STRM_Num);
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status DMA_CTRL_StreamEnable_Disable(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_StreamState)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_StreamState >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_CRTL_STR_EN_DIS_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_STREMSTATE_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_StreamState;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_GET_InterruptStatus(void *DMA_Peri, u64_t DMA_Strm, u32_t *Status)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = (DMA_Strm >> DMA_INPUT_ASSERT_OFFSET) & DMA_IRQ_GET_1ST_BYTE_MASK;

    if (DMA_Peri == NULL || Status == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (!Input_Assert || Input_Assert > DMA_GET_INT_STS_CLR_ASSERT_H_MASK)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        switch (Input_Assert)
        {
        case DMA_GET_INT_STS_CLR_ASSERT_L_MASK:
            *Status = ((DMA_Peri_t *)DMA_Peri)->LISR & (u32_t)DMA_Strm;
            break;

        case DMA_GET_INT_STS_CLR_ASSERT_H_MASK:
            *Status = ((DMA_Peri_t *)DMA_Peri)->HISR & (u32_t)DMA_Strm;
            break;

        default:
            break;
        }
    }
    return LOC_Status;
}

Error_Status DMA_CTRL_ClearInterruptFlag(void *DMA_Peri, u64_t DMA_Strm)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = (DMA_Strm >> DMA_INPUT_ASSERT_OFFSET) & DMA_IRQ_GET_1ST_BYTE_MASK;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (!Input_Assert || Input_Assert > DMA_GET_INT_STS_CLR_ASSERT_H_MASK)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        switch (Input_Assert)
        {
        case DMA_GET_INT_STS_CLR_ASSERT_L_MASK:
            ((DMA_Peri_t *)DMA_Peri)->LIFCR = (u32_t)DMA_Strm;
            break;

        case DMA_GET_INT_STS_CLR_ASSERT_H_MASK:
            ((DMA_Peri_t *)DMA_Peri)->HIFCR = (u32_t)DMA_Strm;
            break;

        default:
            break;
        }
    }
    return LOC_Status;
}

Error_Status DMA_CTRL_InterruptEnable_Disable(void *DMA_Peri, u64_t DMA_Peri_F, u8_t STRM_Num, u8_t State)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Peri_F >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_INT_EN_DIS_ASSERT_L_MASK && Input_Assert != DMA_INT_EN_DIS_ASSERT_H_MASK)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else if (STRM_Num > DMA_MAX_STRM_NUM || State > DMA_INT_STATE_ENABLE)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        switch (Input_Assert)
        {
        case DMA_INT_EN_DIS_ASSERT_L_MASK:
            LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;
            LOC_TempReg &= ~(DMA_INT_STATE_CLEAR_FLAG << (u32_t)DMA_Peri_F);
            LOC_TempReg |= State << (u32_t)DMA_Peri_F;
            ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
            break;

        case DMA_INT_EN_DIS_ASSERT_H_MASK:
            LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].FCR;
            LOC_TempReg &= ~(DMA_INT_STATE_CLEAR_FLAG << (u32_t)DMA_Peri_F);
            LOC_TempReg |= State << (u32_t)DMA_Peri_F;
            ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].FCR = LOC_TempReg;
            break;

        default:
            break;
        }
    }
    return LOC_Status;
}

Error_Status DMA_SEL_Channel(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Chn)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Chn >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_CHN_SEL_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_CHN_SEL_CLEARFLAG;
        LOC_TempReg |= (u32_t)DMA_Chn;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CTRL_MemoryBurst(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_B)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Mem_B >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_MEM_B_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_MEM_B_CLEARFLAG;
        LOC_TempReg |= (u32_t)DMA_Mem_B;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_PeripheralBurst(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_B)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Peri_B >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_PERI_B_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_PERI_B_CLEARFLAG;
        LOC_TempReg |= (u32_t)DMA_Peri_B;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_SEL_CurrentTarget(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_CT)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_CT >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_CT_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_CT_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_CT;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_GET_CurrentTarget(void *DMA_Peri, u8_t STRM_Num, u32_t *DMA_CT)
{
    Error_Status LOC_Status = Status_NOK;

    if (DMA_Peri == NULL || DMA_CT == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        *DMA_CT = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR & DMA_CT_GET_FLAG;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_DoubleBuffer(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_DB)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_DB >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_DB_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_DB_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_DB;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_PriorityLevel(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Pri)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Pri >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_PRIORITY_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_PRIORITY_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Pri;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_PeriIncr_OffsetSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Pincos)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Pincos >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_PINCOS_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_PINCOS_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Pincos;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_MemoryDataSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_S)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Mem_S >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_MEM_SIZE_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_MEM_SIZE_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Mem_S;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_PeripheralDataSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_S)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Peri_S >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_PERI_SIZE_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_PERI_SIZE_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Peri_S;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_MemoryIncrement(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_Incr)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Mem_Incr >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_MEM_INCR_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_MEM_INCR_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Mem_Incr;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_PeripheralIncrement(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_Incr)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Peri_Incr >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_PERI_INCR_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_PERI_INCR_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Peri_Incr;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_CircularMode(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Circ)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Circ >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_CIRC_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_CIRC_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Circ;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_SET_DataTransDirection(void *DMA_Peri, u8_t STRM_Num, u64_t Data_Dir)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = Data_Dir >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_TRANS_DIR_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_TRANS_DIR_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)Data_Dir;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_PeripherlaFlowCTRL(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_F)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Peri_F >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_PERO_FLOW_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_PERI_FLOW_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Peri_F;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].CR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_SET_NumOfData(void *DMA_Peri, u8_t STRM_Num, u16_t Num)
{
    Error_Status LOC_Status = Status_NOK;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].NDTR = Num;
    }
    return LOC_Status;
}

Error_Status DMA_SET_PeripheralAddress(void *DMA_Peri, u8_t STRM_Num, u32_t Peri_Address)
{
    Error_Status LOC_Status = Status_NOK;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].PAR = Peri_Address;
    }
    return LOC_Status;
}

Error_Status DMA_SET_Memory0_Address(void *DMA_Peri, u8_t STRM_Num, u32_t Mem_Address)
{
    Error_Status LOC_Status = Status_NOK;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].M0AR = Mem_Address;
    }
    return LOC_Status;
}

Error_Status DMA_SET_Memory1_Address(void *DMA_Peri, u8_t STRM_Num, u32_t Mem_Address)
{
    Error_Status LOC_Status = Status_NOK;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].M1AR = Mem_Address;
    }
    return LOC_Status;
}

Error_Status DMA_GET_FIFO_Status(void *DMA_Peri, u8_t STRM_Num, u8_t *Status)
{
    Error_Status LOC_Status = Status_NOK;

    if (DMA_Peri == NULL || Status == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        *Status = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].FCR & DMA_FIFO_GET_FLAG;
    }
    return LOC_Status;
}

Error_Status DMA_CTRL_DirectMode(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Dir_M)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Dir_M >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].FCR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_DIR_MODE_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_DIR_MODE_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Dir_M;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].FCR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_CFG_FIFOThreshold(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Fifo_Thres)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = DMA_Fifo_Thres >> DMA_INPUT_ASSERT_OFFSET;
    u32_t LOC_TempReg = ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].FCR;

    if (DMA_Peri == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (Input_Assert != DMA_FIFO_THRES_ASSERT_MASK || STRM_Num > DMA_MAX_STRM_NUM)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_TempReg &= ~DMA_FIFO_THRES_CLEAR_FLAG;
        LOC_TempReg |= (u32_t)DMA_Fifo_Thres;
        ((DMA_Peri_t *)DMA_Peri)->DMA_STRM_REG[STRM_Num].FCR = LOC_TempReg;
    }
    return LOC_Status;
}

Error_Status DMA_Set_CallBackFunction(void *DMA_Peri, u64_t DMA_Strm, Handler_t CB)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t Input_Assert = (DMA_Strm >> DMA_INPUT_ASSERT_OFFSET) & DMA_IRQ_GET_1ST_BYTE_MASK;
    u8_t CB_Index = (DMA_Strm >> DMA_INPUT_ASSERT_OFFSET) & DMA_IRQ_GET_2ND_BYTE_MASK;

    if (DMA_Peri == NULL || CB == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (!Input_Assert || Input_Assert > DMA_GET_INT_STS_CLR_ASSERT_H_MASK)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        IRQ_CallBackFunction[((u32_t)DMA_Peri & DMA_2_GET_MASK) >> DMA_10_BIT_OFFSET_MASK][CB_Index] = CB;
    }
    return LOC_Status;
}

static void General_IRQ_Handler(void *DMA_Peri, u16_t STRM_Num)
{
    u8_t Iterator;
    u8_t REG_Index;
    u8_t Start_Bit;
    u32_t REG_Value;
    u8_t DMA_Index;

    Start_Bit = STRM_Num >> DMA_4_BIT_OFFSET_MASK;
    DMA_Index = ((u32_t)DMA_Peri & DMA_2_GET_MASK) >> DMA_10_BIT_OFFSET_MASK;
    STRM_Num = STRM_Num & DMA_IRQ_GET_LOW_NIB_MASK;
    REG_Value = (STRM_Num > DMA_LISR_THRES) ? ((DMA_Peri_t *)DMA_Peri)->HISR : ((DMA_Peri_t *)DMA_Peri)->LISR;
    REG_Index = (STRM_Num > DMA_LISR_THRES) ? DMA_HIGH_REG : DMA_LOW_REG;

    for (Iterator = Start_Bit; Iterator < (Start_Bit + DMA_5_BIT_OFFSET_MASK); Iterator++)
    {
        if (REG_Value & (1 << Iterator))
        {
            break;
        }
    }

    if (IRQ_CallBackFunction[DMA_Index][Iterator + (DMA_MAX_IRQ_IDX * REG_Index)])
    {
        IRQ_CallBackFunction[DMA_Index][Iterator + (DMA_MAX_IRQ_IDX * REG_Index)]();
    }
}

void DMA1_Stream0_IRQHandler(void)
{
    General_IRQ_Handler(DMA_1, DMA_STRM_0_IRQ);
}

void DMA1_Stream1_IRQHandler(void)
{
    General_IRQ_Handler(DMA_1, DMA_STRM_1_IRQ);
}

void DMA1_Stream2_IRQHandler(void)
{
    General_IRQ_Handler(DMA_1, DMA_STRM_2_IRQ);
}

void DMA1_Stream3_IRQHandler(void)
{
    General_IRQ_Handler(DMA_1, DMA_STRM_3_IRQ);
}

void DMA1_Stream4_IRQHandler(void)
{
    General_IRQ_Handler(DMA_1, DMA_STRM_4_IRQ);
}

void DMA1_Stream5_IRQHandler(void)
{
    General_IRQ_Handler(DMA_1, DMA_STRM_5_IRQ);
}

void DMA1_Stream6_IRQHandler(void)
{
    General_IRQ_Handler(DMA_1, DMA_STRM_6_IRQ);
}

void DMA1_Stream7_IRQHandler(void)
{
    General_IRQ_Handler(DMA_1, DMA_STRM_7_IRQ);
}

void DMA2_Stream0_IRQHandler(void)
{
    General_IRQ_Handler(DMA_2, DMA_STRM_0_IRQ);
}

void DMA2_Stream1_IRQHandler(void)
{
    General_IRQ_Handler(DMA_2, DMA_STRM_1_IRQ);
}

void DMA2_Stream2_IRQHandler(void)
{
    General_IRQ_Handler(DMA_2, DMA_STRM_2_IRQ);
}

void DMA2_Stream3_IRQHandler(void)
{
    General_IRQ_Handler(DMA_2, DMA_STRM_3_IRQ);
}

void DMA2_Stream4_IRQHandler(void)
{
    General_IRQ_Handler(DMA_2, DMA_STRM_4_IRQ);
}

void DMA2_Stream5_IRQHandler(void)
{
    General_IRQ_Handler(DMA_2, DMA_STRM_5_IRQ);
}

void DMA2_Stream6_IRQHandler(void)
{
    General_IRQ_Handler(DMA_2, DMA_STRM_6_IRQ);
}

void DMA2_Stream7_IRQHandler(void)
{
    General_IRQ_Handler(DMA_2, DMA_STRM_7_IRQ);
}