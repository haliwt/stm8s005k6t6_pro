/*
*********************************************************************************************************
*
* be used to TIM1 as tick timer timing 
* Timer Timing interrupt is 1ms
*
*********************************************************************************************************
*/

#include "stm8s.h"
#include "bsp_timer.h"
#include "bsp.h"

#define ENABLE_SYSTICK_INT()	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE)//TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE)
#define DISABLE_SYSTICK_INT()	TIM1_ITConfig(TIM1_IT_UPDATE, DISABLE)//TIM4_ITConfig(TIM4_IT_UPDATE, DISABLE)

/*
	在此定义若干个软件定时器全局变量
	注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
*/
#define TMR_COUNT	4			/* 软件定时器的个数  */

static __IO uint32_t s_uiDelayCount = 0;
static __IO uint8_t s_ucTimeOutFlag = 0;

SOFT_TMR g_Tmr[TMR_COUNT];

/* 全局运行时间，单位1ms */
__IO uint32_t g_iRunTime = 0;

/* 保存 TIM2定时中断到后执行的回调函数指针 */
static void (*s_TIM2_CallBack1)(void);
static void (*s_TIM2_CallBack2)(void);
static void (*s_TIM2_CallBack3)(void);

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);
static void TIM1_Config(void);

/*
*********************************************************************************************************
*	函 数 名: SysTick_ISR
*	功能说明: SysTick中断服务程序，每隔2ms进入1次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/*
		STM8S 启动时，主时钟源默认为HSI RC时钟(16MHz)的8分频，即fHSI/8 = 2MHz
	*/
	bsp_ConfgiSysClk(CPU_SYS_CLK_INIT);		/* 修改系统时钟 */

	/* 清零所有的软件定时器 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		g_Tmr[i].Count = 0;
		g_Tmr[i].PreLoad = 0;
		g_Tmr[i].Flag = 0;
		g_Tmr[i].Mode = TMR_ONCE_MODE;	/* 缺省是1次性工作模式 */
	}

	TIM1_Config();
}

/*
*********************************************************************************************************
*	函 数 名: SysTick_ISR
*	功能说明: SysTick中断服务程序，每隔2ms进入1次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
extern void bsp_RunPer10ms(void);
extern void bsp_RunPer1ms(void);
void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;

	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
	//TIM4_ClearFlag(TIM4_FLAG_UPDATE);

	/* 每隔1ms进来1次 */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
        {
			s_ucTimeOutFlag = 1;
		}
	}

	/* 每隔1ms 进来一次 */
	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&g_Tmr[i]);
	}
	
	g_iRunTime++;	/* 全局运行时间每1ms增1,  24.8天一个循环 */

	bsp_RunPer1ms();	/* 此函数在 bsp.c 中实现(可以为空) */
		
	if (++s_count < (10 / SYSTICK_PERIOD))
	{
		return;
	}

	s_count = 0;

	bsp_RunPer10ms();	/* 此函数在 bsp.c 中实现(可以为空) */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SoftTimerDec
*	功能说明: 对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参：_tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
//	if (_tmr->Flag == 0)
	{
		if (_tmr->Count > 0)
		{
			/* 如果定时器变量减到1则设置定时器到达标志 */
			if (--_tmr->Count == 0)
			{
				_tmr->Flag = 1;

				/* 如果是自动模式，则自动重装计数器 */
				if(_tmr->Mode == TMR_AUTO_MODE)
				{
					_tmr->Count = _tmr->PreLoad;
				}
			}
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayMS
*	功能说明: ms级延迟，延迟精度为正负2ms
*	形    参：n : 延迟长度，单位1 ms。 n 应大于2
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
	if (n == 0)
	{
		return;
	}
	else if (n == 1)
	{
		n = 2;
	}

	DISABLE_SYSTICK_INT();  			/* 关中断 */
	s_uiDelayCount = n / SYSTICK_PERIOD;
	s_ucTimeOutFlag = 0;
	ENABLE_SYSTICK_INT();  				/* 开中断 */

	while (1)
	{
		bsp_Idle();

		/* 等待延迟时间到 */
		/* 注意：编译器认为 s_ucTimeOutFlag = 0，所以可能优化错误
		因此 s_ucTimeOutFlag 变量必须申明为_IO_型 */
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartTimer
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参：	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位10ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); 死机 */
		return;
	}

	_period = _period / SYSTICK_PERIOD;
	if (_period == 0)
	{
		_period = 1;
	}
	
	DISABLE_SYSTICK_INT();  			/* 关中断 */

	g_Tmr[_id].Count = _period;			/* 实时计数器初值 */
	g_Tmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	g_Tmr[_id].Flag = 0;				/* 定时时间到标志 */
	g_Tmr[_id].Mode = TMR_ONCE_MODE;	/* 1次性工作模式 */

	ENABLE_SYSTICK_INT();  				/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartAutoTimer
*	功能说明: 启动一个自动定时器，并设置定时周期。
*	形    参：	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*				_period : 定时周期，单位10ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); 死机 */
		return;
	}

	_period = _period / SYSTICK_PERIOD;
	if (_period == 0)
	{
		_period = 1;
	}
	
	DISABLE_SYSTICK_INT();  		/* 关中断 */

	g_Tmr[_id].Count = _period;			/* 实时计数器初值 */
	g_Tmr[_id].PreLoad = _period;		/* 计数器自动重装值，仅自动模式起作用 */
	g_Tmr[_id].Flag = 0;				/* 定时时间到标志 */
	g_Tmr[_id].Mode = TMR_AUTO_MODE;	/* 自动工作模式 */

	ENABLE_SYSTICK_INT();  		/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StopTimer
