//============================================================================
// Name        : ThreadTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;


int wow=010l;
pthread_mutex_t mutx;

void *threadTest(void * z){
int t = *(int *)z;
	for (int var = 0; var < 1	; ++var) {

		pthread_mutex_lock(&mutx);
		cout<< var <<"my name is hhk "<<t<<"zzzzzzzz"<<wow<<endl;
		wow=var;
		pthread_mutex_unlock(&mutx);
//		usleep(4);


	}
	return NULL;
}
int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

	int mstate =	pthread_mutex_init(&mutx,NULL);
	if(mstate){
		cout<<"뮤텍스 실패"<<endl;
		exit(1);
	}

	pthread_t t_id[100];
	int state=0;
	int input  = 5;
	int portIndex [4]={0,1,2,3};

	for (int var = 0; var < 100; ++var) {
		state = pthread_create(&t_id[var],NULL,threadTest,&var);
		if(state!=0){
			cout<<"쓰레드생성오류"<<endl;
			exit(1);
		}
		cout<<state<<endl;

	}

	void * t_return[100];
	for (int var = 0; var < 100; ++var) {
		state = pthread_join(t_id[var],&t_return[var]);
		if(state!=0){
			cout<<"쓰레드JOIN 오류"<<endl;
			exit(1);
		}

	}

		pthread_mutex_destroy(&mutx);

	return 0;
}
