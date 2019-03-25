#include "GUI.h"
#include "DIALOG.h"
#include "BUTTON.h"
#include "My_gui.h"
#include "stmflash.h"
#include "usart.h"
#include "CHECKBOX.h"
#include "delay.h"
#include "includes.h"
#include "ILI93xx.h"
#include "as608.h"
#include "usart3.h" 
#include "wifi.h"

#define len_passward         sizeof(Show_password)	
#define size_password        len_passward/4+((len_passward%4)?1:0)
#define len_standard_pack    sizeof(standard_pack)	
#define size_standard_pack   len_standard_pack/4+((len_standard_pack%4)?1:0)
#define len_Fraction         sizeof(Fraction)	
#define size_Fraction        len_Fraction/4+((len_Fraction%4)?1:0)

u32 Show_password[11]={  0,0,0,0,0,0,0,0,0,0,0 };
u32 input_number[11]= {  0,0,0,0,0,0,0,0,0,0,0 };
int ID2,flag,num,YES,interface=1,f=0;
int Fraction[9]={0,0,0,0,0,0,0,0,0};
int exhibition,exhibition_1,acv_flag,value_1,Look,esp_back=0,esp_reset;
extern int ID1,ID;
static int standard_pack[8][11]={
 {1,3,7,5,0,8,9,5,6,4,4},
 {1,2,1,1,1,1,1,1,1,1,1},
 {1,1,2,1,1,1,1,1,1,1,1},
 {1,1,1,2,1,1,1,1,1,1,1}, 
 {1,1,1,1,2,1,1,1,1,1,1},
 {1,1,1,1,1,2,1,1,1,1,1},
 {1,1,1,1,1,1,2,1,1,1,1},
 {1,1,1,1,1,1,1,2,1,1,1}, 
 };

void checkfunction(void);
void check_and_go(void);
void check_ID(void);


WM_HWIN CreateFramewin_1(void);
WM_HWIN CreateFramewin_2(void);
WM_HWIN CreateFramewin_3(void);
WM_HWIN CreateFramewin_4(void); 
WM_HWIN DialoghWin;             //对话框窗口
radio_struct radio_widg;
 
 
/***************界面一*******************/
#define ID_BUTTON_13     (GUI_ID_USER + 0x43)
#define ID_BUTTON_14     (GUI_ID_USER + 0x49)
#define ID_DROPDOWN_0    (GUI_ID_USER + 0x02)

 
/***************界面二********************/
#define ID_BUTTON_0      (GUI_ID_USER + 0x09)
#define ID_BUTTON_1      (GUI_ID_USER + 0x0B)
#define ID_BUTTON_2      (GUI_ID_USER + 0x0C)
#define ID_BUTTON_3      (GUI_ID_USER + 0x0D)
#define ID_BUTTON_4      (GUI_ID_USER + 0x0E)
#define ID_BUTTON_5      (GUI_ID_USER + 0x0F)
#define ID_BUTTON_6      (GUI_ID_USER + 0x10)
#define ID_BUTTON_7      (GUI_ID_USER + 0x11)
#define ID_BUTTON_8      (GUI_ID_USER + 0x12)
#define ID_BUTTON_9   	 (GUI_ID_USER + 0x13)
#define ID_BUTTON_10     (GUI_ID_USER + 0x14)
#define ID_BUTTON_11     (GUI_ID_USER + 0x15)
#define ID_BUTTON_12     (GUI_ID_USER + 0x16)

/***************界面三*******************/
#define ID_BUTTON_15     (GUI_ID_USER + 0x24)
#define ID_BUTTON_16     (GUI_ID_USER + 0x25)
#define ID_BUTTON_17     (GUI_ID_USER + 0x26)
#define ID_BUTTON_18     (GUI_ID_USER + 0x27)
#define ID_BUTTON_19     (GUI_ID_USER + 0x28)
#define ID_BUTTON_20     (GUI_ID_USER + 0x29)

