

#ifndef SW_H_
#define SW_H_

#include "STD_Types.h"
#include "SW_cfg.h"

#define SW_PULL_UP 0x00000008
#define SW_PULL_DOWN 0x00000018
#define SW_STATE_PRESSED 1
#define SW_STATE_NOT_PRESSED 0
#define SW_4BIT_OFFSET 4

typedef struct
{
    void *Port;
    u32_t Pin;
    u8_t Direct;
} SW_cfg_t;

/*
 * use this function to initialize all the switches you configured
 */
void SW_Init();

/*
 * use this function to get the state of a switch
 * Parameters:
 * SW -> the name of the desired led
 * Return:
 * SW_STATE_PRESSED, SW_STATE_NOT_PRESSED
 */
u8_t SW_GetState(u32_t Switch);

#endif