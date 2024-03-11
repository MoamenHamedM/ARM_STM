/*
 * RCC_DRIVER.c
 *
 *  Created on: Feb 5, 2024
 *      Author: Moamen_Hamed
 */

#include "MCAL/RCC_DRIVER.h"

#define RCC_BASE_ADDRESS 0x40023800
#define CFGR_SYSCLOCK_CLEARMASK 0x00000003
#define CFGR_AHBPRE_CLEARMASK 0x000000F0
#define CFGR_APBHPRE_CLEARMASK 0x0000E000
#define CFGR_APBSPRE_CLEARMASK 0x00001C00
#define PLLCFGR_PLLCFG_CLEARMASK 0x0F037FFF
#define PLLCFGR_PLLSRC_CLEARMASK 0x00400000
#define APB_HIGHSPEED_OFFSET 0x0000000D
#define APB_LOWSPEED_OFFSET 0x0000000A
#define PLL_M_OFFSET 0x00000000
#define PLL_N_OFFSET 0x00000006
#define PLL_P_OFFSET 0x00000010
#define PLL_Q_OFFSET 0x00000018
#define RCC_PERI_MASK 0x03000000
#define RCC_REG_AHB1_MASK 0x00000000
#define RCC_REG_AHB2_MASK 0x01000000
#define RCC_REG_APB1_MASK 0x02000000
#define RCC_REG_APB2_MASK 0x03000000

typedef struct
{
  volatile u32_t CR;
  volatile u32_t PLLCFGR;
  volatile u32_t CFGR;
  volatile u32_t CIR;
  volatile u32_t AHB1RSTR;
  volatile u32_t AHB2RSTR;
  volatile u32_t Reserved1;
  volatile u32_t Reserved2;
  volatile u32_t APB1RSTR;
  volatile u32_t APB2RSTR;
  volatile u32_t Reserved3;
  volatile u32_t Reserved4;
  volatile u32_t AHB1ENR;
  volatile u32_t AHB2ENR;
  volatile u32_t Reserved5;
  volatile u32_t Reserved6;
  volatile u32_t APB1ENR;
  volatile u32_t APB2ENR;
} RCC_PERI_t;

RCC_PERI_t *const RCC = (RCC_PERI_t *)RCC_BASE_ADDRESS;

u32_t SYSCLK_Freq = 0xF42400; // default frequency of the HSI clock 16,000,000 MHz

static Error_Status RCC_CTRL_AHB1_PeriEnable(u32_t AHB1_Peri);
static Error_Status RCC_CTRL_AHB1_PeriDisable(u32_t AHB1_Peri);
static Error_Status RCC_CTRL_AHB2_PeriEnable(u32_t AHB2_Peri);
static Error_Status RCC_CTRL_AHB2_PeriDisable(u32_t AHB2_Peri);
static Error_Status RCC_CTRL_APB1_PeriEnable(u32_t APB1_Peri);
static Error_Status RCC_CTRL_APB1_PeriDisable(u32_t APB1_Peri);
static Error_Status RCC_CTRL_APB2_PeriEnable(u32_t APB2_Peri);
static Error_Status RCC_CTRL_APB2_PeriDisable(u32_t APB2_Peri);

Error_Status RCC_CTRL_ClockON(u32_t Clock)
{
  Error_Status LOC_Result = Status_NOK;

  if (Clock > CLOCK_PLL_CTRL)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->CR |= Clock;
  }
  return LOC_Result;
}

Error_Status RCC_CTRL_ClockOFF(u32_t Clock)
{
  Error_Status LOC_Result = Status_NOK;

  if (Clock > CLOCK_PLL_CTRL)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->CR &= ~Clock;
  }
  return LOC_Result;
}

Error_Status RCC_Read_ClockStatus(u32_t Clock)
{
  Error_Status LOC_Result = Status_NOK;

  if (Clock > CLOCK_PLL_STATUS)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = (RCC->CR & (Clock));
  }
  return LOC_Result;
}

Error_Status RCC_SEL_SystemCLK(u32_t SYS_Clock)
{
  Error_Status LOC_Result = Status_NOK;
  u32_t LOC_CFGRValue = RCC->CFGR;

  if (SYS_Clock > SYS_ClOCK_PLL)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    LOC_CFGRValue &= ~CFGR_SYSCLOCK_CLEARMASK;
    LOC_CFGRValue |= SYS_Clock;
    RCC->CFGR = LOC_CFGRValue;
  }
  return LOC_Result;
}

