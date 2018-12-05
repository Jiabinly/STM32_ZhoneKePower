
// Header:Interface for gprs mqtt of n720 !
// File Name: n720.c
// Author:Jiabinly
// Date:2018/10/11

#include "n720.h"

uint8_t Gprs_Buf[200];

extern USART_DataType USART3_Data;


//需要保存的参数有软件版本号、硬件版本号


/*
  - Output: return 1 if  succesfully. 
  - Purpose: Restart the module.
*/
uint8_t MD_PowerReset(void)
{    
		 uint8_t i,ii = 0;
//  	 printf("888888888\r\n");
		do{	
				 i++;
				 GPIO_SetBits(GPIOD, GPIO_Pin_8);
				 delay_ms(5000);
				 GPIO_ResetBits(GPIOD, GPIO_Pin_8);
				 delay_ms(1000);
				 if(MQTT_Handler()==0x05)//模组重新上电后继续尝试连接服务器,重启成功
				 {
					 printf("Reset OK@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@!\r\n");
					 return 1;
				 }
				 else
				 {
						ii++;
				 }
			}while(i<4);
		 
		 if(ii==3)
		 {
			 //若重启三次仍然没有建立连接,再次尝试重启系统；
				__set_FAULTMASK(1); // 关闭所有中断
				NVIC_SystemReset(); // 复位

		 }

		 return 0;  
}


/*
  - Output: return 1 if  succesfully. 
  - Purpose: Module mode selection.if mode = 1,The module normal operation.else mode =2,Module into upgrade mode.
*/
//uint8_t MD_ModeSelect(uint8_t mode)
//{    
//	  if(mode==ModeWork)     //模块正常工作
//		{
//			GPIO_ResetBits(GPIOE, GPIO_Pin_14);
//			
//		}
//		if(mode==ModeUpgrade)  //模块升级模式
//		{
//			GPIO_SetBits(GPIOE, GPIO_Pin_14);
//		}
//		 return 1;  
//}



/*
  - Output: return 1 if  succesfully. Return 0 otherwise.
  - Purpose: Test whether the n720 module connection is intact.
*/
uint8_t Gprs_testComms(void)
{
	uint8_t i = 0;
	
	USART_SendString(USART3,(uint8_t *)"AT\r\n", 0);
	do{
//			#ifdef DEBUG
//	     	   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
//			#endif
			if (USART_CheckRec((uint8_t *)"+PBREADY")||(USART_CheckRec((uint8_t *)"OK")))
			{		
				#ifdef DEBUG
				USART_SendString(USART1, USART3_Data.DATA_RX, 0);
				#endif
					return RESPONSEOK;
			}
			i++;
			delay_ms(50);
		}while(i<30);  
	return ERROR_NORSP;
}


uint8_t Gprs_AT(void)
{
	uint8_t i = 0;
	USART_SendString(USART3,(uint8_t *)"AT\r\n", 0);
	do{
			
			if (USART_CheckRec((uint8_t *)"OK")) 
			{		
//				#ifdef DEBUG
				printf("AT\r\nOK\r\n");
//				#endif
			
					return RESPONSEOK;
			}
			i++;
			delay_ms(50);
		}while(i<30);  
	return ERROR_NORSP;
}
/*
  - Output: return 1 if  succesfully. Return 0 otherwise.
  - Purpose: get the n720 module firmware version number.
*/
uint8_t Gprs_GMR(void) 
{
		uint8_t i = 0;
//		uint8_t *pri;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3,(uint8_t *)"AT+GMR\r\n",0);
	
//	  #ifdef DEBUG
//	
//	     	    USART_SendString(USART1, (uint8_t *)"AT+GMR\r\n",0);      
//	  #endif
	 
		do {
//				  #ifdef DEBUG
//	     	   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
//	        #endif
			     
					if (USART_CheckRec((uint8_t *)"OK")&&(USART_CheckRec((uint8_t *)"+GMR:")))  
					{
//								pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX,"+GMR:");
								#ifdef DEBUG
								USART_SendString(USART1, USART3_Data.DATA_RX, 0);
								#endif
//								NB_WriteBasicOPt();
						
								return 1;

					}else if(USART_CheckRec((uint8_t *)"ERROR"))
					{
						    return 0;
					}
					
					i++;
					delay_ms(50);
		} while (i < 30);
		return 0; 
}


