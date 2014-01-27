/*
 * Struct.h
 *
 *  Created on: 2010. 6. 14.
 *      Author: KDT
 */

#ifndef STRUCT_H_
#define STRUCT_H_

	struct DevItem{
		char devPath[XMLNODEPATH_MAXSIZE];	//디바이스 경로
		char id[XMLNODEVAL_MAXSIZE]; //검증 경로
		int devid;
		char validateId[XMLNODEVAL_MAXSIZE]; //검증 경로
		char getheringActionPath[XMLNODEPATH_MAXSIZE];//액션경로
		int sendSize;		//보낼사이즈
		int receiveSize;	//받아올사이즈
	};


#endif /* STRUCT_H_ */
