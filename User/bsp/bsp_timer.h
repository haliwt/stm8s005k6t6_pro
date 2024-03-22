/*
*********************************************************************************************************
*
*	ģ������ : ��ʱ��ģ��
*	�ļ����� : bsp_timer.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/

/*
	Ϊ�˽��ʹ���������Cout  PreLoad �� uint32_t �޸�Ϊ uint16_t
*/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#define SYSTICK_PERIOD	1				/* ϵͳ�δ���������Ϊ2ms�� ��λms ������ϵͳʱ��16M�����2ms��*/

/* ��ѡ��CPUϵͳʱ��Դ */
typedef enum
{
  	HSI_2M = 0,			/* �ڲ�����RC��Դ16M, ��Ƶ��������Ϊ 1/8 �ɵõ�2M */
  	HSI_4M,				/* �ڲ�����RC��Դ16M, ��Ƶ��������Ϊ 1/4 �ɵõ�4M */
  	HSI_8M,				/* �ڲ�����RC��Դ16M, ��Ƶ��������Ϊ 1/2 �ɵõ�8M */
  	HSI_16M,			/* �ڲ�����RC��Դ16M, ��Ƶ��������Ϊ 1/1 �ɵõ�16M */

  	LSI_128K,			/* �ڲ�����RCʱ��Դ��Ƶ�ʹ̶�Ϊ128KHz */

		HSE_XTAL			/* �ⲿ����ʱ�� (Ƶ����������Ӿ���������޷�Ƶѡ��)
					�ر�ע�⣬���ʹ���ⲿʱ�Ӳ��Ұ��ӵľ�����16M�����޸� stm8s.h �� HSE_VALUE */
}SYS_CLK_E;

#define CPU_SYS_CLK_INIT	HSI_16M

enum
{
	TMR_ONCE_MODE = 0,	/* һ�ι���ģʽ */
	TMR_AUTO_MODE = 1		/* �Զ���ʱ����ģʽ */
};

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint8_t Mode;		/* ������ģʽ��1���� */
	volatile uint8_t Flag;		/* ��ʱ�����־  */
	volatile uint16_t Count;	/* ������ */
	volatile uint16_t PreLoad;	/* ������Ԥװֵ */
}SOFT_TMR;

/* ���ⲿ���õĺ������� */
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

