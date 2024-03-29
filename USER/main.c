#include "sys.h"
#include "delay.h"
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
#include "sdio_sdcard.h"
#include "wifi.h"
#include "stdlib.h"
#include "usmart.h"
#include "usart.h"
#include "usart2.h"
#include "usart3.h"
#include "gifdisplay.h"
#include "databox.h"

int key_number,transform;
int ID,ID1,show_flag;
int weight[18],WEIGHT[2];
int lcd_discolor[4]={	BLACK,GRAY,BLUE,RED};
u16 get(int t);
u16 wave_scan(void);
extern int YES,num,flag,value_1,exhibition,exhibition_1,acv_flag,esp_back,wifi,Look,esp_reset;
extern u8  show,interface,res1,my_result[23],R_flag,N_flag;
extern u32 input_number[11];
extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN];
extern DATABOX databox1,databox2;
extern DATABOX *sptr1;
void image_interface(int m, int n);
void scoreboard(void);

//指纹模块部分
#define usart2_baund  57600//串口2波特率，根据指纹模块波特率更改

SysPara AS608Para;//指纹模块AS608参数
u8** kbd_tbl;
u16 ValidN;//模块内有效指纹个数
u16 GET_NUM(void);//获取数值

void press_FR(void);//刷指纹
//void scoreboard(void);



//任务优先级
#define START_TASK_PRIO				3
//任务堆栈大小	
#define START_STK_SIZE 				128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				4
//任务堆栈大小
#define TOUCH_STK_SIZE				128
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//按键任务
//设置任务优先级
#define KEY_TASK_PRIO 				5
//任务堆栈大小
#define KEY_STK_SIZE				256
//任务控制块
OS_TCB KeyTaskTCB;
//任务堆栈
CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
//led0任务
void key_task(void *p_arg);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO			6
//任务堆栈大小
#define EMWINDEMO_STK_SIZE			1024
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task任务
void emwindemo_task(void *p_arg);

//设置任务优先级
#define Showtime_TASK_PRIO			7
//任务堆栈大小
#define Showtime_STK_SIZE			1024
//任务控制块
OS_TCB Showtime_TaskTCB;
//任务堆栈
CPU_STK Showtime_TASK_STK[Showtime_STK_SIZE];
//emwindemo_task任务
void Showtime_task(void *p_arg);



//设置任务优先级
#define TIME_TASK_PRIO			7
//任务堆栈大小
#define TIME_STK_SIZE			3000
//任务控制块
OS_TCB TIMETaskTCB;
//任务堆栈
CPU_STK TIME_TASK_STK[TIME_STK_SIZE];
//emwindemo_task任务
void TIME_task_1(void *p_arg);


//设置任务优先级
#define Getdata_TASK_PRIO			7
//任务堆栈大小
#define Getdata_STK_SIZE			256
//任务控制块
OS_TCB Getdata_TaskTCB;
//任务堆栈
CPU_STK Getdata_TASK_STK[Getdata_STK_SIZE];
//emwindemo_task任务
void Getdata_task(void *p_arg);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK1_PRIO			8
//任务堆栈大小
#define EMWINDEMO_STK1_SIZE			1024
//任务控制块
OS_TCB EmwindemoTask1TCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK1_STK[EMWINDEMO_STK1_SIZE];
//emwindemo_task任务
void emwindemo_task1(void *p_arg1);


void image_interface(int m, int n)
{
	int length=100,height=160,p,q;
  p=(m+1)*40;
	q=40*(n+1);
	LCD_DrawRectangle(49,49, 51+length, 51+height);
	LCD_Fill(51,51+height-p, 49+length, 49+height,lcd_discolor[m]);				//填充指定颜色
	LCD_DrawRectangle(399,49,401+length,51+height);			
	LCD_Fill(401,51+height-q,399+length,49+height,lcd_discolor[n]);				//填充指定颜色
}



int main(void)
{
    OS_ERR err;
	  CPU_SR_ALLOC(); 
    Stm32_Clock_Init(360,25,2,8);   //设置时钟,180Mhz   
    HAL_Init();                     //初始化HAL库
    delay_init(180);                //初始化延时函数
    uart_init(115200);              //初始化USART
    LED_Init();                     //初始化LED 
    KEY_Init();                     //初始化按键
    PCF8574_Init();                 //初始化PCF8574
    SDRAM_Init();                   //SDRAM初始化
    TFTLCD_Init();  		            //LCD初始化
	  TIM2_PWM_Init(500-1,90-1);
    TP_Init();				         //触摸屏初始�
	  usart3_init(115200);			 //串口3初始化 
    usart2_init(usart2_baund); //初始化串口2,用于与指纹模块通讯
	  usmart_dev.init(168);		   //初始化USMART
	  PS_StaGPIO_Init();	//初始化FR读状态引脚
		W25QXX_Init();			//初始化W25Q128
    my_mem_init(SRAMIN);		//初始化内部内存池
	  my_mem_init(SRAMEX);		//初始化外部内存池
	  my_mem_init(SRAMCCM);		//初始化CCM内存池
	  exfuns_init();			    //为fatfs相关变量申请内存  
 	  f_mount(fs[1],"1:",1);  //挂载FLASH.
		f_mount(fs[0],"0:",1); 	//挂载SD卡 
		SD_Init();              //SD卡初始化
    while(font_init()) 			//检查字库
	  {	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//清除显示	
		delay_ms(200);   
  	}	
    OSInit(&err);		            //初始化UCOSIII
	  OS_CRITICAL_ENTER();            //进入临界区
	//创建开始任务
	  OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	            //退出临界区	 
	OSStart(&err);                  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候	//使能时间片轮转调度功能,设置默认的时间片长度
	
      OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);    //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms 
    
