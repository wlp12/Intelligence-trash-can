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


//Ö¸ÎÆÄ£¿é²¿·Ö
#define usart2_baund  57600//´®¿Ú2²¨ÌØÂÊ£¬¸ù¾ÝÖ¸ÎÆÄ£¿é²¨ÌØÂÊ¸ü¸Ä

SysPara AS608Para;//Ö¸ÎÆÄ£¿éAS608²ÎÊý
u8** kbd_tbl;
u16 ValidN;//Ä£¿éÄÚÓÐÐ§Ö¸ÎÆ¸öÊý
u16 GET_NUM(void);//»ñÈ¡ÊýÖµ

void press_FR(void);//Ë¢Ö¸ÎÆ
void scoreboard(void);



//ÈÎÎñÓÅÏÈ¼¶
#define START_TASK_PRIO				3
//ÈÎÎñ¶ÑÕ»´óÐ¡	
#define START_STK_SIZE 				128
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB StartTaskTCB;
//ÈÎÎñ¶ÑÕ»	
CPU_STK START_TASK_STK[START_STK_SIZE];
//ÈÎÎñº¯Êý
void start_task(void *p_arg);

//TOUCHÈÎÎñ
//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define TOUCH_TASK_PRIO				4
//ÈÎÎñ¶ÑÕ»´óÐ¡
#define TOUCH_STK_SIZE				128
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB TouchTaskTCB;
//ÈÎÎñ¶ÑÕ»
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touchÈÎÎñ
void touch_task(void *p_arg);

//EMWINDEMOÈÎÎñ
//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define EMWINDEMO_TASK_PRIO			5
//ÈÎÎñ¶ÑÕ»´óÐ¡
#define EMWINDEMO_STK_SIZE			512
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB EmwindemoTaskTCB;
//ÈÎÎñ¶ÑÕ»
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_taskÈÎÎñ
void emwindemo_task(void *p_arg);

//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define Showtime_TASK_PRIO			6
//ÈÎÎñ¶ÑÕ»´óÐ¡
#define Showtime_STK_SIZE			1024
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB Showtime_TaskTCB;
//ÈÎÎñ¶ÑÕ»
CPU_STK Showtime_TASK_STK[Showtime_STK_SIZE];
//emwindemo_taskÈÎÎñ
void Showtime_task(void *p_arg);



//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define TIME_TASK_PRIO			6
//ÈÎÎñ¶ÑÕ»´óÐ¡
#define TIME_STK_SIZE			3000
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB TIMETaskTCB;
//ÈÎÎñ¶ÑÕ»
CPU_STK TIME_TASK_STK[TIME_STK_SIZE];
//emwindemo_taskÈÎÎñ
void TIME_task_1(void *p_arg);


//ÉèÖÃÈÎÎñÓÅÏÈ¼¶
#define Getdata_TASK_PRIO			6
//ÈÎÎñ¶ÑÕ»´óÐ¡
#define Getdata_STK_SIZE			256
//ÈÎÎñ¿ØÖÆ¿é
OS_TCB Getdata_TaskTCB;
//ÈÎÎñ¶ÑÕ»
CPU_STK Getdata_TASK_STK[Getdata_STK_SIZE];
//emwindemo_taskÈÎÎñ
void Getdata_task(void *p_arg);


void image_interface(int m, int n)
{
	int length=100,height=160,p,q;
  p=(m+1)*40;
	q=40*(n+1);
	LCD_DrawRectangle(49,49, 51+length, 51+height);
	LCD_Fill(51,51+height-p, 49+length, 49+height,lcd_discolor[m]);				//Ìî³äÖ¸¶¨ÑÕÉ«
	LCD_DrawRectangle(399,49,401+length,51+height);			
	LCD_Fill(401,51+height-q,399+length,49+height,lcd_discolor[n]);				//Ìî³äÖ¸¶¨ÑÕÉ«
}



