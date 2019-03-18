#ifndef _my_gui_H
#define _my_gui_H
#include "sys.h"
#include "WM.h"

void checkfunction(void);
void my_gui_create(void);//����GUI
void read_config(void);
void auto_config(void);
void reset_flag(void);
void door_open_control(void);
void door_close_control(void);
void door_sleep1_control(void);
void door_sleep2_control(void);

typedef struct
{
    WM_HWIN rdadio; //RADIO���ھ��
    u8      value;  //RADIO��ǰ��ѡ�е�ITEM
}radio_struct;

void CreateRadioDemo(void);


#endif


