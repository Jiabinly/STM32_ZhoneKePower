#ifndef _CCB_H
#define _CCB_H
#include "sys.h"
#include "delay.h"
#include "stdio.h"

#define SIC0xx 0x04  //SIC0xx���վ���ͣ�0x02=12��������0x04=24������
#define RS485_TX_EN PAout(1)
#define UART2_RX_BUF_LEN  30

void USART2_Init(void);
void USART2_Send_Data(unsigned char *buf);

void CCB_Init(void);    //CCBͨ��RS485�ڳ�ʼ������
void CCB_Scan(void);    //All online's CCB scan 
void CCB_Type(void);    //��ѯ����CCB������
void CCB_ChnOn(u8 channel); //ͨ������
void CCB_ChnCurrent(u8 channel); //������ѯ
void CCB_ChnOff(u8 channel); //ͨ���ر�
void CCB_ErrQuery(void); //�쳣��ѯ

#endif