int main(void)
{
    OS_ERR err;
	  CPU_SR_ALLOC(); 
    Stm32_Clock_Init(360,25,2,8);   //ÉèÖÃÊ±ÖÓ,180Mhz   
    HAL_Init();                     //³õÊ¼»¯HAL¿â
    delay_init(180);                //³õÊ¼»¯ÑÓÊ±º¯Êý
    uart_init(115200);              //³õÊ¼»¯USART
    LED_Init();                     //³õÊ¼»¯LED 
    KEY_Init();                     //³õÊ¼»¯°´¼ü
    PCF8574_Init();                 //³õÊ¼»¯PCF8574
    SDRAM_Init();                   //SDRAM³õÊ¼»¯
    TFTLCD_Init();  		        //LCD³õÊ¼»¯
	  TIM2_PWM_Init(500-1,90-1);
    TP_Init();				        //´¥ÃþÆÁ³õÊ¼»
	  usart3_init(115200);				//´®¿Ú3³õÊ¼»¯ 
    usart2_init(usart2_baund);//³õÊ¼»¯´®¿Ú2,ÓÃÓÚÓëÖ¸ÎÆÄ£¿éÍ¨Ñ¶
	  usmart_dev.init(168);		//³õÊ¼»¯USMART
	  PS_StaGPIO_Init();	//³õÊ¼»¯FR¶Á×´Ì¬Òý½Å
		W25QXX_Init();			//³õÊ¼»¯W25Q128
    my_mem_init(SRAMIN);		    //³õÊ¼»¯ÄÚ²¿ÄÚ´æ³Ø
	  my_mem_init(SRAMEX);		    //³õÊ¼»¯Íâ²¿ÄÚ´æ³Ø
	  my_mem_init(SRAMCCM);		    //³õÊ¼»¯CCMÄÚ´æ³Ø
	  exfuns_init();			//ÎªfatfsÏà¹Ø±äÁ¿ÉêÇëÄÚ´æ  
 	  f_mount(fs[1],"1:",1);  //¹ÒÔØFLASH.
    while(font_init()) 			//¼ì²é×Ö¿â
	  {	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//Çå³ýÏÔÊ¾	
		delay_ms(200);   
  	}	
    OSInit(&err);		            //³õÊ¼»¯UCOSIII
	  OS_CRITICAL_ENTER();            //½øÈëÁÙ½çÇø
	//´´½¨¿ªÊ¼ÈÎÎñ
	  OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//ÈÎÎñ¿ØÖÆ¿é
				 (CPU_CHAR	* )"start task", 		//ÈÎÎñÃû×Ö
                 (OS_TASK_PTR )start_task, 			//ÈÎÎñº¯Êý
                 (void		* )0,					//´«µÝ¸øÈÎÎñº¯ÊýµÄ²ÎÊý
                 (OS_PRIO	  )START_TASK_PRIO,     //ÈÎÎñÓÅÏÈ¼¶
                 (CPU_STK   * )&START_TASK_STK[0],	//ÈÎÎñ¶ÑÕ»»ùµØÖ·
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//ÈÎÎñ¶ÑÕ»Éî¶ÈÏÞÎ»
                 (CPU_STK_SIZE)START_STK_SIZE,		//ÈÎÎñ¶ÑÕ»´óÐ¡
                 (OS_MSG_QTY  )0,					//ÈÎÎñÄÚ²¿ÏûÏ¢¶ÓÁÐÄÜ¹»½ÓÊÕµÄ×î´óÏûÏ¢ÊýÄ¿,Îª0Ê±½ûÖ¹½ÓÊÕÏûÏ¢
                 (OS_TICK	  )0,					//µ±Ê¹ÄÜÊ±¼äÆ¬ÂÖ×ªÊ±µÄÊ±¼äÆ¬³¤¶È£¬Îª0Ê±ÎªÄ¬ÈÏ³¤¶È£¬
                 (void   	* )0,					//ÓÃ»§²¹³äµÄ´æ´¢Çø
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //ÈÎÎñÑ¡Ïî
                 (OS_ERR 	* )&err);				//´æ·Å¸Ãº¯Êý´íÎóÊ±µÄ·µ»ØÖµ
	OS_CRITICAL_EXIT();	            //ÍË³öÁÙ½çÇø	 
	OSStart(&err);                  //¿ªÆôUCOSIII
	while(1);
}

