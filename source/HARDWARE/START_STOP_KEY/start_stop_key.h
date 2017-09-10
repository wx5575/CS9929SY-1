/**
  ******************************************************************************
  * @file    start_stop_key.h
  * @author  ����
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   ������λ������������
  ******************************************************************************
  */

#ifndef __START_STOP_KEY_H__
#define __START_STOP_KEY_H__

#include "sys_level.h"
#include "bit_banding.h"


#define START_PIN       PFin(11)    ///<������λ�󶨶���
#define STOP_PIN        PBin(2)     ///<��λ��λ�󶨶���
#define SYN_STOP_PIN    PEout(6)    ///<ͬ����λ����
#define SYN_START_PIN   PEout(5)    ///<ͬ����������

#define PIN_H           1///<��������ߵ�ƽ
#define PIN_L           0///<��������͵�ƽ


#define TEST_OVER_N     1   //����δ����
#define TEST_OVER_Y     0   //���Խ���

#define ROAD1_TEST_OVER_PIN     PBin(5) ///<��һ·���Խ���
#define ROAD2_TEST_OVER_PIN     PBin(4) ///<�ڶ�·���Խ���
#define ROAD3_TEST_OVER_PIN     PGin(15) ///<����·���Խ���
#define ROAD4_TEST_OVER_PIN     PGin(13) ///<����·���Խ���

#define TEST_ALARM_N     1   //����δ���� �����Խ���ʱ�Ϳ�����Ϊ���Ժϸ��ź���
#define TEST_ALARM_Y     0   //���Ա���

#define ROAD1_TEST_ALARM_PIN     PBin(8) ///<��һ·�����ź�
#define ROAD2_TEST_ALARM_PIN     PBin(3) ///<�ڶ�·�����ź�
#define ROAD3_TEST_ALARM_PIN     PGin(14) ///<����·�����ź�
#define ROAD4_TEST_ALARM_PIN     PGin(12) ///<����·�����ź�

#endif //__START_STOP_KEY_H__

/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
