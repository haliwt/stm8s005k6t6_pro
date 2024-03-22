/*
*********************************************************************************************************
*
*	ģ������ : printfģ��
*	�ļ����� : bsp_printf.c
*	��    �� : V2.0
*	˵    �� : ʵ��printf��scanf�����ض��򵽴���1����֧��printf��Ϣ��UART1
*				ʵ���ض���ֻ��Ҫ���2������:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*
*				���cģ���޶�Ӧ��h�ļ���
*				�����ҪӦ�ó���֧�� printf ������ֻ�ý� bsp_printf.c ����ļ���ӵ����̼��ɡ�
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2012-10-12 armfly  ST�̼���汾 V2.1.0
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "stm8s.h"
#include <stdio.h>

/*
	���ڲ�ͬ�ı����� putcha �� getchar �βκͷ���ֵ���в�ͬ��
	��˴˴����ú궨��ķ�ʽ����

	_RAISONANCE_ �� _COSMIC_ ��2�������ɱ������Զ���ӵ�Ԥ�����
*/
#ifdef _RAISONANCE_
	#define PUTCHAR_PROTOTYPE int putchar (char c)
	#define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
	#define PUTCHAR_PROTOTYPE char putchar (char c)
	#define GETCHAR_PROTOTYPE char getchar (void)
#else /* _IAR_ */
	#define PUTCHAR_PROTOTYPE int putchar (int c)
	#define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */

/*
*********************************************************************************************************
*	�� �� ��: putchar
*	����˵��: �ض��� putchar ������ ��������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: �ȴ����͵��ַ�
*	�� �� ֵ: �����ѷ��͵��ַ�
*********************************************************************************************************
*/
PUTCHAR_PROTOTYPE
{
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)	  
	/* ����һ���ַ� c ��UART1 */
	UART2_SendData8(c);

	/* �ȴ�������� */
	while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
#endif

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)

	#if 0	
		/* ����һ���ַ� c ��UART1 */
		UART1_SendData8(c);
	
		/* �ȴ�������� */
		while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);
	#else
		/* ����һ���ַ� c ��UART1 */
		UART3_SendData8(c);
	
		/* �ȴ�������� */
		while (UART3_GetFlagStatus(UART3_FLAG_TXE) == RESET);	
	#endif
	
#endif
	
	return (c);
}

/*
*********************************************************************************************************
*	�� �� ��: getchar
*	����˵��: �ض���C���е� getchar ����,��������ʹ��scanff�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: �����Ѷ������ַ�
*********************************************************************************************************
*/
GETCHAR_PROTOTYPE
{
	#ifdef _COSMIC_
		char c = 0;
	#else
		int c = 0;
	#endif


#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)	
	/* �ȴ������ݵ���  */
	while (UART2_GetFlagStatus(UART2_FLAG_RXNE) == RESET);

	/* ��ȡ���ݼĴ��� */
	c = UART2_ReceiveData8();
#endif

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)

	#if 0
		/* �ȴ������ݵ���  */
		while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET);
	
		/* ��ȡ���ݼĴ��� */
		c = UART1_ReceiveData8();
	#else
		/* �ȴ������ݵ���  */
		while (UART3_GetFlagStatus(UART3_FLAG_RXNE) == RESET);
	
		/* ��ȡ���ݼĴ��� */
		c = UART3_ReceiveData8();	
	#endif	
#endif
	
	return (c);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
