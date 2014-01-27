#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "WindowSerialCommunication.h"
#include "APP_UTILS.h"

#define W_factor	25			// 태양고도가 <0 일 때 W값을 25로 설정한다.
#define el_factor	5			// 태양의 고도를 5단위씩으로 끊어 W값에 영향을 준다.
#define dis_w		5			// 불쾌지수가 '불편함을 느낌' 상태이면 태양의 고도 를 dis_w만큼 올려준다.

int sel_light = 1;							// 1: 과목으로 조명 결정, 0: 감성언어로 조명 결정

int adj = 1;								// 언어별 색온도, 즐거운
int subj = 11;								// 과목별 색온도, 음악시간

double date_no = 1;							// 0: 현재시간, 1: 설정시간

double longitude = 126;						// 서울기준 경도
double lat = 35;							// 서울기준 위도

double Td = 50.9;							// 건구온도(섭씨)
double H = 71;								// 습도(%)

double xyY[3];
double RGB[3];
double date[6] = {2010, 12, 18, 10 , 5, 55};	// [year month day hour minute second];
//double date[6] = {2010, 12, 29, 10, 26, 00};	// [year month day hour minute second];

double discomfort_m(double dry_temp, double humidity);
double fuzzy_dis(double dis, double Td);
double solar(double longitude,double lat);
double subject_t(int select);
void tempe2xyY(double temperature);
void adj_order(double adj);



