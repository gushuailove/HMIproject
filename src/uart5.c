/**************************************************************************
 * 文件名  ：uart5.c
 * 描述    ：         
						 ------------------------
 *          | PC12  - UART5(Tx)      |
 *          | PD2   - UART5(Rx)      |
 *           ------------------------
 * 日期    ：2014-7-29
 * 作者    ：gushuai 
**********************************************************************************/	
#include "uart5.h"

static Uart5Buf  Data;
static volatile TimerDelay* uart5_timer;


/*
UART5 中断配置
 */
static void NVIC_uart5_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);        //中断配置	
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
}


/*
UART5 GPIO 配置,工作模式配置
 */
void uart5_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
 	/* config UART5 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
 	
 	/* UART5 GPIO config */
 	/* Configure UART5 Tx (PC.12) as alternate function push-pull */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);    
 	/* Configure UART5 Rx (PD.2) as input floating */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* UART5 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	
	USART_Cmd(UART5, ENABLE);
	USART_ClearFlag(UART5, USART_FLAG_TC);
	NVIC_uart5_Configuration();                           //串口中断配置---抢1，响0 

	/* 相关数据初始化*/
	uart5_timer = Delay_Initial(UART5_TIMER);
	Data.valid = 0;
	Data.f = 0;

}


/*
输出指向缓存的指针
 */
Uart5Buf*  get_buf_uart5_pt(void)
{
	Uart5Buf* i;
	i = &Data;
	return i;
}


/*
UART5 发送数据函数，发送为指针(send_data)到
指针(send_data+send_size-1)指向的一段内容，
长度为send_size
 */
void uart5_send(uint8_t*  send_data , uint16_t  send_size)
{ 
	uint16_t i;  	
	for(i=0;i<send_size;i++){
		USART_SendData(UART5, *send_data);
		send_data++;
	  while (!(UART5->SR & USART_FLAG_TXE));
	}	
}


/*
UART5 中断函数
 */
void UART5_IRQHandler(void)
{
	UART5_Buf_Type c;//临时存放串口收到的单个数据
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{ 	
		do{
			if(Data.f == 1){
				continue;
			}
			Close_Delay(uart5_timer);
			Open_Delay(uart5_timer); //重新打开延时
			
			c=UART5->DR;
			if(Data.valid < UART5_RX_BUF){
				Data.array[Data.valid++] = c;//数据推向缓存	
			}
			else{
				Data.f = 2;
				Data.valid = 0;
			}
		}while(0);
	} 
	USART_ClearITPendingBit(UART5, USART_IT_RXNE);		 
}

/*
将串口数据缓存指针赋给temp_data中保存的指针
返回值  0：缓存中没有完整数据 1：有数  2：数据溢出
 */
uint8_t Get_Buf_Uart5_PT(Uart5Buf*  temp_data)
{
	if(Data.f == 2){
		Data.f = 0;
		return 2;
	}
	if(Delay_Ok(uart5_timer)){
		Data.f = 1;
		*temp_data = Data;
		Data.valid = 0;
		Data.f = 0;
		return 1;
	}
	return 0;		
}
