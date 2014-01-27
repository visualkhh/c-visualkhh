/*
 * XMLParser.cpp
 *
 *  Created on: 2010. 5. 13.
 *      Author: KDT
 */

#include "XMLParser.h"

XMLParser::XMLParser()
{
	this->doc=NULL;
	memset(this->filePath,0,XMLFILEPATH_MAXSIZE);
//	Util.Debug("%s construct",__FUNCTION__);
}
XMLParser::XMLParser(const char * filePath)
{
	openFile(filePath);
}

bool XMLParser::openFile(const char *filePath)
{
	freeDoc(); //우선 닫고 해라.

	strcpy(this->filePath,filePath);

    xmlNodePtr cur;
    this->doc = xmlParseFile(this->filePath);
    if (this->doc == NULL ) {
            Util.Debug("Document not parsed successfully.");
            return false;
    }

    cur = xmlDocGetRootElement(this->doc);
      if (cur == NULL) {
			  Util.Debug("empty document");
              xmlFreeDoc(this->doc);
              return true;
      }
      Util.Debug("openDocXml:(%s) Root Name :  %s",this->filePath,cur->name);
      return true;
}

XMLParser::~XMLParser(){
	freeDoc();
}
void XMLParser::freeDoc(){
	if(this->doc!=NULL){
	xmlFreeDoc(this->doc);
	xmlCleanupParser();
	this->doc=NULL;
//	Util.Debug("\nfreeDocXML : (%s)",this->filePath);
	}
}int XMLParser::getDepth(const char *nodePath){
	int depth = 0;
	char tempPath [XMLNODEPATH_MAXSIZE]={0,};
	strcpy(tempPath,nodePath);
	char	*ptr;
	ptr = strtok(tempPath, "/");
	depth++;
	 char* temp = strtok( NULL, "/");
	while( temp )
	{
		ptr = temp;
		depth++;
		temp = strtok( NULL, "/");
	}
	return depth;
}
void XMLParser::getLastNodeName(const char *nodePath,char* rnodeName){
	char * lastNodeName = strrchr(nodePath,'/');
	strcpy(rnodeName,lastNodeName+1);
}
bool XMLParser::getAttrName( char * nodePath,char* rattrName){
	if(Util.u_String_GetSplitStrB(nodePath,'@',1,rattrName)==false)
		return false;

	int strLength = strlen(nodePath);
	char * temppath=new char[strLength];
	if(Util.u_String_GetSplitStrF(nodePath,'@',1,temppath)==false){
		delete[]temppath;
		return false;
	}else{
		strcpy(nodePath,temppath);
		delete[]temppath;
		return true;
	}

}


xmlNodePtr XMLParser::moveNode_otherDepth(const char* nodePath){

	xmlNodePtr cur=NULL;
	cur = xmlDocGetRootElement(this->doc);

	char tempPath[XMLNODEPATH_MAXSIZE]={0,};
	strcpy(tempPath,nodePath);
	char * ptr = NULL;


	ptr = strtok(tempPath, "/");
	if(ptr == NULL){
			cout<<"isNode Not Find Search"<<endl;
			return cur;
	}

		cur = cur->xmlChildrenNode;  //root->Children

		cur = moveNode_SameDepth(cur,ptr);
		if(cur == NULL)
			return NULL;



		char* temp = strtok( NULL, "/");
		while( temp )
		{

			ptr = temp;
			cur = cur->xmlChildrenNode;
			cur = moveNode_SameDepth(cur,ptr);

			if(cur==NULL)
				return NULL;


			temp = strtok( NULL, "/");
		}


	return cur;
}

