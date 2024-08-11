#ifndef _I2C_H
#define _I2C_H

#include "./SYSTEM/sys/sys.h"
/* I2C1 引脚 定义 */

#define I2C1_SCL_GPIO_PORT              GPIOB
#define I2C1_SCL_GPIO_PIN               GPIO_PIN_6

#define I2C1_SDA_GPIO_PORT             GPIOB
#define I2C1_SDA_GPIO_PIN              GPIO_PIN_7
#define I2C1_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

/* I2C1相关定义 */
#define I2C1_I2C                       I2C1
#define I2C1_I2C_CLK_ENABLE()           do{ __HAL_RCC_I2C1_CLK_ENABLE(); }while(0)    

#define SCL(x)   do{ x ? \
                      HAL_GPIO_WritePin(I2C1_SCL_GPIO_PORT, I2C1_SCL_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(I2C1_SCL_GPIO_PORT, I2C1_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* LED0翻转 */

#define SDA(x)   do{ x ? \
                      HAL_GPIO_WritePin(I2C1_SDA_GPIO_PORT, I2C1_SDA_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(I2C1_SDA_GPIO_PORT, I2C1_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* LED1翻转 */

#define SDA_Read                    	HAL_GPIO_ReadPin(I2C1_SDA_GPIO_PORT, I2C1_SDA_GPIO_PIN) 

//void i2c_init(void);
void i2c_gpio_init(void);
void iic_delay(void);
void i2c_start(void);
void i2c_end(void);
uint8_t iic_wait_ack(void);
void iic_ack(void);
void iic_nack(void);
void iic_send_byte(uint8_t data);
uint8_t iic_read_byte(uint8_t ack);

#endif

