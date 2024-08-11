#ifndef _DISTANCE_H
#define _DISTANCE_H
#include "./SYSTEM/sys/sys.h"

#define DIS0_GPIO_PORT                  GPIOD
#define DIS0_GPIO_PIN                   GPIO_PIN_6
#define DIS0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)             /* PG口时钟使能 */

#define DIS(x)   do{ x ? \
                      HAL_GPIO_WritePin(DIS0_GPIO_PORT, DIS0_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(DIS0_GPIO_PORT, DIS0_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)    

#define DIS_Read                    	HAL_GPIO_ReadPin(DIS0_GPIO_PORT, DIS0_GPIO_PIN)

void dis_init(void);
void judge_dis(void);//函数声明

#endif
