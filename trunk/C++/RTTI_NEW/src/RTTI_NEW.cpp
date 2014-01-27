//============================================================================
// Name        : RTTI_NEW.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "wow.h"
using namespace std;
//
int mains() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!


//wow* w = new wow();
//w->setstate(1);
//w->showString("Number1");
wow w ;
w.setstate(100);
w.showString("Number1");
w.puString();
test * t =&w;
t->puString();
supertest * st = &w;
st->puString();

printf("(%x) sizeof(wow) %d",&w,sizeof(wow));
	FILE * fp;
	char words[100];

	if((fp=fopen("wow.txts","w+"))==NULL){
		fprintf(stdin,"con't open\n");
	}

	puts("====================");
		fwrite(&w,sizeof(wow),1,fp);


		unsigned char * buff[999]={0,};

		memcpy(buff,&w,sizeof(wow));



		for (int i = 0; i <sizeof(wow); ++i) {
			printf("%02x \t",buff[i]);
		}


	rewind(fp);
	printf("\n");
		unsigned char * rbuff[999]={0,};

	fread(rbuff,sizeof(wow),1,fp);


	for (int i = 0; i <sizeof(wow); ++i) {
		printf("%02x \t",rbuff[i]);
	}
	printf("\n");



	wow* ww = (wow*)buff;
	ww->showString("Number2");

	wow* www = (wow*)rbuff;
	www->showString("Number3");


	wow wwww;
	wwww.setstate(5);
	wwww.showString("Number4");

	memcpy(&wwww,rbuff,sizeof(wow));
	wwww.showString("Number4-1");





//	while(fscanf(fp,"%s",words)==1)
//		puts(words);

	fclose(fp);









//	FILE * fp;
//	char words[100];
//
//	if((fp=fopen("wow.txts","a+"))==NULL){
//		fprintf(stdin,"con't open\n");
//	}
//
//	puts("====================");
//	while(gets(words)!=NULL && words[0]!='\0')
//		fprintf(fp,"%s",words);
//
//	rewind(fp);
//
//	while(fscanf(fp,"%s",words)==1)
//		puts(words);
//
//	fclose(fp);

//
}