/***********共用**************/
#define ID_FRAMEWIN_0    (GUI_ID_USER + 0x07)
#define ID_TEXT_0        (GUI_ID_USER + 0x1B)
#define ID_TEXT_1        (GUI_ID_USER + 0x1C)
#define ID_TEXT_2 			 (GUI_ID_USER + 0x1D)
#define ID_TEXT_3        (GUI_ID_USER + 0x1E)
#define ID_TEXT_4        (GUI_ID_USER + 0x44)
#define ID_TEXT_5        (GUI_ID_USER + 0x45)
#define ID_TEXT_6        (GUI_ID_USER + 0x46)

/***************界面四*******************/
#define ID_BUTTON_21     (GUI_ID_USER + 0x30)
#define ID_BUTTON_22     (GUI_ID_USER + 0x31)
#define ID_BUTTON_23     (GUI_ID_USER + 0x32)
#define ID_BUTTON_24     (GUI_ID_USER + 0x33)
#define ID_BUTTON_25     (GUI_ID_USER + 0x34)
#define ID_TEXT_7        (GUI_ID_USER + 0x47)

//智能垃圾桶的初始页面
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_1[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0,  0,    0, 1050, 640, 0, 0x64,  0 }, //MY_HOME方框
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_13,  550,  30,  220, 130, 0,  0x0, 0 },//按键
  { BUTTON_CreateIndirect,   "Button",   ID_BUTTON_14,  550, 200,  220, 130, 0,  0x0, 0 },//按键
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_6,     900, 400,  200,  30, 0, 0x64, 0 },	
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 550, 330,  220, 150, 0, 0x0 , 0 },
//  { BUTTON_CreateIndirect,   "Ok",       ID_BUTTON_00,  100, 170,  100,	 40},
};


//输入身份信息的对话界面
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_2[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0,   0,1050,640, 0,0x64, 0 },//MY_HOME方框
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0,     140, 100, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1,     350, 100, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_2,     560, 100, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_3,     140, 200, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_4,     350, 200, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_5,     560, 200, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_6,     140, 300, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_7,     350, 300, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_8,     560, 300, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_9,     140, 400, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_10,    350, 400, 200, 70, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_11,    560, 400, 200, 70, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Button", ID_BUTTON_12,    800, 300, 100,170, 0, 0x0, 0 },

};
//实时显示智能垃圾桶内部参数的界面
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_3[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0,    0,   0,1050,640, 0, 0x64, 0 }, //MY_HOME方框
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_0,      170,   5, 200, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_1,      170,  55, 200, 30, 0, 0x64, 0 },//文本
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_2,      170, 105, 200, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_3,      520,   5, 200, 30, 0, 0x64, 0 },
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_4,      520,  55, 200, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect,     "Text",     ID_TEXT_5,      520, 105, 200, 30, 0, 0x64, 0 },
  { BUTTON_CreateIndirect,   "Button", ID_BUTTON_15,     50,  200, 120, 80, 0,  0x0, 0 },
  { BUTTON_CreateIndirect,   "Button", ID_BUTTON_16,     50,  300, 120, 80, 0,  0x0, 0 },
  { BUTTON_CreateIndirect,   "Button", ID_BUTTON_17,     400, 200, 120, 80, 0,  0x0, 0 },
  { BUTTON_CreateIndirect,   "Button", ID_BUTTON_18,     400, 300, 120, 80, 0,  0x0, 0 },
  { BUTTON_CreateIndirect,   "Button", ID_BUTTON_19,     630, 320, 130, 90, 0,  0x0, 0 },
	{ BUTTON_CreateIndirect,   "Button", ID_BUTTON_20,     630, 160, 140,100, 0,  0x0, 0 },
}; 

