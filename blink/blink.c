#include "stm32f3xx.h"

#define FLASH_LATENCY	(2 << FLASH_ACR_LATENCY_Pos)
#define PLL_MUL		RCC_CFGR_PLLMUL9


#ifndef __NO_SYSTEM_INIT
void SystemInit()
{}
#endif

void main()
{
  volatile uint32_t cnt;

  /* Set Flash latency. */
  FLASH->ACR = FLASH->ACR & ~FLASH_ACR_LATENCY | FLASH_LATENCY;

  /* Configure PLL and select as sysclk source. */
  RCC->CFGR = RCC->CFGR & ~RCC_CFGR_PLLMUL | RCC_CFGR_PLLMUL16;
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY));
  RCC->CFGR = RCC->CFGR & ~RCC_CFGR_SW | RCC_CFGR_SW_PLL;

  RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

  GPIOB->MODER = GPIOB->MODER & ~GPIO_MODER_MODER3 | GPIO_MODER_MODER3_0;

  while (1) {
    GPIOB->BSRR = 0x08;
    cnt = 900000;
    while (cnt--);
    GPIOB->BRR = 0x08;
    cnt = 5000000;
    while (cnt--);
  }
}
