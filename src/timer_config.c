/**************************************************************************
 * 文件名  ：timer_config.c
 * 描述    ：         
 * 作者    ：李彦辉 
 * 日期    ：2016-8-1
**********************************************************************************/	
#include "timer_config.h"
#include "timer.h"

static volatile TimerDelay* timer_delay;

/*******************************************************************************
* 函数名称 : Tim3_Config
* 描述     : 时钟配置
* 输入     : 无
* 输出     : 无
* 返回值   : 无
*******************************************************************************/
void Tim3_Config(void)
{
 	TIM3_NVIC_Configuration();
	TIM3_Configuration();
			
	timer_delay = Delay_Initial(100);
} 

/*******************************************************************************
* 函数名称 : TIM3_NVIC_Configuration
* 描述     : NVIC配置
* 输入     : 无
* 输出     : 无
* 返回值   : 无
*******************************************************************************/
void TIM3_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  													
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* 函数名称 : TIM3_Configuration
* 描述     : TIM3配置
* 输入     : 无
* 输出     : 无
* 返回值   : 无
*******************************************************************************/
void TIM3_Configuration(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
  TIM_DeInit(TIM3);
  TIM_TimeBaseStructure.TIM_Period=10000;		 								

  TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    		
  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
  TIM_Cmd(TIM3, ENABLE);																		
    
}	

/*******************************************************************************
* 函数名称 : TIM3_IRQHandler
* 描述     : TIM3中断
* 输入     : 无
* 输出     : 无
* 返回值   : 无
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
	{		
 		Timer_tick();
		TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);    
	}		 	
}

/*************************************************************
|1|描述  ：延时函数  |2|调用  ：无
|3|输入  ：无        |4|返回  : 无 
*************************************************************/
void Timer_Delay(uint16_t delay)
{
	if(delay < 1)
		return;
	
	Change_Timer(timer_delay, delay);
	Open_Delay(timer_delay);
	while(!Delay_Ok(timer_delay))
		;
		return;
}
