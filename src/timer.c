 /**********************************************************************************
 * 文 件 名：timer.c
 * 描    述：通过timer.c的使用，能快速方便的利用硬件的定时计数器进行延时与计时，但要求
             使用中断方式。结构体Timer_Delay用来延时，Timer_Court用来计时，使用时，需将
						 Timer_tick()放入中断函数内。
						 
						 延时使用步骤：
						 ①定义一个volatile Timer_Delay*指针变量
						 ②调用初始化函数Delay_Initial，参数为第一步定义的指针以及需要的延时时间
						 ③在需要延时的地方调用函数Open_Delay，参数为第一步定义的指针
						 ④调用Delay_Ok函数进行检测，若延时时间到，则返回1，否则返回0. 
						 
						 计时使用步骤：
						 ①定义一个Timer_Court*指针变量
						 ②调用初始化函数Court_initial，参数为第一步定义的指针
						 ③在需要计时的地方调用函数Open_Court，参数为第一步定义的指针
						 ④在需要计时结束处，调用Court_Ok函数进行计算
 * 时    间：2016-8-1
 * 版本号  ：v1.2
 * 作    者：李彦辉
**********************************************************************************/

#include "timer.h"

volatile TimerDelay delay_timers[ DELAY_NUM ];
volatile TimerCourt court_timers[ COURT_NUM ];


/*************************************************************
|1|描述  ：初始化一个延时           |2|调用 ：无
|3|输入  ：uint16_t                 |4|返回 : TimerDelay* 
*************************************************************/
volatile TimerDelay* Delay_Initial(uint16_t  timer_court)
{
	uint8_t  temp = 0;
	while(delay_timers[ temp ].Used)
		temp++;      //如果delay_timers中第temp个元素未被使用
	delay_timers[ temp ].Court = timer_court;
	delay_timers[ temp ].Used = 1;
	return &delay_timers[ temp ];
}


/*************************************************************
|1|描述  ：打开需要延时的Timer_Delay     |2|调用 ：当需要打开某个延时时调用
|3|输入  ：Timer_Delay*                  |4|返回  : 无 
*************************************************************/
void Open_Delay(volatile TimerDelay*  timer_delay)
{
// 	if(timer_delay->Flag1 == 0)
// 	{			
		timer_delay->court = timer_delay->Court;			
		timer_delay->Flag1 = 1;
		timer_delay->Flag2 = 0;
		timer_delay->Flag3 = 0;
// 	}
	return;
}

/*************************************************************
|1|描述  ：检测延时是否完成，如果完成返回1，将变量清零  
|2|调用  ：无
|3|输入  ：Timer_Delay*     |4|返回: 0--延时未完成  1--延时完成
*************************************************************/
uint8_t Delay_Ok(volatile TimerDelay*  timer_delay) //延时完成返回1，未完成返回0
{
	if(timer_delay->Flag2 == 1)
		{
			timer_delay->Flag2 = 0;
			timer_delay->Flag1 = 0;
			return 1;
		}		
	return 0;
}

/*************************************************************
|1|描述  ：暂停某个延时      |2|调用 ：当需要暂停某个延时时调用
|3|输入  ：Timer_Delay*      |4|返回  : 无 
*************************************************************/
void Stop_Delay(volatile TimerDelay*  timer_delay)
{
	timer_delay->Flag3 = timer_delay->Flag1;
	timer_delay->Flag1 = 0;
}

/*************************************************************
|1|描述  ：重新开始暂停的延时   |2|调用  ：无
|3|输入  ：Timer_Delay*         |4|返回  : 无 
*************************************************************/
void Restart_Delay(volatile TimerDelay*  timer_delay)
{
	timer_delay->Flag1 = timer_delay->Flag3;
	timer_delay->Flag3 = 0;
}
//只把暂停时 延时没结束的重新打开
void Clear_Restart_Delay(volatile TimerDelay*  timer_delay)
{
	timer_delay->Flag1 = timer_delay->Flag3;
	timer_delay->court = timer_delay->Court;
	timer_delay->Flag3 = 0;	
}

/*************************************************************
|1|描述  ：关闭某个延时    |2|调用  ：无
|3|输入  ：Timer_Delay*    |4|返回  : 无 
*************************************************************/
void Close_Delay(volatile TimerDelay*  timer_delay)
{
	timer_delay->Flag1 = 0;   
	timer_delay->Flag2 = 0;  
	timer_delay->Flag3 = 0;      
	timer_delay->court = 0; 
}

/*************************************************************
|1|描述  ：修改某个延时    |2|调用  ：无
|3|输入  ：Timer_Delay*    |4|返回  : 无 
*************************************************************/
void Change_Timer(volatile TimerDelay* timer_delay, uint16_t  new_time)
{
	timer_delay->Court = new_time;	
}

/*************************************************************
|1|描述  ：初始化一个计时    |2|调用 ：无
|3|输入  ：Timer_Court*      |4|返回 : 无 
*************************************************************/
volatile TimerCourt* Court_Initial(void)
{
	uint8_t temp=0;
	while(court_timers[temp].Used)
		temp++;      //如果delay_timers中第temp个元素未被使用
	court_timers[temp].Used=1;
	return &court_timers[temp];
}

/*************************************************************
|1|描述  ：打开需要延时的Timer_Court     |2|调用：
|3|输入  ：Timer_Court*                  |4|返回: 无 
*************************************************************/
void Open_Court(volatile TimerCourt* timer_court)
{
	if(timer_court->Flag==0)
		{
			timer_court->court=0;
			timer_court->Flag=1; 
		}
}

/*************************************************************
|1|描述  ：计时完成时调用该函数，返回延时时间     |2|调用：
|3|输入  ：Timer_Court*                           |4|返回: 计时时间
*************************************************************/
uint16_t Court_Ok(volatile TimerCourt* timer_court) 
{
	uint16_t temp; 
	temp=timer_court->court;
	if(timer_court->Flag==1)
		{
			timer_court->court=0;
			timer_court->Flag=0;
		}
		return temp;
}

/*************************************************************
|1|描述  ：暂停某个计时      |2|调用 ：当需要暂停某个计时时调用
|3|输入  ：Timer_Court*      |4|返回  : 无 
*************************************************************/
void Stop_Court(volatile TimerCourt* timer_delay)
{
	timer_delay->Flag1=timer_delay->Flag;
	timer_delay->Flag=0;
}

/*************************************************************
|1|描述  ：重新开始暂停的延时   |2|调用  ：无
|3|输入  ：Timer_Court*         |4|返回  : 无 
*************************************************************/
void Restart_Court(volatile TimerCourt* timer_delay)
{
	timer_delay->Flag=timer_delay->Flag1;
}

/*************************************************************
 * 函数名：Timer_tick()
 * 描述  ：每执行一次，将所有需要计时或延时的court++
 * 输入  ：Timer_Court*
 * 返回  : 无
 * 调用  ：中断函数调用
*************************************************************/
void Timer_tick(void)
{
	uint8_t temp=DELAY_NUM;
	while(temp)
	{
	temp--;
		if((delay_timers[temp].Flag1==1)&&(delay_timers[temp].Flag2==0))
		{
			delay_timers[temp].court--;
			if(delay_timers[temp].court==0)
			delay_timers[temp].Flag2=1;
		}
	}
	temp=COURT_NUM;
	while(temp)
	{
	temp--;
		if(court_timers[temp].Flag==1)
		{
			court_timers[temp].court++;
		}
	}
}
