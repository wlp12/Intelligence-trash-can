#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//定时器中断驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

extern vu16 USART2_RX_STA;
extern vu16 USART3_RX_STA;
TIM_HandleTypeDef TIM7_Handler;      //定时器句柄 
TIM_HandleTypeDef TIM3_Handler;      //定时器句柄 

TIM_HandleTypeDef TIM2_Handler;         //定时器3PWM句柄 
TIM_OC_InitTypeDef TIM2_CH1Handler;	    //定时器3通道4句柄
TIM_OC_InitTypeDef TIM2_CH2Handler;	    //定时器3通道4句柄

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
	__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
	
	TIM3_Handler.Instance=TIM3;                          //通用定时器3
	TIM3_Handler.Init.Prescaler=psc;                     //分频系数
	TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
	TIM3_Handler.Init.Period=arr;                        //自动装载值
	TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	HAL_TIM_Base_Init(&TIM3_Handler);
    	
	HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
	HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
  HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
  if(__HAL_TIM_GET_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE) != RESET)
  {
    if(__HAL_TIM_GET_IT_SOURCE(&TIM3_Handler, TIM_IT_UPDATE) !=RESET)
    {
      __HAL_TIM_CLEAR_IT(&TIM3_Handler, TIM_IT_UPDATE);
      USART2_RX_STA|=1<<15;	//标记接收完成
			__HAL_TIM_DISABLE(&TIM3_Handler);//关闭定时器3
    }
  }
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	  __HAL_RCC_TIM2_CLK_ENABLE();			//使能定时器2
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	  __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_15;           	
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	  GPIO_Initure.Alternate= GPIO_AF1_TIM2;	//TIM2_CH1
	  HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	
	  GPIO_Initure.Pin=GPIO_PIN_3;          
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	  GPIO_Initure.Alternate= GPIO_AF1_TIM2;	//TIM2_CH2
	  HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
}

void TIM2_PWM_Init(u16 arr,u16 psc)
{ 
    TIM2_Handler.Instance=TIM2;            //定时器3
    TIM2_Handler.Init.Prescaler=psc;       //定时器分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM2_Handler.Init.Period=arr;          //自动重装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM2_Handler);       //初始化PWM
    
    TIM2_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM2_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM2_CH1Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH1Handler,TIM_CHANNEL_1);//配置TIM3通道4
	
	  TIM2_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM2_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM2_CH2Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CH2Handler,TIM_CHANNEL_2);//配置TIM3通道4
	
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_1);//开启PWM通道4
	  HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);//开启PWM通道4
		
}

//基本定时器7中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM7_Int_Init(u16 arr,u16 psc)
{
		TIM7_Handler.Instance=TIM7;                          //通用定时器3
    TIM7_Handler.Init.Prescaler=psc;                     //分频系数
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM7_Handler.Init.Period=arr;                        //自动装载值
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM7_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM7_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE									 
}
//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM7)
	{
		__HAL_RCC_TIM7_CLK_ENABLE();            //使能TIM7时钟
		HAL_NVIC_SetPriority(TIM7_IRQn,0,1);    //设置中断优先级，抢占优先级0，子优先级1
		HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断   
	}
}
//定时器7中断服务程序		    
void TIM7_IRQHandler(void)
{ 	    		    
		USART3_RX_STA|=1<<15;	//标记接收完成
		__HAL_TIM_CLEAR_FLAG(&TIM7_Handler,TIM_EventSource_Update );       //清除TIM7更新中断标志  
		TIM7->CR1&=~(1<<0);     			//关闭定时器7     											 
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
