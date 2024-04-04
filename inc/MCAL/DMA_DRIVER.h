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

#define DMA_STRM_0_TRANS_COMP_FLAG 0x050100000020
#define DMA_STRM_1_TRANS_COMP_FLAG 0x0B0100000800
#define DMA_STRM_2_TRANS_COMP_FLAG 0x150100200000
#define DMA_STRM_3_TRANS_COMP_FLAG 0x1B0108000000
#define DMA_STRM_4_TRANS_COMP_FLAG 0x050200000020
#define DMA_STRM_5_TRANS_COMP_FLAG 0x0B0200000800
#define DMA_STRM_6_TRANS_COMP_FLAG 0x150200200000
#define DMA_STRM_7_TRANS_COMP_FLAG 0x1B0208000000

#define DMA_STRM_0_H_TRANS_COMP_FLAG 0x040100000010
#define DMA_STRM_1_H_TRANS_COMP_FLAG 0x0A0100000400
#define DMA_STRM_2_H_TRANS_COMP_FLAG 0x140100100000
#define DMA_STRM_3_H_TRANS_COMP_FLAG 0x1A0104000000
#define DMA_STRM_4_H_TRANS_COMP_FLAG 0x0400200000010
#define DMA_STRM_5_H_TRANS_COMP_FLAG 0x0A00200000400
#define DMA_STRM_6_H_TRANS_COMP_FLAG 0x1400200100000
#define DMA_STRM_7_H_TRANS_COMP_FLAG 0x1A00204000000

#define DMA_STRM_0_TRANS_ERR_FLAG 0x030100000004
#define DMA_STRM_1_TRANS_ERR_FLAG 0x090100000200
#define DMA_STRM_2_TRANS_ERR_FLAG 0x130100080000
#define DMA_STRM_3_TRANS_ERR_FLAG 0x190102000000
#define DMA_STRM_4_TRANS_ERR_FLAG 0x030200000004
#define DMA_STRM_5_TRANS_ERR_FLAG 0x090200000200
#define DMA_STRM_6_TRANS_ERR_FLAG 0x130200080000
#define DMA_STRM_7_TRANS_ERR_FLAG 0x190202000000

#define DMA_STRM_0_DIRM_ERR_FLAG 0x020100000002
#define DMA_STRM_1_DIRM_ERR_FLAG 0x080100000100
#define DMA_STRM_2_DIRM_ERR_FLAG 0x120100040000
#define DMA_STRM_3_DIRM_ERR_FLAG 0x180101000000
#define DMA_STRM_4_DIRM_ERR_FLAG 0x020200000002
#define DMA_STRM_5_DIRM_ERR_FLAG 0x080200000100
#define DMA_STRM_6_DIRM_ERR_FLAG 0x120200040000
#define DMA_STRM_7_DIRM_ERR_FLAG 0x180201000000

#define DMA_STRM_0_FIFO_ERR_FLAG 0x000100000001
#define DMA_STRM_1_FIFO_ERR_FLAG 0x060100000040
#define DMA_STRM_2_FIFO_ERR_FLAG 0x100100010000
#define DMA_STRM_3_FIFO_ERR_FLAG 0x160100400000
#define DMA_STRM_4_FIFO_ERR_FLAG 0x000200000001
#define DMA_STRM_5_FIFO_ERR_FLAG 0x060200000040
#define DMA_STRM_6_FIFO_ERR_FLAG 0x100200010000
#define DMA_STRM_7_FIFO_ERR_FLAG 0x160200400000

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
typedef void (*Handler_t)(void);

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * Enable or disable a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_StreamState -> State of the DMA stream (DMA_STREMSTATE_DISABLE or DMA_STREMSTATE_ENABLE).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CTRL_StreamEnable_Disable(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_StreamState);

