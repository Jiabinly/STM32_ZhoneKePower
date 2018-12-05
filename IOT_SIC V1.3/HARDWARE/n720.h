#ifndef __N720_H
#define __N720_H

#include "stm32f10x.h"
//#include "module.h"
#include "usart1.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG 1        //����ģʽ
 
#define RESPONSEOK 1    //�ظ���ȷ
#define ERROR_NORSP 0   //�޻ظ�
#define ERROR_RSPER 2   //�ظ�����
#define ERROR_FALL 3    //ʧ��
#define ALREADYCON 4     //�Ѿ�����

#define ModeWork 1      //ģ����������ģʽ
#define ModeUpgrade 2   //ģ������ģʽ

#define CLOUDCONN      1      //��������
#define CLOUDSUB       2			 //������Ϣ
#define CLOUDPUB       3      //������Ϣ
#define CLOUDSTATE     4      //��ѯMQTT����״̬
#define CLOUDDISCONN   5      //�ر�����



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
