#include "stm32f10x.h"
#include "timer_config.h"
#include "timer.h"
#include "MCGSTouch.h"

#define SYS_OV_TIME 500//system overtimer is 5s
#define REMAININ1_TIME 6000//remaining time count uint is 1min

#define WORK_MODE_ALL 1//remaining time count uint is 1s
#define WORK_MODE_HALF 2//remaining time count uint is 1s


volatile TimerDelay* sys_ov_timers[CHANNEL_MAX];
volatile TimerDelay* remaining_timers[CHANNEL_MAX];

typedef uint32_t SysHandle(uint8_t channel_id);

SysHandle* sys_handle_fuc[] = {
	sys_poweron_handle,
	sys_waiting_handle,
	sys_start_handle,
	sys_stop_handle,
	}


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
 * 函数名：main
 * 描述  : 演示程序
 * 输入  ：无
 * 输出  : 无
 */
int main(void)
{

	Tim3_Config();
	
	//MCGSTouch_Initial初始化
	MCGSTouch_Initial();
	
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

typedef struct
{
	DeviceStateType sys_state;
	uint8_t state;
	uint8_t remaining_time;
	uint8_t work_mode;
}ChannelMessage;

ChannelMessage channel_message[CHANNEL_MAX];

/*
return 0--not switch channel 1--switch channel
*/
uint32_t sys_poweron_handle(uint8_t channel_id)
{
	uint8_t work_mode, work_time, key, way, time;
	volatile TimerDelay* ov_timer = sys_ov_timers[channel_id - 1];
	uint8_t* state = &(channel_message[channel_id - 1].state);
	
	switch (*state){
		case 0://get default value
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			read_work_value(channel_id, &work_mode, &work_time);
			MCGSTouch_Send(channel_id, DISPLAY_INTERFACE_1, work_mode, work_time);
			(*state) ++;
			return 0;
		case 1://judge achieve success
			if(MCGSTouch_Receive(DISPLAY_INTERFACE_1,&key,&way,&time) == 1){
				channel_message[channel_id].sys_state ++;
				*state = 0;
			}
			if(Delay_Ok(ov_timer){//error overtime
				*state = 0;
			}
			else{//wait touchscreen answer
				return 0;
			}			
			break;
		default:
			*state = 0;
			break;
	}
	return 1;
}

/*
return 0--not switch channel 1--switch channel
*/
uint32_t sys_waiting_handle(uint8_t channel_id)
{
	uint8_t key_state, work_time, key, way, time;
	volatile TimerDelay* ov_timer = sys_ov_timers[channel_id - 1];
	uint8_t* state = &(channel_message[channel_id - 1].state);

	switch (*state){
		case 0://send command to check key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id, READ_KEY, 0, 1);
			(*state) ++;
			return 0;
		case 1://get touch screen result
			key_state = MCGSTouch_Receive(DISPLAY_INTERFACE_1,&key,&way,&time);
			if(key_state == 2){//no press
				(*state) ++;
			}
			else if(key_state == 3){//press
				if(key==KEY_START){
					*state = 3;
					write_work_value(channel_id, way, time);//write touchscreen data
				}
				else{//error
					*state = 0;
				}
			}
			else if(Delay_Ok(ov_timer)){//error overtime
				*state = 0;
			}
			else{//wait touchscreen answer
				return 0;
			}
			break;
		case 2://judge foot key
			if(device_data[channel_id - 1].bit.start){
				*state = 3;
			}
			else{
				*state = 0;//loop check
			}
			break;
		case 3://set interface 1 mode and time
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(WAY_1,SET_MODE_TIME,0,0);
			(*state) ++;
			return 0;
		case 4://judje reply
			if(MCGSTouch_Receive(SET_MODE_TIME,&key,&way,&time) == 1){
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer){
				*state = 0;
			}
			else{
				return 0;
			}
			break;
		case 5://clear press key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(WAY_1,EMPTY_KEY,0,1);
			(*state) ++;
			break;
		case 6:
			if(MCGSTouch_Receive(EMPTY_KEY,&key,&way,&time) == 1){
				(*state) ++;
			}
			else if(Delay_Ok(ov_timer){
				*state = 0;
			}
			else{
				return 0;
			}
		case 7://complete
			read_work_value(channel_id, &(channel_message[channel_id].work_mode), &(channel_message[channel_id].remaining_time));
			channel_message[channel_id].sys_state ++;
			*state = 0;
			break;
		default:
			*state = 0;
			break;
	}
	return 1;
}

#define START_STATE_NO_WATER  10
/*
return 0--not switch channel 1--switch channel
*/
uint32_t sys_start_handle(uint8_t channel_id)
{
	uint8_t key_state, work_time, key, way, time;
	volatile TimerDelay* ov_timer = sys_ov_timers[channel_id - 1];
	volatile TimerDelay* rm_timer = remaining_timers[channel_id - 1];
	uint8_t* state = &(channel_message[channel_id - 1].state);

	switch (*state){
		case 0://switch interface to 2
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(WAY_1,DISPLAY_INTERFACE_2,1,1); 
			(*state) ++;
			return 0;
		case 1://get touch screen result
			if(MCGSTouch_Receive(DISPLAY_INTERFACE_2,&key,&way,&time) == 1){
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer){
				*state = 0;
			}
			else{
				return 0;
			}
			break;
		case 2://update remaining time
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);		
			MCGSTouch_Send(channel_id,UPDATE_TIME,1,channel_message[channel_id].remaining_time);// mode update??
			return 0;
		case 3://judge reply
			if(MCGSTouch_Receive(UPDATE_TIME,&key,&way,&time) == 1){					
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer){
				*state = 0;
			}
			else{
				return 0;
			}
			break;
		case 4://ask touchscreen key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);		
			MCGSTouch_Send(channel_id,READ_KEY,0,1);
			return 0;
		case 5://judge reply
			key_state = MCGSTouch_Receive(READ_KEY,&key,&way,&time);
			if(key_state == 2){//no press
				(*state) ++;
			}
			else if(key_state == 3){//press
				if(key == KEY_PAUSE){
					*state = 0;
					channel_message[channel_id].sys_state ++;
				}
				else if(key == KEY_CANCEL){
					*state = 0;
					channel_message[channel_id].sys_state = DEVICE_STATE_WAITING;
				}
				else{//error
					*state = 0;
				}
			}
			else if(Delay_Ok(ov_timer)){//error overtime
				*state = 0;
			}
			else{//wait touchscreen answer
				return 0;
			}
			break;
		case 6://ask device IO
			if(device_data[channel_id - 1].bit.stop){
				*state = 0;
				channel_message[channel_id].sys_state ++;
			}
			else if(device_data[channel_id - 1].bit.water && 
				(channel_message[channel_id].work_mode == WORK_MODE_ALL){//no water
				*state = START_STATE_NO_WATER;
				channel_message[channel_id].sys_state ++;
			}
			else{
				(*state) ++;
			}
			break;
		case 7://count time
			Open_Delay(rm_timer);
			if(Delay_Ok(rm_timer)){
				*state = 2;
			}
			else{
				*state = 4;
			}
		case START_STATE_NO_WATER:
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);		
			MCGSTouch_Send(channel_id, DISPLAY_INTERFACE_3, 0, 0);
			return 0;
		case START_STATE_NO_WATER + 1:
			if(MMCGSTouch_Receive(DISPLAY_INTERFACE_3,&key,&way,&time) == 1){
				*state ++;				
			}
			else if(Delay_Ok(ov_timer){
				*state = 0;
			}
			else{
				return 0;
			}
			break;
		case START_STATE_NO_WATER + 2://send command to check key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id, READ_KEY, 0, 1);
			(*state) ++;
			return 0;
		case START_STATE_NO_WATER + 3://get touch screen result
			key_state = MCGSTouch_Receive(DISPLAY_INTERFACE_1,&key,&way,&time);
			if(key_state == 2){//no press
				(*state) --;
			}
			else if(key_state == 3){//press
				if(key == KEY_ENTER){
					*state = 0;//?????????????					
				}
				else{//error
					*state = 0;
				}
			}
			else if(Delay_Ok(ov_timer)){//error overtime
				*state = 0;
			}
			else{//wait touchscreen answer
				return 0;
			}
			break;
		default:
			*state = 0;
			break;
	}
	return 1;
}

/*
return 0--not switch channel 1--switch channel
*/
uint32_t sys_stop_handle(uint8_t channel_id)
{
	uint8_t key_state, work_time, key, way, time;
	volatile TimerDelay* ov_timer = sys_ov_timers[channel_id - 1];
	uint8_t* state = &(channel_message[channel_id - 1].state);

	switch (*state){
		case 0://display continue key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id,DISPLAY_CONTINUED,0,1);
			(*state) ++;
			return 0;
		case 1://judge reply
			if(MCGSTouch_Receive(DISPLAY_INTERFACE_2,&key,&way,&time) == 1){
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer){
				*state = 0;
			}
			else{
				return 0;
			}
			break;
		case 2://ask key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);		
			MCGSTouch_Send(channel_id,READ_KEY,0,1);
			return 0;
		case 3://judge reply
			key_state = MCGSTouch_Receive(READ_KEY,&key,&way,&time);
			if(key_state == 2){//no press
				(*state) ++;
			}
			else if(key_state == 3){//press
				if(key == KEY_CONTINUED){
					*state = 0;
					channel_message[channel_id].sys_state --;
				}
				else if(key == KEY_CANCEL){
					*state = 0;
					channel_message[channel_id].sys_state = DEVICE_STATE_WAITING;
				}
				else{//error
					*state = 0;
				}
			}
			else if(Delay_Ok(ov_timer)){//error overtime
				*state = 0;
			}
			else{//wait touchscreen answer
				return 0;
			}
			break;
		case 4://ask device IO
			if(device_data[channel_id - 1].bit.continue){
				*state = 0;
				channel_message[channel_id].sys_state --;
			}
			else{
				*state = 2;
			}
			break;
		default:
			*state = 0;
			break;
	}
	return 1;
}


int main(void)
{
	static uint8_t chn = 0;
	uint8_t i,j,k,func_index;
	for(i = 0, i < CHANNEL_MAX, i++){
		sys_ov_timers[i] = Delay_Initial(SYS_OV_TIME);
		remaining_timers[i] = Delay_Initial(REMAININ1_TIME);
	}

	while(1){
		func_index = channel_message[chn].sys_state;
		if(sys_handle_fuc[func_index](chn)){
			chn ++;
		}
		if(chn == CHANNEL_MAX)
			chn = 0;
	}
}



