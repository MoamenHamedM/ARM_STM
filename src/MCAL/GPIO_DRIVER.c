/*
 * GPIO_DRIVER.c
 *
 *  Created on: Feb 13, 2024
 *      Author: Moamen_Hamed
 */

#include "MCAL/GPIO_DRIVER.h"

#define GPIO_CLEAR_MASK 0x00000003
#define GPIO_MODE_MASK 0x00000003
#define GPIO_TYPE_MASK 0x00000004
#define GPIO_UP_DN_MASK 0x00000018
#define GPIO_TYPE_SHIFT 0x00000002
#define GPIO_UP_DN_SHIFT 0x00000003
#define GPIO_REG_SIZE 0x00000010
#define GPIO_AFR_CLEAR_MASK 0x0000000F
#define GPIO_1BIT_OFFSET 0x00000001
#define GPIO_2BIT_OFFSET 0x00000002
#define GPIO_4BIT_OFFSET 0x00000004

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

Error_Status GPIO_Init(GPIO_Pin_t *GPIO_Element)
{
  Error_Status LOC_Status = Status_NOK;
  if (GPIO_Element == NULL)
  {
    LOC_Status = Status_Null_Pointer;
  }
  else if ((GPIO_Element->Mode > GPIO_MODE_AF_OD_PD) || (GPIO_Element->Pin > GPIO_PIN_15))
  {
    LOC_Status = Status_Invalid_Input;
  }
  else if ((GPIO_Element->Port > GPIO_PORT_C) || (GPIO_Element->Speed > GPIO_SPEED_VHIGH))
  {
    LOC_Status = Status_Invalid_Input;
  }
  else
  {
    LOC_Status = Status_OK;

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
  return LOC_Status;
}

Error_Status GPIO_Set_PinValue(void *GPIO_Port, u32_t GPIO_Pin, u32_t GPIO_State)
{
  Error_Status LOC_Status = Status_NOK;
  if (GPIO_Port == NULL)
  {
    LOC_Status = Status_Null_Pointer;
  }
  else if (GPIO_Pin > GPIO_PIN_15 || GPIO_State > GPIO_STATE_SET)
  {
    LOC_Status = Status_Invalid_Input;
  }
  else
  {
    LOC_Status = Status_OK;
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
  return LOC_Status;
}

Error_Status GPIO_Get_PinValue(void *GPIO_Port, u32_t GPIO_Pin, u8_t *GPIO_Pin_State)
{
  Error_Status LOC_Status = Status_NOK;

  if (GPIO_Port == NULL || GPIO_Pin_State == NULL)
  {
    LOC_Status = Status_Null_Pointer;
  }
  else if (GPIO_Pin > GPIO_PIN_15)
  {
    LOC_Status = Status_Invalid_Input;
  }
  else
  {
    LOC_Status = Status_OK;
    *GPIO_Pin_State = (((GPIO_Port_t *)GPIO_Port)->IDR >> GPIO_Pin) & 1;
  }

  return LOC_Status;
}

Error_Status GPIO_CFG_AlternateFunction(void *GPIO_Port, u32_t GPIO_Pin, u32_t GPIO_Func)
{
  Error_Status LOC_Status = Status_NOK;

  if (GPIO_Port == NULL)
  {
    LOC_Status = Status_Null_Pointer;
  }
  else if (GPIO_Pin > GPIO_PIN_15 || GPIO_Func > GPIO_FUNC_AF15)
  {
    LOC_Status = Status_Invalid_Input;
  }
  else
  {
    LOC_Status = Status_OK;
    u32_t LOC_AFR_Value = 0;

    if (GPIO_Pin > GPIO_PIN_7)
    {
      LOC_AFR_Value = (((GPIO_Port_t *)GPIO_Port)->AFRH);
      LOC_AFR_Value &= ~(GPIO_AFR_CLEAR_MASK << (GPIO_Pin * GPIO_4BIT_OFFSET));
      LOC_AFR_Value |= (GPIO_Func << (GPIO_Pin * GPIO_4BIT_OFFSET));
      (((GPIO_Port_t *)GPIO_Port)->AFRH) = LOC_AFR_Value;
    }
    else
    {
      LOC_AFR_Value = (((GPIO_Port_t *)GPIO_Port)->AFRL);
      LOC_AFR_Value &= ~(GPIO_AFR_CLEAR_MASK << (GPIO_Pin * GPIO_4BIT_OFFSET));
      LOC_AFR_Value |= (GPIO_Func << (GPIO_Pin * GPIO_4BIT_OFFSET));
      (((GPIO_Port_t *)GPIO_Port)->AFRL) = LOC_AFR_Value;
    }
  }
  return LOC_Status;
}