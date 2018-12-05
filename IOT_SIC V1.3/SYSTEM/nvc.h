#ifndef __NVC_H
#define __NVC_H	 
#include "sys.h"

#define VOICE_NUM0  0x00    //默认播放第1段语音，最多223段(0xDF)语音
#define VOLUME_MIN 0xE0    //最小音量
#define VOLUME_MAX 0xE7    //最大音量
#define PALY_LOOP 0xF2     //循环播放
#define PALY_STOP 0xFE     //停止播放

#define NVC_BUSY  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) //读取NVC_BUSY状态
#define NVC_SDA PBout(0)                                  //NVC单线串口

void NVC_Init(void);
void NVC_SendData(int NvcData);
void NVC_SendByte(int NvcByte);
#endif
