/**
  ******************************************************************************
  * @file    dialog_info.h
  * @author  王鑫
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   用户应用程序
  ******************************************************************************
  */
#include "bsp_tft_lcd.h"
#include "LCD_RA8875.h"

#define USE_SMALL_FONT 1

uint16_t g_LcdHeight = 480;			/* 显示屏分辨率-高度 */
uint16_t g_LcdWidth = 800;			/* 显示屏分辨率-宽度 */
uint8_t g_LcdDirection;				/* 显示方向.0，1，2，3 */

void LCD_FSMCConfig(void);

/*
*********************************************************************************************************
*	函 数 名: LCD_GetHeight
*	功能说明: 读取LCD分辨率之高度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

uint16_t LCD_GetHeight(void)
{
	return g_LcdHeight;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetWidth
*	功能说明: 读取LCD分辨率之宽度
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t LCD_GetWidth(void)
{
	return g_LcdWidth;
}
/*
*********************************************************************************************************
*	函 数 名: LCD_PutPixel
*	功能说明: 画1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  : 像素颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	RA8875_PutPixel(_usX, _usY, _usColor);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_GetPixel
*	功能说明: 读取1个像素
*	形    参:
*			_usX,_usY : 像素坐标
*			_usColor  : 像素颜色
*	返 回 值: RGB颜色值
*********************************************************************************************************
*/
uint16_t LCD_GetPixel(uint16_t _usX, uint16_t _usY)
{
	uint16_t usRGB;

	usRGB = RA8875_GetPixel(_usX, _usY);

	return usRGB;
}

/*
*********************************************************************************************************
*	函 数 名: LCD_DrawLine
*	功能说明: 采用 Bresenham 算法，在2点间画一条直线。
*	形    参:
*			_usX1, _usY1 : 起始点坐标
*			_usX2, _usY2 : 终止点Y坐标
*			_usColor     : 颜色
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{
	RA8875_DrawLine(_usX1 , _usY1 , _usX2, _usY2 , _usColor);
}


/*
*********************************************************************************************************
*	函 数 名: LCD_DispStr
*	功能说明: 在LCD指定坐标（左上角）显示一个字符串
*	形    参：
*		_usX : X坐标，对于3.0寸宽屏，范围为【0 - 399】
*		_usY : Y坐标，对于3.0寸宽屏，范围为 【0 - 239】
*		_ptr  : 字符串指针
*		_tFont : 字体结构体，包含颜色、背景色(支持透明)、字体代码、文字间距等参数
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint8_t buf[32 * 32 / 8];	/* 最大支持24点阵汉字 */
	uint8_t m, width;
	uint8_t font_width,font_height, font_bytes, one_line_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;
	const uint8_t *pHzDot;
    
	/* 如果字体结构为空指针，则缺省按16点阵 */
	if (_tFont->usFontCode == FC_ST_12)
	{
		font_height = 12;
		font_width = 12;
		font_bytes = 24;
		pAscDot = g_Ascii12;
		pHzDot = g_Hz12;
	}
    /* 24 */
	else if (_tFont->usFontCode == FC_ST_24)
    {
		/* 缺省是 24 点阵 */
		font_height = 24;
		font_width = 24;
		font_bytes = 96;
		pAscDot = g_Ascii24;
		pHzDot = g_Hz24;
    }
    /* 29 */
	else if (_tFont->usFontCode == FC_ST_29)
    {
		/* 缺省是 29 点阵 */
		font_height = 29;
		font_width = 29;
		font_bytes = 116;
		pAscDot = g_Ascii29;
		pHzDot = g_Hz29;
    }
    /* 32 */
	else if (_tFont->usFontCode == FC_ST_32)
    {
		/* 缺省是 32 点阵 */
		font_height = 32;
		font_width = 32;
		font_bytes = 128;
		pAscDot = g_Ascii32;
		pHzDot = g_Hz29;
    }
	else
	{
		/* 缺省是16点阵 */
		font_height = 16;
		font_width = 16;
		font_bytes = 32;
		pAscDot = g_Ascii16;
		pHzDot = g_Hz16;
	}

	/* 开始循环处理字符 */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* 读取字符串数据， 该数据可能是ascii代码，也可能汉字代码的高字节 */
		if (code1 < 0x80)
		{
			/* 将ascii字符点阵复制到buf */
			memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
			width = font_width / 2;
            width = (width % 8 == 0? width : (width + 8) / 8 * 8);
            one_line_bytes = width / 8;
		}
		else
		{
			width = font_width;
            width = (width % 8 == 0? width : (width + 8) / 8 * 8);
            one_line_bytes = width / 8;
            
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}

			/* 计算16点阵汉字点阵地址
				ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
				;
			*/
			#ifdef USE_SMALL_FONT
				m = 0;
                
				while(1)
				{
					address = m * (font_bytes + 2);
					m++;
					if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
					{
						address += 2;
						memcpy(buf, &pHzDot[address], font_bytes);
						break;
					}
					else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
					{
						/* 字库搜索完毕，未找到，则填充全FF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
				}
			#else	/* 用全字库 */
				/* 此处需要根据字库文件存放位置进行修改 */
				address = ((code1-0xa1) * 94 + (code2-0xa1)) * font_bytes + HZK16_ADDR;
				memcpy(buf, (const uint8_t *)address, font_bytes);
			#endif

				width = font_width;
		}
        
		y = _usY;
		/* 开始刷LCD */
		for (m = 0; m < font_height; m++)	/* 字符高度 */
		{
			x = _usX;
			for (i = 0; i < width; i++)	/* 字符宽度 */
			{
				if ((buf[m * one_line_bytes + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					LCD_PutPixel(x, y, _tFont->usTextColor);	/* 设置像素颜色为文字色 */
				}
				else
				{
					LCD_PutPixel(x, y, _tFont->usBackColor);	/* 设置像素颜色为文字背景色 */
				}
                
//				if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
//				{
//					LCD_PutPixel(x, y, _tFont->usTextColor);	/* 设置像素颜色为文字色 */
//				}
//				else
//				{
//					LCD_PutPixel(x, y, _tFont->usBackColor);	/* 设置像素颜色为文字背景色 */
//				}

				x++;
			}
			y++;
		}

		if (_tFont->usSpace > 0)
		{
			/* 如果文字底色按_tFont->usBackColor，并且字间距大于点阵的宽度，那么需要在文字之间填充(暂时未实现) */
		}
		_usX += width + _tFont->usSpace;	/* 列地址递增 */
		_ptr++;			/* 指向下一个字符 */
	}
}
