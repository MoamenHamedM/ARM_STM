
#include "STD_Types.h"
#include "HAL/LIN_Types.h"
#include "LIN_cfg.h"
#include "HAL/LIN_MASTER/LIN_Master_config.h"
#include "HAL/LIN_SLAVE/LIN_Slave_config.h"

static Message0_Data_t MSG0Data;
static Message1_Data_t MSG1Data;

static const LIN_Signal_t Signals[_Signal_NUM] =
    {
        [First_signal] = {.Start_Index = 0, .Length = 3},
        [Second_signal] = {.Start_Index = 4, .Length = 2},
        [Third_signal] = {.Start_Index = 7, .Length = 5}};

/* STATIC BECAUSE IT MAKES A MULTIPLE DEFIITION PROBLEM BECUASE THE SLAVE AND MASTER NODES ARE COMPILED
 * ON THE SAME MCU WHICH IS NOT THE CASE IN REAL SITUATION WHERE ONLY THE SLAVE OR THE MASTER EXIST */

static const LIN_Message_t Slave1_Messages[_SLAVE1_MSG_NUM] =
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
        [First_MSG] =
            {
                .ID = 0x5,
                .PID = 0x45,
                .Data = MSG1Data,
                .Data_Length = 1,
                .Relation = Receiver,
                .Signals_Num = 1,
                .Signals[0] = &Signals[0],
            }};

const LIN_Message_t Master_Messages[_MASTER_MSG_NUM] =
    {
        [First_MSG] =
            {
                .ID = 0x7,
                .PID = 0x47,
                .Data = MSG0Data,
                .Data_Length = 2,
                .Relation = Receiver,
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
                .Relation = Sender,
                .Signals_Num = 1,
                .Signals[0] = &Signals[0],
            }};

const LIN_SchedTableEntry_t SchedTable[SCHED_TABLE_MESSAGES_NUM] =
    {
        [0] = {.Message = &Slave1_Messages[0], .TimeSlotMs = 10},
        [1] = {.Message = &Slave1_Messages[1], .TimeSlotMs = 15}};
