#ifndef SCHEDULER_
#define SCHEDULER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_Types.h"

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void (*RunnableCB_t)(void);

typedef struct
{
    char *name;
    u32_t periodMs;
    RunnableCB_t CB;
} Runnable_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * use this function to start the operation of the Scheduler
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SCH_CTRL_StartScheduler();

/*
 * use this function to initialize the scheduler
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SCH_CFG_SchedulerInit();

#endif // SCHEDULER_