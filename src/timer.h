/**************************************************************************
 * �ļ���  ��timer.h
 * ����    ��         
 * ����    ������� 
 * ����    ��2016-8-1
**********************************************************************************/	

#ifndef __TIMER_H
#define	__TIMER_H
#include "stm32f10x.h"

#define  DELAY_NUM 37
#define  COURT_NUM 5


typedef struct
{
  uint8_t Flag1:1;   //��ʱ��ʼ��־
	uint8_t Flag2:1;   //��ʱ��ɱ�־
	uint8_t Flag3:1;   //��ͣʱ��������Flag1����Ϣ
	uint8_t Used:1;    //�Ƿ�ʹ��   1--��ʹ��
	uint16_t court;    //ÿ���жϽ�court��1
	uint16_t Court;	   //��ʱʱ��
} TimerDelay;

typedef struct
{
  uint8_t Flag:1;    //��ʱ��ʼ��־ 1--��ʼ
	uint8_t Flag1:1;
	uint8_t Used:1;    //�Ƿ�ʹ��   1--��ʹ��
	uint16_t court;    //��ʱֵ
} TimerCourt;


extern volatile TimerDelay delay_timers[DELAY_NUM];
extern volatile TimerCourt court_timers[COURT_NUM];


volatile TimerDelay* Delay_Initial(uint16_t timer_court);
void Open_Delay(volatile TimerDelay* timer_delay); 
uint8_t Delay_Ok(volatile TimerDelay* timer_delay); //��ʱ��ɷ���1��δ��ɷ���0
void Stop_Delay(volatile TimerDelay* timer_delay);//��;��ͣ��ʱ
void Restart_Delay(volatile TimerDelay* timer_delay);//���¿�ʼ��ͣ����ʱ
void Clear_Restart_Delay(volatile TimerDelay*  timer_delay);//���¿�ʼ��ͣ����ʱ�������֮ǰ�ļ���
void Close_Delay(volatile TimerDelay* timer_delay);//�ر���ʱ
void Change_Timer(volatile TimerDelay* timer_delay, uint16_t  new_time);
	
volatile TimerCourt* Court_Initial(void);
void Open_Court(volatile TimerCourt* timer_court); 
uint16_t Court_Ok(volatile TimerCourt* timer_court); //��ʱ����ʱ���ã����ؼ�ʱʱ��
void Stop_Court(volatile TimerCourt* timer_court);//��;��ͣ��ʱ
void Restart_Court(volatile TimerCourt* timer_court);//���¿�ʼ��ͣ�ļ�ʱ
void Timer_tick(void); //�жϵ���


#endif
