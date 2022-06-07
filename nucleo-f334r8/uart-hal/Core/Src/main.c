#include "main.h"
#include "dac.h"
#include "usart.h"
#include "gpio.h"
#include "user.h"
#include <string.h>

void SystemClock_Config(void);
uint8_t uart_rx_buf[4]; // uart buffer
enum RX_STATE{START, LED, PWM};
enum RX_STATE rx_state;

uint16_t volatile blinkDelay = 100;

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DAC1_Init();
  MX_USART2_UART_Init();

  blink();
  rx_state = START;
  HAL_UART_Receive_IT(&huart2, uart_rx_buf, 4);
  while (1)
  {
    blink();
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // commands
  // led:set:[0|1] - set led blink speed (0=fast, 1=slow)

  uint8_t tx_buf[4] = {0};
  if (rx_state == START)
  {  
    if (0 == strncmp((const char*)uart_rx_buf, "led:", 4))
    {
      // re-trigger uart rx again
      HAL_UART_Receive_IT(huart, uart_rx_buf, 1);
      rx_state = LED;
      memcpy(tx_buf, "led",3);
    }
    if (0 == strncmp((const char*)uart_rx_buf, "pwm:", 4))
    {
      // re-trigger uart rx again
      HAL_UART_Receive_IT(huart, uart_rx_buf, 2);
      rx_state = PWM;
      memcpy(tx_buf, "pwm",3);
    }
    HAL_UART_Transmit(huart, tx_buf, 3, 100);
    return;
  }

  if (rx_state==LED)
  {
    if (0 == strncmp((const char*)uart_rx_buf, "1", 1))
    {
      blinkDelay = 500;
      memcpy(tx_buf, "1", 1);
    } else {
      blinkDelay = 50;
      memcpy(tx_buf, "0", 1);
    }
    rx_state = START;
    HAL_UART_Receive_IT(huart, uart_rx_buf, 4);    
    HAL_UART_Transmit(huart, tx_buf, 1, 100);
  }

  
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
