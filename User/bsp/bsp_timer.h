/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

/*
	为了降低代码容量，Cout  PreLoad 由 uint32_t 修改为 uint16_t
*/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#define SYSTICK_PERIOD	1				/* 系统滴答周期设置为2ms， 单位ms （对于系统时钟16M，最大2ms）*/

/* 可选的CPU系统时钟源 */
typedef enum
{
  	HSI_2M = 0,			/* 内部高速RC振荡源16M, 分频倍率设置为 1/8 可得到2M */
  	HSI_4M,				/* 内部高速RC振荡源16M, 分频倍率设置为 1/4 可得到4M */
  	HSI_8M,				/* 内部高速RC振荡源16M, 分频倍率设置为 1/2 可得到8M */
  	HSI_16M,			/* 内部高速RC振荡源16M, 分频倍率设置为 1/1 可得到16M */

  	LSI_128K,			/* 内部低速RC时钟源，频率固定为128KHz */

		HSE_XTAL			/* 外部高速时钟 (频率由用于外接晶振决定，无分频选项)
					特别注意，如果使用外部时钟并且板子的晶振不是16M，请修改 stm8s.h 中 HSE_VALUE */
}SYS_CLK_E;

#define CPU_SYS_CLK_INIT	HSI_16M

enum
{
	TMR_ONCE_MODE = 0,	/* 一次工作模式 */
	TMR_AUTO_MODE = 1		/* 自动定时工作模式 */
};

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint8_t Mode;		/* 计数器模式，1次性 */
	volatile uint8_t Flag;		/* 定时到达标志  */
	volatile uint16_t Count;	/* 计数器 */
	volatile uint16_t PreLoad;	/* 计数器预装值 */
}SOFT_TMR;

/* 供外部调用的函数声明 */
void bsp_InitTimer(void);
void bsp_DelayMS(uint32_t n);
void bsp_DelayUS(uint16_t _usDelay);
void bsp_StartTimer(uint8_t _id, uint32_t _period);
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period);
void bsp_StopTimer(uint8_t _id);
uint8_t bsp_CheckTimer(uint8_t _id);
uint32_t bsp_GetRunTime(void);
void bsp_ConfgiSysClk(SYS_CLK_E);

void bsp_InitHardTimer(void);
void bsp_StartHardTimer(uint8_t _CC, uint16_t _usTimeOut, void * _pCallBack);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

