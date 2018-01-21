/************************************************************************************
 * 文件名  ：usart1.c
 * 描述    ：调用stm32底层串口1的接收函数，在中断中将接收的数据推入自定义的缓存Data中，
				 上层函数通过，调用Get_Buf_Usart1_PT获取Data数据。获取方法：
					①调用usart1_config进行串口初始化
					②定义Usart1Buf类型的结构体，用来存放Data里面的数据
					③调用Get_Buf_Usart1_PT函数，判断串口缓存结构体Data中是否接收到完整的
						串口数据，如果接收到，函数将Data中的数据复制到第二步定义的Usart1Buf
						类型的结构体。
				例：
					Usart1Buf  buf;
					
					usart1_config();
					if(Get_Buf_Usart1_PT(buf) = 1){
						...//串口缓存中的数据已经复制到 buf 中，可以进行数据处理
					}
					
					
 * 硬件连接：------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * 日  期  ：2014-7-29
 * 作  者  ：李彦辉
**********************************************************************************/	
#include "usart1.h"

static Usart1Buf  Data;
static volatile TimerDelay* usart1_timer;

/*
USART1 中断配置
 */
void NVIC_usart1_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);        //中断配置	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


/*
USART1 GPIO 配置,工作模式配置
 */
void usart1_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	

#if(USART1_MAP)	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

#else
 	/* config USART1 clock */
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
 	GPIO_PinRemapConfig(GPIO_Remap_USART1 ,ENABLE);//复用
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
 	/* USART1 GPIO config */
 	/* Configure USART1 Tx  (PB6)as alternate function push-pull */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);    
 	/* Configure USART1 Rx (PB7) as input floating */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif //USART1_MAP


/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	NVIC_usart1_Configuration();                           //串口中断配置---抢1，响0 
	
/* 相关数据初始化*/
	usart1_timer = Delay_Initial(USART1_TIMER);
	Data.valid = 0;
	Data.f = 0;
}


/*
输出指向缓存的指针
 */
Usart1Buf*  get_buf_usart1_pt(void)
{
	Usart1Buf* i;
	i = &Data;
	return i;
}


/*
USART1 发送数据函数，发送为指针(send_data)到
指针(send_data+send_size-1)指向的一段内容，
长度为send_size
 */
void usart1_send(uint8_t*  send_data , uint16_t  send_size)
{ 
	uint16_t i;  	
	for(i=0;i<send_size;i++){
		USART_SendData(USART1, *send_data);		
		send_data++;
	  while (!(USART1->SR & USART_FLAG_TXE));
	}	
}



void USART1_IRQHandler(void)
{
	USART1_Buf_Type c;//临时存放串口收到的单个数据
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		do{
			if(Data.f == 1){
				continue;
			}
			Close_Delay(usart1_timer);
			Open_Delay(usart1_timer); //重新打开延时
			
			c=USART1->DR;
			if(Data.valid < USART1_RX_BUF){
				Data.array[Data.valid++] = c;//数据推向缓存	
			}
			else{
				Data.f = 2;
				Data.valid = 0;
			}
		}while(0);
	} 
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);		 
}

/*
将串口数据缓存指针赋给temp_data中保存的指针
返回值  0：缓存中没有完整数据 1：有数  2：数据溢出
 */
uint8_t Get_Buf_Usart1_PT(Usart1Buf*  temp_data)
{
	if(Data.f == 2){
		Data.f = 0;
		return 2;
	}
	if(Delay_Ok(usart1_timer)){
		Data.f = 1;
		*temp_data = Data;
		Data.valid = 0;
		Data.f = 0;
		return 1;
	}
	return 0;		
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

