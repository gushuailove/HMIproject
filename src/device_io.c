#include "device_io.h"


/*debug use*/
static uint8_t test_foot_value = 0;
static uint8_t test_sensor_value = 0;


DeviceData device_data[CHANNEL_MAX];
static uint8_t work_mode[CHANNEL_MAX] = {WORK_MODE_ALL, WORK_MODE_ALL};
static uint8_t work_time[CHANNEL_MAX] = {30, 30};


uint8_t detect_foot_io(void)
{
	if(test_foot_value){
		test_foot_value = 0;
		return 1;
	}
	return 0;
}

uint8_t detect_sensor_io(void)
{
	if(test_sensor_value){
		test_sensor_value = 0;
		return 1;
	}
	return 0;
}

void read_work_value(uint8_t chn, uint8_t* mode, uint8_t* time)
{
	if(chn >= CHANNEL_MAX)//error
		return;
	*mode = work_mode[chn - 1];
	*time = work_time[chn - 1];
}


void write_work_value(uint8_t chn, uint8_t mode, uint8_t time)//write touchscreen data
{
	if(chn >= CHANNEL_MAX)//error
		return;
	work_mode[chn - 1] = mode;
	work_time[chn - 1] = time;
}



void device_loop(DeviceStateType device_state, uint8_t channel_id)
{
	static DeviceStateType current_state[CHANNEL_MAX];
	DeviceData *data = *(device_data[channel_id]);
		
	if(current_state[channel_id] != device_state){//state switch
		data->all = 0;
		current_state[channel_id] = device_state[channel];		
	}
	switch(device_state){
		case DEVICE_STATE_POWERON://waiting update last set data
			
			break;
		case DEVICE_STATE_WAITING:
			if(detect_foot_io()){
				data->bit.start = 1;
			}
			break;
		case DEVICE_STATE_START:
			if(detect_foot_io()){
				data->bit.start = 1;
			}
			if(detect_sensor_io()){
				data->water = 1;
			}			
			break;
		case DEVICE_STATE_STOP:
			if(detect_foot_io()){
				data->bit.restart = 1;
			}			
			break;
		default:
			break;
	}
}




