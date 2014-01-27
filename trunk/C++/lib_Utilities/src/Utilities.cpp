/*
 * Utilities.cpp
 *
 *  Created on: 2010. 6. 10.
 *      Author: KDT
 */

#include "Utilities.h"


Utilities::Utilities() {
	// TODO Auto-generated constructor stub

}

Utilities::~Utilities() {
	// TODO Auto-generated destructor stub
}



double Utilities::calcBasic(const char * expresion ){

	char exp[100]={0,};
	u_String_MakeExpresion(expresion,exp);



	if(!isalnum(exp[0]) &&  !exp[0] == '+' && !exp[0] == '-'){
		return 0;
	}

	double prefixNumber=atof(exp);
	double postfixNumber=0.0;

	char *ptr = NULL;
	ptr = index(exp,'+');
	if(ptr !=NULL){
		postfixNumber =atof(ptr+1);
//		Debug("aaaaaa  (%f) (%f)   =  (%f)",prefixNumber,postfixNumber,(double)(double)prefixNumber+(double)postfixNumber);
		return (double)(double)prefixNumber+(double)postfixNumber;
	}
	ptr = index(exp,'-');
	if(ptr !=NULL){
		postfixNumber =atof(ptr+1);
		return (double)(double)prefixNumber-(double)postfixNumber;
	}
	ptr = index(exp,'*');
	if(ptr !=NULL){
		postfixNumber =atof(ptr+1);
		return (double)(double)prefixNumber*(double)postfixNumber;
	}
	ptr = index(exp,'/');
	if(ptr !=NULL){
		postfixNumber =atof(ptr+1);
		return (double)(double)prefixNumber/(double)postfixNumber;
	}

 return (double)atof(exp);
}
double Utilities::calcOperator(const char * expresion ){


	char exp[100]={0,};
	u_String_MakeExpresion(expresion,exp);

	if(!isalnum(exp[0])){
		return 0;
	}

	Debug("[%s]   %s",__FUNCTION__,expresion);

	char tempexp[100]={'\0',};


//	Debug("strlength(%d)",strlen(exp));

	for (int firstFixNum = 0; firstFixNum < strlen(exp); ++firstFixNum) {
//		Debug("firstFixNum(%d )exp[firstFixNum](%c)  strlen(exp) (%d)",firstFixNum,exp[firstFixNum],strlen(exp));
		if(exp[firstFixNum]=='*' || exp[firstFixNum]=='/'){ 					//심볼찾음
//			Debug("welcom index %d",firstFixNum);
			memset(tempexp,0,100);
			int miniFixNum=0;
			for (int preFixNum = firstFixNum; preFixNum >= miniFixNum; --preFixNum) {					//뒤prefix 찾으로가자
				if(preFixNum == firstFixNum)continue;
///////////////////////////////////
				//맨끝에왔을때는 무조건 그쪽부터 카피하고     찾은 fix에서  지금께 심볼이면 하고  그리고 그 심볼이 . 아니여야하고   그전이 또 기호가 - + 가들어가면 안된다
				if (preFixNum==0 || ( !isdigit(exp[preFixNum])    && exp[preFixNum]!='.' &&  preFixNum>0 && exp[preFixNum-1]!='-'&& exp[preFixNum-1]!='+'  )) {		//prefix 찾자~!~ 찾았따.
						miniFixNum = preFixNum;

						int startIndex = preFixNum+1;
						if(preFixNum==0)  //0이면은 그지금이자리부터 시작해라.
						startIndex = preFixNum;


					    for (int cpyNum = 0; cpyNum < strlen(exp); ++cpyNum) {	//prefix 부터 ~postfix까지가져와라.

					    	if(startIndex+cpyNum > firstFixNum){							//해당 심볼 중단점을 지났다/
//								Debug("check");
					    		if(!isdigit(exp[startIndex+cpyNum ])  && exp[startIndex+cpyNum]!='.'){
					    			tempexp[cpyNum]='\0';				//끝내라
					    			firstFixNum=0;  //split 한후 바로 붙어있을경우있으니..훔..
//									Debug("ininin  %s [%c] (%d)(%d)",tempexp,exp[startIndex+cpyNum ],startIndex+cpyNum,firstFixNum);
					    			break;
					    		}
					    	}

					    	tempexp[cpyNum]=exp[startIndex+cpyNum];
//					    	Debug("outout(%d) %s  (%d)(%d)",cpyNum,tempexp,startIndex+cpyNum,firstFixNum);
					    }
				}
///////////////////////////////////
			}
			int tempexpLen = strlen(tempexp);
			double tempval = calcBasic(tempexp);
//			Debug("----  %s [%s]  val(%f)   firstFixNum(%d)",tempexp,exp,tempval,firstFixNum);
			char *splitptr = strstr(exp,tempexp);
			memset(splitptr,' ',tempexpLen);
			splitptr[0]='%';splitptr[1]='s';
			sprintf(tempexp,exp,"%.3f");

			sprintf(exp,tempexp,tempval);
			u_String_SpaceDelete(exp); //공백제거
		}
//		break;
	}

		Debug("calc!!   * /    %s",exp);







////////////////////////////////////조금바꼈음.-123 가나올수있으므로.//////////////////////////////////////////////


		for (int firstFixNum = 0; firstFixNum < strlen(exp); ++firstFixNum) {
		//		Debug("firstFixNum(%d )  strlen(exp) (%d)",firstFixNum,strlen(exp));
				if(exp[firstFixNum]=='+' || exp[firstFixNum]=='-'){ 					//심볼찾음
					memset(tempexp,0,100);
					int miniFixNum=0;
					for (int preFixNum = firstFixNum; preFixNum >= miniFixNum; --preFixNum) {					//뒤prefix 찾으로가자
						if(preFixNum == firstFixNum)continue;
		////////////////////////////문자다.///////

						//맨끝에왔을때는 무조건 그쪽부터 카피하고     찾은 fix에서  지금께 심볼이면 하고  그리고 그 심볼이 . 아니여야하고   그전이 또 기호가 - + 가들어가면 안된다
						if (preFixNum==0 || ( !isdigit(exp[preFixNum])    && exp[preFixNum]!='.' &&  preFixNum>0 && exp[preFixNum-1]!='-'&& exp[preFixNum-1]!='+'  )) {		//prefix 찾자~!~ 찾았따.
								miniFixNum = preFixNum;

								int startIndex = preFixNum+1;
								if(preFixNum==0)  //0이면은 그지금이자리부터 시작해라.
								startIndex = preFixNum;


							    for (int cpyNum = 0; cpyNum < strlen(exp); ++cpyNum) {	//prefix 부터 ~postfix까지가져와라.

							    	if(startIndex+cpyNum > firstFixNum){							//해당 심볼 중단점을 지났다/
							    		if(!isdigit(exp[startIndex+cpyNum ])  && exp[startIndex+cpyNum]!='.'){
							    			tempexp[cpyNum]='\0';				//끝내라
							    			firstFixNum=0;  //split 한후 바로 붙어있을경우있으니..훔..
//											Debug("ininin  %s [%c] (%d)(%d)",tempexp,exp[startIndex+cpyNum ],startIndex+cpyNum,firstFixNum);
							    			break;
							    		}
							    	}

							    	tempexp[cpyNum]=exp[startIndex+cpyNum];
//							    	Debug("outout(%d) %s  (%d)(%d)",cpyNum,tempexp,startIndex+cpyNum,firstFixNum);
							    }
						}
		///////////////////////////////////
					}

//					SleepH(1);
					int tempexpLen = strlen(tempexp);
					double tempval = calcBasic(tempexp);

					char *splitptr = strstr(exp,tempexp);
					memset(splitptr,' ',tempexpLen);
					splitptr[0]='%';splitptr[1]='s';
					sprintf(tempexp,exp,"%.3f");

					sprintf(exp,tempexp,tempval);
					u_String_SpaceDelete(exp); //공백제거
				}
		//		break;
			}

				Debug("calc!!   * /    %s",exp);
				return atof(exp);









//
//	char tempexp[100]={'\0',};
//	char tempP[100]={'\0',};
//	char tempB[100]={'\0',};
//
//	int bcont = u_String_GetSplitCount(exp,'*');
//	for (int i = 0; i < bcont; ++i) {
//		if(u_String_GetSplitStrB(exp,'*',1,tempB) == false)
//			return false;
//		if(u_String_GetSplitStrF(exp,'*',1,tempP) == false)
//			return false;
//
//		for (int x = 0 ; x< strlen(tempB); ++x) {
//			if(!isdigit(tempB[x])  && tempB[x]!='.'){
//				tempB[x]='\0';
//				break;
//			}
//		}
//
//		for (int y = strlen(tempP)-1; y >= 0; --y) {
//			if(!isdigit(tempP[y])&& tempP[y]!='.'){
//				memmove(tempP,&tempP[y+1],strlen(&tempP[y]));
//				break;
//			}
//		}
//		sprintf(tempexp,"%s%c%s",tempP,'*',tempB);
//		int tempexpLen = strlen(tempexp);
//		double tempval = calcBasic(tempexp);
//
//		char * splitCharptr = strstr(exp,tempexp);
//		memset(splitCharptr,' ',tempexpLen);
//
//		splitCharptr[0]='%';splitCharptr[1]='f';
//		sprintf(tempexp,exp,tempval);
//
//
//		strcpy(exp,tempexp);
//		u_String_SpaceDelete(exp); //공백제거
////		Debug("%s    , %s  (%f)",expresion,exp,tempval);
//
//	}
//
//
////	Debug("calc!!   *    %s",exp);
//
//
//
//
//
//	bcont=0;
//	 bcont = u_String_GetSplitCount(exp,'/');
//		for (int i = 0; i < bcont; ++i) {
//			if(u_String_GetSplitStrB(exp,'/',1,tempB) == false)
//				return false;
//			if(u_String_GetSplitStrF(exp,'/',1,tempP) == false)
//				return false;
//
//			for (int x = 0 ; x< strlen(tempB); ++x) {
//				if(!isdigit(tempB[x])&& tempB[x]!='.'){
//					tempB[x]='\0';
//					break;
//				}
//			}
//
//			for (int y = strlen(tempP)-1; y >= 0; --y) {
//				if(!isdigit(tempP[y])&& tempP[y]!='.'){
//					memmove(tempP,&tempP[y+1],strlen(&tempP[y]));
//					break;
//				}
//			}
//			sprintf(tempexp,"%s%c%s",tempP,'/',tempB);
//			int tempexpLen = strlen(tempexp);
//			double tempval = calcBasic(tempexp);
//
//			char * splitCharptr = strstr(exp,tempexp);
//			memset(splitCharptr,' ',tempexpLen);
//			splitCharptr[0]='%';splitCharptr[1]='f';
//			sprintf(tempexp,exp,tempval);
//
//
//			strcpy(exp,tempexp);
//			u_String_SpaceDelete(exp); //공백제거
////			Debug("%s    , %s  (%f)(tempP %s tempB %s)",expresion,exp,tempval,tempP,tempB);
//
//		}
//
//
////		Debug("calc!!   /    %s  \n\n",exp);
//
//
//
//
//
//		bcont=0;
//		 bcont = u_String_GetSplitCount(exp,'+');
//			for (int i = 0; i < bcont; ++i) {
//				if(u_String_GetSplitStrB(exp,'+',1,tempB) == false)
//					return false;
//				if(u_String_GetSplitStrF(exp,'+',1,tempP) == false)
//					return false;
//
//				for (int x = 0 ; x< strlen(tempB); ++x) {
//					if(!isdigit(tempB[x])&& tempB[x]!='.'){
//						tempB[x]='\0';
//						break;
//					}
//				}
//
//				for (int y = strlen(tempP)-1; y >= 0; --y) {
//					if(!isdigit(tempP[y])&& tempP[y]!='.'){
//						memmove(tempP,&tempP[y+1],strlen(&tempP[y]));
//						break;
//					}
//				}
//				sprintf(tempexp,"%s%c%s",tempP,'+',tempB);
//				int tempexpLen = strlen(tempexp);
//				double tempval = (double)calcBasic(tempexp);
////				Debug("*******   %s (%f)",tempexp,tempval);
//
//				char * splitCharptr = strstr(exp,tempexp);
//				memset(splitCharptr,' ',tempexpLen);
//				splitCharptr[0]='%';splitCharptr[1]='f';
//				sprintf(tempexp,exp,tempval);
//
//
//				strcpy(exp,tempexp);
//				u_String_SpaceDelete(exp); //공백제거
////				Debug("exp: %s  val: %f  (tempP: %s tempB: %s)",exp,tempval,tempP,tempB);
//
//			}
//
//
////			Debug("calc!!   +    %s  \n\n",exp);
//
//
//
//
//
//
//			bcont=0;
//				 bcont = u_String_GetSplitCount(exp,'-');
//					for (int i = 0; i < bcont; ++i) {
//						if(u_String_GetSplitStrB(exp,'-',1,tempB) == false)
//							return false;
//						if(u_String_GetSplitStrF(exp,'-',1,tempP) == false)
//							return false;
//
//						for (int x = 0 ; x< strlen(tempB); ++x) {
//							if(!isdigit(tempB[x])&& tempB[x]!='.'){
//								tempB[x]='\0';
//								break;
//							}
//						}
//
//						for (int y = strlen(tempP)-1; y >= 0; --y) {
//							if(!isdigit(tempP[y])&& tempP[y]!='.'){
//								memmove(tempP,&tempP[y+1],strlen(&tempP[y]));
//								break;
//							}
//						}
//						sprintf(tempexp,"%s%c%s",tempP,'-',tempB);
//						int tempexpLen = strlen(tempexp);
//						double tempval = (double)calcBasic(tempexp);
//		//				Debug("*******   %s (%f)",tempexp,tempval);
//
//						char * splitCharptr = strstr(exp,tempexp);
//						memset(splitCharptr,' ',tempexpLen);
//						splitCharptr[0]='%';splitCharptr[1]='f';
//						sprintf(tempexp,exp,tempval);
//
//
//						strcpy(exp,tempexp);
//						u_String_SpaceDelete(exp); //공백제거
////						Debug("exp: %s  val: %f  (tempP: %s tempB: %s)",exp,tempval,tempP,tempB);
//
//					}
//
//
////					Debug("calc!!   -    %s  \n\n",exp);
//					Debug("[%s] Return val = (%f)",__FUNCTION__,atof(exp));
//					return atof(exp);
//
}