/*
  - Output: return 1 if  succesfully. Return 0 otherwise.
  - Purpose: Get the SIM card id.if return ERROR,no SIM card!
*/
uint8_t Gprs_CCID(void) 
{
		uint8_t i = 0;
//		uint8_t *pri;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3,(uint8_t *)"AT+CCID\r\n",0);
	
		do { 
					#ifdef DEBUG
	     	   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
	        #endif
					if (USART_CheckRec((uint8_t *)"OK")&&(USART_CheckRec((uint8_t *)"+CCID:")))  
					{
								return RESPONSEOK;
					}
					else
					{
						if((USART_CheckRec((uint8_t *)"ERROR")))
						{
							return ERROR_RSPER;
						}
//						else
//						{
//							USART_SendString(USART3, (uint8_t *)"AT+CCID\r\n",0);
//						}
					}
					i++;
					delay_ms(50);
		} while (i < 30);  //查询5次
		return ERROR_NORSP; 
}


/*
  - Output: return 1 if  succesfully. Return 0 otherwise.
  - Purpose: Check whether the SIM card is inserted properly!
*/
uint8_t Gprs_CPIN(void) 
{
		uint8_t i = 0;
//	  uint8_t *pri;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, (uint8_t *)"AT+CPIN?\r\n",0);
	
		do {
				  #ifdef DEBUG
	     	   USART_SendString(USART1, USART3_Data.DATA_RX, 0);
	        #endif
			     
					if (USART_CheckRec((uint8_t *)"OK")&&(USART_CheckRec((uint8_t *)"+CPIN: READY")))  
					{
							return RESPONSEOK;
					}
					else
					{
						 if(USART_CheckRec((uint8_t *)"+CPIN: ERROR"))
						 {
							 return ERROR_RSPER;
						 }
//						 else
//						 {
//							 USART_SendString(USART3,(uint8_t *)"AT+CCID\r\n",0);
//						 }
					}
					
					i++;
					delay_ms(50);
		} while (i < 30);  //查询3次
		return ERROR_NORSP; 
}


/*
  - Output: return 1 if  succesfully. Return 0 otherwise.
  - Purpose: Query signal quality!
*/
uint8_t Gprs_CSQ(void) 
{
		uint8_t i = 0,temp;
		uint8_t *pri;
		memset(Gprs_Buf,0,50);
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, (uint8_t *)"AT+CSQ\r\n", 0);
		do {
					#ifdef DEBUG
							    USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif	
			    if(USART_CheckRec((uint8_t *)"OK")) 
					{
							pri = (uint8_t *)strstr((char *)USART3_Data.DATA_RX, "+CSQ:");
							memcpy(Gprs_Buf,pri,10);
							sscanf((char *)pri, "%*[^:]:%[^,]",Gprs_Buf); 
							temp=atoi((char *)Gprs_Buf);  
							if((temp>0)&&(temp<=31))
				      {
								return RESPONSEOK;
							}
			    }
//					else 
//				  {
//						USART_SendString(USART3,(uint8_t *) "AT+CSQ\r\n", 0);
//			    }
				  i++;
				  delay_ms(200);
		} while (i < 150); 
		return ERROR_NORSP;   
}


/*
  - Output: return 1 if  succesfully. Return 0 otherwise.
  - Purpose: Query network registration status
*/
uint8_t Gprs_CREG(void) 
{
	
		uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3,(uint8_t *)"AT+CREG?\r\n", 0);
		do {
					#ifdef DEBUG
									USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					if(((USART_CheckRec((uint8_t *)"OK"))&&(USART_CheckRec((uint8_t *)"+CREG: 0,1")))||
						((USART_CheckRec((uint8_t *)"OK"))&&(USART_CheckRec((uint8_t *)"+CREG: 0,5")))) 
					{
							 return RESPONSEOK;
					}
					else 
					{
						if(USART_CheckRec((uint8_t *)"ERROR"))
						{
							return ERROR_RSPER;
						}
//						else
//						{
//							USART_SendString(USART3,(uint8_t *)"AT+CREG?\r\n", 0);
//						}
					}

					i++;
					delay_ms(100);
		} while (i < 200); 
		return ERROR_NORSP;   
}


/*
  - Output: return 1 if  succesfully. Return 0 otherwise.
  - Purpose: Query service attachment status
*/
uint8_t Gprs_ReadCGATT(void) 
{
		uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, (uint8_t *)"AT+CGATT?\r\n",0);
	
		do {
				#ifdef DEBUG
            USART_SendString(USART1, USART3_Data.DATA_RX, 0);
        #endif
				if (USART_CheckRec((uint8_t *)"OK")&&(USART_CheckRec((uint8_t *)"+CGATT: 1"))) 
				{
					return RESPONSEOK;
				}			
//				else 
//				{
//						USART_SendString(USART3,(uint8_t *)"AT+CGATT?\r\n",0);
//				}

			  i++;
				delay_ms(200);
		} while (i < 150);
	 
