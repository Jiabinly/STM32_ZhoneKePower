#ifndef __NVC_H
#define __NVC_H	 
#include "sys.h"

#define VOICE_NUM0  0x00    //Ĭ�ϲ��ŵ�1�����������223��(0xDF)����
#define VOLUME_MIN 0xE0    //��С����
#define VOLUME_MAX 0xE7    //�������
#define PALY_LOOP 0xF2     //ѭ������
#define PALY_STOP 0xFE     //ֹͣ����

#define NVC_BUSY  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) //��ȡNVC_BUSY״̬
#define NVC_SDA PBout(0)                                  //NVC���ߴ���

void NVC_Init(void);
void NVC_SendData(int NvcData);
void NVC_SendByte(int NvcByte);
#endif
