#include <string.h>
#include <math.h>
#include "user.h"
#include "tim.h"


extern volatile uint16_t blinkDelay;
extern volatile uint8_t rx_i;
extern PWM_Config pwm1;

void blink(void)
{
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
  HAL_Delay(blinkDelay);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
  HAL_Delay(blinkDelay);
}



void processCommand(UartMessage *cmd)
{
  uint8_t tmpBuf[10];
  uint8_t len = 0;
  cmd->subMsgId = 1;
  for (uint8_t i = 0; i < rx_i; i++)
  {
    if ((cmd->rx_buf[i] == ':') || (cmd->rx_buf[i] == '\n'))
    {
       // end of sub-section
      switch (cmd->subMsgId)
      {
        case 1:
          // command type
          if (0==strncmp(tmpBuf, "led",3)){
            cmd->cmdType = CMD_LED;
          }
          if (0==strncmp(tmpBuf, "pwm",3)){
            cmd->cmdType = CMD_PWM;
          }
          break;
        
        case 2:
          // command channel
          cmd->ch = tmpBuf[0] - '0';
          break;
        
        case 3:
          // command option
          if (0==strncmp(tmpBuf, "state", 5)){
            cmd->cmdOption = CMDOPT_LED_STATE;
          } else if (0==strncmp(tmpBuf, "freq", 4)){
            cmd->cmdOption = CMDOPT_PWM_FREQ;
          }  else if (0==strncmp(tmpBuf, "duty", 4)){
            cmd->cmdOption = CMDOPT_PWM_DUTY;
          }
          break;
        case 4:
        {
          // need braces because can't declar a variable after a label, case: is a label
          // command value
          if ((len==1) && (tmpBuf[0]=='?')){
            cmd->cmdSetGet = CMD_GET;
          } else {
            cmd->cmdSetGet = CMD_SET;
            uint16_t value = 0;
            for (uint8_t j=0; j<len; j++){
              value += (tmpBuf[len-1-j] - '0') * (powf(10,j));
            }
            cmd->value = value;
          }
        }
        default:
          break;
      } 
      len = 0;
      cmd->subMsgId++;
    } 
    else {
      tmpBuf[len] = cmd->rx_buf[i];
      len++;
    }

    if (cmd->rx_buf[i] == '\n'){
      cmd->isMsgComplete = 1;
      memcpy(cmd->tx_msg, "done", 4);
      cmd->tx_msg[4] = 10; //linefeed
      cmd->tx_len = 5;
      return;
    }
  }
}

void executeCommand(UartMessage *cmd)
{
  uint8_t msg[20];
  switch (cmd->cmdType)
  {
    case CMD_LED:
      if (cmd->cmdSetGet==CMD_GET){
        // state = (GPIOA->ODR & GPIO_PIN_5) == 0 => 
        // => if the value of ODR & bitmask for pin 5 is 0,
        // then output is 0, otherwise its high
        // so invert state to be "positive logic"
        uint8_t state = !(GPIOA->ODR & LD2_Pin) == 0;
        memcpy(msg, "led:", 4);
        msg[4] = cmd->ch + '0';
        msg[5] = '=';
        msg[6] = state + '0';
        msg[7] = '\n';
        memcpy(cmd->tx_msg, msg, 8);
      } else {
        HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, cmd->value);
      }
      break;
    
    case CMD_PWM:
      if (cmd->cmdSetGet==CMD_SET){
        if (cmd->cmdOption == CMDOPT_PWM_FREQ){
          pwm1.Freq = cmd->value;
        }else if (cmd->cmdOption==CMDOPT_PWM_DUTY){
          pwm1.Duty = cmd->value;
        }else {
          break;
        }
        configTimer(&htim3, &pwm1);
      }
    break;
    
    default:
    break;;
  }
}


void configTimer(TIM_HandleTypeDef *htim, PWM_Config *pwm)
{
  uint32_t TIM_CLK = 64e6;
  uint32_t div  = (TIM_CLK / pwm->Freq);
  uint32_t psc_code = (div/UINT16_MAX);
  uint32_t arr_code = (div/(psc_code+1));
  uint32_t pwm_code = (uint32_t)(pwm->Duty/100.0 * arr_code);

  __HAL_TIM_SET_PRESCALER(htim, psc_code);
  __HAL_TIM_SetAutoreload(htim, arr_code);
  __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, pwm_code);
  
}