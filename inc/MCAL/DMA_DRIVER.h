#ifndef DMA_DRIVER_
#define DMA_DRIVER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_Types.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define DMA_1 (void *)0x40026000
#define DMA_2 (void *)0x40026400

#define DMA_STRM_0 0
#define DMA_STRM_1 1
#define DMA_STRM_2 2
#define DMA_STRM_3 3
#define DMA_STRM_4 4
#define DMA_STRM_5 5
#define DMA_STRM_6 6
#define DMA_STRM_7 7

#define DMA_STREMSTATE_DISABLE 0x4000000000
#define DMA_STREMSTATE_ENABLE 0x4000000001

#define DMA_STRM_0_TRANS_COMP_FLAG 0x100000020
#define DMA_STRM_1_TRANS_COMP_FLAG 0x100000800
#define DMA_STRM_2_TRANS_COMP_FLAG 0x100200000
#define DMA_STRM_3_TRANS_COMP_FLAG 0x108000000
#define DMA_STRM_4_TRANS_COMP_FLAG 0x200000020
#define DMA_STRM_5_TRANS_COMP_FLAG 0x200000800
#define DMA_STRM_6_TRANS_COMP_FLAG 0x200200000
#define DMA_STRM_7_TRANS_COMP_FLAG 0x208000000

#define DMA_STRM_0_H_TRANS_COMP_FLAG 0x100000010
#define DMA_STRM_1_H_TRANS_COMP_FLAG 0x100000400
#define DMA_STRM_2_H_TRANS_COMP_FLAG 0x100100000
#define DMA_STRM_3_H_TRANS_COMP_FLAG 0x104000000
#define DMA_STRM_4_H_TRANS_COMP_FLAG 0x200000010
#define DMA_STRM_5_H_TRANS_COMP_FLAG 0x200000400
#define DMA_STRM_6_H_TRANS_COMP_FLAG 0x200100000
#define DMA_STRM_7_H_TRANS_COMP_FLAG 0x204000000

#define DMA_STRM_0_TRANS_ERR_FLAG 0x100000004
#define DMA_STRM_1_TRANS_ERR_FLAG 0x100000200
#define DMA_STRM_2_TRANS_ERR_FLAG 0x100080000
#define DMA_STRM_3_TRANS_ERR_FLAG 0x102000000
#define DMA_STRM_4_TRANS_ERR_FLAG 0x200000004
#define DMA_STRM_5_TRANS_ERR_FLAG 0x200000200
#define DMA_STRM_6_TRANS_ERR_FLAG 0x200080000
#define DMA_STRM_7_TRANS_ERR_FLAG 0x202000000

#define DMA_STRM_0_DIRM_ERR_FLAG 0x100000002
#define DMA_STRM_1_DIRM_ERR_FLAG 0x100000100
#define DMA_STRM_2_DIRM_ERR_FLAG 0x100040000
#define DMA_STRM_3_DIRM_ERR_FLAG 0x101000000
#define DMA_STRM_4_DIRM_ERR_FLAG 0x200000002
#define DMA_STRM_5_DIRM_ERR_FLAG 0x200000100
#define DMA_STRM_6_DIRM_ERR_FLAG 0x200040000
#define DMA_STRM_7_DIRM_ERR_FLAG 0x201000000

#define DMA_STRM_0_FIFO_ERR_FLAG 0x100000001
#define DMA_STRM_1_FIFO_ERR_FLAG 0x100000040
#define DMA_STRM_2_FIFO_ERR_FLAG 0x100010000
#define DMA_STRM_3_FIFO_ERR_FLAG 0x100400000
#define DMA_STRM_4_FIFO_ERR_FLAG 0x200000001
#define DMA_STRM_5_FIFO_ERR_FLAG 0x200000040
#define DMA_STRM_6_FIFO_ERR_FLAG 0x200010000
#define DMA_STRM_7_FIFO_ERR_FLAG 0x200400000

#define DMA_TCIE_FLAG 0x2000000008
#define DMA_HTIE_FLAG 0x2000000004
#define DMA_TEIE_FLAG 0x2000000001
#define DMA_DMEIE_FLAG 0x2000000001
#define DMA_FIFO_EI_FLAG 0x3000000008

#define DMA_INT_STATE_ENABLE 1
#define DMA_INT_STATE_DISABLE 0

#define DMA_CHN_0 0x300000000
#define DMA_CHN_1 0x302000000
#define DMA_CHN_2 0x304000000
#define DMA_CHN_3 0x306000000
#define DMA_CHN_4 0x308000000
#define DMA_CHN_5 0x30A000000
#define DMA_CHN_6 0x30C000000
#define DMA_CHN_7 0x30E000000

#define DMA_MEM_B_SINGLE 0x400000000
#define DMA_MEM_B_INCR4 0x400800000
#define DMA_MEM_B_INCR8 0x401000000
#define DMA_MEM_B_INCR16 0x403000000

