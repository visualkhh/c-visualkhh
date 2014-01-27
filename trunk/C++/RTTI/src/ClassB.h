/*
 * ClassB.h
 *
 *  Created on: 2010. 1. 18.
 *      Author: KDT
 */

#ifndef CLASSB_H_
#define CLASSB_H_

/*
 *
 */
#include <iostream>
#include "Object.h"
using namespace std;
class ClassB :public Object {
public:
	ClassB();
	virtual ~ClassB();
	static Object * getInstance();
	void getClassName();
};

#endif /* CLASSB_H_ */
