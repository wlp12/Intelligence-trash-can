#include "usart2.h" 
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//����2��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2015/9/7
//�汾��V1.5
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved								  
//////////////////////////////////////////////////////////////////////////////////

UART_HandleTypeDef UART2_Handler;//UART���
//��ʼ��IO ����2
//bound:������	  
void usart2_init(u32 bound)
{  
	 
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
	__HAL_RCC_USART2_CLK_ENABLE();		//ʹ��USART2ʱ��
	//GPIO�˿�����
	GPIO_Initure.Pin=GPIO_PIN_2|GPIO_PIN_3;	//PA2��PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;		//����
	GPIO_Initure.Speed=GPIO_SPEED_FAST;//����
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART2
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA2\PA3
	//UART ��ʼ������
	UART2_Handler.Instance=USART2;					    			//USART2
	UART2_Handler.Init.BaudRate=bound;				  			//������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B; //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE; //��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;					//�շ�ģʽ
	HAL_UART_Init(&UART2_Handler);					    			//ʹ��

	__HAL_UART_ENABLE_IT(&UART2_Handler,UART_IT_RXNE);//���������ж�
	HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART2�ж�ͨ��
	HAL_NVIC_SetPriority(USART2_IRQn,1,0);	//��ռ���ȼ�1�������ȼ�0	
	TIM3_Init(1000-1,8400-1);		//100ms�ж�
	__HAL_TIM_DISABLE(&TIM3_Handler);//�رն�ʱ��3	
	USART2_RX_STA=0;		//����
  	
}

//���ڷ��ͻ����� 	
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
#ifdef USART2_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				    //���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
//ͨ���жϽ�������2���ַ�֮���ʱ������100ms�������ǲ���һ������������.
//���2���ַ����ռ������100ms,����Ϊ����1����������.Ҳ���ǳ���100msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART2_RX_STA=0;   	 
void USART2_IRQHandler(void)
{
	u8 Res;	    
	if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))//���յ�����
	{	 
		HAL_UART_Receive(&UART2_Handler,&Res,1,1000); 
		if((USART2_RX_STA&0x8000)==0)//�������һ������,��û�б�����,���ٽ�����������
		{ 		
			if(USART2_RX_STA<USART2_MAX_RECV_LEN)		//�����Խ�������
			{
				__HAL_TIM_SET_COUNTER(&TIM3_Handler,0);//���������        				 
				if(USART2_RX_STA==0)		
					__HAL_TIM_ENABLE(&TIM3_Handler);  //ʹ�ܶ�ʱ��3 
				USART2_RX_BUF[USART2_RX_STA++]=Res;		//��¼���յ���ֵ	 
			}else 
			{
				USART2_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
			} 
		}  	
  }										 
}  
#endif	
 
 
 











