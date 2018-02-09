
#include "stm32f10x.h"
               
#define LED_NUM                      (1)
volatile uint32_t msTicks;                                 // counts 1ms timeTicks
/*----------------------------------------------------------------------------
 * SysTick_Handler:
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}
/*----------------------------------------------------------------------------
 * Delay: delays a number of Systicks
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;
  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}
/*----------------------------------------------------------------------------
 * SystemCoreClockConfigure: configure SystemCoreClock using HSI
                             (HSE is not populated on Nucleo board)
 *----------------------------------------------------------------------------*/
void SystemCoreClockConfigure(void) {
  RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready
  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock
  FLASH->ACR  = FLASH_ACR_PRFTBE;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_LATENCY;                         // Flash 1 wait state
  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;                        // APB1 = HCLK/2
  RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;                        // APB2 = HCLK
  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL
  //  PLL configuration:  = HSI/2 * 12 = 48 MHz
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
  //RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL12);
  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready
  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
}
int32_t Buttons_Initialize (void) {
  RCC->APB2ENR |= (1ul << 4);                /* Enable GPIOC clock            */
  GPIOC->CRH &= ~(15ul << 4*(13-8));
  GPIOC->CRH |=  ( 4ul << 4*(13-8));
  return (0);
}
int32_t Buttons_Uninitialize (void) {
  GPIOC->CRH &= ~(15ul << 4*(13-8));
  return (0);
}
uint32_t Buttons_GetState (void) {
  uint32_t val = 0;
  if ((GPIOC->IDR & (1ul << 13)) == 0) {
    /* USER button */
    val |= 1;
  }
  return (val);
}
/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
uint32_t Buttons_GetCount (void) {
  return 1;
}
int32_t LED_Initialize (void) {
  RCC->APB2ENR |= (1UL << 2);                /* Enable GPIOA clock            */
  /* Configure LED (PA.5) pins as output */
  GPIOA->CRL   &= ~((15ul << 4*5));
  GPIOA->CRL   |=  (( 1ul << 4*5));
  return (0);
}
int32_t LED_Uninitialize (void) {
  return (0);
}
#define led_mask 1ul << 5
#define LED_On GPIOA->BSRR |= led_mask
#define LED_Off  GPIOA->BSRR |= led_mask << 16
  
  
int main (void) {
 
  SystemCoreClockConfigure();                              // configure HSI as System Clock
  SystemCoreClockUpdate();
  LED_Initialize();
  Buttons_Initialize();
  //SER_Initialize();
  SysTick_Config(SystemCoreClock / 1000);                  // SysTick 1 msec interrupts
  for (;;) {
    LED_On;                                           // Turn specified LED on
    Delay(500);                                            // Wait 500ms
    while (Buttons_GetState() & (1 << 0));                 // Wait while holding USER button
    LED_Off;                                          // Turn specified LED off
    Delay(10);                                            // Wait 500ms
    while (Buttons_GetState() & (1 << 0));                 // Wait while holding USER button
  }
}
