#ifndef DEVICE_IO_H__
#define DEVICE_IO_H__


#define CHANNEL_MAX  2

#define WORK_MODE_ALL 1//remaining time count uint is 1s
#define WORK_MODE_HALF 2//remaining time count uint is 1s


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
		unsigned continue:1;
		unsigned cancel:1;
		unsigned water:1;
	}bit;
}DeviceData;

extern DeviceData device_data[CHANNEL_MAX];

void read_work_value(uint8_t chn, uint8_t* mode, uint8_t* time);
void write_work_value(uint8_t chn, uint8_t mode, uint8_t time);//write touchscreen data
void device_loop(DeviceStateType device_state, uint8_t channel_id);

#endif

