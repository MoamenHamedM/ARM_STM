

#include "HAL/LED.h"
#include "MCAL/GPIO_DRIVER.h"

extern const LED_cfg_t Leds[_LED_Num];

Error_Status LED_Init()
{
    Error_Status LOC_Status = Status_NOK;
    GPIO_Pin_t Led;
    u8_t index;
    Led.Mode = GPIO_MODE_OP_PP;
    Led.Speed = GPIO_SPEED_HIGH;

    for (index = 0; index < _LED_Num; index++)
    {
        Led.Pin = Leds[index].Pin;
        Led.Port = Leds[index].Port;
        LOC_Status = GPIO_Init(&Led);
    }
    return LOC_Status;
}

Error_Status LED_SetState(u32_t Led, u8_t State)
{
    Error_Status LOC_Status = Status_NOK;
    LOC_Status = GPIO_Set_PinValue(Leds[Led].Port, Leds[Led].Pin, Leds[Led].Direct ^ State);
    return LOC_Status;
}

Error_Status LED_ToggleLed(u32_t Led)
{
    Error_Status LOC_Status = Status_NOK;
    u8_t LOC_State;
    LOC_Status = GPIO_Get_PinValue(Leds[Led].Port, Leds[Led].Pin, &LOC_State);
    LOC_Status = GPIO_Set_PinValue(Leds[Led].Port, Leds[Led].Pin, LOC_State ^ 1);

    return LOC_Status;
}