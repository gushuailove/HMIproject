#include "sys_handle.h"
#include "device_io.h"
#include "MCGSTouch.h"


uint32_t sys_poweron_handle(uint8_t channel_id);
uint32_t sys_waiting_handle(uint8_t channel_id);
uint32_t sys_start_handle(uint8_t channel_id);
uint32_t sys_stop_handle(uint8_t channel_id);

volatile TimerDelay* sys_ov_timers[CHANNEL_MAX];
volatile TimerDelay* remaining_timers[CHANNEL_MAX];

ChannelMessage channel_message[CHANNEL_MAX];

SysHandle* sys_handle_fuc[] = {
	sys_poweron_handle,
	sys_waiting_handle,
	sys_start_handle,
	sys_stop_handle,
};




void sys_handle_initial(void)
{
	uint8_t i;

	for(i = 0; i < CHANNEL_MAX; i++){
		sys_ov_timers[i] = Delay_Initial(SYS_OV_TIME);
		remaining_timers[i] = Delay_Initial(REMAININ1_TIME);
	//	memset(&channel_message[i], 0, sizeof(channel_message[i]));
	}
}

/*
return N_S_C_R--not switch channel S_C_R--switch channel
*/
uint32_t sys_poweron_handle(uint8_t channel_id)
{
	uint8_t work_mode, work_time, key, way, time;
	volatile TimerDelay* ov_timer = sys_ov_timers[channel_id];
	uint8_t* state = &(channel_message[channel_id].state);
	
	if(channel_id>=2){
		while(1);
	}
	switch (*state){
		case 0://get default value
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			read_work_value(channel_id, &work_mode, &work_time);
			MCGSTouch_Send(channel_id+1, DISPLAY_INTERFACE_1, work_mode, work_time);
			(*state) ++;
			return N_S_C_R;
		case 1://judge achieve success
			if(MCGSTouch_Receive(DISPLAY_INTERFACE_1,&key,&way,&time) == 1){
				channel_message[channel_id].sys_state ++;
				*state = 0;
				break;
			}
			if(Delay_Ok(ov_timer)){//error overtime
				*state = 0;
			}
			else{//wait touchscreen answer
				return N_S_C_R;
			}			
			break;
		default:
			*state = 0;
			break;
	}
	return S_C_R;
}

/*
return N_S_C_R--not switch channel S_C_R--switch channel
*/
uint32_t sys_waiting_handle(uint8_t channel_id)
{
	uint8_t key_state, work_time, key, way, time;
	volatile TimerDelay* ov_timer = sys_ov_timers[channel_id];
	uint8_t* state = &(channel_message[channel_id].state);

	switch (*state){
		case 0://send command to check key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id+1, READ_KEY, 0, 1);
			(*state) ++;
			return N_S_C_R;
		case 1://get touch screen result
			key_state = MCGSTouch_Receive(READ_KEY,&key,&way,&time);
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
				return N_S_C_R;
			}
			break;
		case 2://judge foot key
			if(device_data[channel_id].bit.start){
				*state = 3;
			}
			else{
				*state = 0;//loop check
			}
			break;
		case 3://set interface 1 mode and time
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id+1,SET_MODE_TIME,0,0);
			(*state) ++;
			return N_S_C_R;
		case 4://judje reply
			if(MCGSTouch_Receive(SET_MODE_TIME,&key,&way,&time) == 1){
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer)){
				*state = 0;
			}
			else{
				return N_S_C_R;
			}
			break;
		case 5://clear press key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id+1,EMPTY_KEY,0,1);
			(*state) ++;
			return N_S_C_R;
		case 6:
			if(MCGSTouch_Receive(EMPTY_KEY,&key,&way,&time) == 1){
				(*state) ++;
			}
			else if(Delay_Ok(ov_timer)){
				*state = 0;
			}
			else{
				return N_S_C_R;
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
	return S_C_R;
}

#define START_STATE_NO_WATER  10
/*
return N_S_C_R--not switch channel S_C_R--switch channel
*/
uint32_t sys_start_handle(uint8_t channel_id)
{
	uint8_t key_state, work_time, key, way, time;
	volatile TimerDelay* ov_timer = sys_ov_timers[channel_id];
	volatile TimerDelay* rm_timer = remaining_timers[channel_id];
	uint8_t* state = &(channel_message[channel_id].state);

	switch (*state){
		case 0://switch interface to 2
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id+1,DISPLAY_INTERFACE_2,1,1); 
			(*state) ++;
			return N_S_C_R;
		case 1://get touch screen result
			if(MCGSTouch_Receive(DISPLAY_INTERFACE_2,&key,&way,&time) == 1){
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer)){
				*state = 0;
			}
			else{
				return N_S_C_R;
			}
			break;
		case 2://update remaining time
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);		
			MCGSTouch_Send(channel_id+1,UPDATE_TIME,1,channel_message[channel_id].remaining_time);// mode update??
			(*state) ++;
			return N_S_C_R;
		case 3://judge reply
			if(MCGSTouch_Receive(UPDATE_TIME,&key,&way,&time) == 1){					
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer)){
				*state = 0;
			}
			else{
				return N_S_C_R;
			}
			break;
		case 4://ask touchscreen key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);		
			MCGSTouch_Send(channel_id+1,READ_KEY,0,1);
			(*state) ++;
			return N_S_C_R;
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
				return N_S_C_R;
			}
			break;
		case 6://ask device IO
			if(device_data[channel_id].bit.stop){
				*state = 0;
				channel_message[channel_id].sys_state ++;
			}
			else if(device_data[channel_id].bit.water && 
				(channel_message[channel_id].work_mode == WORK_MODE_ALL)){//no water
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
			break;			
		case START_STATE_NO_WATER:
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);		
			MCGSTouch_Send(channel_id+1, DISPLAY_INTERFACE_3, 0, 0);
			return N_S_C_R;
		case START_STATE_NO_WATER + 1:
			if(MCGSTouch_Receive(DISPLAY_INTERFACE_3,&key,&way,&time) == 1){
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer)){
				*state = 0;
			}
			else{
				return N_S_C_R;
			}
			break;
		case START_STATE_NO_WATER + 2://send command to check key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id+1, READ_KEY, 0, 1);
			(*state) ++;
			return N_S_C_R;
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
				return N_S_C_R;
			}
			break;
		default:
			*state = 0;
			break;
	}
	return S_C_R;
}

/*
return N_S_C_R--not switch channel S_C_R--switch channel
*/
uint32_t sys_stop_handle(uint8_t channel_id)
{
	uint8_t key_state, work_time, key, way, time;
	volatile TimerDelay* ov_timer = sys_ov_timers[channel_id];
	uint8_t* state = &(channel_message[channel_id].state);

	switch (*state){
		case 0://display continue key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);
			MCGSTouch_Send(channel_id+1,DISPLAY_CONTINUED,0,1);
			(*state) ++;
			return N_S_C_R;
		case 1://judge reply
			if(MCGSTouch_Receive(DISPLAY_INTERFACE_2,&key,&way,&time) == 1){
				(*state) ++;				
			}
			else if(Delay_Ok(ov_timer)){
				*state = 0;
			}
			else{
				return N_S_C_R;
			}
			break;
		case 2://ask key
			Close_Delay(ov_timer);
			Open_Delay(ov_timer);		
			MCGSTouch_Send(channel_id+1,READ_KEY,0,1);
			return N_S_C_R;
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
				return N_S_C_R;
			}
			break;
		case 4://ask device IO
			if(device_data[channel_id].bit.continued){
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
	return S_C_R;
}



