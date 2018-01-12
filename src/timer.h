/**************************************************************************
 * 文件名  ：timer.h
 * 描述    ：         
 * 作者    ：李彦辉 
 * 日期    ：2016-8-1
**********************************************************************************/	

#ifndef __TIMER_H
#define	__TIMER_H
#include "stm32f10x.h"

#define  DELAY_NUM 37
#define  COURT_NUM 5


typedef struct
{
  uint8_t Flag1:1;   //延时开始标志
	uint8_t Flag2:1;   //延时完成标志
	uint8_t Flag3:1;   //暂停时用来保存Flag1的信息
	uint8_t Used:1;    //是否被使用   1--被使用
	uint16_t court;    //每次中断将court减1
	uint16_t Court;	   //延时时间
} TimerDelay;

typedef struct
{
  uint8_t Flag:1;    //计时开始标志 1--开始
	uint8_t Flag1:1;
	uint8_t Used:1;    //是否被使用   1--被使用
	uint16_t court;    //计时值
} TimerCourt;


extern volatile TimerDelay delay_timers[DELAY_NUM];
extern volatile TimerCourt court_timers[COURT_NUM];


volatile TimerDelay* Delay_Initial(uint16_t timer_court);
void Open_Delay(volatile TimerDelay* timer_delay); 
uint8_t Delay_Ok(volatile TimerDelay* timer_delay); //延时完成返回1，未完成返回0
void Stop_Delay(volatile TimerDelay* timer_delay);//中途暂停延时
void Restart_Delay(volatile TimerDelay* timer_delay);//重新开始暂停的延时
void Clear_Restart_Delay(volatile TimerDelay*  timer_delay);//重新开始暂停的延时，并清除之前的计数
void Close_Delay(volatile TimerDelay* timer_delay);//关闭延时
void Change_Timer(volatile TimerDelay* timer_delay, uint16_t  new_time);
	
volatile TimerCourt* Court_Initial(void);
void Open_Court(volatile TimerCourt* timer_court); 
uint16_t Court_Ok(volatile TimerCourt* timer_court); //计时结束时调用，返回计时时间
void Stop_Court(volatile TimerCourt* timer_court);//中途暂停计时
void Restart_Court(volatile TimerCourt* timer_court);//重新开始暂停的计时
void Timer_tick(void); //中断调用


#endif
