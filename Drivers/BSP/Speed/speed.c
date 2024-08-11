#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"
#include "./BSP/Speed/speed.h"
#include "./BSP/TRANS/trans.h"

extern uint16_t pulse;
extern uint16_t pulse2;
extern uint16_t pulse3;
extern uint16_t pulse_we_need;
extern uint16_t pulse2_we_need;
extern uint16_t pulse3_we_need;
extern uint16_t arr1;
extern uint16_t psc1;
extern uint16_t arr2;
extern uint16_t psc2;
extern uint16_t arr3;
extern uint16_t psc3;

void speed_change()
{
		uint16_t base_pulse1 = pulse_we_need/10;
		uint16_t pulse1_speed1 = base_pulse1;
		uint16_t pulse1_speed4 = base_pulse1*4;
		uint16_t pulse1_speed7 = base_pulse1*7;
			
		uint16_t base_pulse2 = pulse2_we_need/10;
		uint16_t pulse2_speed1 = base_pulse2;
		uint16_t pulse2_speed4 = base_pulse2*4;	
		uint16_t pulse2_speed7 = base_pulse2*7;	
		
		uint16_t base_pulse3 = pulse3_we_need/10;
		uint16_t pulse3_speed1 = base_pulse3;
		uint16_t pulse3_speed4 = base_pulse3*4;	
		uint16_t pulse3_speed7 = base_pulse3*7;	
	
		extern TIM_HandleTypeDef g_timx1_handle; /* 定时器x句柄 */
		if(pulse>pulse1_speed1&&pulse<pulse1_speed4){
			if(pulse%base_pulse1==0){
				HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);										 /* 先关闭ITMx中断 */
				arr1=arr1-3;//自动装载值
				psc1=psc1-3;//预分频系数
				gtim_timx_int_init(arr1, psc1);
				HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);										 /* 打开ITMx中断 */
			}		
		}
		if(pulse>=pulse1_speed4&&pulse<pulse1_speed7){
			if(pulse%base_pulse1==0){
				HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);										 /* 先关闭ITMx中断 */
				arr1=arr1;//自动装载值
				psc1=psc1;//预分频系数
				gtim_timx_int_init(arr1, psc1);
				HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);										 /* 打开ITMx中断 */
			}
		}
		if(pulse>=pulse1_speed7)
		{
			if(pulse%base_pulse1==0){
				HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);										 /* 先关闭ITMx中断 */
				arr1=arr1+1;//自动装载值
				psc1=psc1+1;//预分频系数
				gtim_timx_int_init(arr1, psc1);
				HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);										 /* 先关闭ITMx中断 */
			}
		}
	
////////////////////////////////////////	
		extern TIM_HandleTypeDef g_timx2_handle; 		
		if(pulse2>pulse2_speed1&&pulse2<pulse2_speed4){
			if(pulse2%base_pulse2==0){
				HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);	
				arr2=arr2-3;
				psc2=psc2-3;
				gtim_timx2_int_init(arr2, psc2);//之前用的全是1电机的定时器设置函数，所以2，3电机不减速
				HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);       
			}
		}
		if(pulse2>=pulse2_speed4&&pulse2<pulse2_speed7){
			if(pulse2%base_pulse2==0){
				HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);
				arr2=arr2;
				psc2=psc2;
				gtim_timx2_int_init(arr2, psc2);
				HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);
			}
		}
		if(pulse2>=pulse2_speed7)
		{
			if(pulse2%base_pulse2==0){
				HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);
				arr2=arr2+1;
				psc2=psc2+1;
				gtim_timx2_int_init(arr2, psc2);
				HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);
			}
		}
		
////////////////////////////////////////////
		extern TIM_HandleTypeDef g_timx3_handle; 
		if(pulse3>pulse3_speed1&&pulse3<pulse3_speed4){
			if(pulse3%base_pulse3==0){
				HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
				arr3=arr3-3;
				psc3=psc3-3;			
				gtim_timx3_int_init(arr3, psc3);
				HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn); 
			}
		}
		if(pulse3>=pulse3_speed4&&pulse2<pulse3_speed7){
			if(pulse3%base_pulse3==0){
				HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
				arr3=arr3;
				psc3=psc3;	
				gtim_timx3_int_init(arr3, psc3);
				HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn); 
			}
		}
		if(pulse3>=pulse3_speed7)
		{
			if(pulse3%base_pulse3==0){	
				HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
				arr3=arr3+1;
				psc3=psc3+1;		
				gtim_timx3_int_init(arr3, psc3);
				HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn); 
			}	
		}
		
		
/////////////////////////////////////////////脉冲结束之后的一系列操作
		if(pulse>pulse_we_need)
		{
			pulse=0;
			pulse_we_need=0;
			arr1=89;//自动装载值
			psc1=899;//预分频系数
			gtim_timx_int_init(arr1, psc1);
			HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);	
		}//	还是大于判断比较好
		if(pulse2>pulse2_we_need)
		{
			pulse2=0;
			pulse2_we_need=0;
			arr2=79;
			psc2=799;
			gtim_timx2_int_init(arr2, psc2);
			HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);
		}
		if(pulse3>pulse3_we_need)
		{
			pulse3=0;
			pulse3_we_need=0;
			arr3=89;
			psc3=899;	
			gtim_timx3_int_init(arr3, psc3);
			HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
		}
			
			if(pulse>pulse_we_need&&pulse2>pulse2_we_need)
			{
				pulse=0;
				pulse2=0;
				pulse_we_need=0;
				pulse2_we_need=0;
				arr1=89;//自动装载值
				psc1=899;//预分频系数
				gtim_timx_int_init(arr1, psc1);
				arr2=79;
				psc2=799;
				gtim_timx2_int_init(arr2, psc2);
				HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);	
				HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);
			}//之前不是同时，所以才不需要这个判断
			
			if(pulse2>pulse2_we_need&&pulse3>pulse3_we_need)
			{
				pulse2=0;
				pulse3=0;
				pulse2_we_need=0;
				pulse3_we_need=0;
				arr2=79;
				psc2=799;
				gtim_timx2_int_init(arr2, psc2);
				arr3=89;
				psc3=899;	
				gtim_timx3_int_init(arr3, psc3);
				HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);	
				HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
			}//之前不是同时，所以才不需要这个判断
			
			if(pulse>pulse_we_need&&pulse3>pulse3_we_need)
			{
				pulse=0;
				pulse3=0;
				pulse_we_need=0;
				pulse3_we_need=0;
				arr1=89;//自动装载值
				psc1=899;//预分频系数
				gtim_timx_int_init(arr1, psc1);
				arr3=89;
				psc3=899;	
				gtim_timx3_int_init(arr3, psc3);
				HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);	
				HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
			}//之前不是同时，所以才不需要这个判断
			
			if(pulse>pulse_we_need&&pulse2>pulse2_we_need&&pulse3>pulse3_we_need)
			{
				pulse=0;
				pulse2=0;
				pulse3=0;
				pulse_we_need=0;
				pulse2_we_need=0;
				pulse3_we_need=0;
				arr1=89;//自动装载值
				psc1=899;//预分频系数
				gtim_timx_int_init(arr1, psc1);
				arr2=79;
				psc2=799;
				gtim_timx2_int_init(arr2, psc2);
				arr3=89;
				psc3=899;	
				gtim_timx3_int_init(arr3, psc3);
				HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);
				HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);	
				HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
			}//之前不是同时，所以才不需要这个判断
}
