#include "./BSP/DIR/DIR.h"

void Pulse_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    MOTER_PULSE_GPIO_CLK_ENABLE();                                 /* PE口时钟使能 */

    gpio_init_struct.Pin = MOTER1_PULSE_GPIO_PIN|MOTER2_PULSE_GPIO_PIN|MOTER3_PULSE_GPIO_PIN;        /* MOTER1,MOTER2引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* 不上拉也不下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;          /* 低速 */
    HAL_GPIO_Init(MOTER_PULSE_GPIO_PORT, &gpio_init_struct);       /* 初始化MOTER引脚 */
	
		PULSE1(1);/*关闭电机1脉冲计数*/
		PULSE2(1);/*关闭电机2脉冲计数*/
		PULSE3(1);/*关闭电机3脉冲计数*/
	
}

void Dir_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    MOTER_GPIO_CLK_ENABLE();                                 /* PE口时钟使能 */
    gpio_init_struct.Pin = MOTER1_GPIO_PIN|MOTER2_GPIO_PIN;                   /* MOTER1,MOTER2引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* 开漏输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /*不上拉也不下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;          /* 低速 */
    HAL_GPIO_Init(MOTER_GPIO_PORT, &gpio_init_struct);       /* 初始化MOTER引脚 */ 
		
		MOTER3_GPIO_CLK_ENABLE();
		gpio_init_struct.Pin = MOTER3_GPIO_PIN;                   /* MOTER1,MOTER2引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* 开漏输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;                    /*不上拉也不下拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;          /* 低速 */
    HAL_GPIO_Init(MOTER3_GPIO_PORT, &gpio_init_struct);       /* 初始化MOTER引脚 */ 
	
		MOTER3(1);
		MOTER2(1);
		MOTER1(1);
}








