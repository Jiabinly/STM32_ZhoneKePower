#include "beep.h"
#include "delay.h" 	   

//��ʼ��PB8Ϊ�����.��ʹ������ڵ�ʱ��		    
//��������ʼ��
u8 BEEP_State=0;
u16 BEEP_Number=0;

void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��GPIOB�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = BEEP;				 //BEEP-->PB.1 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOB.8
 
 GPIO_ResetBits(GPIOB,BEEP);//���0���رշ��������

}

//�������������رպ���
void BEEP_Onoff()
{
//	BEEP_State=1;
  GPIO_SetBits(GPIOB,BEEP);
	delay_ms(1000);
	GPIO_ResetBits(GPIOB,BEEP);
	
	
	
}

