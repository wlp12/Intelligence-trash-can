#include "wifi.h"
#include "led.h"
int wifi=0;
//客户端1向服务器发送数据
u8 client(void)
{
  u8 ipbuf[16]="192.168.1.53"; 	//输入服务器的ip
	u8 *p;
	WIFI_R=1;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET);	//PB0置1  	   	
	delay_ms(1000);
	WIFI_R=0;
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET);	//PB0置1 
	delay_ms(1000);
	p=mymalloc(SRAMIN,100);							//申请32字节内存
	atk_8266_send_cmd("AT+CWMODE=1","OK",50);		//设置WIFI STA模式
	delay_ms(1000);	
	wifi=1;
	atk_8266_send_cmd("AT+RST","OK",20);		//重启模块 
	delay_ms(1000);         //延时2S等待重启成功
	wifi=2;
	//设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
	atk_8266_send_cmd(p,"WIFI GOT IP",1000);						//连接目标路由器，并获得IP
	delay_ms(1000);
	wifi=3;
  sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器,
	delay_ms(1000);	
	wifi=4;
	atk_8266_send_cmd(p,"OK",1000);
	delay_ms(1000);
	wifi=5;
	atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传	，开启透传
	delay_ms(1000);
	wifi=6;
}






