

#include "MCAL/GPIO_DRIVER.h"
#include "HAL/LED.h"

const LED_cfg_t Leds[_LED_Num] =
    {
        [LED_Toggle] = {
            .Port = GPIO_PORT_A,
            .Pin = GPIO_PIN_11,
            .Direct = LED_DIR_FORWARD}};