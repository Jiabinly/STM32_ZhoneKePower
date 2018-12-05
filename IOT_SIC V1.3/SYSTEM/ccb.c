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
	
//开启GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); 
	
// RS485_TX_EN GPIO模式
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;				   //485_EN-->PA.1端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					    //根据设定参数初始化GPIOA.1
 GPIO_ResetBits(GPIOA,GPIO_Pin_1);                  //RS485 默认接收状态
	
//配置USART2 Tx GPIO模式/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
//配置USART2 Rx GPIO模式/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//配置USART2工作模式	
	USART_InitStructure.USART_BaudRate = 9600;	//波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//模式
  USART_Init(USART2, &USART_InitStructure);
	
//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
//开启USART2中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启串口接收中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);   //开启串口空闲中断
	USART_ClearITPendingBit(USART2, USART_IT_IDLE);	//清除串口空闲中断标志位
	
//使能USART2/,默认为接收状态
	USART_Cmd(USART2, ENABLE);
//GPIO_ResetBits(GPIOA,GPIO_Pin_1); 
}
//USART2发送函数
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

//USART2接收中断函数
void USART2_IRQHandler(void)          
{	
	//u8 i;
	u8 clear=clear;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 
//	USART_ClearITPendingBit(USART2, USART_IT_RXNE); 
//	USART_ClearFlag(USART2,USART_FLAG_RXNE); //清除接收中断标志位
		Uart2RxBuf[Uart2RxDataConut]=USART_ReceiveData(USART2); //读取数据			
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
		clear=USART2->DR; //先读SR后读DR，清除空闲中断标志位 
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

//[0]=起始符号,[1]=包长度,[2]=命令,[3]=单板地址,[N]=校验码

//CCB地址查询命令
u8 Cmd_Addr_Scan[5]={0x1F,0x05,0x01,0x00,0x00};  

//CCB类型查询命
u8 Cmd_Type_Query[5]={0x1F,0x05,0x02,0x00,0x00}; 

//CCB通道开启命令,[4]=开启通道号
u8 Cmd_Channel_On[6]={0x1F,0x06,0x03,0x00,0x01,0x00}; 

//CCB通道电流查询命令,[4]=查询电流通道号,[5]=充电电流值整数，[6]=充电电流值小数，单位A
u8 Cmd_ChCurrent_Query[8]={0x1F,0x08,0x04,0x00,0x01,0x00,0x00,0x00}; 

//CCB通道关闭命令,[4]=关闭通道号
u8 Cmd_Channel_Off[6]={0x1F,0x06,0x05,0x00,0x01,0x00}; 

//CCB通道异常查询命令
u8 Cmd_Error_Query[5]={0x1F,0x05,0x06,0x00,0x00}; 

//CCB01通信RS485接口初始化函数
void CCB_Init(void)
{
	USART2_Init();    //初始化串口2
	CCB_Scan(); //遍历所有在线CCB单板
	CCB_Type();//查询在线CCB单板类型
}


//遍历CCB板在线函数
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
				if(Uart2RxBuf[2]==Cmd_Addr_Scan[2]) //返回命令确认
				{	
					if(Uart2RxBuf[3]==Cmd_Addr_Scan[3]) //返回地址确认
					{
						printf("CCB of address %02x is on line!\r\n",i);
						//将在线单板地址存储在指定位置
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
//查询在线单板类型函数
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
			if(RxCheckSum(Uart2RxBuf)) //接收校验
			{
				if(Uart2RxBuf[2]==Cmd_Type_Query[2]) //返回命令确认
				{	
					if(Uart2RxBuf[3]==Cmd_Type_Query[3]) //单板地址确认
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
//CCB充电通道开启函数
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
			if(RxCheckSum(Uart2RxBuf)) //接收校验
			{
				if(Uart2RxBuf[2]==Cmd_Channel_On[2]) //返回命令确认
				{	
					if(Uart2RxBuf[3]==Cmd_Channel_On[3]) //单板地址确认
					{
						if (Uart2RxBuf[5]==0x00) //开启成功
						{
//							printf("The SIC0xx Channel %d open success\r\n",channel);
							if (channel>8)
							{
								NVC_SendData((channel+1)/10+5);
							}
							NVC_SendData((channel+1)%10+5);
							NVC_SendData(4);
						}
						else if (Uart2RxBuf[5]==0x01) //开启失败
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
//CCB充电通道电流查询函数
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
			if(RxCheckSum(Uart2RxBuf)) //接收校验
			{
				if(Uart2RxBuf[2]==Cmd_ChCurrent_Query[2]) //返回命令确认
				{	
					if(Uart2RxBuf[3]==Cmd_ChCurrent_Query[3]) //单板地址确认
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
//CCB充电通道关闭函数
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
			if(RxCheckSum(Uart2RxBuf)) //接收校验
			{
				if(Uart2RxBuf[2]==Cmd_Channel_Off[2]) //返回命令确认
				{	
					if(Uart2RxBuf[3]==Cmd_Channel_Off[3]) //单板地址确认
					{
						if (Uart2RxBuf[5]==0x00) //关闭成功
						{
							printf("The SIC0xx Channel %d close success\r\n",channel);
							if (channel>8)
							{
								NVC_SendData((channel+1)/10+5);
							}
							NVC_SendData((channel+1)%10+5);
							NVC_SendData(3);
						}
						else if (Uart2RxBuf[5]==0x01) //关闭失败
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
//CCB异常查询函数
void CCB_ErrQuery(void)
{
	;
}
