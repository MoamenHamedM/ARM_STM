

#include "GPIO_DRIVER.h"
#include "LED.h"

const LED_cfg_t Leds[_LED_Num] =
    {
        [LED_Alarm] = {
            .Port = GPIO_PORT_A,
            .Pin = GPIO_PIN_0,
            .Direct = LED_DIR_REVERSE},
        [LED_Red] = {.Port = GPIO_PORT_A, .Pin = GPIO_PIN_1, .Direct = LED_DIR_REVERSE},
        [LED_green] = {.Port = GPIO_PORT_B, .Pin = GPIO_PIN_0, .Direct = LED_DIR_FORWARD}};