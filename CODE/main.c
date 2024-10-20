#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "pca9685.h"
#include "usart.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <stdarg.h>
#include "inverse.h"
#include "math.h"
#include "serial.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "FSR.h"
#include "control.h"
#define NUM_SERVOS 6
#define INCREMENT 1.0  // 每次更新的角度增量
#define TOLERANCE 1
#define DELAY_MS 20  // 延迟时间

//差分法参数	

double t=0; 
int i = 0;
	
u8 state = 0;	
	
extern  double *x;
double aa[6];
uint8_t RxData;
double val1 = 180 / 3.1415926;
int i1=0;
		int i2=0;
		double xx[24];
		uint8_t  yy[4];
    int  t1;
		double y1;
		int t2;
		double current_angles[NUM_SERVOS] = {60, 60, 60, 60, 60, 60};  // 初始化为当前角度
 int servo_pins[6] = {4, 7, 8, 11, 12, 15};			
	static u8 current_angle[6] = {0,0,0,0,0,0};	








	
	
		
void print11(double *x){
	for(int i=0;i<6;i++)
	{ aa[i]=x[i];
		Serial_SendString("\r\nAngel is:");
		Serial_SendNumber(aa[i],2);
		delay_ms(2000);
	}
};




int main(void)
{	
float pitch,roll,yaw;
	delay_init();//延时函数初始化
	Servo_Init(50,60);//初始化舵机驱动
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	delay_ms(1000);
	//uart_init(9600);
	Serial_Init();

	
	//NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	FSR_IO_Init();
	//	printf("Test start\r\n");

//	MPU_Init();		
//while(mpu_dmp_init())
//	{
//	delay_ms(20);
//	}	





	while(1)
		{

		 

			
	
     if (Serial_GetRxFlag() == 1 )
				 {

					 t1=Serial_GetRxData();
					 xx[t2]=(double)t1;
					 t2++;
					 if(t2==24)
					 {
						 t2=0;
						x=inv(xx[3],xx[7],xx[11],xx[15],xx[19],xx[23]);
					  servocontrol11(x);
						free(x);
				   }
			 }		
			
			
			
	
			
 }
				}
				
				
				
				


		