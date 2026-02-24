#include "main.h"
#include "stm32f4xx.h"

void delay(uint32_t time){
	while(time--);
}
int counter;

void RCC_Config(void){
	RCC->CR &= ~(1 << 0);
	RCC->CR |= 1 << 16;
	while(!(RCC->CR & (1 << 17)));
	RCC->CR |= 1 << 19;
	RCC->PLLCFGR = 0x00000000;

	RCC->PLLCFGR |= 1 << 22;
	RCC->PLLCFGR |= 1 << 2;

	RCC->PLLCFGR |= 168 << 6;

	RCC->CFGR |= (5 << 10);
	RCC->CFGR |= (4 << 13);
	FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10) | 5;


	RCC->PLLCFGR &= ~(1 << 12);
	RCC->CR |= 1 << 24;
	while(!(RCC->CR & (1 << 25)));

	RCC->CFGR &= ~(1 << 0);
	RCC->CFGR |= (1 << 1);
	while(!(RCC->CFGR & (1 << 1)));

}
void DAC1_Config(){
	RCC->AHB1ENR |= (uint32_t)0x00000001;
	RCC->APB1ENR |= (uint32_t)0x20000000;

	GPIOA->MODER |= (3 << 8);

	GPIOA->PUPDR &= ~(3 << 8);

	DAC->CR |= (uint32_t)0x00000001;
	DAC->SWTRIGR = (uint32_t)0x00000000;
	DAC->DHR12R1 = (uint32_t)0x00000000;
}

int main(void)
{
	RCC_Config();
	SystemCoreClockUpdate();
	DAC1_Config();




  while (1)
  {
	  for(counter=0; counter < 4096; counter++)
	  {
		  DAC->DHR12R1 = counter;
		  delay(16800);
	  }
	  counter = 0;
  }
}

