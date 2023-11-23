#include "headfile.h"
#include "oledfont.h"

/**
 * 该函数通过发送一系列命令来配置各种设置来初始化 OLED 显示屏。
 */
void OLED_Init(void)
{
	delay_ms(200);
	OLED_WR_Byte(0xAE, OLED_CMD); //--display off
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
	OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control
	OLED_WR_Byte(0xFF, OLED_CMD); //--128
	OLED_WR_Byte(0xA1, OLED_CMD); // set segment remap
	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse 旋转180度
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD); // Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
	OLED_WR_Byte(0x00, OLED_CMD); //
	OLED_WR_Byte(0xD5, OLED_CMD); // set osc division
	OLED_WR_Byte(0x80, OLED_CMD); //
	OLED_WR_Byte(0xD8, OLED_CMD); // set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD); //
	OLED_WR_Byte(0xD9, OLED_CMD); // Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD); //
	OLED_WR_Byte(0xDA, OLED_CMD); // set com pin configuartion
	OLED_WR_Byte(0x12, OLED_CMD); //
	OLED_WR_Byte(0xDB, OLED_CMD); // set Vcomh
	OLED_WR_Byte(0x30, OLED_CMD); //
	OLED_WR_Byte(0x8D, OLED_CMD); // set charge pump enable
	OLED_WR_Byte(0x14, OLED_CMD); //
	OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}

/**
 * 该函数通过向该行上的所有像素写入 0 来清除 OLED 显示器上的指定行。
 *
 * @param line 需要清除的OLED显示屏的行号。 OLED 显示屏分为 8 行，编号从 0 到 7。
 */
void OLED_ClearLine(uint8_t line)
{
	uint8_t n;
	OLED_WR_Byte(0xb0 + line, OLED_CMD); // 设置页地址（0~7）
	OLED_WR_Byte(0x00, OLED_CMD);		 // 设置显示位置—列低地址
	OLED_WR_Byte(0x10, OLED_CMD);		 // 设置显示位置—列高地址
	for (n = 0; n < 128; n++)
		OLED_WR_Byte(0, OLED_DATA);
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		OLED_WR_Byte(0xb0 + m, 0); // page0-page1
		OLED_WR_Byte(0x00, 0);	   // low column start address
		OLED_WR_Byte(0x10, 0);	   // high column start address
		for (n = 0; n < 128; n++)
		{
			OLED_WR_Byte(fill_Data, 1);
		}
	}
}

/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for (; Del_50ms > 0; Del_50ms--)
		for (m = 6245; m > 0; m--)
			;
}

void Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while (Del_1ms--)
	{
		for (j = 0; j < 123; j++)
			;
	}
}

// 坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

// 开启OLED显示
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}

// 关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}

// 清屏函数,清完屏,整个屏幕是黑色的!
void OLED_Clear(unsigned dat)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(dat, OLED_DATA);
	} // 更新显示
}

void OLED_On(void)
{
	uint8_t i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
		OLED_WR_Byte(0x00, OLED_CMD);	  // 设置显示位置—列低地址
		OLED_WR_Byte(0x10, OLED_CMD);	  // 设置显示位置—列高地址
		for (n = 0; n < 128; n++)
			OLED_WR_Byte(1, OLED_DATA);
	} // 更新显示
}

/**
 * 该函数在 OLED 屏幕上显示字符，可选择字体大小和显示模式。
 *
 * @param x 要显示的字符的水平位置，取值范围为 0 到 127。
 * @param y 字符将在 OLED 屏幕上显示的位置的 y 坐标。它的范围从 0 到 8。
 * @param chr 要在 OLED 屏幕上显示的字符。它可以是任何 ASCII 字符。
 * @param Char_Size 要显示的字符的大小，16 或 12。
 * @param mode mode 1，字符将以反色模式显示，0，字符将以正常模式显示。
 */
void OLED_ShowChar(uint8_t x,		  // x:0~127
				   uint8_t y,		  // y:0~8
				   uint8_t chr,		  // char:要显示的字符
				   uint8_t Char_Size, // size:选择字体 16/12
				   uint8_t mode)	  // mode:1,反白显示;0,正常显示
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; // 得到偏移后的值
	if (x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}
	if (!mode)
	{
		if (Char_Size == 16)
		{
			OLED_Set_Pos(x, y);
			for (i = 0; i < 8; i++)
				OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
			OLED_Set_Pos(x, y + 1);
			for (i = 0; i < 8; i++)
				OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
		}
		else
		{
			OLED_Set_Pos(x, y);
			for (i = 0; i < 6; i++)
				OLED_WR_Byte(F6x8[c][i], OLED_DATA);
		}
	}
	else
	{
		if (Char_Size == 16)
		{
			OLED_Set_Pos(x, y);
			for (i = 0; i < 8; i++)
				OLED_WR_Byte(0xFF - F8X16[c * 16 + i], OLED_DATA);
			OLED_Set_Pos(x, y + 1);
			for (i = 0; i < 8; i++)
				OLED_WR_Byte(0xFF - F8X16[c * 16 + i + 8], OLED_DATA);
		}
		else
		{
			OLED_Set_Pos(x, y);
			for (i = 0; i < 6; i++)
				OLED_WR_Byte(0xFF - F6x8[c][i], OLED_DATA);
		}
	}
}