//¿ªÊ¼ÈÎÎñº¯Êý
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//Í³¼ÆÈÎÎñ                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//Èç¹ûÊ¹ÄÜÁË²âÁ¿ÖÐ¶Ï¹Ø±ÕÊ±¼ä
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //µ±Ê¹ÓÃÊ±¼äÆ¬ÂÖ×ªµÄÊ±ºò
	//Ê¹ÄÜÊ±¼äÆ¬ÂÖ×ªµ÷¶È¹¦ÄÜ,ÉèÖÃÄ¬ÈÏµÄÊ±¼äÆ¬³¤¶È
      OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);    //Ê¹ÄÜÊ±¼äÆ¬ÂÖ×ªµ÷¶È¹¦ÄÜ,Ê±¼äÆ¬³¤¶ÈÎª1¸öÏµÍ³Ê±ÖÓ½ÚÅÄ£¬¼È1*5=5ms 
      OSTaskTimeQuantaSet(&Task1_TaskTCB,1,&err);
//      OSTaskTimeQuantaSet(&TIMETaskTCB,1,&err);	
      OSTaskTimeQuantaSet(&Getdata_TaskTCB,,&err);
      OSTaskTimeQuantaSet(&Showtime_TaskTCB,1,&err);
#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//Ê¹ÄÜCRCÊ±ÖÓ
    WM_SetCreateFlags(WM_CF_MEMDEV); //Æô¶¯ËùÓÐ´°¿ÚµÄ´æ´¢Éè±¸
	GUI_Init();  			//STemWin³õÊ¼»¯
	WM_MULTIBUF_Enable(1);  //¿ªÆôSTemWin¶à»º³å,RGBÆÁ¿ÉÄÜ»áÓÃµ½
	OS_CRITICAL_ENTER();	//½øÈëÁÙ½çÇø
	//STemWin DemoÈÎÎñ	
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
	//´¥ÃþÆÁÈÎÎñ
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
									 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//¹ÒÆð¿ªÊ¼ÈÎÎñ			 
	OS_CRITICAL_EXIT();	//ÍË³öÁÙ½çÇø
}

//EMWINDEMOÈÎÎñ
void emwindemo_task(void *p_arg)
{

	GUI_CURSOR_Show(); 	
	//¸ü»»Æ¤·ô
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

//TOUCHÈÎÎñ
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±5ms
	}
}

