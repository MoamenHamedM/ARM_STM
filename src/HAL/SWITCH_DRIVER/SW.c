

#include "SW.h"
#include "GPIO_DRIVER.h"

extern const SW_cfg_t Switches[_SW_Num];

Error_Status SW_Init()
{
    Error_Status LOC_Status = Status_NOK;
    GPIO_Pin_t Switch;
    u8_t index;

    for (index = 0; index < _SW_Num; index++)
    {
        Switch.Pin = Switches[index].Pin;
        Switch.Port = Switches[index].Port;
        Switch.Mode = Switches[index].Direct;
        LOC_Status = GPIO_Init(&Switch);
    }
    return LOC_Status;
}

Error_Status SW_GetState(u32_t Switch, u8_t *SW_State)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t LOC_Temp;
    if (SW_State == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        LOC_Status = GPIO_Get_PinValue(Switches[Switch].Port, Switches[Switch].Pin, &LOC_Temp);
        *SW_State = !(LOC_Temp ^ (Switches[Switch].Direct >> SW_4BIT_OFFSET));
    }
    return LOC_Status;
}