Error_Status RCC_SEL_PLLSource(u32_t PLL_Source)
{
  Error_Status LOC_Result = Status_NOK;
  u32_t LOC_PLLCFGRValue = RCC->PLLCFGR;

  if (PLL_Source > PLL_SOURCE_HSE)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    LOC_PLLCFGRValue &= ~PLLCFGR_PLLSRC_CLEARMASK;
    LOC_PLLCFGRValue |= PLL_Source;
    RCC->PLLCFGR = LOC_PLLCFGRValue;
  }
  return LOC_Result;
}

Error_Status RCC_CFG_PLLClock(u32_t PLL_N, u32_t PLL_M, u32_t PLL_P, u32_t PLL_Q)
{
  Error_Status LOC_Result = Status_NOK;
  u32_t LOC_PLLCFGRValue = RCC->PLLCFGR;

  if (PLL_N > 432 || PLL_N < 192)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else if (PLL_M > 63 || PLL_M < 2)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else if (PLL_P > 8 || PLL_P < 2)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else if (PLL_Q > 15 || PLL_Q < 2)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else if ((RCC->CR & (CLOCK_PLL_STATUS)) == Status_OK)
  {
    LOC_Result = Status_NOK;
  }
  else
  {
    PLL_P = (PLL_P / 2) - 1;
    LOC_Result = Status_OK;
    LOC_PLLCFGRValue &= ~PLLCFGR_PLLCFG_CLEARMASK;
    LOC_PLLCFGRValue |= (PLL_M << PLL_M_OFFSET);
    LOC_PLLCFGRValue |= (PLL_N << PLL_N_OFFSET);
    LOC_PLLCFGRValue |= (PLL_P << PLL_P_OFFSET);
    LOC_PLLCFGRValue |= (PLL_Q << PLL_Q_OFFSET);
    RCC->PLLCFGR = LOC_PLLCFGRValue;
  }

  return LOC_Result;
}

Error_Status RCC_CTRL_Peripheral_Enable(u32_t RCC_Peri)
{
  Error_Status LOC_Result = Status_NOK;
  u32_t LOC_PeriSelect = RCC_Peri & RCC_PERI_MASK;

  switch (LOC_PeriSelect)
  {
  case RCC_REG_AHB1_MASK:
    LOC_Result = RCC_CTRL_AHB1_PeriEnable(RCC_Peri);
    break;
  case RCC_REG_AHB2_MASK:
    RCC_Peri &= ~RCC_PERI_MASK;
    LOC_Result = RCC_CTRL_AHB2_PeriEnable(RCC_Peri);
    break;
  case RCC_REG_APB1_MASK:
    RCC_Peri &= ~RCC_PERI_MASK;
    LOC_Result = RCC_CTRL_APB1_PeriEnable(RCC_Peri);
    break;
  case RCC_REG_APB2_MASK:
    RCC_Peri &= ~RCC_PERI_MASK;
    LOC_Result = RCC_CTRL_APB2_PeriEnable(RCC_Peri);
    break;
  default:
    /*Do Nothing*/
    break;
  }
  return LOC_Result;
}

Error_Status RCC_CTRL_Peripheral_Disable(u32_t RCC_Peri)
{
  Error_Status LOC_Result = Status_NOK;
  u32_t LOC_PeriSelect = RCC_Peri & RCC_PERI_MASK;

  switch (LOC_PeriSelect)
  {
  case RCC_REG_AHB1_MASK:
    LOC_Result = RCC_CTRL_AHB1_PeriDisable(RCC_Peri);
    break;
  case RCC_REG_AHB2_MASK:
    RCC_Peri &= ~RCC_PERI_MASK;
    LOC_Result = RCC_CTRL_AHB2_PeriDisable(RCC_Peri);
    break;
  case RCC_REG_APB1_MASK:
    RCC_Peri &= ~RCC_PERI_MASK;
    LOC_Result = RCC_CTRL_APB1_PeriDisable(RCC_Peri);
    break;
  case RCC_REG_APB2_MASK:
    RCC_Peri &= ~RCC_PERI_MASK;
    LOC_Result = RCC_CTRL_APB2_PeriDisable(RCC_Peri);
    break;
  default:
    /*Do Nothing*/
    break;
  }
  return LOC_Result;
}

