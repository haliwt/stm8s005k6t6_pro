/*
*********************************************************************************************************
*
*	模块名称 : 底层驱动模块
*	文件名称 : bsp.h
*	版    本 : V1.0
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。
*	 	       应用程序只需 #include bsp.h 即可，不需要#include 每个模块的 h 文件
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H

#include "stm8s.h"
#include "stdio.h"

#include "bsp_timer.h"
//#include "bsp_watchdog.h"
#include "bsp_uart.h"
#include "bsp_key.h"
//#include "bsp_stm8_eeprom.h"
#include "bsp_led.h"

void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
