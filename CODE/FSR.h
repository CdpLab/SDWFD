#ifndef __FSR_H
#define __FSR_H	 
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define KEY3 PAin(0)	//PA0  WK_UP

#define FSR_GPIO  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)//读取



#define KEY_PRESS		1


void FSR_IO_Init(void);//IO初始化
void EXTI15_10_IRQHandler(void);
u8 FSR_Scan(u8);  	//按键扫描函数	
void Adc_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
long map(long x, long in_min, long in_max, long out_min, long out_max);
uint16_t average(uint16_t arr[], int size);
#endif
