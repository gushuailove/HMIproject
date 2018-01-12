#ifndef __MCGSTOUCH_H
#define	__MCGSTOUCH_H

#include "stm32f10x.h"
#include "uart5.h"

/**************串口初始化宏定义**************/
#define		USATTBUF					Uart5Buf
#define		USARTCONFIG				uart5_config
#define		GETBUF						Get_Buf_Uart5_PT
#define		SENDMESSAGE				uart5_send
/********************************************/

/******************Way类型******************/
#define		WAY_1					1
#define		WAY_2					2
/********************************************/

/******************Type类型******************/
#define		DISPLAY_INTERFACE_1		1
#define		DISPLAY_INTERFACE_2		2
#define		DISPLAY_INTERFACE_3		3
#define		DISPLAY_CONTINUED			4
#define		DISPLAY_PAUSE					5
#define		UPDATE_TIME						6
#define		READ_KEY							7
#define		EMPTY_KEY							8
#define		SET_MODE_TIME					9
/********************************************/

/*********************Key********************/
#define		KEY_START							0x0c
#define		KEY_CONTINUED					0x04
#define		KEY_PAUSE							0x06
#define		KEY_CANCEL						0x08
#define		KEY_ENTER							0x0a
/********************************************/

/*******************函数声明*****************/
void MCGSTouch_Initial(void);
void MCGSTouch_Send(uint8_t way,uint8_t type,uint8_t data1,uint8_t data2);
uint8_t MCGSTouch_Receive(uint8_t type,uint8_t *key,uint8_t *way,uint8_t *time);
/********************************************/
#endif 
