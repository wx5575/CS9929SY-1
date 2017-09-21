/**
  ******************************************************************************
  * @file    rtc_config.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   RTCʱ�ӳ�ʼ��
  ******************************************************************************
  */
#include "rtc_config.h"
#include "stdio.h"
#include "string.h"
#include "time.h"



/**
  * @brief  ��ȡRTC��
  * @param  ��
  * @retval ��-����
  */
uint16_t get_rtc_year(void)
{
	RTC_DateTypeDef rtc_date;
    
	RTC_GetDate(RTC_Format_BIN, &rtc_date);
    
	return rtc_date.RTC_Year + 2000;
}

/**
  * @brief  ��ȡRTC��
  * @param  ��
  * @retval ��-����
  */
uint16_t get_rtc_month(void)
{
	RTC_DateTypeDef rtc_date;
	
	RTC_GetDate(RTC_Format_BIN, &rtc_date);
    
	return rtc_date.RTC_Month;
}

/**
  * @brief  ��ȡRTC��
  * @param  ��
  * @retval ��-����
  */
uint16_t get_rtc_day(void)
{
	RTC_DateTypeDef rtc_date;
    
	RTC_GetDate(RTC_Format_BIN, &rtc_date);
    
	return rtc_date.RTC_Date;
}
/**
  * @brief  ��ȡRTCʱ
  * @param  ��
  * @retval ʱ-����
  */
uint16_t get_rtc_hour(void)
{
	RTC_TimeTypeDef rtc_time;
	
	RTC_GetTime(RTC_Format_BIN, &rtc_time);
    
	return rtc_time.RTC_Hours;
}
/**
  * @brief  ��ȡRTC��
  * @param  ��
  * @retval ��-����
  */
uint16_t get_rtc_minute(void)
{
	RTC_TimeTypeDef rtc_time;
	
	RTC_GetTime(RTC_Format_BIN, &rtc_time);
	
	return rtc_time.RTC_Minutes;
}
/**
  * @brief  ��ȡRTC��
  * @param  ��
  * @retval ��-����
  */
uint16_t get_rtc_second(void)
{
	RTC_TimeTypeDef rtc_time;
	
	RTC_GetTime(RTC_Format_BIN, &rtc_time);
	
	return rtc_time.RTC_Seconds;
}

/**
  * @brief  ��ȡRTCʱ���ַ���
  * @param  [out] buf ���ʱ���ַ���
  * @retval RTCʱ���ַ���
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
  * @brief  ����RTCʱ���ַ���
  * @param  [in] year month day hours minutes seconds ������ʱ����
  * @retval RTCʱ���ַ���
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
  * @brief  RTC ��ʼ������
  * @param  ��
  * @retval ��
  */
static int RTC_Configuration(void)
{
	uint8_t redo_times = 0;
	
	RTC_InitTypeDef  RTC_InitStructure;
	RTC_TimeTypeDef  RTC_TimeStructure;
	RTC_DateTypeDef  RTC_DateStructure;
	
    /* ʹ��PWRʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
 	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
	
    
    
 	RTC_DeInit();
	/* �������RTC */
	PWR_BackupAccessCmd(ENABLE);
	
	/* ʹ��LSE����  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* �ȴ����� */
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
		if(++redo_times > 1000)
		{
			return -1;
		}
	}
	
	/* ѡ��RTCʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* ʹ��RTCʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* �ȴ�RTC APB�Ĵ���ͬ�� */
	RTC_WaitForSynchro();
	
	/* ����RTC���ݼĴ����ͷ�Ƶ��  */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	RTC_Init(&RTC_InitStructure);
	
	
	/* ���������պ����� */
	RTC_DateStructure.RTC_Year = 0x14;
	RTC_DateStructure.RTC_Month = RTC_Month_July;
	RTC_DateStructure.RTC_Date = 0x07;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	/* ����ʱ���룬�Լ���ʾ��ʽ */
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = 0x12;
	RTC_TimeStructure.RTC_Minutes = 0x00;
	RTC_TimeStructure.RTC_Seconds = 0x00; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
	
	/* ���ñ��ݼĴ�������ʾ�Ѿ����ù�RTC */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0xA5A5);
	
    return 0;
}


/**
  * @brief  RTC ������ʼ������
  * @param  ��
  * @retval ��
  */
void rtc_init(void)
{
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//ʹ��PWRʱ��
//    PWR_BackupAccessCmd(ENABLE);//ʹ�ܺ󱸼Ĵ�������
    
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
  * @brief  RTC �жϷ�����
  * @param  ��
  * @retval ��
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

/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
