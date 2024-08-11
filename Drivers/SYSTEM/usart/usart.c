#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./BSP/TIMER/gtim.h"
#include "string.h"
#include "./BSP/RELAY/relay.h"
#include "./BSP/STEPMOTER/littlemoter.h"
#include "./BSP/TRANS/trans.h"
#include "./BSP/DMA/dma.h"

#define MY_BUFFER_SIZE 100  // 假设接收缓冲区大小为100
char complete_rx_buffer[MY_BUFFER_SIZE];
uint8_t len_usart3;
uint8_t len;
#define MY_USART3_BUFFER_SIZE 100  // 假设接收缓冲区大小为100
char complete_usart3_rx_buffer[MY_USART3_BUFFER_SIZE];

extern float low;
extern float high_all;
extern float rot;

/* 如果使用os,则包括下面的头文件即可. */
#if SYS_SUPPORT_OS
#include "os.h" /* os 使用 */
#endif



/* 接收缓冲, 最大USART_REC_LEN个字节. */
uint8_t g_usart_rx_buf[USART_REC_LEN];
uint16_t g_usart_rx_sta = 0;
uint8_t g_rx_buffer[RXBUFFERSIZE];  /* HAL库使用的串口接收缓冲 */
uint8_t rx_data;
UART_HandleTypeDef g_uart1_handle; 


uint8_t g_usart2_rx_buf[USART2_REC_LEN];
uint16_t g_usart2_rx_sta = 0;
uint8_t g_usart2_rx_buffer[USART2_RXBUFFERSIZE];  /* HAL库使用的串口接收缓冲 */
UART_HandleTypeDef g_uart2_handle;  /* UART句柄 */



uint8_t g_usart3_rx_buf[USART3_REC_LEN];
uint8_t g_usart3_rx_buffer[USART3_RXBUFFERSIZE];  /* HAL库使用的串口接收缓冲 */
uint16_t g_usart3_rx_sta = 0;
uint8_t rx3_data;
UART_HandleTypeDef g_uart3_handle;  /* UART句柄 */

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)            /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");  /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");    /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}


/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* MDK下需要重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
    while ((USART_UX->SR & 0X40) == 0);     /* 等待上一个字符发送完成 */

    USART_UX->DR = (uint8_t)ch;             /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}
