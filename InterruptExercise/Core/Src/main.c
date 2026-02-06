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
	GPIOD->OSPEEDR = (uint32_t)0xFF000000;
	GPIOD->OTYPER = 0x0;

}

void EXTI_Config(){
	RCC->APB2ENR = (uint32_t)0x00004000;

	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);

	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_SetPriority(EXTI1_IRQn, 1);
	NVIC_SetPriority(EXTI2_IRQn, 2);


	SYSCFG->EXTICR[0] = 0x00000000;
	SYSCFG->EXTICR[1] = 0x00000000;
	SYSCFG->EXTICR[2] = 0x00000000;


	EXTI->IMR = (uint32_t)0x00000007;
	EXTI->RTSR = (uint32_t)0x00000007;

}

void delay(volatile uint32_t time){
	while(time--);
}

void EXTI0_IRQHandler(){
	if (EXTI->PR & 0x01) {
		for (int a = 0; a < 5; a++) {
			GPIOD->ODR |= 1 << (12 + a);
			delay(84000000 / 8);
			GPIOD->ODR ^= 1 << (12 + a);
		}
		EXTI->PR = 0x01;
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
	  GPIOD->ODR = 0x0000F000;
  }
}

