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
		gpio_init_struct.Pin = I2C1_SCL_GPIO_PIN;                   /* SCL引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(I2C1_SCL_GPIO_PORT, &gpio_init_struct);    

    gpio_init_struct.Pin = I2C1_SDA_GPIO_PIN;                   /* SDA引脚 */
		gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* 开漏输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(I2C1_SDA_GPIO_PORT, &gpio_init_struct);    
    
		//不能只关闭总线SCL和SDA，需要停止设备
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
		SDA(0);/* START信号: 当SCL为高时, SDA从高变成低, 表示起始信号 */
		iic_delay();
		SCL(0); /* 钳住I2C总线，准备发送或接收数据 */
		iic_delay();	
}

void i2c_end()
{
		SDA(0);/* STOP信号: 当SCL为高时, SDA从低变成高, 表示停止信号 */
		iic_delay();
		SCL(1);
		iic_delay();
		SDA(1);/* 发送I2C总线结束信号 */
		iic_delay();
}

/**
 * @brief       等待应答信号到来
 * @param       无
 * @retval      1，接收应答失败
 *              0，接收应答成功
 */
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    SDA(1);     /* 主机释放SDA线(此时外部器件可以拉低SDA线) */
    iic_delay();
    SCL(1);     /* SCL=1, 此时从机可以返回ACK */
    iic_delay();

    while (SDA_Read)    /* 等待应答 */
    {
        waittime++;

        if (waittime > 250)
        {
            i2c_end();
            rack = 1;
            break;
        }
    }

    SCL(0);     /* SCL=0, 结束ACK检查 */
    iic_delay();
    return rack;
}

/**
 * @brief       产生ACK应答
 * @param       无
 * @retval      无
 */
void iic_ack(void)
{
    SDA(0);     /* SCL 0 -> 1  时 SDA = 0,表示应答 */
    iic_delay();
    SCL(1);     /* 产生一个时钟 */
    iic_delay();
    SCL(0);
    iic_delay();
    SDA(1);     /* 主机释放SDA线 */
    iic_delay();
}

/**
 * @brief       不产生ACK应答
 * @param       无
 * @retval      无
 */
void iic_nack(void)
{
    SDA(1);     /* SCL 0 -> 1  时 SDA = 1,表示不应答 */
    iic_delay();
    SCL(1);     /* 产生一个时钟 */
    iic_delay();
    SCL(0);
    iic_delay();
}

/**
 * @brief       IIC发送一个字节
 * @param       data: 要发送的数据
 * @retval      无
 */
void iic_send_byte(uint8_t data)
{
    uint8_t t;
    
    for (t = 0; t < 8; t++)
    {
        SDA((data & 0x80) >> 7);    /* 高位先发送 */
        iic_delay();
        SCL(1);
        iic_delay();
        SCL(0);
        data <<= 1;     /* 左移1位,用于下一次发送 */
    }
    SDA(1);         /* 发送完成, 主机释放SDA线 */
}


/**
 * @brief       IIC读取一个字节
 * @param       ack:  ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
 */
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )    /* 接收1个字节数据 */
    {
        receive <<= 1;  /* 高位先输出,所以先收到的数据位要左移 */
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
        iic_nack();     /* 发送nACK */
    }
    else
    {
        iic_ack();      /* 发送ACK */
    }

    return receive;
}