#endif
/******************************************************************************************/
/*接收数据判断函数,在这里通过串口调试步进电机*/
void data_analysis(uint8_t *command)
{
	if(strncmp((char *)command,"moter1right",10) == 0)
	{
		trans_init(-100,0,0,1);//向左，far和high,belt方向,high高度很少
		UART4_SendString("trans_init:-100,0,0;moter1right\r\n");
		OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter1left",9) == 0)
	{
		trans_init(100,0,0,0);//向左，far和high,belt方向,high高度很少
		UART4_SendString("trans_init:100,0,0;moter1left\r\n");
		OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter2right",10) == 0)
	{
		trans_init(0,90,0,1);//向左，far和high,belt方向,high高度很少
		UART4_SendString("trans_init:0,90,0;moter2right\r\n");
		OSTimeDly(300);

	}	
	else if(strncmp((char *)command,"moter2left",9) == 0)
	{
		trans_init(0,-50,0,0);//向左，far和high,belt方向,high高度很少
		UART4_SendString("trans_init:0,-50,0;moter2left\r\n");
		OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter3right",10) == 0)
	{                                             /*set 0或reset 1改变方向*/
		trans_init(0,0,80,1);//向左，far和high,belt方向,high高度很少
		UART4_SendString("trans_init:0,0,80;moter3right\r\n");
		OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter3left",9) == 0)
	{
		trans_init(0,0,-10,0);//向左，far和high,belt方向,high高度很少
		UART4_SendString("trans_init:0,0,-10;moter3left\r\n");
		OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter12left",10) == 0)
	{
		MOTER2(0);
		MOTER1(0);
		HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);                     /* 开启ITMx中断 */
		HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn); 
		MOTER2(0);
		MOTER1(0);
    OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter12right",11) == 0)
	{
		MOTER2(1);
		MOTER1(1);
		HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);                     /* 开启ITMx中断 */
		HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn); 
		MOTER2(1);
		MOTER1(1);
    OSTimeDly(300);
	}
	else if(strncmp((char *)command,"moter13left",10) == 0)
	{
		MOTER3(0);
		MOTER1(0);
		HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn);                     /* 开启ITMx中断 */
		HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn); 
		MOTER3(0);
		MOTER1(0);
    OSTimeDly(300);
	}
	else if(strncmp((char *)command,"moter13right",11) == 0)
	{
		MOTER3(1);
		MOTER1(1);
		HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn);                     /* 开启ITMx中断 */ 
		HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn); 
		MOTER3(1);
		MOTER1(1);
    OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter23left",10) == 0)
	{
		MOTER3(0);
		MOTER2(0);
		HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn);                     /* 开启ITMx中断 */
		HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn); 
		MOTER3(0);
		MOTER2(0);
    OSTimeDly(300);
	}
	else if(strncmp((char *)command,"moter23right",11) == 0)
	{
		MOTER3(1);
		MOTER2(1);
		HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn);                     /* 开启ITMx中断 */ 
		HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn); 
		MOTER3(1);
		MOTER2(1);
    OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"togetherleft",11) == 0)
	{
		MOTER2(0);
		MOTER1(0);
		MOTER3(0);
		HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);                     /* 开启ITMx中断 */ 
		HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn);
		HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn); 
		MOTER2(0);
		MOTER1(0);
		MOTER3(0);
    OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"togetherright",11) == 0)
	{
		MOTER2(1);
		MOTER1(1);
		MOTER3(1);
		HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);                     /* 开启ITMx中断 */
		HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn);
		HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn); 
		MOTER2(1);
		MOTER1(1);
		MOTER3(1);
    OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter1stop",11) == 0)
	{
		HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);                     /* 开启ITMx中断 */ 
   	OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"moter2stop",11) == 0)
	{
		HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);                     /* 开启ITMx中断 */ 
    OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"motor3stop",11) == 0)
	{
		HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
    OSTimeDly(300);
	}	
	else if(strncmp((char *)command,"togetherstop",11) == 0)
	{
		relay_moterstop();
		HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);                     /* 开启ITMx中断 */
		OSTimeDly(300);
		HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
		OSTimeDly(300);
		HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn); 
    OSTimeDly(300);
	}
	else if(strncmp((char *)command,"littlemotor_touch",15) == 0)
	{
		delay_ms(100);
		movemoter(forword, 1100, 1);
		stopmoter();
		delay_ms(500);
	}	
	else if(strncmp((char *)command,"littlemotor_put", 13) == 0)
	{
		delay_ms(100);
		movemoter(reversal, 850, 1);
		stopmoter();
		delay_ms(500);
	}
	else if(strncmp((char *)command,"belt_left", 9) == 0)
	{
		relay_moter1();
	}
}

void handle_angle_data(const char *data) {
    char *token;
    float low = 0, rot = 0, high_all = 0, dir_1 = 0, dir_2 = 0, dir_3 = 0;
	//不填方向默认为0，向内/下/左运动

    // 使用strtok进行字符串分割
    token = strtok((char *)data, ";");
    if (token != NULL) {
        low = atof(token);
        token = strtok(NULL, ";");
        if (token != NULL) {
            rot = atof(token);
            token = strtok(NULL, ";");
            if (token != NULL) {
                high_all = atof(token);
								token = strtok(NULL, ";");
								if (token != NULL){ 
										dir_1 = atof(token);
										token = strtok(NULL, ";");
										if (token != NULL){ 
												dir_2 = atof(token);
												token = strtok(NULL, ";");	
												if (token != NULL){ 
														dir_3 = atof(token);
														token = strtok(NULL, ";");
												}
										}
								}
            }
        }
    }
		// 调用处理函数
    angle_to_pulse_input(low, rot, high_all, dir_1, dir_2, dir_3);//trans_init里的是末端距离，
		//这里输入的是角度,trans_init那个在系统任务里就有体现，不需要开启定时器中断，但直接输入角度的它需要
}

