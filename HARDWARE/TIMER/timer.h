#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//定时器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/6
//版本：V1.0
//版权所有，盗版必究。a
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 
extern TIM_HandleTypeDef TIM2_Handler;      //定时器3PWM句柄 
extern TIM_OC_InitTypeDef TIM2_CH1Handler;  //定时器3通道4句柄
extern TIM_OC_InitTypeDef TIM2_CH2Handler;  //定时器3通道4句柄


void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);
void TIM_SetTIM2Compare(u32 compare,u8 mode);



#endif