int main(void) {
	double dw = 0;
	double temperature;
	double XYZ[3];
	double max_RGB;
	double cw;
	double dis;		//불쾌지수  (건구온도(섭씨),습도(%))
	double dis_fac;  //use 불쾌지수
	double cel;
	double W_el;		//use White
	double W;
	double RGBW[4];
	int i;

	// 불쾌지수
	dis = discomfort_m(Td,H);

	// 불쾌지수가 일정치 이상 올라가면 조도를 높혀서 불쾌감을 떨어트린다.
	dis_fac = fuzzy_dis(dis, Td);

	// 불쾌지수가 '불편함을 느낌'상태이면 dw가 dis_w값이고 불쾌지수에 의해 변화가 없다면 dw값은 0이다.
	dw = 0;
	if(dis_fac==2)  //불편함을 느낌
		dw = dis_w;
	else if(dis_fac==3) { //불쾌지수에 의해 변화가 없다
		sel_light = 0;
		adj = 5;
	}


	// 태양고도
	cel =solar(longitude,lat);

	// 태양의 고도에 따라 값이 W값을 조절하여 바뀌는데 고도가 5씩 바뀔때마다 W값에 영향을 준다.
	// 불쾌지수에 의한 dw값을 더해줘서 W값을 계산한다.
	W_el = (double)((int)(cel/el_factor))*el_factor + dw;


	// W값을 30% 이상 올리지 않는다. W값이 너무 크면 조도값이 너무 커지고 RGB가 W에 의해 가려진다(백색조명만 보임).
	// 따라서 색온도에 따른 변화가 거의 없다고 판단되어 W_factor로 제한한다.
	// 태양의 고도에 따라 W(%)값을 변화해주는 공식이다.
	if(W_el < 0)
		cw = W_factor;
	else if(W_el < 30)
		cw = W_factor-W_el;
	else
		cw = (W_factor-W_el)*2;


	// 과목 or 언어 조명 결정
	if(sel_light == 1){
		// 수업 과목
		temperature = subject_t(subj);    // 과목별 색온도 결정
		tempe2xyY(temperature);

		XYZ[0] = xyY[2] * xyY[0] / xyY[1];
		XYZ[1] = xyY[2];
		XYZ[2] = xyY[2] * (1 - xyY[0] - xyY[1]) / xyY[1];


		RGB[0] = 2.3707 * XYZ[0] - 0.9001 * XYZ[1] - 0.4706 * XYZ[2];
		RGB[1] = -0.5139 * XYZ[0] + 1.4253 * XYZ[1] + 0.0886 * XYZ[2];
		RGB[2] = 0.0053 * XYZ[0] - 0.0147 * XYZ[1] + 1.0094 * XYZ[2];
	}
	else {
		// 형용사 언어
	    // 형용사 이미지 스케일상의 감성언어에서 조명에 관련된 감성언어를 추출하고 감성언어에 대한 좌표와 가장 가까이에 있는
		// 칼라 이미지 스케일상의 색상을 선택된 감성언어에 적합한 색상으로 간주한다.
		// 선택되어진 RGB값과 조도에 따라 W값을 결정, 이 때 W값에 따라 RGB값을 수정한다.
		adj_order(adj);
	}
	printf("****R(%f) ,G(%f) ,B(%f)   subj(%d) adj (%d)\n",RGB[0],RGB[1],RGB[2],subj,adj);

	max_RGB = RGB[0];
	if(max_RGB<RGB[1])
		max_RGB = RGB[1];
	if(max_RGB<RGB[2])
		max_RGB = RGB[2];

	for(i=0; i<3; i++) {
		RGB[i] /= max_RGB;
		RGB[i] *= (255-fabs(cw));
		RGB[i] += 0.5;
		RGB[i] = (double)((int)(RGB[i]));
	}

	if(cw<0)
		W = 0;
	else
		W = cw;

	RGBW[0] = RGB[0];
	RGBW[1] = RGB[1];
	RGBW[2] = RGB[2];
	RGBW[3] = W;

	printf("R(%f) ,G(%f) ,B(%f) ,W(%f)\n",RGBW[0],RGBW[1],RGBW[2],RGBW[3]);




//make Data!
	Sendinfo sendDataTemp={0,};
	    sendDataTemp.Addr=0x01;
	    sendDataTemp.FunctionCode=0x10;
	    sendDataTemp.StartAddr=41004;
	    sendDataTemp.DataCount=4;
	    sendDataTemp.ByteCount=sendDataTemp.DataCount*2;
	    sendDataTemp.DataWhite=(unsigned short)RGBW[3];
	    sendDataTemp.DataRead=(unsigned short)RGBW[0];
	    sendDataTemp.DataGreen=(unsigned short)RGBW[1];
	    sendDataTemp.DataBlue=(unsigned short)RGBW[2];

	//byte! orderring
	Sendinfo sendData={0,};
	sendData.Addr=sendDataTemp.Addr;
    sendData.FunctionCode =   sendDataTemp.FunctionCode;
	ntoh((char*)&sendDataTemp.StartAddr,(char*)&sendData.StartAddr,2);
	ntoh((char*)&sendDataTemp.DataCount,(char*)&sendData.DataCount,2);
	ntoh((char*)&sendDataTemp.ByteCount,(char*)&sendData.ByteCount,2);
	ntoh((char*)&sendDataTemp.DataWhite,(char*)&sendData.DataWhite,2);
	ntoh((char*)&sendDataTemp.DataRead,(char*)&sendData.DataRead,2);
	ntoh((char*)&sendDataTemp.DataGreen,(char*)&sendData.DataGreen,2);
	ntoh((char*)&sendDataTemp.DataBlue,(char*)&sendData.DataBlue,2);
	unsigned short crc = makeCRC((unsigned char*)&sendData,sizeof(sendData)-2);
	ntoh((char*)&crc,(char*)&sendData.CRC,2 );


	printf("\n  R(%d) ,G(%d) ,B(%d) ,W(%d)\n",sendDataTemp.DataRead,sendDataTemp.DataGreen,sendDataTemp.DataBlue,sendDataTemp.DataWhite);

		myOpen("COM4",9600);
		mySend(sendData,sizeof(sendData));
		Sleep(5);
		char    read_buff[8192];
		int receiveSize = myReceive(read_buff);

			printf("%\n");
		for (int i = 0; i < receiveSize; ++i) {
			printf("%02X .",read_buff[i]);
		}
		myClose();

		printf("END!!");
	   return 0;
}

double discomfort_m(double dry_temp, double humidity) {
	double dis;
	dis = 0.81*dry_temp + 0.01*humidity*(0.99*dry_temp - 14.3) + 46.3;

	return dis;
}

