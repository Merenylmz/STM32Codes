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


void GPIO_Config(void){

	RCC->AHB1ENR |= 1 << 3;

	GPIOD->MODER |= 1 << 24;
	GPIOD->MODER &= ~(1 << 25);
	GPIOD->MODER |= 1 << 26;
	GPIOD->MODER &= ~(1 << 27);
	GPIOD->MODER |= 1 << 28;
	GPIOD->MODER &= ~(1 << 29);
	GPIOD->MODER |= 1 << 30;
	GPIOD->MODER &= ~(1 << 31);

	GPIOD->OSPEEDR = (uint32_t)0XFF000000;

}


int main(void){
	RCC_Config();
	SystemCoreClockUpdate();

	GPIO_Config();

	while(1){

		GPIOD->ODR |= 1 << 12;
		GPIOD->ODR |= 1 << 13;
		GPIOD->ODR |= 1 << 14;
		GPIOD->ODR |= 1 << 15;

		for(int i = 0; i < 16800000; i++);

		GPIOD->ODR &= ~(1 << 12);
		GPIOD->ODR &= ~(1 << 13);
		GPIOD->ODR &= ~(1 << 14);
		GPIOD->ODR &= ~(1 << 15);

		for(int a = 0; a < 16800000; a++);

	}

}
