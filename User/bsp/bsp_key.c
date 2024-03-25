/*
*********************************************************************************************************
*
*	ģ������ : ��������ģ��
*	�ļ����� : bsp_button.c
*	��    �� : V1.2
*	˵    �� : ʵ�ְ����ļ�⣬��������˲����ƣ����Լ�������¼���
*				(1) ��������
*				(2) ��������
*				(3) ������
*				(4) ����ʱ�Զ�����
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2012-10-12 armfly  ST�̼���汾 V2.1.0
*		V1.1    2013-09-18 armfly  ��� bsp_SetKeyParam() ���� bsp_ClearKey() ����
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"

static KEY_T s_tBtn[BUTTON_COUNT];

static KEY_FIFO_T s_Key;		/* ����FIFO����,�ṹ�� */

static void bsp_InitButtonVar(void);
static void bsp_InitButtonHard(void);
static void bsp_DetectButton(uint8_t _ucIndex);

/*
*********************************************************************************************************
*	�� �� ��: IsKeyDownX
*	����˵��: �жϰ����Ƿ���
*	��    ��: ��
*	�� �� ֵ: ����ֵ1 ��ʾ���£�0��ʾδ����
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
*	�� �� ��: bsp_InitButton
*	����˵��: ��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitButton(void)
{
	bsp_InitButtonVar();		/* ��ʼ���������� */
	bsp_InitButtonHard();		/* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��: _KeyCode : ��������
*	�� �� ֵ: ��
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
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��: ��
*	�� �� ֵ: ��������
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
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    ��: ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_Key.Read = s_Key.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_IsKeyPressed
*	����˵��: ��ⰴ���Ƿ���
*	��    ��: _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_IsKeyPressed(uint8_t _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    ��: _ucKeyID : ����ID����0��ʼ
*			_LongTime : �����¼�ʱ��
*			 _RepeatSpeed : �����ٶȡ��������10ms�ط�һ����Ϣ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* �����������ٶȣ�0��ʾ��֧������ */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* ���������� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitButtonHard
*	����˵��: ��ʼ������Ӳ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitButtonHard(void)
{
	/* ���ð���������Ϊ�������������ж� */
	GPIO_Init(GPIO_KEY_SET, (GPIO_Pin_TypeDef)PIN_SET, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(GPIO_KEY_OK, (GPIO_Pin_TypeDef)PIN_OK, GPIO_MODE_IN_PU_NO_IT);
	//GPIO_Init(GPIO_KEY3, (GPIO_Pin_TypeDef)PIN_KEY3, GPIO_MODE_IN_PU_NO_IT);	
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitButtonVar
*	����˵��: ��ʼ����������
*	��    ��: strName : ���������ַ���
*			  strDate : ���̷�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitButtonVar(void)
{
	uint8_t i;

	/* �԰���FIFO��дָ������ */
	s_Key.Read = 0;
	s_Key.Write = 0;

	/* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
	for (i = 0; i < BUTTON_COUNT; i++)
	{
		s_tBtn[i].LongTime = 0;						/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count = BUTTON_FILTER_TIME / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
		s_tBtn[i].State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
		s_tBtn[i].RepeatSpeed = 0;						/* �����������ٶȣ�0��ʾ��֧������ */
		s_tBtn[i].RepeatCount = 0;						/* ���������� */
	}

	/* �жϰ������µĺ��� */
	s_tBtn[0].IsKeyDownFunc = IsKeyDownSet;
	s_tBtn[1].IsKeyDownFunc = IsKeyDownOK;
	//s_tBtn[2].IsKeyDownFunc = IsKeyDown3;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectButton
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��: �����ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectButton(uint8_t i)
{
	KEY_T *pBtn;

	/* ���û�г�ʼ�������������򱨴�
	if (s_tBtn[i].IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
	}
	*/

	pBtn = &s_tBtn[i];
	if (pBtn->IsKeyDownFunc()) //����ָ����ã��ṹ���Ա��������
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
				pBtn->State = 1; //��������

				/* ���Ͱ�ť���µ���Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 1)); //1,4,7-> �̰����� 3,6,9-> �������� 2,5,8-> ���𰴼�
			}

			if (pBtn->LongTime > 0) //������ʼ�����壬����������ʱ��bsp_SetKeyParam�� ��
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* ��ֵ���밴��FIFO */
						bsp_PutKey((uint8_t)(3 * i + 3)); //����������3,6,9-> �̰����� 1,4,7-> �������� 2,5,8-> ���𰴼�
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* ��������ÿ��10ms����1������ */
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

				/* ���Ͱ�ť�������Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyPro
*	����˵��: ������а�����������״̬�����뱻�����Եĵ��á�
*	��    ��: ��
*	�� �� ֵ: ��
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



