#include "main.h"

void RCC_Config(void){
	RCC->CR |= (1 << 16);
	while(!(RCC->CR & (1 << 17)));
	FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10) | 5;
	RCC->PLLCFGR = 0x00000000;
	RCC->PLLCFGR |= (1 << 22);
	RCC->PLLCFGR |= (4 << 0);
	RCC->PLLCFGR |= (168 << 6);

	RCC->CFGR |= (5 << 10);
	RCC->CFGR |= (4 << 13);
	RCC->CR |= (1 << 24);
	while(!(RCC->CR & (1 << 25)));
	RCC->CFGR &= ~(3 << 0);
	RCC->CFGR |= (2 << 0);
	while((RCC->CFGR & (3 << 2)) != (2 << 2));
}

void GPIO_Config(){
	RCC->AHB1ENR |= (1 << 3) | (1 << 1) | (1 << 0);

	GPIOB->MODER |= (2 << 20) | (2 << 22);
	GPIOB->AFR[1] |= (7 << 8) | (7 << 12);
}

void USART_Config(){
	RCC->APB1ENR |= 1 << 18;

	USART3->BRR = 0x1112;
	USART3->CR1 |= 1 << 2;
	USART3->CR1 |= 1 << 3;
	USART3->CR1 |= 1 << 5;
	USART3->CR1 &= ~(1 << 10);
	USART3->CR1 &= ~(1 << 12);
	USART3->CR2 &= ~(1 << 12);
	USART3->CR1 |= 1 << 13;
}

void NVIC_Config(){
	NVIC->ISER[1] |= 0x00000080;
}

volatile int i = 0;
volatile char RX_Buffer[100];

void USART3_IRQHandler(){

	if(USART3->SR & (1 << 5)) {
		RX_Buffer[i] = USART3->DR;
		i++;

		if(i >= 100) {
			i = 0;
		}
	}
}

void sendChar(char message){
	while(!(USART3->SR & 0x00000080));
	USART3->DR = message;
}

void sendMessage(char *str){
	while(*str){
		sendChar(*str);
		str++;
	}
}

int main(void)
{
	RCC_Config();
	GPIO_Config();
	USART_Config();
	NVIC_Config();

	sendMessage("Hello World\r\n");


	for(int j=0; j<0x1000000; j++);

  while (1)
  {

  }
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
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
