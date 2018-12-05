#ifndef _CCB_H
#define _CCB_H
#include "sys.h"
#include "delay.h"
#include "stdio.h"

#define SIC0xx 0x04  //SIC0xx充电站类型，0x02=12口整机，0x04=24口整机
#define RS485_TX_EN PAout(1)
#define UART2_RX_BUF_LEN  30

void USART2_Init(void);
void USART2_Send_Data(unsigned char *buf);

void CCB_Init(void);    //CCB通信RS485口初始化函数
void CCB_Scan(void);    //All online's CCB scan 
void CCB_Type(void);    //查询在线CCB的类型
void CCB_ChnOn(u8 channel); //通道开启
void CCB_ChnCurrent(u8 channel); //电流查询
void CCB_ChnOff(u8 channel); //通道关闭
void CCB_ErrQuery(void); //异常查询

#endif
