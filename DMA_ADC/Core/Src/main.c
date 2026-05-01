#include "main.h"


uint32_t adc;
uint32_t adc1[8];

void RCC_Config(void){
	RCC->CR &= ~(1 << 0);
	RCC->CR |= 1 << 16;
	while(!(RCC->CR & (1 << 17)));
	RCC->CR |= 1 << 19;
	RCC->PLLCFGR = 0x00000000;
	RCC->PLLCFGR |= 1 << 22; // PLL Source HSE
	RCC->PLLCFGR |= 1 << 2;// PLL_M
	RCC->PLLCFGR |= 168 << 6; //PLL_N

	RCC->CFGR |= (5 << 10);
	RCC->CFGR |= (4 << 13);
	FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10) | 5;


	RCC->PLLCFGR &= ~(1 << 12);//PLL_N

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

}

void ADC_Config()
{
	RCC->APB2ENR |= 0x00000100;	// ADC1 Clock enable

	ADC->CCR 	|= 1 << 16;		// ADC Clock Divided By 4
	//ADC1->SMPR2 |= 6 << 0;	// 144 Cycles for Channel 0
	ADC1->CR1 	|= 0 << 24;		// ADC Resolution 12 bit
	ADC1->CR1	|= 1 << 8;		// Scan conversion mode enable
	ADC1->CR2   |= 1 << 0;		// ADC enable
	ADC1->CR2   |= 1 << 1;		// Continuous conversion mode enable
	ADC1->CR2   |= 1 << 8;		// DMA Enable
	ADC1->CR2   |= 1 << 9;		// DDS
	ADC1->CR2   |= 1 << 10;		// EOCS
	//ADC1->CR2   |= 1 << 30;	//
	ADC1->SQR1 	|= 0 << 20;		// L = 1 conversion number
	ADC1->SQR3  |= 0 << 0;		// put channel number CH0

}

void DMA_Config(){
		RCC->AHB1ENR |= 0x00400000;			// RCC->AHB1ENR |= (1<<22); // DMA2 clk Enable

		while((DMA2_Stream4->CR & 0x00000001) == 1);	// wait for stream4 to be 0(stop)

		DMA2_Stream4->PAR|= (uint32_t) &ADC1->DR;
		DMA2_Stream4->M0AR = (uint32_t) &adc1;
		DMA2_Stream4->NDTR = 1;
		DMA2_Stream4->CR |= 0 << 6;	 	// Peripheral to Memory
		DMA2_Stream4->CR |= 1 << 8;		// Circular mode
		DMA2_Stream4->CR |= 1 << 10;		// memory incremented
		DMA2_Stream4->CR |= 2 << 11;		// peripheral data size 32 bit (word)
		DMA2_Stream4->CR |= 2 << 13;		// memory data size 32 bit (word)
		DMA2_Stream4->CR |= 2 << 16;		// priority level high
		DMA2_Stream4->CR |= 0 << 25;		// channel 0 selected
		DMA2_Stream4->FCR |= 0 << 2;		// direct mode enable
		DMA2_Stream4->CR |= 1 << 0;			// start stream 4
		//ADC1->CR2 |= ADC_CR2_SWSTART;
}

int main(void)
{

	RCC_Config();
	GPIO_Config();
	ADC_Config();
	DMA_Config();
	ADC1->CR2 |= ADC_CR2_SWSTART;
  while (1)
  {
	  adc = adc1[0];
  }

}


void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
