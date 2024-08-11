#include "./BSP/DMA/dma.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

UART_HandleTypeDef g_uart4_handle;       /* HAL UART��� */
DMA_HandleTypeDef g_dma_handle;
//uint8_t g_uart4_rx_buf[UART4_REC_LEN];  /* ���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
uint8_t g_uart4_rx_buffer[RXBUFFERSIZE];  /* HAL��ʹ�õĴ��ڽ��ջ��� */
uint16_t g_uart4_rx_sta = 0;/*���ձ�־λ*/

//DMA2 UART4�������log��ӡ���������ܣ���ֹUSART1��������
void dma_gpio_init(void)
{
		GPIO_InitTypeDef gpio_init_struct;
    UART4_GPIO_CLK_ENABLE();                                 /* GPIOCʱ��ʹ�� */
		UART4_UX_CLK_ENABLE();                                  /* ʹ�ܴ���ʱ�� */
   
    gpio_init_struct.Pin = UARTTX4_GPIO_PIN;                   /* TX���� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(UART4_GPIO_PORT, &gpio_init_struct);       /* ÿ�����ŵĳ�ʼ�����ã���ģʽ���ٶȡ��������ȣ����ܲ�ͬ����Ҫ�ֱ��ʼ��*/

    gpio_init_struct.Pin = UARTRX4_GPIO_PIN;                   /* RX���� */
		gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;            /* ������������*/
		HAL_GPIO_Init(UART4_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��RX���� gpio_init_struct�ǵ���ͬһ�������ĵ�ַ������temp */
    //�������� gpio_init_struct �ṹ�壬����Ϊÿ�����ŵ��������ṹ��������Ӷ��򻯴���    
}

void dma_uart_init(uint32_t baudrate)
{
		/*UART ��ʼ������*/
    g_uart4_handle.Instance = UART4_UX;                                       /* USART_UX */
    g_uart4_handle.Init.BaudRate = baudrate;                                  /* ������ */
    g_uart4_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* �ֳ�Ϊ8λ���ݸ�ʽ */
    g_uart4_handle.Init.StopBits = UART_STOPBITS_1;                           /* һ��ֹͣλ */
    g_uart4_handle.Init.Parity = UART_PARITY_NONE;                            /* ����żУ��λ */
    g_uart4_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* ��Ӳ������ */
    g_uart4_handle.Init.Mode = UART_MODE_TX_RX;                               /* �շ�ģʽ */
    HAL_UART_Init(&g_uart4_handle);    	/* HAL_UART_Init()��ʹ��UART4*/
	
    HAL_NVIC_SetPriority(UART4_UX_IRQn, 5, 0); // ���ýϵ͵����ȼ�
    HAL_NVIC_EnableIRQ(UART4_UX_IRQn);
    HAL_UART_Receive_IT(&g_uart4_handle, (uint8_t *)g_uart4_rx_buffer, RXBUFFERSIZE);
	
    /* �ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ô��ڽ��ջ����Լ����ջ��������������� */
   // HAL_UART_Receive_IT(&g_uart4_handle, (uint8_t *)g_uart4_rx_buffer, RXBUFFERSIZE); 
	//����Ϊ�жϹ���Ƶ������ֻ�ǲ���ԭ��ԭ������ȫ���в��ˣ������ǳ�����DMA�жϣ�����һֱ���Ÿ���
	
	
}

void dma_init(DMA_Channel_TypeDef* DMAx_CHx)
{
		if((uint32_t)DMAx_CHx>(uint32_t)DMA1_Channel7)     //����DMA1��channel7�ˣ���Ҫ��DMA2����
		{
				__HAL_RCC_DMA2_CLK_ENABLE();
		}
		else{
				__HAL_RCC_DMA1_CLK_ENABLE();
		}

	 /* Tx DMA ���� */
	 g_dma_handle.Instance = DMAx_CHx; /* UART4_TX �� DMA ͨ��: DMA2_Channel5 */
	 g_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH; /* �洢��������ģʽ */
	 g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE; /* ���������ģʽ */
	 g_dma_handle.Init.MemInc = DMA_MINC_ENABLE; /* �洢������ģʽ */
	 g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* ����λ�� */
	 g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE; /* �洢��λ�� */
	 g_dma_handle.Init.Mode = DMA_NORMAL; /* DMA ģʽ:����ģʽ */
	 g_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM; /* �е����ȼ� */
	 HAL_DMA_Init(&g_dma_handle);
		
	  /* �� DMA �� UART4 ��ϵ����(���� DMA),����֮��������*/
	 __HAL_LINKDMA(&g_uart4_handle, hdmatx, g_dma_handle);//hdmatx �� UART ����д�����DMAͨ���ĳ�Ա����
	 //�� g_uart4_handle.hdmatx ��ֵΪ g_dma_handle �ĵ�ַ������UART��DMA֮�����ϵ
	 //����������������һ�飬ֱ������������￪��DMA�ж�
	 HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn,5,0);
	 HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
}

void debug_print(char *string)
{
	uint16_t len = strlen(string);
	uint16_t SEND_BUF_SIZE = len + 2; /* �������ݳ��ȣ������س��ͻ��з�*/
	uint16_t g_sendbuf[SEND_BUF_SIZE]; /* �������ݻ�����,���ݴ�С */
	static uint16_t k=0;
	static uint8_t mask=0;
	// ��䷢�ͻ�����
  // memcpy(g_sendbuf, string, len); // �����ַ������ݵ����ͻ�����
	//mask ���ڿ��Ʋ�����ַ�˳��ʹ���Ȳ���س�����0x0d����Ȼ����뻻�з���0x0a��
	for(uint16_t i=0; i<len;i++)
	{
		if(k>=len)
		{
			if(mask){
				g_sendbuf[i]=0x0a;//���뻻�з�����ʱ�Ѿ������˻س���
				k=0;
				i=0;
			}else{
				g_sendbuf[i]=0x0d;
				mask++;//�Ȼس�����
			}
		}else{
				mask=0;
				g_sendbuf[i]=string[k];
				k++;		
		}
	}
	//ֱ���� string ������ַ���ǰ����ȷ�� string ���ڴ������㹻�󣬷�����ܻᵼ���ڴ�Խ�����
	/*for(uint16_t i=0; i<len;i++)//��䷢�ͻ�����
	{
			g_sendbuf[i]=string[i];
	}
	g_sendbuf[len]=0x0d;
	g_sendbuf[len+1]=0x0a;*/
	HAL_UART_Transmit_DMA(&g_uart4_handle, (uint8_t *)g_sendbuf, SEND_BUF_SIZE);//resume�ָ�
	
}

void debug_print_formatted(const char *format, ...)
{
    char buffer[256];  // ����һ���㹻��Ļ������洢��ʽ���ַ���
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);//�����֪��Ϊʲô�������ò��ɹ�����Ҫ���ò��ҹ���
    va_end(args);

    // ���� debug_print ���͸�ʽ���ַ���
    debug_print(buffer);
}


void UART4_SendString(char *string)
{	
	HAL_UART_Transmit(&g_uart4_handle, (uint8_t *)string, strlen(string), HAL_MAX_DELAY);
	
}

/* DMA�жϻص����� */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART4_UX)
    {
        HAL_UART_DMAStop(huart); /* ��������Ժ�رմ��� DMA ��FLAG�жϵĻ���̫ȷ�϶����ֵ*/
    }
}

/* DMA�жϴ����� */
void DMA2_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&g_dma_handle);
}

void UART4_UX_IRQHandler(void)
{
    HAL_UART_IRQHandler(&g_uart4_handle);   /* ����HAL���жϴ����ú��� */
}
