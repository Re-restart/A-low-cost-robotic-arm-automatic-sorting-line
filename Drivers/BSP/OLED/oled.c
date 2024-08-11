#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"
#include "./BSP/DIR/DIR.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"
#include "./BSP/IIC/i2c.h"
#include "./BSP/OLED/oled.h"
#include "./BSP/OLED/oledfont.h"
#include "stdlib.h"
static uint8_t g_oled_gram[128][8];  //��ά�����ʾ�Դ�

/**********************************************
// OLED��д����
**********************************************/
//ע�⣡��������i2c�������ݺ�д���ݲ�һ������������д������Ҫģ��i2c���豸д�����ݣ�������
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
	i2c_start();
	iic_send_byte(0x78);
	iic_wait_ack();
	if (mode)
	{
		iic_send_byte(0x40);
	}
	else
	{
		iic_send_byte(0x00);
	}
	iic_wait_ack();
	iic_send_byte(dat);
	iic_wait_ack();
	i2c_end();
}


void oled_refresh_gram(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); /* ����ҳ��ַ��0~7�� */
				OLED_WR_Byte(0x00, OLED_CMD);     /* ������ʾλ�á��е͵�ַ */
        OLED_WR_Byte(0x10, OLED_CMD);     /* ������ʾλ�á��иߵ�ַ */
    for (n = 0; n < 128; n++)
        {
            OLED_WR_Byte(g_oled_gram[n][i], OLED_DATA);
        }
    }
}

void oled_display_on(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);   /* SET DCDC���� */
    OLED_WR_Byte(0X14, OLED_CMD);   /* DCDC ON */
    OLED_WR_Byte(0XAF, OLED_CMD);   /* DISPLAY ON */
}

/**
 * @brief       �ر�OLED��ʾ
 * @param       ��
 * @retval      ��
 */
void oled_display_off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);   /* SET DCDC���� */
    OLED_WR_Byte(0X10, OLED_CMD);   /* DCDC OFF */
    OLED_WR_Byte(0XAE, OLED_CMD);   /* DISPLAY OFF */
}

/**
 * @brief       ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
 * @param       ��
 * @retval      ��
 */
void oled_clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)for (n = 0; n < 128; n++)g_oled_gram[n][i] = 0X00;
    oled_refresh_gram();    /* ������ʾ */
}
    
void oled_clear_little(void)
{
    uint8_t i, n;
		while (i == 3){
			for (n = 80; n < 128; n++)g_oled_gram[n][i] = 0X00;
		}
    oled_refresh_gram();    /* ������ʾ */
}
/**********************************************
// IIC д����
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   i2c_start();
   OLED_WR_Byte(0x78, OLED_CMD);            //Slave address,SA0=0
   OLED_WR_Byte(0x00, OLED_CMD);			//write command
   OLED_WR_Byte(IIC_Command, OLED_CMD); 
   i2c_end();
}
/**********************************************
// IIC д����
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   i2c_start();
   OLED_WR_Byte(0x78, OLED_DATA);			//D/C#=0; R/W#=0
   OLED_WR_Byte(0x40, OLED_DATA);			//write data
   iic_send_byte(IIC_Data);
   i2c_end();
}
//ssd1315��������ssd1306��ͬ,����write iic commend�Ǹ�������������oled��д��ʼ�����룡
void oled_init()
{	
			OLED_WR_Byte(0xAE, OLED_CMD);   //display off
			OLED_WR_Byte(0x20, OLED_CMD);	//Set Memory Addressing Mode	
			OLED_WR_Byte(0x10, OLED_CMD);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
			OLED_WR_Byte(0xb0, OLED_CMD);	//Set Page Start Address for Page Addressing Mode,0-7
			OLED_WR_Byte(0xc8, OLED_CMD);	//Set COM Output Scan Direction
			OLED_WR_Byte(0x00, OLED_CMD);//---set low column address
			OLED_WR_Byte(0x10, OLED_CMD);//---set high column address
			OLED_WR_Byte(0x40, OLED_CMD);//--set start line address
			OLED_WR_Byte(0x81, OLED_CMD);//--set contrast control register
			OLED_WR_Byte(0xdf, OLED_CMD);
			OLED_WR_Byte(0xa1, OLED_CMD);//--set segment re-map 0 to 127
			OLED_WR_Byte(0xa6, OLED_CMD);//--set normal display
			OLED_WR_Byte(0xa8, OLED_CMD);//--set multiplex ratio(1 to 64)
			OLED_WR_Byte(0x3F, OLED_CMD);//
			OLED_WR_Byte(0xa4, OLED_CMD);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
			OLED_WR_Byte(0xd3, OLED_CMD);//-set display offset
			OLED_WR_Byte(0x00, OLED_CMD);//-not offset
			OLED_WR_Byte(0xd5, OLED_CMD);//--set display clock divide ratio/oscillator frequency
			OLED_WR_Byte(0xf0, OLED_CMD);//--set divide ratio
			OLED_WR_Byte(0xd9, OLED_CMD);//--set pre-charge period
			OLED_WR_Byte(0x22, OLED_CMD); //
			OLED_WR_Byte(0xda, OLED_CMD);//--set com pins hardware configuration
			OLED_WR_Byte(0x12, OLED_CMD);
			OLED_WR_Byte(0xdb, OLED_CMD);//--set vcomh
			OLED_WR_Byte(0x20, OLED_CMD);//0x20,0.77xVcc
			OLED_WR_Byte(0x8d, OLED_CMD);//--set DC-DC enable
			OLED_WR_Byte(0x14, OLED_CMD);//
			OLED_WR_Byte(0xaf, OLED_CMD);//--turn on oled panel 	
}


void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot)
{
		 uint8_t pos, bx, temp = 0;
		 if (x > 127 || y > 63) return; /* ������Χ�� */
		 
		 pos = y / 8; /* ���� GRAM ����� y �������ڵ��ֽ�, ÿ���ֽڿ��Դ洢 8 �������� */
		 
		 bx = y % 8; /* ȡ����,������� y �ڶ�Ӧ�ֽ������λ��,����(y)λ�� */
		 temp = 1 << bx; /* ��λ��ʾ���к�, �õ� y ��Ӧ�� bit λ��,���� bit ���� 1 */
		 if (dot) /* ��ʵ�ĵ� */
		 {
				g_oled_gram[x][pos] |= temp;
		 }
		 else /* ���յ�,������ʾ */
		 {
				g_oled_gram[x][pos] &= ~temp;
		 }
}

