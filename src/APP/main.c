
#include "MCAL/RCC_DRIVER.h"
#include "MCAL/GPIO_DRIVER.h"
#include "MCAL/NVIC_DRIVER.h"
#include "HAL/LED.h"
#include "HAL/SW.h"
#include "HAL/CLOCK_HANDLER.h"
#include "MCAL/SYSTICK_DRIVER.h"
#include "HAL/SCHEDULER.h"
#include "HAL/LCD_DRIVER.h"
#include "MCAL/USART_DRIVER.h"

#define TEST_RCC 0
#define TEST_GPIO 1
#define TEST_LED_SW 2
#define TEST_NVIC 3
#define TEST_SYSTICK 4
#define TEST_SCHD 5
#define TEST_LCD 6
#define TEST_USART 7
#define APP TEST_LCD

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void Runnable_LED_Toggle(void);
void LCD_Write();
void Led_On();

#if APP == TEST_NVIC
void delay_ms(u32_t ms)
{
  for (volatile u32_t i = 0; i < ms * 16000; ++i)
  {
  }
}
void EXTI0_IRQHandler(void)
{
  LED_SetState(LED_Toggle, LED_STATE_ON);
  // NVIC_CTRL_SetIRQPending(NVIC_IRQ_EXTI1);
  //  NVIC_CTRL_GenerateSWI(NVIC_IRQ_EXTI1);
  //  delay_ms(100);
}
void EXTI1_IRQHandler(void)
{
  // NVIC_CTRL_GenerateSWI(NVIC_IRQ_EXTI0);
  NVIC_CTRL_GenerateSWI(NVIC_IRQ_EXTI0);
  // LED_SetState(LED_Toggle, LED_STATE_OFF);
  // delay_ms(100);
}

#endif

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
  u8_t SW_State = 0;

  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOA);
  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOB);

  LED_Init();
  SW_Init();

  // Infinite loop
  while (1)
  {
    SW_GetState(SW_ON, &SW_State);

    while (SW_State == SW_STATE_NOT_PRESSED)
    {
      SW_GetState(SW_ON, &SW_State);
    }
    if (flag == 0)
    {
      LED_SetState(LED_Toggle, LED_STATE_ON);
      flag = 1;
    }
    else if (flag == 1)
    {
      LED_SetState(LED_Toggle, LED_STATE_OFF);
      flag = 0;
    }

    while (SW_State == SW_STATE_PRESSED)
    {
      SW_GetState(SW_ON, &SW_State);
    }
  }

#endif

#if APP == TEST_NVIC

  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOA);
  LED_Init();
  NVIC_CTRL_EnableIRQ(NVIC_IRQ_EXTI0);
  NVIC_CTRL_EnableIRQ(NVIC_IRQ_EXTI1);
  NVIC_CFG_SetSubGroupBits(SUBGROUPBIT_ONE);
  NVIC_CFG_SetPriority(NVIC_IRQ_EXTI0, 2, 1);
  NVIC_CFG_SetPriority(NVIC_IRQ_EXTI1, 1, 1);
  // NVIC_CTRL_SetIRQPending(NVIC_IRQ_EXTI1);
  NVIC_CTRL_GenerateSWI(NVIC_IRQ_EXTI0);

  while (1)
  {
  }
#endif
#if APP == TEST_SYSTICK
  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOA);
  LED_Init();
  SYSTICK_SET_CallBack(Runnable_LED_Toggle);
  SYSTICK_CTRL_Interrupt(SYSTICK_IRQ_ENABLE);
  SYSTICK_SET_CurrentVal(0);
  SYSTICK_CFG_CLKSource(SYSTICK_CLK_AHB);
  SYSTICK_SET_TimeTicksMs(300);
  SYSTICK_CTRL_StartTimer();
  while (1)
  {
    /* code */
  }

#endif
#if APP == TEST_SCHD
  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOA);
  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOB);
  LED_Init();
  SCH_CFG_SchedulerInit();
  SCH_CTRL_StartScheduler();

#endif

#if APP == TEST_LCD
  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOA);
  LED_Init();
  LCD_Init();
  SCH_CFG_SchedulerInit();
  LCD_Write();
  SCH_CTRL_StartScheduler();

#endif

#if APP == TEST_USART
  GPIO_Pin_t USART_Pins[2] = {[0] = {.Pin = GPIO_PIN_6, .Port = GPIO_PORT_B, .Mode = GPIO_MODE_AF_PP, .Speed = GPIO_SPEED_VHIGH},
                              [1] = {.Pin = GPIO_PIN_7, .Port = GPIO_PORT_B, .Mode = GPIO_MODE_AF_PP, .Speed = GPIO_SPEED_VHIGH}};

  USART_Req_t USARAT_Byte = {.length = 1, .buffer = (u8_t)'A', .USART_Peri = USART_Peri_1, .CB = Led_On};

  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOA);
  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_GPIOB);
  CLK_HAND_CTRL_PeriClockEnable(CLK_HAND_PERI_USART1);
  NVIC_CTRL_EnableIRQ(NVIC_IRQ_USART1);
  USART_Init();
  GPIO_Init(USART_Pins, 2);
  LED_Init();
  GPIO_CFG_AlternateFunction(USART_Pins[0].Port, USART_Pins[0].Pin, GPIO_FUNC_AF7);
  GPIO_CFG_AlternateFunction(USART_Pins[1].Port, USART_Pins[1].Pin, GPIO_FUNC_AF7);
  USART_SendByte(USARAT_Byte);
  USART_TXBufferAsyncZC(USARAT_Byte);
  while (1)
    ;
#endif

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

void Led_On()
{
  LED_SetState(LED_Toggle, LED_STATE_ON);
}

void LCD_Write()
{
  LCD_WriteStringAsync((u8_t *)"hello", 5, NULL);
  LCD_SetCursorPositionAsync(0, 6, NULL);
  // LCD_WriteStringAsync("hello", 5, NULL);
  // LCD_SetCursorPositionAsync(1, 6, NULL);
  LCD_WriteStringAsync((u8_t *)"hello", 5, Led_On);
}