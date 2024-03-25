#include "bsp.h"

relay_state_t grelay_t;

uint8_t (*led_tape_state)(void);
uint8_t (*fan_state)(void);
uint8_t (*kill_state)(void);
uint8_t (*temp_state)(void);

static uint8_t led_tape_default_fun(void);
static uint8_t fan_default_fun(void);
static uint8_t kill_default_fun(void);
static uint8_t  temp_default_fun(void);



/************************************************************************************************
 * 
 * Function Name:void Relay_Init(void)
 * Function : 
 * Input Ref:NO
 * Return Ref:NO
 * 
************************************************************************************************/
void Relay_Init(void)
{
    //relay GPIO init
    GPIO_Init(GPIO_RELAY_LED_TAPE, (GPIO_Pin_TypeDef)PIN_RELAY_LED_TAPE, GPIO_MODE_OUT_PP_HIGH_SLOW);
	GPIO_Init(GPIO_RELAY_FAN, (GPIO_Pin_TypeDef)PIN_RELAY_FAN, GPIO_MODE_OUT_PP_HIGH_SLOW);
	GPIO_Init(GPIO_RELAY_KILL, (GPIO_Pin_TypeDef)PIN_RELAY_KILL, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(GPIO_RELAY_TEMP, (GPIO_Pin_TypeDef)PIN_RELAY_TEMP, GPIO_MODE_OUT_PP_HIGH_SLOW);	

    //relay GPIO of led init
    GPIO_Init(GPIO_RELAY_TAPE_LED, (GPIO_Pin_TypeDef)PIN_RELAY_TAPE_LED, GPIO_MODE_OUT_PP_HIGH_SLOW);
	GPIO_Init(GPIO_RELAY_FAN_LED, (GPIO_Pin_TypeDef)PIN_RELAY_FAN_LED, GPIO_MODE_OUT_PP_HIGH_SLOW);
	GPIO_Init(GPIO_RELAY_KILL_LED, (GPIO_Pin_TypeDef)PIN_RELAY_KILL_LED, GPIO_MODE_OUT_PP_HIGH_SLOW);
    GPIO_Init(GPIO_RELAY_TEMP_LED, (GPIO_Pin_TypeDef)PIN_RELAY_TEMP_LED, GPIO_MODE_OUT_PP_HIGH_SLOW);	
    //
    Relay_Tape_Hander(led_tape_default_fun);
    Relay_Fan_Hander(fan_default_fun);
    Relay_Kill_Hander(kill_default_fun);
    Relay_Temp_Hander(temp_default_fun);

}


/************************************************************************************************
 * 
 * Function Name:void Relay_Init(void)
 * Function : 
 * Input Ref:NO
 * Return Ref:NO
 * 
************************************************************************************************/
static uint8_t led_tape_default_fun(void)
{
    if(grelay_t.led_tape == 1) return 1;
    else return 0;

}

void Relay_Tape_Hander(uint8_t (*tape_handler)(void))
{
     led_tape_state = tape_handler;
}

//FAN RELAY
static uint8_t fan_default_fun(void)
{
    if(grelay_t.fan == 1) return 1;
    else return 0;
}

void Relay_Fan_Hander(uint8_t (*fan_handler)(void))
{
     fan_state = fan_handler;
}
//KILL RELAY
static uint8_t kill_default_fun(void)
{
    if(grelay_t.kill == 1) return 1;
    else return 0;

}

void Relay_Kill_Hander(uint8_t (*kill_handler)(void))
{
     kill_state = kill_handler;
}

//keep temperature value 
static uint8_t temp_default_fun(void)
{
    if(grelay_t.temp == 1) return 1;
    else return 0;
}
void Relay_Temp_Hander(uint8_t (*temp_handler)(void))
{
     temp_state = temp_handler;
}