//循环查询10次仍未附着成功，进行主动附着
		USART_SendString(USART3, (uint8_t *)"AT+CGATT=0\r\n",0);
		if (USART_CheckRec((uint8_t *)"OK"))
		{
			USART_SendString(USART3,(uint8_t *)"AT+CGATT=1\r\n",0);
			if (USART_CheckRec((uint8_t *)"OK"))
			{
				USART_SendString(USART3,(uint8_t *)"AT+CGATT?\r\n",0);
				if (USART_CheckRec((uint8_t *)"OK")&&(USART_CheckRec((uint8_t *)"+CGATT: 1"))) 
				{
					return RESPONSEOK;
				}
				else
				{
					return ERROR_FALL; 
				}
			}
		}
		return RESPONSEOK;
}



/*
N720初始化，检查网络模块注册状态；
*/
uint8_t N720_Module_Config(void)
 {
	 
//	uint8_t i = 0;
//	 delay_ms(7000);//上电延时2秒钟
	 
	 //1,检查串口是否已通；
//	 do
//	 {
//		 i++;
//		 if(Gprs_testComms()== 1)
//		 {
//			 printf("$$$$\r\n");
//			 break;
//		 }
//		 if(Gprs_testComms()== 0)
//		 {
//			#ifdef DEBUG
//				printf("Serial connection is not good,Module restart...\r\n");
//			#endif
//			 MD_PowerReset();//无返回，则重启模块；
//		 }
//	 }while((i<=15)&&(Gprs_testComms()== 0));
		
		
		if(Gprs_testComms()== 0)
		{
			printf("Serial connection is not good,Module restart...\r\n");
			MD_PowerReset();
		 return 0;
		}
		
//		if(Gprs_AT()== 0)
//		{
//			printf("Serial connection is not good,Module restart...\r\n");
//			MD_PowerReset();
//			return 0;
//		}
		 
	
	 
	 //2,查询是否读到SIM卡
	 if(Gprs_CCID()== 0||Gprs_CCID()==2)
	 {
		#ifdef DEBUG
			printf("SIM card not read,Module restart...\r\n");
		#endif
		//无返回，则重启模块；
		 MD_PowerReset();
		 return 0;
	 }
	 
	 //3,检查SIM卡状态
	 if(Gprs_CPIN()== 0||Gprs_CPIN()==2)
	 {
		#ifdef DEBUG
			printf("SIM card unavailable,Replace the SIM card...\r\n");
		#endif
		//无返回，则重启模块；
		 MD_PowerReset();
		 return 0;
	 }
	 
	 //4,检查信号强度
	 if(Gprs_CSQ()== 0)
	 {
		#ifdef DEBUG
			printf("Signal quality is not good,Module restart...\r\n");
		#endif
		//无返回，则重启模块；
		  MD_PowerReset();
		 return 0;
	 }
	 
	 //5,查询网络注册情况
	 if(Gprs_CREG()==0||Gprs_CREG()==2)
	 {
		#ifdef DEBUG
			printf("Network registration status is not good,Module restart...\r\n");
		#endif
		//无返回，则重启模块；
		  MD_PowerReset();
		 return 0;
	 }
	 
	 //6,查询网络附着状态，未附着上进行主动附着
	 if(Gprs_ReadCGATT()==3)
	 {
		#ifdef DEBUG
			printf("Network registration status is not good,Module restart...\r\n");
		#endif
		//无返回，则重启模块；
		  MD_PowerReset();
		 return 0;
	 }
	 
		return RESPONSEOK;
}
 

/*
设置网络连接初始化参数
*/
uint8_t	MQTT_CGDCONT(void)
{
//AT+CGDCONT=1,"IP","CMNET"
		uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, (uint8_t *)"AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n",0);
	
		do {
				#ifdef DEBUG
            USART_SendString(USART1, USART3_Data.DATA_RX, 0);
        #endif
				if (USART_CheckRec((uint8_t *)"OK")) 
				{
					return RESPONSEOK;
				}			
			  i++;
				delay_ms(100);
		} while (i < 100);
		return ERROR_NORSP; 
}


