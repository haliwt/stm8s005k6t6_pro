/*
*********************************************************************************************************
*
*	模块名称 : 按键驱动模块
*	文件名称 : bsp_button.c
*	版    本 : V1.2
*	说    明 : 实现按键的检测，具有软件滤波机制，可以检测如下事件：
*				(1) 按键按下
*				(2) 按键弹起
*				(3) 长按键
*				(4) 长按时自动连发
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2012-10-12 armfly  ST固件库版本 V2.1.0
*		V1.1    2013-09-18 armfly  添加 bsp_SetKeyParam() 函数 bsp_ClearKey() 函数
*
*	Copyright (C), 2014-2015, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

static KEY_T s_tBtn[BUTTON_COUNT];

static KEY_FIFO_T s_Key;		/* 按键FIFO变量,结构体 */

static void bsp_InitButtonVar(void);
static void bsp_InitButtonHard(void);
static void bsp_DetectButton(uint8_t _ucIndex);

/*
*********************************************************************************************************
*	函 数 名: IsKeyDownX
*	功能说明: 判断按键是否按下
*	形    参: 无
*	返 回 值: 返回值1 表示按下，0表示未按下
*********************************************************************************************************
*/
static uint8_t IsKeyDownSet(void)
{ 
	if ((GPIO_KEY_SET->IDR & PIN_SET) == 0) return 1; else return 0;
};
static uint8_t IsKeyDownOK(void){ if ((GPIO_KEY_OK->IDR & PIN_OK) == 0) return 1; else return 0;};
//static uint8_t IsKeyDown3(void){ if ((GPIO_KEY3->IDR & PIN_KEY3) == 0) return 1; else return 0;};

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButton
*	功能说明: 初始化按键
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitButton(void)
{
	bsp_InitButtonVar();		/* 初始化按键变量 */
	bsp_InitButtonHard();		/* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参: _KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE) //20
	{
		s_Key.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参: 无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];

		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参: 无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_Key.Read = s_Key.Write;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_IsKeyPressed
*	功能说明: 检测按键是否按下
*	形    参: _ucKeyID : 按键ID，从0开始
*	返 回 值: 1 表示按下， 0 表示未按下
*********************************************************************************************************
*/
uint8_t bsp_IsKeyPressed(uint8_t _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_SetKeyParam
*	功能说明: 设置按键参数
*	形    参: _ucKeyID : 按键ID，从0开始
*			_LongTime : 长按事件时间
*			 _RepeatSpeed : 连发速度。间隔几个10ms重发一个消息。
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* 长按时间 0 表示不检测长按键事件 */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* 按键连发的速度，0表示不支持连发 */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* 连发计数器 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonHard
*	功能说明: 初始化按键硬件
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitButtonHard(void)
{
	/* 配置按键采样口为输入上拉，无中断 */
	GPIO_Init(GPIO_KEY_SET, (GPIO_Pin_TypeDef)PIN_SET, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(GPIO_KEY_OK, (GPIO_Pin_TypeDef)PIN_OK, GPIO_MODE_IN_PU_NO_IT);
	//GPIO_Init(GPIO_KEY3, (GPIO_Pin_TypeDef)PIN_KEY3, GPIO_MODE_IN_PU_NO_IT);	
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonVar
*	功能说明: 初始化按键变量
*	形    参: strName : 例程名称字符串
*			  strDate : 例程发布日期
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitButtonVar(void)
{
	uint8_t i;

	/* 对按键FIFO读写指针清零 */
	s_Key.Read = 0;
	s_Key.Write = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < BUTTON_COUNT; i++)
	{
		s_tBtn[i].LongTime = 0;						/* 长按时间 0 表示不检测长按键事件 */
		s_tBtn[i].Count = BUTTON_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
		s_tBtn[i].State = 0;							/* 按键缺省状态，0为未按下 */
		s_tBtn[i].RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
		s_tBtn[i].RepeatCount = 0;						/* 连发计数器 */
	}

	/* 判断按键按下的函数 */
	s_tBtn[0].IsKeyDownFunc = IsKeyDownSet;
	s_tBtn[1].IsKeyDownFunc = IsKeyDownOK;
	//s_tBtn[2].IsKeyDownFunc = IsKeyDown3;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectButton
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参: 按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectButton(uint8_t i)
{
	KEY_T *pBtn;

	/* 如果没有初始化按键函数，则报错
	if (s_tBtn[i].IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
	}
	*/

	pBtn = &s_tBtn[i];
	if (pBtn->IsKeyDownFunc()) //函数指针调用，结构体成员函数调用
	{
		if (pBtn->Count < BUTTON_FILTER_TIME) //BUTTON_FILTER_TIME=4
		{
			pBtn->Count = BUTTON_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * BUTTON_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1; //按键按下

				/* 发送按钮按下的消息 */
				bsp_PutKey((uint8_t)(3 * i + 1)); //1,4,7-> 短按按键 3,6,9-> 长按按键 2,5,8-> 弹起按键
			}

			if (pBtn->LongTime > 0) //开机初始化定义，按键长按的时间bsp_SetKeyParam（ ）
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* 发送按钮持续按下的消息 */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* 键值放入按键FIFO */
						bsp_PutKey((uint8_t)(3 * i + 3)); //长按按键：3,6,9-> 短按按键 1,4,7-> 长按按键 2,5,8-> 弹起按键
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* 常按键后，每隔10ms发送1个按键 */
							bsp_PutKey((uint8_t)(3 * i + 1));
						}
					}
				}
			}
		}
	}
	else
	{
		if(pBtn->Count > BUTTON_FILTER_TIME) //BUTTON_FILTER_TIME = 4
		{
			pBtn->Count = BUTTON_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				/* 发送按钮弹起的消息 */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyPro
*	功能说明: 检测所有按键。非阻塞状态，必须被周期性的调用。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_KeyPro(void)
{
	uint8_t i;

	for (i = 0; i < BUTTON_COUNT; i++)
	{
		bsp_DetectButton(i);
	}
}



