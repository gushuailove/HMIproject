/**************************************************************************
 * 文件名  ：pwm.c
 * 描述    ：         
 * 作者    ： 
 * 日期    ：
**********************************************************************************/	
#include "pwm.h"
#include "stm32f10x_gpio.h"
void TIM2_NVIC_Configuration(void);

/*******************************************************************************
* 函数名称 : Tim2_Config
* 描述     : 时钟配置
* 输入     : 无
* 输出     : 无
* 返回值   : 无
*******************************************************************************/
void Tim2_Config(void)
{
 	TIM2_NVIC_Configuration();
	TIM2_Configuration();
} 

/*******************************************************************************
* 函数名称 : TIM2_NVIC_Configuration
* 描述     : NVIC配置
* 输入     : 无
* 输出     : 无
* 返回值   : 无
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
* 函数名称 : TIM2_Configuration
* 描述     : TIM2配置
* 输入     : 无
* 输出     : 无
* 返回值   : 无
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
* 函数名称 : TIM2_IRQHandler
* 描述     : TIM2中断
* 输入     : 无
* 输出     : 无
* 返回值   : 无
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

