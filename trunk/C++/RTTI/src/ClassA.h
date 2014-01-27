/*
 * ClassA.h
 *
 *  Created on: 2010. 1. 18.
 *      Author: KDT
 */

#ifndef CLASSA_H_
#define CLASSA_H_

/*
 *
 */
#include <iostream>
#include "Object.h"
using namespace std;
class ClassA :public Object{
public:
	ClassA();
	virtual ~ClassA();
	static Object * getInstance();
	void getClassName();
};

#endif /* CLASSC_H_ */
