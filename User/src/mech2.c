
#include "mech2.h"

void AnswerError(int WhichError, int ErrorType)
{
	CanTxMsg message;

	message.IDE		= CAN_Id_Extended;
	message.ExtId	= 0x050201EE;
	message.DLC		= 2;
	message.RTR		= CAN_RTR_Data;
	message.Data[0] = WhichError;
	message.Data[1] = ErrorType;

	CAN_Transmit(CAN1, &message);
}

// 将电机ID转化为 回复主控的错误编号（直接设置ID = 错误编号则其他地方需要改）
int IdTransmiter(uint8_t id)
{
	switch (id) {
		case SeedPawId:
			return SeedPaw_;

		case SeedStorageId:
			return SeedStorage_;

		case BallPaw1Id:
			return BallPaw1_;

		case BallPaw2Id:
			return BallPaw2_;

		case FireSteeringId:
			return FireSteering_;

		case FireUpDownId:
			return FireUpDown_;

		case FireArmId:
			return FireArm_;
	}
	return 0;
}
