

#include "SW.h"
#include "GPIO_DRIVER.h"

extern const SW_cfg_t Switches[_SW_Num];

void SW_Init()
{
    GPIO_Pin_t Switch;
    u8_t index;

    for (index = 0; index < _SW_Num; index++)
    {
        Switch.Pin = Switches[index].Pin;
        Switch.Port = Switches[index].Port;
        Switch.Mode = Switches[index].Direct;
        GPIO_Init(&Switch);
    }
}

u8_t SW_GetState(u32_t Switch)
{
    u8_t LOC_Result;
    LOC_Result = GPIO_Get_PinValue(Switches[Switch].Port, Switches[Switch].Pin);

    return !(LOC_Result ^ (Switches[Switch].Direct >> SW_4BIT_OFFSET));
}