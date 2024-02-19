#ifndef _MAIN_H
#define _MAIN_H

#include "DJmotor.h"
#include "beep.h"
#include "can1.h"
#include "can2.h"
#include "includes.h"
#include "led.h"
#include "mech.h"
#include "param.h"
#include "stm32f4xx.h"
#include "tim2.h"
#include "tim3.h"

/*------------------------------------------------------*/
#define START_TASK_PRIO		5
#define R1_TASK_PRIO		15	  // R1任务
#define TESTORDER_TASK_PRIO 17	  // 调试指令
#define BEEP_TASK_PRIO		42
#define LED_TASK_PRIO		43
/*-----------end-----------------*/

//
#define SemPend(sem)                   \
	{                                  \
		sem->OSEventCnt = 0;           \
		OSSemPend(sem, 0, Error##sem); \
	}

#define TASK_STK_SIZE 1024
// *
OS_STK TESTORDER_TASK_STK[TASK_STK_SIZE];	 // 调试指令
OS_STK R1_TASK_STK[TASK_STK_SIZE];			 // 调试指令
OS_STK START_TASK_STK[TASK_STK_SIZE];		 // 开始任务堆栈
OS_STK BEEP_TASK_STK[TASK_STK_SIZE];
OS_STK LED_TASK_STK[TASK_STK_SIZE];

extern CanRxMsg RX_message;

static void Start_Task(void* pdata);
static void TestOrder_Task(void* pdata);	// 调试任务
static void Beep_Task(void* pdata);
static void Led_Task(void* pdata);
static void R1_Task(void* pdata);

/*------------------------------------------------------*/

#endif
