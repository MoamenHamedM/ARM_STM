/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "HAL/CLOCK_HANDLER.h"
#include "MCAL/RCC_DRIVER.h"

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

Error_Status CLK_HAND_CTRL_PeriClockEnable(u32_t CLK_HAND_Peri)
{
    Error_Status LOC_Result = Status_NOK;
    LOC_Result = RCC_CTRL_Peripheral_Enable(CLK_HAND_Peri);
    return LOC_Result;
}

Error_Status CLK_HAND_CTRL_PeriClockDisable(u32_t CLK_HAND_Peri)
{
    Error_Status LOC_Result = Status_NOK;
    LOC_Result = RCC_CTRL_Peripheral_Disable(CLK_HAND_Peri);
    return LOC_Result;
}