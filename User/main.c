#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "uc-os2_demo.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./BSP/TIMER/gtim.h"
#include "./MALLOC/malloc.h"
#include "./BSP/IIC/i2c.h"
#include "./BSP/RELAY/relay.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/STEPMOTER/littlemoter.h"
#include "./BSP/DISTANCE/distance.h"
#include "./BSP/DMA/dma.h"

//ע��������ɶ����û�е�����B10�ով͵���
uint16_t pulse=0;
uint16_t pulse2=0;
uint16_t pulse3=0;//��������ֵ
uint16_t pulse_we_need=0;//һ������0.45�ȣ�1000���������Լ���ת450��
uint16_t pulse2_we_need=0;
uint16_t pulse3_we_need=0;
uint16_t arr1=89;
uint16_t psc1=899;
uint16_t arr2=79;
uint16_t psc2=799;
uint16_t arr3=89;
uint16_t psc3=899;
uint16_t arr4=99;
uint16_t psc4=7199;
float low;
float high_all;
float rot;


int main(void)
{
  HAL_Init();/* ��ʼ��HAL�� */
  sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��,72M */
  delay_init(72);                     /* ��ʼ����ʱ���� */
  led_init();                         /* ��ʼ��LED */
  usart3_init(115200);
	usart2_init(9600);      //�����ʲ�Ҫ���Ǽ�飡��������
	usart_init(115200);                         /* USART1��ʼ�� */
	gtim_timx_int_init(arr1, psc1);     /* arr����ת�٣�psc���Ƽ���Ƶ�� ÿ1000��ʱ�����ڲ���һ���жϣ���������ʱ�ӵ�Ƶ�ʷ�ƵΪ720��*/   
	gtim_timx2_int_init(arr2,psc2);
	gtim_timx3_int_init(arr3,psc3);
	gtim_timx4_int_init(arr4,psc4);
	dma_gpio_init();
	dma_uart_init(115200);  // ���貨����Ϊ115200
	dma_init(DMA2_Channel5);
 	Dir_init();                                 /* ������������ʼ�� */	
	Pulse_init();                           /* �����������ڳ�ʼ�� */
	i2c_gpio_init();
	relay_init();                       /*�̵�����ʼ��*/
	oled_init();
	STEPmoter_init();
	dis_init();
	uc_os2_demo();                      /* ����uC/OS-II���� */
}
