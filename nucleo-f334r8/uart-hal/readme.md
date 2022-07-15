# UART-HAL
Send commands from PC to the nucleo board to enable/disable the on-board LED, or set a timer output frequency or duty cycle.

Commands can be sent/responses read with the `uart.py` python script, which requires the `pyserial` package (`pip install pyserial`). 
## Commands
Command structure is designed to be extendable to additional peripherals or channels per peripheral, and are in a standard form of 
```
PERIPH:CH:OPTION:VALUE
```
### LED Commands
To set the on-board LED state use
```
PERIPH = led
CH = 1
OPTION = state
VALUE = 0 | 1
``` 
For the simple LED, the command always starts with `led:1:state:`. 

To turn the LED on:
```
led:1:state:1
```
To turn the LED off:
```
led:1:state:0
```
Additional LEDs could be supported using the `<CH>` parameter, and brigthness control could be used with a timer output to the LED pin with possible commands such as `led:1:dim:50` for 50% dimming via PWM.
### PWM Timer Commands
For the PWM output timer
```
PERIPH = pwm
CH = 1
OPTION = freq | duty
VALUE = <number>
``` 
OPTION can be used to set the PMW frequency or duty cycle, and the VALUE parameter provides the set point.
```
pwm:1:freq:1000 // set to 1000 Hz 
pwm:1:freq:3500 // set to 3.5 kHZ
pwm:1:duty:10 // set to 10% duty 
pwm:1:duty:50 // set to 50% duty
```


## Notes 
* To add a c/h file to the build
  - create the .c in /Src and .h in /Inc
  - `#include` the .h in the .c and possibly main.c too
  - `#include` anything needed inside the user.c file (such as main.h to get hal and `#defines`)
  - **go to makefile and add the user.c file to the `C_SOURCES = \` section**
