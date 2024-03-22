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
	�ڴ˶������ɸ������ʱ��ȫ�ֱ���
	ע�⣬��������__IO �� volatile����Ϊ����������жϺ���������ͬʱ�����ʣ��п�����ɱ����������Ż���
*/
#define TMR_COUNT	4			/* �����ʱ���ĸ���  */

static __IO uint32_t s_uiDelayCount = 0;
static __IO uint8_t s_ucTimeOutFlag = 0;

SOFT_TMR g_Tmr[TMR_COUNT];

/* ȫ������ʱ�䣬��λ1ms */
__IO uint32_t g_iRunTime = 0;

/* ���� TIM2��ʱ�жϵ���ִ�еĻص�����ָ�� */
static void (*s_TIM2_CallBack1)(void);
static void (*s_TIM2_CallBack2)(void);
static void (*s_TIM2_CallBack3)(void);

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);
static void TIM1_Config(void);

/*
*********************************************************************************************************
*	�� �� ��: SysTick_ISR
*	����˵��: SysTick�жϷ������ÿ��2ms����1��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitTimer(void)
{
	uint8_t i;

	/*
		STM8S ����ʱ����ʱ��ԴĬ��ΪHSI RCʱ��(16MHz)��8��Ƶ����fHSI/8 = 2MHz
	*/
	bsp_ConfgiSysClk(CPU_SYS_CLK_INIT);		/* �޸�ϵͳʱ�� */

	/* �������е������ʱ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		g_Tmr[i].Count = 0;
		g_Tmr[i].PreLoad = 0;
		g_Tmr[i].Flag = 0;
		g_Tmr[i].Mode = TMR_ONCE_MODE;	/* ȱʡ��1���Թ���ģʽ */
	}

	TIM1_Config();
}

/*
*********************************************************************************************************
*	�� �� ��: SysTick_ISR
*	����˵��: SysTick�жϷ������ÿ��2ms����1��
*	��    �Σ���
*	�� �� ֵ: ��
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

	/* ÿ��1ms����1�� */
	if (s_uiDelayCount > 0)
	{
		if (--s_uiDelayCount == 0)
        {
			s_ucTimeOutFlag = 1;
		}
	}

	/* ÿ��1ms ����һ�� */
	for (i = 0; i < TMR_COUNT; i++)
	{
		bsp_SoftTimerDec(&g_Tmr[i]);
	}
	
	g_iRunTime++;	/* ȫ������ʱ��ÿ1ms��1,  24.8��һ��ѭ�� */

	bsp_RunPer1ms();	/* �˺����� bsp.c ��ʵ��(����Ϊ��) */
		
	if (++s_count < (10 / SYSTICK_PERIOD))
	{
		return;
	}

	s_count = 0;

	bsp_RunPer10ms();	/* �˺����� bsp.c ��ʵ��(����Ϊ��) */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SoftTimerDec