#define DMA_PERI_B_SINGLE 0x500000000
#define DMA_PERI_B_INCR4 0x500200000
#define DMA_PERI_B_INCR8 0x500400000
#define DMA_PERI_B_INCR16 0x500600000

#define DMA_CT_MEM0 0x600000000
#define DMA_CT_MEM1 0x600080000

#define DMA_DB_OFF 0x700000000
#define DMA_DB_ON 0x700040000

#define DMA_PRIORITY_LOW 0x800000000
#define DMA_PRIORITY_MED 0x800010000
#define DMA_PRIORITY_HIGH 0x800020000
#define DMA_PRIORITY_VHIGH 0x800030000

#define DMA_PINCOS_PSIZE 0x900000000
#define DMA_PINCOS_FIXED 0x900008000

#define DMA_MEM_SIZE_BYTE 0xA00000000
#define DMA_MEM_SIZE_HWORD 0xA00002000
#define DMA_MEM_SIZE_WORD 0xA00004000

#define DMA_PERI_SIZE_BYTE 0xB00000000
#define DMA_PERI_SIZE_HWORD 0xB00000800
#define DMA_PERI_SIZE_WORD 0xB00001000

#define DMA_MEM_INCR_OFF 0xC00000000
#define DMA_MEM_INCR_ON 0xC00000400

#define DMA_PERI_INCR_OFF 0xD00000000
#define DMA_PERI_INCR_ON 0xD00000200

#define DMA_CIRC_OFF 0xE00000000
#define DMA_CIRC_ON 0xE00000100

#define DMA_TRANS_DIR_P_TO_M 0xF00000000
#define DMA_TRANS_DIR_M_TO_P 0xF00000040
#define DMA_TRANS_DIR_M_TO_M 0xF00000080

#define DMA_PERI_FLOW_DMA 0x1000000000
#define DMA_PERI_FLOW_PERI 0x1000000020

#define DMA_DIR_MODE_ENABLE 0x5000000000
#define DMA_DIR_MODE_DISABLE 0x5000000004

#define DMA_FIFO_THRES_1_4 0x6000000000
#define DMA_FIFO_THRES_1_2 0x6000000001
#define DMA_FIFO_THRES_3_4 0x6000000002
#define DMA_FIFO_THRES_FULL 0x6000000003

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

Error_Status DMA_CTRL_StreamEnable_Disable(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_StreamState);

Error_Status DMA_GET_InterruptStatus(void *DMA_Peri, u64_t DMA_Strm, u32_t *Status);

Error_Status DMA_CTRL_ClearInterruptFlag(void *DMA_Peri, u64_t DMA_Strm);

Error_Status DMA_CTRL_InterruptEnable_Disable(void *DMA_Peri, u64_t DMA_Peri_F, u8_t STRM_Num, u8_t State);

Error_Status DMA_SEL_Channel(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Chn);

Error_Status DMA_CTRL_MemoryBurst(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_B);

Error_Status DMA_CFG_PeripheralBurst(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_B);

Error_Status DMA_SEL_CurrentTarget(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_CT);

Error_Status DMA_GET_CurrentTarget(void *DMA_Peri, u8_t STRM_Num, u32_t *DMA_CT);

Error_Status DMA_CFG_DoubleBuffer(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_DB);

Error_Status DMA_CFG_PriorityLevel(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Pri);

Error_Status DMA_CFG_PeriIncr_OffsetSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Pincos);

Error_Status DMA_CFG_MemoryDataSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_S);

Error_Status DMA_CFG_PeripheralDataSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_S);

Error_Status DMA_CFG_MemoryIncrement(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_Incr);

Error_Status DMA_CFG_PeripheralIncrement(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_Incr);

Error_Status DMA_CFG_CircularMode(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Circ);

Error_Status DMA_CFG_PeripherlaFlowCTRL(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_F);

Error_Status DMA_SET_DataTransDirection(void *DMA_Peri, u8_t STRM_Num, u32_t Data_Dir);

Error_Status DMA_SET_NumOfData(void *DMA_Peri, u8_t STRM_Num, u16_t Num);

Error_Status DMA_SET_PeripheralAddress(void *DMA_Peri, u8_t STRM_Num, u32_t Peri_Address);

Error_Status DMA_SET_Memory0_Address(void *DMA_Peri, u8_t STRM_Num, u32_t Mem_Address);

Error_Status DMA_SET_Memory1_Address(void *DMA_Peri, u8_t STRM_Num, u32_t Mem_Address);

Error_Status DMA_GET_FIFO_Status(void *DMA_Peri, u8_t STRM_Num, u8_t *Status);

Error_Status DMA_CTRL_DirectMode(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Dir_M);

Error_Status DMA_CFG_FIFOThreshold(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Fifo_Thres);

#endif