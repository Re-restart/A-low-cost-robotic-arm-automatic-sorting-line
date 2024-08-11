#ifndef _DMA_H
#define _DMA_H
#include "./SYSTEM/sys/sys.h"

#define UART4_GPIO_CLK_ENABLE()      do{__HAL_RCC_GPIOC_CLK_ENABLE(); }while(0);
#define UART4_UX_CLK_ENABLE()        do{__HAL_RCC_UART4_CLK_ENABLE(); }while(0);
#define UART4_GPIO_PORT              GPIOC
#define UARTTX4_GPIO_PIN             GPIO_PIN_10
#define UARTRX4_GPIO_PIN             GPIO_PIN_11
#define UART4_UX                     UART4
#define UART4_UX_IRQn                UART4_IRQn
#define UART4_UX_IRQHandler          UART4_IRQHandler

/******************************************************************************************/

#define UART4_REC_LEN               256         /* 定义最大接收字节数 200 */
#define UART4_EN_RX                 1           /* 使能（1）/禁止（0）串口1接收 */
#define RXBUFFERSIZE                1                        /* 缓存大小 */

extern uint8_t g_uart4_rx_buf[UART4_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint8_t g_uart4_rx_buffer[RXBUFFERSIZE];  /* HAL库使用的串口接收缓冲 */
extern uint16_t g_uart4_rx_sta;/*接收标志位*/
extern uint8_t g_sendbuf[256];  // 定义全局发送缓冲区
	
void dma_uart_init(uint32_t baudrate);
void dma_gpio_init(void);
void dma_init(DMA_Channel_TypeDef* DMAx_CHx);
void debug_print(char *string);
void debug_print_formatted(const char *format, ...);
void UART4_SendString(char *string);

#endif
