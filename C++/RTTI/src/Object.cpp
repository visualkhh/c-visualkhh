/*
 * Object.cpp
 *
 *  Created on: 2010. 1. 18.
 *      Author: KDT
 */

#include "Object.h"

Object::Object() {
	// TODO Auto-generated constructor stub

}

Object::~Object() {
	// TODO Auto-generated destructor stub
}
Object * Object::getInstance(){
			return new Object();
}
void Object::getClassName(){
	cout<<"Object"<<endl;
}