float u8int2float(const uint8_t* data, int length, int is_signed) {
    // 根据实际情况实现u8int2float函数
    // 这里假设简单地将字符串转换为浮点数
    char temp[10];
    strncpy(temp, (const char*)data, length);
    temp[length] = '\0';
    return atof(temp);
}

void data_analysis2(const char *command)
{
	handle_angle_data(command);
	
}
///////////////////////////////////////


/**
 * @brief       串口数据接收回调函数
                数据处理在这里进行
 * @param       huart:串口句柄
 * @retval      无
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	static uint16_t data_len = 0;
  if (huart->Instance == USART_UX)                    /* 如果是串口1 */
  {
		static char data_buffer[USART_REC_LEN];
		data_buffer[data_len++] = rx_data;  // 收集数据，rx_data是末尾标志位
		if (rx_data == '.' || data_len >= USART_REC_LEN)
		{   	
            data_buffer[data_len - 1] = '\0'; // 替换英文句号为字符串结束符
            if (data_buffer[0] == ':')  // 检查开始标志
            {
                data_analysis2(data_buffer + 1);     // 处理命令，跳过开始标志
            }

            data_len = 0;                     // 重置数据长度
        }

        // 重新启动接收中断以继续接收数据
        HAL_UART_Receive_IT(&g_uart1_handle, &rx_data, 1);
    }
		if (huart->Instance == USART2_UX)                    /* 如果是串口2 */
    {
        if ((g_usart2_rx_sta & 0x8000) == 0)             /* 接收未完成 */
        {
					if (g_usart2_rx_buffer[0] != 0x0a) //这个明天查一下，写一下功能  
					{
						g_usart2_rx_sta = 0;                 /* 接收错误,重新开始 */
					}else{
						g_usart2_rx_sta |= 0x8000;    
						HAL_NVIC_EnableIRQ(GTIM_TIMX4_INT_IRQn);		
					}
         }
				HAL_NVIC_DisableIRQ(GTIM_TIMX4_INT_IRQn);		
				HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)g_usart2_rx_buffer, USART2_RXBUFFERSIZE);	
		}
		//是不是因为printf会导致无法进入中断回调函数（ERROR8 数据溢出），所以才没法处理命令？
		//是这个原因！！！！问题已经被解决
		if (huart->Instance == USART3_UX) // 确认是否是串口3
    {
				static uint8_t data_buffer[USART_REC_LEN];
        data_buffer[data_len++] = rx3_data;  // 使用正确的变量rx3_data收集数据

        // 检查是否达到最大长度或接收到命令终止符
        if (rx3_data == '.' || data_len >= USART3_REC_LEN)
				{   	
            data_buffer[data_len - 1] = '\0'; // 替换英文句号为字符串结束符
            if (data_buffer[0] == ':')  // 检查开始标志
            {
                data_analysis(data_buffer + 1);     // 处理命令，跳过开始标志
            }

            data_len = 0;                     // 重置数据长度
        }

        // 重新启动接收中断以继续接收数据
        HAL_UART_Receive_IT(&g_uart3_handle, &rx3_data, 1);
    }
}


#if USART_EN_RX /*如果使能了接收*/

