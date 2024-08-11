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


//语音任务以裸机阻塞的形式完成，不再作为RTOS任务
//是不是之前rtos系统任务太多了？才会导致机械臂运动方面和稳定版代码有很大的差距
extern uint16_t pulse;
extern uint16_t pulse2;
extern uint16_t pulse3;
extern bool dma_tx_complete;
/******************************************************************************************/
/* UCOSII任务设置 */

/* START 任务 配置
 * 包括: 任务优先级 堆栈大小 等
 */
#define BEGIN_TASK_PRIO                 5     /* 初始化任务的优先级设置为5*/
#define BEGIN_STK_SIZE                  128     /* 堆栈大小 */

OS_STK BEGIN_TASK_STK[BEGIN_STK_SIZE];          /* 任务堆栈 */
void begin_task(void *pdata);                   /* 任务函数 */

#define START_TASK_PRIO                 10      /* 开始任务的优先级设置为10 */
#define START_STK_SIZE                  128     /* 堆栈大小 */

OS_STK START_TASK_STK[START_STK_SIZE];          /* 任务堆栈 */
void start_task(void *pdata);                   /* 任务函数 */

#define RELAY_TASK_PRIO                 8      /* 传送带任务的优先级设置为8 */
#define RELAY_STK_SIZE                  128     /* 堆栈大小 */

OS_STK RELAY_TASK_STK[RELAY_STK_SIZE];          /* 任务堆栈 */
void relay_task(void *pdata);                   /* 任务函数 */

#define DIS_TASK_PRIO                 2      /* 光电检测任务的优先级设置为最低 */
#define DIS_STK_SIZE                  128     /* 堆栈大小 */

OS_STK DIS_TASK_STK[DIS_STK_SIZE];            /* 任务堆栈 */
void dis_task(void *pdata);                   /* 任务函数 */

/* 主 任务 配置
 * 包括: 任务优先级 堆栈大小 等
 */
#define MAIN_TASK_PRIO                  4       /* 优先级设置(越小优先级越高) */
#define MAIN_STK_SIZE                   512     /* 堆栈大小 */

OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];            /* 任务堆栈 */
void main_task(void *pdata);                    /* 任务函数 */

OS_EVENT *msg_key;                              /* 按键邮箱事件块指针 */
OS_EVENT *sem_beep;                             /* 蜂鸣器信号量指针 */
void ucos_load_main_ui(void);
//比按键优先级更高的是传感器，可以第一时间反馈位置

/******************************************************************************************************/
/**
 * @brief       入口函数
 * @param       无
 * @retval      无
 */
void uc_os2_demo(void)
{
		ucos_load_main_ui(); 
    OSInit();                                                               /* UCOS初始化 */
    OSTaskCreateExt((void(*)(void *) )start_task,                           /* 任务函数 */
                    (void *          )0,                                    /* 传递给任务函数的参数 */
                    (OS_STK *        )&START_TASK_STK[START_STK_SIZE - 1],
                    (INT8U           )START_TASK_PRIO,                      /* 任务优先级 */
                    (INT16U          )START_TASK_PRIO,                      /* 任务ID */
                    (OS_STK *        )&START_TASK_STK[0],                   /* 任务堆栈栈底 */
                    (INT32U          )START_STK_SIZE,                       /* 任务堆栈大小 */
                    (void *          )0,                                    /* 用户补充的存储区 */
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP); /* 任务选项,为了保险起见，所有任务都保存浮点寄存器的值 */
    OSStart();                                                              /* 开始任务 */
    
    for (;;)
    {
        /* 不会进入这里 */
    }
}

/**
 * @brief       开始任务
 * @param       无
 * @retval      无
 */
