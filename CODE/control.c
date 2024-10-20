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
#define NUM_SERVOS 6
#define INCREMENT 1.0  // 每次更新的角度增量
#define TOLERANCE 1
#define DELAY_MS 20  // 延迟时间

//差分法参数	
double timestep = 0.009;
double a0; double a1; double a2; double a3;double a4; double a5;
double omegaf;double omegai; double acci; double accf;
double tf = 0.8;
double thetai=45; double thetaf=135;
double theta0;double omega0;double acc0;
double t=0; 
int i = 0;
	
u8 state = 0;	
	
double *x;
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





//			for ( t = 0; t <= tf; t += timestep) {
//    thetai=60; thetaf=120;
//    omegai = 35; omegaf = 125;
//    acci = 20; accf = 40;
//    // 计算插值系数
//    a0 = thetai;
//    a1 = omegai;
//    a2 = acci/2;
//    a3 = (20*thetaf-20*thetai-(8*omegaf+12*omegai)*tf-(3*acci-accf)*(pow(tf,2))) / (2*(pow(tf,3)));
//    a4 = (30*thetai-30*thetaf+(14*omegaf+16*omegai)*tf+(3*acci-2*accf)*(pow(tf,2))) / (2*(pow(tf,4)));
//    a5 = (12*thetaf - 12*thetai - (6*omegaf+6*omegai)*tf-(acci-accf)*(pow(tf,2))) / (2*(pow(tf,5)));
//    // 计算当前时刻的角度、角速度和角加速度
//    theta0 = a0 +a1*t +a2*(pow(t,2))+ a3*(pow(t,3)) + a4*(pow(t,4)) + a5*(pow(t,5));
//    omega0 = a1 + 2*a2*t + 3*a3*(pow(t,2)) + 4*a4*(pow(t,3)) + 5*a5*(pow(t,4));
//    acc0 = 2*a2 + 6*a3*t + 12*a4*(pow(t,2)) + 20*a5*(pow(t,3));
//    // 设置舵机角度
//    Servo_angle(4,theta0);





void servocontrol11(double *x) {
    int x_int[NUM_SERVOS];  // 新的整数数组

    // 将目标数组 x 中的浮点数转换为整数
    for (int i = 0; i < NUM_SERVOS; i++) {
        x_int[i] = (int)round(x[i]);  // 使用 round 将浮点数四舍五入，并转为 int 类型
    }

		for (int i = 0; i < 6; i++) {
    if (x_int[i] < -60) {
        x_int[i] = -60; // 如果角度小于 -60，则截断为 -60
    } else if (x_int[i] > 60) {
        x_int[i] = 60; // 如果角度大于 60，则截断为 60
}
		
    // 进入循环，直到所有角度都达到目标
    int reached[NUM_SERVOS] = {0};  // 标记每个舵机是否达到目标
    while (1) {
        int all_reached = 1;  // 标记是否所有舵机都达到目标

        for (int i = 0; i < NUM_SERVOS; i++) {
            if (!reached[i]) {
                // 如果 current_angle[i] 和 x_int[i] 不相等，更新角度并控制舵机
                if (current_angle[i] < x_int[i]) {
                    current_angle[i]++;
                } else if (current_angle[i] > x_int[i]) {
                    current_angle[i]--;
                }

                // 当角度发生变化时，控制舵机
                if (i % 2 == 0) {  // i 是偶数
                    Servo_angle(servo_pins[i], 60 - current_angle[i]);
                } else {  // i 是奇数
                    Servo_angle(servo_pins[i], 60 + current_angle[i]);
                }

                // 检查是否已经达到目标角度
                if (abs(current_angle[i] - x_int[i]) <= TOLERANCE) {
                    reached[i] = 1;  // 标记为已达到目标
                } else {
                    all_reached = 0;  // 有舵机未达到目标
                }
            }
        }

        // 如果所有舵机都达到目标角度，则退出循环
        if (all_reached) {
            break;
        }

        // 每次循环结束后，增加适当的延迟，避免舵机频繁更新
        delay_ms(DELAY_MS);
    }
}




