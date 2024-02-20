

#include "HAL/LED.h"
#include "MCAL/GPIO_DRIVER.h"
#include "MCAL/RCC_DRIVER.h"

#define FIND_REG_MASK(x, minReg, maxReg, minMask, maxMask) \
    (u32_t)((float)((x) - (minReg)) / ((maxReg) - (minReg)) * ((maxMask) - (minMask)) + (minMask))

extern const LED_cfg_t Leds[_LED_Num];

Error_Status LED_Init()
{
    Error_Status LOC_Status = Status_NOK;
    GPIO_Pin_t Led;
    u8_t index;
    u32_t GPIO_RegMask;
    Led.Mode = GPIO_MODE_OP_PP;
    Led.Speed = GPIO_SPEED_HIGH;

    for (index = 0; index < _LED_Num; index++)
    {
        Led.Pin = Leds[index].Pin;
        Led.Port = Leds[index].Port;
        GPIO_RegMask = FIND_REG_MASK((u32_t)Led.Port, MIN_REG_VALUE, MAX_REG_VALUE, MIN_MASK_VALUE, MAX_MASK_VALUE);
        LOC_Status = RCC_CTRL_Peripheral_Enable(GPIO_RegMask);
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