

#if 0
#include "HAL/LED.h"

#define RED 0
#define YELLOW 1
#define GREEN 2
#define UNIT_TIME_PERIOD 100
#define RED_TIME_PERIOD 2000
#define YELLOW_TIME_PERIOD 700
#define GREEN_TIME_PERIOD 900

void Runnable_LED_StateMachine(void)
{
    static u8_t LOC_State = RED;
    static u8_t Prev;
    static u32_t LOC_Counter = 0;

    LOC_Counter += UNIT_TIME_PERIOD;

    switch (LOC_State)
    {
    case RED:
        LED_SetState(LED_Red, LED_STATE_ON);
        LED_SetState(LED_Yellow, LED_STATE_OFF);
        LED_SetState(LED_Green, LED_STATE_OFF);

        if (LOC_Counter % RED_TIME_PERIOD == 0)
        {
            LOC_State = YELLOW;
            Prev = RED;
        }
        break;

    case YELLOW:
        LED_SetState(LED_Red, LED_STATE_OFF);
        LED_SetState(LED_Yellow, LED_STATE_ON);
        LED_SetState(LED_Green, LED_STATE_OFF);

        if (LOC_Counter % YELLOW_TIME_PERIOD == 0)
        {
            LOC_State = (Prev == RED) ? GREEN : RED;
        }
        break;

    case GREEN:
        LED_SetState(LED_Red, LED_STATE_OFF);
        LED_SetState(LED_Yellow, LED_STATE_OFF);
        LED_SetState(LED_Green, LED_STATE_ON);

        if (LOC_Counter % GREEN_TIME_PERIOD == 0)
        {
            LOC_State = YELLOW;
            Prev = GREEN;
        }
        break;

    default:
        break;
    }
}
#endif