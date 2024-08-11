#ifndef __DIR_H
#define __DIR_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* ���� ���� */

#define MOTER3_GPIO_PORT                  GPIOE
#define MOTER3_GPIO_PIN                   GPIO_PIN_0
#define MOTER3_GPIO_CLK_ENABLE()           do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)             /* PE��ʱ��ʹ�� */

#define MOTER_GPIO_PORT                   GPIOF
#define MOTER1_GPIO_PIN                   GPIO_PIN_0
#define MOTER2_GPIO_PIN                   GPIO_PIN_1
#define MOTER_GPIO_CLK_ENABLE()           do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)             /* PE��ʱ��ʹ�� */


#define MOTER_PULSE_GPIO_PORT             GPIOA
#define MOTER1_PULSE_GPIO_PIN             GPIO_PIN_5
#define MOTER2_PULSE_GPIO_PIN             GPIO_PIN_6
#define MOTER3_PULSE_GPIO_PIN             GPIO_PIN_7
#define MOTER_PULSE_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA��ʱ��ʹ�� */

/******************************************************************************************/
/* �˿ڶ��� */
#define MOTER1(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER1_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER1��ת */

#define MOTER2(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER2_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_GPIO_PORT, MOTER2_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER2��ת */
#define MOTER3(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER3_GPIO_PORT, MOTER3_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER3_GPIO_PORT, MOTER3_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* MOTER3��ת */
			
									
#define PULSE1(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER1_PULSE_GPIO_PIN , GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER1_PULSE_GPIO_PIN , GPIO_PIN_RESET); \
                  }while(0)      /* PULSE1��ת */
#define PULSE2(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER2_PULSE_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER2_PULSE_GPIO_PIN , GPIO_PIN_RESET); \
                  }while(0)      /* PULSE2��ת */
#define PULSE3(x)   do{ x ? \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER3_PULSE_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(MOTER_PULSE_GPIO_PORT, MOTER3_PULSE_GPIO_PIN , GPIO_PIN_RESET); \
                  }while(0)      /* PULSE3��ת */
/******************************************************************************************/
/* ����ڵķ�ת */
#define PULSE1_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTER_PULSE_GPIO_PORT, MOTER1_PULSE_GPIO_PIN); }while(0)        /* ��תpulse1 */
#define PULSE2_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTER_PULSE_GPIO_PORT, MOTER2_PULSE_GPIO_PIN); }while(0)        /* ��תpulse2 */								
#define PULSE3_TOGGLE()   do{ HAL_GPIO_TogglePin(MOTER_PULSE_GPIO_PORT, MOTER3_PULSE_GPIO_PIN); }while(0)        /* ��תpulse3 */									
																	
void Dir_init(void);
void Pulse_init(void);
									
#endif
	
