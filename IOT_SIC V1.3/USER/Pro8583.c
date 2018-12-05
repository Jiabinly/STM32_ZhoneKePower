

#include "Pro8583.h"

uint8_t Data_Recv[512]={0};
uint8_t PayloadBuf[512]={0};

FIELDS m_fields;

int bitLen8583[] = {
		2, 1, 1, 20, 6, 4, 1, 10,
		20, 6, 2, 10, 1, 8, 10, 4,
		8, 2, 1, 6, 8, 10, 10, 20,
		50, 80, 120, 200, 300, 1000, 1200, 1400
	};

My8583_ATTR my8583[LEN_OF_8583] ={
	{LEN_FIELD0, FIX_LEN, R_BCD,2,m_fields.field0},    /* 0 -- CMD */
	{LEN_FIELD1, FIX_LEN, R_BCD,1,m_fields.field1},    /* 1 --Result */
	{LEN_FIELD2, FIX_LEN, R_BCD,1,m_fields.field2},    /*2 --�˿ں�*/
	{LEN_FIELD3, FIX_LEN, R_BCD,20,m_fields.field3},   /*3 --������*/
	{LEN_FIELD4, FIX_LEN, R_BCD,6,m_fields.field4},    /*4 --�汾��*/
	{LEN_FIELD5, FIX_LEN, R_BCD,4,m_fields.field5},    /*5 --����ѹ/�����/�������/����ǰIC�����*/
	{LEN_FIELD6, FIX_LEN, R_BCD,1,m_fields.field6},    /*6 -- ���ö˿���/TYPE/״̬/���� */
	{LEN_FIELD7, FIX_LEN, R_BCD,10,m_fields.field7},   /*7 --�汾*/
	{LEN_FIELD8, FIX_LEN, R_BCD,20,m_fields.field8},   /*8 --IC����/��Ʒ����*/
	{LEN_FIELD9, FIX_LEN, R_BCD,6,m_fields.field9},    /*9 --�˿ں�/Ӳ���汾��*/
	{LEN_FIELD10, FIX_LEN, R_BCD,2,m_fields.field10},  /*10 --������/�����ͣ�ļ��ʱ��/�����ͣ�ļ��ʱ��/CSQ*/
	{LEN_FIELD11, FIX_LEN, R_BCD,10,m_fields.field11}, /*11*/
	{LEN_FIELD12, FIX_LEN, R_BCD,1,m_fields.field12},  /*12*/
	{LEN_FIELD13, FIX_LEN, R_BCD,8,m_fields.field13},  /*13*/
	{LEN_FIELD14, FIX_LEN, R_BCD,10,m_fields.field14}, /*14 */
	{LEN_FIELD15, FIX_LEN, R_BCD,4,m_fields.field15},  /*15 --��ǰʱ��/��繦��*/
	{LEN_FIELD16, FIX_LEN, R_BCD,8,m_fields.field16},  /*16 --����ܹ���*/
	{LEN_FIELD17, FIX_LEN, R_BCD,2,m_fields.field17},  /*17 --�����ͣ�ļ��ʱ��/BER*/
	{LEN_FIELD18, FIX_LEN, R_BCD,1,m_fields.field18},  /*18 -- ���ر������ʱ��/��״̬*/
	{LEN_FIELD19, FIX_LEN, R_BCD,6,m_fields.field19},  /*19 --ϵͳ��������ʱ��*/
	{LEN_FIELD20, FIX_LEN, R_BCD,8,m_fields.field20},  /*20*/
	{LEN_FIELD21, FIX_LEN, R_BCD,10,m_fields.field21}, /*21*/
	{LEN_FIELD22, FIX_LEN, R_BCD,10,m_fields.field22}, /*22*/
	{LEN_FIELD23, FIX_LEN, R_BCD,20,m_fields.field23}, /*23 -- SIM����ʶ���ʶ*/
	{LEN_FIELD24, FIX_LEN, R_BCD,50,m_fields.field24}, /*24 -- �˿�����/�ƶ��豸ʶ����*/
	{LEN_FIELD25, FIX_LEN, R_BCD,80,m_fields.field25}, /*25*/
	{LEN_FIELD26, FIX_LEN, R_BCD,120,m_fields.field26}, /*26 -- �˿�����/�˿�״̬*/
	{LEN_FIELD27, FIX_LEN, R_BCD,200,m_fields.field27}, /*27 -- ����ж˿�����*/
	{LEN_FIELD28, FIX_LEN, R_BCD,300,m_fields.field28}, /*28*/
	{LEN_FIELD29, FIX_LEN, R_BCD,1000,m_fields.field29}, /*29*/
	{LEN_FIELD30, FIX_LEN, R_BCD,1200,m_fields.field30}, /*30*/
	{LEN_FIELD31, FIX_LEN, R_BCD,1400,m_fields.field31}, /*31*/
	
};