static const GUI_WIDGET_CREATE_INFO _aDialogCreate_4[] = {
  { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0,  0,    0, 1050, 640, 0, 0x64,  0 }, //MY_HOME方框
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_21,  80,  360, 160, 80, 0,  0x0, 0 },//按键
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_22, 330,  360, 160, 80, 0,  0x0, 0 },//按键
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_23, 830,  360, 160, 80, 0,  0x0, 0 },//按键
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_24, 580,  360, 160, 80, 0,  0x0, 0 },//按键  
	{ BUTTON_CreateIndirect,   "Button",   ID_BUTTON_25, 880,    0, 120, 60, 0,  0x0, 0 },//按键  
	{ TEXT_CreateIndirect,     "Text",     ID_TEXT_7,    880,  100, 100, 30, 0, 0x64, 0 },
};

//对话框回调函数
static void _cbDialog_1(WM_MESSAGE * pMsg);//界面1
static void _cbDialog_2(WM_MESSAGE * pMsg);//界面2
static void _cbDialog_3(WM_MESSAGE * pMsg);//界面3
static void _cbDialog_4(WM_MESSAGE * pMsg);//界面4


//界面1
static void _cbDialog_1(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  
  switch (pMsg->MsgId) {                   //区分各种消息类型
  case WM_INIT_DIALOG:                     //外观初始化| GUI_TA_VERTICAL
   
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 100);
    FRAMEWIN_SetText(hItem, "Welcome to use smart trash cans");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_VCENTER |GUI_TA_CENTER);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32B_1);
    FRAMEWIN_SetTextColor(hItem, 0x00000008);
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_14);
    BUTTON_SetText(hItem, "Household");
    BUTTON_SetFont(hItem, GUI_FONT_24B_1);
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_13);
    BUTTON_SetText(hItem, "Passerby");
    BUTTON_SetFont(hItem, GUI_FONT_24B_1);

	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "C1-135");
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
		DROPDOWN_SetFont(hItem, &GUI_Font32_ASCII);
		DROPDOWN_SetAutoScroll(hItem,1);	//启用自动使用滚动条
		DROPDOWN_AddString(hItem, "Choose a way");
		DROPDOWN_AddString(hItem, "Ensure?");
		DROPDOWN_AddString(hItem, "Phone?");
		DROPDOWN_AddString(hItem, "Face?");
		
    case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
	
    switch(Id) {
    case ID_BUTTON_13: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   GUI_EndDialog(pMsg->hWin, 0);	
           interface=3;
			     delay_ms(50);
           CreateFramewin_3();			     
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		}
      break;
		case ID_BUTTON_14: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				      f=1;		     
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		}
      break;
    case ID_DROPDOWN_0: // Notifications sent by 'Button'
     switch(NCode) 
					{
						case WM_NOTIFICATION_CLICKED:
							break;
						case WM_NOTIFICATION_RELEASED:
							break;
						case WM_NOTIFICATION_SEL_CHANGED:
                 hItem=WM_GetDialogItem(pMsg->hWin,ID_DROPDOWN_0);
                 value_1=DROPDOWN_GetSel(hItem);
                 if(value_1==2&&f==1)
									{
										f=0;
									  GUI_EndDialog(pMsg->hWin, 0);	
                    interface=2;
			              delay_ms(50);
                    CreateFramewin_2();			     
									}
								else if(value_1==1&&f==1)
								{
									f=0;
									GUI_EndDialog(pMsg->hWin, 0);	
                  interface=4;
			            delay_ms(50);
                  CreateFramewin_4();			   
								}	
							break;
					}
		 break;	  
     default:
     WM_DefaultProc(pMsg);
     break;
  }
}
}

