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

void MainManager_Task(void *p);    //主任务
void Charging_Task(void *p);       //与充电板交互任务
void ModTXD_Task(void *p);         //发送接收数据任务
void DealUsart_Task(void *p);      //终端处理任务
void InteractCon_Task(void *p);    //人机交互处理任务

#endif
