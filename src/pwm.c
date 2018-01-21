/**************************************************************************
 * �ļ���  ��pwm.c
 * ����    ��         
 * ����    �� 
 * ����    ��
**********************************************************************************/	
#include "pwm.h"
#include "stm32f10x_gpio.h"
void TIM2_NVIC_Configuration(void);

/*******************************************************************************
* �������� : Tim2_Config
* ����     : ʱ������
* ����     : ��
* ���     : ��
* ����ֵ   : ��
*******************************************************************************/
void Tim2_Config(void)
{
 	TIM2_NVIC_Configuration();
	TIM2_Configuration();
} 

/*******************************************************************************
* �������� : TIM2_NVIC_Configuration
* ����     : NVIC����
* ����     : ��
* ���     : ��
* ����ֵ   : ��
*******************************************************************************/
void TIM2_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* �������� : TIM2_Configuration
* ����     : TIM2����
* ����     : ��
* ���     : ��
* ����ֵ   : ��
*******************************************************************************/
void TIM2_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
  TIM_DeInit(TIM2);
  TIM_TimeBaseStructure.TIM_Period=500;		 								

  TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		
  TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM2, ENABLE);																		
    
}	
	static uint8_t count = 0;
/*******************************************************************************
* �������� : TIM2_IRQHandler
* ����     : TIM2�ж�
* ����     : ��
* ���     : ��
* ����ֵ   : ��
*******************************************************************************/
void TIM2_IRQHandler(void)
{

	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{		
		if(count == 1){
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			count = 0;
		}
		else{
			GPIO_SetBits(GPIOA, GPIO_Pin_2);
			count = 1;
		}
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);    
	}		 	
}

