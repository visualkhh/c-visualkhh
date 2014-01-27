/*
 * ClassC.cpp
 *
 *  Created on: 2010. 1. 18.
 *      Author: KDT
 */

#include "ClassC.h"

ClassC::ClassC() {
	// TODO Auto-generated constructor stub

}

ClassC::~ClassC() {
	// TODO Auto-generated destructor stub
}
Object * ClassC::getInstance(){
			return new ClassC();
}
void ClassC::getClassName(){
	cout<<"ClassC"<<endl;
}
