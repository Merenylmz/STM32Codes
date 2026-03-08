#include "main.h"
#include <stdio.h>
#include "stm32f4xx.h"


void RCC_Config(void){
	RCC->CR &= ~(1 << 0); //HSI OFF
	RCC->CR |= 1 << 16; // HSE ON
	while(!(RCC->CR & (1 << 17))); //Wait HSE active
	RCC->CR |= 1 << 19;
	RCC->PLLCFGR = 0x00000000; // genel sıfırlama
	RCC->PLLCFGR |= 1 << 22; // PLL Source HSE
	RCC->PLLCFGR |= 1 << 2;// PLL_M

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


uint16_t counter;

void TIMER2_Config(){
	RCC->APB1ENR |= 1 << 0;

	TIM2->CR1 |= 0 << 4;
	TIM2->CR1 |= 0 << 5;
	TIM2->CR1 |= 0 << 6;
	TIM2->CR1 |= 0 << 8;
	TIM2->CR1 |= 0 << 9;

	TIM2->SMCR |= 0 << 0;
	TIM2->EGR |= 1 << 0;
	TIM2->PSC |= 41999;
	TIM2->ARR |= 4000;


	TIM2->CR1 |= 1 << 0;
}

int main(void)
{
	RCC_Config();
	SystemCoreClockUpdate();

	TIMER2_Config();

  while (1)
  {

	  counter = TIM2->CNT;
  }
}

