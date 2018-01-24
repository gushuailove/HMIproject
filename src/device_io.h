#ifndef DEVICE_IO_H__
#define DEVICE_IO_H__
#include  "stdint.h"
#include "stm32f10x_gpio.h"
#define CHANNEL_MAX  2

#define WORK_MODE_ALL 0//remaining time count uint is 1s
#define WORK_MODE_HALF 1//remaining time count uint is 1s

extern uint8_t speak_switch;

typedef enum {
	SWITCH_PUMP = 0,//ref overlay sheet	
	SWITCH_O3,
	SWITCH_ATOMIZER,
	SWITCH_MAX
}SwitchIO;
typedef enum {
	FOOT_KEY = 0,//ref overlay sheet	
	WATER_KEY,
	KEY_MAX
}KeyIO;


typedef enum {
	DEVICE_STATE_POWERON = 0,//ref overlay sheet	
	DEVICE_STATE_WAITING,
	DEVICE_STATE_START,
	DEVICE_STATE_STOP,
}DeviceStateType;


typedef union
{
	uint32_t all;
	struct
	{
		unsigned start:1;
		unsigned stop:1;
		unsigned continued:1;
		unsigned water:1;
	}bit;
}DeviceData;

typedef struct IODEVICE
{
	GPIO_TypeDef* port;
	uint16_t pin;
}IO_DEVICE;
//             pump   o3    atomizer   footkey   water
//channel 1    PC9    PB15  PC7        PA5		 PC4
//channel 2    PA8    PC6   PC8        PC10		 PA7
extern const IO_DEVICE array_io[CHANNEL_MAX][SWITCH_MAX];

extern DeviceData device_data[CHANNEL_MAX];

void device_initial(void);
void read_work_value(uint8_t chn, uint8_t* mode, uint8_t* time);
void write_work_value(uint8_t chn, uint8_t mode, uint8_t time);//write touchscreen data
void device_loop(DeviceStateType device_state, uint8_t channel_id);
void set_device_switch(SwitchIO io, uint8_t channel_id);
void reset_device_switch(SwitchIO io, uint8_t channel_id);
void open_switch(uint8_t mode, uint8_t channel_id);
void close_switch(uint8_t channel_id);
#endif

