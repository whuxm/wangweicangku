#include "DJmotor.h"

DJMOTOR		 DJmotor[8];
DJMotorError Error;

// TODO:核查传动比，核查极限参数，调节pid
void DJmotorInit(void)
{
	DJMotorParam  Motor2006Param, Motor3508Param;
	DJMotorArgum  Argum;
	DJMotorLimit  Limit;
	DJMotorStatus Status;

	Motor2006Param.ReductionRatio = M2006_RATIO;
	Motor2006Param.PlusePerRound  = 8191;
	Motor2006Param.GearRatio	  = 1;		  // TODO:根据实际机构修改
	Motor2006Param.IDE			  = 0x200;	  // 对应ID为1-4
	Motor2006Param.StdId		  = CAN_Id_Standard;
	Motor2006Param.RTR			  = CAN_RTR_Data;
	Motor2006Param.DLC			  = 8;
	Motor2006Param.Current_Limit  = 4500;

	Motor3508Param.ReductionRatio = M3508_RAITO;
	Motor3508Param.PlusePerRound  = 8191;
	Motor3508Param.GearRatio	  = 1;		  // TODO:根据实际情况更改
	Motor3508Param.IDE			  = 0x1FF;	  // 对应ID为5-8
	Motor3508Param.StdId		  = CAN_Id_Standard;
	Motor3508Param.RTR			  = CAN_RTR_Data;
	Motor3508Param.DLC			  = 8;
	Motor3508Param.Current_Limit  = 10000;

	Argum.LastRxTime			  = 0;
	Argum.pluse_lock			  = 0;
	Argum.ZeroCnt				  = 0;
	Argum.PawLockCnt			  = 0;
	Argum.MaxDistance			  = 1500;
	Argum.DuZhuanCnt			  = 0;
	Argum.Counter				  = 0;

	Limit.POS_rpm_Flag			  = true;	 // 位置模式下的速度限制
	Limit.PosAngleLimitFlag		  = false;
	Limit.RPMLimitFlag			  = false;
	Limit.CurrentLimitFlag		  = true;
	Limit.RPMMaxSpeed			  = 600;
	Limit.POS_rpmLimit			  = 3000;
	Limit.MaxCurrent			  = 5000;
	Limit.ZeroSpeed				  = 500;

	// TODO:
	Limit.MaxAngle				  = 270.0;
	Limit.MinAngle				  = -270.0;

	Limit.ZeroCurrent			  = 3000;
	//    Limit.SetLockCurrent = 4500;

	Status.ArrrivedFlag			  = false;
	Status.OvertimeFlag			  = false;
	Status.ZeroFlag				  = false;
	Status.StuckFlag			  = false;
	Status.isSetZero			  = true;

	Error.RleaseWhenStuckFlag	  = false;
	Error.StuckCount			  = 0;
	Error.StuckDetectionFlag	  = true;
	Error.TimeoutCount			  = true;
	Error.TimeoutDetectionFlag	  = true;
	Error.DuanLianCnt			  = 0;

	for (int i = 0; i < 8; i++) {
		DJmotor[i].mode					= DJ_ZERO;

		DJmotor[i].enable				= false;
		DJmotor[i].begin				= false;
		DJmotor[i].Argum				= Argum;
		DJmotor[i].Limit				= Limit;
		DJmotor[i].StatusFlag			= Status;
		DJmotor[i].Error				= Error;
		DJmotor[i].ValueNow.pluse_total = 0;	// 累计脉冲要置零
		DJmotor[i].ValuePre.pluse_read	= 0;
	}

	DJmotor[1].id		 = 0x02;
	DJmotor[1].mode		 = DJ_POSITION;
	DJmotor[1].Param	 = Motor3508Param;
	DJmotor[1].Param.IDE = 0x200;

	for (int i = 2; i <= 3; i++) {
		DJmotor[i].id		 = i + 1;
		DJmotor[i].mode		 = DJ_POSITION;
		DJmotor[i].Param	 = Motor2006Param;
		DJmotor[i].Param.IDE = 0x200;
	}
	for (int i = 4; i <= 7; i++) {
		DJmotor[i].id		 = i + 1;
		DJmotor[i].mode		 = DJ_POSITION;
		DJmotor[i].Param	 = Motor2006Param;
		DJmotor[i].Param.IDE = 0x1FF;
	}

	// TODO:待搓
	for (int i = 0; i < 8; i++) {
		PID_Init(&DJmotor[i].POS_PID, 3, 0.08, 0, 0);
		PID_Init(&DJmotor[i].RPM_PID, 8, 0.3, 0, 0);
	}

	// 机构减速比
	BallPaw1.Param.GearRatio	 = 3;
	BallPaw2.Param.GearRatio	 = 3;
	SeedPaw.Param.GearRatio		 = 66 / 20.f;
	SeedStorage.Param.GearRatio	 = 80 / 20.f;
	FireSteering.Param.GearRatio = 100 / 20.f;
	FireUpDown.Param.GearRatio	 = 127 / 17.f;
}

