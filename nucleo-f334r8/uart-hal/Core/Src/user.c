#include "user.h"

extern volatile uint16_t blinkDelay;

void blink(void)
{
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
    HAL_Delay(blinkDelay);
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
    HAL_Delay(blinkDelay);
}