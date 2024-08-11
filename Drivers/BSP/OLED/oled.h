#ifndef _OLED_H
#define _OLED_H

#include "./SYSTEM/sys/sys.h"

/* 命令/数据 定义 */
#define OLED_CMD        0       /* 写命令 */
#define OLED_DATA       1       /* 写数据 */

void oled_init(void);
void OLED_WR_Byte(uint8_t dat, uint8_t mode);
void oled_refresh_gram(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot);
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode);
static uint32_t oled_pow(uint8_t m, uint8_t n);
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode);
void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size);
void Picture(void);
void oled_clear_little(void);
void oled_show_chinese(int8_t x, uint8_t y, unsigned char no, unsigned char *pfont, uint8_t mode);
void oled_show_chinese_again(int8_t x, uint8_t y, unsigned char no, unsigned char *pfont, uint8_t mode);
#endif

