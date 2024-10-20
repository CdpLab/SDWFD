#include "pca9685.h"
#include "myiic.h"
#include "delay.h"
#include "math.h"



void pca_write(u8 adrr,u8 data)//向PCA写数据,adrrd地址,data数据
{ 
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	
	IIC_Stop();
}

u8 pca_read(u8 adrr)//从PCA读数据
{
	u8 data;
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr);
	IIC_Wait_Ack();
	
	IIC_Send_Byte(adrr);
	IIC_Wait_Ack();
	
	IIC_Start();
	
	IIC_Send_Byte(pca_adrr|0x01);
	IIC_Wait_Ack();
	
	data=IIC_Read_Byte(0);
	IIC_Stop();
	
	return data;
}


void pca_setfreq(float freq)//设置PWM频率
{
		u8 prescale,oldmode,newmode;
		double prescaleval;
		freq *= 0.92; 
		prescaleval = 25000000;
		prescaleval /= 4096;
		prescaleval /= freq;
		prescaleval -= 1;
		prescale =floor(prescaleval + 0.5f);//freq是我们需要的频率，prescale是我们需要设置地值

		oldmode = pca_read(pca_mode1);
	
		newmode = (oldmode&0x7F) | 0x10; // sleep
	
		pca_write(pca_mode1, newmode); // go to sleep
	
		pca_write(pca_pre, prescale); // set the prescaler
	
		pca_write(pca_mode1, oldmode);
		delay_ms(2);
	
		pca_write(pca_mode1, oldmode | 0xa1); 
}


/*num:舵机PWM输出引脚0~15，on:PWM上升计数值0~4096,off:PWM下降计数值0~4096
一个PWM周期分成4096份 
由0开始+1计数，计到on时跳变为高电平，继续计数到off时，跳变为低电平，直到计满4096重新开始。

所以当 on 不等于0时可作延时,当on等于0时，off/4096的值就是PWM的占空比。*/

//on = 0, off/4096为占空比
//
//设置pwm占空比，当on为0时，0ff / 4096 的值就为占空比
void pca_setpwm(u8 num, u32 on, u32 off)
{
		pca_write(LED0_ON_L+4*num,on);
		pca_write(LED0_ON_H+4*num,on>>8);
		pca_write(LED0_OFF_L+4*num,off);
		pca_write(LED0_OFF_H+4*num,off>>8);
}


/*
	函数作用：初始化舵机驱动板
	参数：1.PWM频率
		    2.初始化舵机角度
  功能：0-15通道全部设置为0，设置舵机频率以及初始角度
*/
void Servo_Init(float hz,u8 angle)
{
	u32 off=0;                    //占空比为0
	IIC_Init();
	pca_write(pca_mode1,0x0);
	pca_setfreq(hz);              //设置PWM频率
	off=(u32)(103+angle*2.2);
	pca_setpwm(0,0,off);
	pca_setpwm(1,0,off);
	pca_setpwm(2,0,off);
	pca_setpwm(3,0,off);
	pca_setpwm(4,0,off);
	pca_setpwm(5,0,off);
	pca_setpwm(6,0,off);
	pca_setpwm(7,0,off);
	pca_setpwm(8,0,off);
	pca_setpwm(9,0,off);
	pca_setpwm(10,0,off);
	pca_setpwm(11,0,off);
	pca_setpwm(12,0,off);
	pca_setpwm(13,0,off);
	pca_setpwm(14,0,off);
	pca_setpwm(15,0,off);
	delay_ms(500);
}




/*
	函数作用： 控制舵机转动
	参数：     num：pca9685 输出端口0-15 ； angle：角度，比如输入60，舵机则转动60度。
  功能：     设置舵机按照预设角度转动
 
*/
//void Servo_angle(u8 num,u8 target_angle)
//{u8 current_angle = 0;
//	u32 off = 0;
//	 while (current_angle != target_angle) {
//		 if (current_angle < target_angle) {
//            current_angle++; // 逐步增加角度
//        } else {
//            current_angle--; // 逐步减少角度
//        }
//  off = (u32)(103+current_angle*2.2);
//	pca_setpwm(num,0,off);
//			delay_ms(20);
//}
//	 }

void Servo_angle(u8 num,u8 angle)
{
	u32 off = 0;
  off = (u32)(103+angle*2.2);
	
		pca_setpwm(num,0,off);
	
	
	
//	 u8 target_angle = 0; // 目标角度
//    static u8 current_angle[16] = {60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60}; // 当前角度
//     u8 total_steps = 0; // 总步数
//     u8 step_delay = 0; // 步间延时
//     u32 off_time = 0; // PWM 下降计数值
//    // 设置目标角度
//    target_angle = angle;
//    // 插补总量
//    total_steps = abs(target_angle*2.2 - current_angle[num]*2.2); // 总步数为目标角度和当前角度之差的绝对值
//        int n=100;// 计算插补次数
//    u8 dp = 0;
//    if (target_angle > current_angle[num])// 计算插补增量
//        dp = total_steps/n;
//    else if (target_angle < current_angle[num])
//        dp = -total_steps/n;
//off_time = (u32)(103 + current_angle[num] * 2.2);
//    // 开始插补运动
//    for (int step_count = 0; step_count < n; step_count++) { 
////       if(abs(off_time-(u16)(103 + target_angle * 2.2))<2)
////				 {
////				 off_time = (u16)(103 + target_angle * 2.2);
////				 pca_setpwm(num, 0, off_time);
////			    }
////			 else
//				   // 计算下降时间
//			   off_time +=dp;
//         pca_setpwm(num, 0, off_time); // 更新舵机 PWM     
//			delay_ms(2);
//    }
//		current_angle[num]=target_angle;
		
		

}