xmlNodePtr XMLParser::moveNode_SameDepth(xmlNodePtr cur,const char* searchNodeName){

		int strlength 		=	strlen(searchNodeName);
		char * tempStr 		= 	new char[strlength];
		const char * nodeName		=	NULL;
		int index 			= 	Util.u_String_getArrayNum(searchNodeName);
		bool cs = Util.u_String_GetSplitStrF(searchNodeName,'[',1,tempStr);

		if(cs==false){  //[ 인덱스가없다..
			delete[] tempStr;
			tempStr=NULL;
			nodeName = searchNodeName;
		}else{ //인덱스가없다
			nodeName = tempStr;
		}
//		Util.Debug("%s  %d=======SamDepth",searchNodeName,index);


//		cout<<"moveNodeSamDepth   "<<cur->name<<"       \n"
//				"serarchNodeName  " <<searchNodeName<<"    \n"
//				"isSpli?"<<rt <<"   Str  "<<tempStr<<endl;

//		cout<<"----index----] "<<index<<endl;


	   while (cur != NULL) {
		   //text(3) comment(8)노드는 세아리지않습니다.
			if(cur->type==8 || cur->type==3){
				cur = cur->next;
				continue;
			}

	    	 if((!xmlStrcmp(cur->name, (const xmlChar *) nodeName))) {

	    		 if(index<=1){

//	    			 char temp[10];
//	    			 getNodeVal(cur,temp);
//	    			 cout<<index<<"  index Return---  name: "<<cur->name<<"  : "<<temp<<endl;
					 return cur;
	    		 }
	    		 index--;
	    		 char temp[10];
	    		 getNodeVal(cur,temp);
//				 cout<<index<<"  index bypass---  name: "<<cur->name<<"  : "<<temp<<endl;

	    	 }
	    	 cur = cur->next;
	    }

	return NULL;
}
int XMLParser::getPathCount_SamDepth(xmlNodePtr cur,const char* searchNodeName){
	int count = 0 ;
		   while (cur != NULL) {
			   //text(3) comment(8)노드는 세아리지않습니다.
				if(cur->type==8 || cur->type==3){
					cur = cur->next;
					continue;
				}
		    	 if((!xmlStrcmp(cur->name, (const xmlChar *) searchNodeName))) {
		    		 count++;
		    	 }
		    	 cur = cur->next;
		    }
		return count;
}

int XMLParser::getPathChildLength(xmlNodePtr cur){
	int count = 0 ;
			   while (cur != NULL) {
				   //text(3) comment(8)노드는 세아리지않습니다.
					if(cur->type==8 || cur->type==3){
						cur = cur->next;
						continue;
					}
			    	 count++;
			    	 cur = cur->next;
			    }
			return count;
}



bool XMLParser::getNodeVal(xmlNodePtr cur,char * rValue){
	xmlChar *key=NULL;
	key = xmlNodeListGetString(this->doc, cur->xmlChildrenNode, 1);
	if(key==NULL)
		return false;
	strcpy(rValue,(char*)key);
	xmlFree(key);
	return true;
}


bool XMLParser::getNodeAttr(xmlNodePtr cur, const char* attrName,char * rValue){


	char * tempAttr = NULL;

	tempAttr = (char *) xmlGetProp( cur, (const xmlChar*)attrName );
	if(tempAttr == NULL){
		return false;
	}
	strcpy(rValue,tempAttr);

	return true;

//
//	xmlChar *key;
//	key = xmlNodeListGetString(this->doc, cur->xmlChildrenNode, 1);
//	strcpy(rValue,(char*)key);
//	xmlFree(key);
//	return 1;
}
bool XMLParser::filter(const char* path){



	if(this->doc==NULL){
		Util.Debug("Not file Open");
		return false;
	}


	if(path[0]=='/'){
		if(strlen(path)>0){
			Util.Debug("Input Path = (%s)", path);
			return true;
		}
	}
	Util.Debug("filter Out PathNo!format  ex) /asd/asd[1]/as@asd");
	return false;

}
bool XMLParser::getPathNodeVal_u(const char *nodePath,char * rValue)
{
//	cout<<"getPathNodeVal : "<<nodePath<<endl;
	if(filter(nodePath)==false)
		return false;
	xmlNodePtr cur=NULL;
	char tempPath [XMLNODEPATH_MAXSIZE]={0,};
	char tempAttr [XMLNODEANAME_MAXSIZE]={0,};
	strcpy(tempPath,nodePath);
	bool isAttr = 0;
	//마지막의 @로 선언된 속성이름을 가져온다. 그리고 원래path안에있는  @속성도 없에준다
	isAttr = getAttrName(tempPath,tempAttr);
//	Util.Debug("tempPath   (1)  %s ",tempPath);
//	if(isAttr==true)
//	Util.Debug("tempPath (%s) tempAttr   (2)  %s ",tempPath,tempAttr);
	cur = moveNode_otherDepth(tempPath);
	if(cur == NULL){
		Util.Debug("Not Find Node");
		return false;
	}

	bool result=0;
	if(isAttr == false){
		result = getNodeVal(cur,rValue);
		if (result == false)
		return false;
	}else if (isAttr == true){
		result = getNodeAttr(cur,tempAttr,rValue);
		if(result==false)
		return true;
	}
	return true;
}
bool XMLParser::getPathNodeVal_u(const char *nodePath,int * rValue)
{

	bool rv = false;
	char tempNodeVal[XMLNODEVAL_MAXSIZE]={0,};
	rv = this->getPathNodeVal_u(nodePath,tempNodeVal);
	if(rv == false)
	return false;

	*rValue = atoi(tempNodeVal);
	return true;
}
bool XMLParser::getPathNodeVal_u(const char *nodePath,float * rValue)
{
	bool rv = false;
	char tempNodeVal[XMLNODEVAL_MAXSIZE]={0,};
	rv = this->getPathNodeVal_u(nodePath,tempNodeVal);
	if(rv == false)
	return false;

	*rValue = atof(tempNodeVal);
	return true;
}

