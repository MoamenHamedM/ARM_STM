#ifndef STM32F401CC_IRQ_
#define STM32F401CC_IRQ_

#define IMPLEMENTED_PRIORITY_BITS 4

typedef enum
{
    NVIC_IRQ_WWDG,
    NVIC_IRQ_EXTI16,
    NVIC_IRQ_EXTI21,
    NVIC_IRQ_EXTI22,
    NVIC_IRQ_FLASH,
    NVIC_IRQ_RCC,
    NVIC_IRQ_EXTI0,
    NVIC_IRQ_EXTI1,
    NVIC_IRQ_EXTI2,
    NVIC_IRQ_EXTI3,
    NVIC_IRQ_EXTI4,
    NVIC_IRQ_DMA1_STREAM0,
    NVIC_IRQ_DMA1_STREAM1,
    NVIC_IRQ_DMA1_STREAM2,
    NVIC_IRQ_DMA1_STREAM3,
    NVIC_IRQ_DMA1_STREAM4,
    NVIC_IRQ_DMA1_STREAM5,
    NVIC_IRQ_DMA1_STREAM6,
    NVIC_IRQ_ADC,
    NVIC_IRQ_EXTI9,
    NVIC_IRQ_TIM1_BRK_TIM9,
    NVIC_IRQ_TIM1_UP_TIM10,
    NVIC_IRQ_TIM1_TRG_COM_TIM11,
    NVIC_IRQ_TIM1_CC,
    NVIC_IRQ_TIM2,
    NVIC_IRQ_TIM3,
    NVIC_IRQ_TIM4,
    NVIC_IRQ_I2C1_EV,
    NVIC_IRQ_I2C1_ER,
    NVIC_IRQ_I2C2_EV,
    NVIC_IRQ_I2C2_ER,
    NVIC_IRQ_SPI1,
    NVIC_IRQ_SPI2,
    NVIC_IRQ_USART1,
    NVIC_IRQ_USART2,
    NVIC_IRQ_EXTI15_10,
    NVIC_IRQ_EXTI17,
    NVIC_IRQ_EXTI18,
    NVIC_IRQ_DMA1_STREAM7,
    NVIC_IRQ_SDIO,
    NVIC_IRQ_TIM5,
    NVIC_IRQ_SPI3,
    NVIC_IRQ_DMA2_STREAM0,
    NVIC_IRQ_DMA2_STREAM1,
    NVIC_IRQ_DMA2_STREAM2,
    NVIC_IRQ_DMA2_STREAM3,
    NVIC_IRQ_DMA2_STREAM4,
    NVIC_IRQ_OTG_FS,
    NVIC_IRQ_DMA2_STREAM5,
    NVIC_IRQ_DMA2_STREAM6,
    NVIC_IRQ_DMA2_STREAM7,
    NVIC_IRQ_USART6,
    NVIC_IRQ_I2C3_EV,
    NVIC_IRQ_I2C3_ER,
    NVIC_IRQ_FPU,
    NVIC_IRQ_SPI4,
    _NVIC_IRQ_NUM
} IRQ_ID_t;

#endif // STM32F401CC_IRQ_