static Error_Status RCC_CTRL_AHB1_PeriEnable(u32_t AHB1_Peri)
{
  Error_Status LOC_Result = Status_NOK;

  if (AHB1_Peri > RCC_PERI_AHB1_DMA2)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->AHB1ENR |= AHB1_Peri;
  }
  return LOC_Result;
}

static Error_Status RCC_CTRL_AHB1_PeriDisable(u32_t AHB1_Peri)
{
  Error_Status LOC_Result = Status_NOK;

  if (AHB1_Peri > RCC_PERI_AHB1_DMA2)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->AHB1ENR &= ~AHB1_Peri;
  }
  return LOC_Result;
}

static Error_Status RCC_CTRL_AHB2_PeriEnable(u32_t AHB2_Peri)
{
  Error_Status LOC_Result = Status_NOK;

  if (AHB2_Peri != RCC_PERI_AHB2_OTG)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->AHB2ENR |= AHB2_Peri;
  }
  return LOC_Result;
}

static Error_Status RCC_CTRL_AHB2_PeriDisable(u32_t AHB2_Peri)
{
  Error_Status LOC_Result = Status_NOK;

  if (AHB2_Peri != RCC_PERI_AHB2_OTG)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->AHB2ENR &= ~AHB2_Peri;
  }
  return LOC_Result;
}

static Error_Status RCC_CTRL_APB1_PeriEnable(u32_t APB1_Peri)
{
  Error_Status LOC_Result = Status_NOK;

  if (APB1_Peri > RCC_PERI_APB1_PWR)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->APB1ENR |= APB1_Peri;
  }
  return LOC_Result;
}

static Error_Status RCC_CTRL_APB1_PeriDisable(u32_t APB1_Peri)
{
  Error_Status LOC_Result = Status_NOK;

  if (APB1_Peri > RCC_PERI_APB1_PWR)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->APB1ENR &= ~APB1_Peri;
  }
  return LOC_Result;
}

static Error_Status RCC_CTRL_APB2_PeriEnable(u32_t APB2_Peri)
{
  Error_Status LOC_Result = Status_NOK;

  if (APB2_Peri > RCC_PERI_APB2_TIM11)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->APB2ENR |= APB2_Peri;
  }
  return LOC_Result;
}

static Error_Status RCC_CTRL_APB2_PeriDisable(u32_t APB2_Peri)
{
  Error_Status LOC_Result = Status_NOK;

  if (APB2_Peri > RCC_PERI_APB2_TIM11)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    RCC->APB2ENR &= ~APB2_Peri;
  }
  return LOC_Result;
}

Error_Status RCC_CFG_AHP_Prescaler(u32_t AHP_Pre)
{
  Error_Status LOC_Result = Status_NOK;
  u32_t LOC_CFGRValue = RCC->CFGR;
  if (AHP_Pre > AHP_PRE_512)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    LOC_CFGRValue &= ~CFGR_AHBPRE_CLEARMASK;
    LOC_CFGRValue |= AHP_Pre;
    RCC->CFGR = LOC_CFGRValue;
  }
  return LOC_Result;
}

Error_Status RCC_CFG_APB_HighSpeed_Prescaler(u32_t APB_Pre)
{
  Error_Status LOC_Result = Status_NOK;
  u32_t LOC_CFGRValue = RCC->CFGR;
  if (APB_Pre > APB_PRE_16)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    LOC_CFGRValue &= ~CFGR_APBHPRE_CLEARMASK;
    LOC_CFGRValue |= (APB_Pre << APB_HIGHSPEED_OFFSET);
    RCC->CFGR = LOC_CFGRValue;
  }
  return LOC_Result;
}

Error_Status RCC_CFG_APB_LowSpeed_Prescaler(u32_t APB_Pre)
{
  Error_Status LOC_Result = Status_NOK;
  u32_t LOC_CFGRValue = RCC->CFGR;
  if (APB_Pre > APB_PRE_16)
  {
    LOC_Result = Status_Invalid_Input;
  }
  else
  {
    LOC_Result = Status_OK;
    LOC_CFGRValue &= ~CFGR_APBSPRE_CLEARMASK;
    LOC_CFGRValue |= (APB_Pre << APB_LOWSPEED_OFFSET);
    RCC->CFGR = LOC_CFGRValue;
  }
  return LOC_Result;
}
