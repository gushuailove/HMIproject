#include "device_io.h"


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




