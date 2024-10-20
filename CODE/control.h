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

void servocontrol11(double *x);