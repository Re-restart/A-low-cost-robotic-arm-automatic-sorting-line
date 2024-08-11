#include "./BSP/RELAY/relay.h"
#include "./SYSTEM/delay/delay.h"
//这俩继电器都是低电平触发！！！
void relay_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    RELAY_GPIO_CLK_ENABLE();

    gpio_init_struct.Pin = RELAY1_GPIO_PIN;                  
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(RELAY1_GPIO_PORT, &gpio_init_struct);       

    gpio_init_struct.Pin = RELAY2_GPIO_PIN;                   
    HAL_GPIO_Init(RELAY2_GPIO_PORT, &gpio_init_struct);       
    

    RELAY1(0);                                               
    RELAY2(1);                                               
}


void relay_moter1(void)
{
//  HAL_GPIO_WritePin(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN, GPIO_PIN_SET);
//	delay_ms(1500);
  HAL_GPIO_WritePin(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN, GPIO_PIN_RESET);
	delay_ms(1500);                                         
}

void relay_moterstop(void)
{
//  HAL_GPIO_WritePin(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN, GPIO_PIN_SET);
//	delay_ms(1500);

	HAL_GPIO_WritePin(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN, GPIO_PIN_SET);
	delay_ms(1500);    	
}

