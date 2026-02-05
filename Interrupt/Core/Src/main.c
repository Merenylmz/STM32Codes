#include "main.h"
#include "stm32f4xx.h"

void RCC_Config(void){
	RCC->CR &= ~(1 << 0); //HSI OFF
	RCC->CR |= 1 << 16; // HSE ON
	while(!(RCC->CR & (1 << 17))); //Wait HSE active
	RCC->CR |= 1 << 19;
	RCC->PLLCFGR = 0x00000000; // genel sıfırlama
	/*RCC->PLLCFGR &= ~(1<<0);
	RCC->PLLCFGR &= ~(1<<1);*/
	RCC->PLLCFGR |= 1 << 22; // PLL Source HSE
	RCC->PLLCFGR |= 1 << 2;// PLL_M
	// iki şekildede yapılabilir burada decimal olan 168 i register a binary olarak aktarır. diğerinde ise amaç 8 yazmaktı 2 üzeri 3ten onuda hallettik kısaca direk binary verdik
	RCC->PLLCFGR |= 168 << 6; //PLL_N

	RCC->CFGR |= (5 << 10);
	RCC->CFGR |= (4 << 13);
	FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10) | 5;


	RCC->PLLCFGR &= ~(1 << 12);//PLL_N
	//RCC->PLLCFGR |= 1 << 17;

	RCC->CR |= 1 << 24;
	while(!(RCC->CR & (1 << 25)));

	RCC->CFGR &= ~(1 << 0);
	RCC->CFGR |= (1 << 1);
	while(!(RCC->CFGR & (1 << 1)));

}

void GPIO_Config(){
	RCC->AHB1ENR = (uint32_t)0x00000009;


	GPIOD->MODER = (uint32_t)0x55000000;
	GPIOD->OSPEEDR = (uint32_t)0XFF000000;
}

void EXTI_Config(){
	RCC->APB2ENR |= 1 << 14;
	SYSCFG->EXTICR[0] = 0x00000000;

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);

	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_SetPriority(EXTI1_IRQn, 1);
	NVIC_SetPriority(EXTI2_IRQn, 2);

	EXTI->IMR = (uint32_t)0x00000007;
	EXTI->RTSR = (uint32_t)0x00000007;

}

void delay(volatile uint32_t n){
	while(n--);
}

void EXTI0_IRQHandler(){
	if (EXTI->PR & 0x00000001) {
		for (int a = 0; a < 5; ++a) {
			GPIOD->ODR = (uint32_t)0x00001000;
			delay(1680000);
			GPIOD->ODR = (uint32_t)0x00000000;
			delay(1680000);
		}
		EXTI->PR = (uint32_t)0x00000001;
	}
}

void EXTI1_IRQHandler(){
	if (EXTI->PR & 0x00000002) {
		for (int a = 0; a < 5; ++a) {
			GPIOD->ODR = (uint32_t)0x00002000;
			delay(1680000);
			GPIOD->ODR = (uint32_t)0x00000000;
			delay(1680000);
		}
		EXTI->PR = (uint32_t)0x00000002;
	}
}

void EXTI2_IRQHandler(){
	if (EXTI->PR & 0x00000003) {
		for (int a = 0; a < 5; ++a) {
			GPIOD->ODR = (uint32_t)0x00004000;
			delay(1680000);
			GPIOD->ODR = (uint32_t)0x00000000;
			delay(1680000);
		}
		EXTI->PR = (uint32_t)0x00000003;
	}
}


int main(void)
{
	RCC_Config();
	SystemCoreClockUpdate();
	GPIO_Config();
	EXTI_Config();

  while (1)
  {
	  GPIOD->ODR = (uint32_t)0x0000F000;


  }
}

