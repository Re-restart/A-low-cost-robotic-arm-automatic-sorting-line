#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"
#include "./BSP/IIC/i2c.h"

void i2c_gpio_init()
{
		GPIO_InitTypeDef gpio_init_struct;

		I2C1_GPIO_CLK_ENABLE();
		gpio_init_struct.Pin = I2C1_SCL_GPIO_PIN;                   /* SCL���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(I2C1_SCL_GPIO_PORT, &gpio_init_struct);    

    gpio_init_struct.Pin = I2C1_SDA_GPIO_PIN;                   /* SDA���� */
		gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* ��©��� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(I2C1_SDA_GPIO_PORT, &gpio_init_struct);    
    
		//����ֻ�ر�����SCL��SDA����Ҫֹͣ�豸
    i2c_end();
		
}

void iic_delay()
{
		delay_us(2);
}

void i2c_start()
{
		SDA(1);
		SCL(1);
		iic_delay();
		SDA(0);/* START�ź�: ��SCLΪ��ʱ, SDA�Ӹ߱�ɵ�, ��ʾ��ʼ�ź� */
		iic_delay();
		SCL(0); /* ǯסI2C���ߣ�׼�����ͻ�������� */
		iic_delay();	
}

void i2c_end()
{
		SDA(0);/* STOP�ź�: ��SCLΪ��ʱ, SDA�ӵͱ�ɸ�, ��ʾֹͣ�ź� */
		iic_delay();
		SCL(1);
		iic_delay();
		SDA(1);/* ����I2C���߽����ź� */
		iic_delay();
}

/**
 * @brief       �ȴ�Ӧ���źŵ���
 * @param       ��
 * @retval      1������Ӧ��ʧ��
 *              0������Ӧ��ɹ�
 */
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    SDA(1);     /* �����ͷ�SDA��(��ʱ�ⲿ������������SDA��) */
    iic_delay();
    SCL(1);     /* SCL=1, ��ʱ�ӻ����Է���ACK */
    iic_delay();

    while (SDA_Read)    /* �ȴ�Ӧ�� */
    {
        waittime++;

        if (waittime > 250)
        {
            i2c_end();
            rack = 1;
            break;
        }
    }

    SCL(0);     /* SCL=0, ����ACK��� */
    iic_delay();
    return rack;
}

/**
 * @brief       ����ACKӦ��
 * @param       ��
 * @retval      ��
 */
void iic_ack(void)
{
    SDA(0);     /* SCL 0 -> 1  ʱ SDA = 0,��ʾӦ�� */
    iic_delay();
    SCL(1);     /* ����һ��ʱ�� */
    iic_delay();
    SCL(0);
    iic_delay();
    SDA(1);     /* �����ͷ�SDA�� */
    iic_delay();
}

/**
 * @brief       ������ACKӦ��
 * @param       ��
 * @retval      ��
 */
void iic_nack(void)
{
    SDA(1);     /* SCL 0 -> 1  ʱ SDA = 1,��ʾ��Ӧ�� */
    iic_delay();
    SCL(1);     /* ����һ��ʱ�� */
    iic_delay();
    SCL(0);
    iic_delay();
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       data: Ҫ���͵�����
 * @retval      ��
 */
void iic_send_byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        SDA((data & 0x80) >> 7);    /* ��λ�ȷ��� */
        iic_delay();
        SCL(1);
        iic_delay();
        SCL(0);
        data <<= 1;     /* ����1λ,������һ�η��� */
    }
    SDA(1);         /* �������, �����ͷ�SDA�� */
}


/**
 * @brief       IIC��ȡһ���ֽ�
 * @param       ack:  ack=1ʱ������ack; ack=0ʱ������nack
 * @retval      ���յ�������
 */
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* ����1���ֽ����� */
    {
        receive <<= 1;  /* ��λ�����,�������յ�������λҪ���� */
        SCL(1);
        iic_delay();

        if (SDA_Read)
        {
            receive++;
        }
        
        SCL(0);
        iic_delay();
    }

    if (!ack)
    {
        iic_nack();     /* ����nACK */
    }
    else
    {
        iic_ack();      /* ����ACK */
    }

    return receive;
}
