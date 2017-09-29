/**
  ******************************************************************************
  * @file    coded_disc.h
  * @author  ÍõöÎ
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   ÂëÅÌÇý¶¯³ÌÐò
  ******************************************************************************
  */

#ifndef __CODED_DISC_H__
#define __CODED_DISC_H__



extern void coded_disc_init(void);
extern void register_coded_disc_send_msg_fun(void(*fun)(uint32_t *));
extern void scan_coded_disc(void);

#endif //__CODED_DISC_H__

/************************ (C) COPYRIGHT 2017 ³¤Ê¢ÒÇÆ÷ *****END OF FILE****/
