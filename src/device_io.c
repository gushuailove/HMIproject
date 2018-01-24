///////////////////////////////////
//
//             pump   o3    atomizer   footkey   water
//channel 1    PC9    PB15  PC7        PA5		 PC4
//channel 2    PA8    PC6   PC8        PC10		 PA7
//
///////////////////////////////////
#include "device_io.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "timer.h"


const IO_DEVICE array_io[CHANNEL_MAX][SWITCH_MAX] = {
				{{GPIOC, GPIO_Pin_9},{GPIOB, GPIO_Pin_15},{GPIOC, GPIO_Pin_7}},
				{{GPIOA, GPIO_Pin_8},{GPIOC, GPIO_Pin_6},{GPIOC, GPIO_Pin_8}}
};

const IO_DEVICE array_key[CHANNEL_MAX][KEY_MAX] = {
				{{GPIOA, GPIO_Pin_5},{GPIOC, GPIO_Pin_4}},
				{{GPIOC, GPIO_Pin_10},{GPIOA, GPIO_Pin_7}}
};
DeviceData device_data[CHANNEL_MAX];
uint8_t speak_switch;

static uint8_t work_mode[CHANNEL_MAX] = {WORK_MODE_ALL, WORK_MODE_ALL};
static uint8_t work_time[CHANNEL_MAX] = {30, 30};

static volatile TimerDelay* ov_timer;
static volatile TimerDelay* flt_timer;

static void loop_speak(void);
static uint8_t detect_io(uint8_t type, uint8_t channel_id);


uint8_t detect_sensor_io(void)
{
	static uint8_t test_sensor_value = 0;		
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
	*mode = work_mode[chn];
	*time = work_time[chn];
}


void write_work_value(uint8_t chn, uint8_t mode, uint8_t time)//write touchscreen data
{
	if(chn >= CHANNEL_MAX)//error
		return;
	if((work_time[chn] > 30)||(work_time[chn] <= 0)){
		work_time[chn] = 30;
	}
	else{
		work_time[chn] = time;
	}
	work_mode[chn] = mode;

}




void device_initial(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);	 // turn off all switch	
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);	 
	GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);	 // turn off all switch
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_10;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
// 	test_foot_value = 0;
// 	test_foot_value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
// 	test_foot_value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
// 	test_foot_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
// 	test_foot_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);

	ov_timer = Delay_Initial(10);
	flt_timer = Delay_Initial(10);
	for(i = 0; i < CHANNEL_MAX; i++){
		work_mode[i] = WORK_MODE_ALL;
		work_time[i] = 30;
	}	
	speak_switch = 0;
}


void device_loop(DeviceStateType device_state, uint8_t channel_id)
{
	static DeviceStateType current_state[CHANNEL_MAX];
	DeviceData *data = &(device_data[channel_id]);
		
	if(current_state[channel_id] != device_state){//state switch
		data->all = 0;
		current_state[channel_id] = device_state;		
	}
	switch(device_state){
		case DEVICE_STATE_POWERON://waiting update last set data
			
			break;
		case DEVICE_STATE_WAITING:
			if(detect_io(1, channel_id)){
				data->bit.start = 1;
			}
			break;
		case DEVICE_STATE_START:
			if(detect_io(2, channel_id)){
				data->bit.stop = 1;
			}
			if(detect_io(0, channel_id)){
				data->bit.water = 1;
			}			
			break;
		case DEVICE_STATE_STOP:
			if(detect_io(1, channel_id)){
				data->bit.continued = 1;
			}			
			break;
		default:
			break;
	}
	loop_speak();
	detect_io(0, channel_id);
}

void set_device_switch(SwitchIO io, uint8_t channel_id)
{
	GPIO_SetBits(array_io[channel_id][io].port, array_io[channel_id][io].pin);
}

void reset_device_switch(SwitchIO io, uint8_t channel_id)
{
	GPIO_ResetBits(array_io[channel_id][io].port, array_io[channel_id][io].pin);
}

uint8_t get_device_switch(KeyIO io, uint8_t channel_id)
{
	return GPIO_ReadInputDataBit(array_key[channel_id][io].port, array_key[channel_id][io].pin);
}

void open_switch(uint8_t mode, uint8_t channel_id)
{
	if(mode == WORK_MODE_ALL){
		set_device_switch(SWITCH_PUMP, channel_id);
		set_device_switch(SWITCH_O3, channel_id);
		set_device_switch(SWITCH_ATOMIZER, channel_id);					
	}
	else if(mode == WORK_MODE_HALF){
		set_device_switch(SWITCH_PUMP, channel_id);
		set_device_switch(SWITCH_O3, channel_id);
	}
	else{
		;
	}
}

void close_switch(uint8_t channel_id)
{
	reset_device_switch(SWITCH_PUMP, channel_id);
	reset_device_switch(SWITCH_O3, channel_id);
	reset_device_switch(SWITCH_ATOMIZER, channel_id);	
}

static void loop_speak()
{
	static uint8_t k;
	if(speak_switch == 0){
		TIM_Cmd(TIM2, DISABLE);
		return;		
	}

	Open_Delay(ov_timer);
	if(Delay_Ok(ov_timer)){
		k ++;
		if(k >= 20){
			TIM_Cmd(TIM2, ENABLE);
			k = 0;
		}
		else{
			TIM_Cmd(TIM2, DISABLE);
		}
	}
}

/*
type =0--is no woter?   =1--is start  =2--is stop
return 0--no !0--yes
*/
uint8_t detect_io(uint8_t type, uint8_t channel_id)
{
	static uint8_t foot_curr_value[CHANNEL_MAX];
	static uint8_t foot_last_value[CHANNEL_MAX];
	static uint8_t foot_temp_value[CHANNEL_MAX];
	static uint8_t foot_count[CHANNEL_MAX];
	static uint8_t water_curr_value[CHANNEL_MAX];
	static uint8_t water_temp_value[CHANNEL_MAX];
	static uint8_t water_count[CHANNEL_MAX];
	uint8_t temp;

	Open_Delay(flt_timer);
	if(Delay_Ok(flt_timer)){
		//footkey filter
		temp = get_device_switch(FOOT_KEY, channel_id);
		if(temp != foot_temp_value[channel_id]){
			foot_temp_value[channel_id] = temp;
		}
		else{
			foot_count[channel_id] ++;
			if(foot_count[channel_id] >= 10){//change state
				foot_count[channel_id] = 0;
				foot_last_value[channel_id] = foot_curr_value[channel_id];
				foot_curr_value[channel_id] = temp;				
			}
		}
		//water sensor filter
		temp = get_device_switch(WATER_KEY, channel_id);
		if(temp != water_temp_value[channel_id]){
			water_temp_value[channel_id] = temp;
		}
		else{
			water_count[channel_id] ++;
			if(water_count[channel_id] >= 10){//change state
				water_count[channel_id] = 0;
				water_curr_value[channel_id] = temp;				
			}
		}
	}
	switch(type){
		case 0://low votage signal
			if(water_curr_value[channel_id] == 0)
				return 1;
			break;
		case 1://low to high
			if((foot_last_value[channel_id] == 0)&&(foot_curr_value[channel_id] == 1))
				return 1;
			break;
		case 2://high to low
			if((foot_last_value[channel_id] == 1)&&(foot_curr_value[channel_id] == 0))
				return 1;
			break;
	}
	return 0;
}