/**
 * @brief DJ motor data return processing
 * @param  motor            My Param doc
 */
void DJ_Position_Calculate(DJMOTORpointer motor)
{
	motor->ValueNow.pluse_distance = motor->ValueNow.pluse_read - motor->ValuePre.pluse_read;	 // 计算每次转过的距离
	motor->ValuePre				   = motor->ValueNow;

	if (ABS(motor->ValueNow.pluse_distance) > 4096)	   // 由电机报文的发送频率计算可知道 > 4096时编码器越过了0位置
		motor->ValueNow.pluse_distance
			= motor->ValueNow.pluse_distance - PlusOrMinus(motor->ValueNow.pluse_distance) * motor->Param.PlusePerRound;

	motor->ValueNow.pluse_total += motor->ValueNow.pluse_distance;	  // 当前位置的累计脉冲记录位置
	motor->ValueNow.angle
		= motor->ValueNow.pluse_total * 360 / (motor->Param.PlusePerRound * motor->Param.ReductionRatio * motor->Param.GearRatio);

	if (motor->begin)
		motor->Argum.pluse_lock = motor->ValueNow.pluse_total;

	// 归零函数  如果不写的话每次出了最开始的那一次复位，其余复位都不是零点\
   // 这里不加的话 每次复位 电调会记录-10度到 10度 之间的一个角度值 不知道为啥
	if (motor->StatusFlag.isSetZero) {
		DJ_SetZero(motor);
	}
}

/**
 * @brief speed mode of DJ motor
 * @param  motor            My Param doc
 */
void DJ_SpeedMode(DJMOTORpointer motor)
{
	motor->RPM_PID.SetVal	 = motor->ValueSet.speed;
	motor->RPM_PID.CurVal	 = motor->ValueNow.speed;
	motor->ValueSet.current += PID_Caculate_Delta(&motor->RPM_PID);	   // 目标是改变ValueSet.current
}

/**
 * @brief Locking the current absolute position of the motor
 * @param  motor            My Param doc
 */
void DJ_LockPosition(DJMOTORpointer motor)
{
	motor->POS_PID.SetVal	 = motor->Argum.pluse_lock;	   // 累计脉冲
	motor->POS_PID.CurVal	 = motor->ValueNow.pluse_total;
	motor->RPM_PID.SetVal	 = PID_Caculate_Delta(&motor->POS_PID);	   // 位置环的pid输出为速度环的输入
	motor->RPM_PID.CurVal	 = motor->ValueNow.speed;
	motor->ValueSet.current += PID_Caculate_Delta(&motor->RPM_PID);	   // 速度环的pid输出为电流环的输入
	PEAK(motor->ValueSet.current, 3000);							   // 自锁电流限制
}

/**
 * @brief Three loop control of motor position mode
 * @param  motor            My Param doc
 */
