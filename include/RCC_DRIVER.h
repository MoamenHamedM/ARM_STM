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
#define RCC_PERI_AHB1_DMA1 0x00200000
#define RCC_PERI_AHB1_DMA2 0x00400000
#define RCC_PERI_AHB1_GPIOA 0x00000001
#define RCC_PERI_AHB1_GPIOB 0x00000002
#define RCC_PERI_AHB1_GPIOC 0x00000004
#define RCC_PERI_AHB1_GPIOD 0x00000008
#define RCC_PERI_AHB1_GPIOE 0x00000010
#define RCC_PERI_AHB1_GPIOH 0x00000080
#define RCC_PERI_AHB1_CRC 0x00001000

/***************Macros for AHB2 peripherals***************/
#define RCC_PERI_AHB2_OTG 0x01200080

/***************Macros for APB2 peripherals***************/
#define RCC_PERI_APB1_TIM2 0x02000001
#define RCC_PERI_APB1_TIM3 0x02000002
#define RCC_PERI_APB1_TIM4 0x02000004
#define RCC_PERI_APB1_TIM5 0x02000008
#define RCC_PERI_APB1_WWDG 0x02000800
#define RCC_PERI_APB1_SPI2 0x02004000
#define RCC_PERI_APB1_SPI3 0x02008000
#define RCC_PERI_APB1_USART2 0x02020000
#define RCC_PERI_APB1_I2C1 0x02200000
#define RCC_PERI_APB1_I2C2 0x02400000
#define RCC_PERI_APB1_I2C3 0x02800000
#define RCC_PERI_APB1_PWR 0x12000000

/***************Macros for APB2 peripherals***************/
#define RCC_PERI_APB2_TIM1 0x03000001
#define RCC_PERI_APB2_USART1 0x03000010
#define RCC_PERI_APB2_USART6 0x03000020
#define RCC_PERI_APB2_ADC1 0x03000100
#define RCC_PERI_APB2_SDIO 0x03000800
#define RCC_PERI_APB2_SPI1 0x03001000
#define RCC_PERI_APB2_SPI4 0x03002000
#define RCC_PERI_APB2_SYSCF 0x03004000
#define RCC_PERI_APB2_TIM9 0x03010000
#define RCC_PERI_APB2_TIM10 0x03020000
#define RCC_PERI_APB2_TIM11 0x03040000

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
 * return:
 * Status_NOK, Status_OK
 */
Error_Status RCC_Read_ClockStatus(u32_t Clock);

/*
 * Function to select a clock in the system to be the system clock
 * - make sure to read the status of the desired clock -
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
 * Function to enable any Peripheral in the System
 * parameters:
 * RCC_Peri -> any provided preipheral mask
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_Peripheral_Enable(u32_t RCC_Peri);

/*
 * Function to enable any Peripheral in the System
 * parameters:
 * RCC_Peri -> any provided preipheral mask
 * return:
 * Status_NOK, Status_OK, Status_Null_Pointer, Status_Invalid_Input
 */
Error_Status RCC_CTRL_Peripheral_Disable(u32_t RCC_Peri);

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
