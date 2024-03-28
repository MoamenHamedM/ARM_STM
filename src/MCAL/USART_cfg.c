
#include "MCAL/USART_DRIVER.h"

const USART_cfg_t USART_CFG[_USART_Num] =
    {
        [USART_1] = {
            .address = USART_Peri_1,
            .BaudRate = 9600,
            .WordLength = USART_WORD_LENGTH_8,
            .ParityControl = USART_PARITY_DISABLE,
            .ParitySelect = USART_PARITY_DISABLE,
            .StopBits = USART_STOP_BITS_1,
            .OverSampling = USART_OVERSAMPLING_16}};