
#include "RCC_DRIVER.h"
#include "GPIO_DRIVER.h"
#include "LED.h"
#include "SW.h"

#define TEST_RCC 0
#define TEST_GPIO 1
#define TEST_LED_SW 2
#define APP TEST_LED_SW

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

  RCC_CTRL_Peripheral_Enable(RCC_PERI_AHB1_DMA1);
  RCC_CTRL_Peripheral_Disable(RCC_PERI_AHB1_DMA1);

  RCC_CFG_AHP_Prescaler(AHP_PRE_2);
  RCC_CFG_APB_HighSpeed_Prescaler(APB_PRE_2);

  while (LOC_Result == RESULT_NOK)
  {
    LOC_Result = RCC_Read_ClockStatus(CLOCK_PLL_STATUS);
  }
#endif

#if APP == TEST_GPIO

  RCC_CTRL_Peripheral_Enable(RCC_PERI_AHB1_GPIOA);

  GPIO_Pin_t Led = {.Pin = GPIO_PIN_1, .Port = GPIO_PORT_A, .Mode = GPIO_MODE_OP_PP_PU, .Speed = GPIO_SPEED_MED};
  GPIO_Init(&Led);

  GPIO_Set_PinValue(Led.Port, Led.Pin, GPIO_STATE_SET);
  GPIO_Set_PinValue(Led.Port, Led.Pin, GPIO_STATE_RESET);
  GPIO_Set_PinValue(GPIO_PORT_A, GPIO_PIN_1, GPIO_STATE_SET);
  GPIO_Set_PinValue(GPIO_PORT_A, GPIO_PIN_1, GPIO_STATE_RESET);
  GPIO_Set_PinValue(Led.Port, Led.Pin, GPIO_STATE_SET);

#endif

#if APP == TEST_LED_SW

  u8_t flag = 0;
  u8_t SW_State = SW_STATE_NOT_PRESSED;

  RCC_CTRL_Peripheral_Enable(RCC_PERI_AHB1_GPIOA);
  RCC_CTRL_Peripheral_Enable(RCC_PERI_AHB1_GPIOB);

  LED_Init();
  SW_Init();

  // Infinite loop
  while (1)
  {
    while (SW_State == SW_STATE_NOT_PRESSED)
    {
      SW_GetState(SW_ON, &SW_State);
    }

    if (flag == 0)
    {
      LED_SetState(LED_Alarm, LED_STATE_ON);
      flag = 1;
    }
    else if (flag == 1)
    {
      LED_SetState(LED_Alarm, LED_STATE_OFF);
      flag = 0;
    }

    while (SW_State == SW_STATE_PRESSED)
    {
      SW_GetState(SW_ON, &SW_State);
    }
  }

#endif

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
