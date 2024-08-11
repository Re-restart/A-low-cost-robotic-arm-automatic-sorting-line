#ifndef __LITTLEMOTER_H
#define __LITTLEMOTER_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

typedef enum{
	forword=0,
	reversal=1
}direction;

/******************************************************************************************/
/* 引脚 定义 */
#define STEP_GPIO_PORT                    GPIOD
#define STEP_GPIO2_PORT                   GPIOE
#define STEP_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)             /* PE口时钟使能 */
#define STEP_GPIO2_CLK_ENABLE()           do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)           

#define STEP1_GPIO_PIN                   GPIO_PIN_9
#define STEP2_GPIO_PIN                   GPIO_PIN_11
#define STEP3_GPIO_PIN                   GPIO_PIN_13
#define STEP4_GPIO_PIN                   GPIO_PIN_15
/******************************************************************************************/
/* 端口定义 */
#define STEP1(x)   do{ x ? \
                      HAL_GPIO_WritePin(STEP_GPIO_PORT, STEP1_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(STEP_GPIO_PORT, STEP1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER1翻转 */

#define STEP2(x)   do{ x ? \
                      HAL_GPIO_WritePin(STEP_GPIO2_PORT, STEP2_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(STEP_GPIO2_PORT, STEP2_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER2翻转 */
#define STEP3(x)   do{ x ? \
                      HAL_GPIO_WritePin(STEP_GPIO2_PORT, STEP3_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(STEP_GPIO2_PORT, STEP3_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER3翻转 */
#define STEP4(x)   do{ x ? \
                      HAL_GPIO_WritePin(STEP_GPIO2_PORT, STEP4_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(STEP_GPIO2_PORT, STEP4_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* PULSE1翻转 */
/******************************************************************************************/																
void STEPmoter_init(void);
void stopmoter(void);
void startmoter(uint16_t num, uint16_t time);
void movemoter(direction dir, uint32_t step, uint16_t time);
void movemoter_rotate(direction dir, uint32_t loop, uint16_t time);							
									
#endif
	