*	功能说明: 停止一个定时器
*	形    参：	_id     : 定时器ID，值域【1,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*						  定时器ID = 0 已用于bsp_DelayMS()函数
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); 死机 */
		return;
	}

	DISABLE_SYSTICK_INT();  	/* 关中断 */

	g_Tmr[_id].Count = 0;				/* 实时计数器初值 */
	g_Tmr[_id].Flag = 0;				/* 定时时间到标志 */
	g_Tmr[_id].Mode = TMR_ONCE_MODE;	/* 自动工作模式 */

	ENABLE_SYSTICK_INT();  		/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CheckTimer
*	功能说明: 检测定时器是否超时
*	形    参：	_id     : 定时器ID，值域【0,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*	返 回 值: 返回 0 表示定时未到， 1表示定时到
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	if (g_Tmr[_id].Flag == 1)
	{
		g_Tmr[_id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetRunTime
*	功能说明: 获取CPU运行时间
*	形    参：无
*	返 回 值: CPU运行时间，单位10ms
*********************************************************************************************************
*/
uint32_t bsp_GetRunTime(void)
{
	uint16_t runtime;

	DISABLE_SYSTICK_INT();  		/* 关中断 */

	runtime = g_iRunTime;	        /* 由于在Systick中断被改写，因此关中断进行保护 */

	ENABLE_SYSTICK_INT();  		    /* 开中断 */

	return runtime;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayUS
*	功能说明: 延迟若干us.  不准确，未验证
*	形    参：延迟时间，单位US； 不大于2000us。
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayUS(uint16_t _usDelay)
{
	/* 2014-6-22 实测 2000个延迟 1.9ms  (16M主频)*/
    while (_usDelay--)
	{
		nop(); nop(); nop(); nop(); nop(); nop(); nop();
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_ConfgiSysClk
*	功能说明: 配置CPU系统时钟
*	形    参: 枚举SYS_CLK_E ； 可选参数为:
*				HSI_2M = 0		内部高速RC振荡源16M, 分频倍率设置为 1/8 可得到2M
*				HSI_4M,			内部高速RC振荡源16M, 分频倍率设置为 1/4 可得到4M
*				HSI_8M,			内部高速RC振荡源16M, 分频倍率设置为 1/2 可得到8M
*				HSI_16M,		内部高速RC振荡源16M, 分频倍率设置为 1/1 可得到16M
*
*			  	LSI_128K,		内部低速RC时钟源，频率固定为128KHz
*
*				HSE_XTAL,		外部高速时钟 (频率由用于外接晶振决定，无分频选项)
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_ConfgiSysClk(SYS_CLK_E _iSysClk)
{
#if 1	/* 空间不足，优化代码, 只保留需要的时钟设置 */
	if (_iSysClk == HSI_16M)
	{
		/* 自动切换, 切换到内部高速时钟HSI, 禁止系统时钟中断, 关闭当前的时钟源  */
		CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  /* 设置内部高速RC时钟HSI为时钟源，分频系数为1 */
		while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* 等待HSI时钟稳定 */
	}
#else
	switch (_iSysClk)
	{
		case HSE_XTAL:
			/* 自动切换, 切换到外部高速时钟HSE, 禁止系统时钟中断, 关闭当前的时钟源  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			break;

		case LSI_128K:
			/* 自动切换, 切换到内部低速时钟LSI, 禁止系统时钟中断, 关闭当前的时钟源  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_LSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			break;

		case HSI_16M:
			/* 自动切换, 切换到内部高速时钟HSI, 禁止系统时钟中断, 关闭当前的时钟源  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  /* 设置内部高速RC时钟HSI为时钟源，分频系数为1 */
			while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* 等待HSI时钟稳定 */
			break;

		case HSI_8M:
			/* 自动切换, 切换到内部高速时钟HSI, 禁止系统时钟中断, 关闭当前的时钟源  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);  /* 设置内部高速RC时钟HSI为时钟源，分频系数为2 */
			while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* 等待HSI时钟稳定 */
			break;

		case HSI_4M:
			/* 自动切换, 切换到内部高速时钟HSI, 禁止系统时钟中断, 关闭当前的时钟源  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV4);  /* 设置内部高速RC时钟HSI为时钟源，分频系数为4 */
			while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* 等待HSI时钟稳定 */
			break;

		case HSI_2M:
			/* 自动切换, 切换到内部高速时钟HSI, 禁止系统时钟中断, 关闭当前的时钟源  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);  /* 设置内部高速RC时钟HSI为时钟源，分频系数为8 */
			while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* 等待HSI时钟稳定 */
			break;

		default:
			break;
	}
#endif
}

/*
*********************************************************************************************************
*	Function Name: TIM1_Config
*	Function: timer timing is interrupt is 1ms
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TIM1_Config(void)
{
    /* TIM1 Peripheral Configuration */ 
  	TIM1_DeInit();   

	/* Time Base configuration */
    TIM1_TimeBaseInit(0,TIM1_COUNTERMODE_UP,16000, 0); //F=16MHz,1ms.

	
	/* Clear TIM1 update flag */
	TIM1_ClearFlag(TIM1_FLAG_UPDATE);

	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);  /* 使能TIM4中断 */
	//TIM1_UpdateDisableConfig(ENABLE);		/* 使能TIM4自动溢出事件 */

	 /* Time Base configuration */
    TIM1_ARRPreloadConfig(ENABLE);

	TIM1_Cmd(ENABLE);						/* 使能TIM4 */


}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitHardTimer
*	功能说明: 配置TIM2，16分频，用于us级别定时。 TIM2将自由运行
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitHardTimer(void)
{
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, 0xFFFF);	/* 16分频后，计数单位为1us */
	TIM2_Cmd(ENABLE);						/* 使能TIM2 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartHardTimer
*	功能说明: 使用TIM2做单次定时器使用, 定时时间到后执行回调函数。可以同时启动3个定时器，互不干扰。
*             定时精度正负20us （主要耗费在调用本函数的执行时间，函数内部进行了补偿减小误差）
*	形    参：_CC : 捕获通道几，1，2，3
*             _usTimeOut : 超时时间, 单位 us.  范围 20us ---> 65 000us. 
*             _pCallBack : 定时时间到后，被执行的函数
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint8_t _CC, uint16_t _usTimeOut, void * _pCallBack)
{
    uint16_t cnt_now;
    uint16_t cnt_tar;

    /* 
        执行下面这个语句，时长 = 18us (通过逻辑分析仪测量IO翻转) 
        bsp_StartTimer2(3, 500, (void *)test1); 
    */
    if (_usTimeOut < 20)
    {
        ;
    }
    else
    {
        _usTimeOut -= 16;
    }

    cnt_now = TIM2_GetCounter();    /* 读取当前的计数器值 */
    cnt_tar = cnt_now + _usTimeOut;          /* 计算捕获的计数器值 */
    if (_CC == 1)
    {
        TIM2_SetCompare1(cnt_tar);      /* 设置捕获比较计数器CC1 */
        s_TIM2_CallBack1 = (void (*)(void))_pCallBack;
        TIM2_ITConfig(TIM2_IT_CC1, ENABLE);  /* 使能TIM2中断 */
        TIM2_ClearITPendingBit(TIM2_IT_CC1);
    }
    else if (_CC == 2)
    {
        TIM2_SetCompare2(cnt_tar);      /* 设置捕获比较计数器CC2 */
        s_TIM2_CallBack2 = (void (*)(void))_pCallBack;
        TIM2_ITConfig(TIM2_IT_CC2, ENABLE);  /* 使能TIM2中断 */
        TIM2_ClearITPendingBit(TIM2_IT_CC2);
    }
    else if (_CC == 3)
    {
        TIM2_SetCompare3(cnt_tar);      /* 设置捕获比较计数器CC3 */
        s_TIM2_CallBack3 = (void (*)(void))_pCallBack;
        TIM2_ITConfig(TIM2_IT_CC3, ENABLE);  /* 使能TIM2中断 */
        TIM2_ClearITPendingBit(TIM2_IT_CC3);
    }
    else
    {
        return;    
    }	
}

/*
*********************************************************************************************************
*	函 数 名: TIM2_ISR
*	功能说明: TIM2中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void TIM2_ISR(void)
{
    if (TIM2_GetITStatus(TIM2_IT_CC1))
    {
        TIM2_ClearITPendingBit(TIM2_IT_CC1);
        TIM2_ITConfig(TIM2_IT_CC1, DISABLE);  /* 禁能CC1中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM2_CallBack1();
    }
    if (TIM2_GetITStatus(TIM2_IT_CC2))
    {
    
        TIM2_ClearITPendingBit(TIM2_IT_CC2);
        TIM2_ITConfig(TIM2_IT_CC2, DISABLE);  /* 禁能CC2中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM2_CallBack2();
    }
    
    if (TIM2_GetITStatus(TIM2_IT_CC3))
    {
        TIM2_ClearITPendingBit(TIM2_IT_CC3);
        TIM2_ITConfig(TIM2_IT_CC3, DISABLE);  /* 禁能CC3中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM2_CallBack3();
    }
}

