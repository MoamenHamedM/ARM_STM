#ifndef NVIC_DRIVER_
#define NVIC_DRIVER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "STD_Types.h"
#include "STM32f401CC_IRQ.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define NVIC_STATUS_NOT_ACTIVE 0x00000000
#define NVIC_STATUS_ACTIVE 0x00000001
#define NVIC_SUBGROUB_NUM 1

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * use this function to Enable an IRQ
 * parameters:
 * NVIC_IQR -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_EnableIRQ(IRQ_ID_t NVIC_IQR);

/*
 * use this function to Disable an IRQ
 * parameters:
 * NVIC_IQR -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_DisableIRQ(IRQ_ID_t NVIC_IQR);

/*
 * use this function to Set the pending bit corresponding to an IRQ
 * parameters:
 * NVIC_IQR -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_SetIRQPending(IRQ_ID_t NVIC_IQR);

/*
 * use this function to Clear the pending bit correspondin to an IRQ
 * parameters:
 * NVIC_IQR -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_ClearIRQPending(IRQ_ID_t NVIC_IQR);

/*
 * use this function to Generate a software interrupt corresponding to an IRQ
 * parameters:
 * NVIC_IQR -> the ID of the requested interrupt from the target IRQ_ID list
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CTRL_GenerateSWI(IRQ_ID_t NVIC_IQR);

/*
 * use this function to read the status of an IRQ
 * parameters:
 * NVIC_IQR -> the ID of the requested interrupt from the target IRQ_ID list
 * Status -> address of the var to hold the status -> NVIC_STATUS_NOT_ACTIVE, NVIC_STATUS_ACTIVE
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_Get_ActiveStatus(IRQ_ID_t NVIC_IQR, u32_t *Status);

/*
 * use this function to Set the priority levels corresponding to an IRQ
 * parameters:
 * NVIC_IQR -> the ID of the requested interrupt from the target IRQ_ID list
 * PreemptPri -> value of the Preemption priority level
 * SubGroupPri -> value of the Subgroub priority level
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status NVIC_CFG_SetPriority(IRQ_ID_t NVIC_IQR, u8_t PreemptPri, u8_t SubGroupPri);

#endif // NVIC_DRIVER_