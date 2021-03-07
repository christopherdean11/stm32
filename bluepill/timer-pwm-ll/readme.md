# Use a Timer to create PWM channels with the Low-Level (LL) API 

This code uses TIM1 to create the PWM channels. Connect LEDs to PA8 and PA9 and the two LEDs will slowly ramp from 0% to 100% brightness. The LEDs will ramp in opposite directions.

### Code
See [main.c](Core/Src/main.c) for the full code

The snippet shows the heart of the code:  
```c
// Enable Ch1 and Ch2
  LL_TIM_CC_EnableChannel(TIM1, TIM_CCER_CC1E | TIM_CCER_CC2E);
  // Enable outputs by setting MOE (master output enable) bit high in BDTR (break and dead-time reg)
  LL_TIM_EnableAllOutputs(TIM1);
  // Enable the timer (CEN bit in CR1)
  LL_TIM_EnableCounter(TIM1);

  while (1)
  {
	  for (uint32_t i = 0; i < 1000; i +=10 ){
		  LL_TIM_OC_SetCompareCH1(TIM1, i);
		  LL_TIM_OC_SetCompareCH2(TIM1, 1000-i);
		  HAL_Delay(20);
	  }
	  for (uint32_t i = 0; i < 1000; i +=10 ){
		  LL_TIM_OC_SetCompareCH1(TIM1, 1000-i);
		  LL_TIM_OC_SetCompareCH2(TIM1, i);
		  HAL_Delay(20);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

```

### CubeMX Bug?
 There appears to be a bug when generating code with CubeMX code generator, when setting the Prescaler (PSC) or Auto-Reload Register (ARR) to something like `48-1` with the LL API (entering `N-1` helps when you come back later to remember its effectively 48 but needs to use code 47). It generates code like this:
```c
  TIM_InitStruct.Prescaler = 48-LL_TIM_IC_FILTER_FDIV1_N2; // entered as "48-1" into Cube text box
```
when you really wanted
```c
  TIM_InitStruct.Prescaler = 48-1;
```
but if you enter it as `47` exactly, it generates:
```c
  TIM_InitStruct.Prescaler = 47; // entered exactly 47 into Cube text box
```
I found this when the frequency was not 1000Hz but was close, the PSC used was 48 not 47. So watch out for that. 
