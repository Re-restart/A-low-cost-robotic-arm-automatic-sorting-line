#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "uc-os2_demo.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./BSP/TIMER/gtim.h"
#include "./MALLOC/malloc.h"
#include "./BSP/IIC/i2c.h"
#include "./BSP/RELAY/relay.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/STEPMOTER/littlemoter.h"
#include "./BSP/DISTANCE/distance.h"
#include "./BSP/DMA/dma.h"

//注意检查引脚啥的有没有掉！！B10刚刚就掉了
uint16_t pulse=0;
uint16_t pulse2=0;
uint16_t pulse3=0;//脉冲数初值
uint16_t pulse_we_need=0;//一个脉冲0.45度，1000个脉冲电机自己旋转450度
uint16_t pulse2_we_need=0;
uint16_t pulse3_we_need=0;
uint16_t arr1=89;
uint16_t psc1=899;
uint16_t arr2=79;
uint16_t psc2=799;
uint16_t arr3=89;
uint16_t psc3=899;
uint16_t arr4=99;
uint16_t psc4=7199;
float low;
float high_all;
float rot;


int main(void)
{
  HAL_Init();/* 初始化HAL库 */
  sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟,72M */
  delay_init(72);                     /* 初始化延时函数 */
  led_init();                         /* 初始化LED */
  usart3_init(115200);
	usart2_init(9600);      //波特率不要忘记检查！！！！！
	usart_init(115200);                         /* USART1初始化 */
	gtim_timx_int_init(arr1, psc1);     /* arr控制转速，psc控制计数频率 每1000个时钟周期产生一次中断，并将输入时钟的频率分频为720倍*/   
	gtim_timx2_int_init(arr2,psc2);
	gtim_timx3_int_init(arr3,psc3);
	gtim_timx4_int_init(arr4,psc4);
	dma_gpio_init();
	dma_uart_init(115200);  // 假设波特率为115200
	dma_init(DMA2_Channel5);
 	Dir_init();                                 /* 步进电机方向初始化 */	
	Pulse_init();                           /* 步进电机脉冲口初始化 */
	i2c_gpio_init();
	relay_init();                       /*继电器初始化*/
	oled_init();
	STEPmoter_init();
	dis_init();
	uc_os2_demo();                      /* 运行uC/OS-II例程 */
}
