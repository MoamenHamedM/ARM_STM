
#include "MCAL/GPIO_DRIVER.h"
#include "HAL/SW.h"

const SW_cfg_t Switches[_SW_Num] =
    {
        [SW_ON] = {
            .Port = GPIO_PORT_B,
            .Pin = GPIO_PIN_0,
            .Direct = SW_PULL_UP}};