int XMLParser::getPathChildLength_u(const char* nodePath){

	if(filter(nodePath)==false)
		return false;


	xmlNodePtr cur=NULL;
	char tempPath [XMLNODEPATH_MAXSIZE]= {0,};
	char tempAttr [XMLNODEANAME_MAXSIZE] = {0,};

	strcpy(tempPath,nodePath);

	int isAttr = 0;
	//마지막의 @로 선언된 속성이름을 가져온다. 그리고 원래path안에있는  @속성도 없에준다
	isAttr = getAttrName(tempPath,tempAttr);


//	util.Debug("tempPath   (1)  %s ",tempPath);
//	if(isAttr==true)
//	util.Debug("tempAttr   (2)  %s ",tempAttr);

	cur = moveNode_otherDepth(tempPath);
	if(cur == NULL)
	return 0;

	int count =  getPathChildLength(cur->children);
	return count;


}
int XMLParser::getPathCount_SamDepth_u(const char* nodePath){

	if(filter(nodePath)==false)
		return false;


	xmlNodePtr cur=NULL;
	char tempPath [XMLNODEPATH_MAXSIZE]= {0,};
	char lastName [XMLNODEANAME_MAXSIZE] = {0,};
	char tempAttr [XMLNODEANAME_MAXSIZE] = {0,};

	strcpy(tempPath,nodePath);

	getLastNodeName(tempPath,lastName);
//	Util.Debug("lastName  %s ",lastName);

	int isAttr = 0;
	//마지막의 @로 선언된 속성이름을 가져온다. 그리고 원래path안에있는  @속성도 없에준다
	isAttr = getAttrName(tempPath,tempAttr);


//	util.kDebug("tempPath   (1)  %s ",tempPath);
//	if(isAttr==true)
//	Util.Debug("tempPath, (%s)  tempAttr   (2)  %s ",tempPath,tempAttr);

	cur = moveNode_otherDepth(tempPath);
	if(cur == NULL)
	return 0;

	int count =  getPathCount_SamDepth(cur,lastName);
	return count;
}

//format 형식의 ...path.......
bool XMLParser::getPathNodeVal(const char* nodePath_format, char* rValue ,...){
	va_list args;
	va_start(args, nodePath_format);
	va_end(args);
	char *temp = new char[XMLNODEPATH_MAXSIZE];
	memset(temp,'\0',XMLNODEPATH_MAXSIZE);
	vsprintf (temp, nodePath_format, args);
	bool rv = getPathNodeVal_u(temp,rValue);
	delete[]temp;
	return rv;
}
bool XMLParser::getPathNodeVal(const char* nodePath_format,int* rValue, ...){
	va_list args;
	va_start(args, nodePath_format);
	va_end(args);
	char *temp = new char[XMLNODEPATH_MAXSIZE];
	memset(temp,'\0',XMLNODEPATH_MAXSIZE);
	vsprintf (temp, nodePath_format, args);
	bool rv = getPathNodeVal_u(temp,rValue);
	delete[]temp;
	return rv;
}
bool XMLParser::getPathNodeVal(const char* nodePath_format,float* rValue, ...){
	va_list args;
	va_start(args, nodePath_format);
	va_end(args);
	char *temp = new char[XMLNODEPATH_MAXSIZE];
	memset(temp,'\0',XMLNODEPATH_MAXSIZE);
	vsprintf (temp, nodePath_format, args);
	bool rv = getPathNodeVal_u(temp,rValue);
	delete[]temp;
	return rv;
}
int  XMLParser::getPathChildLength(const char* nodePath_format, ...){
	va_list args;
	va_start(args, nodePath_format);
	va_end(args);
	char *temp = new char[XMLNODEPATH_MAXSIZE];
	memset(temp,'\0',XMLNODEPATH_MAXSIZE);
	vsprintf (temp, nodePath_format, args);
	int rv = getPathChildLength_u(temp);
	delete[]temp;
	return rv;
}
int  XMLParser::getPathCount_SamDepth(const char* nodePath_format, ...){
	va_list args;
	va_start(args, nodePath_format);
	va_end(args);
	char *temp = new char[XMLNODEPATH_MAXSIZE];
	memset(temp,'\0',XMLNODEPATH_MAXSIZE);
	vsprintf (temp, nodePath_format, args);
	int rv = getPathCount_SamDepth_u(temp);
	delete[]temp;
	return rv;
}




