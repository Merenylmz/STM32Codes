
#include "main.h"
void SystemClock_Config(void);

volatile uint32_t adcValue, ccrValue, deneme; // potans degeri
volatile short i = 1;

void RCC_Config(){
	RCC->CR |= 0x01010000;
	while(!(RCC->CR & RCC_CR_HSERDY));

	FLASH->ACR |= 0x00000005; // işlemci hızlamadan önce bir gecikme verir.

	RCC->PLLCFGR = 0x07405408;
	RCC->CR |= (1 << 24); // PLLON
	while(!(RCC->CR & RCC_CR_PLLRDY));

	RCC->CFGR |= 0x0000940A;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void GPIO_Config(){
	RCC->AHB1ENR |= 0x00000009;

	GPIOD->MODER &= ~(3 << 24);
	GPIOD->MODER |= 2 << 24;

	GPIOD->AFR[1] &= ~(0xF << 16);
	GPIOD->AFR[1] |= (0x2 << 16);

	GPIOA->MODER |= 3 << 0;

	//For The Button
	GPIOA->MODER &= ~(3 << 2);
	GPIOA->PUPDR &= ~(3 << 2);
	GPIOA->PUPDR |= 2 << 2;
}

void TIM_Config(){
	RCC->APB1ENR |= 0x00000004;

	TIM4->PSC = 83;
	TIM4->ARR = 999;
	TIM4->CCMR1 |= (6 << 4);
	TIM4->CCMR1 |= (1 << 3);
	TIM4->CCER |= 1 << 0 | 1 << 4 | 1 << 8 | 1 << 12;

	TIM4->CCR1 |= 999; // baslangic degeri

	TIM4->EGR |= 1 << 0;
	TIM4->CR1 |= 1 << 0;
}

void ADC_Config(){
	RCC->APB2ENR |= 1 << 8;

	ADC->CCR |= 1 << 16;

	ADC1->SQR3 &= ~(0x1F << 0);

	ADC1->CR2 |= 1 << 1;
	ADC1->CR2 |= 1 << 0;
	ADC1->CR2 |= 1 << 30;
}

uint32_t readADC(){
	while(!(ADC1->SR & (1 << 1)));

	return (uint32_t)ADC1->DR;
}

void delay(uint16_t delay){
	for(volatile int k = 0; k < delay; k++);
}

void updatePWMFrequency(int val){
	TIM4->PSC = val;
}

int main(void)
{

  //HAL_Init();
  //SystemClock_Config();

	RCC_Config();
	GPIO_Config();
	TIM_Config();
	ADC_Config();

  while (1)
  {
	  deneme = TIM4->PSC;
	  if (GPIOA->IDR & (1 << 1)) {
		  millis();
		  ++i;
		  switch (i) {
			case 1: updatePWMFrequency(167); break;
			case 2: updatePWMFrequency(83); break;
			case 3: updatePWMFrequency(41); i = 0; break;
			default:
				updatePWMFrequency(83);
				break;
		}
	}
	  adcValue = readADC();
	  ccrValue = (adcValue*999)/4095;
	  TIM4->CCR1 = ccrValue;

	  delay(50000);
  }

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
