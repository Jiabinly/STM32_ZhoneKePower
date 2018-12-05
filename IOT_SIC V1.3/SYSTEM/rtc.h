#ifndef __RTC_H
#define __RTC_H	    
#include "stm32f10x.h"
#include "led.h"
#include "protocol.h"
#include "includes.h"

extern u32 HeartBeatCount ;
u8 RTC_Init(uint32_t time_h);
void RTC_IRQHandler(void);
void ALARM_CNT_Set(u32 cntx);

#endif

