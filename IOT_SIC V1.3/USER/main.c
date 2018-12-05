#include "main.h"


OS_CPU_SR   cpu_sr=0;
OS_FLAG_GRP * flags_event;	
extern uint8_t flag_usart3 ;
uint8_t RecvBuf[512]= {0};


/*�����������ȼ�*/
#define MainManager_Pri     10  //����������ȼ����
#define Charging_Pri        1   //��紦���������ȼ� 
#define ModTXD_Pri          2   //���������������ȼ�
#define DealUsart_Pri       3   //�ն˴����������ȼ�
#define InteractCon_Pri     4   //���������������ȼ�

/*�������������ջ��С*/
#define MainManager_Size  2048   //���ö�ջ��С
#define Charging_Size     2048   //���ö�ջ��С
#define ModTXD_Size       2048   //���ö�ջ��С
#define DealUsart_Size    2048   //���ö�ջ��С
#define InteractCon_Size  2048   //���ö�ջ��С

/*�����ջ*/
OS_STK  MainManager_Stack[MainManager_Size];
OS_STK  Charging_Stack[Charging_Size];
OS_STK  ModTXD_Stack[ModTXD_Size];
OS_STK  DealUsart_Stack[DealUsart_Size];
OS_STK  InteractCon_Stack[InteractCon_Size];

/*������*/
void MainManager_Task(void *p);    //������
void Charging_Task(void *p);       //����彻������
void ModTXD_Task(void *p);         //���ͽ�����������
void DealUsart_Task(void *p);      //�ն˴�������
void InteractCon_Task(void *p);    //�˻�������������



int main(void)
 { 
//	 uint8_t SendDataBuf[1024]={0};
	 int i = 0;
	 delay_init();	    				//��ʱ������ʼ��
	 GPIO_Configuration();   //����1��3����
	 USART_Configuration();   //��ʼ������1��3��
	 
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
 
//	 	EXTIX_Init();        			//��ʼ���ⲿ�жϣ���Ӧ�����ж�
//	  printf("EXTI init end!\r\n");

//		BEEP_Init();      				//��ʼ��������
		printf("Beep init end!\r\n");
		USART2_Init();	        	//��ʼ������2
	  printf("RS485 init end!\r\n");
	  NVC_Init(); 						//��ʼ������оƬ
		printf("Speaker init end!\r\n");
		printf("-----------------MCB board init end !----------------------\r\n");
		CCB_Init(); //CCB��ʼ��
		printf("-----------------CCB board init end !----------------------\r\n");	
		BEEP_Onoff();         	//��������һ��,��ʾϵͳ��ʼ����
	 
		flag_usart3 = 1;  //�������������ϵͳ�жϱ�־λ
		
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

//����彻������
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


//���ͽ�����������
void ModTXD_Task(void *p)
{
	u8 err;
	
	for(;;)
	{
		OSFlagPend(flags_event,ExtIntBit ,OS_FLAG_WAIT_SET_ANY|OS_FLAG_CONSUME,0,&err);  //���ܵ�ƽ̨��������
		Cut_Server_Payload(USART3_Data.DATA_RX);
		
		OSFlagPost(flags_event,DataProFinish,OS_FLAG_SET,&err);

	}
}


//�ն˴�������
void DealUsart_Task(void *p)
{
	for(;;)
	{
//		SendMessage("123456\r\n");
//		delay_ms(1000);
	}
}


//�˻�������������
void InteractCon_Task(void *p)
{
	for(;;)
	{
		
	}
}



