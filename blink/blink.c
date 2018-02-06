#include "stm32f3xx.h"

#define HSI_VALUE	8000000
#define FLASH_LATENCY	(2 << FLASH_ACR_LATENCY_Pos)
#define PLLMUL		16

uint32_t SystemCoreClock;


#ifndef __NO_SYSTEM_INIT
void SystemInit()
{
  /* Set Flash latency. */
  FLASH->ACR = FLASH->ACR & ~FLASH_ACR_LATENCY | FLASH_LATENCY;

  /* Configure PLL and select as sysclk source. */
  RCC->CFGR = RCC->CFGR & ~RCC_CFGR_PLLMUL |
    ((PLLMUL - 1) << RCC_CFGR_PLLMUL_Pos);
  RCC->CR |= RCC_CR_PLLON;
  while (!(RCC->CR & RCC_CR_PLLRDY));
  RCC->CFGR = RCC->CFGR & ~RCC_CFGR_SW | RCC_CFGR_SW_PLL;

  SystemCoreClock = HSI_VALUE / 2 * PLLMUL;
}
#endif


void main()
{
  volatile uint32_t cnt;

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
