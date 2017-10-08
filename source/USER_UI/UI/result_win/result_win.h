/**
  ******************************************************************************
  * @file    result_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   结果管理窗口
  ******************************************************************************
  */

#ifndef __RESULT_WIN_H__
#define __RESULT_WIN_H__

typedef enum{
    RESULT_WIN_PRODUCT_NUM,
    RESULT_WIN_NUM01,
    RESULT_WIN_NUM02,
    RESULT_WIN_NUM03,
    RESULT_WIN_NUM04,
    RESULT_WIN_NUM05,
    RESULT_WIN_NUM06,
    RESULT_WIN_NUM07,
    RESULT_WIN_NUM08,
    RESULT_WIN_NUM09,
    RESULT_WIN_NUM10,
    RESULT_WIN_NUM11,
    RESULT_WIN_NUM12,
    RESULT_WIN_NUM13,
    RESULT_WIN_NUM14,
    RESULT_WIN_NUM15,
    RESULT_WIN_NUM16,

    RESULT_WIN_TEST_RES_INF,
    RESULT_WIN_TEST_RES_1,
    RESULT_WIN_TEST_RES_2,
    RESULT_WIN_TEST_RES_3,
    RESULT_WIN_TEST_RES_4,
    RESULT_WIN_TEST_RES_5,
    RESULT_WIN_TEST_RES_6,
    RESULT_WIN_TEST_RES_7,
    RESULT_WIN_TEST_RES_8,
    RESULT_WIN_SETTING_PAR,
    RESULT_WIN_SETTING_PAR_C,
    RESULT_WIN_TEST_DATA,
    RESULT_WIN_TEST_DATA_C,
}RESULT_WIN_TEXT_ELE_INDEX;

extern void update_cur_result_inf(uint32_t result_num);

#endif //__RESULT_WIN_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
