#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
#include "usart.h"
#include <string.h>
#include <stdlib.h>
#define BUFFER_SIZE 64 // 调整为适当的缓冲区大小

uint8_t rxBuffer[BUFFER_SIZE];
uint16_t rxIndex = 0;
uint16_t rxLength = 0;
uint8_t Serial_RxData;
uint8_t Serial_RxFlag;
uint8_t receivedData[4]; // 用于存储接收的字节数组
volatile uint8_t rxBufferIndex = 0; // 接收缓冲区索引
uint8_t Serial_TxPacket[4];				//FF 01 02 03 04 FE
uint8_t Serial_RxPacket[4];
void Serial_Init(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte(String[i]);
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y --)
	{
		Result *= X;
	}
	return Result;
}

void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
	}
}


void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		Serial_RxData = USART_ReceiveData(USART1);
		Serial_RxFlag = 1;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

//void Serial_GetArray(uint8_t *Array, uint16_t Length)
//{
//	uint16_t i;
//	uint16_t GetArray[Length];
//	for (i = 0; i < Length; i ++)
//	{
//		GetArray[i]=Serial_GetRxData();
//	}
//}
//void USART12_IRQHandler(void)
//{
//	static uint8_t RxState = 0;
//	static uint8_t pRxPacket = 0;
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		uint8_t RxData = USART_ReceiveData(USART1);
//		
//		if (RxState == 0)
//		{
//			if (RxData == 0xFF)
//			{
//				RxState = 1;
//				pRxPacket = 0;
//			}
//		}
//		else if (RxState == 1)
//		{
//			Serial_RxPacket[pRxPacket] = RxData;
//			pRxPacket ++;
//			if (pRxPacket >= 4)
//			{
//				RxState = 2;
//			}
//		}
//		else if (RxState == 2)
//		{
//			if (RxData == 0xFE)
//			{
//				RxState = 0;
//				Serial_RxFlag = 1;
//			}
//		}
//		
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}

//uint8_t Serial_GetRxData1(void)
//{
//	return *receivedData;
//}

//void USART12_IRQHandler(void)
//{
//    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//    {   
//        // 从USART1接收数据并存储到receivedData数组
//        receivedData[rxBufferIndex] = USART_ReceiveData(USART1);
//        Serial_RxFlag = 1;
//        // 增加缓冲区索引
//        rxBufferIndex++;

//        // 如果已经接收到4个字节，可以进行处理
//        if (rxBufferIndex == 4)
//        {
//            // 在这里处理接收到的数据，例如将字节数组还原为整数

//            // 处理完后，可以重置索引以准备接收下一组数据
//            rxBufferIndex = 0;
//        }
//    }
//}


//void USART1_IRQHandler(void)
//{
//    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//    {
//        uint8_t receivedByte = USART_ReceiveData(USART1);
//        
//        if (rxIndex == 0)
//        {
//            if (receivedByte == 0x55) // 包头标识
//            {
//                rxBuffer[rxIndex] = receivedByte;
//                rxIndex++;
//            }
//        }
//        else if (rxIndex == 1)
//        {
//            rxLength = receivedByte; // 数据包长度
//            rxBuffer[rxIndex] = receivedByte;
//            rxIndex++;
//        }
//        else
//        {
//            rxBuffer[rxIndex] = receivedByte;
//            rxIndex++;
//            
//            if (rxIndex == (rxLength + 2)) // 包括包头和数据
//            {
//                // 整个数据包接收完毕，可以进行处理
//                // 在这里，rxBuffer 中包含了完整的数据包
//                
//                // 处理完数据后，重置接收状态
//                rxIndex = 0;
//                rxLength = 0;
//            }
//        }

//        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//    }
//}
//void Read_Usart(char rx,float *arrays)
//{
//	static char shuzu[3][8]={0};		//接收3个数据（可更改），每个数据包含小数点和负号可容纳8位（十进制）  发送的字符(包含点.在内)长度最好<8
//	static int begin=0,x=0,y=0;		//有效数据标志，二维数组元素变量
//	int num;
//	if(rx=='[')		//起始标志
//	{
//		begin=1;
//		x=0;
//		y=0;
//	}
//	else if(rx==']')	//结束标志 在结束时字符转换为实数
//	{
//		begin=0;
//		for(num=0;num<=x;num++)
//		{
//			arrays[num]=(float)atof(shuzu[num]);//需要添加头文件<stdlib.h>
//		}
//		/* 打印数据 */
//		for(num=0;num<=x;num++)
//		{
//			printf("%d : %f\r\n",num+1,arrays[num]);
//		}
//		printf("\r\n");
//		memset(shuzu,0,sizeof(shuzu));	//清空数组，初始化为0
//	}
//	else if(rx==',')
//	{
//		x++;
//		y=0;
//	}
//	else if(begin==1)
//	{
//		shuzu[x][y]=rx;
//		y++;
//	}	
//}
