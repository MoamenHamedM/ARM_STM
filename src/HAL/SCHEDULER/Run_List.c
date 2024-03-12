
#include "Run_List.h"
#include "HAL/SCHEDULER.h"
#include "HAL/Runnables_Prototypes.h"

const Runnable_t Run_List[_Run_Num] =
    {
        [LedToggle] = {
            .name = "toggle led",
            .periodMs = 500,
            .CB = Runnable_LED_Toggle},
        [Sw_run] = {.name = "get state of all switches", .periodMs = 10, .CB = SW_Runnable},
        [LedSWOnOff] = {.name = "control a led with a switch", .periodMs = 200, .CB = Runnable_LED_SW_ON_OFF},
        [LED_StateMachine] = {.name = "traffic light state machine", .periodMs = 100, .CB = Runnable_LED_StateMachine}};