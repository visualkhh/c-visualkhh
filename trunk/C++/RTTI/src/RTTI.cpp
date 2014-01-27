//============================================================================
// Name        : RTTI.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string.h>
#include "Object.h"
#include "ClassA.h"
#include "ClassB.h"
#include "ClassC.h"
using namespace std;

int rtticnt = 0;
struct rttirow
{
	char * ClassName;
	Object *Instance;
};
struct rttiTable
{
	rttirow row[100];
};
struct rttiTable RTTI_TABLE;

void RegisterCls(char *ClassName, Object * Init)
{

	RTTI_TABLE.row[rtticnt].ClassName = ClassName;
	RTTI_TABLE.row[rtticnt].Instance = Init;
	rtticnt++;
}

Object *ClassForName(char *ClassName)
{
	for (int i = 0; i < rtticnt; i++)
	{
		if (strcmp(RTTI_TABLE.row[i].ClassName, ClassName) == 0)
		{
			return RTTI_TABLE.row[i].Instance;
		}
	}
	return NULL;
}

int main()
{
	cout << "hello" << endl;
	RegisterCls("ClassAA", ClassA::getInstance());
	RegisterCls("ClassBB", ClassB::getInstance());
	RegisterCls("ClassCC", ClassC::getInstance());
	Object *rcvObject = ClassForName("ClassAA");
	rcvObject->getClassName();
	return 0;
}
