
#include "Run_List.h"
#include "HAL/SCHEDULER.h"

extern void Runnable_APP1(void);
extern void Runnable_APP2(void);
extern void SW_Runnable(void);

const Runnable_t Run_List[_Run_Num] =
    {
        [APP1] = {
            .name = "toggle led 1 s",
            .periodMs = 500,
            .CB = &Runnable_APP1},
        [Sw_run] = {.name = "get state of all switches", .periodMs = 10, .CB = &SW_Runnable},
        [APP2] = {.name = "control a led with a switch", .periodMs = 200, .CB = &Runnable_APP2}};