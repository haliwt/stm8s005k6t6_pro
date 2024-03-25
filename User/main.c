/*
*********************************************************************************************************
*
*	模块名称 : 主程序
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : EK2-003_独立按键检测例程
*	修改记录 :
*		版本号  日期       作者    说明
*		V1.0    2014-09-18 发布1.0版本
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "stm8s.h"
#include "bsp.h"

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"EK2-003_独立按键检测例程"
#define EXAMPLE_DATE	"2014-09-18"
#define DEMO_VER		"1.0"

static void PrintfLogo(void);

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: C程序入口
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void main(void)
{
    uint8_t ucKeyCode;
	int16_t iCount = 0;

	bsp_Init();     /* 初始化底层硬件。 该函数在 bsp.c文件 */
	PrintfLogo();	/* 打印例程logo */

	printf("\r\n请按键,程序将打印出键值\r\n");
	printf("提示：\r\n");
	printf("   OK键按下点亮LED1，弹起熄灭LED1\r\n");
	printf("   K1键长按1秒产生1次常按事件,之后按照100ms的速度自动重发键值\r\n");
	printf("   K2键长按2秒产生1次常按事件,之后按照20ms的速度自动重发键值\r\n");	

	/* 设置 K1键长按1秒后，自动连发，连发速度（间隔几个10ms）为每100ms一个，也就是每秒10个 */
	//bsp_SetKeyParam(KID_K1,100, 10);
	
	/* 设置 K2键长按2秒后，自动连发，连发速度（间隔几个10ms）为每20ms一个，也就是每秒50个 */
	bsp_SetKeyParam(KID_K2,200, 2);
	
	while (1)
	{
		bsp_Idle();

        ucKeyCode = bsp_GetKey();
        if (ucKeyCode != KEY_NONE)
        {
			switch (ucKeyCode)
			{
				case KEYDOWN_SET:
				  	printf("K1键 按下 %d\r\n", iCount++);
			  		break;
		

				case KEYDOWN_OK:
					printf("K2键 按下 %d\r\n", iCount--);
			  		break;

				case KEYLONG_OK:
				  	printf("K2键 长按事件\r\n");
			  		break;							

					
			}
    	}
	}
}

/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* 友情提示：
		使用STVD开发时，printf/sprintf 入口参数 %d 必须是 int 类型(16位)，否则打印结果不正确。
		IAR无此问题。		
	*/
	
  	printf("\r\n");
	printf("*************************************************************\r\n");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	/* 打印ST固件库版本，这3个定义宏在stm8s.h文件中 */
	printf("* 固件库版本 : %d.%d.%d\r\n", (int)__STM8S_STDPERIPH_VERSION_MAIN,
			(int)__STM8S_STDPERIPH_VERSION_SUB1,(int)__STM8S_STDPERIPH_VERSION_SUB2);
	printf("* \n\r");	/* 打印一行空格 */
	printf("* QQ    : 1295744630 \r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* Copyright www.armfly.com 安富莱电子\r\n");
	printf("*************************************************************\n\r");
}



