/************************************************************************************
 * �ļ���  ��usart1.c
 * ����    ������stm32�ײ㴮��1�Ľ��պ��������ж��н����յ����������Զ���Ļ���Data�У�
				 �ϲ㺯��ͨ��������Get_Buf_Usart1_PT��ȡData���ݡ���ȡ������
					�ٵ���usart1_config���д��ڳ�ʼ��
					�ڶ���Usart1Buf���͵Ľṹ�壬�������Data���������
					�۵���Get_Buf_Usart1_PT�������жϴ��ڻ���ṹ��Data���Ƿ���յ�������
						�������ݣ�������յ���������Data�е����ݸ��Ƶ��ڶ��������Usart1Buf
						���͵Ľṹ�塣
				����
					Usart1Buf  buf;
					
					usart1_config();
					if(Get_Buf_Usart1_PT(buf) = 1){
						...//���ڻ����е������Ѿ����Ƶ� buf �У����Խ������ݴ���
					}
					
					
 * Ӳ�����ӣ�------------------------
 *          | PA9  - USART1(Tx)      |
 *          | PA10 - USART1(Rx)      |
 *           ------------------------
 * ��  ��  ��2014-7-29
 * ��  ��  �������
**********************************************************************************/	
#include "usart1.h"

static Usart1Buf  Data;
static volatile TimerDelay* usart1_timer;

/*
USART1 �ж�����
 */
void NVIC_usart1_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);        //�ж�����	
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


/*
USART1 GPIO ����,����ģʽ����
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
 	GPIO_PinRemapConfig(GPIO_Remap_USART1 ,ENABLE);//����
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
	NVIC_usart1_Configuration();                           //�����ж�����---��1����0 
	
/* ������ݳ�ʼ��*/
	usart1_timer = Delay_Initial(USART1_TIMER);
	Data.valid = 0;
	Data.f = 0;
}


/*
���ָ�򻺴��ָ��
 */
Usart1Buf*  get_buf_usart1_pt(void)
{
	Usart1Buf* i;
	i = &Data;
	return i;
}


/*
USART1 �������ݺ���������Ϊָ��(send_data)��
ָ��(send_data+send_size-1)ָ���һ�����ݣ�
����Ϊsend_size
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
	USART1_Buf_Type c;//��ʱ��Ŵ����յ��ĵ�������
	if(USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		do{
			if(Data.f == 1){
				continue;
			}
			Close_Delay(usart1_timer);
			Open_Delay(usart1_timer); //���´���ʱ
			
			c=USART1->DR;
			if(Data.valid < USART1_RX_BUF){
				Data.array[Data.valid++] = c;//�������򻺴�	
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
���������ݻ���ָ�븳��temp_data�б����ָ��
����ֵ  0��������û���������� 1������  2���������
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

