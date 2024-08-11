#include "./BSP/DISTANCE/distance.h"
#include "./BSP/RELAY/relay.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/TRANS/trans.h"
#include "./BSP/STEPMOTER/littlemoter.h"

/**
 * @brief       初始化LED相关IO口, 并使能时钟
 * @param       无
 * @retval      无
 */
void dis_init(void)//函数定义
{
    GPIO_InitTypeDef gpio_init_struct;
    DIS0_GPIO_CLK_ENABLE();                                 /* 引脚时钟使能 */

    gpio_init_struct.Pin = DIS0_GPIO_PIN;                   /* DIS0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;            /* 改一下复用输入试试 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(DIS0_GPIO_PORT, &gpio_init_struct);       /* 初始化DIS0引脚 */
	
    DIS(1);                                                /* 保持高电平引脚状态*/
}

void judge_dis(void){
	static uint16_t flag_detect=0,i=1,j=3;
	GPIO_PinState pinstate = HAL_GPIO_ReadPin(DIS0_GPIO_PORT, DIS0_GPIO_PIN);
	
  if (pinstate==GPIO_PIN_RESET)
  {//如果用气泵就好了。。。。抓取实在鸡肋
			relay_moterstop();
			angle_to_pulse_input(5, 7, 0, 1, 0, 0);
			movemoter(forword, 1100, 1);
			stopmoter();
			movemoter(reversal, 850, 1);
			stopmoter();
			flag_detect=flag_detect+1;
			DIS(1);
			delay_ms(1500);
			if(pinstate==GPIO_PIN_RESET&&flag_detect>1)
			{
				i=i+1;
				j=j+1.5;
				angle_to_pulse_input(i, j, 0.05, 0, 0, 1);
				movemoter(reversal, 850, 1);
				stopmoter();
				movemoter(forword, 1100, 1);
				stopmoter();
			}
			angle_to_pulse_input(1, 1.5, 1.6, 0, 1, 0);//在这个角度可以停住
			movemoter(forword, 1100, 1);
			stopmoter();
			movemoter(reversal, 850, 1);
			stopmoter();
			
  }
  else
  {
			pinstate = GPIO_PIN_SET;
  }
}

