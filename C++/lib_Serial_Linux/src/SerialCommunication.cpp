/*
 * SerialCommunication.cpp
 *
 *  Created on: 2010. 5. 12.
 *      Author: KDT
 */

#include "SerialCommunication.h"

SerialCommunication::SerialCommunication()
{
//	Util.Debug("welCom Linux Serial Communication Class Constructors");
	this->Fd =0;
	this->ComType=0;
	this->ComSpeed=0;
	this->PortOpend=0;
	memset(this->PortPath,0,RS485PORTPATH_MAXSIZE);
}
SerialCommunication::SerialCommunication(char * PortName)
{
	Util.Debug("welCom Linux Serial Communication Class Constructors(char * PortName)");
	this->Fd =0;
	this->open(PortName);
}

SerialCommunication::~SerialCommunication()
{
//	Util.Debug("GoodBye Linux Serial Communication Class Destructor");
}

bool SerialCommunication::open(char *portPath)
{

	strcpy(this->PortPath,portPath);

	Fd = ::open( this->PortPath, O_RDWR | O_NOCTTY | O_NONBLOCK );
	if( Fd < 0 ){
		Util.Debug("Fail Open Port (Return 0)  %s",this->PortPath);
		this->PortOpend=0;
		return false;
	}
	Util.Debug("Success Open Port  %s  fd=(%d) ",this->PortPath , Fd );
		this->PortOpend=1;
	return true;
}

bool SerialCommunication::setting(int comType,int comSpeed)
{

	this->ComType 	= comType;
	this->ComSpeed = comSpeed;
	if(this->ComType != 485 && this->ComType != 232 )
		this->PortOpend=false;

	if(this->PortOpend==false){
		Util.Debug("NO Open Port  Flag(%d)",this->PortOpend);
		return -1;
	}

	struct termios newtio;
	memset( &newtio, 0, sizeof(newtio) );



	switch( this->ComSpeed )
	{
		case 1200  : newtio.c_cflag = B1200  ; break;
		case 1800  : newtio.c_cflag = B1800  ; break;
		case 2400  : newtio.c_cflag = B2400  ; break;
		case 4800  : newtio.c_cflag = B4800  ; break;
		case 9600  : newtio.c_cflag = B9600  ; break;
		case 19200 : newtio.c_cflag = B19200 ; break;
		case 38400 : newtio.c_cflag = B38400 ; break;
		case 57600 : newtio.c_cflag = B57600 ; break;
		case 115200: newtio.c_cflag = B115200; break;
        default    : newtio.c_cflag = B9600  ; break;
	}

		newtio.c_cflag |= CS8;                                      // (4) 데이터 비트가 8bit
		newtio.c_cflag |= CLOCAL | CREAD;                           // (5) 쓰기는 기본, 읽기도 가능하게
		newtio.c_oflag 		= 0;
		newtio.c_lflag 		= 0;
		newtio.c_cc[VTIME] 	= 0;
		newtio.c_cc[VMIN] 	= 0;


		if( this->ComType == 232 )
		{
		   newtio.c_iflag = 0;                                         // (6) 패리티 사용 안함
		}
		else if ( this->ComType == 485 )
		{
			newtio.c_iflag = IGNPAR;
		}


	   tcflush (Fd, TCIFLUSH );
	   tcsetattr(Fd, TCSANOW, &newtio );
		int RTS = 1000/1800;
		ioctl( Fd, TIOCMBIS, &RTS );

	   Util.Debug("Success Speed Setting  (%d)",this->ComSpeed);
	   return 1;
}


int SerialCommunication::getFd()
{
	return this->Fd;
}
bool SerialCommunication::isOpen()
{
	return this->PortOpend;
}

