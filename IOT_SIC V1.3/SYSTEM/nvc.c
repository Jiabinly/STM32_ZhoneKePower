#include "nvc.h"
#include "delay.h"

void NVC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PC端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;				     //NVC_DATA-->PB.0端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					       //根据设定参数初始化GPIOB.0
  GPIO_SetBits(GPIOB,GPIO_Pin_0); 						           //NVC_DATA默认输出高 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		   //NVC_BUSY-->PC.5 端口配置, 
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU; 	       //上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  		       //推挽输出 ，IO口速度为50MHz
	
	NVC_SendData(VOLUME_MAX);                     //音量默认最大
	NVC_SendData(VOICE_NUM0);                     //播放第一首歌       
}

//先读忙状态，后播放
void NVC_SendData(int NvcData)
{ 
	while(NVC_BUSY==0)
	{
		//等待上一段语音结束
	}
	//NVC_SendByte(PALY_STOP);
	//delay_ms(100);
	NVC_SendByte(NvcData);
}
//一线数据帧发送
void NVC_SendByte(int NvcByte)
{ 
	u8 i=0;
	NVC_SDA=0;
	delay_ms(2);
	for (i=0;i<8;i++)
	{
		NVC_SDA=1;
		if(NvcByte&1)
		{
			delay_us(1500);
			NVC_SDA=0;
			delay_us(500);
		}
		else
		{
			delay_us(500);	
			NVC_SDA=0;		
			delay_us(1500);			
		}
		NvcByte>>=1;
	}
	NVC_SDA=1;
}