/////////////String Parsing ///////////////////////
//문자열 중에[ ]  사이에 싸여있는 숫자를 리턴합니다.
int Utilities::u_String_getArrayNum(const char * str){
	int strlength = strlen(str);
	char px='[';
	char bx=']';

	if(u_String_GetSplitCount(str,px)==0 || u_String_GetSplitCount(str,bx)==0)
		return -1;

	char * tempStr 		=  new char[strlength];
	if(u_String_between(str,px,bx,tempStr)==false){
		delete[] tempStr;
		return -1;
	}

	int arrNum=0;
		arrNum = atoi(tempStr);

	delete[] tempStr;
	return arrNum;


}
//
//문자열 중에  인자값으로 넘어온 문자이 몇개가있는지 찾습니다.
int Utilities::u_String_GetSplitCount(const char * str, char splitChar){

	int count 	= 	0;
	for (int i = 0; i <= strlen(str); ++i) {
		  if(str[i] == splitChar)
			  count++;
	  }
	  return count;
}

//문자열 중에  인자값으로 넘어온 문자열이 로 쪼갰을때  몇번째  조각을 돌려줍니다.. 앞부분
bool Utilities::u_String_GetSplitStrF(const char * str, char splitChar,int splitNum ,char * rStr){

	if(u_String_GetSplitCount(str,splitChar)<splitNum){
		strcpy(rStr,str);
		return false;
	}
	int strlength  = strlen(str);
	char *temp = new char[strlength];
	memset(temp,0,strlength);

	int count 	= 	0;
	int ri		=	0;

	for (int i = 0; i <= strlength; ++i) {

		  if(str[i] == splitChar)
			  count++;

		  if(count == splitNum-1){


			  if(str[i] == splitChar){ //스필문자가나오면 너너
				  i++; continue;
			  }


			  temp[ri] = str[i];
			  ri++;
		  }
	  }
		 temp[ri]='\0';
		  strcpy(rStr,temp);
		  delete[]temp;
		  return true;
}

