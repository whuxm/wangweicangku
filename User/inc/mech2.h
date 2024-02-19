#ifndef _NEW_MECH2_H
#define _NEW_MECH2_H

#include "stm32f4xx_can.h"

// 错误类型
#define DuanLian 1
#define DuZhuan	 2

// 气缸开关。气缸的编号定义在了电机结构体中
#define Open  1
#define Close 0

// 机构电机Id(上位机Id) 1-8
#define SeedPawId	   2
#define SeedStorageId  3
#define BallPaw1Id	   4	// 一号夹球爪子
#define BallPaw2Id	   5	// 二号夹球爪子
#define FireSteeringId 6	// 踢射回转
#define FireUpDownId   7	// 俯仰角
#define FireArmId	   1	// 踢射臂

// 电机
#define SeedPaw		 DJmotor[SeedPawId - 1]			// 3508取苗
#define SeedStorage	 DJmotor[SeedStorageId - 1]		// 2006存苗
#define BallPaw1	 DJmotor[BallPaw1Id - 1]		// 2006一号夹球爪子  left
#define BallPaw2	 DJmotor[BallPaw2Id - 1]		// 2006二号夹球爪子  right
#define FireSteering DJmotor[FireSteeringId - 1]	// 2006踢射回转
#define FireUpDown	 DJmotor[FireUpDownId - 1]		// 2006俯仰角
#define FireArm		 Zdrive[FireArmId - 1]			// U10 踢射臂

// 错误电机编号
#define FireUpDown_	  1	   // 俯仰角
#define FireSteering_ 2	   // 踢射回转
#define FireArm_	  3	   // 踢射臂
#define BallPaw1_	  4	   // 一号夹球爪子 左
#define BallPaw2_	  5	   // 二号夹球爪子 右
#define SeedPaw_	  6	   // 3508取苗
#define SeedStorage_  7	   // 2006存苗

void AnswerError(int WhichError, int ErrorType);
int	 IdTransmiter(uint8_t id);

#endif
