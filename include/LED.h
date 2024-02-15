

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
 */
void LED_Init();

/*
 * use this function to set the sate of a led
 * Parameters:
 * Led -> the name of the desired led
 * State -> LED_STATE_ON, LED_STATE_OFF
 */
void LED_SetState(u32_t Led, u8_t State);

#endif