//문자열 중에  인자값으로 넘어온 문자열이 로 쪼갰을때  몇번째  조각을 돌려줍니다.. 뒷부분
bool Utilities::u_String_GetSplitStrB(const char * str, char splitChar,int splitNum ,char * rStr){



	if(u_String_GetSplitCount(str,splitChar)<splitNum)
		return false;

	int strlength  = strlen(str);
	char *temp = new char[strlength];
	memset(temp,0,strlength);

	int count 	= 	0;
	int ri		=	0;


	  for (int i = 0; i <= strlength; ++i) {

		  if(count == splitNum){
			  if(str[i] == splitChar) //스필문자가나오면 너너
				  break;
			  temp[ri] = str[i];
			  ri++;
		  }

		  if(str[i] == splitChar)
			  count++;


	  }
	  temp[ri]='\0';
	  strcpy(rStr,temp);
	  delete[]temp;
	  return true;
}

//Type Conversion ----part
int Utilities::u_String_Splite_To_Hex(const char * str, char* splitChar,byte rByteArry[]){


		char *tempstr =new char[COMBUFFER_MAXSIZE];
		memset(tempstr,0,COMBUFFER_MAXSIZE);
		strcpy(tempstr,str);

		int count=0;
	   char	*ptr=NULL;
	   char * strtolpter=NULL;
	   byte hexVal = 0;

	   ptr = strtok( tempstr, splitChar);
//	   Debug("%s\n", ptr);
	   rByteArry[count] = (char)strtol( ptr, &strtolpter, 16);          // 16진수로 변환
	   count++;


	   while( ptr = strtok( NULL, splitChar))
	   {
//		   Debug("%s\n", ptr);
		   rByteArry[count] = (char)strtol( ptr, &strtolpter, 16);          // 16진수로 변환

		   count++;
	   }



	   delete[] tempstr;
	   return count;

}

