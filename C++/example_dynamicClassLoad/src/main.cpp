/*
 * main.cpp
 *
 *  Created on: 2010. 5. 28.
 *      Author: KDT
 */
#include <iostream>
#include <time.h>
#include <sys/timeb.h>
#include <stdio.h>
#include "Class.h"
#include "Printer.h"
using namespace std;
typedef void* (*GetInstanceFuncType)();
long getLongFullTime()
{
	   struct timeb rawtime;
	   struct tm * timeinfo;
	   char 	temp[50] = { 0, };
	   ftime(& rawtime);
	   timeinfo = localtime(&rawtime.time);
   sprintf(temp, "%02d%02d%02d%03d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec,rawtime.millitm);

   return atol(temp);
}


int test(){
	Printer *printer = (Printer *)Class::forName("./Debug/src/InkPrinter.o");
	if(printer==NULL)
	return -1;

	printer->On();
	printer->Off();
	printer->Print();

	delete printer;
	printer =NULL;
}


int main(int argc, char **argv) {
	cout<<"hi printer world"<<__FILE__<<endl;



//while(1)
	test();










//
//		  while(1){
//			  void *handle_th = dlopen("./Debug/src/InkPrinter.o", RTLD_LAZY);
//				if(!handle_th){
//					cerr << "Cannot load library: " << dlerror() << '\n';
//					exit(0);
//				}
//
//				 GetInstanceFuncType GetInstance_th = (GetInstanceFuncType) dlsym(handle_th,"GetInstance");
//				  if ((error = dlerror()) != NULL){
//					  printf("ERROR : %s\n",error);
//					  exit(0);
//					}
////				  Printer * printer_t =(Printer *) GetInstance_th();
//
//				cout<<"good Creative Dynamic Library"<<endl;
//		  }
	//dlclose()


}





//
//InkPrinter* inkprint = new InkPrinter();
//inkprint->On();
//inkprint->Off();
//Printer * printer = (Printer*)inkprint;
//printer->On();