//½çÃæÏÔÊ¾Êý¾ÝÈÎÎñ
void Showtime_task(void *p_arg)
{	
	OS_ERR err;	
	u8 ensure;
	int i;
	char *str;
	while(1)
	{	 
	if(interface==1)//½çÃæ1
		 {
			 exhibition=0; exhibition_1=0; acv_flag=0; ID=0;ID1=0;Look=0;esp_reset=0;esp_back=1;;	//Çå³ý±êÖ¾Î»           
	   }		
	if(interface==2)  //½çÃæ2
		 {	
			exhibition=0; exhibition_1=0; acv_flag=0; ID=0;ID1=0;Look=0;esp_reset=0;esp_back=1;;	//Çå³ý±êÖ¾Î»  
			 for(i=0;i<11;i++)		{ LCD_ShowxNum(150+60*i,75,input_number[i],1,24,0);	}	
			 if(YES==1)  
			  {				
		     POINT_COLOR=RED;
				 LCD_ShowString(300,100,300,10,24,"No this number");
				 POINT_COLOR=BLACK;
			  }		
		}	
  if(interface==3)//½çÃæ3
		{  	
      ID=0;ID1=0;exhibition=0;exhibition_1=0;acv_flag=0;esp_reset=0;Look=0;esp_back=1;	//Çå³ý±êÖ¾Î»		 			  						 			
	  }	
	if(interface==4)//½çÃæ4
    {	
			LCD_ShowxNum(300,300,wifi,4,16,0);
	    POINT_COLOR=BLUE; 
			LCD_ShowString(20,140,300,10,16,"error:");
      while(PS_HandShake(&AS608Addr))//ÓëAS608Ä£¿éÎÕÊÖ
      {
       Show_Str_Mid(20,50,"ÓëÖ¸ÎÆÄ£¿éÎÕÊÖ..",16,240);		  
      }
      Show_Str_Mid(20,80,"Í¨Ñ¶³É¹¦!!!",16,240);
      str=mymalloc(SRAMIN,30);
      sprintf(str,"²¨ÌØÂÊ:%d   µØÖ·:%x",usart2_baund,AS608Addr);
      Show_Str(20,110,240,16,(u8*)str,16,0);	
	    ensure=PS_ValidTempleteNum(&ValidN);//¶Á¿âÖ¸ÎÆ¸öÊý
	    if(ensure!=0x00)     
			LCD_ShowxNum(80,140,ensure,4,16,0);		
	    ensure=PS_ReadSysPara(&AS608Para);  //¶Á²ÎÊý 
	    if(ensure==0x00)
	    {			 
		   mymemset(str,0,50);
		   sprintf(str,"¿âÈÝÁ¿:%d    ¶Ô±ÈµÈ¼¶: %d",AS608Para.PS_max-ValidN,AS608Para.PS_level);
		   Show_Str(20,170,240,16,(u8*)str,24,0);
	     }
	    else
		   LCD_ShowxNum(80,140,ensure,4,16,0);					
	     myfree(SRAMIN,str);    				
		   press_FR();//Ë¢Ö¸ÎÆ
			if(interface==4)
			{ 
			 LCD_ShowString(60,280,300,10,24,"Owner ID:");
       LCD_ShowxNum(180,280,ID1,4,24,0);
			}				
			if(exhibition_1==1)
			{
			
				printf("%s\r\n",my_result);	//·¢ËÍÃüÁî
			 LCD_ShowString(250,430,400,12,12,my_result);
			 scoreboard();
			}	  
     }
			 OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±	
	   }			
	 }
	 
void Getdata_task(void  *p_arg) 
{
	OS_ERR  err;
	while(1)
	 {	 		
//		 if(esp_back==1)
//			{
//			  atk_8266_quit_trans();	//ÍË³öÍ¸´«
//	      atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //¹Ø±ÕÍ¸´«Ä£Ê½
//				esp_back=0;
//			}
	  if(interface==4)//½çÃæ4
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
			   atk_8266_send_cmd("AT+CIPSEND","OK",200);  //·¢ËÍÖ¸¶¨³¤¶ÈµÄÊý¾Ý£¬¿ªÊ¼Í¸´«	
      	 delay_ms(200);
	       wifi=7;				 
				 Look=0;
			  }
			if(exhibition==1)
			  {		
				 u3_printf("ok=%d\r\n",ID1);	//·¢ËÍÃüÁî
				 exhibition=0;
			  }
	   }	   
    OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//ÑÓÊ±5ms	
   }
}


//Ë¢Ö¸ÎÆ
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//»ñÈ¡Í¼Ïñ³É¹¦ 
	{	
		PCF8574_WriteBit(BEEP_IO,0);//´ò¿ª·äÃùÆ÷	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //Éú³ÉÌØÕ÷³É¹¦
		{		
			PCF8574_WriteBit(BEEP_IO,1);//¹Ø±Õ·äÃùÆ÷	
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//ËÑË÷³É¹¦
			{				
				Show_Str_Mid(0,250,"Ë¢Ö¸ÎÆ³É¹¦",24,240);				
				str=mymalloc(SRAMIN,50);
				ID1=seach.pageID;
				myfree(SRAMIN,str);
			}
			else 
			LCD_ShowxNum(80,140,ensure,4,16,0);			
	  }
		else
		  LCD_ShowxNum(80,140,ensure,4,16,0);
	 PCF8574_WriteBit(BEEP_IO,1);//¹Ø±Õ·äÃùÆ÷
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

