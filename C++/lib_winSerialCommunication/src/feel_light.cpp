#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "WindowSerialCommunication.h"
#include "APP_UTILS.h"

#define W_factor	25			// �¾���� <0 �� �� W���� 25�� �����Ѵ�.
#define el_factor	5			// �¾��� ���� 5���������� ���� W���� ������ �ش�.
#define dis_w		5			// ���������� '�������� ����' �����̸� �¾��� �� �� dis_w��ŭ �÷��ش�.

int sel_light = 1;							// 1: �������� ���� ����, 0: �������� ���� ����

int adj = 1;								// �� ���µ�, ��ſ�
int subj = 11;								// ���� ���µ�, ���ǽð�

double date_no = 1;							// 0: ����ð�, 1: �����ð�

double longitude = 126;						// ������� �浵
double lat = 35;							// ������� ����

double Td = 50.9;							// �Ǳ��µ�(����)
double H = 71;								// ����(%)

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
	double dis;		//��������  (�Ǳ��µ�(����),����(%))
	double dis_fac;  //use ��������
	double cel;
	double W_el;		//use White
	double W;
	double RGBW[4];
	int i;

	// ��������
	dis = discomfort_m(Td,H);

	// ���������� ����ġ �̻� �ö󰡸� ������ ������ ���谨�� ����Ʈ����.
	dis_fac = fuzzy_dis(dis, Td);

	// ���������� '�������� ����'�����̸� dw�� dis_w���̰� ���������� ���� ��ȭ�� ���ٸ� dw���� 0�̴�.
	dw = 0;
	if(dis_fac==2)  //�������� ����
		dw = dis_w;
	else if(dis_fac==3) { //���������� ���� ��ȭ�� ����
		sel_light = 0;
		adj = 5;
	}


	// �¾��
	cel =solar(longitude,lat);

	// �¾��� ���� ���� ���� W���� �����Ͽ� �ٲ�µ� ���� 5�� �ٲ𶧸��� W���� ������ �ش�.
	// ���������� ���� dw���� �����༭ W���� ����Ѵ�.
	W_el = (double)((int)(cel/el_factor))*el_factor + dw;


	// W���� 30% �̻� �ø��� �ʴ´�. W���� �ʹ� ũ�� �������� �ʹ� Ŀ���� RGB�� W�� ���� ��������(������� ����).
	// ���� ���µ��� ���� ��ȭ�� ���� ���ٰ� �ǴܵǾ� W_factor�� �����Ѵ�.
	// �¾��� ���� ���� W(%)���� ��ȭ���ִ� �����̴�.
	if(W_el < 0)
		cw = W_factor;
	else if(W_el < 30)
		cw = W_factor-W_el;
	else
		cw = (W_factor-W_el)*2;


	// ���� or ��� ���� ����
	if(sel_light == 1){
		// ���� ����
		temperature = subject_t(subj);    // ���� ���µ� ����
		tempe2xyY(temperature);

		XYZ[0] = xyY[2] * xyY[0] / xyY[1];
		XYZ[1] = xyY[2];
		XYZ[2] = xyY[2] * (1 - xyY[0] - xyY[1]) / xyY[1];


		RGB[0] = 2.3707 * XYZ[0] - 0.9001 * XYZ[1] - 0.4706 * XYZ[2];
		RGB[1] = -0.5139 * XYZ[0] + 1.4253 * XYZ[1] + 0.0886 * XYZ[2];
		RGB[2] = 0.0053 * XYZ[0] - 0.0147 * XYZ[1] + 1.0094 * XYZ[2];
	}
	else {
		// ����� ���
	    // ����� �̹��� �����ϻ��� �������� ���� ���õ� ������ �����ϰ� ������ ���� ��ǥ�� ���� �����̿� �ִ�
		// Į�� �̹��� �����ϻ��� ������ ���õ� ������ ������ �������� �����Ѵ�.
		// ���õǾ��� RGB���� ������ ���� W���� ����, �� �� W���� ���� RGB���� �����Ѵ�.
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
	double B_test[9] = {1.000317, 2.430594, 0, 0.878982, 2.044006, 1.10057, 1.894398, 1.879864, 2.975559};		// �� ��Ģ�� �Ķ����
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
			w[k] = mem[0][i]*mem[1][j];		// ���յ��� ���Ѵ�.
			sum_w += w[k];
			k += 1;
		}
	}

	k = 0;
	for(i=0; i<3; i++) {
		for(j=0; j<3; j++) {
			w_ji[k] = w[k]/sum_w;		// ����ȭ�� �����ش�.
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


	double timezone = -9;		// ���ѹα� Ÿ����
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

	longitude *= -1;	// �⺻ ��������� (-)�ε� ����� ���Ǹ� ���� (-)�� �����ش�.

	if(date_no==0){ //����ð�����

		timer = time(NULL); // ���� �ð��� �� ������ ���
		t = localtime(&timer);	// �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�

	}else if(date_no==1){//���ó�¥ ����
		   struct tm  user_stime;
		   user_stime.tm_year   = date[0]   -1900;   // ���� :�⵵�� 1900����� ����
		   user_stime.tm_mon    = date[1]      -1;      // ���� :���� 0���� ����
		   user_stime.tm_mday   = date[2];
		   user_stime.tm_hour   = date[3];
		   user_stime.tm_min    = date[4];
		   user_stime.tm_sec    = date[5];
		   user_stime.tm_isdst  = 0;           // ��� Ÿ�� ��� ����
		   timer   = mktime( &user_stime);
			t = localtime(&timer);	// �� ������ �ð��� �и��Ͽ� ����ü�� �ֱ�
	}







		year 	= t->tm_year +1900;
		month 	= t->tm_mon +1;
		day 	= t->tm_mday;
		hr 		= t->tm_hour;
		mn 		= t->tm_min;
		sc 		= t->tm_sec;
	// ���� 1�� 1�Ͽ������� ����°�ΰ�
	dayofyear = t->tm_yday +1;	// 1�� 1�� �� ��, 0���̹Ƿ� +1���ش�.


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

	// ���µ�(Color temperaturerature)���� xy�� ��ȯ�ϴ� ���� 1,667���� 25,000������ ���µ��� xy��ǥ�� ��ȯ�� ���ִ�.
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

	// xyY���� ���ؾ��ϴµ� XYZ������ ��ȭ�� �� Y���� X,Z�� �����ϰ� ��������. �׷��� RGB�������� ��ȯ���� �� RGB����
	// �߿��� ����Ʈ�� �� �� �ִµ� �츮�� ����ϴ� XYZ to RGB�Ŀ����� Y���� ���� RGB�� ����ϰ� ��ȭ�ϰ� W���� ÷������ ��
	// RGB�� �ִ�� ����� �ֱ� ������ Y�� �߿����� �ʴٰ� �Ǵ�. Y�� ���� 100���� �����Ѵ�.
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
