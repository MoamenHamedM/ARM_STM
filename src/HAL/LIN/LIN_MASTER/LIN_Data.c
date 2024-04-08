
#include "STD_Types.h"
#include "HAL/LIN_Types.h"
#include "HAL/LIN_MASTER/LIN_Master_config.h"

#define SEND_CONTROLS_MESSAGE 0
#define RECIEVE_CONTROLS_MESSAGE 1

extern const LIN_Message_t Master_Messages[_MASTER_MSG_NUM];

void SendControls(u8_t *controls)
{
    u64_t Data;
}