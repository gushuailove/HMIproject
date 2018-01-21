/************************************************************************************
 * 文件名  ：usart1.h
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 日  期  ：2014-7-29
 * 作  者  ：李彦辉
**********************************************************************************/	
#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include "timer.h"

#define		USART1_RX_BUF				100    		//接收数据缓存长度
#define		USART1_MAP				0						//条件编译，用于串口是否映射  1：映射（PA9 PA10） 0：不映射（PB6 PB7）
#define		USART1_TIMER         3					//串口数据接收延时，在30ms内没有新的数据则认为单次数据接收完成

typedef  unsigned char  USART1_Buf_Type;

typedef struct
{
	USART1_Buf_Type  array[USART1_RX_BUF]; //数据缓存
	uint16_t  valid;  										 //缓存中当前的有效数据个数
	uint8_t  f;													   //缓存中是否有完整数据  0:没有 1：有 2：数据溢出
}Usart1Buf;

Usart1Buf*  get_buf_usart1_pt(void); 		//输出指向缓存的指针
void usart1_config(void);								//USART1 GPIO 配置,工作模式配置
void usart1_send(uint8_t*  send_data , uint16_t  send_size);//USART1 发送数据函数
uint8_t Get_Buf_Usart1_PT(Usart1Buf*  temp_data);//将串口数据缓存指针赋给temp_data中保存的指针函数


#endif /* __USART1_H */
