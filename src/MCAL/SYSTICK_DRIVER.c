/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MCAL/SYSTICK_DRIVER.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define SYSTICK_BASE_ADDRESS 0xE000E010
#define SYSTICK_START_MASK 0x00000001
#define SYSTICK_IRQ_CLEARFLAG 0x00000002
#define SYSTICK_CLK_CLEARFLAG 0x00000004
#define SYSTICK_MAXCLK_MASK 0x00FFFFFF
#define SYSTICK_MAX_CALLBACK 5

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    u32_t CTRL;
    u32_t LOAD;
    u32_t VAL;
    u32_t CALIB;
} SYSTICK_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
SYSTICK_t *const SYSTICK = (SYSTICK_t *)SYSTICK_BASE_ADDRESS;
static SYSTICK_CBF_t APPCBF[SYSTICK_MAX_CALLBACK];
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status SYSTICK_CTRL_StartTimer()
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_CTRL = SYSTICK->CTRL;

    LOC_CTRL |= SYSTICK_START_MASK;
    SYSTICK->CTRL = LOC_CTRL;
    LOC_Status = Status_OK;
    return LOC_Status;
}

Error_Status SYSTICK_CTRL_Interrupt(u32_t SYSTICK_IRQ)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_CTRL = SYSTICK->CTRL;

    if (SYSTICK_IRQ > SYSTICK_IRQ_ENABLE)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_CTRL &= ~SYSTICK_IRQ_CLEARFLAG;
        LOC_CTRL |= SYSTICK_IRQ;
        SYSTICK->CTRL = LOC_CTRL;
    }

    return LOC_Status;
}
Error_Status SYSTICK_CFG_CLKSource(u32_t SYSTICK_CLK)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_CTRL = SYSTICK->CTRL;

    if (SYSTICK_CLK > SYSTICK_CLK_AHB)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_CTRL &= ~SYSTICK_CLK_CLEARFLAG;
        LOC_CTRL |= SYSTICK_CLK;
        SYSTICK->CTRL = LOC_CTRL;
    }

    return LOC_Status;
}
Error_Status SYSTICK_SetTime(u32_t Clock_Ticks)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_LOAD = 0;

    if (Clock_Ticks > SYSTICK_MAXCLK_MASK)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_LOAD = Clock_Ticks;
        SYSTICK->LOAD = LOC_LOAD;
    }

    return LOC_Status;
}

Error_Status SYSTICK_SetCallBack(SYSTICK_CBF_t SYSTICK_CBF, u8_t CallBackNum)
{
    Error_Status LOC_Status = Status_NOK;

    if (SYSTICK_CBF == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else if (CallBackNum > SYSTICK_MAX_CALLBACK)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        APPCBF[CallBackNum] = SYSTICK_CBF;
    }

    return LOC_Status;
}
Error_Status SYSTICK_SetCurrentVal(u32_t Curr_Val)
{
    Error_Status LOC_Status = Status_NOK;
    u32_t LOC_VAL = 0;

    if (Curr_Val > SYSTICK_MAXCLK_MASK)
    {
        LOC_Status = Status_Invalid_Input;
    }
    else
    {
        LOC_Status = Status_OK;
        LOC_VAL = LOC_VAL;
        SYSTICK->VAL = LOC_VAL;
    }

    return LOC_Status;
}

Error_Status SYSTICK_GetCurrentVal(u32_t *Curr_Val)
{
    Error_Status LOC_Status = Status_NOK;

    if (Curr_Val == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = Status_OK;
        *Curr_Val = SYSTICK->VAL;
    }

    return LOC_Status;
}

void SysTick_Handler(void)
{
    int i;
    for (i = 0; i < SYSTICK_MAX_CALLBACK; i++)
    {
        if (APPCBF[i])
        {
            APPCBF[i]();
        }
    }
}