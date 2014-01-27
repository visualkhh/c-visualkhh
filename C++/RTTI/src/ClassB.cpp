/*
 * ClassB.cpp
 *
 *  Created on: 2010. 1. 18.
 *      Author: KDT
 */

#include "ClassB.h"

ClassB::ClassB() {
	// TODO Auto-generated constructor stub

}

ClassB::~ClassB() {
	// TODO Auto-generated destructor stub
}
Object * ClassB::getInstance(){
			return new ClassB();
}
void ClassB::getClassName(){
	cout<<"ClassB"<<endl;
}