//界面2
static void _cbDialog_2(WM_MESSAGE * pMsg) 
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int     number,u;
	
  switch (pMsg->MsgId) {                   //区分各种消息类型
  case WM_INIT_DIALOG:                     //外观初始化
   
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 50);
    FRAMEWIN_SetText(hItem, "Please enter your phone number");
    FRAMEWIN_SetTextAlign(hItem,  GUI_TA_VCENTER | GUI_TA_CENTER);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);
    FRAMEWIN_SetTextColor(hItem, 0x00000000);
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "1");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetText(hItem, "2");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetText(hItem, "3");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
  
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "4");
  
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetText(hItem, "5");
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_5);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "6");
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_6);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "7");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_7);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "8");
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_8);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
    BUTTON_SetText(hItem, "9");
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_9);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "Delete");
 
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_10);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "0");
 
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_11);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "ENTER");
  
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_12);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "Back");
		

  
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   number=1;
				   flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						input_number[flag-1]=number;
				   }			     
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		}
      break;
    case ID_BUTTON_1: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
					 number=2;
				   flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						input_number[flag-1]=number; 
				   }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
    case ID_BUTTON_2: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
           number=3;
				   flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						 input_number[flag-1]=number;
				   }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
    case ID_BUTTON_3: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
					 number=4;
				   flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						input_number[flag-1]=number; 
				   }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
    case ID_BUTTON_4: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
					  number=5;
				     flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						 input_number[flag-1]=number;
				   }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
   case ID_BUTTON_5: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   number=6;
					 flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						 input_number[flag-1]=number;
				   }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
   case ID_BUTTON_6: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
					 number=7;
					 flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						input_number[flag-1]=number;
				   }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
   case ID_BUTTON_7: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   number=8;
					 flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						input_number[flag-1]=number;
				   }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
   case ID_BUTTON_8: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
					 number=9;
					 flag++;
			     if(flag>=12) flag=0;
			      else if(flag>0)  
					 {
						input_number[flag-1]=number;
				   }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
    case ID_BUTTON_9: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:	
      if(flag>=12||flag<0) flag=0;
			flag--;
			input_number[flag]=0;
					
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
    case ID_BUTTON_10: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
					 number=0;
					 flag++;
			     if(flag>=12) flag=0;
			     else if(flag>0)  
					 {
						input_number[flag-1]=number;
					 }
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
    case ID_BUTTON_11: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
            check_and_go();
