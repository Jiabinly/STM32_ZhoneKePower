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
//�ض���fputc����
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
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USATR1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //ʹ��USART2
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����жϷ���2
	
  USART_DeInit(USART1);   //��λ����1
//  USART_DeInit(USART2); //��λ����2
  USART_DeInit(USART3);   //��λ����3
  
	
	 //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//USATR��ʼ������
  USART_InitStructure.USART_BaudRate = 115200;               //����1������
  USART_InitStructure.USART_WordLength =USART_WordLength_8b; //8λ�ֳ����ݸ�ʽ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;     //1��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
  USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //��Ӳ������������
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure);      //��ʼ������
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //�����ж�
  USART_Cmd(USART1, ENABLE);  
//  USART_SendString(USART1, "\r\n",0);	           //ʹ�ܴ���
  NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;	 //�����ж�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_InitStructure.USART_BaudRate = 9600;               //����1������
  USART_InitStructure.USART_WordLength =USART_WordLength_8b; //8λ�ֳ����ݸ�ʽ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;     //1��ֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;        //����żУ��λ
  USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None; //��Ӳ������������
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure);      //��ʼ������
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //�����ж�
  USART_Cmd(USART3, ENABLE);  
//  USART_SendString(USART1, "\r\n",0);	           //ʹ�ܴ���
  NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;	 //�����ж�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//���ڷ����ַ��������ܷ���0x00����ַ�����
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



void USART1_IRQHandler(void) //����1�жϷ������
{
	  uint8_t Res;	
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    Res = USART1->DR; //��ȡ���ܵ�������
    if (Res == 0x0D) 
		{ //\r��ȥ
			
    } 
		else if (Res == 0x0A) 
		{ // \n���봦����
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
		else { //�ǽ�β����
			    flag_usart1=1;
          USART1_Data.DATA_RX[USART1_Data.RX_STA++] = Res;
					if (USART1_Data.RX_STA == USART_REC_LEN) 
					{
							USART1_Data.RX_STA = 0;
					}
    }
  }
}

void USART3_IRQHandler(void) //����3�жϷ�����
{
  uint8_t Res;
	uint8_t err;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) 
	{
      USART_ClearITPendingBit(USART3, USART_IT_RXNE);
      Res = USART3->DR;            //��ȡ���ܵ�������

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
//			Res = USART3->DR; //��ȡ���ܵ�������
//			if(flag_usart3==1)
//			{
//					if (Res == 0x0D) 
//					{ //\r��ȥ
//						
//					} 
//					else if (Res == 0x0A) 
//					{ // \n���봦����
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
//					{ //�ǽ�β����
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
//      Res = USART3->DR;            //��ȡ���ܵ�������
//		
//			
//      USART3_Data.DATA_RX[USART3_Data.RX_STA++] = Res;
//		  
//      if (USART3_Data.RX_STA == USART_REC_LEN) 
//			{
//        USART3_Data.RX_STA = 0;
//			}
//			
////			if(flag_usart3==1)   //���ܷ��������ݱ�־
////			{
////				OSIntEnter() ;
////				
////				OSFlagPost(flags_event,ExtIntBit,OS_FLAG_SET,&err);
////				OSIntExit();
////			}
//			
//  }  
	
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
	
}
