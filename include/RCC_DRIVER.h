/*
 * RCC_DRIVER.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Moamen_Hamed
 */

#ifndef RCC_DRIVER_H_
#define RCC_DRIVER_H_
#include "STD_Types.h"

/***************Macros for all clock operations***************/
#define CLOCK_HSI_CTRL 0x00000001
#define CLOCK_HSE_CTRL 0x00010000
#define CLOCK_PLL_CTRL 0x01000000
#define CLOCK_HSI_STATUS 0x00000002
#define CLOCK_HSE_STATUS 0x00020000
#define CLOCK_PLL_STATUS 0x02000000
#define SYS_ClOCK_HSI 0x00000000
#define SYS_ClOCK_HSE 0x00000001
#define SYS_ClOCK_PLL 0x00000002
#define PLL_SOURCE_HSI 0x00000000
#define PLL_SOURCE_HSE 0x00400000

/***************Macros for AHB1 peripherals***************/
#define AHB1_PERI_DMA1 0x00200000
#define AHB1_PERI_DMA2 0x00400000
#define AHB1_PERI_GPIOA 0x00000001
#define AHB1_PERI_GPIOB 0x00000002
#define AHB1_PERI_GPIOC 0x00000004
#define AHB1_PERI_GPIOD 0x00000008
#define AHB1_PERI_GPIOE 0x00000010
#define AHB1_PERI_GPIOH 0x00000080
#define AHB1_PERI_CRC 0x00001000

/***************Macros for AHB2 peripherals***************/
#define AHB2_PERI_OTG 0x00000080

/***************Macros for APB1 peripherals***************/
#define APB1_PERI_TIM2 0x00000001
#define APB1_PERI_TIM3 0x00000002
#define APB1_PERI_TIM4 0x00000004
#define APB1_PERI_TIM5 0x00000008
#define APB1_PERI_WWDG 0x00000800
#define APB1_PERI_SPI2 0x00004000
#define APB1_PERI_SPI3 0x00008000
#define APB1_PERI_USART2 0x00020000
#define APB1_PERI_I2C1 0x00200000
#define APB1_PERI_I2C2 0x00400000
#define APB1_PERI_I2C3 0x00800000
#define APB1_PERI_PWR 0x10000000

/***************Macros for APB2 peripherals***************/
#define APB2_PERI_TIM1 0x00000001
#define APB2_PERI_USART1 0x00000010
#define APB2_PERI_USART6 0x00000020
#define APB2_PERI_ADC1 0x00000100
#define APB2_PERI_SDIO 0x00000800
#define APB2_PERI_SPI1 0x00001000
#define APB2_PERI_SPI4 0x00002000
#define APB2_PERI_SYSCF 0x00004000
#define APB2_PERI_TIM9 0x00010000
#define APB2_PERI_TIM10 0x00020000
#define APB2_PERI_TIM11 0x00040000

/***************Macros for AHP prescaler***************/
#define AHP_PRE_1 0x00000000
#define AHP_PRE_2 0x00000080
#define AHP_PRE_4 0x00000090
#define AHP_PRE_8 0x000000A0
#define AHP_PRE_16 0x000000B0
#define AHP_PRE_64 0x000000C0
#define AHP_PRE_128 0x000000D0
#define AHP_PRE_256 0x000000E0
#define AHP_PRE_512 0x000000F0

/***************Macros for APB prescaler***************/
#define APB_PRE_1 0x00000000
#define APB_PRE_2 0x00000004
#define APB_PRE_4 0x00000005
#define APB_PRE_8 0x00000006
#define APB_PRE_16 0x00000007

/*
 * Function to switch on any clock in the system
 * parameters:
 * Clock  -> CLOCK_HSI_CTRL, CLOCK_HSE_CTRL, CLOCK_PLL_CTRL
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_ClockON(u32_t Clock);

/*
 * Function to switch off any clock in the system
 * parameters:
 * Clock  -> CLOCK_HSI_CTRL, CLOCK_HSE_CTRL, CLOCK_PLL_CTRL
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_ClockOFF(u32_t Clock);

/*
 * Function to check if any clock in the system is ready or not
 * parameters:
 * Clock  -> CLOCK_HSI_STATUS, CLOCK_HSE_STATUS, CLOCK_PLL_STATUS
 */
u32_t RCC_Read_ClockStatus(u32_t Clock);

