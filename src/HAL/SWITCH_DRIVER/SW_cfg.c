
#include "MCAL/GPIO_DRIVER.h"
#include "HAL/SW.h"

const SW_cfg_t Switches[_SW_Num] =
    {
        [SW_ONE] = {
            .Port = GPIO_PORT_A,
            .Pin = GPIO_PIN_1,
            .Direct = SW_PULL_UP}};