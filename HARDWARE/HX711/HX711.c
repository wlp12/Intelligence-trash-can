#include "hx711.h" 
#include "delay.h" 

#define ADIO   GPIOC        //ad口  
#define DATA   GPIO_Pin_12	  //数据口 
#define CLK    GPIO_Pin_11	  //时钟 
#define DATA_1 GPIO_Pin_10	  //数据口1
#define CLK_1  GPIO_Pin_9	  //时钟1 
#define ADCLK RCC_AHB1Periph_GPIOC  //生命ad时钟挂在APB2P.C上

double hx711_buffer=0,hx711_buffer_1=0;//定义hx711_buffer来接受hx711read读取的信息
double	weight_maopi=0,weight_maopi_1=0;//毛皮重量
double weight_shiwu=0,weight_shiwu_1=0;//实物重量
double a,c;//处理毛皮重量等同于毛皮重量（可有可无）
double b,d;//毛皮重量小数部分
u32 am,bm;//毛皮重量整数部分
  
void ADInit(void) //AD初始化
{ 
	GPIO_InitTypeDef GPIO_InitStructure; //结构体

	RCC_AHB1PeriphClockCmd(ADCLK,ENABLE); //使能时钟口

	GPIO_InitStructure.GPIO_Pin = CLK; //PC10为时钟口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //推挽输出 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(ADIO,&GPIO_InitStructure); //PC.10时钟口

	GPIO_InitStructure.GPIO_Pin = DATA; //PC11为数据口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	GPIO_Init(ADIO,&GPIO_InitStructure);	//PC.11数据口
	
	GPIO_InitStructure.GPIO_Pin = CLK_1; //PC8为时钟口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //推挽输出 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(ADIO,&GPIO_InitStructure); //PC.8时钟口

	GPIO_InitStructure.GPIO_Pin = DATA_1; //PC9为数据口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	GPIO_Init(ADIO,&GPIO_InitStructure);	//PC.9数据口
	} 

unsigned long HX711_Read(void)  //读取711读到的数据
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
unsigned long HX711_Read_1(void)  //读取711读到的数据
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
	  weight_maopi=weight_maopi/7.26;//参数可换，不同的称重模块对应不同的参数，需自己标定
	  a=weight_maopi;
	  am=(u32)weight_maopi;
    return a;	//返回值	
  }
double Get_Maopi_1(void)
  {
	  hx711_buffer_1=HX711_Read_1();
	  weight_maopi_1=hx711_buffer_1/100;
	  weight_maopi_1=weight_maopi_1/7.26;//参数可换，不同的称重模块对应不同的参数，需自己标定
	  c=weight_maopi_1;
	  bm=(u32)weight_maopi_1;
    return c;	//返回值	
  }
double Get_Weight(void)
{
	double m;
	hx711_buffer=HX711_Read();
	hx711_buffer=hx711_buffer/100;
	hx711_buffer=hx711_buffer/7.26;//处理实物重量
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
	hx711_buffer_1=hx711_buffer_1/7.26;//处理实物重量
	if(hx711_buffer_1>weight_maopi_1)
	{
	weight_shiwu_1=hx711_buffer_1;
	weight_shiwu_1=weight_shiwu_1-weight_maopi_1;
	}
	m=weight_shiwu_1;
	return m;
}
