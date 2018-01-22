#ifndef __UART5_H
#define	__UART5_H

#include "stm32f10x.h"
#include "timer.h"

#define		UART5_RX_BUF		200    //接收数据缓存长度
#define		UART5_TIMER         10					//串口数据接收延时，在30ms内没有新的数据则认为单次数据接收完成

typedef  unsigned char  UART5_Buf_Type;
typedef struct
{
	UART5_Buf_Type  array[UART5_RX_BUF];  //数据缓存
	uint16_t   valid;  //缓存中当前的有效数据个数
	uint8_t  f;													//缓存中是否有完整数据  0:没有 1：有 2：数据溢出
}Uart5Buf;

Uart5Buf*  get_buf_uart5_pt(void); //输出指向缓存的指针
void uart5_config(void);  //UART5 GPIO 配置,工作模式配置
void uart5_send(uint8_t*  send_data , uint16_t  send_size);//UART5 发送数据函数
uint8_t Get_Buf_Uart5_PT(Uart5Buf*  temp_data);
void NVIC_uart5_Configuration(void);
#endif /* __UART5_H */

