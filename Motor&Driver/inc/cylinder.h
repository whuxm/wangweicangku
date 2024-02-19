#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define Set_CYL8CLK() 		GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define Reset_CYL8CLK() 	GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define Set_CYLData() 		GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define Reset_CYLData() 	GPIO_ResetBits(GPIOC, GPIO_Pin_1)

typedef struct 
{
    void (*CYL_Ctrl)(u8 IO, u8 status);
} CyLinderStruct;

void CYL_Configuration(void);
void CYL_Ctrl(u8 IO,u8 status);

extern CyLinderStruct Cyl;
extern char CYL_table[8];

/*---------------------Valve------------------------*/

//#define Set_CYL8CLK() 		GPIO_SetBits(GPIOC, GPIO_Pin_0)
//#define Reset_CYL8CLK() 	GPIO_ResetBits(GPIOC, GPIO_Pin_0)
//#define Set_CYLData() 		GPIO_SetBits(GPIOC, GPIO_Pin_1)
//#define Reset_CYLData() 	GPIO_ResetBits(GPIOC, GPIO_Pin_1)

//typedef struct 
//{
//    void (*CYL_Ctrl)(u8 IO, u8 status);

//} CyLinderStruct;

//void CYL_Configuration(void);
//void CYL_Ctrl(u8 IO,u8 status);

//extern CyLinderStruct Cyl;

/* ----------------------------- Macro ----------------------------- */

//#define Set_CYLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_2)//USART2-TX
//#define Reset_CYLCLK() 	    GPIO_ResetBits(GPIOA, GPIO_Pin_2)
//#define Set_CYLDATA() 		GPIO_SetBits(GPIOA, GPIO_Pin_3)
//#define Reset_CYLDATA() 	GPIO_ResetBits(GPIOA, GPIO_Pin_3)
/* Tx-Rx reversed */
#define Set_CYLDATA() 		GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define Reset_CYLDATA()	GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define Set_CYLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define Reset_CYLCLK() 	GPIO_ResetBits(GPIOA, GPIO_Pin_3) 

//#define Set_CYLCLK() GPIO_SetBits(GPIOA, GPIO_Pin_0) // USART4-TX
//#define Reset_CYLCLK() GPIO_ResetBits(GPIOA, GPIO_Pin_0)
//#define Set_CYLDATA() GPIO_SetBits(GPIOA, GPIO_Pin_1)
//#define Reset_CYLDATA() GPIO_ResetBits(GPIOA, GPIO_Pin_1)
///* Tx-Rx reversed */
//#define Set_CYLDATA() 	GPIO_SetBits(GPIOA, GPIO_Pin_0)
//#define Reset_CYLDATA()	GPIO_ResetBits(GPIOA, GPIO_Pin_0)
//#define Set_CYLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_1)
//#define Reset_CYLCLK() 	GPIO_ResetBits(GPIOA, GPIO_Pin_1)


//#define Set_CYLDATA() 	GPIO_SetBits(GPIOA, GPIO_Pin_10)// USART1-TX
//#define Reset_CYLDATA()	GPIO_ResetBits(GPIOA, GPIO_Pin_10)
//#define Set_CYLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_9)
//#define Reset_CYLCLK() 	GPIO_ResetBits(GPIOA, GPIO_Pin_9)

//#define Set_CYLDATA() 	GPIO_SetBits(GPIOA, GPIO_Pin_9)
//#define Reset_CYLDATA()	GPIO_ResetBits(GPIOA, GPIO_Pin_9)
//#define Set_CYLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_10)
//#define Reset_CYLCLK() 	GPIO_ResetBits(GPIOA, GPIO_Pin_10)





#define Set_YTLDATA() 	GPIO_SetBits(GPIOA, GPIO_Pin_10)
#define Reset_YTLDATA()	GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define Set_YTLCLK() 		GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define Reset_YTLCLK() 	GPIO_ResetBits(GPIOA, GPIO_Pin_9)

void YTL_Configuration(void);
void YTL_Ctrl(u8 Data);

/* ----------------------------- Decleration ----------------------------- */

void Valve_Init(void);
void Valve_Ctrl(u8 data);

/* ----------------------------- End of file ----------------------------- */


#endif
