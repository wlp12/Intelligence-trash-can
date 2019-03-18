#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "tftlcd.h"
#include "adc.h"
#include "pcf8574.h"
#include "timer.h"
#include "sdram.h"
#include "malloc.h"
#include "touch.h"
#include "rtc.h"
#include "GUI.h"
#include "WM.h"
#include "radiodemo.h"
#include "includes.h"
#include "DIALOG.h"
#include "My_gui.h"
#include "usart2.h"
#include "w25qxx.h"
#include "text1.h"
#include "fontupd.h"
#include "as608.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "usmart.h"
#include "usart2.h"
#include "string.h"
#include "sdio_sdcard.h"
#include "wifi.h"
#include "stdlib.h"
#include "usart3.h"

int ID,ID1;
int weight[18],WEIGHT[2];
int lcd_discolor[4]={	BLACK,GRAY,BLUE,RED};
u16 get(int t);
u16 wave_scan(void);
extern int YES,num,flag,value_1,exhibition,exhibition_1,acv_flag,esp_back,wifi,Look,esp_reset;
extern u8  show,interface,res1,my_result[23],k,j;
extern u32 input_number[11];
extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];
void image_interface(int m, int n);


//ָ��ģ�鲿��
#define usart2_baund  57600//����2�����ʣ�����ָ��ģ�鲨���ʸ���

SysPara AS608Para;//ָ��ģ��AS608����
u8** kbd_tbl;
u16 ValidN;//ģ������Чָ�Ƹ���
u16 GET_NUM(void);//��ȡ��ֵ

void press_FR(void);//ˢָ��
void scoreboard(void);



//�������ȼ�
#define START_TASK_PRIO				3
//�����ջ��С	
#define START_STK_SIZE 				128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//TOUCH����
//�����������ȼ�
#define TOUCH_TASK_PRIO				4
//�����ջ��С
#define TOUCH_STK_SIZE				128
//������ƿ�
OS_TCB TouchTaskTCB;
//�����ջ
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch����
void touch_task(void *p_arg);

//EMWINDEMO����
//�����������ȼ�
#define EMWINDEMO_TASK_PRIO			5
//�����ջ��С
#define EMWINDEMO_STK_SIZE			512
//������ƿ�
OS_TCB EmwindemoTaskTCB;
//�����ջ
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task����
void emwindemo_task(void *p_arg);

//�����������ȼ�
#define Showtime_TASK_PRIO			6
//�����ջ��С
#define Showtime_STK_SIZE			1024
//������ƿ�
OS_TCB Showtime_TaskTCB;
//�����ջ
CPU_STK Showtime_TASK_STK[Showtime_STK_SIZE];
//emwindemo_task����
void Showtime_task(void *p_arg);



//�����������ȼ�
#define TIME_TASK_PRIO			6
//�����ջ��С
#define TIME_STK_SIZE			3000
//������ƿ�
OS_TCB TIMETaskTCB;
//�����ջ
CPU_STK TIME_TASK_STK[TIME_STK_SIZE];
//emwindemo_task����
void TIME_task_1(void *p_arg);


//�����������ȼ�
#define Getdata_TASK_PRIO			6
//�����ջ��С
#define Getdata_STK_SIZE			256
//������ƿ�
OS_TCB Getdata_TaskTCB;
//�����ջ
CPU_STK Getdata_TASK_STK[Getdata_STK_SIZE];
//emwindemo_task����
void Getdata_task(void *p_arg);


void image_interface(int m, int n)
{
	int length=100,height=160,p,q;
  p=(m+1)*40;
	q=40*(n+1);
	LCD_DrawRectangle(49,49, 51+length, 51+height);
	LCD_Fill(51,51+height-p, 49+length, 49+height,lcd_discolor[m]);				//���ָ����ɫ
	LCD_DrawRectangle(399,49,401+length,51+height);			
	LCD_Fill(401,51+height-q,399+length,49+height,lcd_discolor[n]);				//���ָ����ɫ
}



int main(void)
{
    OS_ERR err;
	  CPU_SR_ALLOC(); 
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz   
    HAL_Init();                     //��ʼ��HAL��
    delay_init(180);                //��ʼ����ʱ����
    uart_init(115200);              //��ʼ��USART
    LED_Init();                     //��ʼ��LED 
    KEY_Init();                     //��ʼ������
    PCF8574_Init();                 //��ʼ��PCF8574
    SDRAM_Init();                   //SDRAM��ʼ��
    TFTLCD_Init();  		        //LCD��ʼ��
	  TIM2_PWM_Init(500-1,90-1);
    TP_Init();				        //��������ʼ�
	  usart3_init(115200);				//����3��ʼ�� 
    usart2_init(usart2_baund);//��ʼ������2,������ָ��ģ��ͨѶ
	  usmart_dev.init(168);		//��ʼ��USMART
	  PS_StaGPIO_Init();	//��ʼ��FR��״̬����
		W25QXX_Init();			//��ʼ��W25Q128
    my_mem_init(SRAMIN);		    //��ʼ���ڲ��ڴ��
	  my_mem_init(SRAMEX);		    //��ʼ���ⲿ�ڴ��
	  my_mem_init(SRAMCCM);		    //��ʼ��CCM�ڴ��
	  exfuns_init();			//Ϊfatfs��ر��������ڴ�  
 	  f_mount(fs[1],"1:",1);  //����FLASH.
    while(font_init()) 			//����ֿ�
	  {	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ	
		delay_ms(200);   
  	}	
    OSInit(&err);		            //��ʼ��UCOSIII
	  OS_CRITICAL_ENTER();            //�����ٽ���
	//������ʼ����
	  OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	            //�˳��ٽ���	 
	OSStart(&err);                  //����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	//ʹ��ʱ��Ƭ��ת���ȹ���,����Ĭ�ϵ�ʱ��Ƭ����
      OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);    //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms 
      OSTaskTimeQuantaSet(&Task1_TaskTCB,1,&err);