/*
 * Function to select a clock in the system to be the system clock
 * parameters:
 * Clock  -> CLOCK_HSI, CLOCK_HSE, CLOCK_PLL
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_SEL_SystemCLK(u32_t SYS_Clock);

/*
 * Function to select the clock source for PLL clock
 * parameters:
 * PLL_Source  -> PLL_SOURCE_HSI, PLL_SOURCE_HSE
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_SEL_PLLSource(u32_t PLL_Source);

/*
 * Function to configure the PLL clock parameters in the system
 * parameters:
 * PLL_N  -> the values should be within the limits 192 ≤ PLL_N ≤ 432
 * ensure that the VCO output frequency is between 192 and 432 MHz.
 * PLL_M  -> the values should be within the limits 2 ≤ PLL_M ≤ 63
 * ensure that the VCO input frequency ranges from 1 to 2 MHz.
 * PLL_P  -> the values should be on of the following: 2, 4, 6, 8
 * ensure not to exceed 84 MHz on this domain.
 * PLL_Q  -> the values should be within the limits 2 ≤ PLL_Q ≤ 15
 * The USB OTG FS requires a 48 MHz clock to work correctly.
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CFG_PLLClock(u32_t PLL_N, u32_t PLL_M, u32_t PLL_P, u32_t PLL_Q);

/*
 * Function to enable the Peripherals in the AHB1 register
 * parameters:
 * AHB1_Peri -> AHB1_PERI_DMA1, AHB1_PERI_DMA2, AHB1_PERI_GPIOA,
 * AHB1_PERI_GPIOB, AHB1_PERI_GPIOC, AHB1_PERI_GPIOD, AHB1_PERI_GPIOE,
 * AHB1_PERI_GPIOH, AHB1_PERI_CRC
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_AHB1_PeriEnable(u32_t AHB1_Peri);

/*
 * Function to disable the Peripherals in the AHB1 register
 * parameters:
 * AHB1_Peri -> AHB1_PERI_DMA1, AHB1_PERI_DMA2, AHB1_PERI_GPIOA,
 * AHB1_PERI_GPIOB, AHB1_PERI_GPIOC, AHB1_PERI_GPIOD, AHB1_PERI_GPIOE,
 * AHB1_PERI_GPIOH, AHB1_PERI_CRC
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_AHB1_PeriDisable(u32_t AHB1_Peri);

/*
 * Function to enable the Peripherals in the AHB2 register
 * parameters:
 * AHB2_Peri -> AHB2_PERI_OTG
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_AHB2_PeriEnable(u32_t AHB2_Peri);

/*
 * Function to disable the Peripherals in the AHB2 register
 * parameters:
 * AHB2_Peri -> AHB2_PERI_OTG
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_AHB2_PeriDisable(u32_t AHB2_Peri);

/*
 * Function to enable the Peripherals in the APB1 register
 * parameters:
 * APB1_Peri -> APB1_PERI_TIM2, APB1_PERI_TIM3, APB1_PERI_TIM4,
 * APB1_PERI_TIM5, APB1_PERI_WWDG, APB1_PERI_SPI2, APB1_PERI_SPI3,
 * APB1_PERI_USART2, APB1_PERI_I2C1, APB1_PERI_I2C2, APB1_PERI_I2C3, APB1_PERI_PWR
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_APB1_PeriEnable(u32_t APB1_Peri);

/*
 * Function to disable the Peripherals in the APB1 register
 * parameters:
 * APB1_Peri -> APB1_PERI_TIM2, APB1_PERI_TIM3, APB1_PERI_TIM4,
 * APB1_PERI_TIM5, APB1_PERI_WWDG, APB1_PERI_SPI2, APB1_PERI_SPI3,
 * APB1_PERI_USART2, APB1_PERI_I2C1, APB1_PERI_I2C2, APB1_PERI_I2C3, APB1_PERI_PWR
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_APB1_PeriDisable(u32_t APB1_Peri);

/*
 * Function to enable the Peripherals in the APB2 register
 * parameters:
 * APB2_Peri -> APB2_PERI_TIM1, APB2_PERI_USART1, APB2_PERI_USART6,
 * APB2_PERI_ADC1, APB2_PERI_SDIO, APB2_PERI_SPI1, APB2_PERI_SPI4,
 * APB2_PERI_SYSCF, APB2_PERI_TIM9, APB2_PERI_TIM10, APB2_PERI_TIM11
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_APB2_PeriEnable(u32_t APB2_Peri);

/*
 * Function to disable the Peripherals in the APB2 register
 * parameters:
 * APB2_Peri -> APB2_PERI_TIM1, APB2_PERI_USART1, APB2_PERI_USART6,
 * APB2_PERI_ADC1, APB2_PERI_SDIO, APB2_PERI_SPI1, APB2_PERI_SPI4,
 * APB2_PERI_SYSCF, APB2_PERI_TIM9, APB2_PERI_TIM10, APB2_PERI_TIM11
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_APB2_PeriDisable(u32_t APB2_Peri);

/*
 * Function to configure the AHB Prescaler in the system
 * parameters:
 * AHP_Pre -> AHP_PRE_1, AHP_PRE_2, AHP_PRE_4,
 * AHP_PRE_8, AHP_PRE_16, AHP_PRE_64,
 * AHP_PRE_128, AHP_PRE_256, AHP_PRE_512
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CFG_AHP_Prescaler(u32_t AHP_Pre);

/*
 * Function to configure the APB high speed Prescaler in the system
 * parameters:
 * APB_Pre  -> APB_PRE_1, APB_PRE_2, APB_PRE_4, APB_PRE_8, APB_PRE_16
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CFG_APB_HighSpeed_Prescaler(u32_t APB_Pre);

/*
 * Function to configure the APB high speed Prescaler in the system
 * parameters:
 * APB_Pre  -> APB_PRE_1, APB_PRE_2, APB_PRE_4, APB_PRE_8, APB_PRE_16
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CFG_APB_LowSpeed_Prescaler(u32_t APB_Pre);

#endif /* RCC_DRIVER_H_ */
