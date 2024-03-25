/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.c
*	版    本 : V1.0
*	说    明 : 硬件底层驱动程序集合
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
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化硬件设备
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Init(void)
{
	bsp_InitTimer();	/* 初始化系统滴答定时器 */

	bsp_InitUart(9600); 	/* 初始化串口 */
	
	bsp_InitLed();			/* 初始化LED端口 */

	bsp_InitButton();		/* 初始化按键 */

	ADC_NTC_Init();

	Relay_Init();

	/* CPU复位后程所有的中断被禁止，必须通过RIM指令来使能它们*/
	enableInterrupts();	/* 使能中断。这个宏在 stm8s.h 中定义 */

	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer10ms
*	功能说明: 该函数每隔10ms被调用1次。在bsp_timer.c的定时中断调用。主要用于硬件事件检测
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_RunPer10ms(void)
{
	bsp_KeyPro();	/* 按键扫描 */
}


/*
*********************************************************************************************************
*	函 数 名: bsp_RunPer1ms
*	功能说明: 该函数每隔1ms被调用1次。在bsp_timer.c的定时中断调用。主要用于硬件事件检测
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
extern void LED8_Scan(void);
void bsp_RunPer1ms(void)
{
	;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲，延迟时执行的函数。用于喂狗。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{
	;
}