//	    OSTaskTimeQuantaSet(&EmwindemoTask1TCB,1,&err);
//	    OSTaskTimeQuantaSet(&EmwindemoTaskTCB,1,&err);
	
      OSTaskTimeQuantaSet(&Getdata_TaskTCB,,&err);
      OSTaskTimeQuantaSet(&Showtime_TaskTCB,1,&err);
#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//使能CRC时钟
    WM_SetCreateFlags(WM_CF_MEMDEV); //启动所有窗口的存储设备
	GUI_Init();  			//STemWin初始化
	WM_MULTIBUF_Enable(1);  //开启STemWin多缓冲,RGB屏可能会用到
	OS_CRITICAL_ENTER();	//进入临界区
		//STemWin Demo任务1	
	OSTaskCreate((OS_TCB*     )&EmwindemoTask1TCB,		
				 (CPU_CHAR*   )"Emwindemo task1", 		
                 (OS_TASK_PTR )emwindemo_task1, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK1_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK1_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK1_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK1_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);

	//触摸屏任务
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
	//按键任务
	OSTaskCreate((OS_TCB*     )&KeyTaskTCB,		
				 (CPU_CHAR*   )"Key task", 		
                 (OS_TASK_PTR )key_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )KEY_TASK_PRIO,     
                 (CPU_STK*    )&KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);                 
		//STemWin Demo任务	
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
								 								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务	 						 
  delay_ms(100);										 
	OS_CRITICAL_EXIT();	//退出临界区	
	OSTaskSuspend((OS_TCB*)&EmwindemoTask1TCB,&err); //挂起任务									 
}

//按键处理任务
void key_task(void *pdata)
{
	u8 key;
	OS_ERR err;
	
	while(1)
	{
		key = KEY_Scan(0);
		switch(key)
		{
			case KEY0_PRES:					
        key_number++;
//        if(key_number%2==1)	
//				{				
////         OSTaskSuspend((OS_TCB*)&EmwindemoTask1TCB,&err);  //挂起任务
//			   OSTaskResume((OS_TCB*)&EmwindemoTask1TCB,&err);	   //恢复任务
//				}
//				else
//				{			
//			   OSTaskSuspend((OS_TCB*)&EmwindemoTaskTCB,&err);   //挂起任务
////			  OSTaskResume((OS_TCB*) &EmwindemoTask1TCB,&err);	//恢复任务
//				}	
//				GUI_Clear();
				break;
		}
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);//延时10ms
	}
}

//EMWINDEMO任务
void emwindemo_task(void *p_arg)
{
	GUI_CURSOR_Show(); 	
	//更换皮肤
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
	GUI_Clear();
	while(1)
	{		 
	 my_gui_create();
	}
}
//EMWINDEMO任务
void emwindemo_task1(void *p_arg1)
{
//	GUI_CURSOR_Show();
	GUI_Clear();
	while(1)
	{	
    gifdisplay_demo();
	}
}
//TOUCH任务
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}

