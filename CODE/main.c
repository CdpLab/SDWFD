#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "key.h"
#include "limits.h"
#include "queue.h"
#include "serial.h"
#include "inverse.h"
#include "pca9685.h"
#include "adc.h"
#include "event_groups.h"
#include "semphr.h"
#include "limits.h"

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define Inverse_task_PRIO		3
//任务堆栈大小	
#define Inverse_STK_SIZE 		128  
//任务句柄
TaskHandle_t Inversetask_Handler;
//任务函数
void Inverse_task(void *pvParameters);

//任务优先级
#define Controll_task_PRIO		4
//任务堆栈大小	
#define Controll_STK_SIZE 		50  
//任务句柄
TaskHandle_t Controlltask_Handler;
//任务函数
void Controll_task(void *pvParameters);

//任务优先级
#define Bluetooth_TASK_PRIO		2
//任务堆栈大小	
#define Bluetooth_STK_SIZE 		50  
//任务句柄
TaskHandle_t BluetoothTask_Handler;
//任务函数
void Bluetooth_task(void *pvParameters);

//任务优先级
#define FSR_TASK_PRIO		5
//任务堆栈大小	
#define FSR_STK_SIZE 		128  
//任务句柄
TaskHandle_t FSRTask_Handler;
//任务函数
void FSR_task(void *pvParameters);

#define  QUEUE_LEN    6   /* 队列的长度，最大可包含多少个消息 */
#define  QUEUE_SIZE   10   /* 队列中每个消息大小（字节） */
#define  Angel_LEN    6   /* 队列的长度，最大可包含多少个消息 */
#define  Angel_SIZE   10   /* 队列中每个消息大小（字节） */
#define  USE_CHAR  1  /* 测试字符串的时候配置为 1 ，测试变量配置为 0  */
#define PRESS_MAX	6000
#define PRESS_MIN	20
#define VOLTAGE_MIN 100
#define VOLTAGE_MAX 3300
#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)//设置事件掩码的位1

QueueHandle_t Bluetooth_Queue =NULL;
QueueHandle_t Angel_Queue =NULL;
EventGroupHandle_t Event_Handle =NULL;
SemaphoreHandle_t MuxSem_Handle =NULL;
u16 adcBuffer[10];  
int main()
{
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	LED_Init();
	KEY_Init();
	Servo_Init(50,60);
	USART1_Init(9600);
	Serial_Init();
   Adc_Init();
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //进入临界区
  /* 创建 Event_Handle */
	Event_Handle = xEventGroupCreate();
	/* 创建MuxSem */
	MuxSem_Handle = xSemaphoreCreateMutex();		
					
	xTaskCreate((TaskFunction_t )Bluetooth_task,     
                (const char*    )"Bluetooth_task",   
                (uint16_t       )Bluetooth_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Bluetooth_TASK_PRIO,
                (TaskHandle_t*  )&BluetoothTask_Handler);					

		Bluetooth_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* 消息队列的长度 */
                            (UBaseType_t ) QUEUE_SIZE);/* 消息的大小 */				
	
		Angel_Queue = xQueueCreate((UBaseType_t ) Angel_LEN,/* 消息队列的长度 */
                            (UBaseType_t ) Angel_SIZE);/* 消息的大小 */														
	/	
	xTaskCreate((TaskFunction_t )Inverse_task,     
                (const char*    )"Inverse_task",   
                (uint16_t       )Inverse_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Inverse_task_PRIO,
                (TaskHandle_t*  )&Inversetask_Handler);					
		
	xTaskCreate((TaskFunction_t )Controll_task,     
                (const char*    )"Controll_task",   
                (uint16_t       )Controll_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Controll_task_PRIO,
                (TaskHandle_t*  )&Controlltask_Handler);			
		
	xTaskCreate((TaskFunction_t )FSR_task,     
                (const char*    )"FSR_task",   
                (uint16_t       )FSR_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )FSR_TASK_PRIO,
                (TaskHandle_t*  )&FSRTask_Handler);							
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
} 

