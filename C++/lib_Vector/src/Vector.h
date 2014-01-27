/*
 * Vector.h
 *
 *  Created on: 2010. 6. 14.
 *      Author: KDT
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Hinclude.h"
#include <vector>
using namespace std;
using std::vector;

/*
 *
 */
template <class T>
class Vector {
private:
	Utilities Util;
protected:
    	long Position;
		vector<T> VectorList;

public:
	Vector();
	virtual ~Vector();
	bool add(const T item);
	bool clear( );
	T getNexItemtLoop();
	T get(int index);
	int getCount();
	int getNextIndex();
	bool Modify(int index,const T modifyitem);
};
template<class T> inline Vector<T>::Vector()
{
	this->Position = 0;
//	Util.Debug("%s Const",__FUNCTION__);
}



template<class T> inline Vector<T>::~Vector()
{
	clear();
//	Util.Debug("%s disc",__FUNCTION__);
}

template<class T> inline bool Vector<T>::add(const T item)
{
		VectorList.push_back(item);
		Position++;
		return true;
}



template<class T> inline bool Vector<T>::clear()
{
	VectorList.clear();
	return true;
}



template<class T> inline T Vector<T>::getNexItemtLoop()
{
	T tempItem;
	memset(&tempItem, 0x00,  sizeof(T));

	if( VectorList.size() <= 0  )
		return tempItem;

	if( Position >= VectorList.size() ) Position = 0;

	tempItem = VectorList.at(Position);

	Position++;

	return tempItem;
}



template<class T> inline T Vector<T>::get(int index)
{
	T tempItem;
	memset(&tempItem, 0x00,  sizeof(T));

	if( VectorList.size() <= 0  )
		return tempItem;

	if( index >= VectorList.size() ) Position = 0;

	tempItem = VectorList.at(index);

	return tempItem;

}



template<class T> inline int Vector<T>::getCount()
{
	return	VectorList.size();
}



template<class T> inline int Vector<T>::getNextIndex()
{
	return	Position;
}



template<class T> inline bool Vector<T>::Modify(int index,const T modifyitem)
{
	T *tempItemPtr;
		//memset(&aRowMsg, 0x00,  sizeof(__ROW_MSG_KIND));

		if( VectorList.size() <= 0  ) return -1;
		tempItemPtr = &VectorList.at(index);
		memcpy(tempItemPtr,&modifyitem,sizeof(T));


		return true;
}











#endif /* VECTOR_H_ */