uint8_t  Cut_Server_Payload(uint8_t recv[])
{
	char buf[32]={0} ;
	uint16_t PayloadSize = 0;
	
	memset(PayloadBuf,0, strlen((char *)PayloadBuf) );
	memcpy(Data_Recv, recv, strlen((char *)recv)+1);
	printf("Data_Recv = %s\r\n",Data_Recv );
	memset(recv,0, strlen((char *) recv) );
	//+CLOUDPUBLISH: 11015,/Lk2upKmgG0w/500000000051/get,8,88990011
	sscanf((char *)Data_Recv, "%*[^ ] %*[^/]/%*[^/]/%*[^/]/get,%[^,],%s",buf,PayloadBuf);//buf = '8',PayloadBuf ="88990011"
	PayloadSize = atoi((char *)buf);
//	printf("PayloadBuf size = %d byte\r\n",PayloadSize);
	printf("PayloadBuf = %s\r\n",PayloadBuf);
	if(strlen((char *)PayloadBuf) == PayloadSize )//���ATָ��ͳ�Ƴ������������������ȡ�ɹ�
	{
		printf("PayloadBuf size = %d byte\r\n",PayloadSize);
	}
	else                                          //������Ȳ����������û��������ȴ��ط�
	{
		memset(PayloadBuf,0, strlen((char *)PayloadBuf) );
	}
	return 0;
}



//���н���
uint8_t Rece_Data_Proc(void)
{
		int i = 0;
		int msgType = -1;
		long msgId = -1;
		int len = 0;
		char tempbuf[512] = {0};
		char mapData[512] = {0};
		uint8_t check = 0;
		uint8_t devicelen = 0;
		
		
		if (strlen((char* )PayloadBuf) == 0)	
		{
			return 0;
		}
		
		//У������
		for ( i=0; i<strlen((char* )PayloadBuf); i++) 
		{
				check ^= PayloadBuf[i];
		}
		if (check != 0)	
		{
			return 0;
		}	//���У��ʧ��
		
	
		//�������ַ�ת��Ϊbyte������ֵ��
		for(i=0; i<strlen((char* )PayloadBuf);i++)
		{
			PayloadBuf[i] = (uint8_t)atoi((char *)PayloadBuf[i]);
		}
		
		//��ȡ��β֡
		 mid(tempbuf,(char*)PayloadBuf, strlen((char* )PayloadBuf)-5,1);
		 printf("tempbuf_midcut = %s\r\n",tempbuf);
		
//		//�ж������Ƿ�������
//		if(strlen((char* )PayloadBuf)%16 == 0)
//		{
//			return 0;
//		}
		
			//��֤8483������ȷ��
			if(tempbuf[0] == 0x05)
			{
				len = (tempbuf[1] < 0 ? 256 + tempbuf[1] : tempbuf[1]) * 256 + (tempbuf[2] < 0 ? 256 + tempbuf[2] : tempbuf[2]);
				for (i=0; i<len; i++) 
				{
					check ^= tempbuf[1 + i];
				}
				if (check != 0)	return 0;
				
				msgType = (tempbuf[3] < 0 ? 256 + tempbuf[3] : tempbuf[3])  * 256 + (tempbuf[4] < 0 ? 256 + tempbuf[4] : tempbuf[4]);
				msgId = (tempbuf[5] < 0 ? 256 + tempbuf[5] : tempbuf[5]) * 16777216
					+ (tempbuf[6] < 0 ? 256 + tempbuf[6] : tempbuf[6]) * 65536
					+ (tempbuf[7] < 0 ? 256 + tempbuf[7] : tempbuf[7]) * 256
					+ (tempbuf[8] < 0 ? 256 + tempbuf[8] : tempbuf[8]);
				right(mapData,tempbuf, strlen((char* )tempbuf)-9);
				printf("mapData = %s\r\n",mapData);
				
				if(create8583((uint8_t*)mapData)== 0)   //����λͼ��DATA  
				{
					return 0;
				}
			}
//			if (map == null)	return;
//			
//			action = map.getInteger(0);	//0λΪaction
//			if (action == null)	return;
//		} else {
//			return;
		
	
	return 0;
	
}



