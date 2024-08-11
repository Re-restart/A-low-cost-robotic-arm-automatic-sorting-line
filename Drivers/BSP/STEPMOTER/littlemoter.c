#include "./BSP/STEPMOTER/littlemoter.h"
#include "./SYSTEM/delay/delay.h"

//�о�û�п���������أ�ֻ��ͨ���ӳ���ʱʱ������ٶȣ�֮����Ҫ����
void STEPmoter_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
	
    STEP_GPIO_CLK_ENABLE(); 
    gpio_init_struct.Pin = STEP1_GPIO_PIN;        /* MOTER1,MOTER2���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* �������,�ʺ���Ҫ�����ߵ�ƽ��͵�ƽ*/
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* ������Ҳ������ */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;          /* ���� */
    HAL_GPIO_Init(STEP_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��MOTER���� */
	
		STEP_GPIO2_CLK_ENABLE();
		gpio_init_struct.Pin = STEP2_GPIO_PIN|STEP3_GPIO_PIN|STEP4_GPIO_PIN;        /* MOTER1,MOTER2���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* �������,�ʺ���Ҫ�����ߵ�ƽ��͵�ƽ*/
    gpio_init_struct.Pull = GPIO_NOPULL;                    /* ������Ҳ������ */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;          /* ���� */
    HAL_GPIO_Init(STEP_GPIO2_PORT, &gpio_init_struct);       /* ��ʼ��MOTER���� */
	
		STEP1(1);
		STEP2(1);
		STEP3(1);
		STEP4(1);
}

void stopmoter()
{
		STEP1(1);
		STEP2(1);
		STEP3(1);
		STEP4(1);
}

void startmoter(uint16_t num, uint16_t time)
{
	switch(num)
	{
		case 0:
				STEP1(0);
				STEP2(1);
				STEP3(1);
				STEP4(1);
				break;
		case 1:
				STEP1(0);
				STEP2(0);
				STEP3(1);
				STEP4(1);	
				break;
		case 2:
				STEP1(1);
				STEP2(0);
				STEP3(1);
				STEP4(1);		
		    break;
		case 3:
				STEP1(1);
				STEP2(0);
				STEP3(0);
				STEP4(1);		
				break;
		case 4:
				STEP1(1);
				STEP2(1);
				STEP3(0);
				STEP4(1);
				break;
		case 5:
				STEP1(1);
				STEP2(1);
				STEP3(0);
				STEP4(0);	
				break;
		case 6:
				STEP1(1);
				STEP2(1);
				STEP3(1);
				STEP4(0);		
		    break;
		case 7:
				STEP1(0);
				STEP2(1);
				STEP3(1);
				STEP4(0);		
				break;}
	delay_ms(time);
	stopmoter();
	
}

//step�ǵ����ת�Ĳ���,��������ת
void movemoter(direction dir, uint32_t step, uint16_t time)
{
	uint16_t STEP=0;
	if(dir==forword)
	{
		for(uint16_t i=0;i<step;i++){
			STEP++;
			startmoter(STEP, time);
			if(STEP>7)
			{
				STEP=0;
			}
		}
	}else if(dir==reversal){
		STEP=8;
		for(int16_t j=step;j>0;j--){
			STEP--;
			startmoter(STEP, time);
			if(STEP==0)
			{
				STEP=8;
			}
		}
	}
}

//step�ǵ����ת�Ĳ���,��Ȧ����ת
void movemoter_rotate(direction dir, uint32_t loop, uint16_t time)
{
	movemoter(dir, loop*4076, time);
}