void SerialCommunication::trace( void * buffer,long size,char * title){


	unsigned char* bufferView = (unsigned char* )buffer;

	char logTime[32] = {0, };
	memset(logTime, 0x00, sizeof(logTime));
	getLongTime(logTime);
//	printf("\n======================\n");
	printf("%s  %s  length(%d)",logTime,title,size);
	printf("\n-------------------------------------------------");

	int rownum=0;
	for(long i = 0 ; i < size ; i++){

			if( i % 16 == 0){
				rownum++;
				printf("\n");
				//printf("%3d]",rownum);
			}

		printf("%02X",bufferView[i]);
		printf("  ");
	}
	printf("\n");

}


char * SerialCommunication::getLongTime(char *datetimebuff){
   struct timeb rawtime;
   struct tm * timeinfo;

   ftime(& rawtime);
   timeinfo = localtime(&rawtime.time);

   sprintf(datetimebuff, "%04d-%02d-%02d %02d:%02d:%02d:%03d",
		timeinfo->tm_year + 1900,
		timeinfo->tm_mon + 1,
		timeinfo->tm_mday,
		timeinfo->tm_hour,
		timeinfo->tm_min,
		timeinfo->tm_sec,
		rawtime.millitm );

   return datetimebuff;
}


long SerialCommunication::getLongFullTime()
{
	   struct timeb rawtime;
	   struct tm * timeinfo;

	   char temp [50]={0,};
	   ftime(& rawtime);
	   timeinfo = localtime(&rawtime.time);
   sprintf(temp, "%02d%02d%02d%03d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,rawtime.millitm);

   return atol(temp);
}



int SerialCommunication::write(void *sendBuffer,int writeSize){

	if(this->PortOpend<=0){
		Util.Debug("NO Open Port  Flag(%d)",this->PortOpend);
		return -1;
	}
	trace(sendBuffer,writeSize,"Write");
	int writeLength=0;
	if( this->ComType == 232 )
	{
		writeLength = ::write(Fd, sendBuffer, writeSize);
	}
	else if (this->ComType == 485 )
	{
		int CK_EMPTY, CTL_RTS;
		tcflush( Fd, TCIFLUSH ); 												//initialization system buffer

		CTL_RTS = TIOCM_RTS;
		ioctl( Fd , TIOCMBIS, &CTL_RTS ); 										//RTS High Control

		writeLength = ::write(Fd, sendBuffer, writeSize);

		if( writeLength == -1 )
		{
			Util.Debug("K_SERIAL::sendData[Send Error]");
			CTL_RTS = TIOCM_RTS;							   					//RTS Low Control
			ioctl( Fd, TIOCMBIC, &CTL_RTS);
			return -1;
		}

		while( 1 ) 																//Wait For Sending Buffer Empty
		{
			usleep(1);

			int rtn = ioctl( Fd, TIOCSERGETLSR, &CK_EMPTY );

			if( CK_EMPTY )	break;

			if( rtn < 0 )
			{
				Util.Debug(":sendData[Empty Check Error=%d]");
				break;
			}
		}

		CTL_RTS = TIOCM_RTS;							   						//RTS Low Control
		ioctl( Fd, TIOCMBIC, &CTL_RTS);
	}

	return writeLength;


}

//딜레이 시간은  시 분 초 밀리 이런식으로 적어주시면됨니다.
//timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,rawtime.millitm
int SerialCommunication::read(void *containerBuffer,int readSize,int delayTime){
	if(this->PortOpend<=0){
		Util.Debug("NO Open Port  Flag(%d)",this->PortOpend);
		return -1;
	}

	int rCnt = 0;
	char *rData = (char*)containerBuffer;

	long startTime = getLongFullTime();
//	cout<<"StartTime::" <<startTime<<"   DeleayTime::"<<delayTime<<endl;
	while(1)
	{
		usleep(1000);
		int rc = ::read(Fd, rData+rCnt, readSize-rCnt);
//		cout << "read fd = " << fd <<"    rCount:"<<rCnt<<"  RcvLen:"<<rc<<endl;

		if(rc==-1)
		{

//			cout << "errno" << errno << endl;
			return -1;
		}


		rCnt += rc; //다음포인터 지정


		//timeout
		if(getLongFullTime()-startTime >=delayTime){
//			cout<<"this Time::"<<getLongFullTime();
			char TimeoutString[256] = {0, };
			sprintf(TimeoutString, "Read_WantReadSize(%d)",readSize);
			if(rCnt>1)
			trace(containerBuffer,rCnt,TimeoutString);
			return rCnt;
		}

		if(rCnt>=readSize){
			trace(containerBuffer,rCnt,"Read Success");
			return rCnt;
		}
	}
}




//add 이사님   담아올버퍼랑    최마지막에 받은 바이트 다음 기다리는 시간 즉  delayTiem동안 다음비트가 안오면 끝!
int SerialCommunication::readTime(void *containerBuffer,int bufferSize,int delayTime){
	if(this->PortOpend<=0){
		Util.Debug("NO Open Port  Flag(%d)",this->PortOpend);
		return -1;
	}

	int rCnt = 0;
	char *rData = (char*)containerBuffer;

	long startTime = getLongFullTime();
//	cout<<"StartTime::" <<startTime<<"   DeleayTime::"<<delayTime<<endl;
	while(1)
	{
		usleep(100);  //1mm 보다 0이 하나더적다.
		int rc = ::read(Fd, rData+rCnt, bufferSize);
//		cout << "read fd = " << fd <<"    rCount:"<<rCnt<<"  RcvLen:"<<rc<<endl;

		if(rc==-1)
		{

//			cout << "errno" << errno << endl;
			return -1;
		}



		rCnt += rc; //다음포인터 지정

		if(rc>0) //뭐라도 하나라도 받은게있다면  최신 시간으로 바꿈
		 startTime = getLongFullTime();


		//timeout
		if(getLongFullTime()-startTime >=delayTime){
			if(rCnt>0)
			trace(containerBuffer,rCnt,"Read Success");
			return rCnt;
		}

	}
}

int SerialCommunication::groupByteOrder(void * startAddr , int groupSize, int length){

	if(length<groupSize)
		return -1;

	int LoopNumCeil=ceil((float)length/(float)groupSize);
	int LoopNum=length/groupSize;
	if(LoopNumCeil != LoopNum){
		return -1;
	}

//	cout<<"GroupSize "<<groupSize << "Length  "<<length<<"Loop Num"<<LoopNum<<endl;

	char temp[1000]={0,};

	char  * startP = (char*)startAddr;
	for (int i = 0; i < LoopNum; ++i) {
		memcpy(&temp[i*groupSize],&startP[length-groupSize*(i+1)],groupSize);
//		cout<<i*groupSize<<"     "<< length-groupSize*(i+1)<<endl;
	}
	memcpy(startAddr,temp,length);

	return 1;
}

//---------------------------------------------------------------------------
// 설명 : CRC값 생성
// 인자 : 대상데이이터, 데이터길이
// 반환 : short 형 CRC
//---------------------------------------------------------------------------
unsigned short int  SerialCommunication::makeCRC(unsigned char *puchMsg,  unsigned short usDataLen)
{

	static unsigned char auchCRCHi[] =
	{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
	};

	//---------------------------------------------------------------------------
	static char auchCRCLo[] =
	{
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2,
	    0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
	    0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
	    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
	    0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
	    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6,
	    0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
	    0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
	    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE,
	    0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
	    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA,
	    0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
	    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
	    0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62,
	    0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
	    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE,
	    0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
	    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
	    0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76,
	    0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
	    0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
	    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
	    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A,
	    0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86,
	    0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
	};



	unsigned char uchCRCHi = 0xFF;
	unsigned char uchCRCLo = 0xFF;
	unsigned uIndex ;

	while (usDataLen--)
    {
		uIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
//	return (uchCRCHi << 8 | uchCRCLo);
	return ( uchCRCLo << 8 |  uchCRCHi );

}

