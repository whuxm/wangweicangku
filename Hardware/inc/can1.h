#ifndef _CAN1_H
#define _CAN1_H

#include "beep.h"
#include "mech.h"
#include "param.h"
#include "queue.h"
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "tim3.h"
#include "ucos_ii.h"

void CAN1_Init(void);
void AnswerMasterReady(void);
void ResponseSignalToMaster(void);
// void AnswerMasterError(void);

// void AnswerMaster(CanRxMsg rx_message);

void SignalPost_CAN1(uint8_t _num);
#endif