// m^n函数
/**
 * 该函数计算将给定底数提高到给定指数的结果。
 *
 * @param m m 是一个无符号的 8 位整数，表示幂函数中的基值。
 * @param n 函数“oled_pow”中的参数“n”表示以“m”为底的指数。
 * @return 函数“oled_pow”返回一个“uint32_t”数据类型，它是一个无符号的 32 位整数。
 */
uint32_t oled_pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
		result *= m;
	return result;
}

/**
 * 该函数以指定的位置、大小和显示方式在OLED屏幕上显示一个数字。
 *
 * @param x OLED 屏要显示的数字的水平起始位置，取值范围为 0 到 127。
 * @param y y参数表示要在OLED屏幕上显示的数字的垂直位置,它的范围从 0 到 8。
 * @param num 要显示在 OLED 屏幕上的数字。它应该在 0 到 9 之间。
 * @param len 要显示的位数。
 * @param size2 要显示的字体的大小。它可以是 16 或 12。
 * @param mode 1 为反转显示（黑底白字），0 为正常显示（白底黑字）。
 */
void OLED_ShowNum(uint8_t x,	 // x:0~127
				  uint8_t y,	 // y:0~8
				  uint32_t num,	 // num:0~9
				  uint8_t len,	 // len:要显示的位数
				  uint8_t size2, // size2:选择字体 16/12
				  uint8_t mode)	 // mode:1,反白显示;0,正常显示
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2, mode);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2, mode);
	}
}

/**
 * 该函数在 OLED 屏幕上显示一个字符串，可以选择字符的大小和显示模式。
 *
 * @param x 要在 OLED 屏幕上显示的字符串的起始 x 坐标。它应该是 0 到 127 之间的整数值。
 * @param y 字符串将在 OLED 屏幕上显示的起始位置的 y 坐标。它的范围从 0 到 63。
 * @param chr 指向要在 OLED 屏幕上显示的字符串头指针。
 * @param Char_Size OLED 屏幕上要显示的字符串的长度。
 * @param mode 0 为反转显示，1 为正常显示。
 */
void OLED_ShowString(uint8_t x,			// x:0~127
					 uint8_t y,			// y:0~63
					 uint8_t *chr,		// 字符串头指针
					 uint8_t Char_Size, // 字符串的长度
					 uint8_t mode)		// mode:1,反白显示;0,正常显示
{
	// mode:0,反白显示;1,正常显示
	unsigned char j = 0;
	while (chr[j] != '\0')
	{
		OLED_ShowChar(x, y, chr[j], Char_Size, mode);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}

/**
 * 该函数在 OLED 显示器的指定位置显示一个汉字。
 *
 * @param x OLED 屏幕上要显示的汉字起点的水平位置。它的范围从 0 到 127。
 * @param y y参数表示汉字在OLED显示屏上的垂直位置。它的范围从 0 到 63。
 * @param no HZK 数组中要显示的汉字索引。
 */
// 显示汉字
void OLED_ShowCHinese(uint8_t x,  // x:0~127
					  uint8_t y,  // y:0~63
					  uint8_t no) // no:要显示汉字Hzk的下标
{
	uint8_t t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
		adder += 1;
	}
}
/**
 * 该函数在给定起始坐标和结束坐标以及图像数据的情况下，在 OLED 屏幕上显示 BMP 图像。
 * 显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
 * @param x0 要在 OLED 屏幕上显示的图像的起始 x 坐标。
 * @param y0 BMP 图像将在 OLED 屏幕上显示的区域的起始 y 坐标。
 * @param x1 要显示的图像终点的 x 坐标。
 * @param y1 BMP 图像显示区域终点的 y 坐标。
 * @param BMP OLED 屏幕上要显示的图像的头指针。
 */
void OLED_DrawBMP(unsigned char x0,	   // 要显示图片的起点x坐标
				  unsigned char y0,	   // 要显示图片的起点y坐标
				  unsigned char x1,	   // 要显示图片的终点x坐标
				  unsigned char y1,	   // 要显示图片的终点y坐标
				  unsigned char BMP[]) // 要显示图片的头指针
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

//==============================================================
// 函数名：OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t *p)
// 功能描述：写入一组标准ASCII字符串
// 参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
// 返回：无
//==============================================================
void OLED_P6x8Str(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > 126)
		{
			x = 0;
			y++;
		}
		OLED_Set_Pos(x, y);
		for (i = 0; i < 6; i++)
			OLED_WR_Byte(F6x8[c][i], OLED_DATA);
		x += 6;
		j++;
	}
}

//==============================================================
// 函数名：OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t *p)
// 功能描述：写入一组标准ASCII字符串
// 参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
// 返回：无
//==============================================================
void OLED_P6x8StrW(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > 126)
		{
			x = 0;
			y++;
		}
		OLED_Set_Pos(x, y);
		for (i = 0; i < 6; i++)
			OLED_WR_Byte(0xFF - F6x8[c][i], OLED_DATA);
		x += 6;
		j++;
	}
}

