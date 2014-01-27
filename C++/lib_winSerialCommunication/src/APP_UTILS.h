//---------------------------------------------------------------------------
#ifndef APP_UTILSH
#define APP_UTILSH
//---------------------------------------------------------------------------
#include "MSG.h"




//get Sytem Device Path
int getDevPath(long COM_PRT, char *DEV_PATH);


bool isTimeOver(long timeout, long pre_date, long pre_time);

bool isTimeOver(long timeout, long pre_date, long pre_time, long cDate, long cTime);


void trace(int len, void *buff, char *format, ...);
void trace(char *format, ...);
/*
#ifdef __linux__
#define trace(len, buff, args...)  __trace(len, buff, __FILE__, __func__, __LINE__, #args )
#define trace(args...)             __trace( __FILE__, __func__, __LINE__, #args )
#else
#define trace(len, buff, args...)  __trace(len, buff, __FILE__, __FUNC__, __LINE__, #args )
#define trace(args...)             __trace( __FILE__, __FUNC__, __LINE__, #args )
#endif
*/


//CRC ������ �����
unsigned short int  makeCRC(unsigned char *puchMsg,  unsigned short usDataLen);


//---------------------------------------------------------------------------
// ���� : ����Ʈ ���� ��ȯ
// ���� :
// ��ȯ :
//---------------------------------------------------------------------------
void ntoh( char *src, char *des, int size);

#endif
