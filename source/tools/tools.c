/**
  ******************************************************************************
  * @file    tools.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   软件工具函数
  ******************************************************************************
  */
#include "tools.h"
#include "string.h"
#include "stdlib.h"


/**
  * @brief  10的幂
  * @param  [in] n 10的n次幂
  * @retval 结果
  */
uint32_t ten_power(u8 n)
{
	u8 i;
	uint32_t temp = 1;
	for(i = 0; i < n; i++)
	{
		temp *= 10;
	}
	return temp;
}
float myatof(uint8_t *buf)
{
    uint8_t t_buf[2][100];
    int32_t i = 0;
    int32_t j = 0;
    uint8_t k = 0;
    uint8_t len = 0;
    float val_1;
    float val_2;
    float fol;
    
    memset(t_buf, 0, sizeof(t_buf));
    len = strlen((const char*)buf);
    
    if(len >= sizeof(t_buf[0]))
    {
        len = sizeof(t_buf[0] - 1);
    }
    
    for(i = 0; i < len; i++)
    {
        if(buf[i] == '.')
        {
            if(j == 1)
            {
                break;
            }
            
            j++;
            k = 0;
            continue;
        }
        else if(buf[i] < '0' && buf[i] > '9')
        {
            break;
        }
        
        t_buf[j][k++] = buf[i];
    }
    
    val_1 = atoi((const char*)t_buf[0]);
    val_2 = atoi((const char*)t_buf[1]);
    
    fol = val_2;
    fol /= ten_power(k);
    
    return (fol + val_1);
}
/**
  * @brief  自定义格式化字符串,用来取代对sprintf的，因为sprintf不安全
  * @param  [out] buf 输出字符串
  * @param  [in] unit 输入的参数单位后缀
  * @param  [in] format 显示格式
                例:153 表示数据是浮点数 1带前导零,0不带前导零 
                最大长度为5包括小数点，3位小数
  * @param  [in] value 数据
  * @retval 无
  */
void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t format, uint32_t value)
{
    uint8_t dec_n = (format / 1) % 10;/* 小数位 */
    uint8_t lon    = (format / 10) % 10;/* 长度 */
    uint8_t app_0  = (format / 100) % 10;/* 前导零标志 */
    uint8_t int_n = 0;
    uint8_t temp = 0;
    uint8_t temp_l = 0;
    int32_t i = 0;
    int32_t j = 0;
    
    if(buf == NULL)
    {
        return;
    }
    
    if(dec_n > 0 && lon - dec_n < 2)
    {
        strcpy((char*)buf, "0.000");
        if(unit != NULL)
        {
            strcat((char*)buf, (const char*)unit);
        }
        return;
    }
    
    int_n =  lon - dec_n - (dec_n > 0);
    
    for(i = 0, j = 0; i < int_n; i++)
    {
        if(dec_n > 0)
        {
            temp = (value / ten_power(lon - 2 - i)) % 10;
        }
        else
        {
            temp = (value / ten_power(lon - 1 - i)) % 10;
        }
        
        /* 去除前导零 */
        if(app_0 == 0)
        {
            /* 最后一个整数位 */
            if(int_n - i == 1)
            {
                buf[j++] = temp + '0';
            }
            /* 不是最后一个整数位 */
            else
            {
                if(temp || j)
                {
                    buf[j++] = temp + '0';
                }
            }
        }
        /* 不去除前导零 */
        else
        {
            buf[j++] = temp + '0';
        }
    }
    
    if(dec_n)
    {
        buf[j++] = '.';
    }
    
    temp_l = j;
    
    /* 小数部分 */
    for(i = 0; i < dec_n; i++)
    {
        temp = (value / ten_power(dec_n - i - 1)) % 10;
        buf[i + temp_l] = temp + '0';
    }
    
    buf[i + temp_l] = 0;
    
    /* 追加后缀信息 */
    if(unit != NULL)
    {
        strcat((char*)buf, (const char*)unit);
    }
}
/**
  * @brief  使用指定字符来替换前导零
  * @param  [in] buf 字符串
  * @param  [in] ch 指定的字符
  * @retval 结果
  */
uint8_t *dis_str_prev_zero(uint8_t *buf, uint8_t ch)
{
    uint8_t lon = 0;
    int32_t i = 0;
    
    lon = strlen((const char*)buf);
    
    for(i = 0; i < lon; i++)
    {
        if(buf[i] == '0')
        {
            if(buf[i + 1] < '0' || buf[i + 1] > '9')
            {
                break;
            }
            else
            {
                buf[i] = ch;
            }
        }
        else
        {
            break;
        }
    }
    
    return buf;
}
void mysprintf_2(uint8_t *buf, const uint8_t* unit, uint8_t format, uint32_t value)
{
    mysprintf(buf, unit, format, value);
    dis_str_prev_zero(buf, ' ');
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
