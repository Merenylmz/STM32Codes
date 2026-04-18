#include "main.h"

void SystemClock_Config(void);

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
	RCC->AHB1ENR |= 1 << 3;

	GPIOD->MODER |= 2 << 24 | 2 << 26 | 2 << 28 | 2 << 30;
	GPIOD->AFR[1] = 2 << 16 | 2 << 20 | 2 << 24 | 2 << 28;
}

void TIM_Config(){
	RCC->APB1ENR |= 1<<2;

	TIM4->CR1 |= 0 << 4;
	//TIM4->CR1 |= 0 << 5;
	TIM4->CR1 |= 0 << 8;
	TIM4->CCMR1 |= 0 << 0 | 6 << 4 | 0 << 8 | 6 << 12;

	TIM4->CCMR2 |= 0 << 0 | 6 << 4 | 0 << 8 | 6 << 12;


	TIM4->CCER |= 1 << 0 | 1 << 4 | 1 << 8 | 1 << 12;

	TIM4->PSC = 8399;
	TIM4->ARR = 9999;
	TIM4->CCR1 = 2499;
	TIM4->CCR2 = 4999;
	TIM4->CCR3 = 7499;
	TIM4->CCR4 = 9999;

	TIM4->CR1 |= 1 << 0;
}



int main(void)
{

  //HAL_Init();
  //SystemClock_Config();
  RCC_Config();
  GPIO_Config();
  TIM_Config();


  while (1)
  {

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
