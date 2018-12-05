#include "nvc.h"
#include "delay.h"

void NVC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PC�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;				     //NVC_DATA-->PB.0�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					       //�����趨������ʼ��GPIOB.0
  GPIO_SetBits(GPIOB,GPIO_Pin_0); 						           //NVC_DATAĬ������� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		   //NVC_BUSY-->PC.5 �˿�����, 
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU; 	       //��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  		       //������� ��IO���ٶ�Ϊ50MHz
	
	NVC_SendData(VOLUME_MAX);                     //����Ĭ�����
	NVC_SendData(VOICE_NUM0);                     //���ŵ�һ�׸�       
}

//�ȶ�æ״̬���󲥷�
void NVC_SendData(int NvcData)
{ 
	while(NVC_BUSY==0)
	{
		//�ȴ���һ����������
	}
	//NVC_SendByte(PALY_STOP);
	//delay_ms(100);
	NVC_SendByte(NvcData);
}
//һ������֡����
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
