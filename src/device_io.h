#ifndef DEVICE_IO_H__
#define DEVICE_IO_H__
#include  "stdint.h"

#define CHANNEL_MAX  2

#define WORK_MODE_ALL 1//remaining time count uint is 1s
#define WORK_MODE_HALF 2//remaining time count uint is 1s


typedef enum {
	SWITCH_PUMP = 0,//ref overlay sheet	
	SWITCH_O3,
	SWITCH_ATOMIZER,
	SWITCH_MAX
}SwitchIO;


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
		unsigned cancel:1;
		unsigned water:1;
	}bit;
}DeviceData;

extern DeviceData device_data[CHANNEL_MAX];

void device_initial(void);
void read_work_value(uint8_t chn, uint8_t* mode, uint8_t* time);
void write_work_value(uint8_t chn, uint8_t mode, uint8_t time);//write touchscreen data
void device_loop(DeviceStateType device_state, uint8_t channel_id);
void set_device_switch(SwitchIO io, uint8_t channel_id);
void Reset_device_switch(SwitchIO io, uint8_t channel_id);

#endif

