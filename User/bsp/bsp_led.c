/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.c
*	版    本 : V1.0
*	说    明 : 驱动LED指示灯
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2014-09-18 armfly  ST固件库版本 V2.1.0
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 
	PE5 : LED1指示灯 和 串行FLASH的片选复用 
	PG0 : LED2指示灯 和CH376芯片的片选复用
	PD3 : LED3指示灯 和TFT显示器接口中的触摸屏SPI的片选复用
*/
#define LED1_PORT	GPIOE
#define LED1_PIN 	GPIO_PIN_5

#define LED2_PORT	GPIOG	
#define LED2_PIN 	GPIO_PIN_0

#define LED3_PORT	GPIOD			/* 运行指示灯LED的端口 */
#define LED3_PIN 	GPIO_PIN_3	/* 运行指示灯LED的bit */

/*
*********************************************************************************************************
*	函 数 名: bsp_InitLed
*	功能说明: 初始化LED指示灯
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	/* 初始化LED运行指示灯接口，配置为推挽输出 */
	GPIO_Init(LED1_PORT, (GPIO_Pin_TypeDef)LED1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LED2_PORT, (GPIO_Pin_TypeDef)LED2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LED3_PORT, (GPIO_Pin_TypeDef)LED3_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOn
*	功能说明: 点亮指定的LED指示灯。
*	形    参: _no : 1-3 表示3个灯的id
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOn(uint8_t _no)
{
	if (_no == 1)
	{
		GPIO_WriteLow(LED1_PORT, (GPIO_Pin_TypeDef)LED1_PIN);
	}
	else if (_no == 2)
	{
		GPIO_WriteLow(LED2_PORT, (GPIO_Pin_TypeDef)LED2_PIN);
	}
	else if (_no == 3)	
	{
		GPIO_WriteLow(LED3_PORT, (GPIO_Pin_TypeDef)LED3_PIN);
	}			
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedOff
*	功能说明: 熄灭指定的LED指示灯。
*	形    参: _no : 1-3 表示3个灯的id
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedOff(uint8_t _no)
{
	if (_no == 1)
	{
		GPIO_WriteHigh(LED1_PORT, (GPIO_Pin_TypeDef)LED1_PIN);
	}
	else if (_no == 2)
	{
		GPIO_WriteHigh(LED2_PORT, (GPIO_Pin_TypeDef)LED2_PIN);
	}
	else if (_no == 3)	
	{
		GPIO_WriteHigh(LED3_PORT, (GPIO_Pin_TypeDef)LED3_PIN);
	}	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_LedToggle
*	功能说明: 翻转指定的LED指示灯。
*	形    参: _no : 1-3 表示3个灯的id
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_LedToggle(uint8_t _no)
{
	if (_no == 1)
	{
		GPIO_WriteReverse(LED1_PORT, (GPIO_Pin_TypeDef)LED1_PIN);
	}
	else if (_no == 2)
	{
		GPIO_WriteReverse(LED2_PORT, (GPIO_Pin_TypeDef)LED2_PIN);
	}
	else if (_no == 3)	
	{
		GPIO_WriteReverse(LED3_PORT, (GPIO_Pin_TypeDef)LED3_PIN);
	}		
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
