#ifndef __DIR_H
#define __DIR_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* 引脚 定义 */

#define MOTER3_GPIO_PORT                  GPIOE
#define MOTER3_GPIO_PIN                   GPIO_PIN_0
#define MOTER3_GPIO_CLK_ENABLE()           do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */

#define MOTER_GPIO_PORT                   GPIOF
#define MOTER1_GPIO_PIN                   GPIO_PIN_0
#define MOTER2_GPIO_PIN                   GPIO_PIN_1
#define MOTER_GPIO_CLK_ENABLE()           do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */


#define MOTER_PULSE_GPIO_PORT             GPIOA
#define MOTER1_PULSE_GPIO_PIN             GPIO_PIN_5
#define MOTER2_PULSE_GPIO_PIN             GPIO_PIN_6
#define MOTER3_PULSE_GPIO_PIN             GPIO_PIN_7
#define MOTER_PULSE_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA口时钟使能 */

/******************************************************************************************/
/* 端口定义 */
#define MOTER1(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER1_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER1翻转 */

#define MOTER2(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER2_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER2_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER2翻转 */
#define MOTER3(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER3_GPIO_PORT, MOTER3_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER3_GPIO_PORT, MOTER3_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER3翻转 */
			
									
#define PULSE1(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER1_PULSE_GPIO_PIN , GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER1_PULSE_GPIO_PIN , GPIO_PIN_RESET); \
                  }while(0)      /* PULSE1翻转 */
#define PULSE2(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER2_PULSE_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER2_PULSE_GPIO_PIN , GPIO_PIN_RESET); \
                  }while(0)      /* PULSE2翻转 */
#define PULSE3(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER3_PULSE_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER3_PULSE_GPIO_PIN , GPIO_PIN_RESET); \
                  }while(0)      /* PULSE3翻转 */
/******************************************************************************************/
/* 脉冲口的翻转 */
#define PULSE1_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTER_PULSE_GPIO_PORT, MOTER1_PULSE_GPIO_PIN); }while(0)        /* 翻转pulse1 */
#define PULSE2_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTER_PULSE_GPIO_PORT, MOTER2_PULSE_GPIO_PIN); }while(0)        /* 翻转pulse2 */								
#define PULSE3_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTER_PULSE_GPIO_PORT, MOTER3_PULSE_GPIO_PIN); }while(0)        /* 翻转pulse3 */									
																	
void Dir_init(void);
void Pulse_init(void);
									
#endif
	
