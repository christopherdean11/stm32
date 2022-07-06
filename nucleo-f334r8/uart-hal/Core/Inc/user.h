#ifndef USER_H
#define USER_H

#include "main.h"

typedef enum {
  CMD_LED, 
  CMD_PWM, 
  } CMD_TYPE;

typedef enum {
  CMDOPT_LED_STATE,
  CMDOPT_PWM_FREQ,
  CMDOPT_PWM_DUTY,
  CMDOPT_UNKNOWN,
} CMD_OPTION;

typedef enum {
  CMD_SET,
  CMD_GET,
 } CMD_SETGET;


typedef enum {
  WAIT_FOR_DATA, 
  HANDLE_DATA, 
 } UART_RX_STATE;  

typedef struct {
  // led:0, or led:1
  uint8_t rx_buf[30];
  CMD_TYPE cmdType;
  uint8_t ch;
  CMD_OPTION cmdOption;
  CMD_SETGET cmdSetGet;
  uint16_t value;
  uint8_t msgLength;
  uint8_t nextLength;
  uint8_t subMsgId; // ID of section i.e. the parts between colons in the msg
  uint8_t isMsgComplete;
  // void (*processFcn)(UartMessage*);
  uint8_t tx_msg[5]; // maybe remove this one
  uint8_t tx_len;
} UartMessage;


void blink(void);
void processUartData(UartMessage *uartData);
void processLedMsg(UartMessage *uartData);
void processPwmMsg(UartMessage *uartData);
void processCommand(UartMessage *cmd);
void executeCommand(UartMessage *cmd);

#endif