void start_task(void *pdata)
{
    OS_CPU_SR cpu_sr = 0;
    CPU_INT32U cnts;
    msg_key = OSMboxCreate((void *)0);  /* 创建消息邮箱 */
    sem_beep = OSSemCreate(0);          /* 创建信号量 */
    OSStatInit();                   /* 开启统计任务 */
    
    /* 根据配置的节拍频率配置SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OS_TICKS_PER_SEC);
    OS_CPU_SysTickInit(cnts);
    OS_ENTER_CRITICAL();            /* 进入临界区(关闭中断) */
    
    /* 主任务 */
    OSTaskCreateExt((void(*)(void *) )main_task,
                    (void *          )0,
                    (OS_STK *        )&MAIN_TASK_STK[MAIN_STK_SIZE - 1],
                    (INT8U           )MAIN_TASK_PRIO,
                    (INT16U          )MAIN_TASK_PRIO,
                    (OS_STK *        )&MAIN_TASK_STK[0],
                    (INT32U          )MAIN_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
		
		/* 继电器任务 */							
		OSTaskCreateExt((void(*)(void *) )relay_task,
                    (void *          )0,
                    (OS_STK *        )&RELAY_TASK_STK[RELAY_STK_SIZE - 1],
                    (INT8U           )RELAY_TASK_PRIO,
                    (INT16U          )RELAY_TASK_PRIO,
                    (OS_STK *        )&RELAY_TASK_STK[0],
                    (INT32U          )RELAY_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
    
		/* 初始化任务 */									
		OSTaskCreateExt((void(*)(void *) )begin_task,
                    (void *          )0,
                    (OS_STK *        )&BEGIN_TASK_STK[BEGIN_STK_SIZE - 1],
                    (INT8U           )BEGIN_TASK_PRIO,
                    (INT16U          )BEGIN_TASK_PRIO,
                    (OS_STK *        )&BEGIN_TASK_STK[0],
                    (INT32U          )BEGIN_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);	
			
		/* 光电检测任务 */									
		OSTaskCreateExt((void(*)(void *) )dis_task,
                    (void *          )0,
                    (OS_STK *        )&DIS_TASK_STK[DIS_STK_SIZE - 1],//任务堆栈，数组里是堆栈大小，从0开始所以减1
                    (INT8U           )DIS_TASK_PRIO,
                    (INT16U          )DIS_TASK_PRIO,
                    (OS_STK *        )&DIS_TASK_STK[0],//任务堆栈开始
                    (INT32U          )DIS_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);	
										
		OS_EXIT_CRITICAL();             /* 退出临界区(开中断) */
    OSTaskSuspend(START_TASK_PRIO); /* 挂起开始任务 */
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
			
      if (semmask || sem_beep->OSEventCnt)                                    /* 需要显示sem */
			{
				oled_show_num(24, 24, sem_beep->OSEventCnt, 3, 12, 1);
        if (sem_beep->OSEventCnt == 0)
        {
						semmask = 0;                                                    /* 停止更新 */
						oled_refresh_gram();                    /* 更新显示到OLED ,这句必须加！！！！*/
         }
			} 
			if (tcnt == 10)                                                         /* 0.6秒更新一次CPU使用率 */
			{		
					tcnt = 0;
					oled_show_num(80, 24, OSCPUUsage, 3, 12, 1);
					oled_refresh_gram();                    /* 更新显示到OLED ,这句必须加！！！！*/
					
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
 * @brief       开始主界面
 * @param       无
 * @retval      无
 */
void ucos_load_main_ui(void)
{
		UART4_SendString("机械臂程序低配版\r\n");//假如没有波特率设置，就会阻塞,不加延时试一下
		oled_show_chinese_again(0, 0, 0, chinese_char[0], 1); 
		oled_show_chinese_again(0, 8, 0, chinese_char[1], 1); // 显示字符 "机"
		oled_show_chinese_again(16, 0, 1, chinese_char[2], 1); 	
		oled_show_chinese_again(16, 8, 1, chinese_char[3], 1); // 显示字符 "械"	
		oled_show_chinese_again(32, 0, 2, chinese_char[4], 1); 
		oled_show_chinese_again(32, 8, 2, chinese_char[5], 1); // 显示字符 "臂"
		oled_show_chinese_again(48, 0, 3, chinese_char[6], 1); 
		oled_show_chinese_again(48, 8, 3, chinese_char[7], 1); // 显示字符 "分"
		oled_show_chinese_again(64, 0, 4, chinese_char[8], 1); 	
		oled_show_chinese_again(64, 8, 4, chinese_char[9], 1); // 显示字符 "拣"	
		oled_show_chinese_again(80, 0, 5, chinese_char[10], 1); 
		oled_show_chinese_again(80, 8, 5, chinese_char[11], 1); // 显示字符 "系"
		oled_show_chinese_again(96, 0, 6, chinese_char[12], 1); 
		oled_show_chinese_again(96, 8, 6, chinese_char[13], 1); // 显示字符 "统"
		oled_show_string(0, 24, "sem:", 12);
		oled_show_string(56, 24, "cpu:", 12);
    oled_refresh_gram();       	/* 更新显示到OLED */
		
}
