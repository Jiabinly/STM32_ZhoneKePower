#include "main.h"


OS_CPU_SR   cpu_sr=0;
OS_FLAG_GRP * flags_event;	
extern uint8_t flag_usart3 ;
uint8_t RecvBuf[512]= {0};


/*设置任务优先级*/
#define MainManager_Pri     10  //主任务的优先级最低
#define Charging_Pri        1   //充电处理任务优先级 
#define ModTXD_Pri          2   //发送数据任务优先级
#define DealUsart_Pri       3   //终端处理任务优先级
#define InteractCon_Pri     4   //交互处理任务优先级

/*设置设置任务堆栈大小*/
#define MainManager_Size  2048   //设置堆栈大小
#define Charging_Size     2048   //设置堆栈大小
#define ModTXD_Size       2048   //设置堆栈大小
#define DealUsart_Size    2048   //设置堆栈大小
#define InteractCon_Size  2048   //设置堆栈大小

/*任务堆栈*/
OS_STK  MainManager_Stack[MainManager_Size];
OS_STK  Charging_Stack[Charging_Size];
OS_STK  ModTXD_Stack[ModTXD_Size];
OS_STK  DealUsart_Stack[DealUsart_Size];
OS_STK  InteractCon_Stack[InteractCon_Size];

/*任务函数*/
void MainManager_Task(void *p);    //主任务
void Charging_Task(void *p);       //与充电板交互任务
void ModTXD_Task(void *p);         //发送接收数据任务
void DealUsart_Task(void *p);      //终端处理任务
void InteractCon_Task(void *p);    //人机交互处理任务



int main(void)
 { 
//	 uint8_t SendDataBuf[1024]={0};
	 int i = 0;
	 delay_init();	    				//延时函数初始化
	 GPIO_Configuration();   //串口1、3配置
	 USART_Configuration();   //初始化串口1、3；
	 
//	  printf("************\r\n");
//		Gprs_testComms();
//	  printf("************\r\n");
//	 while(1)
//	 {
//		 Gprs_testComms();
//		 delay_ms(500);
//	 }
	  
	 MQTT_Handler();
	 MQTT_SendReceiveData( CLOUDCONN," "," ","");
	 MQTT_SendReceiveData( CLOUDSUB,"Lk2upKmgG0w","500000000051","");
	 MQTT_SendReceiveData( CLOUDPUB,"Lk2upKmgG0w","500000000051","123456789");
//	 Gprs_GMR();
	
	 //Lk2upKmgG0w,500000000051,cr5ha69Fzurz3F2bdBR7ulf3ti4y1xcs
//	 while(1)
//	 {
//		  if(MD_PowerReset()==1)
//			{
//				MQTT_SendReceiveData( CLOUDCONN," "," ","");
//				 MQTT_SendReceiveData( CLOUDSUB,"Lk2upKmgG0w","500000000051","");
//				 MQTT_SendReceiveData( CLOUDPUB,"Lk2upKmgG0w","500000000051","123456789");
//				 Gprs_GMR();
//			}
//			
//			printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\r\n");
//			
//			for(i=1;i<100;i++)
//			{
//				printf("i=%d\r\n",i);
//				delay_ms(1000);
//			}
//			
//	 }
 
//	 	EXTIX_Init();        			//初始化外部中断，响应按键中断
//	  printf("EXTI init end!\r\n");

//		BEEP_Init();      				//初始化蜂鸣器
		printf("Beep init end!\r\n");
		USART2_Init();	        	//初始化串口2
	  printf("RS485 init end!\r\n");
	  NVC_Init(); 						//初始化语音芯片
		printf("Speaker init end!\r\n");
		printf("-----------------MCB board init end !----------------------\r\n");
		CCB_Init(); //CCB初始化
		printf("-----------------CCB board init end !----------------------\r\n");	
		BEEP_Onoff();         	//蜂鸣器响一声,提示系统开始运行
	 
		flag_usart3 = 1;  //串口三进入操作系统中断标志位
		
		OSInit(); 
		OSTaskCreate(MainManager_Task,(void*)0,
		(OS_STK*)&MainManager_Stack[MainManager_Size-1],MainManager_Pri);
		OSStart();
}

void MainManager_Task(void*p)
{
	u8 err;
	for(;;)
		{ 
			flags_event = OSFlagCreate(0,&err); 
			OS_ENTER_CRITICAL();  
			OSTaskCreate(Charging_Task,(void*)0,(OS_STK*)&Charging_Stack[Charging_Size-1],Charging_Pri);
			OSTaskCreate(ModTXD_Task,(void*)0,(OS_STK*)&ModTXD_Stack[ModTXD_Size-1],ModTXD_Pri);
			OSTaskCreate(DealUsart_Task,(void*)0,(OS_STK*)&DealUsart_Stack[DealUsart_Size-1],DealUsart_Pri);
			OSTaskCreate(InteractCon_Task,(void*)0,(OS_STK*)&InteractCon_Stack[InteractCon_Size-1],InteractCon_Pri);
			OSTaskSuspend(MainManager_Pri);
			OS_EXIT_CRITICAL(); 
	  }
}

//与充电板交互任务
void Charging_Task(void *p)
{
	int i = 6;
	u8 err;
	for(;;)
	{
		
//		CCB_ChnOn(i);
//		delay_ms(1000);
//		CCB_ChnOff(i++);
//		delay_ms(1000);
//		if(i>12)
//		{
//			i = 6;
//		}
		
//		OSFlagPend(flags_event,DataProFinish ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);
//		printf("+++++++++++++++++\r\n");
		
		
	}
}


//发送接收数据任务
void ModTXD_Task(void *p)
{
	u8 err;
	
	for(;;)
	{
		OSFlagPend(flags_event,ExtIntBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);  //接受到平台发送数据
		Cut_Server_Payload(USART3_Data.DATA_RX);
		
		OSFlagPost(flags_event,DataProFinish,OS_FLAG_SET,&err);

	}
}


//终端处理任务
void DealUsart_Task(void *p)
{
	for(;;)
	{
//		SendMessage("123456\r\n");
//		delay_ms(1000);
	}
}


//人机交互处理任务
void InteractCon_Task(void *p)
{
	for(;;)
	{
		
	}
}