/*
 * Get the interrupt status of a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   DMA_Strm -> DMA stream number with interrupt flag (DMA_STRM_0_TRANS_COMP_FLAG to DMA_STRM_7_FIFO_ERR_FLAG).
 *   Status -> Pointer to store the interrupt status.
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_GET_InterruptStatus(void *DMA_Peri, u64_t DMA_Strm, u32_t *Status);

/*
 * Clear the interrupt flag of a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   DMA_Strm -> DMA stream number with interrupt flag (DMA_STRM_0_TRANS_COMP_FLAG to DMA_STRM_7_FIFO_ERR_FLAG).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CTRL_ClearInterruptFlag(void *DMA_Peri, u64_t DMA_Strm);

/*
 * Enable or disable interrupts for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   DMA_Peri_F -> Interrupt flag (DMA_TCIE_FLAG, DMA_HTIE_FLAG, DMA_TEIE_FLAG, DMA_DMEIE_FLAG, or DMA_FIFO_EI_FLAG).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   State -> State of the interrupt (DMA_INT_STATE_ENABLE or DMA_INT_STATE_DISABLE).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CTRL_InterruptEnable_Disable(void *DMA_Peri, u64_t DMA_Peri_F, u8_t STRM_Num, u8_t State);

/*
 * Select a DMA channel for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Chn -> DMA channel (DMA_CHN_0 to DMA_CHN_7).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_SEL_Channel(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Chn);

/*
 * Configure memory burst mode for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Mem_B -> Memory burst mode (DMA_MEM_B_SINGLE, DMA_MEM_B_INCR4, DMA_MEM_B_INCR8, or DMA_MEM_B_INCR16).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CTRL_MemoryBurst(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_B);

/*
 * Configure peripheral burst mode for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Peri_B -> Peripheral burst mode (DMA_PERI_B_SINGLE, DMA_PERI_B_INCR4, DMA_PERI_B_INCR8, or DMA_PERI_B_INCR16).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_PeripheralBurst(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_B);

/*
 * Select the current target (memory 0 or memory 1) for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_CT -> Current target (DMA_CT_MEM0 or DMA_CT_MEM1).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_SEL_CurrentTarget(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_CT);

/*
 * Get the current target for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_CT -> Pointer to store the current target.
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_GET_CurrentTarget(void *DMA_Peri, u8_t STRM_Num, u32_t *DMA_CT);

/*
 * Configure double buffer mode for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_DB -> Double buffer mode (DMA_DB_OFF or DMA_DB_ON).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_DoubleBuffer(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_DB);

/*
 * Configure priority level for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Pri -> Priority level (DMA_PRIORITY_LOW, DMA_PRIORITY_MED, DMA_PRIORITY_HIGH, or DMA_PRIORITY_VHIGH).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_PriorityLevel(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Pri);

/*
 * Configure peripheral increment and offset size for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Pincos -> Peripheral increment and offset size (DMA_PINCOS_PSIZE or DMA_PINCOS_FIXED).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_PeriIncr_OffsetSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Pincos);

/*
 * Configure memory data size for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Mem_S -> Memory data size (DMA_MEM_SIZE_BYTE, DMA_MEM_SIZE_HWORD, or DMA_MEM_SIZE_WORD).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_MemoryDataSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_S);

/*
 * Configure peripheral data size for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Peri_S -> Peripheral data size (DMA_PERI_SIZE_BYTE, DMA_PERI_SIZE_HWORD, or DMA_PERI_SIZE_WORD).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_PeripheralDataSize(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_S);

/*
 * Configure memory increment mode for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Mem_Incr -> Memory increment mode (DMA_MEM_INCR_OFF or DMA_MEM_INCR_ON).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_MemoryIncrement(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Mem_Incr);

/*
 * Configure peripheral increment mode for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Peri_Incr -> Peripheral increment mode (DMA_PERI_INCR_OFF or DMA_PERI_INCR_ON).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_PeripheralIncrement(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_Incr);

/*
 * Configure circular mode for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Circ -> Circular mode state (DMA_CIRC_OFF or DMA_CIRC_ON).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_CircularMode(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Circ);

/*
 * Configure peripheral flow control for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Peri_F -> Peripheral flow control (DMA_PERI_FLOW_DMA or DMA_PERI_FLOW_PERI).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_PeripheralFlowCTRL(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Peri_F);

/*
 * Set the data transfer direction for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   Data_Dir -> Data transfer direction (DMA_TRANS_DIR_P_TO_M, DMA_TRANS_DIR_M_TO_P, or DMA_TRANS_DIR_M_TO_M).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_SET_DataTransDirection(void *DMA_Peri, u8_t STRM_Num, u64_t Data_Dir);

/*
 * Set the number of data items to be transferred for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   Num -> Number of data items to be transferred.
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_SET_NumOfData(void *DMA_Peri, u8_t STRM_Num, u16_t Num);

/*
 * Set the peripheral address for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   Peri_Address -> Peripheral address.
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_SET_PeripheralAddress(void *DMA_Peri, u8_t STRM_Num, u32_t Peri_Address);

/*
 * Set the memory address for a DMA stream (Memory 0).
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   Mem_Address -> Memory address (Memory 0).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_SET_Memory0_Address(void *DMA_Peri, u8_t STRM_Num, u32_t Mem_Address);

/*
 * Set the memory address for a DMA stream (Memory 1).
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   Mem_Address -> Memory address (Memory 1).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_SET_Memory1_Address(void *DMA_Peri, u8_t STRM_Num, u32_t Mem_Address);

/*
 * Get the FIFO status for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   Status -> Pointer to store the FIFO status.
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_GET_FIFO_Status(void *DMA_Peri, u8_t STRM_Num, u8_t *Status);

/*
 * Configure direct mode for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Dir_M -> Direct mode configuration (DMA_DIR_MODE_ENABLE or DMA_DIR_MODE_DISABLE).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CTRL_DirectMode(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Dir_M);

/*
 * Configure the FIFO threshold for a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   STRM_Num -> DMA stream number (DMA_STRM_0 to DMA_STRM_7).
 *   DMA_Fifo_Thres -> FIFO threshold (DMA_FIFO_THRES_1_4, DMA_FIFO_THRES_1_2, DMA_FIFO_THRES_3_4, or DMA_FIFO_THRES_FULL).
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_CFG_FIFOThreshold(void *DMA_Peri, u8_t STRM_Num, u64_t DMA_Fifo_Thres);

/*
 * Seth the callback function of an interrupt in a DMA stream.
 *
 * Parameters:
 *   DMA_Peri -> Pointer to the DMA peripheral base address (DMA_1 or DMA_2).
 *   DMA_Strm -> DMA stream number with interrupt flag (DMA_STRM_0_TRANS_COMP_FLAG to DMA_STRM_7_FIFO_ERR_FLAG).
 *   CB       -> The address of the desired callback function.
 *
 * Return:
 *   Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status DMA_Set_CallBackFunction(void *DMA_Peri, u64_t DMA_Strm, Handler_t CB);

#endif