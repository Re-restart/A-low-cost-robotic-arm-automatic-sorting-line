#ifndef _RELAY_H
#define _RELAY_H
#include "./SYSTEM/sys/sys.h"//这个是基础设置文件

//继电器是低电平reset触发 RELAY1(0)亮
#define RELAY1_GPIO_PORT               GPIOF
#define RELAY1_GPIO_PIN                GPIO_PIN_6

#define RELAY2_GPIO_PORT             	 GPIOF
#define RELAY2_GPIO_PIN                GPIO_PIN_7
#define RELAY_GPIO_CLK_ENABLE()     	 do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

#define RELAY1(x)   do{ x ? \
                      HAL_GPIO_WritePin(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)     

#define RELAY2(x)   do{ x ? \
                      HAL_GPIO_WritePin(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(RELAY2_GPIO_PORT, RELAY2_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)    

void relay_init(void);
void relay_moter1(void);								
void relay_moterstop(void);								

#endif
