#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//��������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//�������˿ڶ���

//#define BEEP GPIO_Pin_8	 //BEEP,�������Ϸ������ӿ���PB8
#define BEEP GPIO_Pin_1  // BEEP,MCB01���Ϸ������ӿ���PB1

#define BEEPon Bit_SET     //����BEEP
#define BEEPoff Bit_RESET  // �ر�BEEP

void BEEP_Init(void);	     //��ʼ��BEEP
void BEEP_Onoff(void);		//�������ر�BEEP

#endif

