/*
 * test.cpp
 *
 *  Created on: 2010. 5. 28.
 *      Author: KDT
 */

#include "test.h"

test::test()
{
	void *t = this;
	void *tt = t;
	printf("test constructor stub\n");
	strcpy(this->t,"test constructor stub");
}

test::~test()
{
	printf("test destructor stub \n");
}

int test::showString()
{
	printf("%s \n",this->t);
}

int test::puString()
{
	printf("Test PU\n");
}





