#include "main.h"

// �����ź���

// OS_EVENT* XXXSem;
// INT8U*	  ErrorXXXSem;

extern CanRxMsg			RX_message;	   // Can1Funtion() �� R1_Task() ���õı���
extern volatile uint8_t ChooseFlag;
extern int				Cnt6;
extern uint8_t			SeedPaw1Cylinder;	 // ��ȡ�����׵�λ�������ڼ�λ��
extern uint8_t			SeedPaw2Cylinder;
extern int				A1;
extern int				A2;
extern int				kai;

// DJ����263���׶�0.   Z:����λ�ã� 783.795898
// DJ ���� 160

int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	CAN1_Init();
	CAN2_Init();
	Tim3_Init();
	Tim2_Init();

	CAN_InitSendQueue();
	param_Init();

	BEEP_Init();
	LED_Init();
	DJmotorInit();
	Zdrive_Init();

	Valve_Init();
	MechInit();	   // ��Ҫ�޸ģ��������棩

	OSInit();
	OSTaskCreate(Start_Task, (void*) 0, (OS_STK*) &START_TASK_STK[TASK_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();
}

static void Start_Task(void* pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata			 = pdata;

	OS_CPU_SysTickInit();	 // ����OSʱ��
	OS_ENTER_CRITICAL();	 // �����ٽ���

	// ��������
	OSTaskCreate(TestOrder_Task, (void*) 0, (OS_STK*) &TESTORDER_TASK_STK[TASK_STK_SIZE - 1], TESTORDER_TASK_PRIO);	   // ���� 17

	OSTaskCreate(R1_Task, (void*) 0, (OS_STK*) &R1_TASK_STK[TASK_STK_SIZE - 1], R1_TASK_PRIO);			// ���� 15
	OSTaskCreate(Beep_Task, (void*) 0, (OS_STK*) &BEEP_TASK_STK[TASK_STK_SIZE - 1], BEEP_TASK_PRIO);	// 42
	OSTaskCreate(Led_Task, (void*) 0, (OS_STK*) &LED_TASK_STK[TASK_STK_SIZE - 1], LED_TASK_PRIO);		// 43

	OSTaskSuspend(START_TASK_PRIO);	   // ������ʼ����

	OS_EXIT_CRITICAL();	   // �˳��ٽ���
	OSTimeDly(100);
}

// 17
static void TestOrder_Task(void* pdata)
{
	pdata = pdata;
	while (1) { OSTimeDly(100); }
}

// 15
static void R1_Task(void* pdata)
{
	pdata = pdata;

	// ���ﺬ��ȫ�ֱ������ܣ���һ�β���ִ�����֮ǰ������ִ����һ�β�������ɱ��ָ��
	while (1) {
		switch (ChooseFlag) {
			case 1:
				MechEnable();	 // ʹ��
				ChooseFlag = 0xff;
				break;
			case 2:
				FindZero();	   // Ѱ��
				ChooseFlag = 0xff;
				break;

			case 3:
				Seeding();
				ChooseFlag = 0xff;
				break;

			default:
				break;
		}
		OSTimeDly(100);

		if (kai)
			Valve_Ctrl(CylinderCalculate(A1, A2));
		else {
		}

		if (kai == 6)
			Valve_Ctrl(255);
		if (kai == 7)
			Valve_Ctrl(0);
		if (kai == 8)
			Valve_Ctrl(1 << 7);
		if (kai == 9)
			Valve_Ctrl(1 << 6);
		if (kai == 10)
			Valve_Ctrl(1 << 5);

		// ����Ϊ���Դ��룬���ڲ���GitHubԶ��Эͬ����
		if (kai == 11) {
			Valve_Ctrl(255);
			OSTimeDly(100);
			while (kai == 11) {
				kai++;
				OSTimeDly(100);
			}
		}
	}
}

// 42
static void Beep_Task(void* pdata)
{
	pdata = pdata;
	for (;;) {
		beeptask();
		OSTimeDly(1000);
	}
}

// 43
static void Led_Task(void* pdata)
{
	pdata = pdata;
	for (;;) {
		ledtask();
		OSTimeDly(5000);
	}
}
