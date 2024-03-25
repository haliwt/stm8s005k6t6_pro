#include "bsp.h"


/**
  * @brief  Configure ADC2 Continuous Conversion with End Of Conversion interrupt 
  *         enabled .
  * @param  None
  * @retval None
  */
#if 0
static void ADC_Config()
{
  /*  Init GPIO for ADC2 */
  GPIO_Init(GPIOE, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT);
  
  /* De-Init ADC peripheral*/
  ADC2_DeInit();

  /* Init ADC2 peripheral */
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS, ADC2_CHANNEL_9, ADC2_PRESSEL_FCPU_D2, \
            ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_CHANNEL9,\
            DISABLE);

  /* Enable EOC interrupt */
  ADC2_ITConfig(ENABLE);

  /* Enable general interrupts */  
  enableInterrupts();
  
  /*Start Conversion */
  ADC2_StartConversion();
}

#endif 
void ADC_NTC_Init(void)
{
  // 启用内部时钟和系统时钟
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    //CLK_LSIPrescalerConfig(CLK_PRESCALER_LSIRDY);

    // 使能 GPIO 和 ADC1 时钟
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_GPIO, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC1, ENABLE);

    // 配置 PB3 为模拟输入引脚
    GPIO_Init(GPIOB, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);

    // 配置 ADC1 为 单通道模式
    ADC1_Init(ADC1_CONVERTMODE_SINGLE, ADC1_CHANNEL_3,
           ADC1_PRESSEL_FCPU_DIV128, ADC1_RESOLUTION_12BIT,
           ADC1_SCHannel0_NSSampleTime_CYC3,
           ADC1_SCHannel1_NSSampleTime_CYC3);

    // 使能 ADC1 外设
    ADC1_Cmd(ENABLE);

    // 配置 UART1
  //  UART1_DeInit();
  //  UART1_Init((uint32_t) 9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1,
  //        UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE,
  //         UART1_MODE_TXRX_ENABLE);

    // 配置 ADC1 转换完成中断
    //ADC1_ITConfig(ADC1_IT_EOC, ENABLE);

	// 配置 ADC1 转换完成中断
    //ADC1_ITConfig(ADC1_IT_EOC, ENABLE);

    // 开启全局中断
   // enableInterrupts();



}

/*********************************************************************
	*
	*Function Name:uint16_t Read_NTC_Voltage_Value(void)
	*Fucniton: read ntc resistance value for trsanfor temperature value
	*Input Ref:NO
	*Return Ref:NO
	*
*********************************************************************/
uint16_t Read_NTC_Voltage_Value(void)
{
  uint16_t adc1_value;
  /*Start Conversion */
  ADC1_StartConversion();

  adc1_value =  ADC1_GetConversionValue();

  return adc1_value;

}


