#ifndef _DATABOX_H
#define _DATABOX_H


#define  Rdata_tip     0x72  //��r�ַ�
#define  Tdata_tip     0x74  //��t�ַ�

#define  Address_1     0x47 
#define  Address_2     0x48
#define  Address_3     0x49

#define  command_1     0x75 //�鿴���ְ�
#define  command_2     0x76
#define  command_3     0x77

typedef struct 
 {
  unsigned 	char funtion_part;
	int  data_length;
	unsigned   address;	  
//	char value_data[];
 }DATABOX;



#endif
