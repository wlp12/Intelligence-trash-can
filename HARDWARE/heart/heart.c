#include "heart.h"


int total_Buffer[32];

typedef struct
 {
   short  moduleId;
	 short  cmdId;
	 short  dataLen;
 } Head;
 
 Head  aHead;
 Head *sptr=&aHead;
 
typedef struct
 {
    short  check;
 } End;
 

//enum CmdId
//{   
//}

enum check
{
  ture   =0x0101,
	wrong  =0x1010,
};
 

void  Data( short Id , short  cmdId , short  dataLen ,int pBuffer[16] )
{
	int i;

	sptr->moduleId=Id;
	sptr->cmdId=cmdId;
	sptr->dataLen=dataLen;
	
  total_Buffer[0]=sptr->moduleId;
  total_Buffer[1]=sptr->cmdId;
	total_Buffer[2]=sptr->dataLen;
	total_Buffer[sptr->dataLen-1]=ture;
	
	for(i=3;i<=18;i++)
	{ 
	total_Buffer[i] = pBuffer[i-3];
  }
	
}














 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 