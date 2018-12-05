

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
	{LEN_FIELD2, FIX_LEN, R_BCD,1,m_fields.field2},    /*2 --端口号*/
	{LEN_FIELD3, FIX_LEN, R_BCD,20,m_fields.field3},   /*3 --订单号*/
	{LEN_FIELD4, FIX_LEN, R_BCD,6,m_fields.field4},    /*4 --版本号*/
	{LEN_FIELD5, FIX_LEN, R_BCD,4,m_fields.field5},    /*5 --充电电压/最大功率/错误代码/操作前IC卡余额*/
	{LEN_FIELD6, FIX_LEN, R_BCD,1,m_fields.field6},    /*6 -- 可用端口数/TYPE/状态/动作 */
	{LEN_FIELD7, FIX_LEN, R_BCD,10,m_fields.field7},   /*7 --版本*/
	{LEN_FIELD8, FIX_LEN, R_BCD,20,m_fields.field8},   /*8 --IC卡号/产品名称*/
	{LEN_FIELD9, FIX_LEN, R_BCD,6,m_fields.field9},    /*9 --端口号/硬件版本号*/
	{LEN_FIELD10, FIX_LEN, R_BCD,2,m_fields.field10},  /*10 --充电电流/充电自停的检测时间/充电自停的检测时间/CSQ*/
	{LEN_FIELD11, FIX_LEN, R_BCD,10,m_fields.field11}, /*11*/
	{LEN_FIELD12, FIX_LEN, R_BCD,1,m_fields.field12},  /*12*/
	{LEN_FIELD13, FIX_LEN, R_BCD,8,m_fields.field13},  /*13*/
	{LEN_FIELD14, FIX_LEN, R_BCD,10,m_fields.field14}, /*14 */
	{LEN_FIELD15, FIX_LEN, R_BCD,4,m_fields.field15},  /*15 --当前时间/充电功率*/
	{LEN_FIELD16, FIX_LEN, R_BCD,8,m_fields.field16},  /*16 --充电总功率*/
	{LEN_FIELD17, FIX_LEN, R_BCD,2,m_fields.field17},  /*17 --充电自停的检测时间/BER*/
	{LEN_FIELD18, FIX_LEN, R_BCD,1,m_fields.field18},  /*18 -- 空载保护检测时间/卡状态*/
	{LEN_FIELD19, FIX_LEN, R_BCD,6,m_fields.field19},  /*19 --系统连续运行时间*/
	{LEN_FIELD20, FIX_LEN, R_BCD,8,m_fields.field20},  /*20*/
	{LEN_FIELD21, FIX_LEN, R_BCD,10,m_fields.field21}, /*21*/
	{LEN_FIELD22, FIX_LEN, R_BCD,10,m_fields.field22}, /*22*/
	{LEN_FIELD23, FIX_LEN, R_BCD,20,m_fields.field23}, /*23 -- SIM卡的识别标识*/
	{LEN_FIELD24, FIX_LEN, R_BCD,50,m_fields.field24}, /*24 -- 端口数据/移动设备识别码*/
	{LEN_FIELD25, FIX_LEN, R_BCD,80,m_fields.field25}, /*25*/
	{LEN_FIELD26, FIX_LEN, R_BCD,120,m_fields.field26}, /*26 -- 端口数据/端口状态*/
	{LEN_FIELD27, FIX_LEN, R_BCD,200,m_fields.field27}, /*27 -- 充电中端口数据*/
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
	if(strlen((char *)PayloadBuf) == PayloadSize )//如果AT指令统计长度与包长度相符，则截取成功
	{
		printf("PayloadBuf size = %d byte\r\n",PayloadSize);
	}
	else                                          //如果长度不相符，清除该缓存区，等待重发
	{
		memset(PayloadBuf,0, strlen((char *)PayloadBuf) );
	}
	return 0;
}



//下行解析
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
		
		//校验数据
		for ( i=0; i<strlen((char* )PayloadBuf); i++) 
		{
				check ^= PayloadBuf[i];
		}
		if (check != 0)	
		{
			return 0;
		}	//异或校验失败
		
	
		//将网络字符转换为byte整型数值；
		for(i=0; i<strlen((char* )PayloadBuf);i++)
		{
			PayloadBuf[i] = (uint8_t)atoi((char *)PayloadBuf[i]);
		}
		
		//截取首尾帧
		 mid(tempbuf,(char*)PayloadBuf, strlen((char* )PayloadBuf)-5,1);
		 printf("tempbuf_midcut = %s\r\n",tempbuf);
		
//		//判断数据是否脏数据
//		if(strlen((char* )PayloadBuf)%16 == 0)
//		{
//			return 0;
//		}
		
			//验证8483数据正确性
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
				
				if(create8583((uint8_t*)mapData)== 0)   //解析位图跟DATA  
				{
					return 0;
				}
			}
//			if (map == null)	return;
//			
//			action = map.getInteger(0);	//0位为action
//			if (action == null)	return;
//		} else {
//			return;
		
	
	return 0;
	
}



//上行封装
uint8_t Send_Data_Proc(void)
{
	
	return 0;
	
}


//位图解析
BOOLEAN create8583(uint8_t * data)
{		
		int i = 0;
		int len = 0;
//		char map_string[4] ={0};
//		left(map_string,data,4);//截取位图
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
		{	//验证不通过
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


//字符串转换成整数
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


/*从字符串的左边截取n个字符*/
char * left(char *dst,char *src, int n)
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*有必要吗？非常有必要*/
    return dst;
}

/*从字符串的中间截取n个字符*/
char * mid(char *dst,char *src, int n,int m) /*n为长度，m为位置*/
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    /*从第m个到最后*/
    if(m<0) m=0;    /*从第一个開始*/
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*有必要吗？非常有必要*/
    return dst;
}

/*从字符串的右边截取n个字符*/
char * right(char *dst,char *src, int n)
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len;
    p += (len-n);   /*从右边第n个字符開始。到0结束，非常巧啊*/
    while(*(q++) = *(p++));
    return dst;
}
