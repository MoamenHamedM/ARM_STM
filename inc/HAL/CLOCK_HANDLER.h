#ifndef CLOCK_HANDLER_
#define CLOCK_HANDLER_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_Types.h";

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define CLK_HAND_PERI_DMA1 0x00200000
#define CLK_HAND_PERI_DMA2 0x00400000
#define CLK_HAND_PERI_GPIOA 0x00000001
#define CLK_HAND_PERI_GPIOB 0x00000002
#define CLK_HAND_PERI_GPIOC 0x00000004
#define CLK_HAND_PERI_GPIOD 0x00000008
#define CLK_HAND_PERI_GPIOE 0x00000010
#define CLK_HAND_PERI_GPIOH 0x00000080
#define CLK_HAND_PERI_CRC 0x00001000
#define CLK_HAND_PERI_OTG 0x01200080
#define CLK_HAND_PERI_TIM2 0x02000001
#define CLK_HAND_PERI_TIM3 0x02000002
#define CLK_HAND_PERI_TIM4 0x02000004
#define CLK_HAND_PERI_TIM5 0x02000008
#define CLK_HAND_PERI_WWDG 0x02000800
#define CLK_HAND_PERI_SPI2 0x02004000
#define CLK_HAND_PERI_SPI3 0x02008000
#define CLK_HAND_PERI_USART2 0x02020000
#define CLK_HAND_PERI_I2C1 0x02200000
#define CLK_HAND_PERI_I2C2 0x02400000
#define CLK_HAND_PERI_I2C3 0x02800000
#define CLK_HAND_PERI_PWR 0x12000000
#define CLK_HAND_PERI_TIM1 0x03000001
#define CLK_HAND_PERI_USART1 0x03000010
#define CLK_HAND_PERI_USART6 0x03000020
#define CLK_HAND_PERI_ADC1 0x03000100
#define CLK_HAND_PERI_SDIO 0x03000800
#define CLK_HAND_PERI_SPI1 0x03001000
#define CLK_HAND_PERI_SPI4 0x03002000
#define CLK_HAND_PERI_SYSCF 0x03004000
#define CLK_HAND_PERI_TIM9 0x03010000
#define CLK_HAND_PERI_TIM10 0x03020000
#define CLK_HAND_PERI_TIM11 0x03040000

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/*
 * Function to enable any Peripheral in the System
 * parameters:
 * CLK_HAND_Peri -> any provided preipheral mask
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status CLK_HAND_CTRL_PeriClockEnable(u32_t CLK_HAND_Peri);

/*
 * Function to disable any Peripheral in the System
 * parameters:
 * CLK_HAND_Peri -> any provided preipheral mask
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status CLK_HAND_CTRL_PeriClockDisable(u32_t CLK_HAND_Peri);

#endif // CLOCK_HANDLER_