//					 checkfunction();
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
		case ID_BUTTON_12: // Notifications sent by 'Button'  u3_printf("ok=%d\r\n",ID1);	//发送命令
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   GUI_EndDialog(pMsg->hWin, 0);	
           interface=1;
           delay_ms(50);	
           for(u=0;u<11;u++)
				   {input_number[u]=0;}			
           CreateFramewin_1();	     
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
      }
      break;
	}
	break;
	
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}
int     mq,mp;
//界面3
static void _cbDialog_3(WM_MESSAGE * pMsg)
{
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:    //初始化消息,创建窗口/控件时有效,比如在这里设置一些控件的初始参数
    
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 45);
    FRAMEWIN_SetText(hItem, "Wlcome coming");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_VCENTER | GUI_TA_CENTER	);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32_1);   //设置字体
    FRAMEWIN_SetTextColor(hItem, 0x00000000);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_15);
    BUTTON_SetFont(hItem, GUI_FONT_16_1);
    BUTTON_SetText(hItem, "Dry  OutFire?");
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_16);
    BUTTON_SetFont(hItem, GUI_FONT_16_1);
    BUTTON_SetText(hItem, "Dry FAN?");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_17);
    BUTTON_SetFont(hItem, GUI_FONT_16_1);
    BUTTON_SetText(hItem, "Damp  OutFire?");
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_18);
    BUTTON_SetFont(hItem, GUI_FONT_16_1);
    BUTTON_SetText(hItem, "Damp FAN?");
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_19);
    BUTTON_SetFont(hItem, GUI_FONT_16_1);
	  BUTTON_SetText(hItem, "Come back");
   
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_20);
    BUTTON_SetFont(hItem, GUI_FONT_16_1);
	  BUTTON_SetText(hItem, "OPEN the cans");
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Temperature:");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Humidity:");
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Weight:");
   
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Temperature:");
		
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Humidity:");
   

    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetFont(hItem, GUI_FONT_20_1);
    TEXT_SetText(hItem, "Weight:");
	 
    case WM_NOTIFY_PARENT:                //操作触发消息处理(操作屏幕程序会跑到这里),比如点击按键、点击编辑框(任何的操作)等等......
    Id    = WM_GetId(pMsg->hWinSrc);      //返回小工具的ID
    NCode = pMsg->Data.v;                 //复选框的状态
    switch(Id) {  
		case ID_BUTTON_15: // Notifications sent by 'Button'   //通过按钮发送的通知
      switch(NCode) {
        case WM_NOTIFICATION_CLICKED:					
			   break;
        case WM_NOTIFICATION_RELEASED:
         break;
      }
			break;
    case ID_BUTTON_16: // Notifications sent by 'Button'   //通过按钮发送的通知
      switch(NCode) {
        case WM_NOTIFICATION_CLICKED:	
            			
            
			   break;
        case WM_NOTIFICATION_RELEASED:
         break;
      }
			 break;
			case ID_BUTTON_17: // Notifications sent by 'Button'   //通过按钮发送的通知
      switch(NCode) {
        case WM_NOTIFICATION_CLICKED:					

			   break;
        case WM_NOTIFICATION_RELEASED:
         break;
      }
			 break;
    case ID_BUTTON_18: // Notifications sent by 'Button'   //通过按钮发送的通知
      switch(NCode) {
        case WM_NOTIFICATION_CLICKED:					

			   break;
        case WM_NOTIFICATION_RELEASED:
         break;
        }
			 break;
			case ID_BUTTON_19: // Notifications sent by 'Button'   //通过按钮发送的通知
      switch(NCode) {
        case WM_NOTIFICATION_CLICKED:					
         GUI_EndDialog(pMsg->hWin, 0);	
         interface=1;	
         delay_ms(50);				
         CreateFramewin_1();
			   break;
        case WM_NOTIFICATION_RELEASED:
         break;
      }
			 break;
			case ID_BUTTON_20: // Notifications sent by 'Button'   //通过按钮发送的通知
      switch(NCode) {
        case WM_NOTIFICATION_CLICKED:					
            mq++;
				   
//				DC_Set_n(480);
//          LCD_ShowxNum(150,75,mq,1,24,0);
			   break;
        case WM_NOTIFICATION_RELEASED:
         break;
      }
      break; 
     default:
      WM_DefaultProc(pMsg);
      break;
  }
}


}
//界面1
static void _cbDialog_4(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  switch (pMsg->MsgId) {                   //区分各种消息类型
  case WM_INIT_DIALOG:                     //外观初始化| GUI_TA_VERTICAL
   
    hItem = pMsg->hWin;
    FRAMEWIN_SetTitleHeight(hItem, 100);
    FRAMEWIN_SetText(hItem, "Welcome to use smart trash cans");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_VCENTER |GUI_TA_CENTER);
    FRAMEWIN_SetFont(hItem, GUI_FONT_32B_1);
    FRAMEWIN_SetTextColor(hItem, 0x00000008);
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_22);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "Connect");
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_21);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "Back");
	 
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_23);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "Enter");
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_24);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "Score");
	
	  hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_25);
    BUTTON_SetFont(hItem, GUI_FONT_24_1);
	  BUTTON_SetText(hItem, "WIFI reset");
		
