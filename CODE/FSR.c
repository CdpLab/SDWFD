#include "stm32f10x.h"
#include "FSR.h"
#include "sys.h" 
#include "delay.h"
#include "usart.h"
#include "serial.h"				
#include "control.h"
//按键初始化函数
void FSR_IO_Init(void) //IO初始化
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //默认下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC
	
	GPIO_SetBits(GPIOC,GPIO_Pin_15);					//初始化设置为0
	
  EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
u8 state=0;
extern double* x;
extern  double* xx;
void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
	if(FSR_Scan(1) == 1 && state == 0)
		{  xx[11] -= 1;
				x=inv(xx[3],xx[7],xx[11],xx[15],xx[19],xx[23]);
					  servocontrol11(x);
			free(x);
				state = 1;
		}
		
		if(FSR_Scan(1) == 0 && state == 1)
		{
				//printf("低于阀值\r\n");
			Serial_SendString("contact\r\n");
				state = 0;
		}
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
u8 FSR_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&FSR_GPIO==1)
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(FSR_GPIO==1)return KEY_PRESS;
	}else if(FSR_GPIO==0)key_up=1; 	    
 	return 0;// 无按键按下
}
