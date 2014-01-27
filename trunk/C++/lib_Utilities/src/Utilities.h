/*
 * Utilities.h
 * visualhhk
 *  Created on: 2010. 6. 10.
 *      Author: KDT
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_
#include <iostream>
#include <string>
#include "stdlib.h"
#include "stdarg.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <sys/ioctl.h>
#include <math.h>
#include <ctype.h>


#include "Defined.h"
#include "TypeDef.h"
using namespace std;


/*
 *
 */
 class Utilities {

private:
	//연산하기위한...필요한함수들...
	//format  123*123   123+123 123/123  123-213
	double calcBasic(const char * expresion );
	double calcOperator(const char * expresion );

public:
	Utilities();
	virtual ~Utilities();




	/////////////String Parsing ///////////////////////
	//문자열 중에[ ]  사이에 싸여있는 숫자를 리턴합니다.
	int u_String_getArrayNum(const char * str);

	//문자열 중에  인자값으로 넘어온 문자열이 몇개가있는지 찾습니다.
	int u_String_GetSplitCount(const char * str, char splitChar);

	//문자열 중에  인자값으로 넘어온 문자열이 로 쪼갰을때  몇번째  조각을 돌려줍니다.. 앞부분 없으면 그냥그대로를 rStr에 넣어줌
	bool u_String_GetSplitStrF(const char * str, char splitChar, int splitNum, char * rStr);

	//문자열 중에  인자값으로 넘어온 문자열이 로 쪼갰을때  몇번째  조각을 돌려줍니다.. 뒷부분 없으면 그냥 그대로를 rStr에넣어줌
	bool u_String_GetSplitStrB(const char * str, char splitChar, int splitNum, char * rStr);

	//String to hex  Split 캐릭터로 붙쳐진 문자열을. Hex로  변환해서..12,32,55,22 ->rByteArry[0] ...[1] [2]. retur arry length
	int u_String_Splite_To_Hex(const char * str, char *splitChar,byte rByteArry[]);


	//String 공백을 제거한 스트링을 반환한다.//공백 문자(공백, 개행('\n'), 종이넘기('\f'), 탭('\t', '\v'), 복귀('\r') 문자인지를 판별합니다.
	void u_String_SpaceDelete(char * str);

	//String 공백을 제거,또한 5+123(1+23)123/123  이런 수식을 5+123*(1+23)*123/123  괄호 앞뒤에 *기호 넣어준다
	void u_String_MakeExpresion(const char * str,char * rstr);

	//String  prefix 와 postfix 사이의 문자열을반환한다.
	bool u_String_between(const char * str,const char prefix, const char postfix,char *rstr);

	//String  prefix 와 postfix 사이의 문자열을반환한다.  하지만 위와다르게  최상위 쌍을 찾습니다.
	bool u_String_betweenPair(const char * str,const char prefix, const char postfix,char *rstr);






	/////////////////Debug////////////////////
	void Debug(char* format, ...);
	void Debug(long size,const byte *buffer,char* format, ...);




	//time......
	char * getLongTime(char* dateTimeBuffer);
	long getLongFullTime();



	//System....
	//sencond
	void SleepH(long second);
	//milliseconds   1000 / 1 seconds
	void sleepH(long millisecond);
//	/microseconds    1000000 / 1 seconds
	void usleepH(long micorosecond);


	//math
	bool eval(const char * expresion ,double *rval);












	//communication
	//utility
	void trace(void *buffer,long size,char* title);
	//length 1바이트 로해서 길이   Groupsize = 몇개를 한묶음으로 오더링할것인지
	int groupByteOrder(void * startAddr , int groupSize, int length);//바이트오더링
	// 설명 : CRC값 생성	// 인자 : 대상데이이터, 데이터길이	// 반환 : short 형 CRC
	unsigned short int  makeCRC(unsigned char *puchMsg,  unsigned short usDataLen);






};
#endif /* UTILITIES_H_ */
