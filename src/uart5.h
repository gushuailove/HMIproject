#ifndef __UART5_H
#define	__UART5_H

#include "stm32f10x.h"
#include "timer.h"

#define		UART5_RX_BUF		200    //�������ݻ��泤��
#define		UART5_TIMER         10					//�������ݽ�����ʱ����30ms��û���µ���������Ϊ�������ݽ������

typedef  unsigned char  UART5_Buf_Type;
typedef struct
{
	UART5_Buf_Type  array[UART5_RX_BUF];  //���ݻ���
	uint16_t   valid;  //�����е�ǰ����Ч���ݸ���
	uint8_t  f;													//�������Ƿ�����������  0:û�� 1���� 2���������
}Uart5Buf;

Uart5Buf*  get_buf_uart5_pt(void); //���ָ�򻺴��ָ��
void uart5_config(void);  //UART5 GPIO ����,����ģʽ����
void uart5_send(uint8_t*  send_data , uint16_t  send_size);//UART5 �������ݺ���
uint8_t Get_Buf_Uart5_PT(Uart5Buf*  temp_data);
void NVIC_uart5_Configuration(void);
#endif /* __UART5_H */

