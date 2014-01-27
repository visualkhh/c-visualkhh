/*
 * wow.h
 *
 *  Created on: 2010. 5. 28.
 *      Author: KDT
 */

#ifndef WOW_H_
#define WOW_H_

/*
 *
 */
#include <stdio.h>
#include "test.h"
class wow :public test
{
public:
	int state;
	wow();
	virtual ~wow();
	int why();
	int wows();
	int showString(char * );
	int showSuperString();
	int setstate(int i);
	int puString();
};

#endif /* WOW_H_ */
