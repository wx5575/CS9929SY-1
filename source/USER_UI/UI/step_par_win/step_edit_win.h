/**
  ******************************************************************************
  * @file    step_edit_win.h
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.13
  * @brief   步骤编辑窗口
  ******************************************************************************
  */
#ifndef __STEP_EDIT_WIN_H__
#define __STEP_EDIT_WIN_H__

extern void create_step_edit_win(int hWin);
extern EDIT_ELE_T *get_mode_edit_ele_inf(UN_STRUCT *step);
extern EDIT_ELE_T *get_range_edit_ele_inf(UN_STRUCT *step);
extern EDIT_ELE_T *get_vol_edit_ele_inf(UN_STRUCT *step);
extern EDIT_ELE_T *get_test_time_edit_ele_inf(UN_STRUCT *step);
extern EDIT_ELE_T *get_lower_edit_ele_inf(UN_STRUCT *step);
extern EDIT_ELE_T *get_upper_edit_ele_inf(UN_STRUCT *step);
extern EDIT_ELE_T *get_auto_shift_edit_ele_inf(UN_STRUCT *step);

#endif //__STEP_EDIT_WIN_H__

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
