#include "checksum.h"
#include "stdio.h"
//************����У��*********************
unsigned char RxCheckSum(unsigned char ptr[])
{
	unsigned char i;
	unsigned char checksum;
	checksum = 0;
	for(i=0;i<(ptr[1]-1);i++)
	{
		checksum ^= ptr[i];
	}
	checksum = ~checksum;
	if(ptr[ptr[1]-1] == checksum)
	{
		return 	CheckSum_OK;
	}
	else 
	{
		return 	CheckSum_ERR;
	}
}

//**************����У��**********
void TxCheckSum(unsigned char ptr[])
{
	unsigned char i;
	unsigned char checksum;
	checksum = 0;
	for(i=0;i<(ptr[1]-1);i++)
	{
		checksum ^= ptr[i];
	}
	checksum = ~checksum;
	ptr[ptr[1]-1] = checksum;
	//printf("checsum is %02x \r\n",ptr[ptr[1]-1]);
	
}
