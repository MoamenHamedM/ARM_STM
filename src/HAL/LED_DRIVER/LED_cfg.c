

#include "MCAL/GPIO_DRIVER.h"
#include "HAL/LED.h"

const LED_cfg_t Leds[_LED_Num] =
    {
        [LED_Toggle] = {
            .Port = GPIO_PORT_A,
            .Pin = GPIO_PIN_0,
            .Direct = LED_DIR_FORWARD},
        [LED_SW] = {.Port = GPIO_PORT_A, .Pin = GPIO_PIN_2, .Direct = LED_DIR_FORWARD},
        [LED_Red] = {.Port = GPIO_PORT_B, .Pin = GPIO_PIN_4, .Direct = LED_DIR_FORWARD},
        [LED_Yellow] = {.Port = GPIO_PORT_B, .Pin = GPIO_PIN_5, .Direct = LED_DIR_FORWARD},
        [LED_Green] = {.Port = GPIO_PORT_B, .Pin = GPIO_PIN_6, .Direct = LED_DIR_FORWARD}};