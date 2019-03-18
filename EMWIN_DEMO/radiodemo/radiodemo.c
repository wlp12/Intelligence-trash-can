#include "radiodemo.h"
#include "DIALOG.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//STemwin RADIO控件使用   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/3/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//用户自定义消息
//#define CHANGE_COLOR	(WM_USER +0)

////控件 ID
//#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)
//#define ID_RADIO_0      (GUI_ID_USER + 0x01)
//#define ID_BUTTON_00     (GUI_ID_USER + 0x02)

//int colortbl[]={GUI_RED,GUI_BLUE,GUI_WHITE,GUI_YELLOW,GUI_GREEN};   //背景颜色表

//radio_struct radio_widg;

////对话框资源表
//static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = 
//{
////    { FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 600, 180, FRAMEWIN_CF_MOVEABLE, 0x64, 0 },
//    { RADIO_CreateIndirect, "Radio", ID_RADIO_0,    100, 10, 150, 150, 0, 0x1e05, 0 },
//    { BUTTON_CreateIndirect, "Button", ID_BUTTON_00, 100, 160, 80, 40, 0, 0x0, 0 },
//};

////背景窗口的回调函数
//static void _BkCallback(WM_MESSAGE *pMsg)
//{
//	static WM_HWIN hWin;
//	char text[30];
//	switch(pMsg->MsgId)
//	{
//		case WM_PAINT:
////			GUI_SetFont(&GUI_Font24_ASCII);
////            GUI_SetBkColor(colortbl[radio_widg.value]);//设置背景颜色
////            GUI_Clear();
////        
////            GUI_SetColor(GUI_BLACK);                    //设置前景色
////        	GUI_DispStringAt("WIDGET_RADIO - Sample",0,0);
////            GUI_SetFont(&GUI_Font32_ASCII);
////            sprintf(text,"RADIO VALUE:%d",radio_widg.value);     
////            GUI_DispStringAt(text,0,32);
//			break;	
//		case CHANGE_COLOR:
////			hWin=pMsg->hWinSrc;
////			WM_HideWindow(hWin);	        //隐藏对话框
////			WM_InvalidateWindow(WM_HBKWIN); //背景窗口无效
////			WM_CreateTimer(WM_HBKWIN,0,3000,0);
//			break;
//		case WM_TIMER:
////			WM_ShowWindow(hWin);
//			break;
//		default:
//			WM_DefaultProc(pMsg);
//			break;
//	}	
//}

////对话框回调函数
//static void _cbDialog(WM_MESSAGE * pMsg) 
//{
//    WM_HWIN hItem;
//    int     NCode;
//    int     Id;
//    WM_MESSAGE Msg;
//    
//    switch (pMsg->MsgId) 
//    {
//        case WM_INIT_DIALOG:
//            //初始化FRAME
//            hItem = pMsg->hWin;
//            FRAMEWIN_SetText(hItem, "Please chose way");
//            FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
//            FRAMEWIN_SetFont(hItem, GUI_FONT_24_ASCII);
//            FRAMEWIN_SetTextColor(hItem, 0x0000FFFF);

//            //初始化RADIO
//            hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
//            RADIO_SetTextColor(hItem, 0x00000000);
//				    RADIO_SetText(hItem, "ensure",         0);
//				    RADIO_SetText(hItem, "phone nunber",   1);
//				    RADIO_SetText(hItem, "face",           2);
//            RADIO_SetFont(hItem, GUI_FONT_20_ASCII);
//            
//            //初始化BUTTON0
//            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_00);
//            BUTTON_SetText(hItem, "OK");
//            BUTTON_SetFont(hItem, GUI_FONT_24_ASCII);
//            break;
//        case WM_NOTIFY_PARENT:
//            Id    = WM_GetId(pMsg->hWinSrc);
//            NCode = pMsg->Data.v;
//            switch(Id) 
//            {
//                case ID_RADIO_0: //RADIO0通知代码
//                    switch(NCode) 
//                    {
//                        case WM_NOTIFICATION_CLICKED:
//                            break;
//                        case WM_NOTIFICATION_RELEASED:
//                            break;
//                        case WM_NOTIFICATION_VALUE_CHANGED:
//                            hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);   //获取RADIO的句柄
//                            radio_widg.value=RADIO_GetValue(hItem);             //获取RADIO值
//                            break;
//                    }
//                    break;
//                case ID_BUTTON_00: //BUTTON0通知代码
//                    switch(NCode) 
//                    {
//                        case WM_NOTIFICATION_CLICKED:
//                            break;
//                        case WM_NOTIFICATION_RELEASED:
//                            Msg.MsgId=CHANGE_COLOR;
//							Msg.hWinSrc=pMsg->hWin;
//							WM_SendMessage(WM_HBKWIN,&Msg);
//							break;
//                    }
//                    break;
//            }
//            break;
//    default:
//        WM_DefaultProc(pMsg);
//        break;
//    }
//}

//////创建RADIO控件演示例程
//void CreateRadioDemo(void) 
//{
////    WM_SetCallback(WM_HBKWIN,_BkCallback);
//    
//    radio_widg.value=0;         //默认为0
//    radio_widg.rdadio = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
//}

