#ifndef _WIFI_H
#define _WIFI_H	

#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   	 
#include "key.h"	 	 	 	 	 	 
#include "malloc.h"
#include "string.h"    
#include "usart.h" 


void atk_8266_at_response(u8 mode);
u8* atk_8266_check_cmd(u8 *str);
u8 atk_8266_apsta_check(void);
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime);
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 atk_8266_quit_trans(void);



u8 client(void);	//WIFI AP+STAģʽ����


//�û����ò���
extern const u8* portnum;			//���Ӷ˿�
 
extern const u8* wifista_ssid;		//WIFI STA SSID
extern const u8* wifista_encryption;//WIFI STA ���ܷ�ʽ
extern const u8* wifista_password; 	//WIFI STA ����

extern const u8* wifiap_ssid;		//WIFI AP SSID
extern const u8* wifiap_encryption;	//WIFI AP ���ܷ�ʽ
extern const u8* wifiap_password; 	//WIFI AP ����

extern u8* kbd_fn_tbl[2];
extern const u8* ATK_ESP8266_CWMODE_TBL[3];
extern const u8* ATK_ESP8266_WORKMODE_TBL[3];
extern const u8* ATK_ESP8266_ECN_TBL[5];
#endif


