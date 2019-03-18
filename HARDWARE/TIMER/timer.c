#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//��ʱ���ж���������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

extern vu16 USART2_RX_STA;
extern vu16 USART3_RX_STA;
TIM_HandleTypeDef TIM7_Handler;      //��ʱ����� 
TIM_HandleTypeDef TIM3_Handler;      //��ʱ����� 

TIM_HandleTypeDef TIM2_Handler;         //��ʱ��3PWM��� 
TIM_OC_InitTypeDef TIM2_CH1Handler;	    //��ʱ��3ͨ��4���
TIM_OC_InitTypeDef TIM2_CH2Handler;	    //��ʱ��3ͨ��4���

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!(��ʱ��3����APB1�ϣ�ʱ��ΪHCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
	__HAL_RCC_TIM3_CLK_ENABLE();            //ʹ��TIM3ʱ��
	
	TIM3_Handler.Instance=TIM3;                          //ͨ�ö�ʱ��3
	TIM3_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
	TIM3_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM3_Handler);
    	
	HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
	HAL_NVIC_EnableIRQ(TIM3_IRQn);          //����ITM3�ж�   
  HAL_TIM_Base_Start_IT(&TIM3_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE   
}

//��ʱ��3�жϷ�����
void TIM3_IRQHandler(void)
{
  if(__HAL_TIM_GET_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(&TIM3_Handler, TIM_IT_UPDATE) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(&TIM3_Handler, TIM_IT_UPDATE);
      USART2_RX_STA|=1<<15;	//��ǽ������
			__HAL_TIM_DISABLE(&TIM3_Handler);//�رն�ʱ��3
    }
  }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_TIM2_CLK_ENABLE();			//ʹ�ܶ�ʱ��2
    __HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	  __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_15;           	
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	  GPIO_Initure.Alternate= GPIO_AF1_TIM2;	//TIM2_CH1
	  HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	  GPIO_Initure.Pin=GPIO_PIN_3;          
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
	  GPIO_Initure.Alternate= GPIO_AF1_TIM2;	//TIM2_CH2
	  HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
}

void TIM2_PWM_Init(u16 arr,u16 psc)
{ 
    TIM2_Handler.Instance=TIM2;            //��ʱ��3
    TIM2_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM2_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM2_Handler);       //��ʼ��PWM
    
    TIM2_CH1Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM2_CH1Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM2_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH1Handler,TIM_CHANNEL_1);//����TIM3ͨ��4
	
	  TIM2_CH2Handler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM2_CH2Handler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM2_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW; //����Ƚϼ���Ϊ�� 
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH2Handler,TIM_CHANNEL_2);//����TIM3ͨ��4
	
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_1);//����PWMͨ��4
	  HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);//����PWMͨ��4
		
}

//������ʱ��7�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM7_Int_Init(u16 arr,u16 psc)
{
		TIM7_Handler.Instance=TIM7;                          //ͨ�ö�ʱ��3
    TIM7_Handler.Init.Prescaler=psc;                     //��Ƶϵ��
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM7_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(&TIM7_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //ʹ�ܶ�ʱ��3�Ͷ�ʱ��3�����жϣ�TIM_IT_UPDATE									 
}
//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();            //ʹ��TIM7ʱ��
		HAL_NVIC_SetPriority(TIM7_IRQn,0,1);    //�����ж����ȼ�����ռ���ȼ�0�������ȼ�1
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //����ITM7�ж�   
	}
}
//��ʱ��7�жϷ������		    
void TIM7_IRQHandler(void)
{ 	    		    
		USART3_RX_STA|=1<<15;	//��ǽ������
		__HAL_TIM_CLEAR_FLAG(&TIM7_Handler,TIM_EventSource_Update );       //���TIM7�����жϱ�־  
		TIM7->CR1&=~(1<<0);     			//�رն�ʱ��7     											 
} 


void TIM_SetTIM2Compare(u32 compare,u8 mode)
{
	switch(mode)
	{
		case 1:
		TIM2->CCR1=compare; 
		break;
		case 2:
		TIM2->CCR2=compare; 
		break;
		case 3:
		TIM2->CCR3=compare; 
		break;
		case 4:
		TIM2->CCR4=compare; 
		break;
	}
}
