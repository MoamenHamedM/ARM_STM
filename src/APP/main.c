
#include "RCC_DRIVER.h"
#include "GPIO_DRIVER.h"

#define TEST_RCC 0
#define TEST_GPIO 1
#define APP TEST_GPIO

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int main(int argc, char *argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.

#if APP == TEST_RCC

  u32_t LOC_Result = RESULT_NOK;
  u16_t LOC_TimeOut;
  RCC_CTRL_ClockOFF(CLOCK_PLL_CTRL);
  RCC_CFG_PLLClock(210, 10, 4, 7);
  RCC_CTRL_ClockON(CLOCK_PLL_CTRL);
  LOC_TimeOut = 2000;
  while (LOC_Result == RESULT_NOK && LOC_TimeOut)
  {
    LOC_Result = RCC_Read_ClockStatus(CLOCK_PLL_STATUS);
    LOC_TimeOut--;
  }
  RCC_SEL_SystemCLK(SYS_ClOCK_PLL);

  RCC_CTRL_AHB1_PeriEnable(AHB1_PERI_DMA1);
  RCC_CTRL_AHB1_PeriDisable(AHB1_PERI_DMA1);

  RCC_CFG_AHP_Prescaler(AHP_PRE_2);
  RCC_CFG_APB_HighSpeed_Prescaler(APB_PRE_2);

  while (LOC_Result == RESULT_NOK)
  {
    LOC_Result = RCC_Read_ClockStatus(CLOCK_PLL_STATUS);
  }
#endif

#if APP == TEST_GPIO

  RCC_CTRL_AHB1_PeriEnable(AHB1_PERI_GPIOA);

  GPIO_Pin_t Led = {.Pin = GPIO_PIN_1, .Port = GPIO_PORT_A, .Mode = GPIO_MODE_OP_PP_PU, .Speed = GPIO_SPEED_MED};
  GPIO_Init(&Led);

  GPIO_Set_PinValue(Led.Port, Led.Pin, GPIO_STATE_SET);
  GPIO_Set_PinValue(Led.Port, Led.Pin, GPIO_STATE_RESET);
  GPIO_Set_PinValue(GPIO_PORT_A, GPIO_PIN_1, GPIO_STATE_SET);
  GPIO_Set_PinValue(GPIO_PORT_A, GPIO_PIN_1, GPIO_STATE_RESET);
  GPIO_Set_PinValue(Led.Port, Led.Pin, GPIO_STATE_SET);

#endif

  // Infinite loop
  while (1)
  {
    // Add your code here.
  }
  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
