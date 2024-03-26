
#if 0
#include "HAL/LED.h"
#include "HAL/SW.h"

void Runnable_LED_SW_ON_OFF(void)
{
    u8_t LOC_SW_State = SW_STATE_NOT_PRESSED;

    SW_GetState(SW_ONE, &LOC_SW_State);
    if (LOC_SW_State == SW_STATE_PRESSED)
    {
        LED_SetState(LED_SW, LED_STATE_ON);
    }
    else
    {
        LED_SetState(LED_SW, LED_STATE_OFF);
    }
}
#endif