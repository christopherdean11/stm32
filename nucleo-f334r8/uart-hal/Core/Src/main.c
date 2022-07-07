#include "main.h"
#include "dac.h"
#include "usart.h"
#include "gpio.h"
#include "user.h"
#include "tim.h"
#include <string.h>

void SystemClock_Config(void);
// uint8_t uart_rx_buf[4]; // uart buffer



uint16_t volatile blinkDelay = 200;
uint8_t rx_buf[5];
uint8_t rx_i = 0;
UartMessage uartData;
uint8_t volatile uartDataReady = 0;
PWM_Config pwm1 = {.Freq=1000, .Duty=30};

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DAC1_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();

  uartData.isMsgComplete = 0;
  // uartData.msgLength = ledCmd.bufLengths[0]+1; // +1 for the ":"
  uartData.subMsgId = 1;
  blink();
  
  HAL_UART_Receive_IT(&huart2, rx_buf, 1);
  uartDataReady = 0;
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

  configTimer(&htim3, &pwm1);
  while (1){

    if (uartDataReady){
      processCommand(&uartData);
      uartDataReady = 0;
      HAL_UART_Receive_IT(&huart2, rx_buf, 1);
    }
    if (uartData.isMsgComplete){
      executeCommand(&uartData);
      HAL_UART_Transmit(&huart2, uartData.tx_msg, uartData.tx_len, 100);
      uartData.isMsgComplete = 0;
      rx_i = 0;
    }
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // add to buffer until newline found
  uartData.rx_buf[rx_i] = rx_buf[0];
  rx_i++;
  if (rx_buf[0] == '\n'){
    uartDataReady = 1;
  } 
  HAL_UART_Receive_IT(&huart2, rx_buf, 1);
}







/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

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

#ifdef  USE_FULL_ASSERT
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
