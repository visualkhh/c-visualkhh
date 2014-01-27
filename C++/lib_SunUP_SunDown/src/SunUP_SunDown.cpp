#include<iostream>
#include<stdio.h>
#include <math.h>
using namespace std;

void solar(double longitude,double lat,long * sunup, long * sundown){


	double timezone = -9;		// 대한민국 타임존
	//double T0[6] = {data[0], 1, 1, 0, 0, 0};
	double year;
	double month;
	double day;
	double hr;
	double mn;
	double sc;
	double dayofyear;
	double gam;
	double eqtime;
	double decl;
	double time_offset;
	double tst;
	double hha;
	double el;
	time_t timer;
	struct tm *t;
	double PI = 3.14159265358979323846264338327950288419716939937510;

	double ha;
	double sunrise;
	double sunset	;
	double snoon	;

	longitude *= -1;	// 기본 동경기준은 (-)인데 사용자 편의를 위해 (-)를 곱해준다.

	timer = time(NULL); // 현재 시각을 초 단위로 얻기
	t = localtime(&timer);	// 초 단위의 시간을 분리하여 구조체에 넣기

	year = t->tm_year +1900;
	month = t->tm_mon +1;
	day =  t->tm_mday;
	hr = t->tm_hour;
	mn = t->tm_min;
	sc = t->tm_sec;

//	printf("year%f   month%f   day%f  hr%f   mn%f   mn%f\n",year ,month,day  ,hr   ,mn   ,sc );
	// 올해 1월 1일에서부터 몇일째인가
	dayofyear = t->tm_yday +1;	// 1월 1일 일 때, 0일이므로 +1해준다.

	gam = 2*PI/365*(dayofyear -1 +(hr-12)/24);
	eqtime = 229.18*(0.000075 +0.001868*cos(gam) -0.032077*sin(gam) -0.014615*cos(2*gam) -0.040849*sin(2*gam));
	decl = 0.006918 -0.399912*cos(gam) +0.070257*sin(gam) -0.006758*cos(2*gam)+0.000907*sin(2*gam) -0.002697*cos(3*gam) +0.00148*sin(3*gam);
	decl=decl*180/PI;
	time_offset = eqtime -4*longitude + 60*timezone;
	tst = hr*60 +mn +sc/60 +time_offset;
	hha = tst/4 -180;
	el=asin(sin(lat*PI/180)*sin(decl*PI/180)+cos(lat*PI/180)*cos(decl*PI/180)*cos(hha*PI/180))*180/PI;



///

	 hr=0;
	gam=2*PI/365*(dayofyear-1 + (hr-12)/24);
	eqtime=229.18*(0.000075+0.001868*cos(gam)-0.032077*sin(gam)-0.014615*cos(2*gam)-0.040849*sin(2*gam));
	decl=0.006918-0.399912*cos(gam)+0.070257*sin(gam)-0.006758*cos(2*gam) + 0.000907*sin(2*gam)-0.002697*cos(3*gam)+0.00148*sin(3*gam);
	 decl=decl*180/PI;
	ha=acos(cos(90.833*PI/180)/cos(lat*PI/180)/cos(decl*PI/180)-tan(lat*PI/180)*tan(decl*PI/180))*180/PI;

	sunrise	=(720+360+4*(longitude-ha)-eqtime);	//일출시간
	sunset	=(720+4*(longitude+ha)-eqtime);		//일몰시간
	snoon	=720+4*longitude-eqtime;			//남중시간


	int sunrise_shour		= (int)(sunrise/60);
	int sunrise_smin		= (int)(sunrise-sunrise_shour*60);
	int sunrise_ssec		= (int)((sunrise-sunrise_shour*60 - sunrise_smin)*60);
	sunrise_shour=sunrise_shour+12+timezone;
	printf("Sunrise Up  shour %d  smin %d , smin%d  \n",sunrise_shour,sunrise_smin,sunrise_ssec);
	char 	temp1[50] = { 0, };
	sprintf(temp1, "%02d%02d%02d", sunrise_shour, sunrise_smin, sunrise_ssec);
	*sunup = atol(temp1);



	int sunset_shour		= (int)(sunset/60);
	int sunset_smin		= (int)(sunset-sunset_shour*60);
	int sunset_ssec		= (int)((sunset-sunset_shour*60 - sunset_smin)*60);
	sunset_shour=sunset_shour-timezone;
	char 	temp2[50] = { 0, };
	sprintf(temp2, "%02d%02d%02d", sunset_shour, sunset_smin, sunset_ssec);
	*sundown = atol(temp2);
		printf("sunset Down   shour %d  smin %d , smin%d  \n",sunset_shour,sunset_smin,sunset_ssec);





//	int snoon_shour		= (int)(snoon/60);
//	int snoon_smin		= (int)(snoon-snoon_shour*60);
//	int snoon_ssec		= (int)((snoon-snoon_shour*60 - snoon_smin)*60);
//	sunset_shour=snoon_shour-timezone;
//	printf("snoon   shour %d  smin %d , smin%d  \n",snoon_shour,snoon_smin,snoon_ssec);



}
			//	경도					위도
long getSunUp(double longitude,double lat){
	long sunup=0;
	long sundown=0;
	solar(longitude,lat,&sunup,&sundown);
	return sunup;
}
long getSunDown(double longitude,double lat){
	long sunup=0;
	long sundown=0;
	solar(longitude,lat,&sunup,&sundown);
	return sundown;
}
int main()
{

long sunup=0;
long sundown=0;

	sunup = getSunUp(126,35);
	sundown = getSunDown(126,35);

	long t=60;
	t=t*100;
	printf("%d   %d        %d",sunup,sundown,t);


}
