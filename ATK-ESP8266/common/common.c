#include "wifi.h"
#include "led.h"
int wifi=0;
//�ͻ���1���������������
u8 client(void)
{
  u8 ipbuf[16]="192.168.1.53"; 	//�����������ip
	u8 *p;
	WIFI_R=1;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB0��1  	   	
	delay_ms(1000);
	WIFI_R=0;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);	//PB0��1 
	delay_ms(1000);
	p=mymalloc(SRAMIN,100);							//����32�ֽ��ڴ�
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//����WIFI STAģʽ
	delay_ms(1000);	
	wifi=1;
	atk_8266_send_cmd("AT+RST","OK",20);		//����ģ�� 
	delay_ms(1000);         //��ʱ2S�ȴ������ɹ�
	wifi=2;
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	atk_8266_send_cmd(p,"WIFI GOT IP",1000);						//����Ŀ��·�����������IP
	delay_ms(1000);
	wifi=3;
  sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������,
	delay_ms(1000);	
	wifi=4;
	atk_8266_send_cmd(p,"OK",1000);
	delay_ms(1000);
	wifi=5;
	atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��	������͸��
	delay_ms(1000);
	wifi=6;
}






