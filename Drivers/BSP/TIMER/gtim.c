#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"

extern uint16_t pulse;
extern uint16_t pulse2;
extern uint16_t pulse3;

TIM_HandleTypeDef g_timx1_handle; /* ��ʱ��x��� */
TIM_HandleTypeDef g_timx2_handle; 
TIM_HandleTypeDef g_timx3_handle; 
TIM_HandleTypeDef g_timx4_handle; 
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE();                                 /* ʹ��TIMxʱ�� */

    g_timx1_handle.Instance = GTIM_TIMX_INT;                     /* ͨ�ö�ʱ��x */
    g_timx1_handle.Init.Prescaler = psc;                         /* Ԥ��Ƶϵ�� */
    g_timx1_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ��������ģʽ */
    g_timx1_handle.Init.Period = arr;                            /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_timx1_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 1);             /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�1 */

		HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);										 /* �ȹر�ITMx�ж� */
    HAL_TIM_Base_Start_IT(&g_timx1_handle);                      /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
}

/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
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
			__HAL_TIM_CLEAR_IT(&g_timx1_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
    }
}




//////////////////////////////////////////////////////////////////////////////////////////////////////
void gtim_timx2_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX2_INT_CLK_ENABLE();                                 /* ʹ��TIMxʱ�� */

    g_timx2_handle.Instance = GTIM_TIMX2_INT;                     /* ͨ�ö�ʱ��x */
    g_timx2_handle.Init.Prescaler = psc;                         /* Ԥ��Ƶϵ�� */
    g_timx2_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ��������ģʽ */
    g_timx2_handle.Init.Period = arr;                            /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_timx2_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX2_INT_IRQn, 1, 1);             /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */

		HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);										 /* �ȹر�ITMx�ж� */
    HAL_TIM_Base_Start_IT(&g_timx2_handle);                      /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
}

/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
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
			__HAL_TIM_CLEAR_IT(&g_timx2_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
    }
}




void gtim_timx3_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX3_INT_CLK_ENABLE();                                 /* ʹ��TIMxʱ�� */

    g_timx3_handle.Instance = GTIM_TIMX3_INT;                     /* ͨ�ö�ʱ��x */
    g_timx3_handle.Init.Prescaler = psc;                         /* Ԥ��Ƶϵ�� */
    g_timx3_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ��������ģʽ */
    g_timx3_handle.Init.Period = arr;                            /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_timx3_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX3_INT_IRQn, 1, 1);             /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */

		HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);										 /* �ȹر�ITMx�ж� */
    HAL_TIM_Base_Start_IT(&g_timx3_handle);                      /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
}
/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
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
			__HAL_TIM_CLEAR_IT(&g_timx3_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
    }
}





/////////////////////////////////////////
void gtim_timx4_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX4_INT_CLK_ENABLE();                                 /* ʹ��TIMxʱ�� */

    g_timx4_handle.Instance = GTIM_TIMX4_INT;                     /* ͨ�ö�ʱ��x */
    g_timx4_handle.Init.Prescaler = psc;                         /* Ԥ��Ƶϵ�� */
    g_timx4_handle.Init.CounterMode = TIM_COUNTERMODE_UP;        /* ��������ģʽ */
    g_timx4_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		g_timx4_handle.Init.Period = arr;                            /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_timx4_handle);

    HAL_NVIC_SetPriority(GTIM_TIMX4_INT_IRQn, 2, 2);             /* �����ж����ȼ�����ռ���ȼ�0�������ȼ�2 */

		HAL_NVIC_DisableIRQ(GTIM_TIMX4_INT_IRQn);										 
    HAL_TIM_Base_Start_IT(&g_timx4_handle);                      /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
}
/**
 * @brief       ��ʱ���жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX4_INT_IRQHandler(void)
{
		if(__HAL_TIM_GET_FLAG(&g_timx4_handle, TIM_FLAG_UPDATE) != RESET)
    {	
			g_usart2_rx_sta |= 0x8000; 
			__HAL_TIM_CLEAR_IT(&g_timx4_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
		}
}
