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

#define UART4_REC_LEN               256         /* �����������ֽ��� 200 */
#define UART4_EN_RX                 1           /* ʹ�ܣ�1��/��ֹ��0������1���� */
#define RXBUFFERSIZE                1                        /* �����С */

extern uint8_t g_uart4_rx_buf[UART4_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint8_t g_uart4_rx_buffer[RXBUFFERSIZE];  /* HAL��ʹ�õĴ��ڽ��ջ��� */
extern uint16_t g_uart4_rx_sta;/*���ձ�־λ*/
extern uint8_t g_sendbuf[256];  // ����ȫ�ַ��ͻ�����
	
void dma_uart_init(uint32_t baudrate);
void dma_gpio_init(void);
void dma_init(DMA_Channel_TypeDef* DMAx_CHx);
void debug_print(char *string);
void debug_print_formatted(const char *format, ...);
void UART4_SendString(char *string);

#endif