/*
激活网络连接
*/
uint8_t	MQTT_XIIC(void)
{
//AT+XIIC=1
//AT+XIIC?
	uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, (uint8_t *)"AT+XIIC=1\r\n",0);
	
		do {
				#ifdef DEBUG
            USART_SendString(USART1, USART3_Data.DATA_RX, 0);
        #endif
				if (USART_CheckRec((uint8_t *)"OK")) 
				{
					return RESPONSEOK;
				}			
			  i++;
				delay_ms(100);
		} while (i < 100);
		return ERROR_NORSP; 
}


/*
查询网络连接是否激活
*/
uint8_t	MQTT_XIICQuery (void)
{
//AT+XIIC=1
//AT+XIIC?
		uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, (uint8_t *)"AT+XIIC?\r\n",0);
	
		do {
			
				#ifdef DEBUG
            USART_SendString(USART1, USART3_Data.DATA_RX, 0);
        #endif
			 
				if (USART_CheckRec((uint8_t *)"OK")&&USART_CheckRec((uint8_t *)"+XIIC:    1,")) //+XIIC:    1,10.101.203.236
				{
					return RESPONSEOK;
				}
//				else
//				{
//					USART_SendString(USART3, (uint8_t *)"AT+XIIC?\r\n",0);
//				}
			  i++;
				delay_ms(100);
		} while (i < 100);
		return ERROR_NORSP; 
}


/*
服务器鉴权
*/
uint8_t	MQTT_CLOUDHDAUTH(char *productKey,char *deviceName,char *deviceSecret)
{
	//AT+CLOUDHDAUTH=Lk2upKmgG0w,500000000051,cr5ha69Fzurz3F2bdBR7ulf3ti4y1xcs
	uint8_t i = 0;
	static uint8_t auth_buf[128]={0};
	
//	productKey ="Lk2upKmgG0w";
//	deviceName ="500000000051";
//	deviceSecret ="cr5ha69Fzurz3F2bdBR7ulf3ti4y1xcs";

	USART_ClearBuf(&USART3_Data);
	sprintf((char *)auth_buf,"AT+CLOUDHDAUTH=%s,%s,%s\r\n",productKey,deviceName,deviceSecret);
	USART_SendString(USART3,auth_buf,0);
	do {

				delay_ms(1000);
				if (USART_CheckRec((uint8_t *)"OK")&&(USART_CheckRec((uint8_t *)"+CLOUDHDAUTH: OK")))
				{
						#ifdef DEBUG
            USART_SendString(USART1, USART3_Data.DATA_RX, 0);
						#endif
					return RESPONSEOK;
				}
			  i++;
				delay_ms(100);
		} while (i < 100);
		return ERROR_NORSP; 
}


/*
建立连接
*/
uint8_t	MQTT_CLOUDCONN(void)
{
	//AT+CLOUDCONN=60,0,4
		uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, (uint8_t *)"AT+CLOUDCONN=180,0,4\r\n",0);
	
		do {
				if (USART_CheckRec((uint8_t *)"OK")) 
				{
					#ifdef DEBUG
            USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					USART_ClearBuf(&USART3_Data);
					return RESPONSEOK;
				}
			  i++;
				delay_ms(100);
		} while (i < 100);
		USART_ClearBuf(&USART3_Data);
		return ERROR_NORSP; 
}


/*
订阅消息
*/
uint8_t	MQTT_CLOUDSUB(char * productKey,char *deviceName)
{
	//AT+CLOUDSUB=/Lk2upKmgG0w/500000000051/get,1
	uint8_t i = 0;
	uint8_t SubBuf[128]={0};
		USART_ClearBuf(&USART3_Data);
		sprintf((char *)SubBuf,"AT+CLOUDSUB=/Lk2upKmgG0w/500000000051/get,1\r\n"/*,productKey,deviceName*/);
		USART_SendString(USART3,SubBuf,0);
//		USART_SendString(USART1,SubBuf,0);
		do{
				if (USART_CheckRec((uint8_t *)"OK")&&(USART_CheckRec((uint8_t *)"+CLOUDSUBACK: <OK>"))) 
				{
					#ifdef DEBUG
						USART_SendString(USART1, USART3_Data.DATA_RX, 0);
					#endif
					USART_ClearBuf(&USART3_Data);
					return RESPONSEOK;
				}
				i++;
				delay_ms(100);
			} while (i < 100);
			USART_ClearBuf(&USART3_Data);
			return ERROR_NORSP; 
	
}


