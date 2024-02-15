

#include "GPIO_DRIVER.h"
#include "SW.h"

const SW_cfg_t Switches[_SW_Num] =
    {
        [SW_ON] = {
            .Port = GPIO_PORT_A,
            .Pin = GPIO_PIN_0,
            .Direct = SW_PULL_DOWN},
        [SW_OFF] = {.Port = GPIO_PORT_A, .Pin = GPIO_PIN_1, .Direct = SW_PULL_DOWN},
};