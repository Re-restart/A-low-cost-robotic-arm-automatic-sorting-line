#ifndef _TRANS_H
#define _TRANS_H
#include "./SYSTEM/sys/sys.h"

void trans_init(float axmm, float aymm, float azmm, float dir);//逆解计算定一个大方向就可以
float getXmm(void);
float getYmm(void);
float getZmm(void);
float getRotRad(void);
float getLowRad(void);
float getHighRad(void);



#define PI 3.1415926
void calculate_grad(void);
void direction_choose(float dir_nijie);
void pulse_to_angle(uint16_t pulse1_tran, uint16_t pulse2_tran, uint16_t pulse3_tran);
void angle_to_pulse_input(float low, float rot, float high_all, float dir_1, float dir_2, float dir_3);//角度转脉冲每个电机都需要设置
void angle_to_pulse(void);
#endif
