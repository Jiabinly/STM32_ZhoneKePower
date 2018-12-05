#include "stm32f10x.h"
#include "ccb.h"
#include "nvc.h"
#include "checksum.h"

u8 SIC0xx_TYPE;
u8 Uart2RxBuf[30];
u8 Uart2RxDataConut;
u8 UART2_RX_STATE;

void USART2_Init( void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
//����GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
	
// RS485_TX_EN GPIOģʽ
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;				   //485_EN-->PA.1�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					    //�����趨������ʼ��GPIOA.1
 GPIO_ResetBits(GPIOA,GPIO_Pin_1);                  //RS485 Ĭ�Ͻ���״̬
	
//����USART2 Tx GPIOģʽ/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
//����USART2 Rx GPIOģʽ/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//����USART2����ģʽ	
	USART_InitStructure.USART_BaudRate = 9600;	//������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//����������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//ģʽ
  USART_Init(USART2, &USART_InitStructure);
	
//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
//����USART2�ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //�������ڽ����ж�
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);   //�������ڿ����ж�
	USART_ClearITPendingBit(USART2, USART_IT_IDLE);	//������ڿ����жϱ�־λ
	
//ʹ��USART2/,Ĭ��Ϊ����״̬
	USART_Cmd(USART2, ENABLE);
//GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
}
//USART2���ͺ���
void USART2_Send_Data(unsigned char *buf)
{
	int i;
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	for(i=0;i<buf[1];i++)
	{				
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	
		USART_SendData(USART2, buf[i]);			
	}	
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
}

//USART2�����жϺ���
void USART2_IRQHandler(void)          
{	
	//u8 i;
	u8 clear=clear;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 
//	USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
//	USART_ClearFlag(USART2,USART_FLAG_RXNE); //��������жϱ�־λ
		Uart2RxBuf[Uart2RxDataConut]=USART_ReceiveData(USART2); //��ȡ����			
		Uart2RxDataConut++;	
	}
	if(Uart2RxDataConut >= UART2_RX_BUF_LEN)
	{
		Uart2RxDataConut = 0;
	}
	if(USART_GetITStatus(USART2, USART_IT_IDLE)!=RESET)
	{	
		//USART_ClearITPendingBit(USART2, USART_IT_IDLE); 
		clear=USART2->SR; 
		clear=USART2->DR; //�ȶ�SR���DR����������жϱ�־λ 
//		printf("USART2_ReceiveData:\r\n");
//		for (i=0;i<Uart2RxDataConut;i++)
//		{
//			printf("0x%02X ",Uart2RxBuf[i]);
//		}
//		printf("\r\n");
		Uart2RxDataConut = 0;
		UART2_RX_STATE=1;
	}
}

//[0]=��ʼ����,[1]=������,[2]=����,[3]=�����ַ,[N]=У����

//CCB��ַ��ѯ����
u8 Cmd_Addr_Scan[5]={0x1F,0x05,0x01,0x00,0x00};  

//CCB���Ͳ�ѯ��
u8 Cmd_Type_Query[5]={0x1F,0x05,0x02,0x00,0x00}; 

//CCBͨ����������,[4]=����ͨ����
u8 Cmd_Channel_On[6]={0x1F,0x06,0x03,0x00,0x01,0x00}; 

//CCBͨ��������ѯ����,[4]=��ѯ����ͨ����,[5]=������ֵ������[6]=������ֵС������λA
u8 Cmd_ChCurrent_Query[8]={0x1F,0x08,0x04,0x00,0x01,0x00,0x00,0x00}; 

//CCBͨ���ر�����,[4]=�ر�ͨ����
u8 Cmd_Channel_Off[6]={0x1F,0x06,0x05,0x00,0x01,0x00}; 

//CCBͨ���쳣��ѯ����
u8 Cmd_Error_Query[5]={0x1F,0x05,0x06,0x00,0x00}; 

//CCB01ͨ��RS485�ӿڳ�ʼ������
void CCB_Init(void)
{
	USART2_Init();    //��ʼ������2
	CCB_Scan(); //������������CCB����
	CCB_Type();//��ѯ����CCB��������
}