//界面显示数据任务
void Showtime_task(void *p_arg)
{	
	OS_ERR err;	
	u8 ensure;
	int i;
	char *str;
	while(1)
	{	 
	if(interface==1)//界面1
		 {
			 exhibition=0; exhibition_1=0; acv_flag=0; ID=0;ID1=0;Look=0;esp_reset=0;esp_back=1;;	//清除标志位           
	   }		
	if(interface==2)  //界面2
		 {	
			exhibition=0; exhibition_1=0; acv_flag=0; ID=0;ID1=0;Look=0;esp_reset=0;esp_back=1;;	//清除标志位  
			 for(i=0;i<11;i++)		{ LCD_ShowxNum(150+60*i,75,input_number[i],1,24,0);	}	
			 if(YES==1)  
			  {				
		     POINT_COLOR=RED;
				 LCD_ShowString(300,100,300,10,24,"No this number");
				 POINT_COLOR=BLACK;
			  }		
		}	
  if(interface==3)//界面3
		{  	
      ID=0;ID1=0;exhibition=0;exhibition_1=0;acv_flag=0;esp_reset=0;Look=0;esp_back=1;	//清除标志位		 			  						 			
	  }	
	if(interface==4)//界面4
    {	
			LCD_ShowxNum(900,60,wifi,4,16,0);
	    POINT_COLOR=BLUE; 
			LCD_ShowString(20,200,300,10,16,"error:");
      PS_HandShake(&AS608Addr);//与AS608模块握手
      Show_Str_Mid(20,110,"与指纹模块握手..",16,240);		   
      Show_Str_Mid(20,140,"通讯成功!!!",16,240);
      str=mymalloc(SRAMIN,30);
      sprintf(str,"波特率:%d   地址:%x",usart2_baund,AS608Addr);
      Show_Str(20,170,240,16,(u8*)str,16,0);	
	    ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
	    if(ensure!=0x00)     
			LCD_ShowxNum(80,200,ensure,4,16,0);		
	    ensure=PS_ReadSysPara(&AS608Para);  //读参数 
	    if(ensure==0x00)
	    {			 
		   mymemset(str,0,50);
		   sprintf(str,"库容量:%d    对比等级: %d",AS608Para.PS_max-ValidN,AS608Para.PS_level);
		   Show_Str(20,230,240,16,(u8*)str,24,0);
	     }
	    else
		   LCD_ShowxNum(80,230,ensure,4,16,0);					
	     myfree(SRAMIN,str);    				
		   press_FR();//刷指纹
			if(interface==4)
			{ 
			 LCD_ShowString(60,340,300,10,24,"Owner ID:");
       LCD_ShowxNum(180,340,ID1,4,24,0);
			}				
			if(exhibition_1==1)
			{			
//			 LCD_ShowString(250,490,400,12,12,my_result);
			 scoreboard();
			}	  
     }
			 OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时	
	   }			
	 }
	 
void Getdata_task(void  *p_arg) 
{
	OS_ERR  err;
	while(1)
	 {	 		
	  if(interface==4)//界面4
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
			   atk_8266_send_cmd("AT+CIPSEND","OK",200);  //发送指定长度的数据，开始透传	
      	 delay_ms(200);
	       wifi=7;				 
				 Look=0;
			  }
			if(exhibition==1)
			  {		
				 u3_printf("ok=%d\r\n",ID1);	//发送命令
				 exhibition=0;
			  }
	   }	   
    OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms	
   }
}


//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		PCF8574_WriteBit(BEEP_IO,0);//打开蜂鸣器	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			PCF8574_WriteBit(BEEP_IO,1);//关闭蜂鸣器	
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//搜索成功
			{				
				Show_Str_Mid(0,310,"刷指纹成功",24,240);				
				str=mymalloc(SRAMIN,50);
				ID1=seach.pageID;
				myfree(SRAMIN,str);
			}
			else 
			LCD_ShowxNum(80,200,ensure,4,16,0);			
	  }
		else
		  LCD_ShowxNum(80,200,ensure,4,16,0);
	 PCF8574_WriteBit(BEEP_IO,1);//关闭蜂鸣器
	 delay_ms(600);
		
	}		
}

void scoreboard(void)
{	
   int first,second,third,i,owner;
	 int first_ID,second_ID,third_ID;
	 int s1,t1,data_r[21];
	 if(acv_flag==2&&(sptr1->funtion_part)==command_1&&(sptr1->address)==Address_1)
	 {
	  LCD_ShowString(350,120,100,10,16,"->->->->->->->");
	   for(i=0;i<21;i++)
	   {
	    data_r[i]=(int)(my_result[i]-'0');		
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
	   LCD_ShowString(500,120,75,10,24,"1st id:");    LCD_ShowNum(580,130, first_ID,5,16);	LCD_Fill(630,120, 600+150, 150,DARKBLUE);  LCD_ShowNum(790,130, first,5,16);		
	   LCD_ShowString(500,180,75,10,24,"2nd id:");    LCD_ShowNum(580,190,second_ID,5,16);	LCD_Fill(630,180, 600+s1, 210,LIGHTGREEN);LCD_ShowNum(790,190,second,5,16);		
	   LCD_ShowString(500,240,75,10,24,"3rd id:");    LCD_ShowNum(580,250, third_ID,5,16);	LCD_Fill(630,240, 600+t1, 270,LGRAY);     LCD_ShowNum(790,250, third,5,16);				  
	   POINT_COLOR=BRRED;
	  if(owner== first)       LCD_ShowString(510,300,280,10,24,"Best!you are first!");				
	  else if(owner==second)  LCD_ShowString(510,300,280,10,24,"Nice!you are second!");
	  else if(owner== third)  LCD_ShowString(510,300,280,10,24,"Good!you are third!");
	  else  
	  { 
		  POINT_COLOR=BLUE;
		  LCD_ShowString(540,300,100,10,16,"owner score:");
		  LCD_ShowNum(630,300,owner,5,16);
		  LCD_ShowString(680,300,200,10,16,"Please keep trying");
    }
  }
	 else LCD_ShowString(380,120,100,10,16,"achieving");
}


