/*
 * SerialCommunication.h
 *
 *  Created on: 2010. 5. 12.
 *      Author: KDT
 */

#ifndef SERIALCOMMUNICATION_H_
#define SERIALCOMMUNICATION_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/time.h>
#include <utime.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/ioctl.h>
#include <math.h>
#include "Hinclude.h"
using namespace std;


/*
 *
 */
class SerialCommunication
{
protected:
	Utilities Util;
	int Fd;
	int ComType;
	int ComSpeed;
	bool PortOpend;
	char PortPath[RS485PORTPATH_MAXSIZE];

public:
	SerialCommunication();
	SerialCommunication(char * PortName);
	virtual ~SerialCommunication();
	bool open(char * portPath);
	bool setting(int comType,int comSpeed);
	int getFd();
	bool isOpen();
	int write(void *sendBuffer,int writeSize);
	int read(void *containerBuffer,int readSize,int delayTime);




	//add 이사님   담아올버퍼랑    최마지막에 받은 바이트 다음 기다리는 시간 즉  delayTiem동안 다음비트가 안오면 끝!
	int readTime(void *containerBuffer,int bufferSize, int delayTime);


	//utility
	void trace(void *buffer,long size,char* title);
	char * getLongTime(char* dateTimeBuffer);
	long getLongFullTime();
	//length 1바이트 로해서 길이   Groupsize = 몇개를 한묶음으로 오더링할것인지
	int groupByteOrder(void * startAddr , int groupSize, int length);//바이트오더링
	// 설명 : CRC값 생성	// 인자 : 대상데이이터, 데이터길이	// 반환 : short 형 CRC
	unsigned short int  makeCRC(unsigned char *puchMsg,  unsigned short usDataLen);
};

#endif /* SERIALCOMMUNICATION_H_ */
