#ifndef __HX711_H__ 
#define __HX711_H__ 



void ADInit(void); //AD模块
double Get_Maopi(void);
unsigned long HX711_Read(void);  //读取AD值 
double Get_Weight(void);
double Get_Maopi_1(void);
unsigned long HX711_Read_1(void);  //读取AD值 
double Get_Weight_1(void);



#endif



