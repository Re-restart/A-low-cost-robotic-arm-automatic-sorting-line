#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"
#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)  /* TIM3 时钟使能 */

#define GTIM_TIMX2_INT                       TIM2
#define GTIM_TIMX2_INT_IRQn                  TIM2_IRQn
#define GTIM_TIMX2_INT_IRQHandler            TIM2_IRQHandler
#define GTIM_TIMX2_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)  /* TIM2 时钟使能 */

#define GTIM_TIMX3_INT                       TIM5
#define GTIM_TIMX3_INT_IRQn                  TIM5_IRQn
#define GTIM_TIMX3_INT_IRQHandler            TIM5_IRQHandler
#define GTIM_TIMX3_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)  /* TIM5 时钟使能 */

#define GTIM_TIMX4_INT                       TIM4
#define GTIM_TIMX4_INT_IRQn                  TIM4_IRQn 
#define GTIM_TIMX4_INT_IRQHandler            TIM4_IRQHandler
#define GTIM_TIMX4_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM4_CLK_ENABLE(); }while(0)  /* TIM4 时钟使能 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc);
void gtim_timx2_int_init(uint16_t arr, uint16_t psc);
void gtim_timx3_int_init(uint16_t arr, uint16_t psc);
void gtim_timx4_int_init(uint16_t arr, uint16_t psc);

extern TIM_HandleTypeDef g_timx1_handle; /* 定时器x句柄 */
extern TIM_HandleTypeDef g_timx2_handle; 
extern TIM_HandleTypeDef g_timx3_handle; 
extern TIM_HandleTypeDef g_timx4_handle; 
#endif

















