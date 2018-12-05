#ifndef __PRO8583_H
#define __PRO8583_H

#include "stm32f10x.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "includes.h"		

#define LEN_OF_MAX_BUFFER        1400    /*定义data结构中最长的缓冲区*/
#define LEN_OF_8583        32            /*定义打他的长度*/

#define UNUSED               0
#define L_BCD                1    /*左对齐BCD码*/
#define L_ASC                2    /*左对齐ASC码*/
#define R_BCD                3    
#define R_ASC                4
#define D_BIN                5
#define OVER                 6

#define FIX_LEN              1    /*(FIX_LEN指由8583中的长度决定该域的长度)*/
#define LLVAR_LEN            2    /*(LLVAR_LEN 00~99)*/
#define LLLVAR_LEN           3    /*(LLLVAR_LEN 00~999)*/

#define LEN_FIELD0            2
#define LEN_FIELD1            1
#define LEN_FIELD2            1
#define LEN_FIELD3            20
#define LEN_FIELD4            6
#define LEN_FIELD5            4
#define LEN_FIELD6            1
#define LEN_FIELD7            10
#define LEN_FIELD8            20
#define LEN_FIELD9            6
#define LEN_FIELD10           2
#define LEN_FIELD11           10
#define LEN_FIELD12            1
#define LEN_FIELD13            8
#define LEN_FIELD14           10
#define LEN_FIELD15            4
#define LEN_FIELD16            8
#define LEN_FIELD17            2
#define LEN_FIELD18            1
#define LEN_FIELD19            6
#define LEN_FIELD20            8
#define LEN_FIELD21            10
#define LEN_FIELD22            10
#define LEN_FIELD23            20
#define LEN_FIELD24            50
#define LEN_FIELD25            80
#define LEN_FIELD26            120
#define LEN_FIELD27            200
#define LEN_FIELD28            300
#define LEN_FIELD29            1000
#define LEN_FIELD30            1200
#define LEN_FIELD31            1400



typedef struct {
    uint8_t    field0[LEN_FIELD0];
    uint8_t    field1[LEN_FIELD1];
    uint8_t    field2[LEN_FIELD2];
    uint8_t    field3[LEN_FIELD3];
    uint8_t    field4[LEN_FIELD4];
    uint8_t    field5[LEN_FIELD5];
    uint8_t    field6[LEN_FIELD6];
    uint8_t    field7[LEN_FIELD7];
    uint8_t    field8[LEN_FIELD8];
    uint8_t    field9[LEN_FIELD9];
    uint8_t    field10[LEN_FIELD10];
    uint8_t    field11[LEN_FIELD11];
    uint8_t    field12[LEN_FIELD12];
    uint8_t    field13[LEN_FIELD13];
    uint8_t    field14[LEN_FIELD14];
    uint8_t    field15[LEN_FIELD15];
    uint8_t    field16[LEN_FIELD16];
    uint8_t    field17[LEN_FIELD17];
    uint8_t    field18[LEN_FIELD18];
    uint8_t    field19[LEN_FIELD19];
    uint8_t    field20[LEN_FIELD20];
    uint8_t    field21[LEN_FIELD21];
    uint8_t    field22[LEN_FIELD22];
    uint8_t    field23[LEN_FIELD23];
    uint8_t    field24[LEN_FIELD24];
    uint8_t    field25[LEN_FIELD25];
    uint8_t    field26[LEN_FIELD26];
    uint8_t    field27[LEN_FIELD27];
    uint8_t    field28[LEN_FIELD28];
    uint8_t    field29[LEN_FIELD29];
    uint8_t    field30[LEN_FIELD30];
    uint8_t    field31[LEN_FIELD31];
   
}FIELDS;

typedef struct {
 uint16_t len_max;
 uint8_t len_type;
 uint8_t field_type;
 uint16_t field_len;
 uint8_t * field;
}My8583_ATTR;


uint8_t  Cut_Server_Payload(uint8_t recv[]);
uint8_t Rece_Data_Proc(void);
uint8_t Send_Data_Proc(void);
uint8_t create8583(uint8_t * data);
char * right(char *dst,char *src, int n);
char * mid(char *dst,char *src, int n,int m);
char * left(char *dst,char *src, int n);
int htoi(char s[]);



#endif
