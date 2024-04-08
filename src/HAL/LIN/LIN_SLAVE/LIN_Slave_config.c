
#include "STD_Types.h"
#include "HAL/LIN_Types.h"
#include "HAL/LIN_SLAVE/LIN_Slave_config.h"

static Message0_Data_t MSG0Data;
static Message1_Data_t MSG1Data;

static const LIN_Signal_t Signals[3] =
    {
        [0] = {.Start_Index = 0, .Length = 3},
        [1] = {.Start_Index = 4, .Length = 2},
        [2] = {.Start_Index = 7, .Length = 5}};

const LIN_Message_t Slave1_Messages[_SLAVE1_MSG_NUM] =
    {
        [First_MSG] =
            {
                .ID = 0x7,
                .PID = 0x47,
                .Data = MSG0Data,
                .Data_Length = 2,
                .Relation = Sender,
                .Signals_Num = 2,
                .Signals[0] = &Signals[0],
                .Signals[1] = &Signals[2]

            },
        [Second_MSG] =
            {
                .ID = 0x5,
                .PID = 0x45,
                .Data = MSG1Data,
                .Data_Length = 1,
                .Relation = Receiver,
                .Signals_Num = 1,
                .Signals[0] = &Signals[0],
            }};
