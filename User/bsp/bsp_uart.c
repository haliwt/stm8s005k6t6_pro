/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_uart.c
*	��    �� : V1.0
*	˵    �� : ����UART����ģʽ
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

#define RS485_EN

#define RS485_TXEN_0()  GPIO_WriteLow(GPIOD, GPIO_PIN_4)
#define RS485_TXEN_1()  GPIO_WriteHigh(GPIOD, GPIO_PIN_4)

UART_COMM_T g_tComm;

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitUart
*	����˵��: ��ʼ��CPU��USART1����Ӳ���豸��δ�����жϡ�
*	��    ��: _baud ������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitUart(uint32_t _baud)
{
	g_tComm.Baud = _baud;
	
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)	  
	UART2_DeInit();

	/* ���� UART1
	    - BaudRate = 115200 baud
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Receive and transmit enabled
	    - UART1 Clock disabled
	*/
	UART2_Init((uint32_t)_baud, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO,
	          UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
#endif


#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)		
	
	#if 0	
		UART1_DeInit();
	
		/* ���� UART1
			- BaudRate = 115200 baud
			- Word Length = 8 Bits
			- One Stop Bit
			- No parity
			- Receive and transmit enabled
			- UART1 Clock disabled
		*/
		UART1_Init((uint32_t)_baud, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
				  UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);	
	#else
		/* 208 */
		UART3_DeInit();
	
		/* ���� UART3
			- BaudRate = 9600 baud
			- Word Length = 8 Bits
			- One Stop Bit
			- No parity
			- Receive and transmit enabled
			- UART1 Clock disabled
		*/
		UART3_Init((uint32_t)_baud, UART3_WORDLENGTH_8D, UART3_STOPBITS_1, UART3_PARITY_NO,
				  UART3_MODE_TXRX_ENABLE);	

        UART3_ITConfig(UART3_IT_RXNE_OR, ENABLE);
	#endif
#endif	

#ifdef RS485_EN	
	/* ����PD4 = 0 */
	GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);
#endif		
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SendUart
*	����˵��: �򴮿ڷ���һ������
*	��    �Σ�_pBuf ���ݻ�����
*			 _ucLen ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SendUart(uint8_t *_pBuf, uint8_t _ucLen)
{
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)	   
	while (_ucLen--)
	{
  		UART2_SendData8(*_pBuf++);
  		while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);	/* �ȴ����ͼĴ���Ϊ�� */
  	}
  	while (UART2_GetFlagStatus(UART2_FLAG_TC) == RESET);	/* ����ȴ����1���ֽڷ������ */
#endif

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)	

	#if 0
		while (_ucLen--)
		{
			UART1_SendData8(*_pBuf++);
			while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);	/* �ȴ����ͼĴ���Ϊ�� */
		}
		while (UART1_GetFlagStatus(UART1_FLAG_TC) == RESET);	/* ����ȴ����1���ֽڷ������ */
	#else	/*��208 ���������� */
		while (_ucLen--)
		{
			UART3_SendData8(*_pBuf++);
			while (UART3_GetFlagStatus(UART3_FLAG_TXE) == RESET);	/* �ȴ����ͼĴ���Ϊ�� */
		}
		while (UART3_GetFlagStatus(UART3_FLAG_TC) == RESET);	/* ����ȴ����1���ֽڷ������ */	
	#endif
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_UartRxNew
*	����˵��: �ж�UART�Ƿ��յ�������
*	��    ��: ��
*	�� �� ֵ: 1��ʾ�У� 0��ʾ��
*********************************************************************************************************
*/
static uint8_t bsp_UartRxNew(void)
{
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)	   
	if (UART2_GetFlagStatus(UART2_FLAG_RXNE) != RESET)
	{
		return 1;
	}
	else
	{
	  	return 0;
	}
#endif

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)	
	
	#if 0
		if (UART1_GetFlagStatus(UART1_FLAG_RXNE) != RESET)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	#else
		if (UART3_GetFlagStatus(UART3_FLAG_RXNE) != RESET)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	#endif
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ReadUartData
*	����˵��: �����ڽ������ݼĴ����� ��Ҫ���ж��Ƿ�������
*	��    ��: ��
*	�� �� ֵ: �ֽ�����
*********************************************************************************************************
*/
static uint8_t bsp_ReadUartData(void)
{
  	uint8_t rxdata;
	
#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)	   
	rxdata = UART2_ReceiveData8();
#endif

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)	
	
	#if 0
		rxdata = UART1_ReceiveData8();
	#else
		rxdata = UART3_ReceiveData8();
	#endif
#endif
	return rxdata;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ReadUartData
*	����˵��: �����ڽ������ݼĴ����� ��Ҫ���ж��Ƿ�������
*	��    ��: _data ���ݴ�ŵ�ַ
*	�� �� ֵ: 0��ʾ�����ݣ�1��ʾ�����ݡ� ���ݴ����_data
*********************************************************************************************************
*/
uint8_t bsp_ReadUart(uint8_t *_data)
{
	if (bsp_UartRxNew())
	{
		*_data = bsp_ReadUartData();
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SendRS485
*	����˵��: ��485���߷���һ������
*	��    �Σ�_pBuf ���ݻ�����
*			 _ucLen ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SendRS485(uint8_t *_pBuf, uint8_t _ucLen)
{
	RS485_TXEN_1();		/* ʹ��RS485оƬ�ķ������ţ���ռ���� */
	bsp_SendUart(_pBuf, _ucLen);
  	RS485_TXEN_0();		/* ����RS485оƬ�ķ������ţ��ͷ����� */
}

/*
*********************************************************************************************************
*	�� �� ��: uart_rx_isr
*	����˵��: ���ڽ����жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void uart_rx_isr(void)
{
	uint8_t rxdata;

	rxdata = bsp_ReadUartData();

	/*
		3.5���ַ���ʱ������ֻ������RTUģʽ���棬��ΪRTUģʽû�п�ʼ���ͽ�������
		�������ݰ�֮��ֻ�ܿ�ʱ���������֣�Modbus�����ڲ�ͬ�Ĳ������£����ʱ���ǲ�һ���ģ�
		���Ծ���3.5���ַ���ʱ�䣬�����ʸߣ����ʱ������С�������ʵͣ����ʱ������Ӧ�ʹ�
	*/
	bsp_StartTimer(1, 4);		/* ����4ms ��ʱ (9600) */

	if (g_tComm.RxCount < RX_MAX_RTU)
	{			
		g_tComm.RxBuf[g_tComm.RxCount++] = rxdata;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: uart_rx_isr
*	����˵��: ���ڽ����жϷ������ ��Ҫ����������ִ�С�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t rx_pro(void)
{
	/* �յ����� 
		05 06 00 88 04 57 3B70 (8 �ֽ�) 
			05    :  ��������ĺ�վ��
			06    :  ָ��
			00 88 :  �����������ʾ�Ĵ���
			04 57 :  ����,,,ת���� 10 ������ 1111.��λ��ǰ, 
			3B70  :  �����ֽ� CRC ��	��05�� 57��У��	
	*/
	if (bsp_CheckTimer(1))
	{
        g_tComm.RxNewFlag = 1;
        
        return 1;
    }
    return 0;
}    

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