/*
发布消息
*/
uint8_t	MQTT_CLOUDPUB(char * productKey,char *deviceName,char *SendData)
{
	//AT+CLOUDPUB =/Lk2upKmgG0w/500000000051/update,1,11222211
	uint8_t i = 0;
	uint8_t PubBuf[128]={0};
	USART_ClearBuf(&USART3_Data);
	sprintf((char *)PubBuf,"AT+CLOUDPUB=%c%s%c%s/update,1,%s\r\n",'/',productKey,'/',deviceName,SendData);
	USART_SendString(USART3,PubBuf,0);
	
	do{			
			if (USART_CheckRec((uint8_t *)"OK")&&(USART_CheckRec((uint8_t *)"+CLOUDPUBACK: <OK>"))) 
			{
				#ifdef DEBUG
					USART_SendString(USART1, USART3_Data.DATA_RX, 0);
				#endif
				USART_ClearBuf(&USART3_Data);
				return RESPONSEOK;
			}
			i++;
			delay_ms(100);
		 } while (i < 100);
		USART_ClearBuf(&USART3_Data);
			return ERROR_NORSP; 
	
}
	



/*
查询MQTT连接状态
*/
uint8_t	MQTT_CLOUDSTATE(void)
{
	//AT+CLOUDSTATE?
	uint8_t i = 0;
		USART_ClearBuf(&USART3_Data);
		USART_SendString(USART3, (uint8_t *)"AT+CLOUDSTATE?\r\n",0);
	
		do {
				#ifdef DEBUG
            USART_SendString(USART1, USART3_Data.DATA_RX, 0);
        #endif
				if (USART_CheckRec((uint8_t *)"OK")&&USART_CheckRec((uint8_t *)"+CLOUDSTATE: 1")) 
				{
					return RESPONSEOK;
				}
			  i++;
				delay_ms(500);
		} while (i < 30);
		return ERROR_NORSP; 
}


/*
MQTT流程控制
*/
uint8_t MQTTConProc(char * productKey,char *deviceName,char *deviceSecret)
{
		uint8_t Statua_Count = 0;
		uint8_t Statua_Code = 0;
	  uint8_t Status = 0;
		uint8_t InitCount =0; //激活网络连接计数器；
		uint8_t MqttStatus = 0;
		
		delay_ms(2000);
		printf("delay 2s\r\n");
		delay_ms(2000);
		printf("delay 2s\r\n");
		delay_ms(2000);
		printf("delay 2s\r\n");
		delay_ms(2000);
		printf("delay 2s\r\n");
		delay_ms(2000);
		printf("delay 2s\r\n");
		delay_ms(2000);
		printf("delay 2s\r\n");
		delay_ms(2000);
		printf("delay 2s\r\n");
		delay_ms(2000);
		printf("delay 2s\r\n");
		delay_ms(2000);
		printf("delay 2s\r\n");
		while(Statua_Count < 1)
		{
			
							switch (Statua_Code)
				{
								case 0x00:
													MqttStatus = 0x00;	
													Status = N720_Module_Config();     //N720初始化;
													if(Status ==1)
													{
														Status =0;
														Statua_Code = 0x01;
													}
													else
													{
														goto OUTSW;
													}
													break;
								case 0x01:
													MqttStatus = 0x01;
													Status = MQTT_CGDCONT();            //设置网络连接初始化参数;
													if(Status ==1)
													{
														Status =0;
														Statua_Code = 0x02;
													}
													else
													{
														Statua_Code = 0x00;
													}
													break;
								case 0x02:
													MqttStatus = 0x02;
													Status = MQTT_XIIC();                //激活网络连接
													if(Status ==1)
													{
														Status =0;
														Statua_Code = 0x03;
														InitCount ++;
													}
													else
													{
														Statua_Code = 0x00;
													}
													break;
								case 0x03:
													MqttStatus = 0x03;
													Status = MQTT_XIICQuery ();                //查询网络连接是否激活
													if(Status ==1)
													{
														Status =0;
														Statua_Code = 0x04;
													}
													else
													{
														Statua_Code = 0x02;
														if(InitCount == 3)         //激活三次仍未获得IP，则返回N720初始化
														{
															
															Statua_Code = 0x00; 
														}
														
													}
													break;
													
						
								case 0x04:
													MqttStatus = 0x04;
													Status = MQTT_CLOUDHDAUTH( productKey,deviceName,deviceSecret);     
													if(Status ==1)
													{
														Status =0;
														Statua_Code = 0x05;
													}
													else
													{
														Statua_Code = 0x04;
													}
													break;
													
													
								case 0x05:
													MqttStatus = 0x05;
													goto OUTSW;
													
								default:
												break;
				}

		}
		OUTSW:
				Statua_Count++;
		
		return MqttStatus;
}