void Utilities::u_String_SpaceDelete(char * str){
//	 char *str = "1 A\n\f\t\v\r!@#    ";
	   int   ndx;
	   for ( ndx= 0; ndx < strlen(str); ndx++)
	   {
	      if ( isspace(str[ndx])){
	    	  memmove( &str[ndx], (&str[ndx])+1, strlen((&str[ndx])));
	    	  str[strlen(str)]='\0';
			  ndx--;
	      }
	   }

}
void Utilities::u_String_MakeExpresion(const char * str,char * rstr){
	strcpy(rstr,str);
	u_String_SpaceDelete(rstr);//공백제거

	int strlength  =  strlen(rstr);
	   for ( int ndx= 1; ndx < strlength ; ndx++)  //0은잿겨
	   {
		   if(rstr[ndx]=='('){


			   if(isdigit(rstr[ndx-1])){
//Debug("aaa  %c   (%d)",rstr[ndx-1],ndx);
				   memmove(&rstr[ndx+1],&rstr[ndx],strlen(&rstr[ndx]));
				   rstr[ndx]='*';
				   strlength++;
			   }
		   }
		   if(rstr[ndx]==')'){
			   if(isdigit(rstr[ndx+1])){
//Debug("bbb  %c   (%d)",rstr[ndx+1],ndx);
					memmove(&rstr[ndx+2],&rstr[ndx+1],strlen(&rstr[ndx]));
					rstr[ndx+1]='*';
					strlength++;
			   }
		   }
	   }

}

