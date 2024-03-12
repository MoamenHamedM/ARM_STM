

#include "HAL/SW.h"
#include "MCAL/GPIO_DRIVER.h"

#define SW_STABLE_THRESHOLD 5

extern const SW_cfg_t Switches[_SW_Num];
u8_t SW_PhysicalState[_SW_Num];

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
    if (SW_State == NULL)
    {
        LOC_Status = Status_Null_Pointer;
    }
    else
    {
        *SW_State = !(SW_PhysicalState[Switch] ^ (Switches[Switch].Direct >> SW_4BIT_OFFSET));
    }
    return LOC_Status;
}

void SW_Runnable(void)
{
    u8_t Index;
    u8_t Curr;
    static u8_t Prev[_SW_Num] = {0};
    static u8_t Counts[_SW_Num] = {0};
    for (Index = 0; Index < _SW_Num; Index++)
    {
        GPIO_Get_PinValue(Switches[Index].Port, Switches[Index].Pin, &Curr);
        if (Curr == Prev[Index])
        {
            Counts[Index]++;
        }
        else
        {
            Counts[Index] = 0;
        }
        if (Counts[Index] == SW_STABLE_THRESHOLD)
        {
            SW_PhysicalState[Index] = Curr;
            Counts[Index] = 0;
        }
        Prev[Index] = Curr;
    }
}