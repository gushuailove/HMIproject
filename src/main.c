/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ����3.5.0�汾���Ĺ���ģ�塣         
 * ʵ��ƽ̨��Ұ��STM32������
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "timer_config.h"
#include "MCGSTouch.h"

uint8_t ii = 0;
uint8_t key = 0;
uint8_t way = 0;
uint8_t time = 0;
uint8_t jj = 0;
uint8_t kk = 0;

void delay(uint32_t ms)
{
  while(ms--);
}


/* 
 * ��������main
 * ����  : ��ʾ����
 * ����  ����
 * ���  : ��
 */
int main(void)
{

	Tim3_Config();
	
	//MCGSTouch_Initial��ʼ��
	MCGSTouch_Initial();
	
	while(1)
	{
		switch(ii)
		{
		/****************ͨ��һ����ʾ����һ*********************/
			case 0:
				MCGSTouch_Send(WAY_1,DISPLAY_INTERFACE_1,0,0);
				ii++;
				break;
			case 1:
				if(MCGSTouch_Receive(DISPLAY_INTERFACE_1,&key,&way,&time) == 1)
				{
					ii++;
				}
				break;
		/*******************************************************/	
		/**********ͨ��һ��ѯ�ʽ���һ��ʼ���Ƿ񱻰���***********/					
			case 2:
				MCGSTouch_Send(WAY_1,READ_KEY,0,1);
				ii++;
				break;			
			case 3:
				jj = MCGSTouch_Receive(READ_KEY,&key,&way,&time);
				if(jj == 3)
				{
					if(key==KEY_START)
					{
						ii++;
						break;
					}	
					ii--;					
				}
				else if(jj == 2)
				{
					ii--;
				}
				break;
		/*******************************************************/	
		/***********ͨ��һ�����ý���һ��ģʽ��ʱ��**************/	
			case 4:
				delay(0x1000000);
				MCGSTouch_Send(WAY_1,SET_MODE_TIME,0,0);
				ii++;
				break;
			case 5:
				if(MCGSTouch_Receive(SET_MODE_TIME,&key,&way,&time) == 1)
				{
					ii++;					
				}
				break;
		/*******************************************************/	
		/********************ͨ��һ����հ���*******************/	
			case 6:
				delay(0x1000000);
				MCGSTouch_Send(WAY_1,EMPTY_KEY,0,1);
				ii++;
				break;
			case 7:
				if(MCGSTouch_Receive(EMPTY_KEY,&key,&way,&time) == 1)
				{
					ii++;
				}
				break;
		/*******************************************************/	
		/********************ͨ��һ����ʾ�����*****************/	
			case 8:
				MCGSTouch_Send(WAY_1,DISPLAY_INTERFACE_2,1,1);
				ii++;
				break;
			case 9:
				if(MCGSTouch_Receive(DISPLAY_INTERFACE_2,&key,&way,&time) == 1)
				{
					ii++;
				}
				break;
		/*******************************************************/	
		/*******************ͨ��һ������ʣ��ʱ��****************/					
			case 10:
				if(kk>30)
				{
					ii=ii+2;
					kk=0;
					break;
				}
				MCGSTouch_Send(WAY_1,UPDATE_TIME,1,kk);
				delay(0x800000);
				ii++;
				break;
			case 11:
				if(MCGSTouch_Receive(UPDATE_TIME,&key,&way,&time) == 1)
				{					
					kk=kk+10;
					ii--;
				}
				break;	
		/*******************************************************/	
		/************ͨ��һ��ѯ�ʽ����ĳ���Ƿ񱻰���***********/					
			case 12:
				MCGSTouch_Send(WAY_1,READ_KEY,0,1);
				ii++;
				break;			
			case 13:
				jj = MCGSTouch_Receive(READ_KEY,&key,&way,&time);
				if(jj == 3)
				{
					if(key ==KEY_CONTINUED)
					{
						ii =30;
						break;
					}
					else if(key ==KEY_PAUSE)
					{
						ii =40;
						break;
					}
					else if(key ==KEY_CANCEL)
					{
						ii++;
						break;
					}
					ii--;
				}
				else if(jj == 2)
				{
					ii--;
				}
				break;
		/*******************************************************/	
		/*****************ͨ��һ����ʾ������********************/	
			case 14:
				MCGSTouch_Send(WAY_1,DISPLAY_INTERFACE_3,0,0);
				ii++;
				break;
			case 15:
				if(MCGSTouch_Receive(DISPLAY_INTERFACE_3,&key,&way,&time) == 1)
				{
					ii++;
				}
				break;
		/*******************************************************/	
		/************ͨ��һ��ѯ�ʽ�����ĳ���Ƿ񱻰���***********/	
			case 16:
				MCGSTouch_Send(WAY_1,READ_KEY,0,1);
				ii++;
				break;			
			case 17:
				jj = MCGSTouch_Receive(READ_KEY,&key,&way,&time);
				if(jj == 3)
				{
					if(key ==KEY_ENTER)
					{
						ii=0;
					}					
				}
				else if(jj == 2)
				{
					ii--;
				}
				break;
		/*******************************************************/	
		/*******************ͨ��һ����ʾ��ͣ��******************/	
			case 30:
				MCGSTouch_Send(WAY_1,DISPLAY_PAUSE,0,1);
				ii++;
				break;			
			case 31:
				if(MCGSTouch_Receive(DISPLAY_PAUSE,&key,&way,&time) == 1)
				{
					ii=12;
				}
				break;
		/*******************************************************/	
		/*******************ͨ��һ����ʾ������******************/	
			case 40:
				MCGSTouch_Send(WAY_1,DISPLAY_CONTINUED,0,1);
				ii++;
				break;			
			case 41:
				if(MCGSTouch_Receive(DISPLAY_CONTINUED,&key,&way,&time) == 1)
				{
					ii=12;
				}
				break;
		/*******************************************************/	
		}		
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/