double fuzzy_dis(double dis, double Td) {
	
	int i,j,k;
	double x[2] = {dis, Td};
	double dt[4][2] = {75, 24, 80, 26.5, 83, 28.5, 86, 30};
	double mem[2][3];
	double w[9], w_ji[9];
	double sum_w = 0;
	double B_test[9] = {1.000317, 2.430594, 0, 0.878982, 2.044006, 1.10057, 1.894398, 1.879864, 2.975559};		// 각 규칙의 파라미터
	double dis_fac = 0;

	for(i=0; i<2; i++) {
		k = 0;
		if(x[i]<=dt[0][i])
			mem[i][k] = 1;
		else if(x[i]<dt[1][i])
			mem[i][k] = 1-fabs(x[i]-dt[0][i])/(dt[1][i]-dt[0][i]);
		else
			mem[i][k] = 0;

		k = 1;
		if(x[i]<dt[0][i])
			mem[i][k] = 0;
		else if(x[i]<dt[1][i])
			mem[i][k] = (x[i]-dt[0][i])/(dt[1][i]-dt[0][i]);
		else if(x[i]<dt[2][i])
			mem[i][k] = 1;
		else if(x[i]<dt[3][i])
			mem[i][k] = 1-fabs(x[i]-dt[3][i])/(dt[2][i]-dt[3][i]);
		else
			mem[i][k] = 0;

		k = 2;
		if(x[i]<dt[2][i])
			mem[i][k] = 0;
		else if(x[i]<dt[3][i])
			mem[i][k] = 1-fabs(x[i]-dt[3][i])/(dt[2][i]-dt[3][i]);
		else
			mem[i][k] = 1;
	}

	k = 0;
	for(i=0; i<3; i++) {
		for(j=0; j<3; j++) {
			w[k] = mem[0][i]*mem[1][j];		// 적합도를 구한다.
			sum_w += w[k];
			k += 1;
		}
	}

	k = 0;
	for(i=0; i<3; i++) {
		for(j=0; j<3; j++) {
			w_ji[k] = w[k]/sum_w;		// 정규화를 시켜준다.
			k += 1;
		}
	}

	k = 0;
	for(i=0; i<3; i++) {
		for(j=0; j<3; j++) {
			dis_fac += w_ji[k]*B_test[k];
			k += 1;
		}
	}

	dis_fac += 0.5;
	dis_fac = (double)((int)dis_fac);
	
	return dis_fac;
}


double solar(double longitude,double lat) {


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
	double pi = 3.14159265358979323846264338327950288419716939937510;

	longitude *= -1;	// 기본 동경기준은 (-)인데 사용자 편의를 위해 (-)를 곱해준다.

	if(date_no==0){ //현재시간적용

		timer = time(NULL); // 현재 시각을 초 단위로 얻기
		t = localtime(&timer);	// 초 단위의 시간을 분리하여 구조체에 넣기

	}else if(date_no==1){//셋팅날짜 적용
		   struct tm  user_stime;
		   user_stime.tm_year   = date[0]   -1900;   // 주의 :년도는 1900년부터 시작
		   user_stime.tm_mon    = date[1]      -1;      // 주의 :월은 0부터 시작
		   user_stime.tm_mday   = date[2];
		   user_stime.tm_hour   = date[3];
		   user_stime.tm_min    = date[4];
		   user_stime.tm_sec    = date[5];
		   user_stime.tm_isdst  = 0;           // 썸머 타임 사용 안함
		   timer   = mktime( &user_stime);
			t = localtime(&timer);	// 초 단위의 시간을 분리하여 구조체에 넣기
	}







		year 	= t->tm_year +1900;
		month 	= t->tm_mon +1;
		day 	= t->tm_mday;
		hr 		= t->tm_hour;
		mn 		= t->tm_min;
		sc 		= t->tm_sec;
	// 올해 1월 1일에서부터 몇일째인가
	dayofyear = t->tm_yday +1;	// 1월 1일 일 때, 0일이므로 +1해준다.


	gam = 2*pi/365*(dayofyear -1 +(hr-12)/24);
	eqtime = 229.18*(0.000075 +0.001868*cos(gam) -0.032077*sin(gam) -0.014615*cos(2*gam) -0.040849*sin(2*gam));
	decl = 0.006918 -0.399912*cos(gam) +0.070257*sin(gam) -0.006758*cos(2*gam)+0.000907*sin(2*gam) -0.002697*cos(3*gam) +0.00148*sin(3*gam);
	decl=decl*180/pi;
	time_offset = eqtime -4*longitude + 60*timezone;
	tst = hr*60 +mn +sc/60 +time_offset;
	hha = tst/4 -180;
	el=asin(sin(lat*pi/180)*sin(decl*pi/180)+cos(lat*pi/180)*cos(decl*pi/180)*cos(hha*pi/180))*180/pi;

	return el;

}

