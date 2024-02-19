#ifndef _NEW_MECH_H
#define _NEW_MECH_H

#include "DJmotor.h"
#include "Zdrive.h"
#include "beep.h"
#include "cylinder.h"
#include "mathFunc.h"
#include "mech2.h"
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

// 函数
void MechInit(void);	// 机构初始化

void Can1Funtion(CanRxMsg rx_message);	  // can1中断处理函数
void MechEnable(void);					  // 使能or 失能
void FindZero(void);					  // 寻零
void Seeding(void);

uint8_t CylinderCalculate(int Paw_Id, int command);

void DJ_Check_place(DJMOTORpointer DJ);
void Zdrive_Check_place(ZdriveTypedef* Zdri);

#endif
