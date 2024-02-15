/*
 * GPIO_DRIVER.h
 *
 *  Created on: Feb 13, 2024
 *      Author: Moamen_Hamed
 */

#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "STD_Types.h"

/********************Macros for the GPIO Ports********************/
#define GPIO_PORT_A (void *)0x40020000
#define GPIO_PORT_B (void *)0x40020400
#define GPIO_PORT_C (void *)0x40020800

/********************Macros for the GPIO pins********************/
#define GPIO_PIN_0 0x00000000
#define GPIO_PIN_1 0x00000001
#define GPIO_PIN_2 0x00000002
#define GPIO_PIN_3 0x00000003
#define GPIO_PIN_4 0x00000004
#define GPIO_PIN_5 0x00000005
#define GPIO_PIN_6 0x00000006
#define GPIO_PIN_7 0x00000007
#define GPIO_PIN_8 0x00000008
#define GPIO_PIN_9 0x00000009
#define GPIO_PIN_10 0x0000000A
#define GPIO_PIN_11 0x0000000B
#define GPIO_PIN_12 0x0000000C
#define GPIO_PIN_13 0x0000000D
#define GPIO_PIN_14 0x0000000E
#define GPIO_PIN_15 0x0000000F

/********************Macros for the GPIO pin modes********************/
#define GPIO_MODE_IN_FL 0x00000000
#define GPIO_MODE_IN_PU 0x00000008
#define GPIO_MODE_IN_PD 0x00000018

#define GPIO_MODE_OP_PP 0x00000001
#define GPIO_MODE_OP_PP_PU 0x00000009
#define GPIO_MODE_OP_PP_PD 0x00000011
#define GPIO_MODE_OP_OD 0x00000005
#define GPIO_MODE_OP_OD_PU 0x0000000D
#define GPIO_MODE_OP_OD_PD 0x00000015

#define GPIO_MODE_AF_PP 0x00000002
#define GPIO_MODE_AF_PP_PU 0x0000000A
#define GPIO_MODE_AF_PP_PD 0x00000012
#define GPIO_MODE_AF_OD 0x00000006
#define GPIO_MODE_AF_OD_PU 0x0000000E
#define GPIO_MODE_AF_OD_PD 0x00000016

#define GPIO_MODE_AN 0x00000003

/********************Macros for the GPIO pin speeds********************/
#define GPIO_SPEED_LOW 0x00000000
#define GPIO_SPEED_MED 0x00000001
#define GPIO_SPEED_HIGH 0x00000002
#define GPIO_SPEED_VHIGH 0x00000003

/********************Macros for the GPIO pin states********************/
#define GPIO_STATE_RESET 0x00000000
#define GPIO_STATE_SET 0x00000001

/********************struct to create an object of a GPIO pin********************/
typedef struct
{
  u32_t Pin;
  void *Port;
  u32_t Speed;
  u32_t Mode;
} GPIO_Pin_t;

/*
 * use this function to initialize a GPIO pin
 * parameters:
 * GPIO_Element -> the address of a struct of type GPIO_t
 * you should configure the Pin, Port, Speed, Mode parameters in
 * the struct
 */
void GPIO_Init(GPIO_Pin_t *GPIO_Element);

/*
 * use this function to set a value to a GPIO pin
 * parameters:
 * GPIO_Port -> GPIO_PORT_A, GPIO_PORT_B, GPIO_PORT_C
 * GPIO_Pin -> GPIO_PIN_0 : GPIO_PIN_15
 * GPIO_State -> GPIO_STATE_RESET, GPIO_STATE_SET
 */
void GPIO_Set_PinValue(void *GPIO_Port, u32_t GPIO_Pin, u32_t GPIO_State);

/*
 * use this function to get a value from a GPIO pin
 * parameters:
 * GPIO_Port -> GPIO_PORT_A, GPIO_PORT_B, GPIO_PORT_C
 * GPIO_Pin -> GPIO_PIN_0 : GPIO_PIN_15
 */
u8_t GPIO_Get_PinValue(void *GPIO_Port, u32_t GPIO_Pin);

#endif /* GPIO_DRIVER_H_ */
