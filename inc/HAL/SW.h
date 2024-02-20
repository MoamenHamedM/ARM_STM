

#ifndef SW_H_
#define SW_H_

#include "STD_Types.h"
#include "SW_cfg.h"

#define SW_PULL_UP 0x00000008
#define SW_PULL_DOWN 0x00000010
#define SW_STATE_PRESSED 0x00000001
#define SW_STATE_NOT_PRESSED 0x00000000
#define SW_4BIT_OFFSET 0x00000004

typedef struct
{
    void *Port;
    u32_t Pin;
    u8_t Direct;
} SW_cfg_t;

/*
 * use this function to initialize all the switches you configured
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SW_Init();

/*
 * use this function to get the state of a switch
 * Parameters:
 * SW -> the name of the desired switch
 * SW_State -> address of the var to hold the state -> SW_STATE_PRESSED, SW_STATE_NOT_PRESSED
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status SW_GetState(u32_t Switch, u8_t *SW_State);

#endif