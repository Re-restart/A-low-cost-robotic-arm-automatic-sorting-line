#include "./BSP/TRANS/trans.h"
#include "./BSP/DIR/DIR.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/DMA/dma.h"
#include "./SYSTEM/delay/delay.h"
#include <cmath>
#include <cstdio>  // for printf

//֮ǰû�п��ǹ���С���ִ����ȵ����⣬�������
//����Ӧ��Ӧ�ø�pulse��0�����߿����жϣ���֮ǰ��Ӧ�ǲ��õģ��������ﲢû���ı�pulse��ֻ����pulse_we_need��������ô���£�
extern uint16_t pulse_we_need;
extern uint16_t pulse2_we_need;
extern uint16_t pulse3_we_need;
extern uint16_t pulse;
extern uint16_t pulse2;
extern uint16_t pulse3;
extern float low;
extern float high_all;
extern float rot;
float high;
float xmm;
float ymm;
float zmm;
float angle1_tran;
float angle2_tran;
float angle3_tran;
float per_angle = 0.05635;//360�ȳ���6400

void trans_init(float axmm, float aymm, float azmm, float dir)
{
    xmm = axmm;
    ymm = aymm;
    zmm = azmm;
    calculate_grad();
		direction_choose(dir);
    angle_to_pulse();
    
}

float getXmm(void)
{
    return xmm;
}

float getYmm(void)
{
    return ymm;
}

float getZmm(void)
{
    return zmm;
}

float getRotRad(void)
{
    return rot;
}

float getLowRad(void)
{
    return low;
}

float getHighRad(void)
{
    return high;
}

float normalize_angle(float angle, float modulo, float offset)
{
    return fmod(angle + offset, modulo) - offset;
}

void calculate_grad(void)
{
    float rrot = sqrt((xmm * xmm) + (ymm * ymm));
    float rside = sqrt((rrot * rrot) + (zmm * zmm));

    if (fabs(rrot) < 1e-6) {
        rot = 0; // �������0�����
    } else {
        rot = atan2(ymm, xmm); // ʹ�� atan2 ���� asin
    }

    high = 2.0 * asin((rside * 0.5) / 120.0);

    // ʹ�� atan2 ���� low
    if (fabs(rside) < 1e-6) {
        low = 0; // �������0�����
    } else {
        low = atan2(zmm, rrot) + ((PI - high) / 2.0) - (PI / 2.0);
    }

    if (fabs(rot) > (PI / 2.0)) {
        rot = normalize_angle(rot, PI / 2.0, PI / 4.0);//�ݲ�Ƚϸ�����
    }

    if (fabs(low) > (PI / 4.0)) {
        low = normalize_angle(low, PI / 2.0, PI / 4.0);
    }

    float high_prime = high + low;
    high_all = high_prime;

    if (fabs(high_prime) > (PI / 3.0)) {
        high_all = normalize_angle(high_prime, PI / 3.0, PI / 6.0);
    }
}

void direction_choose(float dir_nijie)
{
    if(dir_nijie==1&&low!=0)
		{
			MOTER1(0);
			delay_ms(500);
		}
		if(dir_nijie==1&&rot!=0)
		{
			MOTER3(0);
			delay_ms(500);
		}
		if(dir_nijie==1&&high_all!=0)
		{
			MOTER2(0);
			delay_ms(500);
		}
		 if(dir_nijie==0&&low!=0)
		{
			MOTER1(1);
			delay_ms(500);
		}
		if(dir_nijie==0&&rot!=0)
		{
			MOTER3(1);
			delay_ms(500);
		}
		if(dir_nijie==0&&high_all!=0)
		{
			MOTER2(1);
			delay_ms(500);
		}
		printf("normalized low: %f, rot: %f, high_all: %f\r\n", low, rot, high_all);
}

void angle_to_pulse(void)
{
    pulse_we_need = (low*9/32) / per_angle;
    pulse2_we_need = (rot*9/32) / per_angle;
    pulse3_we_need = (high_all*9/32) / per_angle;
	  printf("normalized low_pulse: %d, rot_pulse: %d, high_all_pulse: %d\r\n", pulse_we_need, pulse2_we_need, pulse3_we_need);  // �������
}

void angle_to_pulse_input(float low, float rot, float high_all, float dir_1, float dir_2, float dir_3)
{//low��rot, high_all�����ⲿ������Ӧ���˶��ĽǶ�
    pulse_we_need = (low*9/32) / per_angle;
    pulse2_we_need = (rot*9/32) / per_angle;
    pulse3_we_need = (high_all*9/32) / per_angle;//֮ǰ���û���������㣡�����Բ��ܻ��кܶ�����塣����
	  if(dir_1==1&&low!=0)
		{
			MOTER1(0);
			delay_ms(500);
			HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);
			if(pulse==pulse_we_need)
			{
				pulse=0;
				pulse_we_need=0;
				HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);
			}
		}
		if(dir_2==1&&rot!=0)
		{
			MOTER3(0);
			delay_ms(500);
			HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn);
			if(pulse3==pulse3_we_need)
			{
				pulse3=0;
				pulse3_we_need=0;
				HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
				return;
			}
		}
		if(dir_3==1&&high_all!=0)
		{
			MOTER2(0);
			delay_ms(500);
			HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);
			if(pulse2==pulse2_we_need)
			{
				pulse2=0;
				pulse2_we_need=0;
				HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);
			}
		}
		 if(dir_1==0&&low!=0)
		{
			MOTER1(1);
			delay_ms(500);
			HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);
			if(pulse==pulse_we_need)
			{
				pulse=0;
				pulse_we_need=0;
				HAL_NVIC_DisableIRQ(GTIM_TIMX_INT_IRQn);
			}
		}
		if(dir_2==0&&rot!=0)
		{
			MOTER3(1);
			delay_ms(500);
			HAL_NVIC_EnableIRQ(GTIM_TIMX3_INT_IRQn);
			if(pulse3==pulse3_we_need)
			{
				pulse3=0;
				pulse3_we_need=0;
				HAL_NVIC_DisableIRQ(GTIM_TIMX3_INT_IRQn);
			}
		}
		if(dir_3==0&&high_all!=0)
		{
			MOTER2(1);
			delay_ms(500);
			HAL_NVIC_EnableIRQ(GTIM_TIMX2_INT_IRQn);//����Ҫ�ӣ����ӵĻ�ֻ�ܼ������壬���������
			if(pulse2==pulse2_we_need)
			{
				pulse2=0;
				pulse2_we_need=0;
				HAL_NVIC_DisableIRQ(GTIM_TIMX2_INT_IRQn);
			}
		}
		printf("input low_pulse: %d, rot_pulse: %d, high_all_pulse: %d\r\n", pulse_we_need, pulse2_we_need, pulse3_we_need);  // �������
}

void pulse_to_angle(uint16_t pulse1_tran, uint16_t pulse2_tran, uint16_t pulse3_tran)
{
    low = pulse1_tran * per_angle;
    rot = pulse2_tran * per_angle;
    high_all = pulse3_tran * per_angle;
		printf("normalized low: %f, rot: %f, high_all: %f\r\n", angle1_tran, angle2_tran, angle3_tran);  // �������
}

