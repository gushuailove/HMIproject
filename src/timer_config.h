/**************************************************************************
 * 文件名  ：timer_config.h
 * 描述    ：         
 * 作者    ：李彦辉 
 * 日期    ：2016-8-1
**********************************************************************************/	

#ifndef __TIMER_CONFIG_H
#define	__TIMER_CONFIG_H
#include "stm32f10x.h"

void Tim3_Config(void);
void TIM3_NVIC_Configuration(void);
void TIM3_Configuration(void);

void Timer_Delay(uint16_t num);

#endif

