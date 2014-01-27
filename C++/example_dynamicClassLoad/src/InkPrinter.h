/*
 * InkPrint.h
 *
 *  Created on: 2010. 5. 28.
 *      Author: KDT
 */

#ifndef INKPRINT_H_
#define INKPRINT_H_
#include <iostream>
#include "Printer.h"
using namespace std;


/*
 *
 */
class InkPrinter : public Printer
{
public:
	char r[1000];
	InkPrinter();
	virtual ~InkPrinter();
	virtual int 	State();
	virtual int 	Print();
	virtual int 	On();
	virtual int 	Off();
};
#endif /* INKPRINT_H_ */
