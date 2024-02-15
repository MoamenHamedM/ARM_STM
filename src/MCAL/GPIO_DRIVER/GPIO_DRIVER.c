/*
 * GPIO_DRIVER.c
 *
 *  Created on: Feb 13, 2024
 *      Author: Moamen_Hamed
 */

#include "GPIO_DRIVER.h"

#define GPIO_CLEAR_MASK 0x00000003
#define GPIO_MODE_MASK 0x00000003
#define GPIO_TYPE_MASK 0x00000004
#define GPIO_UP_DN_MASK 0x00000018
#define GPIO_TYPE_SHIFT 0x00000002
#define GPIO_UP_DN_SHIFT 0x00000003
#define GPIO_REG_SIZE 0x00000010
#define GPIO_1BIT_OFFSET 0x00000001
#define GPIO_2BIT_OFFSET 0x00000002

typedef struct
{
  volatile u32_t MODER;
  volatile u32_t OTYPER;
  volatile u32_t OSPEEDR;
  volatile u32_t PUPDR;
  volatile u32_t IDR;
  volatile u32_t ODR;
  volatile u32_t BSRR;
  volatile u32_t LCKR;
  volatile u32_t AFRL;
  volatile u32_t AFRH;
} GPIO_Port_t;

void GPIO_Init(GPIO_Pin_t *GPIO_Element)
{
  u32_t LOC_ModeValue = (GPIO_Element->Mode & GPIO_MODE_MASK);
  u32_t LOC_TypeValue = (GPIO_Element->Mode & GPIO_TYPE_MASK) >> GPIO_TYPE_SHIFT;
  u32_t LOC_PUDNValue = (GPIO_Element->Mode & GPIO_UP_DN_MASK) >> GPIO_UP_DN_SHIFT;

  u32_t LOC_MODER_Value = ((GPIO_Port_t *)GPIO_Element->Port)->MODER;
  u32_t LOC_OTYPER_Value = ((GPIO_Port_t *)GPIO_Element->Port)->OTYPER;
  u32_t LOC_OSPEEDR_Value = ((GPIO_Port_t *)GPIO_Element->Port)->OSPEEDR;
  u32_t LOC_PUPDR_Value = ((GPIO_Port_t *)GPIO_Element->Port)->PUPDR;

  LOC_MODER_Value &= ~(GPIO_CLEAR_MASK << (GPIO_2BIT_OFFSET * GPIO_Element->Pin));
  LOC_OTYPER_Value &= ~(GPIO_1BIT_OFFSET << (GPIO_Element->Pin));
  LOC_OSPEEDR_Value &= ~(GPIO_CLEAR_MASK << (GPIO_2BIT_OFFSET * GPIO_Element->Pin));
  LOC_PUPDR_Value &= ~(GPIO_CLEAR_MASK << (GPIO_2BIT_OFFSET * GPIO_Element->Pin));

  LOC_MODER_Value |= (LOC_ModeValue << (GPIO_2BIT_OFFSET * GPIO_Element->Pin));
  LOC_OTYPER_Value |= (LOC_TypeValue << (GPIO_Element->Pin));
  LOC_OSPEEDR_Value |= (GPIO_Element->Speed << (GPIO_2BIT_OFFSET * GPIO_Element->Pin));
  LOC_PUPDR_Value |= (LOC_PUDNValue << (GPIO_2BIT_OFFSET * GPIO_Element->Pin));

  ((GPIO_Port_t *)GPIO_Element->Port)->MODER = LOC_MODER_Value;
  ((GPIO_Port_t *)GPIO_Element->Port)->OTYPER = LOC_OTYPER_Value;
  ((GPIO_Port_t *)GPIO_Element->Port)->OSPEEDR = LOC_OSPEEDR_Value;
  ((GPIO_Port_t *)GPIO_Element->Port)->PUPDR = LOC_PUPDR_Value;
}

void GPIO_Set_PinValue(void *GPIO_Port, u32_t GPIO_Pin, u32_t GPIO_State)
{
  switch (GPIO_State)
  {
  case GPIO_STATE_SET:
    ((GPIO_Port_t *)GPIO_Port)->BSRR = 1 << GPIO_Pin;
    break;
  case GPIO_STATE_RESET:
    ((GPIO_Port_t *)GPIO_Port)->BSRR = 1 << (GPIO_Pin + GPIO_REG_SIZE);
    break;
  default:
    /*Do - Nothing*/
    break;
  }
}

u8_t GPIO_Get_PinValue(void *GPIO_Port, u32_t GPIO_Pin)
{
  u8_t LOC_Result;

  LOC_Result = (((GPIO_Port_t *)GPIO_Port)->IDR >> GPIO_Pin) & 1;

  return LOC_Result;
}
