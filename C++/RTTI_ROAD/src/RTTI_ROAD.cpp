//============================================================================
// Name        : RTTI_ROAD.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "wow.h"
#include "supertest.h"

using namespace std;
int testClass(){


	FILE * fp;

	if((fp=fopen("/cygdrive/c/code/CPP/BlackBox/RTTI_NEW/wow.txts","r"))==NULL){
		cout<<"Not File "<<endl;
//		fprintf(stdin,"con't open\n");
	}

	char rbuff[999]={0,};
	fread(rbuff,sizeof(wow),1,fp);

	printf("%x\n",rbuff);

	wow* www = (wow*)rbuff;
	www->showString("aaaaaaaa");
	www->showSuperString();
	test * t = www;
	t->showString();

	t->puString();
	supertest * st = www;
	st->puString();



}

int main() {
	cout << "!!!Hello World!!!Road" << endl; // prints !!!Hello World!!!
	testClass();

//	wow t;
//	t.setstate(15);
//	t.showString("a");
	return 0;
}