void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    uint8_t *pfont = 0;
    uint8_t csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2); /* �õ�����һ���ַ���Ӧ������ռ���ֽ��� */
    chr = chr - ' ';        /* �õ�ƫ�ƺ��ֵ,��Ϊ�ֿ��Ǵӿո�ʼ�洢��,��һ���ַ��ǿո� */

    if (size == 12)         /* ����1206���� */
    {
        pfont = (uint8_t *)oled_asc2_1206[chr];
    }
    else if (size == 16)     /* ����1608���� */
    {
        pfont = (uint8_t *)oled_asc2_1608[chr];
    }
    else if (size == 24)     /* ����2412���� */
    {
        pfont = (uint8_t *)oled_asc2_2412[chr];
    }
    else                    /* û�е��ֿ� */
    {
        return;
    }

    for (t = 0; t < csize; t++)
    {
        temp = pfont[t];

        for (t1 = 0; t1 < 8; t1++)
        {
            if (temp & 0x80)oled_draw_point(x, y, mode);
            else oled_draw_point(x, y, !mode);

            temp <<= 1;
            y++;

            if ((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

static uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)
    {
        result *= m;
    }

    return result;
}

void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size, uint8_t mode)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for (t = 0; t < len; t++)   /* ������ʾλ��ѭ�� */
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;  /* ��ȡ��Ӧλ������ */
		if (enshow == 0 && t < (len - 1))   /* û��ʹ����ʾ,�һ���λҪ��ʾ */
        {
            if (temp == 0)
            {
                if (mode & 0X80)   /* ��λ��Ҫ���0 */
                {
                    oled_show_char(x + (size / 2)*t, y, '0', size, mode&0X01); /* ��ʾ�ո�,վλ */
                }
                else
                {
                    oled_show_char(x + (size / 2)*t, y, ' ', size, mode & 0X01);  /* �ÿո�ռλ */
                }
                continue;
            }
            else
            {
                enshow = 1; /* ʹ����ʾ */
            }

        }
		oled_show_char(x + (size / 2)*t, y, temp + '0', size, mode&0X01);    /* ��ʾ�ַ� */
    }
}

void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size)
{
    while ((*p <= '~') && (*p >= ' '))   /* �ж��ǲ��ǷǷ��ַ�! */
    {
        if (x > (128 - (size / 2)))     /* ���Խ�� */
        {
            x = 0;
            y += size;                  /* ���� */
        }

        if (y > (64 - size))            /* �߶�Խ�� */
        {
            y = x = 0;
            oled_clear();
        }

        oled_show_char(x, y, *p, size, 1);   /* ��ʾһ���ַ� */
        x += size / 2;      /* ASCII�ַ����Ϊ���ֿ�ȵ�һ�� */
        p++;
    }
}


/******************************************
// picture������ʾһ��ͼƬ
******************************************/
void Picture()
{
  unsigned char x,y;
  for(y=0;y<8;y++)
    {
			OLED_WR_Byte(0xb0+y, OLED_CMD);
      OLED_WR_Byte(0x00, OLED_CMD);
      OLED_WR_Byte(0x10, OLED_CMD);
      for(x=0;x<132;x++)
        {
          OLED_WR_Byte(show[x], OLED_DATA);
        }
    }
}


/******************************************
// ������ʾ���֣� �򵥵ķ����൱�ڻ�ͼ������ͬʱ��ʾ��������
******************************************/
void oled_show_chinese(int8_t x, uint8_t y, unsigned char no, unsigned char *pfont, uint8_t mode)
{
    uint8_t temp, temp2, t, t1;
    for (t = 0; t < 16; t++)
    {
        temp = pfont[t*2];
				temp2 = pfont[t * 2 + 1]; 
				if (temp & 0x80)oled_draw_point(x, y, mode);
        else oled_draw_point(x, y, !mode);
        temp <<= 1;
        for (t1 = 0; t1 < 16; t1++)
        {
            // �°벿��
            if (temp2 & 0x80) oled_draw_point(x, y+8, mode);
            else oled_draw_point(x, y+8, !mode);
            temp2 <<= 1;
            x++;
        }
				 // �н���������
        y++;
        x -= 8; // �ص���ʼ��
    }
}

void oled_show_chinese_again(int8_t x, uint8_t y, unsigned char no, unsigned char *pfont, uint8_t mode)
{
    uint8_t temp, t, t1;
    uint8_t y0 = y;
    for (t = 0; t < 16; t++)
    {
        temp = pfont[t];

        for (t1 = 0; t1 < 16; t1++)
        {
            if (temp & 0x80)oled_draw_point(x, y, mode);
            else oled_draw_point(x, y, !mode);

            temp <<= 1;
            y++;
            if ((y - y0) == 16)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}
