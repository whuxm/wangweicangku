#include "pid.h"

void PID_Init(PID_Type *pid, float kp, float ki, float kd, int32_t set)
{
    pid->KP = kp;
    pid->KI = ki;
    pid->KD = kd;
    pid->SetVal = set;
    pid->CurVal = 0;
    pid->err = 0;
    pid->err_last = 0;
    pid->err_prv = 0;
    pid->delta = 0;
}

/**
 * @brief 增量式pid计算每次得到的增量结果
 * @param  PID_Type  *pid
 * @return int16_t pid->delta
 */
int32_t PID_Caculate_Delta(PID_Type *pid)
{
    pid->err = pid->SetVal - pid->CurVal;

    pid->delta = pid->KP * (pid->err - pid->err_last) + pid->KI * pid->err + pid->KD * (pid->err + pid->err_prv - 2 * pid->err_last);

    pid->err_prv = pid->err_last;
    pid->err_last = pid->err;
    return pid->delta;
}

//int32_t err1[3] = {0}, err2[3] = {0}, error1_2 = 0;
///**
// * @brief 交叉耦合控制同步
// * @param  motor1       控制电机
// * @param  motor2       控制电机
// * @param  k1    同步差增益
// * @param  k2    同步差增益
// * @return int32_t*
// */
//int32_t *JCOHcontrpl(ZDrive motor1, DJMOTORpointer motor2, float k1, float k2)
//{
//    static int32_t delta[2];
//    error1_2 = motor1->ValueNow.speed - motor2->ValueNow.speed;

//    err1[0] = motor1->ValueSet.speed - motor1->ValueNow.speed - k1 * error1_2;
//    err2[0] = motor1->ValueSet.speed - motor2->ValueNow.speed + k2 * error1_2;

//    delta[0] = motor1->RPM_PID.KP * (err1[0] - err1[1]) + motor1->RPM_PID.KI * err1[0] + motor1->RPM_PID.KD * (err1[0] + err1[2] - 2 * err1[1]);

//    delta[1] = motor2->RPM_PID.KP * (err2[0] - err2[1]) + motor2->RPM_PID.KI * err2[0] + motor2->RPM_PID.KD * (err2[0] + err2[2] - 2 * err2[1]);

//    err1[2] = err1[1];
//    err1[1] = err1[0];
//    err2[2] = err2[1];
//    err2[1] = err2[0];
//    return delta;
//}

