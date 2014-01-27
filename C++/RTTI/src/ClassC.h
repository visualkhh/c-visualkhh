/*
 * ClassC.h
 *
 *  Created on: 2010. 1. 18.
 *      Author: KDT
 */

#ifndef CLASSC_H_
#define CLASSC_H_

/*
 *
 */
#include <iostream>
#include "Object.h"
using namespace std;
class ClassC :public Object {
public:
	ClassC();
	virtual ~ClassC();
	static Object * getInstance();
	void getClassName();
};

#endif /* CLASSC_H_ */
