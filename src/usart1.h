/************************************************************************************
 * �ļ���  ��usart1.h
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��  ��  ��2014-7-29
 * ��  ��  �������
**********************************************************************************/	
#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include "timer.h"

#define		USART1_RX_BUF				100    		//�������ݻ��泤��
#define		USART1_MAP				0						//�������룬���ڴ����Ƿ�ӳ��  1��ӳ�䣨PA9 PA10�� 0����ӳ�䣨PB6 PB7��
#define		USART1_TIMER         3					//�������ݽ�����ʱ����30ms��û���µ���������Ϊ�������ݽ������

typedef  unsigned char  USART1_Buf_Type;

typedef struct
{
	USART1_Buf_Type  array[USART1_RX_BUF]; //���ݻ���
	uint16_t  valid;  										 //�����е�ǰ����Ч���ݸ���
	uint8_t  f;													   //�������Ƿ�����������  0:û�� 1���� 2���������
}Usart1Buf;

Usart1Buf*  get_buf_usart1_pt(void); 		//���ָ�򻺴��ָ��
void usart1_config(void);								//USART1 GPIO ����,����ģʽ����
void usart1_send(uint8_t*  send_data , uint16_t  send_size);//USART1 �������ݺ���
uint8_t Get_Buf_Usart1_PT(Usart1Buf*  temp_data);//���������ݻ���ָ�븳��temp_data�б����ָ�뺯��


#endif /* __USART1_H */