//String  prefix 와 postfix 사이의 문자열을반환한다.
bool Utilities::u_String_between(const char * str,const char prefix, const char postfix,char *rstr){

//	Debug("u_String_between  str(%s) strlength(%d) precount(%d) postfix(%d) ",str,strlen(str),u_String_GetSplitCount(str,prefix), u_String_GetSplitCount(str,postfix));
		if(u_String_GetSplitCount(str,prefix) != u_String_GetSplitCount(str,postfix)){
			return false;
		}
	int rstrCount=0;
	int prefixPassCount=0;
	bool isWrite=false;
	int strlength = strlen(str);

	char * temp = new char[strlength];
	memset(temp,0,strlength);

	for (int i = 0; i <= strlength; ++i) {

		if(isWrite == true && str[i]==postfix){
//			Debug("=isWrite Stop===%s",temp);
			temp[rstrCount]='\0';
			strcpy(rstr,temp);
			delete []temp;
			return true;
		}

		if(isWrite){
			temp[rstrCount]=str[i];
			rstrCount++;
		}

		if(isWrite == false && str[i]==prefix){
//			Debug("=isWrite Start===%s",temp);
			isWrite=true;
			continue;
		}


	}
			delete []temp;
	return false;

}
//String  prefix 와 postfix 사이의 문자열을반환한다.  하지만 위와다르게  최상위 쌍을 찾습니다.
bool Utilities::u_String_betweenPair(const char * str,const char prefix, const char postfix,char *rstr){
	if(u_String_GetSplitCount(str,prefix) != u_String_GetSplitCount(str,postfix)){
		return false;
	}
	if(prefix == postfix)
		return false;


	int passCharCnt = 0;
	int fixCharCnt = 0;
	bool startFlag=false;
	int strlength = strlen(str);
	char *temp = new char[strlength] ;
	memset(temp,0,strlength);

//	Debug("-*-  pre %c  post %c",prefix, postfix);

	for (int charNum = 0; charNum <= strlength; ++charNum) {

//		Debug("%d length(%d)",str[charNum]==prefix,strlength);

		if(startFlag==false && str[charNum]==prefix){  //이제 부터 시작하겠스빈다.
//			Debug("--  %s",&str[charNum]);
			startFlag = true;
			fixCharCnt++;
			continue;
		}

		if(startFlag==true){ //쓰기모드
			if(str[charNum]==prefix){
				fixCharCnt++;
			}else if(str[charNum]==postfix){
				fixCharCnt--;
			}

			if(fixCharCnt <=0){  //쓰기중이면서 이제 쌍이맞으면
				strcpy(rstr,temp);
				delete[] temp;
				return true;
			}
				//쓰기
				temp[passCharCnt] = str[charNum];
				passCharCnt++;
		}
	}

	delete[] temp;
	return false;

}


