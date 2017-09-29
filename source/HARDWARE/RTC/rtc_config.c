/**
  ******************************************************************************
  * @file    rtc_config.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   RTC时钟初始化
  ******************************************************************************
  */
#include "rtc_config.h"
#include "stdio.h"
#include "string.h"
#include "time.h"



/**
  * @brief  获取RTC年
  * @param  无
  * @retval 年-整数
  */
uint16_t get_rtc_year(void)
{
	RTC_DateTypeDef rtc_date;
    
	RTC_GetDate(RTC_Format_BIN, &rtc_date);
    
	return rtc_date.RTC_Year + 2000;
}

/**
  * @brief  获取RTC月
  * @param  无
  * @retval 月-整数
  */
uint16_t get_rtc_month(void)
{
	RTC_DateTypeDef rtc_date;
	
	RTC_GetDate(RTC_Format_BIN, &rtc_date);
    
	return rtc_date.RTC_Month;
}

/**
  * @brief  获取RTC日
  * @param  无
  * @retval 日-整数
  */
uint16_t get_rtc_day(void)
{
	RTC_DateTypeDef rtc_date;
    
	RTC_GetDate(RTC_Format_BIN, &rtc_date);
    
	return rtc_date.RTC_Date;
}
/**
  * @brief  获取RTC时
  * @param  无
  * @retval 时-整数
  */
uint16_t get_rtc_hour(void)
{
	RTC_TimeTypeDef rtc_time;
	
	RTC_GetTime(RTC_Format_BIN, &rtc_time);
    
	return rtc_time.RTC_Hours;
}
/**
  * @brief  获取RTC分
  * @param  无
  * @retval 分-整数
  */
uint16_t get_rtc_minute(void)
{
	RTC_TimeTypeDef rtc_time;
	
	RTC_GetTime(RTC_Format_BIN, &rtc_time);
	
	return rtc_time.RTC_Minutes;
}
/**
  * @brief  获取RTC秒
  * @param  无
  * @retval 秒-整数
  */
uint16_t get_rtc_second(void)
{
	RTC_TimeTypeDef rtc_time;
	
	RTC_GetTime(RTC_Format_BIN, &rtc_time);
	
	return rtc_time.RTC_Seconds;
}

/**
  * @brief  获取RTC时间字符串
  * @param  [out] buf 输出时间字符串
  * @retval RTC时间字符串
  */
uint8_t* get_time_str(uint8_t *buf)
{
	static uint8_t time_buf[30] = {0};
	RTC_DateTypeDef rtc_date;
	RTC_TimeTypeDef rtc_time;
	
	RTC_GetTime(RTC_Format_BIN, &rtc_time);
	RTC_GetDate(RTC_Format_BIN, &rtc_date);
	
	sprintf((char*)time_buf,"%04d-%02d-%02d %02d:%02d:%02d",
			rtc_date.RTC_Year + 2000, rtc_date.RTC_Month, rtc_date.RTC_Date,
			rtc_time.RTC_Hours, rtc_time.RTC_Minutes, rtc_time.RTC_Seconds);
	
	if(buf != NULL)
	{
		strcpy((char*)buf, (const char*)time_buf);
	}
	
	return time_buf;
}

/**
  * @brief  设置RTC时间字符串
  * @param  [in] year month day hours minutes seconds 年月日时分秒
  * @retval RTC时间字符串
  */
void rtc_set_time(uint32_t year, uint32_t month, uint32_t day,
		uint32_t hours, uint32_t minutes, uint32_t seconds)
{
	RTC_DateTypeDef rtc_date;
	RTC_TimeTypeDef rtc_time;
	
	rtc_date.RTC_Year		= year - 2000;
	rtc_date.RTC_Month		= month;
	rtc_date.RTC_Date		= day;
	rtc_date.RTC_WeekDay	= 0;
	rtc_time.RTC_Hours		= hours;
	rtc_time.RTC_Minutes	= minutes;
	rtc_time.RTC_Seconds	= seconds;
	
	RTC_SetDate(RTC_Format_BIN,&rtc_date);
	RTC_SetTime(RTC_Format_BIN,&rtc_time);
}

/**
  * @brief  RTC 初始化配置
  * @param  无
  * @retval 无
  */
