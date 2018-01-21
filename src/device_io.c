///////////////////////////////////
//
//             pump   o3    atomizer   footkey   
//channel 1    PC9    PB15  PC7        PA5
//channel 2    PA8    PC6   PC8        PC10
//
///////////////////////////////////
#include "device_io.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

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




void device_initial(void)
{
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
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);	 
	GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);	 // turn off all switch
	
// 	GPIO_SetBits(GPIOA, GPIO_Pin_8);	 // turn off all switch	
// 	GPIO_SetBits(GPIOB, GPIO_Pin_15);	 // turn off all switch	
// 	GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);	 // turn off all switch	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_10;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	test_foot_value = 0;
	test_foot_value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);
	test_foot_value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
	test_foot_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
	test_foot_value = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);
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
			if(detect_foot_io()){
				data->bit.start = 1;
			}
			break;
		case DEVICE_STATE_START:
			if(detect_foot_io()){
				data->bit.start = 1;
			}
			if(detect_sensor_io()){
				data->bit.water = 1;
			}			
			break;
		case DEVICE_STATE_STOP:
			if(detect_foot_io()){
				data->bit.continued = 1;
			}			
			break;
		default:
			break;
	}
}




