#include "stm32f10x.h"
#include "timer_config.h"
#include "pwm.h"
#include "timer.h"
#include "MCGSTouch.h"
#include "sys_handle.h"
#include "device_io.h"

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




int main(void)
{
	static uint8_t chn = 0;
	uint8_t i,func_index;
	device_initial();
//	Tim2_Config();
	Tim3_Config();
	MCGSTouch_Initial();//MCGSTouch_Initial初始化;
	sys_handle_initial();
	
	while(1){
		
		func_index = channel_message[chn].sys_state;
		if(sys_handle_fuc[func_index](chn) == S_C_R){
			chn ++;
		}
		if(chn == CHANNEL_MAX){
			chn = 0;
		}
		for(i = 0; i < CHANNEL_MAX;i++){
			device_loop(channel_message[i].sys_state, i);
		}
		
	}
	
	while(1)
	{
		switch(ii)
		{
		/****************通道一：显示界面一*********************/
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
		/**********通道一：询问界面一开始键是否被按下***********/					
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
		/***********通道一：设置界面一的模式和时间**************/	
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
		/********************通道一：清空按键*******************/	
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
		/********************通道一：显示界面二*****************/	
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
		/*******************通道一：设置剩余时间****************/					
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
		/************通道一：询问界面二某键是否被按下***********/					
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
		/*****************通道一：显示界面三********************/	
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
		/************通道一：询问界面三某键是否被按下***********/	
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
		/*******************通道一：显示暂停键******************/	
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
		/*******************通道一：显示继续键******************/	
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



