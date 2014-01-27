#ifndef PRINT_H_
#define PRINT_H_
#include "Object.h"
class Printer :public Object{

public:
	Printer(){};//이거꼭좀해줘야함 body가없음 여기서처리
	virtual ~Printer(){};  //이거꼭좀해줘야함 body가없음 여기서처리

	virtual int State()=0;
	virtual int Print()=0;
	virtual int On()=0;
	virtual int Off()=0;

private:

};
#endif /* INKPRINT_H_ */
