#ifndef SYS_HANDLE_H__
#define SYS_HANDLE_H__


#define SYS_OV_TIME 500//system overtimer is 5s
#define REMAININ1_TIME 6000//remaining time count uint is 1min

#define N_S_C_R		0//NOT_SWITCH_CHANNEL_RETURN
#define S_C_R		1//SWITCH_CHANNEL_RETURN


typedef uint32_t SysHandle(uint8_t channel_id);


typedef struct
{
	DeviceStateType sys_state;
	uint8_t state;
	uint8_t remaining_time;
	uint8_t work_mode;
}ChannelMessage;

extern const SysHandle* sys_handle_fuc[];


void sys_handle_initial(void);


#endif

