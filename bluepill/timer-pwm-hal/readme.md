# Timer Example Using the HAL

See [main.c](Core/Src/main.c) for the full code

Connect LEDs to PA8 and PA9 and the two LEDs will slowly ramp from 0% to 100% brightness. The LEDs will ramp in opposite directions. 

The heart of the code for this example is in this code chunk:  
```c
/* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Start(&htim1);
  HAL_TIM_Base_Start(&htim2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    for (uint32_t i=0; i < 1000; i+=10){
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, i); // increase PWM
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 1000-i); // decrease PWM
      HAL_Delay(10);
    }
    for (uint32_t i=0; i < 1000; i+=10){
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1000-i); // decrease PWM
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, i); // increase PWM
      HAL_Delay(10);
    }
  }
  /* USER CODE END 3 */
}
```