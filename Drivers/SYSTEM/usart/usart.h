
#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"
#include <stdlib.h>


/******************************************************************************************/
/* 引脚 和 串口 定义 
 * 默认是针对USART1的.
 * 注意: 通过修改这几个宏定义,可以支持USART1~UART5任意一个串口.
 */
#define USART_TX_GPIO_PORT                  GPIOA
#define USART_TX_GPIO_PIN                   GPIO_PIN_9
#define USART_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART_RX_GPIO_PORT                  GPIOA
#define USART_RX_GPIO_PIN                   GPIO_PIN_10
#define USART_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART_UX                            USART1
#define USART_UX_IRQn                       USART1_IRQn
#define USART_UX_IRQHandler                 USART1_IRQHandler
#define USART_UX_CLK_ENABLE()               do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */

/******************************************************************************************/

#define USART_REC_LEN               256         /* 定义最大接收字节数 200 */
#define USART_EN_RX                 1           /* 使能（1）/禁止（0）串口1接收 */
#define RXBUFFERSIZE   1                        /* 缓存大小 */

extern UART_HandleTypeDef g_uart1_handle;       /* HAL UART句柄 */

extern uint8_t  g_usart_rx_buf[USART_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usart_rx_sta;                 /* 接收状态标记 */
extern uint8_t g_rx_buffer[RXBUFFERSIZE];       /* HAL库USART接收Buffer */

/******************************************************************************************/

#define USART2_TX_GPIO_PORT                  GPIOA
#define USART2_TX_GPIO_PIN                   GPIO_PIN_2
#define USART2_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART2_RX_GPIO_PORT                  GPIOA
#define USART2_RX_GPIO_PIN                   GPIO_PIN_3
#define USART2_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART2_UX                            USART2
#define USART2_UX_IRQn                       USART2_IRQn
#define USART2_UX_IRQHandler                 USART2_IRQHandler
#define USART2_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 时钟使能 */

/******************************************************************************************/

#define USART2_REC_LEN               200         /* 定义最大接收字节数 200 */
#define USART2_EN_RX                 1           /* 使能（1）/禁止（0）串口1接收 */
#define USART2_RXBUFFERSIZE          1               /* 缓存大小 */

extern UART_HandleTypeDef g_uart2_handle;       /* HAL UART句柄 */

extern uint8_t  g_usart2_rx_buf[USART2_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usart2_rx_sta;                 /* 接收状态标记 */
extern uint8_t g_usart2_rx_buffer[USART2_RXBUFFERSIZE];       /* HAL库USART接收Buffer */
/************************************************************************/

#define USART3_TX_GPIO_PORT                  GPIOB
#define USART3_TX_GPIO_PIN                   GPIO_PIN_10
#define USART3_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART3_RX_GPIO_PORT                  GPIOB
#define USART3_RX_GPIO_PIN                   GPIO_PIN_11
#define USART3_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define USART3_UX                            USART3
#define USART3_UX_IRQn                       USART3_IRQn
#define USART3_UX_IRQHandler                 USART3_IRQHandler
#define USART3_UX_CLK_ENABLE()               do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)  /* USART1 时钟使能 */

/******************************************************************************************/

#define USART3_REC_LEN               200         /* 定义最大接收字节数 200 */
#define USART3_EN_RX                 1           /* 使能（1）/禁止（0）串口1接收 */
#define USART3_RXBUFFERSIZE          1                        /* 缓存大小 */

extern UART_HandleTypeDef g_uart3_handle;       /* HAL UART句柄 */

extern uint16_t g_usart3_rx_sta;                 /* 接收状态标记 */
extern uint8_t  g_usart3_rx_buf[USART3_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint8_t g_usart3_rx_buffer[USART3_RXBUFFERSIZE];       /* HAL库USART接收Buffer */

void handle_angle_data(const char *data);
float u8int2float(const uint8_t* data, int length, int is_signed);
void usart_init(uint32_t bound);                /* 串口初始化函数 */
void usart3_init(uint32_t bound);
void UART2_SendString(char *string);
void UART3_SendString(char *string);
void usart2_init(uint32_t baudrate);
#endif


