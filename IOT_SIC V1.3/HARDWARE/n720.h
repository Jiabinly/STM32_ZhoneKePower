#ifndef __N720_H
#define __N720_H

#include "stm32f10x.h"
//#include "module.h"
#include "usart1.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG 1        //调试模式
 
#define RESPONSEOK 1    //回复正确
#define ERROR_NORSP 0   //无回复
#define ERROR_RSPER 2   //回复错误
#define ERROR_FALL 3    //失败
#define ALREADYCON 4     //已经连接

#define ModeWork 1      //模块正常工作模式
#define ModeUpgrade 2   //模块升级模式

#define CLOUDCONN      1      //建立连接
#define CLOUDSUB       2			 //订阅消息
#define CLOUDPUB       3      //发布消息
#define CLOUDSTATE     4      //查询MQTT连接状态
#define CLOUDDISCONN   5      //关闭连接



uint8_t Gprs_AT(void);
uint8_t MD_PowerReset(void);
//uint8_t MD_ModeSelect(uint8_t mode);
uint8_t Gprs_testComms(void);
uint8_t Gprs_GMR(void);
uint8_t Gprs_CCID(void);
uint8_t Gprs_CPIN(void);
uint8_t Gprs_CSQ(void);
uint8_t Gprs_CREG(void);
uint8_t Gprs_ReadCGATT(void);
uint8_t N720_Module_Config(void);
uint8_t	MQTT_CGDCONT(void);
uint8_t	MQTT_XIIC(void);
uint8_t	MQTT_XIICQuery (void);
uint8_t	MQTT_CLOUDHDAUTH(char *productKey,char *deviceName,char *deviceSecret);
uint8_t	MQTT_CLOUDCONN(void);
uint8_t	MQTT_CLOUDSUB(char * productKey,char *deviceName);
uint8_t	MQTT_CLOUDPUB(char * productKey,char *deviceName,char *SendData);
uint8_t	MQTT_CLOUDSTATE(void);
uint8_t MQTTConProc(char * productKey,char *deviceName,char *deviceSecret);
uint8_t MQTT_Handler(void);
uint8_t MQTT_SendReceiveData(uint8_t At_num,char * productKey,char *deviceName,char *SendData);


#endif
