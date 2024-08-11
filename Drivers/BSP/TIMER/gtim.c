#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

extern uint16_t pulse;
extern uint16_t pulse2;
extern uint16_t pulse3;

TIM_HandleTypeDef g_timx1_handle; /* 定时器x句柄 */
TIM_HandleTypeDef g_timx2_handle; 
TIM_HandleTypeDef g_timx3_handle; 
TIM_HandleTypeDef g_timx4_handle; 
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE();                                 /* 使能TIMx时钟 */

    g_timx1_handle.Instance = GTIM_TIMX_INT;                     /* 通用定时器x */
    g_timx1_handle.Init.Prescaler = psc;                         /* 预分频系数 */
    g_timx1_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 递增计数模式 */
    g_timx1_handle.Init.Period = arr;                            /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx1_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 1);             /* 设置中断优先级，抢占优先级1，子优先级1 */

		HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);										 /* 先关闭ITMx中断 */
    HAL_TIM_Base_Start_IT(&g_timx1_handle);                      /* 使能定时器x和定时器x更新中断 */
}

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_INT_IRQHandler(void)
{
	  if(__HAL_TIM_GET_FLAG(&g_timx1_handle, TIM_FLAG_UPDATE) != RESET)
    {	
			PULSE1_TOGGLE();
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)== GPIO_PIN_SET)
			{
				pulse=pulse+1;
			}
			__HAL_TIM_CLEAR_IT(&g_timx1_handle, TIM_IT_UPDATE);  /* 清除定时器溢出中断标志位 */
    }
}




//////////////////////////////////////////////////////////////////////////////////////////////////////
void gtim_timx2_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX2_INT_CLK_ENABLE();                                 /* 使能TIMx时钟 */

    g_timx2_handle.Instance = GTIM_TIMX2_INT;                     /* 通用定时器x */
    g_timx2_handle.Init.Prescaler = psc;                         /* 预分频系数 */
    g_timx2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 递增计数模式 */
    g_timx2_handle.Init.Period = arr;                            /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx2_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX2_INT_IRQn, 1, 1);             /* 设置中断优先级，抢占优先级1，子优先级3 */

		HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);										 /* 先关闭ITMx中断 */
    HAL_TIM_Base_Start_IT(&g_timx2_handle);                      /* 使能定时器x和定时器x更新中断 */
}

/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX2_INT_IRQHandler(void)
{
		if(__HAL_TIM_GET_FLAG(&g_timx2_handle, TIM_FLAG_UPDATE) != RESET)
    {	
			
			PULSE2_TOGGLE();
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)== GPIO_PIN_SET)
			{
				pulse2=pulse2+1;
			}
			__HAL_TIM_CLEAR_IT(&g_timx2_handle, TIM_IT_UPDATE);  /* 清除定时器溢出中断标志位 */
    }
}




void gtim_timx3_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX3_INT_CLK_ENABLE();                                 /* 使能TIMx时钟 */

    g_timx3_handle.Instance = GTIM_TIMX3_INT;                     /* 通用定时器x */
    g_timx3_handle.Init.Prescaler = psc;                         /* 预分频系数 */
    g_timx3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 递增计数模式 */
    g_timx3_handle.Init.Period = arr;                            /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx3_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX3_INT_IRQn, 1, 1);             /* 设置中断优先级，抢占优先级1，子优先级3 */

		HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);										 /* 先关闭ITMx中断 */
    HAL_TIM_Base_Start_IT(&g_timx3_handle);                      /* 使能定时器x和定时器x更新中断 */
}
/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX3_INT_IRQHandler(void)
{
		if(__HAL_TIM_GET_FLAG(&g_timx3_handle, TIM_FLAG_UPDATE) != RESET)
    {	
			PULSE3_TOGGLE();
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)== GPIO_PIN_SET)
			{
				pulse3=pulse3+1;
			}
			__HAL_TIM_CLEAR_IT(&g_timx3_handle, TIM_IT_UPDATE);  /* 清除定时器溢出中断标志位 */
    }
}





/////////////////////////////////////////
void gtim_timx4_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX4_INT_CLK_ENABLE();                                 /* 使能TIMx时钟 */

    g_timx4_handle.Instance = GTIM_TIMX4_INT;                     /* 通用定时器x */
    g_timx4_handle.Init.Prescaler = psc;                         /* 预分频系数 */
    g_timx4_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* 递增计数模式 */
    g_timx4_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		g_timx4_handle.Init.Period = arr;                            /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx4_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX4_INT_IRQn, 2, 2);             /* 设置中断优先级，抢占优先级0，子优先级2 */

		HAL_NVIC_DisableIRQ(GTIM_TIMX4_INT_IRQn);										 
    HAL_TIM_Base_Start_IT(&g_timx4_handle);                      /* 使能定时器x和定时器x更新中断 */
}
/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX4_INT_IRQHandler(void)
{
		if(__HAL_TIM_GET_FLAG(&g_timx4_handle, TIM_FLAG_UPDATE) != RESET)
    {	
			g_usart2_rx_sta |= 0x8000; 
			__HAL_TIM_CLEAR_IT(&g_timx4_handle, TIM_IT_UPDATE);  /* 清除定时器溢出中断标志位 */
		}
}