*	����˵��: �����ж�ʱ��������1�����뱻SysTick_ISR�����Ե��á�
*	��    �Σ�_tmr : ��ʱ������ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
//	if (_tmr->Flag == 0)
	{
		if (_tmr->Count > 0)
		{
			/* �����ʱ����������1�����ö�ʱ�������־ */
			if (--_tmr->Count == 0)
			{
				_tmr->Flag = 1;

				/* ������Զ�ģʽ�����Զ���װ������ */
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
*	�� �� ��: bsp_DelayMS
*	����˵��: ms���ӳ٣��ӳپ���Ϊ����2ms
*	��    �Σ�n : �ӳٳ��ȣ���λ1 ms�� n Ӧ����2
*	�� �� ֵ: ��
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

	DISABLE_SYSTICK_INT();  			/* ���ж� */
	s_uiDelayCount = n / SYSTICK_PERIOD;
	s_ucTimeOutFlag = 0;
	ENABLE_SYSTICK_INT();  				/* ���ж� */

	while (1)
	{
		bsp_Idle();

		/* �ȴ��ӳ�ʱ�䵽 */
		/* ע�⣺��������Ϊ s_ucTimeOutFlag = 0�����Կ����Ż�����
		��� s_ucTimeOutFlag ������������Ϊ_IO_�� */
		if (s_ucTimeOutFlag == 1)
		{
			break;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartTimer
*	����˵��: ����һ����ʱ���������ö�ʱ���ڡ�
*	��    �Σ�	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ10ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); ���� */
		return;
	}

	_period = _period / SYSTICK_PERIOD;
	if (_period == 0)
	{
		_period = 1;
	}
	
	DISABLE_SYSTICK_INT();  			/* ���ж� */

	g_Tmr[_id].Count = _period;			/* ʵʱ��������ֵ */
	g_Tmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	g_Tmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	g_Tmr[_id].Mode = TMR_ONCE_MODE;	/* 1���Թ���ģʽ */

	ENABLE_SYSTICK_INT();  				/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartAutoTimer
*	����˵��: ����һ���Զ���ʱ���������ö�ʱ���ڡ�
*	��    �Σ�	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*				_period : ��ʱ���ڣ���λ10ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartAutoTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); ���� */
		return;
	}

	_period = _period / SYSTICK_PERIOD;
	if (_period == 0)
	{
		_period = 1;
	}
	
	DISABLE_SYSTICK_INT();  		/* ���ж� */

	g_Tmr[_id].Count = _period;			/* ʵʱ��������ֵ */
	g_Tmr[_id].PreLoad = _period;		/* �������Զ���װֵ�����Զ�ģʽ������ */
	g_Tmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	g_Tmr[_id].Mode = TMR_AUTO_MODE;	/* �Զ�����ģʽ */

	ENABLE_SYSTICK_INT();  		/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StopTimer
*	����˵��: ֹͣһ����ʱ��
*	��    �Σ�	_id     : ��ʱ��ID��ֵ��1,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*						  ��ʱ��ID = 0 ������bsp_DelayMS()����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StopTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		/* while(1); ���� */
		return;
	}

	DISABLE_SYSTICK_INT();  	/* ���ж� */

	g_Tmr[_id].Count = 0;				/* ʵʱ��������ֵ */
	g_Tmr[_id].Flag = 0;				/* ��ʱʱ�䵽��־ */
	g_Tmr[_id].Mode = TMR_ONCE_MODE;	/* �Զ�����ģʽ */

	ENABLE_SYSTICK_INT();  		/* ���ж� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CheckTimer
*	����˵��: ��ⶨʱ���Ƿ�ʱ
*	��    �Σ�	_id     : ��ʱ��ID��ֵ��0,TMR_COUNT-1�����û���������ά����ʱ��ID���Ա��ⶨʱ��ID��ͻ��
*	�� �� ֵ: ���� 0 ��ʾ��ʱδ���� 1��ʾ��ʱ��
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
*	�� �� ��: bsp_GetRunTime
*	����˵��: ��ȡCPU����ʱ��
*	��    �Σ���
*	�� �� ֵ: CPU����ʱ�䣬��λ10ms
*********************************************************************************************************
*/
uint32_t bsp_GetRunTime(void)
{
	uint16_t runtime;

	DISABLE_SYSTICK_INT();  		/* ���ж� */

	runtime = g_iRunTime;	        /* ������Systick�жϱ���д����˹��жϽ��б��� */

	ENABLE_SYSTICK_INT();  		    /* ���ж� */

	return runtime;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DelayUS
*	����˵��: �ӳ�����us.  ��׼ȷ��δ��֤
*	��    �Σ��ӳ�ʱ�䣬��λUS�� ������2000us��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_DelayUS(uint16_t _usDelay)
{
	/* 2014-6-22 ʵ�� 2000���ӳ� 1.9ms  (16M��Ƶ)*/
    while (_usDelay--)
	{
		nop(); nop(); nop(); nop(); nop(); nop(); nop();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ConfgiSysClk
*	����˵��: ����CPUϵͳʱ��
*	��    ��: ö��SYS_CLK_E �� ��ѡ����Ϊ:
*				HSI_2M = 0		�ڲ�����RC��Դ16M, ��Ƶ��������Ϊ 1/8 �ɵõ�2M
*				HSI_4M,			�ڲ�����RC��Դ16M, ��Ƶ��������Ϊ 1/4 �ɵõ�4M
*				HSI_8M,			�ڲ�����RC��Դ16M, ��Ƶ��������Ϊ 1/2 �ɵõ�8M
*				HSI_16M,		�ڲ�����RC��Դ16M, ��Ƶ��������Ϊ 1/1 �ɵõ�16M
*
*			  	LSI_128K,		�ڲ�����RCʱ��Դ��Ƶ�ʹ̶�Ϊ128KHz
*
*				HSE_XTAL,		�ⲿ����ʱ�� (Ƶ����������Ӿ���������޷�Ƶѡ��)
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_ConfgiSysClk(SYS_CLK_E _iSysClk)
{
#if 1	/* �ռ䲻�㣬�Ż�����, ֻ������Ҫ��ʱ������ */
	if (_iSysClk == HSI_16M)
	{
		/* �Զ��л�, �л����ڲ�����ʱ��HSI, ��ֹϵͳʱ���ж�, �رյ�ǰ��ʱ��Դ  */
		CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
		CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  /* �����ڲ�����RCʱ��HSIΪʱ��Դ����Ƶϵ��Ϊ1 */
		while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* �ȴ�HSIʱ���ȶ� */
	}
#else
	switch (_iSysClk)
	{
		case HSE_XTAL:
			/* �Զ��л�, �л����ⲿ����ʱ��HSE, ��ֹϵͳʱ���ж�, �رյ�ǰ��ʱ��Դ  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			break;

		case LSI_128K:
			/* �Զ��л�, �л����ڲ�����ʱ��LSI, ��ֹϵͳʱ���ж�, �رյ�ǰ��ʱ��Դ  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_LSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			break;

		case HSI_16M:
			/* �Զ��л�, �л����ڲ�����ʱ��HSI, ��ֹϵͳʱ���ж�, �رյ�ǰ��ʱ��Դ  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);  /* �����ڲ�����RCʱ��HSIΪʱ��Դ����Ƶϵ��Ϊ1 */
			while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* �ȴ�HSIʱ���ȶ� */
			break;

		case HSI_8M:
			/* �Զ��л�, �л����ڲ�����ʱ��HSI, ��ֹϵͳʱ���ж�, �رյ�ǰ��ʱ��Դ  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV2);  /* �����ڲ�����RCʱ��HSIΪʱ��Դ����Ƶϵ��Ϊ2 */
			while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* �ȴ�HSIʱ���ȶ� */
			break;

		case HSI_4M:
			/* �Զ��л�, �л����ڲ�����ʱ��HSI, ��ֹϵͳʱ���ж�, �رյ�ǰ��ʱ��Դ  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV4);  /* �����ڲ�����RCʱ��HSIΪʱ��Դ����Ƶϵ��Ϊ4 */
			while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* �ȴ�HSIʱ���ȶ� */
			break;

		case HSI_2M:
			/* �Զ��л�, �л����ڲ�����ʱ��HSI, ��ֹϵͳʱ���ж�, �رյ�ǰ��ʱ��Դ  */
			CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
			CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV8);  /* �����ڲ�����RCʱ��HSIΪʱ��Դ����Ƶϵ��Ϊ8 */
			while (CLK_GetFlagStatus(CLK_FLAG_HSIRDY) == RESET);	/* �ȴ�HSIʱ���ȶ� */
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
*	��    �Σ���
*	�� �� ֵ: ��
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

	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);  /* ʹ��TIM4�ж� */
	//TIM1_UpdateDisableConfig(ENABLE);		/* ʹ��TIM4�Զ�����¼� */

	 /* Time Base configuration */
    TIM1_ARRPreloadConfig(ENABLE);

	TIM1_Cmd(ENABLE);						/* ʹ��TIM4 */


}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitHardTimer
*	����˵��: ����TIM2��16��Ƶ������us����ʱ�� TIM2����������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitHardTimer(void)
{
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, 0xFFFF);	/* 16��Ƶ�󣬼�����λΪ1us */
	TIM2_Cmd(ENABLE);						/* ʹ��TIM2 */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_StartHardTimer
*	����˵��: ʹ��TIM2�����ζ�ʱ��ʹ��, ��ʱʱ�䵽��ִ�лص�����������ͬʱ����3����ʱ�����������š�
*             ��ʱ��������20us ����Ҫ�ķ��ڵ��ñ�������ִ��ʱ�䣬�����ڲ������˲�����С��
*	��    �Σ�_CC : ����ͨ������1��2��3
*             _usTimeOut : ��ʱʱ��, ��λ us.  ��Χ 20us ---> 65 000us. 
*             _pCallBack : ��ʱʱ�䵽�󣬱�ִ�еĺ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_StartHardTimer(uint8_t _CC, uint16_t _usTimeOut, void * _pCallBack)
{
    uint16_t cnt_now;
    uint16_t cnt_tar;

    /* 
        ִ�����������䣬ʱ�� = 18us (ͨ���߼������ǲ���IO��ת) 
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

    cnt_now = TIM2_GetCounter();    /* ��ȡ��ǰ�ļ�����ֵ */
    cnt_tar = cnt_now + _usTimeOut;          /* ���㲶��ļ�����ֵ */
    if (_CC == 1)
    {
        TIM2_SetCompare1(cnt_tar);      /* ���ò���Ƚϼ�����CC1 */
        s_TIM2_CallBack1 = (void (*)(void))_pCallBack;
        TIM2_ITConfig(TIM2_IT_CC1, ENABLE);  /* ʹ��TIM2�ж� */
        TIM2_ClearITPendingBit(TIM2_IT_CC1);
    }
    else if (_CC == 2)
    {
        TIM2_SetCompare2(cnt_tar);      /* ���ò���Ƚϼ�����CC2 */
        s_TIM2_CallBack2 = (void (*)(void))_pCallBack;
        TIM2_ITConfig(TIM2_IT_CC2, ENABLE);  /* ʹ��TIM2�ж� */
        TIM2_ClearITPendingBit(TIM2_IT_CC2);
    }
    else if (_CC == 3)
    {
        TIM2_SetCompare3(cnt_tar);      /* ���ò���Ƚϼ�����CC3 */
        s_TIM2_CallBack3 = (void (*)(void))_pCallBack;
        TIM2_ITConfig(TIM2_IT_CC3, ENABLE);  /* ʹ��TIM2�ж� */
        TIM2_ClearITPendingBit(TIM2_IT_CC3);
    }
    else
    {
        return;    
    }	
}

/*
*********************************************************************************************************
*	�� �� ��: TIM2_ISR
*	����˵��: TIM2�жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TIM2_ISR(void)
{
    if (TIM2_GetITStatus(TIM2_IT_CC1))
    {
        TIM2_ClearITPendingBit(TIM2_IT_CC1);
        TIM2_ITConfig(TIM2_IT_CC1, DISABLE);  /* ����CC1�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM2_CallBack1();
    }
    if (TIM2_GetITStatus(TIM2_IT_CC2))
    {
    
        TIM2_ClearITPendingBit(TIM2_IT_CC2);
        TIM2_ITConfig(TIM2_IT_CC2, DISABLE);  /* ����CC2�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM2_CallBack2();
    }
    
    if (TIM2_GetITStatus(TIM2_IT_CC3))
    {
        TIM2_ClearITPendingBit(TIM2_IT_CC3);
        TIM2_ITConfig(TIM2_IT_CC3, DISABLE);  /* ����CC3�ж� */

        /* �ȹر��жϣ���ִ�лص���������Ϊ�ص�����������Ҫ������ʱ�� */
        s_TIM2_CallBack3();
    }
}

