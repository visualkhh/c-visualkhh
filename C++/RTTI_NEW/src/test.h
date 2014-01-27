/*
 * test.h
 *
 *  Created on: 2010. 5. 28.
 *      Author: KDT
 */

#ifndef TEST_H_
#define TEST_H_

/*
 *
 */
#include <stdio.h>
#include <string.h>
#include "supertest.h"

class test : public supertest
{
	char t[50];
public:
	test();
	virtual ~test();
	int  showString();
	virtual int puString();
};

#endif /* TEST_H_ */
