 /**********************************************************************************
 * �� �� ����timer.c
 * ��    ����ͨ��timer.c��ʹ�ã��ܿ��ٷ��������Ӳ���Ķ�ʱ������������ʱ���ʱ����Ҫ��
             ʹ���жϷ�ʽ���ṹ��Timer_Delay������ʱ��Timer_Court������ʱ��ʹ��ʱ���轫
						 Timer_tick()�����жϺ����ڡ�
						 
						 ��ʱʹ�ò��裺
						 �ٶ���һ��volatile Timer_Delay*ָ�����
						 �ڵ��ó�ʼ������Delay_Initial������Ϊ��һ�������ָ���Լ���Ҫ����ʱʱ��
						 ������Ҫ��ʱ�ĵط����ú���Open_Delay������Ϊ��һ�������ָ��
						 �ܵ���Delay_Ok�������м�⣬����ʱʱ�䵽���򷵻�1�����򷵻�0. 
						 
						 ��ʱʹ�ò��裺
						 �ٶ���һ��Timer_Court*ָ�����
						 �ڵ��ó�ʼ������Court_initial������Ϊ��һ�������ָ��
						 ������Ҫ��ʱ�ĵط����ú���Open_Court������Ϊ��һ�������ָ��
						 ������Ҫ��ʱ������������Court_Ok�������м���
 * ʱ    �䣺2016-8-1
 * �汾��  ��v1.2
 * ��    �ߣ������
**********************************************************************************/

#include "timer.h"

volatile TimerDelay delay_timers[ DELAY_NUM ];
volatile TimerCourt court_timers[ COURT_NUM ];


/*************************************************************
|1|����  ����ʼ��һ����ʱ           |2|���� ����
|3|����  ��uint16_t                 |4|���� : TimerDelay* 
*************************************************************/
volatile TimerDelay* Delay_Initial(uint16_t  timer_court)
{
	uint8_t  temp = 0;
	while(delay_timers[ temp ].Used)
		temp++;      //���delay_timers�е�temp��Ԫ��δ��ʹ��
	delay_timers[ temp ].Court = timer_court;
	delay_timers[ temp ].Used = 1;
	return &delay_timers[ temp ];
}


/*************************************************************
|1|����  ������Ҫ��ʱ��Timer_Delay     |2|���� ������Ҫ��ĳ����ʱʱ����
|3|����  ��Timer_Delay*                  |4|����  : �� 
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
|1|����  �������ʱ�Ƿ���ɣ������ɷ���1������������  
|2|����  ����
|3|����  ��Timer_Delay*     |4|����: 0--��ʱδ���  1--��ʱ���
*************************************************************/
uint8_t Delay_Ok(volatile TimerDelay*  timer_delay) //��ʱ��ɷ���1��δ��ɷ���0
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
|1|����  ����ͣĳ����ʱ      |2|���� ������Ҫ��ͣĳ����ʱʱ����
|3|����  ��Timer_Delay*      |4|����  : �� 
*************************************************************/
void Stop_Delay(volatile TimerDelay*  timer_delay)
{
	timer_delay->Flag3 = timer_delay->Flag1;
	timer_delay->Flag1 = 0;
}

/*************************************************************
|1|����  �����¿�ʼ��ͣ����ʱ   |2|����  ����
|3|����  ��Timer_Delay*         |4|����  : �� 
*************************************************************/
void Restart_Delay(volatile TimerDelay*  timer_delay)
{
	timer_delay->Flag1 = timer_delay->Flag3;
	timer_delay->Flag3 = 0;
}
//ֻ����ͣʱ ��ʱû���������´�
void Clear_Restart_Delay(volatile TimerDelay*  timer_delay)
{
	timer_delay->Flag1 = timer_delay->Flag3;
	timer_delay->court = timer_delay->Court;
	timer_delay->Flag3 = 0;	
}

/*************************************************************
|1|����  ���ر�ĳ����ʱ    |2|����  ����
|3|����  ��Timer_Delay*    |4|����  : �� 
*************************************************************/
void Close_Delay(volatile TimerDelay*  timer_delay)
{
	timer_delay->Flag1 = 0;   
	timer_delay->Flag2 = 0;  
	timer_delay->Flag3 = 0;      
	timer_delay->court = 0; 
}

/*************************************************************
|1|����  ���޸�ĳ����ʱ    |2|����  ����
|3|����  ��Timer_Delay*    |4|����  : �� 
*************************************************************/
void Change_Timer(volatile TimerDelay* timer_delay, uint16_t  new_time)
{
	timer_delay->Court = new_time;	
}

/*************************************************************
|1|����  ����ʼ��һ����ʱ    |2|���� ����
|3|����  ��Timer_Court*      |4|���� : �� 
*************************************************************/
volatile TimerCourt* Court_Initial(void)
{
	uint8_t temp=0;
	while(court_timers[temp].Used)
		temp++;      //���delay_timers�е�temp��Ԫ��δ��ʹ��
	court_timers[temp].Used=1;
	return &court_timers[temp];
}

/*************************************************************
|1|����  ������Ҫ��ʱ��Timer_Court     |2|���ã�
|3|����  ��Timer_Court*                  |4|����: �� 
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
|1|����  ����ʱ���ʱ���øú�����������ʱʱ��     |2|���ã�
|3|����  ��Timer_Court*                           |4|����: ��ʱʱ��
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
|1|����  ����ͣĳ����ʱ      |2|���� ������Ҫ��ͣĳ����ʱʱ����
|3|����  ��Timer_Court*      |4|����  : �� 
*************************************************************/
void Stop_Court(volatile TimerCourt* timer_delay)
{
	timer_delay->Flag1=timer_delay->Flag;
	timer_delay->Flag=0;
}

/*************************************************************
|1|����  �����¿�ʼ��ͣ����ʱ   |2|����  ����
|3|����  ��Timer_Court*         |4|����  : �� 
*************************************************************/
void Restart_Court(volatile TimerCourt* timer_delay)
{
	timer_delay->Flag=timer_delay->Flag1;
}

/*************************************************************
 * ��������Timer_tick()
 * ����  ��ÿִ��һ�Σ���������Ҫ��ʱ����ʱ��court++
 * ����  ��Timer_Court*
 * ����  : ��
 * ����  ���жϺ�������
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
