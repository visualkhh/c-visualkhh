/* visualhhk
 * XMLParser.h
 *
 *  Created on: 2010. 5. 13.
 *      Author: KDT
 */

#ifndef XMLPARSER_H_
#define XMLPARSER_H_
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stdlib.h"
#include "stdarg.h"
#include <iconv.h>
#include <libxml/parser.h>
#include "Utilities.h"
using namespace std;
class XMLParser
{
private:

	char filePath[XMLFILEPATH_MAXSIZE];
	 xmlDocPtr doc;
	 Utilities Util;



	 //현재 path 의 뎁스를 구합합니다.
	 int getDepth(const char * nodePath);
	 //현재 path의 마지막  페스이름을구합니다.
	 void getLastNodeName(const char * nodePath,char* rnodeName);
	 //현재 path의 @로구분된 속성이름 를 가져옴니다.. 그리고 nodePath 뒤에 @속성이있으면 짤라서 준다 즉 원 path로 돌려준다
	 bool getAttrName(char * nodePath,char* rattrName);
	 //노드객체의 내부txt를 가져옴니다.
	 bool getNodeVal(xmlNodePtr cur,char* rValue);
	 //노드객체의 내부 속성값을 가져옴니다.
	 bool getNodeAttr(xmlNodePtr cur, const char* attrName, char* rValue);
	 //해당노드로 뎁스이동을 합니다.
	 xmlNodePtr moveNode_otherDepth(const char* nodePath);
	 //같은뎁스에서 한단계씩 이동합니다.. 해당이름이랑 같은걸 찾으로 없으면 NULL
	 xmlNodePtr moveNode_SameDepth(xmlNodePtr cur,const char* searchNodeName);
	 //해당 노드의 개수를 (같은뎁스) 반환합니다.
	 int getPathCount_SamDepth(xmlNodePtr cur,const char* searchNodeName);
	 //해당 노드의 자식개수를 반환합니다.
	 int getPathChildLength(xmlNodePtr cur);

	 //들어온 path의 필터를 검니다. 모든 입출구쪽은 처음이걸 가져다쓰세요
	 bool filter(const char* path);

	 //using function!
	bool getPathNodeVal_u(const char* nodePath,char* rValue);
	bool getPathNodeVal_u(const char* nodePath,int* rValue);
	bool getPathNodeVal_u(const char* nodePath,float* rValue);
	int getPathChildLength_u(const char* nodePath);
	int getPathCount_SamDepth_u(const char* nodePath);
public:
	XMLParser();
	XMLParser(const char * filePath);
	bool openFile(const char *filePath);
	void freeDoc();
	virtual ~XMLParser();


	//using funtion format!
	bool getPathNodeVal(const char* nodePath_format,char* rValue, ...);
	bool getPathNodeVal(const char* nodePath_format,int* rValue, ...);
	bool getPathNodeVal(const char* nodePath_format,float* rValue, ...);
	int getPathChildLength(const char* nodePath_format, ...);
	int getPathCount_SamDepth(const char* nodePath_format, ...);

};

#endif /* XMLPARSER_H_ */
