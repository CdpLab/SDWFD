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
#define MAX_SOLUTIONS 10

double *inv(double x1, double y, double z, double roll, double  pitch, double  yaw)//三个方向平移量 和三个欧拉角 横滚x 俯仰y 偏航z
{double val1 = 180 / 3.1415926;
	double val = 3.1415926 / 180;
	double l[6];// 连杆虚拟长度长度
	double Rb = 33;//33
	double Rp = 25;//18
	double h0;//平台初始高度
	double beta[6] = { -60,120,60,-120,180,0 };//舵机臂与下平台的夹角
	 double* alpha = malloc(6 * sizeof(double));//舵机臂转动的角度
	double  p[6][3];//上平台坐标
	double  b[6][3];//下平台坐标
	double  q[6][3];//上平台相对基坐标系的坐标
	double L[6];
	double M[6];
	double N[6];//计算α的三个参数
	double Rpb[3][3] = {
			cos(yaw * val) * cos(pitch * val), cos(yaw * val) * sin(pitch * val) * sin(roll * val) - sin(yaw * val) * cos(roll * val), cos(yaw * val) * sin(pitch * val) * cos(roll * val) + sin(yaw * val) * sin(roll * val),
			sin(yaw * val) * cos(pitch * val), sin(yaw * val) * sin(pitch * val) * sin(roll * val) + cos(yaw * val) * cos(roll * val), sin(yaw * val) * sin(pitch * val) * cos(roll * val) - cos(yaw * val) * sin(roll * val),
			-sin(pitch * val), cos(pitch * val) * sin(roll * val), cos(pitch * val) * cos(roll * val)
	};//旋转矩阵

double *x;
	 double min_distance = 1e10; // 初始化为一个很大的值
    double *closest_alpha = malloc(6 * sizeof(double)); // 存储最近的解
    double all_alpha[MAX_SOLUTIONS][6]; // 二维数组，每行表示一个姿态的多个解
int num_solutions; // 当前姿态的解的数量
	double weights[6] = {3.0, 3.0, 3.0, 1.0, 1.0, 1.0}; // 设置平移自由度和旋转自由度的权重
	double gama_b[6] = { 18,42,138,161,-101,-78 };
	double gama_p[6] = { 7,53,127,173,-113,-67 };
	//计算上下平台坐标
	for (int i = 0; i < 6; i++)
	{ 
		b[i][0] = Rb * cos(gama_b[i] * val);
		b[i][1] = Rb * sin(gama_b[i] * val);
		b[i][2] = 0;
		
		p[i][0] = Rp * cos(gama_p[i] * val);
		p[i][1] = Rp * sin(gama_p[i] * val);
		p[i][2] = 0;
	}

	//h0 = sqrt(s * s + a * a - (p[1][0] - b[1][0]) * (p[1][0] - b[1][0]) - (p[1][1] - b[1][1]) * (p[1][1] - b[1][1])) - p[1][2];
	h0=40;//40
	double T[3] = { x1,y,z-h0 };
	//计算p相对于基坐标系的向量q
	for (int i = 0; i < 6; i++)
	{
		q[i][0] = T[0] + Rpb[0][0] * p[i][0] + Rpb[0][1] * p[i][1] + Rpb[0][2] * p[i][2];
		q[i][1] = T[1] + Rpb[1][0] * p[i][0] + Rpb[1][1] * p[i][1] + Rpb[1][2] * p[i][2];
		q[i][2] = T[2] + Rpb[2][0] * p[i][0] + Rpb[2][1] * p[i][1] + Rpb[2][2] * p[i][2];
	}
	//计算虚拟杆长l
	
	for (int i = 0; i < 6; i++)
	{
		l[i] = sqrt((q[i][0] - b[i][0]) * (q[i][0] - b[i][0]) + (q[i][1] - b[i][1]) * (q[i][1] - b[i][1]) + (q[i][2] - b[i][2]) * (q[i][2] - b[i][2]));
		
		if (l[i]>47.84)
		{
			l[i]=47;
		}
		
		if (l[i]<29.47 )
		{
			l[i]=30;
		}
	}
	
	//计算输出角度
	for (int k = 0; k < MAX_SOLUTIONS; k++) { // 假设可以有多个解
        for (int i = 0; i < 6; i++) {
            L[i] = l[i] * l[i] - s * s + a * a;
            M[i] = 2 * a * (q[i][2] - b[i][2]);
            N[i] = 2 * a * (cos(beta[i] * val) * (q[i][0] - b[i][0]) + 
                             sin(beta[i] * val) * (q[i][1] - b[i][1]));
            // 根据逆运动学公式计算角度
            all_alpha[k][i] = (int)((asin(L[i] / sqrt((M[i] * M[i]) + (N[i] * N[i]))) - 
                                atan(N[i] / M[i])) * val1);
        }
        num_solutions++; // 增加解的数量
    }

	
 // 计算与之前解的最近解
    for (int k = 0; k < num_solutions; k++) { // 遍历每个可能的解
        double total_distance = 0;

       for (int i = 0; i < 6; i++) {
        double diff = all_alpha[k][i] - x[i]; // 计算差值
        total_distance += diff * diff; // 差值的平方
    }

    total_distance = sqrt(total_distance); // 对平方和取平方根，得到欧几里得距离

        // 判断是否更新最近解
        if (total_distance < min_distance) {
            min_distance = total_distance; // 更新最小距离
            for (int j = 0; j < 6; j++) {
                closest_alpha[j] = all_alpha[k][j]; // 更新最近解
            }
        }
    }

  
    // 可能需要返回 closest_alpha 或者其他有意义的结果
    return closest_alpha; // 确保返回最近解
		// 释放内存
    free(closest_alpha);
}
