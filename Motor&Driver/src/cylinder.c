#include "cylinder.h"

#include "delay.h"

// imitate I2C

CyLinderStruct Cyl;	   //{}}
char		   CYL_table[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
//									  Lh             Paw  Ld        Tg    Rh
// pc0 sclk ; pc1 io
void		   CYL_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

	Set_CYL8CLK();		// GPIO_SetBits(GPIOC, GPIO_Pin_0)
	Reset_CYL8CLK();	// GPIO_ResetBits(GPIOC, GPIO_Pin_0)
}

void CYL_Ctrl(u8 IO, u8 status)	   // 怎么单独修改一个byte中的一位
{
	IO = 8 - IO;				  // 高位是0，低位是8，反一下顺序
	for (u8 i = 0; i < 8; i++)	  // 因为是8位，所以是循环8次
	{
		Reset_CYL8CLK();	// switch analog clock

		if (i == IO)	// 改变当前位
		{
			CYL_table[i] = status;
		}

		// Send signal based on current settings
		if (CYL_table[i] & 0x01)
			Set_CYLData();	  // GPIO_SetBits(GPIOC, GPIO_Pin_1)
		else
			Reset_CYLData();	// GPIO_ResetBits(GPIOC, GPIO_Pin_1)

		Set_CYL8CLK();
	}
}

void Valve_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Set_CYLCLK();		// 1
	Reset_CYLDATA();	// 0，都开灯
}

/**
 * @brief: Valve control by bits
 */
void Valve_Ctrl(u8 Data)
{
	u8 i;
	if (Data > 0xFF)
		Data = 0XFF;
	for (i = 0; i < 8; i++) {
		Reset_CYLCLK();
		if (Data & 0x01)		// 如果数据最后一位为1
			Set_CYLDATA();		// 1
		else					// 如果为0
			Reset_CYLDATA();	// 0
		Set_CYLCLK();			// 打开时钟，0
		Data >>= 1;				// 右移一位
	}
	Set_CYLDATA();
}

char YTL_table[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void YTL_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin	  = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Set_YTLCLK();	   // PA9 = 1
	Reset_YTLCLK();	   // PA9 = 0
}

// ytl board;
void YTL_Ctrl(u8 Data)	  // 怎么单独修改一个byte中的一位
{
	////// 	u8 i;
	//////	if (Data > 0xFF)
	//////		Data = 0XFF;
	//////	for (i = 0; i < 8; i++) {
	//////			Reset_YTLCLK();
	//////		if (Data & 0x01)		// 如果数据最后一位为1
	//////			Set_YTLDATA();		// 1
	//////		else					// 如果为0
	//////		Reset_YTLDATA();	// 0
	//////		Set_YTLCLK();		// 打开时钟，0
	//////		Data >>= 1;				// 右移一位
	//////	}
	//////Set_YTLDATA();
	//////
	//////
	//	IO = 8 - IO;		//高位是0，低位是8，反一下顺序
	//	for(u8 i=0;i<8;i++)		//因为是8位，所以是循环8次
	//	{
	//		Reset_YTLCLK();  // switch analog clock
	//
	//		if(i==IO)		//改变当前位
	//		{
	//			YTL_table[i] = status;
	//		}

	//		// Send signal based on current settings
	//		if(YTL_table[i])
	//			Set_YTLDATA();
	//		else
	//			Reset_YTLDATA();
	//
	//		Set_YTLCLK();
	//	}

	Reset_YTLCLK();	   // PA9 = 0

	for (u8 i = 0; i < 8; i++) {
		if ((Data >> i) & 0x01) {
			Set_YTLDATA();	  // 1
		} else {
			Reset_YTLDATA();	// 0
		}

		Delay_us(2);
		Set_YTLCLK();	 // 打开时钟，0

		Delay_us(2);
		Reset_YTLCLK();
	}
}
