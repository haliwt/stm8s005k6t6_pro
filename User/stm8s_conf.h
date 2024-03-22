/*
*********************************************************************************************************
*
*	ģ������ : STM8�̼��������ļ���
*	�ļ����� : stm8s_conf.h
*	��    �� : V2.1.0
*	��    �� : 2011-11-18
*	˵    �� :	����ST�̼����ṩ���ļ����û����Ը�����Ҫ����ST�̼��������ģ�顣Ϊ�˷������ǰ��������й̼�
*				��ģ�顣
*
*			 	����ļ��� \Libraries\STM8S_StdPeriph_Driver\inc\stm8s.h ���������������.c�ļ���ֻ��Ҫ
*				include "stm32s.h"���ɣ����ص�����include ����ļ�
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __STM8S_CONF_H
#define __STM8S_CONF_H

#include "stm8s.h"

/*
	�ڱ�����ѡ������Ҫָ��CPUϵ�С���ͬ��ϵ����Ҫ������ͬ������ģ�顣

	������ʾ�����Ǹ������鶨�����֪����ͬϵ�е�CPU֮����������
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
	�û�����ѡ���Ƿ�ʹ��ST�̼���Ķ��Թ��ܡ�ʹ�ܶ��Եķ��������֣�
	(1) ��C��������Ԥ�����ѡ���ж���USE_FULL_ASSERT��
	(2) �ڱ��ļ�����"#define USE_FULL_ASSERT    1
*/
#define USE_FULL_ASSERT    (1)

#ifdef  USE_FULL_ASSERT
	/*
		assert_param�����ں����βμ�顣���expr��false����������assert_failed()�������淢�������Դ�ļ�
		���кš����expr��true������ִ���κβ�����

		assert_failed() ������stm32f10x_assert.c�ļ�(���ǰ������������ļ�)
	*/
	#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

	void assert_failed(uint8_t* file, uint32_t line);
#else
	#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM8S_CONF_H */

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
