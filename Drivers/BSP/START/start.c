#include "./BSP/START/start.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/DIR/DIR.h"
#include "./SYSTEM/delay/delay.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/TRANS/trans.h"
#include "./BSP/STEPMOTER/littlemoter.h"

extern uint16_t pulse_we_need;
extern uint16_t pulse2_we_need;
extern uint16_t pulse3_we_need;
extern uint16_t pulse;
extern uint16_t pulse2;
extern uint16_t pulse3;
extern float low;
extern float high;
extern float rot;

//这三个值有点控制不明白，而且以目前的时间也很难把坐标再转换成运动的距离，况且下位机坐标和上位机视觉坐标不一样，又需要标定了
//就用串口通讯字符串吧，视觉直接写if处理，运动到大致位置就可以了，不用获取坐标了
void start_init(void)
{
	UART2_SendString("<G>完成机械臂初始化");
	trans_init(-1,1,-0.8,1);//向左，far和high,belt方向,high高度很少
}

void dongzuo_1(void)
{
  trans_init(-1,1,-0.8,0);//向左，far和high,belt方向,high高度很少
}

void dongzuo_2(void)
{
  trans_init(2,-2,20,1);//这个会动，low
}