//����CCB�����ߺ���
void CCB_Scan(void)
{
	u8 i ;
	for (i=0; i<SIC0xx;i++)
	{ 
		Cmd_Addr_Scan[3]=i;
		TxCheckSum(Cmd_Addr_Scan);		
		USART2_Send_Data(Cmd_Addr_Scan);
		delay_ms(10);
		//printf("CCB %d is scaning!\r\n",i);
		if(UART2_RX_STATE==1) 
		{
			UART2_RX_STATE=0;
			if(RxCheckSum(Uart2RxBuf))
			{
				if(Uart2RxBuf[2]==Cmd_Addr_Scan[2]) //��������ȷ��
				{	
					if(Uart2RxBuf[3]==Cmd_Addr_Scan[3]) //���ص�ַȷ��
					{
						printf("CCB of address %02x is on line!\r\n",i);
						//�����ߵ����ַ�洢��ָ��λ��
					}
				}
			}
		}	else
		{
			printf("CCB of address %02x doesn't respond!\r\n",i);
		}
		//printf("CCB %d scaned!\r\n",i);
	}
}
//��ѯ���ߵ������ͺ���
void CCB_Type(void)
{
	u8 i;
	for (i=0;i<SIC0xx;i++)
	{ 
		Cmd_Type_Query[3]=i;
		TxCheckSum(Cmd_Type_Query);		
		USART2_Send_Data(Cmd_Type_Query);
		delay_ms(100);
		if(UART2_RX_STATE==1) 
		{
			UART2_RX_STATE=0;
			if(RxCheckSum(Uart2RxBuf)) //����У��
			{
				if(Uart2RxBuf[2]==Cmd_Type_Query[2]) //��������ȷ��
				{	
					if(Uart2RxBuf[3]==Cmd_Type_Query[3]) //�����ַȷ��
					{
						printf("The hardware version of CCB %02x is %x\r\n",i,Uart2RxBuf[4]);
					}
				}
			}
		}
		else
		{
			printf("CCB of address %02x is not on line!\r\n",i);
		}
	}
}
//CCB���ͨ����������
void CCB_ChnOn(u8 channel)
{
	Cmd_Channel_On[3]=channel/6;
	Cmd_Channel_On[4]=channel%6+1;
	TxCheckSum(Cmd_Channel_On);
	USART2_Send_Data(Cmd_Channel_On);
	delay_ms(100);
	if(UART2_RX_STATE==1) 
		{
			UART2_RX_STATE=0;
			if(RxCheckSum(Uart2RxBuf)) //����У��
			{
				if(Uart2RxBuf[2]==Cmd_Channel_On[2]) //��������ȷ��
				{	
					if(Uart2RxBuf[3]==Cmd_Channel_On[3]) //�����ַȷ��
					{
						if (Uart2RxBuf[5]==0x00) //�����ɹ�
						{
//							printf("The SIC0xx Channel %d open success\r\n",channel);
							if (channel>8)
							{
								NVC_SendData((channel+1)/10+5);
							}
							NVC_SendData((channel+1)%10+5);
							NVC_SendData(4);
						}
						else if (Uart2RxBuf[5]==0x01) //����ʧ��
						{
//							printf("The SIC0xx Channel %d open failed\r\n",channel);
						}					
					}
				}
			}
		}
		else
		{
//			printf("The SIC0xx Channel %d has no response\r\n",channel);
		}
}
//CCB���ͨ��������ѯ����
void CCB_ChnCurrent(u8 channel)
{ 
	int i = 0;
	Cmd_ChCurrent_Query[3]=channel/6;
	Cmd_ChCurrent_Query[4]=channel%6+1;
	TxCheckSum(Cmd_ChCurrent_Query);
	USART2_Send_Data(Cmd_ChCurrent_Query);
	printf("ChCurrent Command is");
	for (i=0;i<8;i++)
	{ 
		printf(" %02X",Cmd_ChCurrent_Query[i]);	
	}
	printf("\r\n");
	delay_ms(100);
	if(UART2_RX_STATE==1) 
		{
			UART2_RX_STATE=0;
			if(RxCheckSum(Uart2RxBuf)) //����У��
			{
				if(Uart2RxBuf[2]==Cmd_ChCurrent_Query[2]) //��������ȷ��
				{	
					if(Uart2RxBuf[3]==Cmd_ChCurrent_Query[3]) //�����ַȷ��
					{
						printf("ChCurrent Respond is");
						for (i=0;i<8;i++)
						{ 
							printf(" %02X",Uart2RxBuf[i]);	
						}
						printf("\r\n");
						
						printf("The SIC0xx Channel %d Current is %d.%d%d%d A\r\n",channel,Uart2RxBuf[5]/10,Uart2RxBuf[5]%10,Uart2RxBuf[6]/10,Uart2RxBuf[6]%10);	
					}
				}
			}
		}
		else
		{
			printf("The SIC0xx Channel %d has no response\r\n",channel);
		}	
}
//CCB���ͨ���رպ���
void CCB_ChnOff(u8 channel)
{
	Cmd_Channel_Off[3]=channel/6;
	Cmd_Channel_Off[4]=channel%6+1;
	TxCheckSum(Cmd_Channel_Off);
	USART2_Send_Data(Cmd_Channel_Off);
	delay_ms(100);
	if(UART2_RX_STATE==1) 
		{
			UART2_RX_STATE=0;
			if(RxCheckSum(Uart2RxBuf)) //����У��
			{
				if(Uart2RxBuf[2]==Cmd_Channel_Off[2]) //��������ȷ��
				{	
					if(Uart2RxBuf[3]==Cmd_Channel_Off[3]) //�����ַȷ��
					{
						if (Uart2RxBuf[5]==0x00) //�رճɹ�
						{
							printf("The SIC0xx Channel %d close success\r\n",channel);
							if (channel>8)
							{
								NVC_SendData((channel+1)/10+5);
							}
							NVC_SendData((channel+1)%10+5);
							NVC_SendData(3);
						}
						else if (Uart2RxBuf[5]==0x01) //�ر�ʧ��
						{
							printf("The SIC0xx Channel %d close failed\r\n",channel);
						}					
					}
				}
			}
		}
		else
		{
			printf("The SIC0xx Channel %d has no response\r\n",channel);
		}
}
//CCB�쳣��ѯ����
void CCB_ErrQuery(void)
{
	;
}
