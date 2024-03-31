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

#define DMA_STRM_0_TRANS_COMP_FLAG 0x0100000020
#define DMA_STRM_1_TRANS_COMP_FLAG 0x0100000800
#define DMA_STRM_2_TRANS_COMP_FLAG 0x0100200000
#define DMA_STRM_3_TRANS_COMP_FLAG 0x0108000000
#define DMA_STRM_4_TRANS_COMP_FLAG 0x0200000020
#define DMA_STRM_5_TRANS_COMP_FLAG 0x0200000800
#define DMA_STRM_6_TRANS_COMP_FLAG 0x0200200000
#define DMA_STRM_7_TRANS_COMP_FLAG 0x0208000000

#define DMA_STRM_0_H_TRANS_COMP_FLAG 0x0100000010
#define DMA_STRM_1_H_TRANS_COMP_FLAG 0x0100000400
#define DMA_STRM_2_H_TRANS_COMP_FLAG 0x0100100000
#define DMA_STRM_3_H_TRANS_COMP_FLAG 0x0104000000
#define DMA_STRM_4_H_TRANS_COMP_FLAG 0x0200000010
#define DMA_STRM_5_H_TRANS_COMP_FLAG 0x0200000400
#define DMA_STRM_6_H_TRANS_COMP_FLAG 0x0200100000
#define DMA_STRM_7_H_TRANS_COMP_FLAG 0x0204000000

#define DMA_STRM_0_TRANS_ERR_FLAG 0x0100000004
#define DMA_STRM_1_TRANS_ERR_FLAG 0x0100000200
#define DMA_STRM_2_TRANS_ERR_FLAG 0x0100080000
#define DMA_STRM_3_TRANS_ERR_FLAG 0x0102000000
#define DMA_STRM_4_TRANS_ERR_FLAG 0x0200000004
#define DMA_STRM_5_TRANS_ERR_FLAG 0x0200000200
#define DMA_STRM_6_TRANS_ERR_FLAG 0x0200080000
#define DMA_STRM_7_TRANS_ERR_FLAG 0x0202000000

#define DMA_STRM_0_DIRM_ERR_FLAG 0x0100000002
#define DMA_STRM_1_DIRM_ERR_FLAG 0x0100000100
#define DMA_STRM_2_DIRM_ERR_FLAG 0x0100040000
#define DMA_STRM_3_DIRM_ERR_FLAG 0x0101000000
#define DMA_STRM_4_DIRM_ERR_FLAG 0x0200000002
#define DMA_STRM_5_DIRM_ERR_FLAG 0x0200000100
#define DMA_STRM_6_DIRM_ERR_FLAG 0x0200040000
#define DMA_STRM_7_DIRM_ERR_FLAG 0x0201000000

#define DMA_STRM_0_FIFO_ERR_FLAG 0x0100000001
#define DMA_STRM_1_FIFO_ERR_FLAG 0x0100000040
#define DMA_STRM_2_FIFO_ERR_FLAG 0x0100010000
#define DMA_STRM_3_FIFO_ERR_FLAG 0x0100400000
#define DMA_STRM_4_FIFO_ERR_FLAG 0x0200000001
#define DMA_STRM_5_FIFO_ERR_FLAG 0x0200000040
#define DMA_STRM_6_FIFO_ERR_FLAG 0x0200010000
#define DMA_STRM_7_FIFO_ERR_FLAG 0x0200400000

#define DMA_TCIE_FLAG 0x2000000008
#define DMA_HTIE_FLAG 0x2000000004
#define DMA_TEIE_FLAG 0x2000000001
#define DMA_DMEIE_FLAG 0x2000000001
#define DMA_FIFO_EI_FLAG 0x3000000008

#define DMA_INT_STATE_ENABLE 1
#define DMA_INT_STATE_DISABLE 0

#define DMA_CHN_0 0x0300000000
#define DMA_CHN_1 0x0302000000
#define DMA_CHN_2 0x0304000000
#define DMA_CHN_3 0x0306000000
#define DMA_CHN_4 0x0308000000
#define DMA_CHN_5 0x030A000000
#define DMA_CHN_6 0x030C000000
#define DMA_CHN_7 0x030E000000

#define DMA_MEM_B_SINGLE 0x0400000000
#define DMA_MEM_B_INCR4 0x0400800000
#define DMA_MEM_B_INCR8 0x0401000000
#define DMA_MEM_B_INCR16 0x0403000000

#define DMA_PERI_B_SINGLE 0x0500000000
#define DMA_PERI_B_INCR4 0x0500200000
#define DMA_PERI_B_INCR8 0x0500400000
#define DMA_PERI_B_INCR16 0x0500600000

#define DMA_CT_MEM0 0x0600000000
#define DMA_CT_MEM1 0x0600080000

#define DMA_DB_OFF 0x0700000000
#define DMA_DB_ON 0x0700040000

#define DMA_PRIORITY_LOW 0x0800000000
#define DMA_PRIORITY_MED 0x0800010000
#define DMA_PRIORITY_HIGH 0x0800020000
#define DMA_PRIORITY_VHIGH 0x0800030000

#define DMA_PINCOS_PSIZE 0x0900000000
#define DMA_PINCOS_FIXED 0x0900008000

#define DMA_MEM_SIZE_BYTE 0x0A00000000
#define DMA_MEM_SIZE_HWORD 0x0A00002000
#define DMA_MEM_SIZE_WORD 0x0A00004000

#define DMA_PERI_SIZE_BYTE 0x0B00000000
#define DMA_PERI_SIZE_HWORD 0x0B00000800
#define DMA_PERI_SIZE_WORD 0x0B00001000

#define DMA_MEM_INCR_OFF 0x0C00000000
#define DMA_MEM_INCR_ON 0x0C00000400

#define DMA_PERI_INCR_OFF 0x0D00000000
#define DMA_PERI_INCR_ON 0x0D00000200

#define DMA_CIRC_OFF 0x0E00000000
#define DMA_CIRC_ON 0x0E00000100

#define DMA_TRANS_DIR_P_TO_M 0x0F00000000
#define DMA_TRANS_DIR_M_TO_P 0x0F00000040
#define DMA_TRANS_DIR_M_TO_M 0x0F00000080

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