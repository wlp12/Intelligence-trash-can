#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//��ʱ����������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/6
//�汾��V1.0
//��Ȩ���У�����ؾ���a
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
extern TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 
extern TIM_HandleTypeDef TIM2_Handler;      //��ʱ��3PWM��� 
extern TIM_OC_InitTypeDef TIM2_CH1Handler;  //��ʱ��3ͨ��4���
extern TIM_OC_InitTypeDef TIM2_CH2Handler;  //��ʱ��3ͨ��4���


void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);
void TIM_SetTIM2Compare(u32 compare,u8 mode);



#endif

