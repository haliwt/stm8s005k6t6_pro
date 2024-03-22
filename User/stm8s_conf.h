/*
*********************************************************************************************************
*
*	模块名称 : STM8固件库配置文件。
*	文件名称 : stm8s_conf.h
*	版    本 : V2.1.0
*	日    期 : 2011-11-18
*	说    明 :	这是ST固件库提供的文件。用户可以根据需要包含ST固件库的外设模块。为了方便我们包含了所有固件
*				库模块。
*
*			 	这个文件被 \Libraries\STM8S_StdPeriph_Driver\inc\stm8s.h 包含，因此我们在.c文件中只需要
*				include "stm32s.h"即可，不必单独再include 这个文件
*
*	Copyright (C), 2012-2013, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __STM8S_CONF_H
#define __STM8S_CONF_H

#include "stm8s.h"

/*
	在编译器选项中需要指定CPU系列。不同的系列需要包含不同的外设模块。

	友情提示：我们根据这组定义可以知道不同系列的CPU之间的外设差异
*/

#if defined(STM8S105) || defined(STM8S005) || defined(STM8S103) || defined(STM8S003) ||\
    defined(STM8S903) || defined (STM8AF626x)
#include "stm8s_adc1.h"
#endif /* (STM8S105) ||(STM8S103) || (STM8S903) || STM8AF626x*/

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined (STM8AF52Ax) ||\
    defined (STM8AF62Ax)
 #include "stm8s_adc2.h"
#endif /* (STM8S208) || (STM8S207) || (STM8AF62Ax) || (STM8AF52Ax) */

#include "stm8s_awu.h"
#include "stm8s_beep.h"

#if defined (STM8S208) || defined (STM8AF52Ax)
 #include "stm8s_can.h"
#endif /* STM8S208 || STM8AF52Ax */

#include "stm8s_clk.h"
#include "stm8s_exti.h"
#include "stm8s_flash.h"
#include "stm8s_gpio.h"
#include "stm8s_i2c.h"
#include "stm8s_itc.h"
#include "stm8s_iwdg.h"
#include "stm8s_rst.h"
#include "stm8s_spi.h"
#include "stm8s_tim1.h"

#ifndef STM8S903
 #include "stm8s_tim2.h"
#endif /* STM8S903 */

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) ||defined(STM8S105) ||\
    defined(STM8S005) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
 #include "stm8s_tim3.h"
#endif /* (STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S105) */

#ifndef STM8S903
 #include "stm8s_tim4.h"
#endif /* STM8S903 */

#ifdef STM8S903
 #include "stm8s_tim5.h"
 #include "stm8s_tim6.h"
#endif /* STM8S903 */

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
 #include "stm8s_uart1.h"
#endif /* STM8S208 || STM8S207 || STM8S103 ||STM8S903 || STM8AF52Ax || STM8AF62Ax */

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
 #include "stm8s_uart2.h"
#endif /* STM8S105 || STM8AF626x */

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) || defined (STM8AF52Ax) ||\
    defined (STM8AF62Ax)
 #include "stm8s_uart3.h"
#endif /* STM8S208 || STM8S207 || STM8AF52Ax || STM8AF62Ax */

#include "stm8s_wwdg.h"

/*
	用户可以选择是否使能ST固件库的断言供能。使能断言的方法有两种：
	(1) 在C编译器的预定义宏选项中定义USE_FULL_ASSERT。
	(2) 在本文件定义"#define USE_FULL_ASSERT    1
*/
#define USE_FULL_ASSERT    (1)

#ifdef  USE_FULL_ASSERT
	/*
		assert_param宏用于函数形参检查。如果expr是false，它将调用assert_failed()函数报告发生错误的源文件
		和行号。如果expr是true，将不执行任何操作。

		assert_failed() 函数在stm32f10x_assert.c文件(这是安富莱建立的文件)
	*/
	#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

	void assert_failed(uint8_t* file, uint32_t line);
#else
	#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM8S_CONF_H */

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
