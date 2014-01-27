#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

#include "win_serial_api.h"



#pragma pack(1)
struct Sendinfo{
	unsigned char Addr;//장비 아이디
	unsigned char FunctionCode;//펑션코드
	unsigned short StartAddr;//시작주소
	unsigned short DataCount;//저장할데이터수
	unsigned short ByteCount;//저장할 바이트수

	unsigned short DataWhite;
	unsigned short DataRead;
	unsigned short DataGreen;
	unsigned short DataBlue;

	unsigned short CRC;


};


static win_serial_api_t *wsa = NULL;
static HANDLE  events[MAXIMUM_WAIT_OBJECTS];
static DWORD   active_events;

static int log_func( void *wsa, const char *log_str );
int myOpen(char* port_name,int Comspeed);
int mySend(Sendinfo sendData,int size);
int myReceive(char * read_buff);
int myClose();


