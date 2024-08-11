
#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"
#include <stdlib.h>


/******************************************************************************************/
/* ���� �� ���� ���� 
 * Ĭ�������USART1��.
 * ע��: ͨ���޸��⼸���궨��,����֧��USART1~UART5����һ������.
 */
#define USART_TX_GPIO_PORT                  GPIOA
#define USART_TX_GPIO_PIN                   GPIO_PIN_9
#define USART_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART_RX_GPIO_PORT                  GPIOA
#define USART_RX_GPIO_PIN                   GPIO_PIN_10
#define USART_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART_UX                            USART1
#define USART_UX_IRQn                       USART1_IRQn
#define USART_UX_IRQHandler                 USART1_IRQHandler
#define USART_UX_CLK_ENABLE()               do{ __HAL_RCC_USART1_CLK_ENABLE(); }while(0)  /* USART1 ʱ��ʹ�� */

/******************************************************************************************/

#define USART_REC_LEN               256         /* �����������ֽ��� 200 */
#define USART_EN_RX                 1           /* ʹ�ܣ�1��/��ֹ��0������1���� */
#define RXBUFFERSIZE   1                        /* �����С */

extern UART_HandleTypeDef g_uart1_handle;       /* HAL UART��� */

extern uint8_t  g_usart_rx_buf[USART_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t g_usart_rx_sta;                 /* ����״̬��� */
extern uint8_t g_rx_buffer[RXBUFFERSIZE];       /* HAL��USART����Buffer */

/******************************************************************************************/

#define USART2_TX_GPIO_PORT                  GPIOA
#define USART2_TX_GPIO_PIN                   GPIO_PIN_2
#define USART2_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART2_RX_GPIO_PORT                  GPIOA
#define USART2_RX_GPIO_PIN                   GPIO_PIN_3
#define USART2_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART2_UX                            USART2
#define USART2_UX_IRQn                       USART2_IRQn
#define USART2_UX_IRQHandler                 USART2_IRQHandler
#define USART2_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 ʱ��ʹ�� */

/******************************************************************************************/

#define USART2_REC_LEN               200         /* �����������ֽ��� 200 */
#define USART2_EN_RX                 1           /* ʹ�ܣ�1��/��ֹ��0������1���� */
#define USART2_RXBUFFERSIZE          1               /* �����С */

extern UART_HandleTypeDef g_uart2_handle;       /* HAL UART��� */

extern uint8_t  g_usart2_rx_buf[USART2_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t g_usart2_rx_sta;                 /* ����״̬��� */
extern uint8_t g_usart2_rx_buffer[USART2_RXBUFFERSIZE];       /* HAL��USART����Buffer */
/************************************************************************/

#define USART3_TX_GPIO_PORT                  GPIOB
#define USART3_TX_GPIO_PIN                   GPIO_PIN_10
#define USART3_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART3_RX_GPIO_PORT                  GPIOB
#define USART3_RX_GPIO_PIN                   GPIO_PIN_11
#define USART3_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define USART3_UX                            USART3
#define USART3_UX_IRQn                       USART3_IRQn
#define USART3_UX_IRQHandler                 USART3_IRQHandler
#define USART3_UX_CLK_ENABLE()               do{ __HAL_RCC_USART3_CLK_ENABLE(); }while(0)  /* USART1 ʱ��ʹ�� */

/******************************************************************************************/

#define USART3_REC_LEN               200         /* �����������ֽ��� 200 */
#define USART3_EN_RX                 1           /* ʹ�ܣ�1��/��ֹ��0������1���� */
#define USART3_RXBUFFERSIZE          1                        /* �����С */

extern UART_HandleTypeDef g_uart3_handle;       /* HAL UART��� */

extern uint16_t g_usart3_rx_sta;                 /* ����״̬��� */
extern uint8_t  g_usart3_rx_buf[USART3_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint8_t g_usart3_rx_buffer[USART3_RXBUFFERSIZE];       /* HAL��USART����Buffer */

void handle_angle_data(const char *data);
float u8int2float(const uint8_t* data, int length, int is_signed);
void usart_init(uint32_t bound);                /* ���ڳ�ʼ������ */
void usart3_init(uint32_t bound);
void UART2_SendString(char *string);
void UART3_SendString(char *string);
void usart2_init(uint32_t baudrate);
#endif


