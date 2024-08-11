#include "uc-os2_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/RELAY/relay.h"
#include "./BSP/OLED/oled.h" 
#include "./BSP/OLED/oledchinesefont.h"
#include "./BSP/Speed/speed.h"
#include "./BSP/STEPMOTER/littlemoter.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/START/start.h"
#include "./BSP/DISTANCE/distance.h"
#include "./BSP/DMA/dma.h"
/*uC/OS-II*********************************************************************************************/
#include "os.h"
#include "cpu.h"


//���������������������ʽ��ɣ�������ΪRTOS����
//�ǲ���֮ǰrtosϵͳ����̫���ˣ��Żᵼ�»�е���˶�������ȶ�������кܴ�Ĳ��
extern uint16_t pulse;
extern uint16_t pulse2;
extern uint16_t pulse3;
extern bool dma_tx_complete;
/******************************************************************************************/
/* UCOSII�������� */

/* START ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define BEGIN_TASK_PRIO                 5     /* ��ʼ����������ȼ�����Ϊ5*/
#define BEGIN_STK_SIZE                  128     /* ��ջ��С */

OS_STK BEGIN_TASK_STK[BEGIN_STK_SIZE];          /* �����ջ */
void begin_task(void *pdata);                   /* ������ */

#define START_TASK_PRIO                 10      /* ��ʼ��������ȼ�����Ϊ10 */
#define START_STK_SIZE                  128     /* ��ջ��С */

OS_STK START_TASK_STK[START_STK_SIZE];          /* �����ջ */
void start_task(void *pdata);                   /* ������ */

#define RELAY_TASK_PRIO                 8      /* ���ʹ���������ȼ�����Ϊ8 */
#define RELAY_STK_SIZE                  128     /* ��ջ��С */

OS_STK RELAY_TASK_STK[RELAY_STK_SIZE];          /* �����ջ */
void relay_task(void *pdata);                   /* ������ */

#define DIS_TASK_PRIO                 2      /* �������������ȼ�����Ϊ��� */
#define DIS_STK_SIZE                  128     /* ��ջ��С */

OS_STK DIS_TASK_STK[DIS_STK_SIZE];            /* �����ջ */
void dis_task(void *pdata);                   /* ������ */

/* �� ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define MAIN_TASK_PRIO                  4       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define MAIN_STK_SIZE                   512     /* ��ջ��С */

OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];            /* �����ջ */
void main_task(void *pdata);                    /* ������ */

OS_EVENT *msg_key;                              /* ���������¼���ָ�� */
OS_EVENT *sem_beep;                             /* �������ź���ָ�� */
void ucos_load_main_ui(void);
//�Ȱ������ȼ����ߵ��Ǵ����������Ե�һʱ�䷴��λ��

/******************************************************************************************************/
/**
 * @brief       ��ں���
 * @param       ��
 * @retval      ��
 */
void uc_os2_demo(void)
{
		ucos_load_main_ui(); 
    OSInit();                                                               /* UCOS��ʼ�� */
    OSTaskCreateExt((void(*)(void *) )start_task,                           /* ������ */
                    (void *          )0,                                    /* ���ݸ��������Ĳ��� */
                    (OS_STK *        )&START_TASK_STK[START_STK_SIZE - 1],
                    (INT8U           )START_TASK_PRIO,                      /* �������ȼ� */
                    (INT16U          )START_TASK_PRIO,                      /* ����ID */
                    (OS_STK *        )&START_TASK_STK[0],                   /* �����ջջ�� */
                    (INT32U          )START_STK_SIZE,                       /* �����ջ��С */
                    (void *          )0,                                    /* �û�����Ĵ洢�� */
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP); /* ����ѡ��,Ϊ�˱���������������񶼱��渡��Ĵ�����ֵ */
    OSStart();                                                              /* ��ʼ���� */
    
    for (;;)
    {
        /* ����������� */
    }
}

