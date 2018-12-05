#ifndef __USART1_H
#define __USART1_H

#include "stdio.h"
#include "stm32f10x.h"
#include "delay.h"
#include "string.h"
//#include "protocol.h"
#include "Pro8583.h"


#define USART_REC_LEN 600 

#define ExtIntBit                 0x01     //信号量集第一位，接收数据到    
#define DataProFinish             0x02     //信号量集第一位，数据拆包完成


typedef struct {
  uint8_t DATA_RX[USART_REC_LEN];
  uint16_t RX_STA;
} USART_DataType;

extern USART_DataType USART1_Data;
//extern USART_DataType USART2_Data;
extern USART_DataType USART3_Data;
extern uint8_t RecvBuf[512];

uint8_t USART2_CheckRec(uint8_t *ptr);
void USART_Configuration(void);
void USART_SendString(USART_TypeDef *USARTx, uint8_t *Data, uint16_t Long);
uint8_t USART_CheckRec(uint8_t *ptr);
void USART_ClearBuf(USART_DataType *USART_Data);

#endif


