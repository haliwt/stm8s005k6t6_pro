/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_key.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#define BUTTON_COUNT	2		/* 3����ͨ���� */

/* ���尴��IO */
/* ���尴��IO */
#define GPIO_KEY_SET		GPIOE
#define PIN_SET 	     	GPIO_PIN_5		/* �������� key1 ��  */

#define GPIO_KEY_OK			GPIOB
#define PIN_OK 				GPIO_PIN_0		/* �������� key2 �� */

//#define GPIO_KEY3	GPIOC
//#define PIN_KEY3 	GPIO_PIN_3		/* ������OK�� */

/* ����Ӧ�ó����ܶ��尴���� */
#define KEYDOWN_SET	    KEY_SET_DOWN
#define KEYUP_K1		KEY_SET_UP
#define KEYLONG_K1     	KEY_SET_LONG

#define KEYDOWN_OK	    KEY_OK_DOWN
#define KEYUP_K2		KEY_OK_UP
#define KEYLONG_OK     	KEY_OK_LONG

//#define KEYDOWN_OK		KEY_3_DOWN
//#define KEYUP_OK		KEY_3_UP
//#define KEYLONG_OK		KEY_3_LONG

/* �����˲�ʱ��50ms, ��λ10ms
 ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
*/
#define BUTTON_FILTER_TIME 	4
#define BUTTON_LONG_TIME 	300		/* ����3�룬��Ϊ�����¼� */

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	���Ա������ʵ���˲��Ͷ��ְ���״̬�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t RepeatSpeed;	/* ������������ */
	uint8_t RepeatCount;	/* �������������� */
}KEY_T;

/* �����ֵ����
	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_SET_DOWN,				/* 1������ */
	KEY_SET_UP,				/* 1������ */
	KEY_SET_LONG,				/* 1������ */

	KEY_OK_DOWN,				/* 2������ */
	KEY_OK_UP,				/* 2������ */
	KEY_OK_LONG,				/* 2������ */


}KEY_ENUM;

/* ����ID */
enum
{
	KID_K1 = 0,
	KID_K2,
	KID_OK
};

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	20
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ�� */
	uint8_t Write;					/* ������дָ�� */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitButton(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
void bsp_KeyPro(void);
uint8_t bsp_IsKeyPressed(uint8_t _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);

#endif


