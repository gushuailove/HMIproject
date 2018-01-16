#include "stm32f10x.h"
#include "timer_config.h"
#include "timer.h"
#include "MCGSTouch.h"
#include "sys_handle.h"
#include "device_io.h"





int main(void)
{
	static uint8_t chn = 0;
	uint8_t i,func_index;
	
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
}



