/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_key.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#define BUTTON_COUNT	2		/* 3个普通按键 */

/* 定义按键IO */
/* 定义按键IO */
#define GPIO_KEY_SET		GPIOE
#define PIN_SET 	     	GPIO_PIN_5		/* 独立按键 key1 键  */

#define GPIO_KEY_OK			GPIOB
#define PIN_OK 				GPIO_PIN_0		/* 独立按键 key2 键 */

//#define GPIO_KEY3	GPIOC
//#define PIN_KEY3 	GPIO_PIN_3		/* 编码器OK键 */

/* 根据应用程序功能定义按键宏 */
#define KEYDOWN_SET	    KEY_SET_DOWN
#define KEYUP_K1		KEY_SET_UP
#define KEYLONG_K1     	KEY_SET_LONG

#define KEYDOWN_OK	    KEY_OK_DOWN
#define KEYUP_K2		KEY_OK_UP
#define KEYLONG_OK     	KEY_OK_LONG

//#define KEYDOWN_OK		KEY_3_DOWN
//#define KEYUP_OK		KEY_3_UP
//#define KEYLONG_OK		KEY_3_LONG

/* 按键滤波时间50ms, 单位10ms
 只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
*/
#define BUTTON_FILTER_TIME 	4
#define BUTTON_LONG_TIME 	300		/* 持续3秒，认为长按事件 */

/*
	每个按键对应1个全局的结构体变量。
	其成员变量是实现滤波和多种按键状态所必须的
*/
typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	uint8_t (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	uint8_t Count;			/* 滤波器计数器 */
	uint16_t LongCount;		/* 长按计数器 */
	uint16_t LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	uint8_t  State;			/* 按键当前状态（按下还是弹起） */
	uint8_t RepeatSpeed;	/* 连续按键周期 */
	uint8_t RepeatCount;	/* 连续按键计数器 */
}KEY_T;

/* 定义键值代码
	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_SET_DOWN,				/* 1键按下 */
	KEY_SET_UP,				/* 1键弹起 */
	KEY_SET_LONG,				/* 1键长按 */

	KEY_OK_DOWN,				/* 2键按下 */
	KEY_OK_UP,				/* 2键弹起 */
	KEY_OK_LONG,				/* 2键长按 */


}KEY_ENUM;

/* 按键ID */
enum
{
	KID_K1 = 0,
	KID_K2,
	KID_OK
};

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	20
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	uint8_t Read;					/* 缓冲区读指针 */
	uint8_t Write;					/* 缓冲区写指针 */
}KEY_FIFO_T;

/* 供外部调用的函数声明 */
void bsp_InitButton(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
void bsp_KeyPro(void);
uint8_t bsp_IsKeyPressed(uint8_t _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);

#endif


