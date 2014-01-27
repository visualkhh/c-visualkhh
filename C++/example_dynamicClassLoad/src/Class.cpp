/*
 * Class.cpp
 *
 *  Created on: 2010. 5. 31.
 *      Author: KDT
 */

#include "Class.h"

Class::Class() {
	// TODO Auto-generated constructor stub

}

Class::~Class() {
	// TODO Auto-generated destructor stub
}

void *Class::forName(char *ClassName) {
	char *error;
	void *handle;
	handle = dlopen(ClassName, RTLD_LAZY);

	if(!handle){
		std::cerr << "Cannot load library: " << dlerror() << '\n';
		return NULL;
	}
	// Func_Init
	GetInstanceFuncType GetInstance = (GetInstanceFuncType) dlsym(handle,"GetInstance");
	  if ((error = dlerror()) != NULL){
	  printf("ERROR : %s\n",error);
	  return NULL;
	}
	return GetInstance();
}


