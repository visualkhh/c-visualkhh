/*
 * Object.h
 *
 *  Created on: 2010. 1. 18.
 *      Author: KDT
 */

#ifndef OBJECT_H_
#define OBJECT_H_

/*
 *
 */
#include <iostream>
using namespace std;
class Object {
public:
	Object();
	virtual ~Object();
	static Object * getInstance();
	virtual void getClassName();
};

#endif /* Object_H_ */
