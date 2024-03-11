
#include "HAL/LED.h"
#include "HAL/SW.h"

void Runnable_APP2(void)
{
    u8_t temp;
    SW_GetState(SW_ONE, &temp);
    if (temp == SW_STATE_PRESSED)
    {
        LED_SetState(LED_2, LED_STATE_ON);
    }
    else
    {
        LED_SetState(LED_2, LED_STATE_OFF);
    }
}