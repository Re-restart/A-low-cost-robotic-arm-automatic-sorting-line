/**
 ****************************************************************************************************
 * @file        rtc.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-22
 * @brief       RTC 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200422
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __RTC_H
#define __RTC_H

#include "./SYSTEM/sys/sys.h"


/* 时间结构体, 包括年月日周时分秒等信息 */
typedef struct
{
    uint8_t hour;       /* 时 */
    uint8_t min;        /* 分 */
    uint8_t sec;        /* 秒 */
    /* 公历年月日周 */
    uint16_t year;      /* 年 */
    uint8_t  month;     /* 月 */
    uint8_t  date;      /* 日 */
    uint8_t  week;      /* 周 */
} _calendar_obj;
extern _calendar_obj calendar;                      /* 时间结构体 */


/* 静态函数 */
static uint8_t rtc_is_leap_year(uint16_t year);     /* 判断当前年份是不是闰年 */
static long rtc_date2sec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);   /* 将年月日时分秒转换成秒钟数 */

/* 接口函数 */
uint8_t rtc_init(void);                             /* 初始化RTC */
void rtc_get_time(void);                            /* 获取RTC时间信息 */
uint16_t rtc_read_bkr(uint32_t bkrx);               /* 读取后备寄存器 */
void rtc_write_bkr(uint32_t bkrx, uint16_t data);   /* 写后备寄存器 */ 
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);    /* 根据年月日获取星期几 */
uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);   /* 设置时间 */
uint8_t rtc_set_alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);  /* 设置闹钟时间 */

#endif

















