/*
*********************************************************************************************************
*
*	ģ������ : LEDָʾ������ģ��
*	�ļ����� : bsp_led.c
*	��    �� : V1.0
*	˵    �� : ����LEDָʾ��
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2014-09-18 armfly  ST�̼���汾 V2.1.0
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

/* 
	PE5 : LED1ָʾ�� �� ����FLASH��Ƭѡ���� 
	PG0 : LED2ָʾ�� ��CH376оƬ��Ƭѡ����
	PD3 : LED3ָʾ�� ��TFT��ʾ���ӿ��еĴ�����SPI��Ƭѡ����
*/
#define LED1_PORT	GPIOE
#define LED1_PIN 	GPIO_PIN_5

#define LED2_PORT	GPIOG	
#define LED2_PIN 	GPIO_PIN_0

#define LED3_PORT	GPIOD			/* ����ָʾ��LED�Ķ˿� */
#define LED3_PIN 	GPIO_PIN_3	/* ����ָʾ��LED��bit */

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitLed
*	����˵��: ��ʼ��LEDָʾ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitLed(void)
{
	/* ��ʼ��LED����ָʾ�ƽӿڣ�����Ϊ������� */
	GPIO_Init(LED1_PORT, (GPIO_Pin_TypeDef)LED1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LED2_PORT, (GPIO_Pin_TypeDef)LED2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LED3_PORT, (GPIO_Pin_TypeDef)LED3_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_LedOn
*	����˵��: ����ָ����LEDָʾ�ơ�
*	��    ��: _no : 1-3 ��ʾ3���Ƶ�id
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedOff
*	����˵��: Ϩ��ָ����LEDָʾ�ơ�
*	��    ��: _no : 1-3 ��ʾ3���Ƶ�id
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_LedToggle
*	����˵��: ��תָ����LEDָʾ�ơ�
*	��    ��: _no : 1-3 ��ʾ3���Ƶ�id
*	�� �� ֵ: ��
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

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
