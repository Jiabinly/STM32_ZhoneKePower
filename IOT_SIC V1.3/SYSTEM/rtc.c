
#include "delay.h"
#include "usart.h"
#include "rtc.h" 	
#include "gpio.h"
#include "stm32f10x_bkp.h"
#include "system.h"
#include "timer.h"
#include "Lsm303Agr.h" 
 extern  PROTOCOL_TypeDef PROTOCOL_Structure;
extern uint16_t Ttimes;
extern	uint8_t   temout; 
extern u32 UltrTestCount;
extern OS_FLAG_GRP * flags_event;	
extern USART_DataType USART3_Data;
//extern NBCAL_TypeDef NB_Calendar;
static void RTC_NVIC_Config(void)
{	
     NVIC_InitTypeDef NVIC_InitStructure;
     EXTI_InitTypeDef EXTI_InitStructure;


    EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	
NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;  //
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //RTC
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
 NVIC_Init(&NVIC_InitStructure);
}


//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������

 u8 RTC_Init(uint32_t time_h)
{
	
	//����ǲ��ǵ�һ������ʱ��
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
//	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
//		{	 			

		BKP_DeInit();	//��λ�������� 	
//		RCC_LSEConfig(RCC_LSE_ON);	//�����ⲿ���پ���(LSE),ʹ��������پ���
				RCC_LSICmd(ENABLE);
//		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//���ָ����RCC��־λ�������,�ȴ����پ������
			while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{
			temp++;
			delay_ms(10);
//				printf("temp is %d\r\n",temp);
				
				
			}
		if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������	    
//		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��  
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);				
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��  
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������	
		RTC_SetPrescaler(39999);//1HZ
			
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_SetCounter(0);
//		RTC_Set(NB_Calendar.w_year,NB_Calendar.w_month,NB_Calendar.w_date,NB_Calendar.hour,NB_Calendar.min,NB_Calendar.sec);  //����ʱ��
		RTC_WaitForLastTask();
		RTC_SetAlarm(time_h);
		RTC_WaitForLastTask();//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_ExitConfigMode(); //�˳�����ģʽ  
//					BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//��ָ���ĺ󱸼Ĵ�����д���û���������
//			RTC_WaitForLastTask();

//		}
//	else//ϵͳ������ʱ
//		{

//		RTC_WaitForSynchro();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
//		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//ʹ��RTC���ж�
//		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
//		}

		  RTC_WaitForLastTask();
	    RTC_NVIC_Config();//RCT�жϷ�������		    				     
	    return 0; //ok

}		 


 void SYSCLKConfig_STOP(void)
{  
  /* After wake-up from STOP reconfigure the system clock */
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* Wait till HSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {}
  
  /* Enable PLL */
  RCC_PLLCmd(ENABLE);
  
  /* Wait till PLL is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {}
  
  /* Select PLL as system clock source */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* Wait till PLL is used as system clock source */
  while (RCC_GetSYSCLKSource() != 0x08)
  {}
}


void RTCAlarm_IRQHandler(void)
{
		u8 err=0;
  	u32 count;
		EXTI_ClearITPendingBit(EXTI_Line17);
  	SET_BIT(SysTick->CTRL,SysTick_CTRL_ENABLE_Msk);
	  OSIntEnter() ;
		OSFlagPost(flags_event,RtcDectectBit,OS_FLAG_SET,&err);
	  OSIntExit();
	  count = RTC_GetCounter();
	  RTC_SetAlarm(count+10);
    UltrTestCount++;
	  HeartBeatCount++; //������   360ΪһСʱ   360��24 = 8640��	
  	timecount++;
			// 	printf("RTC is%d\r\n",timecount);
}

//RTCʱ���ж�
//ÿ�봥��һ��  

void RTC_IRQHandler(void)
{		 
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	  
       
  	} 				
	RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�
	RTC_WaitForLastTask();	 		
}




