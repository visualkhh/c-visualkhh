/*
 * wow.cpp
 *
 *  Created on: 2010. 5. 28.
 *      Author: KDT
 */

#include "wow.h"

wow::wow()
{
	printf("wow constructor \n");
	// TODO Auto-generated stub

}

wow::~wow()
{
	printf("wow destructor stub \n");
}

int wow::why()
{
	printf("why\n");
}



int wow::wows()
{
	printf("wow\n");
}

int wow::showString(char *char1)
{
	printf("%s(%d) \n",char1,this->state);
}

int wow::showSuperString()
{
	test::showString();
}

int wow::setstate(int i)
{
	void * t  = this;
	void * tt = &this->state;
	this->state = i;
}

int wow::puString()
{
	printf("wow PU\n");
}








