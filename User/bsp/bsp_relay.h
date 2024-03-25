#ifndef __BSP_RELAY_H
#define __BSP_RELAY_H
#include "stm8s.h"


/* 定义按键IO */
/* 定义按键IO */
#define GPIO_RELAY_LED_TAPE		GPIOC
#define PIN_RELAY_LED_TAPE 	    GPIO_PIN_5		/* relay ctl led tape  */

#define GPIO_RELAY_FAN			GPIOC
#define PIN_RELAY_FAN 			GPIO_PIN_6		/* relay ctl fan*/

#define GPIO_RELAY_KILL  		GPIOC
#define PIN_RELAY_KILL 	        GPIO_PIN_7		/* relay ctl kill*/

#define GPIO_RELAY_TEMP         GPIOC
#define PIN_RELAY_TEMP          GPIO_PIN_0     /*  relay keep temperature ctl*/

//relay corresponding led on or off

#define GPIO_RELAY_TAPE_LED      GPIOC 
#define PIN_RELAY_TAPE_LED       GPIO_PIN_1

#define GPIO_RELAY_FAN_LED      GPIOC 
#define PIN_RELAY_FAN_LED       GPIO_PIN_2

#define GPIO_RELAY_KILL_LED      GPIOC 
#define PIN_RELAY_KILL_LED       GPIO_PIN_3

#define GPIO_RELAY_TEMP_LED      GPIOC 
#define PIN_RELAY_TEMP_LED       GPIO_PIN_4

#define RELAY_TAPE_TURN_ON()       GPIO_WriteLow(GPIO_RELAY_LED_TAPE,PIN_RELAY_LED_TAPE)
#define RELAY_TAPE_TURN_OFF()      GPIO_WriteHigh(GPIO_RELAY_LED_TAPE,PIN_RELAY_LED_TAPE)

#define RELAY_FAN_TURN_ON()        GPIO_WriteLow(GPIO_RELAY_FAN,PIN_RELAY_FAN)
#define RELAY_FAN_TURN_OFF()       GPIO_WriteHigh(GPIO_RELAY_FAN,PIN_RELAY_FAN)

#define RELAY_KILL_TURN_ON()       GPIO_WriteLow(GPIO_RELAY_KILL,PIN_RELAY_KILL)
#define RELAY_KILL_TURN_OFF()      GPIO_WriteHigh(GPIO_RELAY_KILL,PIN_RELAY_KILL)

#define RELAY_TEMP_TURN_ON()       GPIO_WriteLow(GPIO_RELAY_TEMP,PIN_RELAY_TEMP)
#define RELAY_TEMP_TURN_OFF()      GPIO_WriteHigh(GPIO_RELAY_TEMP,PIN_RELAY_TEMP)





typedef struct 
{
	uint8_t led_tape;
	uint8_t fan;
	uint8_t kill;
	uint8_t temp;
    uint8_t setup_keep_temp_value;
    uint8_t setup_keep_temp_value_flag;
}relay_state_t;

extern relay_state_t grelay_t;





extern uint8_t (*led_tape_state)(void);
extern uint8_t (*fan_state)(void);
extern uint8_t (*kill_state)(void);
extern uint8_t (*temp_state)(void);

void Relay_Tape_Hander(uint8_t (*tape_handler)(void));
void Relay_Fan_Hander(uint8_t (*fan_handler)(void));
void Relay_Kill_Hander(uint8_t (*kill_handler)(void));
void Relay_Temp_Hander(uint8_t (*temp_handler)(void));





void Relay_Init(void);






#endif 

