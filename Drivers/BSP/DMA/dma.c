#include "./BSP/DMA/dma.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

UART_HandleTypeDef g_uart4_handle;       /* HAL UART句柄 */
DMA_HandleTypeDef g_dma_handle;
//uint8_t g_uart4_rx_buf[UART4_REC_LEN];  /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
uint8_t g_uart4_rx_buffer[RXBUFFERSIZE];  /* HAL库使用的串口接收缓冲 */
uint16_t g_uart4_rx_sta = 0;/*接收标志位*/

//DMA2 UART4引脚完成log打印及交互功能，防止USART1调试阻塞
void dma_gpio_init(void)
{
		GPIO_InitTypeDef gpio_init_struct;
    UART4_GPIO_CLK_ENABLE();                                 /* GPIOC时钟使能 */
		UART4_UX_CLK_ENABLE();                                  /* 使能串口时钟 */
   
    gpio_init_struct.Pin = UARTTX4_GPIO_PIN;                   /* TX引脚 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;            /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(UART4_GPIO_PORT, &gpio_init_struct);       /* 每个引脚的初始化设置（如模式、速度、上下拉等）可能不同，需要分别初始化*/

    gpio_init_struct.Pin = UARTRX4_GPIO_PIN;                   /* RX引脚 */
		gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;            /* 复用推挽输入*/
		HAL_GPIO_Init(UART4_GPIO_PORT, &gpio_init_struct);       /* 初始化RX引脚 gpio_init_struct是调用同一个变量的地址，类似temp */
    //可以重用 gpio_init_struct 结构体，避免为每个引脚单独声明结构体变量，从而简化代码    
}

void dma_uart_init(uint32_t baudrate)
{
		/*UART 初始化设置*/
    g_uart4_handle.Instance = UART4_UX;                                       /* USART_UX */
    g_uart4_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    g_uart4_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    g_uart4_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    g_uart4_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    g_uart4_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    g_uart4_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&g_uart4_handle);    	/* HAL_UART_Init()会使能UART4*/
	
    HAL_NVIC_SetPriority(UART4_UX_IRQn, 5, 0); // 设置较低的优先级
    HAL_NVIC_EnableIRQ(UART4_UX_IRQn);
    HAL_UART_Receive_IT(&g_uart4_handle, (uint8_t *)g_uart4_rx_buffer, RXBUFFERSIZE);
	
    /* 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置串口接收缓冲以及接收缓冲接收最大数据量 */
   // HAL_UART_Receive_IT(&g_uart4_handle, (uint8_t *)g_uart4_rx_buffer, RXBUFFERSIZE); 
	//是因为中断过于频繁，但只是部分原因，原先是完全运行不了，现在是出不了DMA中断，所以一直播放歌曲
	
	
}

void dma_init(DMA_Channel_TypeDef* DMAx_CHx)
{
		if((uint32_t)DMAx_CHx>(uint32_t)DMA1_Channel7)     //大于DMA1的channel7了，需要用DMA2处理
		{
				__HAL_RCC_DMA2_CLK_ENABLE();
		}
		else{
				__HAL_RCC_DMA1_CLK_ENABLE();
		}

	 /* Tx DMA 配置 */
	 g_dma_handle.Instance = DMAx_CHx; /* UART4_TX 的 DMA 通道: DMA2_Channel5 */
	 g_dma_handle.Init.Direction = DMA_MEMORY_TO_PERIPH; /* 存储器到外设模式 */
	 g_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE; /* 外设非增量模式 */
	 g_dma_handle.Init.MemInc = DMA_MINC_ENABLE; /* 存储器增量模式 */
	 g_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE; /* 外设位宽 */
	 g_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE; /* 存储器位宽 */
	 g_dma_handle.Init.Mode = DMA_NORMAL; /* DMA 模式:正常模式 */
	 g_dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM; /* 中等优先级 */
	 HAL_DMA_Init(&g_dma_handle);
		
	  /* 将 DMA 与 UART4 联系起来(发送 DMA),配置之后再链接*/
	 __HAL_LINKDMA(&g_uart4_handle, hdmatx, g_dma_handle);//hdmatx 是 UART 句柄中代表传输DMA通道的成员变量
	 //将 g_uart4_handle.hdmatx 赋值为 g_dma_handle 的地址，建立UART和DMA之间的联系
	 //不在主函数里再配一遍，直接在这个函数里开启DMA中断
	 HAL_NVIC_SetPriority(DMA2_Channel4_5_IRQn,5,0);
	 HAL_NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
}

void debug_print(char *string)
{
	uint16_t len = strlen(string);
	uint16_t SEND_BUF_SIZE = len + 2; /* 发送数据长度，包括回车和换行符*/
	uint16_t g_sendbuf[SEND_BUF_SIZE]; /* 发送数据缓冲区,数据大小 */
	static uint16_t k=0;
	static uint8_t mask=0;
	// 填充发送缓冲区
  // memcpy(g_sendbuf, string, len); // 复制字符串内容到发送缓冲区
	//mask 用于控制插入的字符顺序，使得先插入回车符（0x0d），然后插入换行符（0x0a）
	for(uint16_t i=0; i<len;i++)
	{
		if(k>=len)
		{
			if(mask){
				g_sendbuf[i]=0x0a;//加入换行符，此时已经加入了回车符
				k=0;
				i=0;
			}else{
				g_sendbuf[i]=0x0d;
				mask++;//先回车后换行
			}
		}else{
				mask=0;
				g_sendbuf[i]=string[k];
				k++;		
		}
	}
	//直接在 string 后添加字符的前提是确保 string 的内存区域足够大，否则可能会导致内存越界错误。
	/*for(uint16_t i=0; i<len;i++)//填充发送缓冲区
	{
			g_sendbuf[i]=string[i];
	}
	g_sendbuf[len]=0x0d;
	g_sendbuf[len+1]=0x0a;*/
	HAL_UART_Transmit_DMA(&g_uart4_handle, (uint8_t *)g_sendbuf, SEND_BUF_SIZE);//resume恢复
	
}

void debug_print_formatted(const char *format, ...)
{
    char buffer[256];  // 定义一个足够大的缓冲区存储格式化字符串
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);//如果不知道为什么函数引用不成功，就要善用查找功能
    va_end(args);

    // 调用 debug_print 发送格式化字符串
    debug_print(buffer);
}


void UART4_SendString(char *string)
{	
	HAL_UART_Transmit(&g_uart4_handle, (uint8_t *)string, strlen(string), HAL_MAX_DELAY);
	
}

/* DMA中断回调函数 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == UART4_UX)
    {
        HAL_UART_DMAStop(huart); /* 传输完成以后关闭串口 DMA ，FLAG判断的话不太确认定义的值*/
    }
}

/* DMA中断处理函数 */
void DMA2_Channel5_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&g_dma_handle);
}

void UART4_UX_IRQHandler(void)
{
    HAL_UART_IRQHandler(&g_uart4_handle);   /* 调用HAL库中断处理公用函数 */
}
