
#include "MCAL/GPIO_DRIVER.h"
#include "HAL/SW.h"

const SW_cfg_t Switches[_SW_Num] =
    {
        [SW_ON] = {
            .Port = GPIO_PORT_B,
            .Pin = GPIO_PIN_2,
            .Direct = SW_PULL_DOWN},
        [SW_OFF] = {.Port = GPIO_PORT_A, .Pin = GPIO_PIN_3, .Direct = SW_PULL_DOWN},
};