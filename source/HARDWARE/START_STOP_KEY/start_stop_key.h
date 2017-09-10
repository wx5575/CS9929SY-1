/**
  ******************************************************************************
  * @file    start_stop_key.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   启动复位按键驱动程序
  ******************************************************************************
  */

#ifndef __START_STOP_KEY_H__
#define __START_STOP_KEY_H__

#include "sys_level.h"
#include "bit_banding.h"


#define START_PIN       PFin(11)    ///<启动键位绑定定义
#define STOP_PIN        PBin(2)     ///<复位键位绑定定义
#define SYN_STOP_PIN    PEout(6)    ///<同步复位引脚
#define SYN_START_PIN   PEout(5)    ///<同步启动引脚

#define PIN_H           1///<引脚输出高电平
#define PIN_L           0///<引脚输出低电平


#define TEST_OVER_N     1   //测试未结束
#define TEST_OVER_Y     0   //测试结束

#define ROAD1_TEST_OVER_PIN     PBin(5) ///<第一路测试结束
#define ROAD2_TEST_OVER_PIN     PBin(4) ///<第二路测试结束
#define ROAD3_TEST_OVER_PIN     PGin(15) ///<第三路测试结束
#define ROAD4_TEST_OVER_PIN     PGin(13) ///<第四路测试结束

#define TEST_ALARM_N     1   //测试未报警 当测试结束时就可以作为测试合格信号了
#define TEST_ALARM_Y     0   //测试报警

#define ROAD1_TEST_ALARM_PIN     PBin(8) ///<第一路报警信号
#define ROAD2_TEST_ALARM_PIN     PBin(3) ///<第二路报警信号
#define ROAD3_TEST_ALARM_PIN     PGin(14) ///<第三路报警信号
#define ROAD4_TEST_ALARM_PIN     PGin(12) ///<第四路报警信号

#endif //__START_STOP_KEY_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
