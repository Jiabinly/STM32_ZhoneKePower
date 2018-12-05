#ifndef __MAIN_H
#define __MAIN_H

#include "sys.h"
#include "delay.h"
//#include "console.h"
//#include "module.h"
#include "gpio.h"
#include "usart1.h"
#include "beep.h"
#include "string.h" 
#include "nvc.h"
#include "ccb.h"
#include "includes.h"
#include "n720.h"
#include "Pro8583.h"

void MainManager_Task(void *p);    //������
void Charging_Task(void *p);       //����彻������
void ModTXD_Task(void *p);         //���ͽ�����������
void DealUsart_Task(void *p);      //�ն˴�������
void InteractCon_Task(void *p);    //�˻�������������

#endif
