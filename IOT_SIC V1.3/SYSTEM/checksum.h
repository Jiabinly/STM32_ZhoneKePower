#ifndef _CHECKSUM
#define _CHECKSUM

#define CheckSum_OK			1           //״̬OK
#define CheckSum_ERR    0           //״̬ERROR
unsigned char RxCheckSum(unsigned char *ptr);
void TxCheckSum(unsigned char *ptr);
#endif