/////////////////debug//////

void Utilities::Debug(char* format, ...){
	va_list args;
	va_start(args, format);
	va_end(args);
	char *temp = new char[DEBUG_PRINT_MAXSIZE];
	memset(temp,'\0',DEBUG_PRINT_MAXSIZE);
	vsprintf (temp, format, args);
	fflush(stdout);
	cout<<temp<<endl;
	delete[]temp;
}

void Utilities::Debug(long size,const byte buff[], char* format, ...){
	va_list args;
	va_start(args, format);
	va_end(args);
	char *temp = new char[DEBUG_PRINT_MAXSIZE];
	memset(temp,'\0',DEBUG_PRINT_MAXSIZE);
	vsprintf (temp, format, args);


	char logTime[32] = {0, };
	memset(logTime, 0x00, sizeof(logTime));
	getLongTime(logTime);
//	cout<<"=================================================="<<endl;
	cout<<logTime<<" "<<temp<<endl;
	cout<<"==================================================";
	int rownum=0;
		for(long i = 0 ; i < size ; i++){

				if( i % 16 == 0){
					rownum++;
					printf("\n");
				}

			printf("%02X",buff[i]);
			printf("  ");
		}
		printf("\n");

	delete[]temp;
}

/////////time part

char * Utilities::getLongTime(char *datetimebuff){
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


long Utilities::getLongFullTime()
{
	   struct timeb rawtime;
	   struct tm * timeinfo;

	   char temp [50]={0,};
	   ftime(& rawtime);
	   timeinfo = localtime(&rawtime.time);
   sprintf(temp, "%02d%02d%02d%03d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,rawtime.millitm);

   return atol(temp);
}



//System....
//sencond
void Utilities::SleepH(long second){
	usleepH(second*1000000);
}
//milliseconds   1000 / 1 seconds
void Utilities::sleepH(long millisecond){
	usleepH(millisecond*1000);
}
//	/microseconds    1000000 / 1 seconds
void Utilities::usleepH(long microsecond){
	usleep(microsecond);
}

bool Utilities::eval(const char * expresion ,double *rval){

	int strlength = strlen(expresion);
	char *temp = new char[strlength] ;
	float val =  0;
	memset(temp,0,strlength);



	char *exp = new char[strlength+100] ;  //*가추가될수도있다 곱하기가.
	memset(exp,0,strlength);
	Debug("**   %s",expresion);
	u_String_MakeExpresion(expresion,exp);
	Debug("**   %s\n\n",exp);





char *ptr=NULL;
ptr = exp;



	while(1){

			if(u_String_betweenPair(ptr,'(',')',temp)){
				ptr = temp;
				Debug("z   %s",ptr);

			}else{
				Debug("in   %s",ptr);
				val = calcOperator(ptr);

				Debug("ex: %s  val =%f ",ptr,val);

							char tempsplit[50]={0,};
							sprintf(tempsplit,"(%s)",temp);
							int templength = strlen(tempsplit);

							char *splitptr=NULL;
							splitptr = strstr(exp,tempsplit);

							if(splitptr==NULL){ //널이면 더이상 없는것이니.빠져나가라
								delete[] temp;
								return true;
							}
				//
				//			Debug("split: %s  , expresion: %s  ,  tempsplit: %s     (%d)(%f) ",splitptr,tt,tempsplit,templength,val);
							memset(splitptr,' ',templength);
							splitptr[0]='%';splitptr[1]='f';
							char temm[100]={0,};
							sprintf(temm,exp,val);
							strcpy(exp,temm);
							u_String_SpaceDelete(exp);//삭제..
							Debug("temm!!!  %s   ",exp);
							ptr=exp;
				//
//							break;
			}

	}


	delete[] temp;
	return false;

	//	for (int charNum = 0; charNum < strlength; ++charNum) {
//		if(expresion[charNum]=='('){
//			fixCharCnt++;
//		}else if(expresion[charNum]==')'){
//			fixCharCnt--;
//		}
//	}

//괄호 체크. 없으면 -




//	char *temp="aaaaaaaaaaaaa(d{sfs}fdsf(";
//	char tt[55]={0,};
//	u_String_between(temp,'{','}',tt);
//	Debug("%s  ======",tt);

//
//	for (int charNum = 0; charNum < strlen(expresion); ++charNum) {
//
//	}








//
//	char tempexp[55]={0,};
//	char temp_t[55]={0,};
//	memset(tempexp,0,55);memset(temp_t,0,55);
//
//
//	if(u_String_between(expresion,'(',')',tempexp)){
//		float val = eval(tempexp);
//		Debug("%f    aa",val);
////		char temp[55]={0,};
////		sprintf("(%s)",tempexp);
////		int len = strlen(tempexp)-1;
////		char * splitstr = strstr(expresion,tempexp);
////		splitstr[0]='%';splitstr[0]='f';
////		for (int i = 2; i < len; ++i)
////			splitstr[i]=' ';
////		Debug("====%s",expresion);
//
//	}else{
//		float returnval = calcOperator(expresion);
//		Debug("---(%f)--- %s",returnval,expresion);
//		return returnval;
//	}


//	int tempexpresionLength = strlen(tempexp)-1; //하나뺌  문자를바꾸기위해.
//
//	//치환하기위헌것,
//	char tempReplace[55]={0,};
//	sprintf(tempReplace,"(%s)",tempexp);
//
//	char * temprepalcePtr = strstr(expresion,tempReplace);
//	Debug("(%s)------ %s",temprepalcePtr,tempReplace);

//	temprepalcePtr[0]='%';
//	temprepalcePtr[1]='f';  //초기화
//	for (int replaceNum = 2; replaceNum < tempexpresionLength; ++replaceNum) {
//		temprepalcePtr[replaceNum]=' ';
//	}
////
//	Debug("------%s",expresion);


//	int prefixlength = temprepalcePtr - expresion;
//	char tempexp[55]={0,};
//	memcpy(tempexp,expresion,prefixlength);


//	if(u_String_GetSplitCount(temp,'(') == u_String_GetSplitCount(temp,')')){
//			eval(temp);
//	}
//	calcOperator(tempexp);
//	Debug("Evanl   %s =(%f)",tempexp,calcOperator(tempexp));
}












//////communication

void Utilities::trace( void * buffer,long size,char * title){


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




int Utilities::groupByteOrder(void * startAddr , int groupSize, int length){

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
unsigned short int  Utilities::makeCRC(unsigned char *puchMsg,  unsigned short usDataLen)
{
	if(usDataLen<=0)
		return -1;
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
