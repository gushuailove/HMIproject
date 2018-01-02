#include "device_io.h"


uint8_t foot_key_flag;


void device_loop(DeviceStateType device_state)
{
	static DeviceStateType current_state[CHANNEL_MAX];
	uint8_t channel;
	DeviceStateType *state;
	DeviceData *data;
	for(channel = 0; channel < CHANNEL_MAX; channel ++){
		state = *(current_state[channel]);
		data = *(device_data[channel]);
		
		if(*state != device_state[channel]){//state switch
			data->all = 0;
			*state = device_state[channel];		
		}
		switch(*state){
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
}




