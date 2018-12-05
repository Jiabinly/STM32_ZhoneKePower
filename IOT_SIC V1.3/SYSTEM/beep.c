#include "beep.h"
#include "delay.h" 	   

//初始化PB8为输出口.并使能这个口的时钟		    
//蜂鸣器初始化
u8 BEEP_State=0;
u16 BEEP_Number=0;

void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB端口时钟
 
 GPIO_InitStructure.GPIO_Pin = BEEP;				 //BEEP-->PB.1 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
 
 GPIO_ResetBits(GPIOB,BEEP);//输出0，关闭蜂鸣器输出

}

//蜂鸣器开启、关闭函数
void BEEP_Onoff()
{
//	BEEP_State=1;
  GPIO_SetBits(GPIOB,BEEP);
	delay_ms(1000);
	GPIO_ResetBits(GPIOB,BEEP);
	
	
	
}