static int RTC_Configuration(void)
{
	uint8_t redo_times = 0;
	
	RTC_InitTypeDef  RTC_InitStructure;
	RTC_TimeTypeDef  RTC_TimeStructure;
	RTC_DateTypeDef  RTC_DateStructure;
	
    /* 使能PWR时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
	
    
    
 	RTC_DeInit();
	/* 允许访问RTC */
	PWR_BackupAccessCmd(ENABLE);
	
	/* 使能LSE振荡器  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* 等待就绪 */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
		if(++redo_times > 1000)
		{
			return -1;
		}
	}
	
	/* 选择RTC时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* 使能RTC时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待RTC APB寄存器同步 */
	RTC_WaitForSynchro();
	
	/* 配置RTC数据寄存器和分频器  */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
	
	
	/* 设置年月日和星期 */
	RTC_DateStructure.RTC_Year = 0x14;
	RTC_DateStructure.RTC_Month = RTC_Month_July;
	RTC_DateStructure.RTC_Date = 0x07;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	/* 设置时分秒，以及显示格式 */
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = 0x12;
	RTC_TimeStructure.RTC_Minutes = 0x00;
	RTC_TimeStructure.RTC_Seconds = 0x00; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
	
	/* 配置备份寄存器，表示已经设置过RTC */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0xA5A5);
	
    return 0;
}


uint32_t get_rtc_time(void)
{
    uint32_t t_time = 0;
	RTC_TimeTypeDef rtc_time;
	
	RTC_GetTime(RTC_Format_BIN, &rtc_time);
    
    t_time |= (rtc_time.RTC_Seconds % 10) << 0;
    t_time |= (rtc_time.RTC_Seconds / 10) << 4;
    t_time |= (rtc_time.RTC_Minutes % 10) << 8;
    t_time |= (rtc_time.RTC_Minutes / 10) << 12;
    t_time |= (rtc_time.RTC_Hours % 10) << 16;
    t_time |= (rtc_time.RTC_Hours / 10) << 20;
    
    return t_time;
}
uint32_t get_rtc_data(void)
{
    uint32_t t_date = 0;
    uint32_t year = 0;
	RTC_DateTypeDef rtc_date;
    
	RTC_GetDate(RTC_Format_BIN, &rtc_date);
    
    t_date |= (rtc_date.RTC_Date % 10) << 0;
    t_date |= (rtc_date.RTC_Date / 10) << 4;
    t_date |= (rtc_date.RTC_Month % 10) << 8;
    t_date |= (rtc_date.RTC_Month / 10) << 12;
    year = rtc_date.RTC_Year + 2000;
    t_date |= ((year / 1) % 10) << 16;
    t_date |= ((year / 10) % 10) << 20;
    t_date |= ((year / 100) % 10) << 24;
    t_date |= ((year / 1000) % 10) << 28;
    
    return t_date;
}

void turn_rtc_time_str(uint32_t t_time, uint8_t* time)
{
    if(time == NULL)
    {
        return;
    }
    
    sprintf((char*)time, "%d%d:%d%d:%d%d",
        (t_time >> 20) & 0xF,
        (t_time >> 16) & 0xF,
        
        (t_time >> 12) & 0xF,
        (t_time >> 8) & 0xF,
        
        (t_time >> 4) & 0xF,
        (t_time >> 0) & 0xF);
}
void turn_rtc_date_str(uint32_t t_date, uint8_t* date)
{
    uint32_t year = 0;
    
    if(date == NULL)
    {
        return;
    }
    
    sprintf((char*)date, "%d%d%d%d-%d%d-%d%d",
        (t_date >> 28) & 0xF,
        (t_date >> 24) & 0xF,
        (t_date >> 20) & 0xF,
        (t_date >> 16) & 0xF,
        
        (t_date >> 12) & 0xF,
        (t_date >> 8) & 0xF,
        
        (t_date >> 4) & 0xF,
        (t_date >> 0) & 0xF);
}

/**
  * @brief  RTC 开机初始化配置
  * @param  无
  * @retval 无
  */
void rtc_init(void)
{
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
//    PWR_BackupAccessCmd(ENABLE);//使能后备寄存器访问
    
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0xA5A5)
    {
        if ( RTC_Configuration() != 0)
        {
            return ;
        }
    }
    else
    {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		PWR_BackupAccessCmd(ENABLE);
		RTC_WaitForSynchro();
		RTC_ClearFlag(RTC_FLAG_ALRAF);
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		RTC_ClearITPendingBit(RTC_IT_ALRB);
// 		EXTI_ClearITPendingBit(EXTI_Line17);
// 		EXTI_ClearITPendingBit(EXTI_Line22);
		
		RTC->WPR = 0XCA;
		RTC->WPR = 0X53;
		RTC->CR = 0;
		RTC->WPR = 0XFF;
    }
}

/**
  * @brief  RTC 中断服务函数
  * @param  无
  * @retval 无
  */
void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
	{
// 		rt_mb_send(&screen_mb, UPDATE_STATUS | STATUS_TIME_EVENT);
		RTC_ClearITPendingBit(RTC_IT_WUT);
// 		EXTI_ClearITPendingBit(EXTI_Line22);
	} 
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
