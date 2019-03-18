#ifndef _my_gui_H
#define _my_gui_H
#include "sys.h"
#include "WM.h"

void checkfunction(void);
void my_gui_create(void);//创建GUI
void read_config(void);
void auto_config(void);
void reset_flag(void);
void door_open_control(void);
void door_close_control(void);
void door_sleep1_control(void);
void door_sleep2_control(void);

typedef struct
{
    WM_HWIN rdadio; //RADIO窗口句柄
    u8      value;  //RADIO当前所选中的ITEM
}radio_struct;

void CreateRadioDemo(void);


#endif