void Bluetooth_task(void *pvParameters) {
    uint16_t t1;
    double xx[6];
    uint16_t t2 = 0;
 BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    while (1) {
        if (Serial_GetRxFlag() == 1) {
            t1 = Serial_GetRxData();
            xx[t2] = (double)t1;
            t2++;
            if (t2 == 6) { 
							xReturn = xSemaphoreTake(MuxSem_Handle,/* 互斥量句柄 */
                              portMAX_DELAY); /* 等待时间 */
							xQueueSend(Bluetooth_Queue, xx, 0);
                t2 = 0; 
        xEventGroupSetBits(Event_Handle,KEY1_EVENT); 
		    xReturn = xSemaphoreGive( MuxSem_Handle );//给出互斥量					
            }
        }
				vTaskDelay(20);
    }	
}

void Inverse_task(void *pvParameters) {
  BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdTRUE */
	double rxData[6];	/* 定义一个接收消息的变量 */
	double *result;
	EventBits_t r_event;  /* 定义一个事件接收变量 */
	while(1)
    {
			  r_event = xEventGroupWaitBits(Event_Handle,  /* 事件对象句柄 */
                                  KEY1_EVENT|KEY2_EVENT,/* 接收线程感兴趣的事件 */
                                  pdTRUE,   /* 退出时清除事件位 */
                                  pdFALSE,   /* 满足感兴趣的所有事件 */
                                  portMAX_DELAY);/* 指定超时事件,一直等 */
			
     xReturn = xQueueReceive( Bluetooth_Queue,    /* 消息队列的句柄 */
								rxData,      /* 发送的消息内容 */
								portMAX_DELAY); /* 等待时间 一直等 */
		if(pdTRUE == xReturn){
			result = inv(rxData[0], rxData[1], rxData[2], rxData[3], rxData[4], rxData[5]);
//		  xQueueSend(Angel_Queue, result, 0);
			xReturn = xTaskNotify(Controll_task, /*任务句柄*/
								(uint32_t)&result, /* 发送的数据，最大为4字节 */
								eSetValueWithOverwrite );/*覆盖当前通知*/
		}
		else
			printf("数据接收出错,错误代码0x%lx\n",xReturn);
    }
}

void Controll_task(void *pvParameters) {
    BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdTRUE */
	double *alpha1;
    while (1) {
			xReturn=xTaskNotifyWait(0x0,			//进入函数的时候不清除任务bit
								ULONG_MAX,	  //退出函数的时候清除所有的bit
								(uint32_t *)&alpha1,		  //保存任务通知值                       
								portMAX_DELAY);	//阻塞时间
			
//                xReturn = xQueueReceive( Angel_Queue,    /* 消息队列的句柄 */
//								alpha1,      /* 发送的消息内容 */
//								portMAX_DELAY); /* 等待时间 一直等 */
		if(pdTRUE == xReturn){
			Servo_angle(10,60-(alpha1[0]));//-
		Servo_angle(11,60+alpha1[1]);//+
		Servo_angle(12,60-(alpha1[2]));
		Servo_angle(13,60+(alpha1[3]));
		Servo_angle(14,60-(alpha1[4])); 
		Servo_angle(15,60+(alpha1[5]));
		}
		else
		{printf("数据接收出错,错误代码0x%lx\n",xReturn);}
			vTaskDelay(20);
            }
        }
				
void FSR_task(void *pvParameters) {
uint8_t state = 0;
uint16_t val = 0;
uint16_t value_AD = 0;
long PRESS_AO = 0;
uint16_t VOLTAGE_AO = 0;
BaseType_t xReturn = pdTRUE;/* 定义一个创建信息返回值，默认为pdTRUE */
	double zero[6]={0};
    while (1) {     
            value_AD = average(adcBuffer,10);	
		VOLTAGE_AO = map(value_AD, 0, 4095, 0, 3300);
		if(VOLTAGE_AO < VOLTAGE_MIN)
		{
			PRESS_AO = 0;
		}
		else if(VOLTAGE_AO > VOLTAGE_MAX)
		{
			PRESS_AO = PRESS_MAX;
		}
		else
		{
			PRESS_AO = map(VOLTAGE_AO, VOLTAGE_MIN, VOLTAGE_MAX, PRESS_MIN, PRESS_MAX);
			if (PRESS_AO>2500)
			{  xReturn = xSemaphoreTake(MuxSem_Handle,/* 互斥量句柄 */
                              portMAX_DELAY); /* 等待时间 */
				 xQueueSend(Bluetooth_Queue, zero, 0);
				 xEventGroupSetBits(Event_Handle,KEY2_EVENT);
				 xReturn = xSemaphoreGive( MuxSem_Handle);
			}
		}   
      vTaskDelay(20);		
		}
            }
        

