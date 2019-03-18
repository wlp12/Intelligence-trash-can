#include "hx711.h" 
#include "delay.h" 

#define ADIO   GPIOC        //ad��  
#define DATA   GPIO_Pin_12	  //���ݿ� 
#define CLK    GPIO_Pin_11	  //ʱ�� 
#define DATA_1 GPIO_Pin_10	  //���ݿ�1
#define CLK_1  GPIO_Pin_9	  //ʱ��1 
#define ADCLK RCC_AHB1Periph_GPIOC  //����adʱ�ӹ���APB2P.C��

double hx711_buffer=0,hx711_buffer_1=0;//����hx711_buffer������hx711read��ȡ����Ϣ
double	weight_maopi=0,weight_maopi_1=0;//ëƤ����
double weight_shiwu=0,weight_shiwu_1=0;//ʵ������
double a,c;//����ëƤ������ͬ��ëƤ���������п��ޣ�
double b,d;//ëƤ����С������
u32 am,bm;//ëƤ������������
  
void ADInit(void) //AD��ʼ��
{ 
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ��

	RCC_AHB1PeriphClockCmd(ADCLK,ENABLE); //ʹ��ʱ�ӿ�

	GPIO_InitStructure.GPIO_Pin = CLK; //PC10Ϊʱ�ӿ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //������� 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(ADIO,&GPIO_InitStructure); //PC.10ʱ�ӿ�

	GPIO_InitStructure.GPIO_Pin = DATA; //PC11Ϊ���ݿ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	GPIO_Init(ADIO,&GPIO_InitStructure);	//PC.11���ݿ�
	
	GPIO_InitStructure.GPIO_Pin = CLK_1; //PC8Ϊʱ�ӿ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //������� 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(ADIO,&GPIO_InitStructure); //PC.8ʱ�ӿ�

	GPIO_InitStructure.GPIO_Pin = DATA_1; //PC9Ϊ���ݿ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	GPIO_Init(ADIO,&GPIO_InitStructure);	//PC.9���ݿ�
	} 

unsigned long HX711_Read(void)  //��ȡ711����������
{ 
	unsigned long val = 0; 
	unsigned char i = 0; 
	float p;
	GPIO_SetBits(ADIO,DATA); 
	GPIO_ResetBits(ADIO,CLK); 
	while(GPIO_ReadInputDataBit(ADIO,DATA)); 
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(ADIO,CLK); 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(ADIO,CLK); 
		if(GPIO_ReadInputDataBit(ADIO,DATA)) 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(ADIO,CLK); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(ADIO,CLK); 
	delay_us(1); 
	p=val;
	return p; 	
}
unsigned long HX711_Read_1(void)  //��ȡ711����������
{ 
	unsigned long val = 0; 
	unsigned char i = 0; 
	float p;
	GPIO_SetBits(ADIO,DATA_1); 
	GPIO_ResetBits(ADIO,CLK_1); 
	while(GPIO_ReadInputDataBit(ADIO,DATA_1)); 
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(ADIO,CLK_1); 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(ADIO,CLK_1); 
		if(GPIO_ReadInputDataBit(ADIO,DATA_1)) 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(ADIO,CLK_1); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(ADIO,CLK_1); 
	delay_us(1); 
	p=val;
	return p; 	
}
double Get_Maopi(void)
  {
	  hx711_buffer=HX711_Read();
	  weight_maopi=hx711_buffer/100;
	  weight_maopi=weight_maopi/7.26;//�����ɻ�����ͬ�ĳ���ģ���Ӧ��ͬ�Ĳ��������Լ��궨
	  a=weight_maopi;
	  am=(u32)weight_maopi;
    return a;	//����ֵ	
  }
double Get_Maopi_1(void)
  {
	  hx711_buffer_1=HX711_Read_1();
	  weight_maopi_1=hx711_buffer_1/100;
	  weight_maopi_1=weight_maopi_1/7.26;//�����ɻ�����ͬ�ĳ���ģ���Ӧ��ͬ�Ĳ��������Լ��궨
	  c=weight_maopi_1;
	  bm=(u32)weight_maopi_1;
    return c;	//����ֵ	
  }
double Get_Weight(void)
{
	double m;
	hx711_buffer=HX711_Read();
	hx711_buffer=hx711_buffer/100;
	hx711_buffer=hx711_buffer/7.26;//����ʵ������
	if(hx711_buffer>weight_maopi)
	{
	weight_shiwu=hx711_buffer;
	weight_shiwu=weight_shiwu-weight_maopi;
	}
	m=weight_shiwu;
	return m;
}

double Get_Weight_1(void)
{
	double m;
	hx711_buffer_1=HX711_Read_1();
	hx711_buffer_1=hx711_buffer_1/100;
	hx711_buffer_1=hx711_buffer_1/7.26;//����ʵ������
	if(hx711_buffer_1>weight_maopi_1)
	{
	weight_shiwu_1=hx711_buffer_1;
	weight_shiwu_1=weight_shiwu_1-weight_maopi_1;
	}
	m=weight_shiwu_1;
	return m;
}