//      OSTaskTimeQuantaSet(&TIMETaskTCB,1,&err);	
      OSTaskTimeQuantaSet(&Getdata_TaskTCB,,&err);
      OSTaskTimeQuantaSet(&Showtime_TaskTCB,1,&err);
#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//ʹ��CRCʱ��
    WM_SetCreateFlags(WM_CF_MEMDEV); //�������д��ڵĴ洢�豸
	GUI_Init();  			//STemWin��ʼ��
	WM_MULTIBUF_Enable(1);  //����STemWin�໺��,RGB�����ܻ��õ�
	OS_CRITICAL_ENTER();	//�����ٽ���
	//STemWin Demo����	
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,		
				 (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//����������
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	
	OSTaskCreate((OS_TCB*     )&Showtime_TaskTCB,		
				 (CPU_CHAR*   )"Showtime task", 		
                 (OS_TASK_PTR )Showtime_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )Showtime_TASK_PRIO,     
                 (CPU_STK*    )&Showtime_TASK_STK[0],	
                 (CPU_STK_SIZE)Showtime_STK_SIZE/10,	
                 (CPU_STK_SIZE)Showtime_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	
  OSTaskCreate((OS_TCB*     )&Getdata_TaskTCB,		
				 (CPU_CHAR*   )"Getdata task", 		
                 (OS_TASK_PTR )Getdata_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )Getdata_TASK_PRIO,     
                 (CPU_STK*    )&Getdata_TASK_STK[0],	
                 (CPU_STK_SIZE)Getdata_STK_SIZE/10,	
                 (CPU_STK_SIZE)Getdata_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
									 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//EMWINDEMO����
void emwindemo_task(void *p_arg)
{

	GUI_CURSOR_Show(); 	
	//����Ƥ��
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX); 

	while(1)
	{	
	 my_gui_create();
	}
}

//TOUCH����
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms
	}
}

//������ʾ��������
void Showtime_task(void *p_arg)
{	
	OS_ERR err;	
	u8 ensure;
	int i;
	char *str;
	while(1)
	{	 
	if(interface==1)//����1
		 {
			 exhibition=0; exhibition_1=0; acv_flag=0; ID=0;ID1=0;Look=0;esp_reset=0;esp_back=1;;	//�����־λ           
	   }		
	if(interface==2)  //����2
		 {	
			exhibition=0; exhibition_1=0; acv_flag=0; ID=0;ID1=0;Look=0;esp_reset=0;esp_back=1;;	//�����־λ  
			 for(i=0;i<11;i++)		{ LCD_ShowxNum(150+60*i,75,input_number[i],1,24,0);	}	
			 if(YES==1)  
			  {				
		     POINT_COLOR=RED;
				 LCD_ShowString(300,100,300,10,24,"No this number");
				 POINT_COLOR=BLACK;
			  }		
		}	
  if(interface==3)//����3
		{  	
      ID=0;ID1=0;exhibition=0;exhibition_1=0;acv_flag=0;esp_reset=0;Look=0;esp_back=1;	//�����־λ		 			  						 			
	  }	
	if(interface==4)//����4
    {	
			LCD_ShowxNum(300,300,wifi,4,16,0);
	    POINT_COLOR=BLUE; 
			LCD_ShowString(20,140,300,10,16,"error:");
      while(PS_HandShake(&AS608Addr))//��AS608ģ������
      {
       Show_Str_Mid(20,50,"��ָ��ģ������..",16,240);		  
      }
      Show_Str_Mid(20,80,"ͨѶ�ɹ�!!!",16,240);
      str=mymalloc(SRAMIN,30);
      sprintf(str,"������:%d   ��ַ:%x",usart2_baund,AS608Addr);
      Show_Str(20,110,240,16,(u8*)str,16,0);	
	    ensure=PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	    if(ensure!=0x00)     
			LCD_ShowxNum(80,140,ensure,4,16,0);		
	    ensure=PS_ReadSysPara(&AS608Para);  //������ 
	    if(ensure==0x00)
	    {			 
		   mymemset(str,0,50);
		   sprintf(str,"������:%d    �Աȵȼ�: %d",AS608Para.PS_max-ValidN,AS608Para.PS_level);
		   Show_Str(20,170,240,16,(u8*)str,24,0);
	     }
	    else
		   LCD_ShowxNum(80,140,ensure,4,16,0);					
	     myfree(SRAMIN,str);    				
		   press_FR();//ˢָ��
			if(interface==4)
			{ 
			 LCD_ShowString(60,280,300,10,24,"Owner ID:");
       LCD_ShowxNum(180,280,ID1,4,24,0);
			}				
			if(exhibition_1==1)
			{
			
				printf("%s\r\n",my_result);	//��������
			 LCD_ShowString(250,430,400,12,12,my_result);
			 scoreboard();
			}	  
     }
			 OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ	
	   }			
	 }
	 