/**
 * @brief       ��ʼ����
 * @param       ��
 * @retval      ��
 */
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr = 0;
    CPU_INT32U cnts;
    msg_key = OSMboxCreate((void *)0);  /* ������Ϣ���� */
    sem_beep = OSSemCreate(0);          /* �����ź��� */
    OSStatInit();                   /* ����ͳ������ */
    
    /* �������õĽ���Ƶ������SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OS_TICKS_PER_SEC);
    OS_CPU_SysTickInit(cnts);
    OS_ENTER_CRITICAL();            /* �����ٽ���(�ر��ж�) */
    
    /* ������ */
    OSTaskCreateExt((void(*)(void *) )main_task,
                    (void *          )0,
                    (OS_STK *        )&MAIN_TASK_STK[MAIN_STK_SIZE - 1],
                    (INT8U           )MAIN_TASK_PRIO,
                    (INT16U          )MAIN_TASK_PRIO,
                    (OS_STK *        )&MAIN_TASK_STK[0],
                    (INT32U          )MAIN_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
		
		/* �̵������� */							
		OSTaskCreateExt((void(*)(void *) )relay_task,
                    (void *          )0,
                    (OS_STK *        )&RELAY_TASK_STK[RELAY_STK_SIZE - 1],
                    (INT8U           )RELAY_TASK_PRIO,
                    (INT16U          )RELAY_TASK_PRIO,
                    (OS_STK *        )&RELAY_TASK_STK[0],
                    (INT32U          )RELAY_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
    
		/* ��ʼ������ */									
		OSTaskCreateExt((void(*)(void *) )begin_task,
                    (void *          )0,
                    (OS_STK *        )&BEGIN_TASK_STK[BEGIN_STK_SIZE - 1],
                    (INT8U           )BEGIN_TASK_PRIO,
                    (INT16U          )BEGIN_TASK_PRIO,
                    (OS_STK *        )&BEGIN_TASK_STK[0],
                    (INT32U          )BEGIN_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);	
			
		/* ��������� */									
		OSTaskCreateExt((void(*)(void *) )dis_task,
                    (void *          )0,
                    (OS_STK *        )&DIS_TASK_STK[DIS_STK_SIZE - 1],//�����ջ���������Ƕ�ջ��С����0��ʼ���Լ�1
                    (INT8U           )DIS_TASK_PRIO,
                    (INT16U          )DIS_TASK_PRIO,
                    (OS_STK *        )&DIS_TASK_STK[0],//�����ջ��ʼ
                    (INT32U          )DIS_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);	
										
		OS_EXIT_CRITICAL();             /* �˳��ٽ���(���ж�) */
    OSTaskSuspend(START_TASK_PRIO); /* ����ʼ���� */
}

void main_task(void *pdata)
{
    uint8_t semmask = 0;
    uint8_t tcnt = 0;
    while (1)
    {
			char pulse_str[6];
	  	char pulse2_str[6];
			char pulse3_str[6];
			
			sprintf(pulse_str,"%d",pulse);
			sprintf(pulse2_str,"%d",pulse2);
			sprintf(pulse3_str,"%d",pulse3);
			
			oled_show_string(0, 36, pulse_str, 3);
			oled_show_string(0, 36, pulse2_str, 3);
			oled_show_string(0, 36, pulse3_str, 3);
			speed_change();
      judge_dis();
			
      if (semmask || sem_beep->OSEventCnt)                                    /* ��Ҫ��ʾsem */
			{
				oled_show_num(24, 24, sem_beep->OSEventCnt, 3, 12, 1);
        if (sem_beep->OSEventCnt == 0)
        {
						semmask = 0;                                                    /* ֹͣ���� */
						oled_refresh_gram();                    /* ������ʾ��OLED ,������ӣ�������*/
         }
			} 
			if (tcnt == 10)                                                         /* 0.6�����һ��CPUʹ���� */
			{		
					tcnt = 0;
					oled_show_num(80, 24, OSCPUUsage, 3, 12, 1);
					oled_refresh_gram();                    /* ������ʾ��OLED ,������ӣ�������*/
					
			} 
			tcnt++;
			OSTimeDly(10);
			oled_clear_little();
		}
}



void relay_task(void *pdata)
{
		relay_moter1();
}

void begin_task(void *pdata)
{
		start_init();
}

void dis_task(void *pdata)
{
		judge_dis();
}

/**
 * @brief       ��ʼ������
 * @param       ��
 * @retval      ��
 */
void ucos_load_main_ui(void)
{
		UART4_SendString("��е�۳�������\r\n");//����û�в��������ã��ͻ�����,������ʱ��һ��
		oled_show_chinese_again(0, 0, 0, chinese_char[0], 1); 
		oled_show_chinese_again(0, 8, 0, chinese_char[1], 1); // ��ʾ�ַ� "��"
		oled_show_chinese_again(16, 0, 1, chinese_char[2], 1); 	
		oled_show_chinese_again(16, 8, 1, chinese_char[3], 1); // ��ʾ�ַ� "е"	
		oled_show_chinese_again(32, 0, 2, chinese_char[4], 1); 
		oled_show_chinese_again(32, 8, 2, chinese_char[5], 1); // ��ʾ�ַ� "��"
		oled_show_chinese_again(48, 0, 3, chinese_char[6], 1); 
		oled_show_chinese_again(48, 8, 3, chinese_char[7], 1); // ��ʾ�ַ� "��"
		oled_show_chinese_again(64, 0, 4, chinese_char[8], 1); 	
		oled_show_chinese_again(64, 8, 4, chinese_char[9], 1); // ��ʾ�ַ� "��"	
		oled_show_chinese_again(80, 0, 5, chinese_char[10], 1); 
		oled_show_chinese_again(80, 8, 5, chinese_char[11], 1); // ��ʾ�ַ� "ϵ"
		oled_show_chinese_again(96, 0, 6, chinese_char[12], 1); 
		oled_show_chinese_again(96, 8, 6, chinese_char[13], 1); // ��ʾ�ַ� "ͳ"
		oled_show_string(0, 24, "sem:", 12);
		oled_show_string(56, 24, "cpu:", 12);
    oled_refresh_gram();       	/* ������ʾ��OLED */
		
}