double subject_t(int select) {
	int i;
	double subj[11];

	FILE * fp_dat1 = fopen("subject_data2.txt", "rt");
	if(fp_dat1==NULL) {
		printf("Subject file open error!\n");
		return 1;
	}

	for(i=0;i<11;i++)
		fscanf(fp_dat1,"%lf",&subj[i]);
	fclose(fp_dat1);

	return subj[select-1];
}

void tempe2xyY(double temperature) {
	double x,y,Y;

	// 색온도(Color temperaturerature)에서 xy로 변환하는 공식 1,667에서 25,000범위의 색온도를 xy좌표로 변환할 수있다.
	if(temperature>1667 && temperature<4000)
		x = -0.2661239*pow(10,9)/pow(temperature,3)-0.234358*pow(10,6)/pow(temperature,2)+0.8776956*pow(10,3)/temperature+0.17991;
	else if(temperature>=4000 && temperature<=25000)
		x = -3.0258469*pow(10,9)/pow(temperature,3) +2.1070379*pow(10,6)/pow(temperature,2) +0.2226347*pow(10,3)/temperature +0.24039;
	else {
		printf("ERROR: CCT must be between 1667 and 25000\n");
		return ;
	}

	if(temperature>=1667 && temperature<2222)
		y = -1.1063814*pow(x,3) - 1.3481102*pow(x,2) + 2.18555832*x - 0.20219683;
	else if(temperature>=2222 && temperature<4000)
		y = -0.9549476*pow(x,3) - 1.37418593*pow(x,2) + 2.09137015*x - 0.16748867;
	else if(temperature>=4000 && temperature<=25000)
		y = 3.0817580*pow(x,3) - 5.8733867*pow(x,2) + 3.75112997*x - 0.37001483;
	else {
		printf("ERROR: CCT must be between 1667 and 25000\n");
		return ;
	}

	// xyY값을 구해야하는데 XYZ값으로 변화할 때 Y값은 X,Z에 일정하게 곱해진다. 그래서 RGB값으로의 변환해줄 때 RGB값의
	// 중요한 포인트가 될 수 있는데 우리가 사용하는 XYZ to RGB식에서는 Y값에 따라 RGB가 비례하게 변화하고 W값을 첨가해줄 때
	// RGB를 최대로 만들어 주기 때문에 Y는 중요하지 않다고 판단. Y의 값은 100으로 고정한다.
	Y = 100;

	xyY[0] = x;
	xyY[1] = y;
	xyY[2] = Y;
}

void adj_order(double adj) {
	int i,j;
	double ad[7][3];

	FILE * fp_dat2 = fopen("adjective_data.txt", "rt");
	if(fp_dat2==NULL) {
		printf("Adjective file open error!\n");
		return ;
	}

	for(i=0;i<7;i++)
		for(j=0; j<3; j++)
			fscanf(fp_dat2,"%lf",&ad[i][j]);
	fclose(fp_dat2);
	
	RGB[0] = ad[(int)adj-1][0];
	RGB[1] = ad[(int)adj-1][1];
	RGB[2] = ad[(int)adj-1][2];


}