void DJ_PositionMode(DJMOTORpointer motor)
{
	motor->ValueSet.pluse_total = motor->ValueSet.angle * motor->Param.PlusePerRound * motor->Param.ReductionRatio
								  * motor->Param.GearRatio * 0.0027777f;	// 将目标角度转换为设定脉冲数值 0.0027777=1/360

	/*---------------------位置模式的最大转角限制-------------------*/
	if (motor->Limit.PosAngleLimitFlag == true) {
		int32_t max_pluse_temp
			= motor->Limit.MaxAngle * motor->Param.PlusePerRound * motor->Param.ReductionRatio * motor->Param.GearRatio / 360;
		int32_t min_pluse_temp
			= motor->Limit.MinAngle * motor->Param.PlusePerRound * motor->Param.ReductionRatio * motor->Param.GearRatio / 360;
		motor->POS_PID.SetVal = (motor->ValueSet.pluse_total > max_pluse_temp)
									? max_pluse_temp
									: (motor->ValueSet.pluse_total > min_pluse_temp ? motor->ValueSet.pluse_total : min_pluse_temp);
	} else
		motor->POS_PID.SetVal = motor->ValueSet.pluse_total;

	motor->POS_PID.CurVal = motor->ValueNow.pluse_total;	// 这里小心别搞成 .pluse_read 否则完蛋。

	motor->RPM_PID.SetVal = PID_Caculate_Delta(&motor->POS_PID);	// 速度环的输入时位置环的pid输出
	if (motor->Limit.POS_rpm_Flag)
		PEAK(motor->RPM_PID.SetVal, motor->Limit.POS_rpmLimit);	   // 对三环pid中间计算量进行限制
	motor->RPM_PID.CurVal = motor->ValueNow.speed;

	motor->ValueSet.current += PID_Caculate_Delta(&motor->RPM_PID);	   // 电流环输入是速度环的输出  据说电调内部有电流环

	// if (motor->ValueSet.pluse_total - motor->ValueNow.pluse_total < 60)
	//     motor->StatusFlag.ArrrivedFlag = true;
	// else
	//     motor->StatusFlag.ArrrivedFlag = false;
}

// 将当前位置设置为零点
void DJ_SetZero(DJMOTORpointer motor)
{
	motor->StatusFlag.isSetZero = false;
	motor->ValueNow.angle		= 0;
	motor->ValueNow.pluse_total = 0;
	motor->Argum.pluse_lock		= 0;
}

// 寻零模式：本质上是一个速度模式，有个外力把它卡住。记录卡的位置为 0点。
void DJ_ZeroMode(DJMOTORpointer motor)
{
	motor->RPM_PID.SetVal	 = motor->Limit.ZeroSpeed;	  // 以寻零最大速度寻零。（直接叫寻零速度得了）
	motor->RPM_PID.CurVal	 = motor->ValueNow.speed;
	motor->ValueSet.current += PID_Caculate_Delta(&motor->RPM_PID);

	PEAK(motor->ValueSet.current, motor->Limit.ZeroCurrent)	   // 寻零电流不要过大

	if (ABS(motor->ValueNow.pluse_distance) < ZERO_DISTANCE_THRESHOLD) {	// 累计小于门槛后，视为寻零成功
		if (motor->Argum.ZeroCnt++ > 100) {
			motor->Argum.ZeroCnt		   = 0;

			motor->StatusFlag.ArrrivedFlag = false;
			motor->StatusFlag.ZeroFlag	   = true;
			motor->begin				   = false;
			motor->ValueSet.angle		   = 0;	   // 防止 切换到位置模式后继续运动

			DJ_SetZero(motor);	  // 当前位置再设置为零点
		}
	} else
		motor->Argum.ZeroCnt = 0;	 // 防止 积累偶然成功
}

/**
 * @brief send control current to driver 向电机发送can消息。反馈与接收 都是 1KHz
 * @param  motor            My Param doc
 */
void DJ_CurretnTransmit(DJMOTORpointer motor, uint8_t i)	// i为大疆 的id（ 1-8 ）
{
	static CanTxMsg tx_message;
	PEAK(motor->ValueSet.current, motor->Param.Current_Limit);

	if (motor->enable != ENABLE)	// 小心这里的逻辑 这里为了防止非0即真的误触发，故限定了仅当ENABLE时才 发送正常电流，否则都置零
		motor->ValueSet.current = 0;

	if (i <= 4)
		tx_message.StdId = 0x200;
	else
		tx_message.StdId = 0x1FF;

	tx_message.IDE = CAN_Id_Standard;
	tx_message.RTR = CAN_RTR_Data;
	tx_message.DLC = 8;

	uint8_t t;

	if (motor->id <= 4)	   // 严格按照格式来
		t = ((motor->id - 1) * 2);
	else
		t = ((motor->id - 5) * 2);

	EncodeS16Data(&motor->ValueSet.current, &tx_message.Data[t]);
	ChangeDataByte(&tx_message.Data[t], &tx_message.Data[t + 1]);

	if ((motor->id == 4) || (motor->id == 8))
		CAN_Transmit(CAN2, &tx_message);
}

