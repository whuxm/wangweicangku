#ifndef _PID_H
#define _PID_H
#include "stm32f4xx.h"

typedef struct 
{
    float KP;
    float KI;
    float KD;
    volatile int32_t SetVal;
    volatile int32_t CurVal;
    volatile int32_t err;
    volatile int32_t err_last;
    volatile int32_t err_prv;
    volatile int32_t delta;
}PID_Type;

void PID_Init(PID_Type *pid, float kp, float ki, float kd, int32_t set);
int32_t PID_Caculate_Delta(PID_Type *pid);

#endif

