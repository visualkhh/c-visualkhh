/*
 * ClassA.cpp
 *
 *  Created on: 2010. 1. 18.
 *      Author: KDT
 */

#include "ClassA.h"

ClassA::ClassA() {
	// TODO Auto-generated constructor stub

}

ClassA::~ClassA() {
	// TODO Auto-generated destructor stub
}
Object * ClassA::getInstance(){

	return new ClassA();
}
void ClassA::getClassName(){

	cout<<__PRETTY_FUNCTION__<<endl;
	cout<<__LINE__<<endl;
	cout<<__FILE__<<endl;
	cout<<__DATE__<<endl;
	cout<<__TIME__<<endl;
	cout<<__STDC__<<endl;



	cout<<"ClassA"<<endl;
}