void Getdata_task(void  *p_arg) 
{
	OS_ERR  err;
	while(1)
	 {	 		
//		 if(esp_back==1)
//			{
//			  atk_8266_quit_trans();	//�˳�͸��
//	      atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
//				esp_back=0;
//			}
	  if(interface==4)//����4
		 { 		 
			 TIM_SetTIM2Compare( 50,1);
       TIM_SetTIM2Compare(150,2);
			 esp_back=0;
			 if(esp_reset==1)
			  {
			   client();				
         delay_ms(200);     
		     esp_reset=0;
			  } 			 			 
		   if(Look==1)
			  {
			   atk_8266_send_cmd("AT+CIPSEND","OK",200);  //����ָ�����ȵ����ݣ���ʼ͸��	
      	 delay_ms(200);
	       wifi=7;				 
				 Look=0;
			  }
			if(exhibition==1)
			  {		
				 u3_printf("ok=%d\r\n",ID1);	//��������
				 exhibition=0;
			  }
	   }	   
    OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms	
   }
}


//ˢָ��
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		PCF8574_WriteBit(BEEP_IO,0);//�򿪷�����	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			PCF8574_WriteBit(BEEP_IO,1);//�رշ�����	
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//�����ɹ�
			{				
				Show_Str_Mid(0,250,"ˢָ�Ƴɹ�",24,240);				
				str=mymalloc(SRAMIN,50);
				ID1=seach.pageID;
				myfree(SRAMIN,str);
			}
			else 
			LCD_ShowxNum(80,140,ensure,4,16,0);			
	  }
		else
		  LCD_ShowxNum(80,140,ensure,4,16,0);
	 PCF8574_WriteBit(BEEP_IO,1);//�رշ�����
	 delay_ms(600);
		
	}		
}

void scoreboard(void)
{
   int first,second,third,i,owner;
	 int first_ID,second_ID,third_ID;
	 int s1,t1;
	 int data_r[21];
	
	if(acv_flag==2&&k==2&&j==2)
	{
	 LCD_ShowString(350,120,100,10,16,"->->->->->");
	 for(i=0;i<21;i++)
	 {
	  data_r[i]=(int)(my_result[i+1]-'0');
 	 }	 
    owner    =data_r[0]*100 +data_r[1]*10 + data_r[2];
    first_ID =data_r[3]*100 +data_r[4]*10 + data_r[5];
	  first    =data_r[6]*100 +data_r[7]*10 + data_r[8];
	  second_ID=data_r[9]*100 +data_r[10]*10+ data_r[11];
	  second   =data_r[12]*100+data_r[13]*10+ data_r[14];
	  third_ID =data_r[15]*100+data_r[16]*10+ data_r[17];
	  third    =data_r[18]*100+data_r[19]*10+ data_r[20];
	 
	  s1=(int)((second*150)/first);
	  t1=(int)(( third*150)/first);
	  LCD_ShowString(470,120,75,10,24,"1st id:");    LCD_ShowNum(550,130, first_ID,5,16);	LCD_Fill(600,120, 600+150, 150,DARKBLUE);  LCD_ShowNum(760,130, first,5,16);		
	  LCD_ShowString(470,180,75,10,24,"2nd id:");    LCD_ShowNum(550,190,second_ID,5,16);	LCD_Fill(600,180, 600+s1, 210,LIGHTGREEN);LCD_ShowNum(760,190,second,5,16);		
	  LCD_ShowString(470,240,75,10,24,"3rd id:");    LCD_ShowNum(550,250, third_ID,5,16);	LCD_Fill(600,240, 600+t1, 270,LGRAY);     LCD_ShowNum(760,250, third,5,16);				  
	  POINT_COLOR=BRRED;
	 if(owner== first)       LCD_ShowString(480,300,280,10,24,"Best!you are first!");				
	 else if(owner==second)  LCD_ShowString(480,300,280,10,24,"Nice!you are second!");
	 else if(owner== third)  LCD_ShowString(480,300,280,10,24,"Good!you are third!");
	 else  
	 { 
		 POINT_COLOR=BLUE;
		 LCD_ShowString(510,300,100,10,16,"owner score:");
		 LCD_ShowNum(600,300,owner,5,16);
		 LCD_ShowString(650,300,200,10,16,"Please keep trying");
   }
 }
	else LCD_ShowString(350,120,100,10,16,"achieving");

}

