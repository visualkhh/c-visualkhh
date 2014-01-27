/*
 * InkPrint.cpp
 *
 *  Created on: 2010. 5. 28.
 *      Author: KDT
 */

#include "InkPrinter.h"

InkPrinter::InkPrinter()
{
	cout<<"Ink Print constructor"<<endl;

}
InkPrinter::~InkPrinter()
{
	cout<<"Ink Print destructor  "<<endl;

}

int InkPrinter::State()
{
	cout<<"Ink Print State  "<<endl;
	return 1;
}



int InkPrinter::Off()
{
	cout<<"Ink Print Off  "<<endl;
	return 1;
}



int InkPrinter::On()
{
	cout<<"Ink Print On  "<<endl;
	return 1;
}



int InkPrinter::Print()
{
	cout<<"Ink Print Printing  "<<endl;
	return 1;
}

//이것만 추가해주면 된다 (변경)
#define __RETURNCLASS__ InkPrinter
extern "C" void * GetInstance(){
	__RETURNCLASS__ *CreateInstance;
	CreateInstance = new __RETURNCLASS__();
	return (void*)CreateInstance;
}
#undef __RETURNCLASS__