//		 hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
//    TEXT_SetFont(hItem, GUI_FONT_20_1);
//    TEXT_SetText(hItem, "notice:");
	  
	
	  case WM_NOTIFY_PARENT:                //操作触发消息处理(操作屏幕程序会跑到这里),比如点击按键、点击编辑框(任何的操作)等等......
    Id    = WM_GetId(pMsg->hWinSrc);      //返回小工具的ID
    NCode = pMsg->Data.v;                 //复选框的状态
    switch(Id) {
    case ID_BUTTON_21: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   GUI_EndDialog(pMsg->hWin, 0);
			     exhibition=0;
           exhibition_1=0;
           acv_flag=0;			
           interface=1;	
//			     esp_back=1;
			     ID1=0;
			     ID=0;
			     Look=0;
			     esp_reset=0;
			     delay_ms(250);
           CreateFramewin_1();	     			
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		}
     break;
		 case ID_BUTTON_22: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
			     Look=1;
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		}
     break;
		case ID_BUTTON_25: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   esp_reset=1;			
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		}
     break;
		 case ID_BUTTON_23: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   GUI_EndDialog(pMsg->hWin, 0);   
           if(ID1>10&&ID1<=300)		
					 {				 
           interface=3;
//           esp_back=1;						 
					 ID=0; 
					 ID1=0;
					 exhibition=0;
					 exhibition_1=0;
           acv_flag=0;						 
					 esp_reset=0;
           Look=0;						 
					 delay_ms(250);			 
           CreateFramewin_3();            					 
					 }	
           else 
					 {
					 interface=4;
					 CreateFramewin_4();	 
					 }						 
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		}
     break;
		case ID_BUTTON_24: // Notifications sent by 'Button'
      switch(NCode) {
		  case WM_NOTIFICATION_CLICKED:
				   exhibition=1;
           exhibition_1=1;
           acv_flag=1;	
			break;
		  case WM_NOTIFICATION_RELEASED:
			break;
		}
     break;
     default:
     WM_DefaultProc(pMsg);
     break;	 
  }
}
}
/***********创建对话框***************/


//创建界面1:
WM_HWIN CreateFramewin_1(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_1, GUI_COUNTOF(_aDialogCreate_1), _cbDialog_1, WM_HBKWIN, 0, 0);
  return hWin;
}
//创建界面2:
WM_HWIN CreateFramewin_2(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_2, GUI_COUNTOF(_aDialogCreate_2), _cbDialog_2, WM_HBKWIN, 0, 0);
  return hWin;
}

WM_HWIN CreateFramewin_3(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_3, GUI_COUNTOF(_aDialogCreate_3), _cbDialog_3, WM_HBKWIN, 0, 0);
  return hWin;
}
//创建界面2:
WM_HWIN CreateFramewin_4(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_4, GUI_COUNTOF(_aDialogCreate_4), _cbDialog_4, WM_HBKWIN, 0, 0);
  return hWin;
}
void my_gui_create(void)
{
	WM_HWIN hWin;
	GUI_Clear();
	hWin=CreateFramewin_1();
	while(1)
	{
		GUI_Delay(1000);
	}
}

void check_and_go(void)
{	
 int m,n,p;
 STMFLASH_Read(FLASH_PASSWORD_SAVE_ADDR,(u32*)Fraction,size_Fraction);
 if(Fraction[8]==0)
  {
	 Fraction[8]=1;
	 STMFLASH_Write(FLASH_PASSWORD_SAVE_ADDR,(u32*)Fraction,size_Fraction);
  }
  for(m=0;m<8;m++)
  {	 
	 p=0;
   for(n=0;n<11;n++)
	  {	
	   if( (input_number[n]-standard_pack[m][n])==0)
	   {	
      p++;
      if(p==11)	    
			{
				interface=3;	
        delay_ms(50);				
        CreateFramewin_3();
				for(n=0;n<11;n++)
				{input_number[n]=0;}
			}
			else if(p<11) YES=1;
     }
	  }	
   }
  STMFLASH_Write(FLASH_PASSWORD_SAVE_ADDR,(u32*)Fraction,size_Fraction);
}
	
void checkfunction(void)
{	
 int m,n,p;

 STMFLASH_Read(FLASH_PASSWORD_SAVE_ADDR,(u32*)Fraction,size_Fraction);
 if(Fraction[8]==0)
  {
	 Fraction[8]=1;
	 STMFLASH_Write(FLASH_PASSWORD_SAVE_ADDR,(u32*)Fraction,size_Fraction);
  }
  for(m=0;m<8;m++)
  {	 
	 p=0;
   for(n=0;n<11;n++)
	  {	
	   if( (input_number[n]-standard_pack[m][n])==0)
	   {	
      p++;
      if(p==11)	    
			{
				ID2=m;
			}
			else if(p<11) YES=1;
     }
	  }	
   }
  STMFLASH_Write(FLASH_PASSWORD_SAVE_ADDR,(u32*)Fraction,size_Fraction);
}




