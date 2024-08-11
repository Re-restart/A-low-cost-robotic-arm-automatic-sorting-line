#include "./BSP/DISTANCE/distance.h"
#include "./BSP/RELAY/relay.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/TRANS/trans.h"
#include "./BSP/STEPMOTER/littlemoter.h"

/**
 * @brief       ��ʼ��LED���IO��, ��ʹ��ʱ��
 * @param       ��
 * @retval      ��
 */
void dis_init(void)//��������
{
    GPIO_InitTypeDef gpio_init_struct;
    DIS0_GPIO_CLK_ENABLE();                                 /* ����ʱ��ʹ�� */

    gpio_init_struct.Pin = DIS0_GPIO_PIN;                   /* DIS0���� */
    gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;            /* ��һ�¸����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(DIS0_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��DIS0���� */
	
    DIS(1);                                                /* ���ָߵ�ƽ����״̬*/
}

void judge_dis(void){
	static uint16_t flag_detect=0,i=1,j=3;
	GPIO_PinState pinstate = HAL_GPIO_ReadPin(DIS0_GPIO_PORT, DIS0_GPIO_PIN);
	
  if (pinstate==GPIO_PIN_RESET)
  {//��������þͺ��ˡ�������ץȡʵ�ڼ���
			relay_moterstop();
			angle_to_pulse_input(5, 7, 0, 1, 0, 0);
			movemoter(forword, 1100, 1);
			stopmoter();
			movemoter(reversal, 850, 1);
			stopmoter();
			flag_detect=flag_detect+1;
			DIS(1);
			delay_ms(1500);
			if(pinstate==GPIO_PIN_RESET&&flag_detect>1)
			{
				i=i+1;
				j=j+1.5;
				angle_to_pulse_input(i, j, 0.05, 0, 0, 1);
				movemoter(reversal, 850, 1);
				stopmoter();
				movemoter(forword, 1100, 1);
				stopmoter();
			}
			angle_to_pulse_input(1, 1.5, 1.6, 0, 1, 0);//������Ƕȿ���ͣס
			movemoter(forword, 1100, 1);
			stopmoter();
			movemoter(reversal, 850, 1);
			stopmoter();
			
  }
  else
  {
			pinstate = GPIO_PIN_SET;
  }
}

