/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "HAL/SCHEDULER.h"
#include "MCAL/SYSTICK_DRIVER.h"
#include "Run_List.h"
#include "SYSTICK_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define TICK_TIME_MS 10

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
static volatile u32_t PendingTicks;
extern const Runnable_t Run_List[_Run_Num];

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
void TickCB(void);
static void SCHD(void);
extern void Runnable_LED_StateMachine(void);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status SCH_CTRL_StartScheduler()
{
    Error_Status LOC_Status = Status_NOK;
    LOC_Status = SYSTICK_CTRL_StartTimer();

    while (1)
    {
        if (PendingTicks)
        {
            PendingTicks--;
            SCHD();
        }
    }
    return LOC_Status;
}

Error_Status SCH_CFG_SchedulerInit()
{
    Error_Status LOC_Status = Status_NOK;
    LOC_Status = SYSTICK_CTRL_Interrupt(SYSTICK_IRQ_ENABLE);
    LOC_Status = SYSTICK_SET_CurrentVal(0);
    LOC_Status = SYSTICK_SET_CallBack(TickCB);
    LOC_Status = SYSTICK_CFG_CLKSource(SYSTICK_CLK_AHB);
    LOC_Status = SYSTICK_SET_TimeTicksMs(TICK_TIME_MS);
    return LOC_Status;
}

static void SCHD(void)
{
    u32_t Index;
    static u32_t TimeStamp = 0;

    for (Index = 0; Index < _Run_Num; Index++)
    {
        if ((Run_List[Index].CB) && ((TimeStamp % Run_List[Index].periodMs) == 0))
        {
            // Runnable_LED_StateMachine();
            Run_List[Index].CB();
        }
    }
    TimeStamp += TICK_TIME_MS;
}

void TickCB(void)
{
    PendingTicks++;
}