// 监控电机状态
void Monitoring(DJMOTORpointer motor)
{
	if (motor->enable && motor->begin) {
		//
		if (motor->ValueNow.pluse_distance < 5 && motor->ValueNow.current > 3000) {
			motor->Error.StuckCount++;
			if (motor->Error.StuckCount > 500)	  // 阈值0.5s
			{
				motor->Error.StuckCount		= 0;
				motor->StatusFlag.StuckFlag = true;
				Led.Stuck					= true;
				if (motor->Limit.isLooseStuck) {
					motor->enable = false;
					Beep.error++;
				}
			}
		} else {
			motor->Error.StuckCount		= 0;
			motor->StatusFlag.StuckFlag = false;
			Led.Stuck					= false;
		}

		if (motor->Argum.LastRxTime++ > 20) {
			if (motor->Error.TimeoutCount++ > 10) {
				motor->Error.TimeoutCount	   = 0;
				motor->StatusFlag.OvertimeFlag = true;
				Led.Timeout					   = true;
				Beep.error++;
			}
		}
	}
}

// 接收CAN2数据处理。1KHz。
void DJ_ReceiveData_CAN2(CanRxMsg Rx_message0)	  // 后缀 0 表示从 FIFO0 接收
{
	if (Rx_message0.StdId >= 0x201 && Rx_message0.StdId <= 0x208) {
		// DJMOTORpointer motor;
		uint8_t ID = Rx_message0.StdId - 0x200;

		DJmotor[ID - 1].ValueNow.pluse_read = (Rx_message0.Data[0] << 8) | Rx_message0.Data[1];	   // 小端存储 高位在前低位在后
		DJmotor[ID - 1].ValueNow.speed	 = (Rx_message0.Data[2] << 8) | Rx_message0.Data[3];
		DJmotor[ID - 1].ValueNow.current = (Rx_message0.Data[4] << 8) | Rx_message0.Data[5];

		if (DJmotor[ID - 1].Param.ReductionRatio == M3508_RAITO)	// M3508
		{
			DJmotor[ID - 1].ValueNow.real_speed	 = (float) (DJmotor[ID - 1].ValueNow.speed * 1 / M3508_RAITO);	  // 输出轴速度
			DJmotor[ID - 1].ValueNow.temperature = Rx_message0.Data[6];
			// DJmotor[k].ValueNow.current_A
			// 	= (float) (DJmotor[k].ValueNow.current * 0.0012207);	// 实际上是current / 16384 *20
		} else	  // M2006
		{
			DJmotor[ID - 1].ValueNow.real_speed = (float) (DJmotor[ID - 1].ValueNow.speed * 1 / M3508_RAITO);	 // 输出轴速度
			DJmotor[ID - 1].ValueNow.current_A	= (float) (DJmotor[ID - 1].ValueNow.current / 10000 * 10);
		}

		DJ_Position_Calculate(&DJmotor[ID - 1]);

		DJmotor[ID - 1].Error.DuanLianCnt = 0;
	}
}

void DJ_Func(void)
{
	for (int i = 0; i < 8; i++) {
		if (DJmotor[i].enable) {
			if (DJmotor[i].begin) {
				switch (DJmotor[i].mode) {
					case DJ_RPM:
						DJ_SpeedMode(&DJmotor[i]);
						break;
					case DJ_POSITION:
						DJ_PositionMode(&DJmotor[i]);
						break;
					case DJ_CURRENT:
						DJ_CurretnTransmit(&DJmotor[i], (i + 1));
						break;
					case DJ_ZERO:
						DJ_ZeroMode(&DJmotor[i]);
						break;
					default:
						break;
				}
			} else
				DJ_LockPosition(&DJmotor[i]);	 // 使能 但不开始就锁定
		}
		DJ_CurretnTransmit(&DJmotor[i], (i + 1));	 // 给电机发送报文

		// 检测用到的电机是否断联
		if (i + 1 == SeedPawId || i + 1 == SeedStorageId || i + 1 == BallPaw1Id || i + 1 == BallPaw2Id || i + 1 == FireSteeringId
			|| i + 1 == FireUpDownId)
			// 检测是否断联。此值在DJ_ReceiveData_CAN2 中不断被置零。如果长时间没有置零，说明该电机断联
			if (DJmotor[i].Error.DuanLianCnt++ > 100)
				AnswerError(IdTransmiter(DJmotor[i].id), DuanLian);
	}
}