//���з�װ
uint8_t Send_Data_Proc(void)
{
	
	return 0;
	
}


//λͼ����
BOOLEAN create8583(uint8_t * data)
{		
		int i = 0;
		int len = 0;
//		char map_string[4] ={0};
//		left(map_string,data,4);//��ȡλͼ
//		uint8_t Only_Data[1028] = {0};
		int map = (data[0] < 0 ? 256 + data[0] : data[0]) * 16777216
					+ (data[1] < 0 ? 256 + data[1] : data[1]) * 65536
					+ (data[2] < 0 ? 256 + data[2] : data[2]) * 256
					+ (data[3] < 0 ? 256 + data[3] : data[3]);
		for (i=0; i<32; i++)
		{
			if ((map & (0x00000001 << (31 - i))) != 0) {
				
				len += bitLen8583[i];
				
			}
		}
		
		if (len != strlen((char* )PayloadBuf) - 5) 
		{	//��֤��ͨ��
			return 0;
		}
//		right((char *)Only_Data,(char *)data, len);
		for (i=0; i<32; i++)
		{
			if ((map & (0x00000001 << (31 - i))) != 0) 
			{
				left((char*)my8583[i].field,(char *)data, bitLen8583[i]);
				len += bitLen8583[i];
			}
		}
		
		return 1;
}


//�ַ���ת��������
int htoi(char s[])
{
		int i = 0;
    int start=0,end;
    int inte=0;
		int j=0;
	
    if(s[0]=='0'&&(s[1]=='x'||s[1]=='X'))
		{
        start=2;
    }
    while(s[j++]!=' ')
		{
    }
    end=j-2;
    for( i=1;end>=start;i=i*16)
		{
        if(s[end]>='0'&&s[end]<='9')
				{
            inte=inte+(s[end]-'0')*i;
        }
				else if(s[end]>='a'&&s[end]<='f')
				{
            inte=inte+(s[end]-'a'+10)*i;
        }
				else if(s[end]>='A'&&s[end]<='F')
				{
            inte=inte+(s[end]-'A'+10)*i;
        }
        end--;
    }
    return inte;
}


/*���ַ�������߽�ȡn���ַ�*/
char * left(char *dst,char *src, int n)
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*�б�Ҫ�𣿷ǳ��б�Ҫ*/
    return dst;
}

/*���ַ������м��ȡn���ַ�*/
char * mid(char *dst,char *src, int n,int m) /*nΪ���ȣ�mΪλ��*/
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    /*�ӵ�m�������*/
    if(m<0) m=0;    /*�ӵ�һ���_ʼ*/
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*�б�Ҫ�𣿷ǳ��б�Ҫ*/
    return dst;
}

/*���ַ������ұ߽�ȡn���ַ�*/
char * right(char *dst,char *src, int n)
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len;
    p += (len-n);   /*���ұߵ�n���ַ��_ʼ����0�������ǳ��ɰ�*/
    while(*(q++) = *(p++));
    return dst;
}