/*
MQTT处理函数
*/

uint8_t MQTT_Handler(void) 
{
	uint8_t MqttStatus = 0;

//	MD_ModeSelect(ModeWork);//模块为正常通信模式，ModeWork  ； ModeUpgrade ，模块升级模式
	MqttStatus = MQTTConProc("Lk2upKmgG0w","500000000051","cr5ha69Fzurz3F2bdBR7ulf3ti4y1xcs");  //MQTT流程控制
	////AT+CLOUDHDAUTH=Lk2upKmgG0w,500000000051,cr5ha69Fzurz3F2bdBR7ulf3ti4y1xcs
  switch (MqttStatus) 
	{
         case 0x00: 
              USART_SendString(USART1, (uint8_t *)"Module Restart OK...\r\n",0);
              break;
         case 0x01:
              USART_SendString(USART1, (uint8_t *)"Set CGDCONT Error...\r\n",0);
							break;
         case 0x02: 
              USART_SendString(USART1, (uint8_t *)"Set XIIC Error...\r\n",0);
              break;
         case 0x03:
              USART_SendString(USART1, (uint8_t *)"Query XIIC Error...\r\n",0);
              break;
         case 0x04: 
              USART_SendString(USART1, (uint8_t *)"Set CLOUDHDAUTH faild...\r\n",0);
              break;
				 case 0x05:
							USART_SendString(USART1, (uint8_t *)"Configuration is successful\r\n",0);
							#ifdef DEBUG
							printf("**************************************************************************************\r\n");
							printf("********************Initialization module is completed!*******************************\r\n");
							printf("**************************************************************************************\r\n");
							#endif
              break;
        default:
              break;
  }
	return MqttStatus;
}


uint8_t MQTT_SendReceiveData(uint8_t At_num,char * productKey,char *deviceName,char *SendData)
{
	uint8_t Status = 0;
	uint8_t Sign = 0;  //是否连续执行标志
	while(At_num<6)
	{
		switch (At_num)
				{		
								case CLOUDCONN: //01
													
			/*连接*/					  Status = MQTT_CLOUDCONN();            
													if(Status ==1)
													{
//														printf("CLOUDCONN success!\r\n");
														if(Sign!= 0)
														{
															if(Sign == 1)
															{
																At_num =  CLOUDSUB;
																Sign = 0;
															}
															if(Sign == 2)
															{
																At_num = CLOUDPUB;
																Sign = 0;
															}
															
														}
														else
														{
															return 1;
														}
													}
													else
													{
														At_num =  CLOUDCONN;
//														printf("CLOUDCONN failed!\r\n");
													}
													break;
								case CLOUDSUB:  // 2
			/*订阅*/				    Status = MQTT_CLOUDSUB(productKey,deviceName);                
													if(Status ==1)
													{
//														printf("CLOUDSUB success!\r\n");
														return 1;
													}
													else
													{
														At_num  =  CLOUDCONN;  //重新连接
														Sign = 1;
//														printf("CLOUDSUB failed!\r\n");
													}
													break;
								case  CLOUDPUB:  //3
			/*发布*/						Status = MQTT_CLOUDPUB(productKey,deviceName,SendData);
													if(Status ==1)
													{
//														printf("CLOUDPUB success!\r\n");
														return 1;
													}
													else
													{
														At_num  =  CLOUDCONN;
														Sign = 2;
//														printf("CLOUDSUB failed!\r\n");
													}
														break;
													
									default:
														break;
				}
				
		}
		USART_ClearBuf(&USART3_Data);
		return 1;
}
			
							
/*发送数据*/
//uint8_t SendMessage(char *outData)  
//{
//	
//	memset(Gprs_Buf,0,200);
//  USART_ClearBuf(&USART3_Data);

//  if(outData == NULL)
//	{
//		printf("outData is NULL.\r\n");
//		return 0;
//	}
//	if(MQTT_CLOUDSTATE()==1)   //发送数据之前，检查MQTT的连接状态
//	{
//		MQTT_CLOUDPUB("Lk2upKmgG0w","500000000051",outData);
//	}
//	else                       //若MQTT状态查询为断开，需要重新建立MQTT连接
//	{
//		MQTT_Handler();
//		MQTT_CLOUDPUB("Lk2upKmgG0w","500000000051",outData);
//	}
//  return 0;   
//}



