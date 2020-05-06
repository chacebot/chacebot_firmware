
/* Input PWM to Timer 4, 3, 2:
 * 
 *
 * PC13		LED
 * 
 */
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "mcuio.h"

static volatile uint32_t STEERING = 0, DRIVE = 0, ENABLE = 0;

void tim4_isr(void)
{
  uint32_t sr = TIM_SR(TIM4);

  if (sr & TIM_SR_CC2IF)
  {
    STEERING = TIM_CCR2(TIM4);
    timer_clear_flag(TIM4, TIM_SR_CC2IF);
  }
}

void tim3_isr(void)
{
  uint32_t sr = TIM_SR(TIM3);

  if (sr & TIM_SR_CC2IF)
  {
    DRIVE = TIM_CCR2(TIM3);
    timer_clear_flag(TIM3, TIM_SR_CC2IF);
  }
}

void tim2_isr(void)
{
  uint32_t sr = TIM_SR(TIM2);

  if (sr & TIM_SR_CC2IF)
  {
    ENABLE = TIM_CCR2(TIM2);
    timer_clear_flag(TIM3, TIM_SR_CC2IF);
  }
}

static void task1(void *args __attribute__((unused)))
{ //Steering Task

  rcc_periph_clock_enable(RCC_TIM4); // Need TIM4 clock

  // PB6 == TIM4.CH1
  rcc_periph_clock_enable(RCC_GPIOB);         // Need GPIOB clock
  gpio_set_mode(GPIOB, GPIO_MODE_INPUT,       // Input
                GPIO_CNF_INPUT_FLOAT, GPIO6); // PB6=TIM4.CH1

  // TIM4:
  timer_disable_counter(TIM4);
  rcc_periph_reset_pulse(RST_TIM4);
  nvic_set_priority(NVIC_DMA1_CHANNEL3_IRQ, 2);
  nvic_enable_irq(NVIC_TIM4_IRQ);
  timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM4, 72 - 1);

  timer_ic_set_input(TIM4, TIM_IC2, TIM_IC_IN_TI1);
  timer_ic_set_filter(TIM4, TIM_IC_IN_TI1, TIM_IC_CK_INT_N_2);

  timer_slave_set_mode(TIM4, TIM_SMCR_SMS_RM);
  timer_slave_set_trigger(TIM4, TIM_SMCR_TS_TI1FP1);
  TIM_CCER(TIM4) &= ~(TIM_CCER_CC2P | TIM_CCER_CC2E);
  TIM_CCER(TIM4) |= TIM_CCER_CC2P | TIM_CCER_CC2E;
  timer_ic_enable(TIM4, TIM_IC2);
  timer_enable_irq(TIM4, TIM_DIER_CC2IE);
  timer_enable_counter(TIM4);

  for (;;)
  {
    vTaskDelay(20);
  }
}

static void task2(void *args __attribute__((unused)))
{ // Drive task

  rcc_periph_clock_enable(RCC_TIM3); // Need TIM3 clock

  // PA6 == TIM3.CH1
  rcc_periph_clock_enable(RCC_GPIOA);         // Need GPIOB clock
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT,       // Input
                GPIO_CNF_INPUT_FLOAT, GPIO6); // PA6=TIM3.CH1

  // TIM3:
  timer_disable_counter(TIM3);
  rcc_periph_reset_pulse(RST_TIM3);

  nvic_set_priority(NVIC_DMA1_CHANNEL3_IRQ, 2);
  nvic_enable_irq(NVIC_TIM3_IRQ);

  timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM3, 72 - 1);

  timer_ic_set_input(TIM3, TIM_IC2, TIM_IC_IN_TI1);
  timer_ic_set_filter(TIM3, TIM_IC_IN_TI1, TIM_IC_CK_INT_N_2);

  timer_slave_set_mode(TIM3, TIM_SMCR_SMS_RM);
  timer_slave_set_trigger(TIM3, TIM_SMCR_TS_TI1FP1);

  TIM_CCER(TIM3) &= ~(TIM_CCER_CC2P | TIM_CCER_CC2E);
  TIM_CCER(TIM3) |= TIM_CCER_CC2P | TIM_CCER_CC2E;

  timer_ic_enable(TIM3, TIM_IC2);
  timer_enable_irq(TIM3, TIM_DIER_CC2IE);
  timer_enable_counter(TIM3);

  for (;;)
  {
    vTaskDelay(20);
  }
}

static void task3(void *args __attribute__((unused)))
{ //Enable Task

  rcc_periph_clock_enable(RCC_TIM2); // Need TIM2 clock

  // PA0 == TIM2.CH1
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT,       // Input
                GPIO_CNF_INPUT_FLOAT, GPIO0); // PA0=TIM2.CH1

  // TIM2:
  timer_disable_counter(TIM2);
  rcc_periph_reset_pulse(RST_TIM2);
  nvic_set_priority(NVIC_DMA1_CHANNEL3_IRQ, 2);
  nvic_enable_irq(NVIC_TIM2_IRQ);
  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_set_prescaler(TIM2, 72 - 1);

  timer_ic_set_input(TIM2, TIM_IC2, TIM_IC_IN_TI1);
  timer_ic_set_filter(TIM2, TIM_IC_IN_TI1, TIM_IC_CK_INT_N_2);
  timer_slave_set_mode(TIM2, TIM_SMCR_SMS_RM);
  timer_slave_set_trigger(TIM2, TIM_SMCR_TS_TI1FP1);
  TIM_CCER(TIM2) &= ~(TIM_CCER_CC2P | TIM_CCER_CC2E);
  TIM_CCER(TIM2) |= TIM_CCER_CC2P | TIM_CCER_CC2E;
  timer_ic_enable(TIM2, TIM_IC2);
  timer_enable_irq(TIM2, TIM_DIER_CC2IE);
  timer_enable_counter(TIM2);
  for (;;)
  {
    vTaskDelay(20);
  }
}

static void task4(void *args __attribute__((unused)))
{ //Sending Task

  gpio_set(GPIOC, GPIO13); // LED on

  for (;;)
  {
    vTaskDelay(20);
    std_printf("{%u,%u,%u}\r\n", (unsigned)STEERING, (unsigned)DRIVE,
               (unsigned)ENABLE);
    if (ENABLE > 1500)
      gpio_clear(GPIOC, GPIO13); // LED on

    else if (ENABLE < 1500)
      gpio_set(GPIOC, GPIO13);
  }
}

int main(void)
{

  rcc_clock_setup_in_hse_8mhz_out_72mhz();

  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                GPIO13);
  gpio_clear(GPIOC, GPIO13); // LED off

  usb_start(1, 1);
  std_set_device(mcu_usb); // Use USB for std I/O

  xTaskCreate(task1, "steering", 100, NULL, 1, NULL);
  xTaskCreate(task2, "drive", 100, NULL, 1, NULL);
  xTaskCreate(task3, "status", 100, NULL, 1, NULL);
  xTaskCreate(task4, "send", 100, NULL, 1, NULL);
  vTaskStartScheduler();
  for (;;)
    ;
  return 0;
}

// End main.c