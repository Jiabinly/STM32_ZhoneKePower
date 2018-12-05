#include "usart1.h"


#if SYSTEM_SUPPORT_OS
#include "includes.h"					 
#endif
#pragma import(__use_no_semihosting)
struct __FILE {
  int handle;
};
uint8_t flag_usart1,flag_usart3=0;	
extern OS_FLAG_GRP * flags_event;	
FILE __stdout;
_sys_exit(int x) { x = x; }
//重定义fputc函数
int fputc(int ch, FILE *f) {
  while ((USART1->SR & 0X40) == 0)
    ;
  USART1->DR = (u8)ch;
  return ch;
}

USART_DataType USART1_Data;
//USART_DataType USART2_Data;
USART_DataType USART3_Data;


void USART_Configuration(void) {
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USATR1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART2
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //设置中断分组2
	
  USART_DeInit(USART1);   //复位串口1
//  USART_DeInit(USART2); //复位串口2
  USART_DeInit(USART3);   //复位串口3
  
	
	 //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//USATR初始化设置
  USART_InitStructure.USART_BaudRate = 115200;               //串口1波特率
  USART_InitStructure.USART_WordLength =USART_WordLength_8b; //8位字长数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;     //1个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //收发模式

  USART_Init(USART1, &USART_InitStructure);      //初始化串口
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启中断
  USART_Cmd(USART1, ENABLE);  
//  USART_SendString(USART1, "\r\n",0);	           //使能串口
  NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;	 //串口中断
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	USART_InitStructure.USART_BaudRate = 9600;               //串口1波特率
  USART_InitStructure.USART_WordLength =USART_WordLength_8b; //8位字长数据格式
  USART_InitStructure.USART_StopBits = USART_StopBits_1;     //1个停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;        //无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //无硬件数据流控制
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //收发模式

  USART_Init(USART3, &USART_InitStructure);      //初始化串口
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启中断
  USART_Cmd(USART3, ENABLE);  
//  USART_SendString(USART1, "\r\n",0);	           //使能串口
  NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;	 //串口中断
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//串口发送字符串，可能发送0x00需带字符长度
void USART_SendString(USART_TypeDef *USARTx, uint8_t *Data, uint16_t Long) {
  if (Long > 0) {
    while (Long--) {
      while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        ;
      USART_SendData(USARTx, *Data++);
    }
  } else {
    while (*Data) {
      while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        ;
      USART_SendData(USARTx, *Data++);
    }
  }
}


void USART_ClearBuf(USART_DataType *USART_Data) {
  memset(USART_Data->DATA_RX, 0,USART_REC_LEN);
  USART_Data->RX_STA = 0;
}



uint8_t USART_CheckRec(uint8_t *ptr) {
  if (strstr((char *)USART3_Data.DATA_RX, (char *)ptr) == NULL)
    return 0;
  else
    return 1;
}


//uint8_t USART2_CheckRec(uint8_t *ptr) {
//  if (strstr((char *)USART2_Data.DATA_RX, (char *)ptr) == NULL)
//    return 0;
//  else
//    return 1;
//}



void USART1_IRQHandler(void) //串口1中断服务程序
{
	  uint8_t Res;	
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    Res = USART1->DR; //读取接受到的数据
    if (Res == 0x0D) 
		{ //\r舍去
			
    } 
		else if (Res == 0x0A) 
		{ // \n进入处理函数
      if (USART1_Data.RX_STA > 5) 
			{
        USART1_Data.DATA_RX[USART1_Data.RX_STA++] = 0;
				printf("**************************************************\r\n");
//        PROTOCOL_InforProcess(USART1_Data.DATA_RX, 0, 0);
				printf("**************************************************\r\n");
        USART1_Data.RX_STA = 0;
      } 
			else 
			{
        USART1_Data.RX_STA = 0;
      }
    } 
		else { //非结尾保存
			    flag_usart1=1;
          USART1_Data.DATA_RX[USART1_Data.RX_STA++] = Res;
					if (USART1_Data.RX_STA == USART_REC_LEN) 
					{
							USART1_Data.RX_STA = 0;
					}
    }
  }
}

void USART3_IRQHandler(void) //串口3中断服务函数
{
  uint8_t Res;
	uint8_t err;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
      USART_ClearITPendingBit(USART3, USART_IT_RXNE);
      Res = USART3->DR;            //读取接受到的数据

		  if(flag_usart3==1)
			{
					if (USART3_Data.RX_STA == USART_REC_LEN) 
					{
						USART3_Data.RX_STA = 0;
					}
					if( Res == '\n')
					{
//							printf("USART3_Data.RX_STA = %d\r\n",USART3_Data.RX_STA);
//							printf("USART3_Data.DATA_RX=%s\r\n",USART3_Data.DATA_RX);
							if((USART3_Data.RX_STA >40))/*&&(((memcmp("+PUBLISHDEFAULT", USART3_Data.DATA_RX, 10)== 0))
																						||(memcmp("+DPUBLISH", USART3_Data.DATA_RX, 10)== 0)))*/
							{
								printf("+++++++USART3_Data.DATA_RX=%s\r\n",USART3_Data.DATA_RX);
//								OSFlagPost(flags_event,ExtIntBit,OS_FLAG_SET,&err);
									Cut_Server_Payload(USART3_Data.DATA_RX);
							}
							
							USART3_Data.RX_STA = 0;
					}
					else
					{
						
						USART3_Data.DATA_RX[USART3_Data.RX_STA++] = Res;
					}
			}
			else
			{
					USART3_Data.DATA_RX[USART3_Data.RX_STA++] = Res;
					
					if (USART3_Data.RX_STA == USART_REC_LEN) 
					{
						USART3_Data.RX_STA = 0;
					}
			}
      	
  }

	
//	 if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
//		{
//			USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//			Res = USART3->DR; //读取接受到的数据
//			if(flag_usart3==1)
//			{
//					if (Res == 0x0D) 
//					{ //\r舍去
//						
//					} 
//					else if (Res == 0x0A) 
//					{ // \n进入处理函数
//						if (USART3_Data.RX_STA > 5) 
//						{
//							USART3_Data.DATA_RX[USART3_Data.RX_STA++] = 0;
//							USART3_Data.RX_STA = 0;
//						} 
//						else 
//						{
//							USART3_Data.RX_STA = 0;
//						}
//					} 
//					else 
//					{ //非结尾保存
//			//			    flag_usart1=1;
//								USART3_Data.DATA_RX[USART3_Data.RX_STA++] = Res;
//								if (USART3_Data.RX_STA == USART_REC_LEN) 
//								{
//										USART3_Data.RX_STA = 0;
//								}
//					}
//			}
//		}
//		else
//		{
//				USART3_Data.DATA_RX[USART3_Data.RX_STA++] = Res;
//				
//				if (USART3_Data.RX_STA == USART_REC_LEN) 
//				{
//					USART3_Data.RX_STA = 0;
//				}
//		}

//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
//	{
//      USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//      Res = USART3->DR;            //读取接受到的数据
//		
//			
//      USART3_Data.DATA_RX[USART3_Data.RX_STA++] = Res;
//		  
//      if (USART3_Data.RX_STA == USART_REC_LEN) 
//			{
//        USART3_Data.RX_STA = 0;
//			}
//			
////			if(flag_usart3==1)   //接受服务器数据标志
////			{
////				OSIntEnter() ;
////				
////				OSFlagPost(flags_event,ExtIntBit,OS_FLAG_SET,&err);
////				OSIntExit();
////			}
//			
//  }  
	
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
	
}
