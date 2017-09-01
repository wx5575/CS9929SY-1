/**
  ******************************************************************************
  * @file    dialog_info.h
  * @author  ����
  * @version V0.0.1
  * @date    2017.4.18
  * @brief   �û�Ӧ�ó���
  ******************************************************************************
  */
#include "bsp_tft_lcd.h"
#include "LCD_RA8875.h"

#define USE_SMALL_FONT 1

uint16_t g_LcdHeight = 480;			/* ��ʾ���ֱ���-�߶� */
uint16_t g_LcdWidth = 800;			/* ��ʾ���ֱ���-��� */
uint8_t g_LcdDirection;				/* ��ʾ����.0��1��2��3 */

void LCD_FSMCConfig(void);

/*
*********************************************************************************************************
*	�� �� ��: LCD_GetHeight
*	����˵��: ��ȡLCD�ֱ���֮�߶�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/

uint16_t LCD_GetHeight(void)
{
	return g_LcdHeight;
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_GetWidth
*	����˵��: ��ȡLCD�ֱ���֮���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t LCD_GetWidth(void)
{
	return g_LcdWidth;
}
/*
*********************************************************************************************************
*	�� �� ��: LCD_PutPixel
*	����˵��: ��1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  : ������ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor)
{
	RA8875_PutPixel(_usX, _usY, _usColor);
}

/*
*********************************************************************************************************
*	�� �� ��: LCD_GetPixel
*	����˵��: ��ȡ1������
*	��    ��:
*			_usX,_usY : ��������
*			_usColor  : ������ɫ
*	�� �� ֵ: RGB��ɫֵ
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
*	�� �� ��: LCD_DrawLine
*	����˵��: ���� Bresenham �㷨����2��仭һ��ֱ�ߡ�
*	��    ��:
*			_usX1, _usY1 : ��ʼ������
*			_usX2, _usY2 : ��ֹ��Y����
*			_usColor     : ��ɫ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor)
{
	RA8875_DrawLine(_usX1 , _usY1 , _usX2, _usY2 , _usColor);
}


/*
*********************************************************************************************************
*	�� �� ��: LCD_DispStr
*	����˵��: ��LCDָ�����꣨���Ͻǣ���ʾһ���ַ���
*	��    �Σ�
*		_usX : X���꣬����3.0���������ΧΪ��0 - 399��
*		_usY : Y���꣬����3.0���������ΧΪ ��0 - 239��
*		_ptr  : �ַ���ָ��
*		_tFont : ����ṹ�壬������ɫ������ɫ(֧��͸��)��������롢���ּ��Ȳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void LCD_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	uint32_t i;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint8_t buf[32 * 32 / 8];	/* ���֧��24������ */
	uint8_t m, width;
	uint8_t font_width,font_height, font_bytes, one_line_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;
	const uint8_t *pHzDot;
    
	/* �������ṹΪ��ָ�룬��ȱʡ��16���� */
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
		/* ȱʡ�� 24 ���� */
		font_height = 24;
		font_width = 24;
		font_bytes = 96;
		pAscDot = g_Ascii24;
		pHzDot = g_Hz24;
    }
    /* 29 */
	else if (_tFont->usFontCode == FC_ST_29)
    {
		/* ȱʡ�� 29 ���� */
		font_height = 29;
		font_width = 29;
		font_bytes = 116;
		pAscDot = g_Ascii29;
		pHzDot = g_Hz29;
    }
    /* 32 */
	else if (_tFont->usFontCode == FC_ST_32)
    {
		/* ȱʡ�� 32 ���� */
		font_height = 32;
		font_width = 32;
		font_bytes = 128;
		pAscDot = g_Ascii32;
		pHzDot = g_Hz29;
    }
	else
	{
		/* ȱʡ��16���� */
		font_height = 16;
		font_width = 16;
		font_bytes = 32;
		pAscDot = g_Ascii16;
		pHzDot = g_Hz16;
	}

	/* ��ʼѭ�������ַ� */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* ��ȡ�ַ������ݣ� �����ݿ�����ascii���룬Ҳ���ܺ��ִ���ĸ��ֽ� */
		if (code1 < 0x80)
		{
			/* ��ascii�ַ������Ƶ�buf */
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

			/* ����16�����ֵ����ַ
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
						/* �ֿ�������ϣ�δ�ҵ��������ȫFF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
				}
			#else	/* ��ȫ�ֿ� */
				/* �˴���Ҫ�����ֿ��ļ����λ�ý����޸� */
				address = ((code1-0xa1) * 94 + (code2-0xa1)) * font_bytes + HZK16_ADDR;
				memcpy(buf, (const uint8_t *)address, font_bytes);
			#endif

				width = font_width;
		}
        
		y = _usY;
		/* ��ʼˢLCD */
		for (m = 0; m < font_height; m++)	/* �ַ��߶� */
		{
			x = _usX;
			for (i = 0; i < width; i++)	/* �ַ���� */
			{
				if ((buf[m * one_line_bytes + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					LCD_PutPixel(x, y, _tFont->usTextColor);	/* ����������ɫΪ����ɫ */
				}
				else
				{
					LCD_PutPixel(x, y, _tFont->usBackColor);	/* ����������ɫΪ���ֱ���ɫ */
				}
                
//				if ((buf[m * ((2 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
//				{
//					LCD_PutPixel(x, y, _tFont->usTextColor);	/* ����������ɫΪ����ɫ */
//				}
//				else
//				{
//					LCD_PutPixel(x, y, _tFont->usBackColor);	/* ����������ɫΪ���ֱ���ɫ */
//				}

				x++;
			}
			y++;
		}

		if (_tFont->usSpace > 0)
		{
			/* ������ֵ�ɫ��_tFont->usBackColor�������ּ����ڵ���Ŀ�ȣ���ô��Ҫ������֮�����(��ʱδʵ��) */
		}
		_usX += width + _tFont->usSpace;	/* �е�ַ���� */
		_ptr++;			/* ָ����һ���ַ� */
	}
}
