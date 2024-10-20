#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <stdarg.h>
#include "math.h"
#include <stdio.h>
void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);
void Serial_GetArray(uint8_t *Array, uint16_t Length);
uint8_t Serial_GetRxFlag(void);
uint8_t Serial_GetRxData(void);
uint8_t Serial_GetRxData1(void);

#endif