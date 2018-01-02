#ifndef DEVICE_IO_H__
#define DEVICE_IO_H__


#define CHANNEL_MAX  2

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
		unsigned restart:1;
		unsigned cancel:1;
		unsigned water:1;
	}bit;
}DeviceData;

DeviceData device_data[CHANNEL_MAX];

void device_loop(DeviceStateType device_statu);

#endif

