/*
 * Class.h
 *
 *  Created on: 2010. 5. 31.
 *      Author: KDT
 */

#ifndef CLASS_H_
#define CLASS_H_

/*
 *
 */
#include <iostream>
#include <dlfcn.h>

typedef void* (*GetInstanceFuncType)();
extern char G_PROGNAME[256];				//프로세스명
extern char G_ARGUMENT[256];				//아귀먼트
class Class
{
public:
	Class();
	virtual ~Class();
	static void *  forName(char* cname);
};

#endif /* CLASS_H_ */