void usart_init(uint32_t baudrate)
{
    /*UART 初始化设置*/
    g_uart1_handle.Instance = USART_UX;                                       /* USART_UX */
    g_uart1_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&g_uart1_handle);                                           /* HAL_UART_Init()会使能UART1 */

    /* 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)g_rx_buffer, RXBUFFERSIZE); 
}

/**
 * @brief       UART底层初始化函数
 * @param       huart: UART句柄类型指针
 * @note        此函数会被HAL_UART_Init()调用
 *              完成时钟使能，引脚配置，中断配置
 * @retval      无
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;

    if (huart->Instance == USART_UX)                            /* 如果是串口1，进行串口1 MSP初始化 */
    {
        USART_TX_GPIO_CLK_ENABLE();                             /* 使能串口TX脚时钟 */
        USART_RX_GPIO_CLK_ENABLE();                             /* 使能串口RX脚时钟 */
        USART_UX_CLK_ENABLE();                                  /* 使能串口时钟 */

        gpio_init_struct.Pin = USART_TX_GPIO_PIN;               /* 串口发送引脚号 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO速度设置为高速 */
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct);
                
        gpio_init_struct.Pin = USART_RX_GPIO_PIN;               /* 串口RX脚 模式设置 */
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;    
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct);   /* 串口RX脚 必须设置成输入模式 */
        
#if USART_EN_RX
        HAL_NVIC_EnableIRQ(USART_UX_IRQn);                      /* 使能USART1中断通道 */
        HAL_NVIC_SetPriority(USART_UX_IRQn, 3, 3);              /* 组2，最低优先级:抢占优先级3，子优先级3 */
#endif
    }
}


void UART1_SendString(char *string)
{
    HAL_UART_Transmit(&g_uart1_handle, (uint8_t *)string, strlen(string), HAL_MAX_DELAY);
}

void UART1_ReceiveStart(void)
{
    HAL_UART_Receive_IT(&g_uart1_handle, (uint8_t *)&rx_data, 1);
}


/**
 * @brief       串口1中断服务函数
 * @param       无
 * @retval      无
 */
void USART_UX_IRQHandler(void)
{
#if SYS_SUPPORT_OS                          /* 使用OS */
    OSIntEnter();    
#endif

    HAL_UART_IRQHandler(&g_uart1_handle);   /* 调用HAL库中断处理公用函数 */

#if SYS_SUPPORT_OS                          /* 使用OS */
    OSIntExit();
#endif

}
#endif







//////////////////////////////////////////////////////////////////////

#if USART2_EN_RX /*如果使能了接收*/


/**
 * @brief       串口X初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @note        注意: 必须设置正确的时钟源, 否则串口波特率就会设置异常.
 *              这里的USART的时钟源在sys_stm32_clock_init()函数中已经设置过了.
 * @retval      无
 */
void usart2_init(uint32_t baudrate)
{
	
		GPIO_InitTypeDef gpio_init_struct;
		USART2_TX_GPIO_CLK_ENABLE();                             /* 使能串口TX脚时钟 */
    USART2_RX_GPIO_CLK_ENABLE();                             /* 使能串口RX脚时钟 */
    USART2_UX_CLK_ENABLE();                                  /* 使能串口时钟 */

    gpio_init_struct.Pin = USART2_TX_GPIO_PIN;               /* 串口发送引脚号 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO速度设置为高速 */
    HAL_GPIO_Init(USART2_TX_GPIO_PORT, &gpio_init_struct);
                
		/*UART 初始化设置*/
    g_uart2_handle.Instance = USART2_UX;                                       /* USART_UX */
    g_uart2_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    g_uart2_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    g_uart2_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    g_uart2_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    g_uart2_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    g_uart2_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&g_uart2_handle);                                           /* HAL_UART_Init()会使能UART3*/
		
    /*(串口中断） 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    HAL_UART_Receive_IT(&g_uart2_handle, (uint8_t *)g_usart2_rx_buffer, USART2_RXBUFFERSIZE); 
}

void UART2_SendString(char *string)
{	
	HAL_UART_Transmit(&g_uart2_handle, (uint8_t *)string, strlen(string), HAL_MAX_DELAY);
	
}


/**
 * @brief       串口3中断服务函数
 * @param       无
 * @retval      无
 */
