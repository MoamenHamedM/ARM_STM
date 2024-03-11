

#ifndef LED_H_
#define LED_H_

#include "STD_Types.h"
#include "LED_cfg.h"

#define LED_DIR_FORWARD 0
#define LED_DIR_REVERSE 1
#define LED_STATE_ON 1
#define LED_STATE_OFF 0

typedef struct
{
    void *Port;
    u32_t Pin;
    u8_t Direct;
} LED_cfg_t;

/*
 * use this function to initialize all the leds you configured
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status LED_Init();

/*
 * use this function to set the sate of a led
 * Parameters:
 * Led -> the name of the desired led
 * State -> LED_STATE_ON, LED_STATE_OFF
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status LED_SetState(u32_t Led, u8_t State);

/*
 * use this function to toggle the state of a led
 * Parameters:
 * Led -> the name of the desired led
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status LED_ToggleLed(u32_t Led);

#endif