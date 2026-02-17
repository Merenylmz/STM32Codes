#include "main.h"
#include "stm32f4xx.h"


volatile uint8_t adcValue;
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
	RCC->AHB1ENR |= (uint32_t)0x00000009;

	GPIOA->MODER |= (uint32_t)0x00000003;
	GPIOA->OSPEEDR |= 0x00000003;
	GPIOD->MODER = (uint32_t)0x55000000;
	GPIOD->OSPEEDR = (uint32_t)0XFF000000;
}

void ADC_Config(){
	RCC->APB2ENR |= 0x00000100;

	ADC1->CR1 |= (uint32_t)0x02000000;
	ADC1->CR2 |= (uint32_t)0x00000001;
	ADC1->SMPR2 |= (uint32_t)0x00000003;

	ADC->CCR |= (uint32_t)0x00010000;

}

uint8_t Read_ADC(){
	ADC1->CR2 |= (uint32_t)0x40000000;

	while(!(ADC1->SR & 0x00000002));

	return (uint8_t)ADC1->DR;
}


void delay(volatile uint32_t n){
	while(n--);
}




int main(void)
{
	RCC_Config();
	SystemCoreClockUpdate();
	GPIO_Config();
	ADC_Config();


  while (1)
  {
	  adcValue = Read_ADC();
	  if(adcValue < 50){
		  GPIOD->ODR |= 1 << 12;
	  } else {
		  GPIOD->ODR &= ~(1 << 12);
	  }

	  delay(10000);
  }
}