void USART2_UX_IRQHandler(void)
{
#if SYS_SUPPORT_OS                          /* 使用OS */
    OSIntEnter();    
#endif

    HAL_UART_IRQHandler(&g_uart2_handle);   /* 调用HAL库中断处理公用函数 */

#if SYS_SUPPORT_OS                          /* 使用OS */
    OSIntExit();
#endif                                                                                                                                                                                                                                                         

}

#endif








/////////////////////////////////////////

#if USART3_EN_RX /*如果使能了接收*/

/* 接收缓冲, 最大USART_REC_LEN个字节. */


/**
 * @brief       串口X初始化函数
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @note        注意: 必须设置正确的时钟源, 否则串口波特率就会设置异常.
 *              这里的USART的时钟源在sys_stm32_clock_init()函数中已经设置过了.
 * @retval      无
 */
void usart3_init(uint32_t baudrate)
{
	
		GPIO_InitTypeDef gpio_init_struct;
		USART3_TX_GPIO_CLK_ENABLE();                             /* 使能串口TX脚时钟 */
    USART3_RX_GPIO_CLK_ENABLE();                             /* 使能串口RX脚时钟 */
    USART3_UX_CLK_ENABLE();                                  /* 使能串口时钟 */

    gpio_init_struct.Pin = USART3_TX_GPIO_PIN;               /* 串口发送引脚号 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO速度设置为高速 */
    HAL_GPIO_Init(USART3_TX_GPIO_PORT, &gpio_init_struct);
	
		gpio_init_struct.Pin = USART3_RX_GPIO_PIN;               /* 串口发送引脚号 */
    gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;                /* 复用推挽输出 */
    HAL_GPIO_Init(USART3_RX_GPIO_PORT, &gpio_init_struct);
                
		/*UART 初始化设置*/
    g_uart3_handle.Instance = USART3_UX;                                       /* USART_UX */
    g_uart3_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    g_uart3_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    g_uart3_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    g_uart3_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    g_uart3_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    g_uart3_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&g_uart3_handle);                                           /* HAL_UART_Init()会使能UART3*/
    HAL_NVIC_EnableIRQ(USART3_UX_IRQn);                      /* 使能USART3中断通道 */
    HAL_NVIC_SetPriority(USART3_UX_IRQn, 2, 2);              /* 组2，最低优先级:抢占优先级3，子优先级3 */

    /*(串口中断） 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量 */
    HAL_UART_Receive_IT(&g_uart3_handle, (uint8_t *)g_usart3_rx_buffer, USART3_RXBUFFERSIZE); 
}

void UART3_SendString(char *string)
{	
	HAL_UART_Transmit(&g_uart3_handle, (uint8_t *)string, strlen(string), HAL_MAX_DELAY);
	
}
//补一下函数看看，而且发现回调函数全局变量data_len有点撞，所以又改了一下回调函数的变量定义
void UART3_ReceiveStart(void)
{
    HAL_UART_Receive_IT(&g_uart3_handle, (uint8_t *)&rx3_data, 1);
}

//如果从串口3发送消息到串口1显示，波特率要设置一致，不然会出现发送消息较多的问题
//hal库的回调函数弱定义，可不可以理解为一直在调用USART1的回调函数，所以串口3无反应？？？   还真是这个原因

/**
 * @brief       串口3中断服务函数
 * @param       无
 * @retval      无
 */
void USART3_UX_IRQHandler(void)
{
#if SYS_SUPPORT_OS                          /* 使用OS */
    OSIntEnter();    
#endif
    HAL_UART_IRQHandler(&g_uart3_handle);   /* 调用HAL库中断处理公用函数 */

#if SYS_SUPPORT_OS                          /* 使用OS */
    OSIntExit();
#endif                                                                                                                                                                                                                                                         

}

#endif


