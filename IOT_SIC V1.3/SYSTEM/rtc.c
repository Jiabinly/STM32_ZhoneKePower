
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
 
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;  //
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //RTC
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
 NVIC_Init(&NVIC_InitStructure);
}


//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常
//其他:错误代码

 u8 RTC_Init(uint32_t time_h)
{
	
	//检查是不是第一次配置时钟
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
//	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
//		{	 			

		BKP_DeInit();	//复位备份区域 	
//		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
				RCC_LSICmd(ENABLE);
//		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
			while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{
			temp++;
			delay_ms(10);
//				printf("temp is %d\r\n",temp);
				
				
			}
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题	    
//		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟  
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);				
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();		//等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_ALR, ENABLE);
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(39999);//1HZ
			
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_SetCounter(0);
//		RTC_Set(NB_Calendar.w_year,NB_Calendar.w_month,NB_Calendar.w_date,NB_Calendar.hour,NB_Calendar.min,NB_Calendar.sec);  //设置时间
		RTC_WaitForLastTask();
		RTC_SetAlarm(time_h);
		RTC_WaitForLastTask();//等待最近一次对RTC寄存器的写操作完成
		RTC_ExitConfigMode(); //退出配置模式  
//					BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//向指定的后备寄存器中写入用户程序数据
//			RTC_WaitForLastTask();

//		}
//	else//系统继续计时
//		{

//		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成
//		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
//		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
//		}

		  RTC_WaitForLastTask();
	    RTC_NVIC_Config();//RCT中断分组设置		    				     
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
	  HeartBeatCount++; //心跳包   360为一小时   360×24 = 8640；	
  	timecount++;
			// 	printf("RTC is%d\r\n",timecount);
}

//RTC时钟中断
//每秒触发一次  

void RTC_IRQHandler(void)
{		 
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  
       
  	} 				
	RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断
	RTC_WaitForLastTask();	 		
}




