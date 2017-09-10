/**
  ******************************************************************************
  * @file    running_test.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.6.23
  * @brief   运行测试
  ******************************************************************************
  */

#include "cs99xx_struct.h"

#ifdef   RUNNING_GLOBALS
#define  RUNNING_EXT
#else
#define  RUNNING_EXT  extern
#endif

#define     TABLE_VALUE_NULL                ((uint16_t)0xffff)

RUNNING_EXT uint8_t cur_status;///<当前测试状态
RUNNING_EXT uint8_t cur_range;///<当前档位

/* 存放AD采集的电压和电流 */
RUNNING_EXT uint16_t sample_vol;         /* 采样电压 */
RUNNING_EXT uint16_t sample_cur;         /* 采样电流 */
RUNNING_EXT float cur_ad_vol;            /* 采样电流 */
RUNNING_EXT uint16_t adc_value[2];       /* 采样电流 */

RUNNING_EXT uint8_t output_buf[10];      /* 输出采集值 */
RUNNING_EXT uint8_t loop_buf[10];        /* 回路采集值 */
RUNNING_EXT uint8_t real_buf[10];        /* 真实电流采集值 */
RUNNING_EXT uint8_t time_buf[10];        /* 测试时间 */

RUNNING_EXT float vol_ave;               /* 电压 */
RUNNING_EXT float cur_ave;               /* 电流 */
RUNNING_EXT float res_ave;               /* 电阻 */
RUNNING_EXT float real_ave;              /* 真实电流 */
RUNNING_EXT float cap_ave;               /* 电容 */
RUNNING_EXT uint16_t g_ad_dog;			 /* ad看门狗 */

RUNNING_EXT float err_vol_bak; /* 报警时电压值 */
RUNNING_EXT float err_cur_bak; /* 报警时电流值 */
RUNNING_EXT float err_res_bak; /* 报警时电阻备份 */
RUNNING_EXT float err_real_bak;/* 报警时真实电流备份 */
RUNNING_EXT float err_cap_bak; /* 报警时电容备份 */

RUNNING_EXT float ir_15s_res;			 /* IR测试 15s时的电阻值 */
RUNNING_EXT float ir_60s_res;			 /* IR测试 60s时的电阻值 */
RUNNING_EXT float ir_absorptance;		 /* IR测试吸收比 */
RUNNING_EXT uint32_t gear_up_limit;      /* ir自动换挡关闭后的上限值 */
RUNNING_EXT uint32_t g_ir_dly;           /* ir换档延时 */
RUNNING_EXT uint16_t g_plc_dly;          /* PLC延时关闭 */
RUNNING_EXT void (*test_irq_fun) (void);
RUNNING_EXT void (*cs99xx_test_fun) (void);

RUNNING_EXT uint32_t uart_err_num;       /* 出口通信失败的次数 */ //load_data

RUNNING_EXT	__IO uint32_t g_test_time;   /* 测试时间片 在定时器中断服务程序中被自加 */
RUNNING_EXT	__IO uint16_t g_dis_time;    /* 用于显示当前测试阶段的计时时间 */
RUNNING_EXT	uint32_t g_buzzer_time;	     /* 蜂鸣器响的长度 */
RUNNING_EXT	uint32_t g_cylinder_ctrl_t;	     /* 气缸控制状态延时 */

RUNNING_EXT uint16_t dly_t;              /* 输出延时 */
RUNNING_EXT uint16_t zeo_t;              /* 第〇阶段的累计时间 */
RUNNING_EXT uint16_t one_t;              /* 第一阶段的累计时间 */
RUNNING_EXT uint16_t two_t;              /* 第二阶段的累计时间 */
RUNNING_EXT uint16_t thr_t;              /* 第三阶段的累计时间 */
RUNNING_EXT uint16_t for_t;              /* 第四阶段的累计时间 */
RUNNING_EXT uint16_t fiv_t;              /* 第五阶段的累计时间 */
RUNNING_EXT uint16_t tes_t;              /* 当前步测试时间 */
RUNNING_EXT uint16_t ris_t;              /* 上升时间 */
RUNNING_EXT uint16_t sta_t;              /* 稳压时间 */
RUNNING_EXT uint16_t fal_t;              /* 下降时间 */
RUNNING_EXT uint16_t int_t;              /* 间隔时间 */
RUNNING_EXT uint16_t cha_t;              /* 缓变时间 */

RUNNING_EXT uint16_t cur_step;           /* 当前步的步号 */
RUNNING_EXT uint8_t cur_mode;            /* 当前步的模式 */
RUNNING_EXT uint8_t cur_offset_result;   /* 当前步的偏移结果 */
RUNNING_EXT uint8_t cur_getc_result;     /* 当前步的获取电容结果 */
RUNNING_EXT uint8_t cur_t_time;          /* 测试时间 */
RUNNING_EXT uint8_t cur_status;          /* 当前状态 */

RUNNING_EXT CS_UNIT_ENUM cur_vol_unit;        /* 当前电压单位 */
RUNNING_EXT CS_UNIT_ENUM cur_cur_unit;        /* 当前电流单位 */
RUNNING_EXT CS_UNIT_ENUM cur_real_unit;       /* 当前真实电流单位 */

RUNNING_EXT uint8_t cur_cylinder_ctrl_status;          /* 当前气缸控制状态定制机专用 */
RUNNING_EXT uint8_t cur_status_plc;      /* 当前状态 */
RUNNING_EXT uint8_t cur_plc_err_st;		 /* PLC报警状态 */
RUNNING_EXT uint8_t cur_comm_bps;        /* 当前的通信波特率 */
RUNNING_EXT uint16_t updata_comm_flag;    /* 更新通信标志 */

RUNNING_EXT uint16_t cur_vol;            /* 当前步的电压值 */
RUNNING_EXT float cur_cap;               /* 当前步的电容值 */
RUNNING_EXT uint16_t cur_open_ratio;     /* 当前步的开路报警值 */
RUNNING_EXT uint16_t cur_short_ratio;    /* 当前步的短路报警值 */
RUNNING_EXT uint8_t cur_auto;            /* 当前ir 自动换挡 */
RUNNING_EXT int8_t steps_con;            /* 步间连续 */
RUNNING_EXT int8_t steps_pass;           /* 步间PASS */
RUNNING_EXT uint8_t cur_gear;            /* 当前步电流档位 */
RUNNING_EXT uint8_t cur_gear_max;        /* 当前步电流档位最大值 */
RUNNING_EXT uint8_t cur_gear_min;        /* 当前步电流档位最小值 */
RUNNING_EXT uint8_t cur_soft_gear;       /* 当前IR软档位 */
RUNNING_EXT uint8_t vol_segment;         /* 电压段 0 10-500v 1 100-MAX */
RUNNING_EXT uint8_t contact_segment;	 /* 双触点变压器触点分段标记 0 第一段 1第二段 */
RUNNING_EXT uint8_t cur_segment;         /* 电流段 GR DV */
RUNNING_EXT uint8_t cur_gear_bak;        /* 档位电流档位备份值 */
RUNNING_EXT uint8_t cur_ir_dis_gear;     /* 自动档关闭时使用 */
RUNNING_EXT uint8_t cur_ir_dis_gear_0;   /* 自动档关闭时使用 */
RUNNING_EXT uint8_t cur_ir_dis_gear_1;   /* 自动档关闭时使用 */
RUNNING_EXT uint32_t g_ir_gear_hold; 	 /* 自动档档位保持时使用 */
RUNNING_EXT uint8_t cur_gear_comm;       /* 当前步电流档位 */
RUNNING_EXT uint8_t cur_real_gear_comm;  /* 当前步电流档位 */
RUNNING_EXT uint8_t cur_method;          /* GR测试方法 GR_V_CONT_MODEL GR_V_PULSE_MODEL */
RUNNING_EXT uint16_t res_ave_comm;       /* IR电阻为通信用 */
RUNNING_EXT uint16_t cur_ave_comm;       /* GR电流为通信用 */
RUNNING_EXT uint8_t cur_output_impedan;  /* DCW 输出阻抗 */
RUNNING_EXT uint16_t cur_intensity;      /* CC当前电流强度 */

RUNNING_EXT float cur_high;              /* 当前上限值 */
RUNNING_EXT float cur_low;               /* 当前下限值 */
RUNNING_EXT float cur_ir_rang_h;         /* 当前下限值 */
RUNNING_EXT uint16_t cur_charge_cur;     /* 当前充电电流 */
RUNNING_EXT uint16_t cur_real_cur;       /* 当前真实电流 */
RUNNING_EXT uint16_t cur_delay_t;        /* 延时报警时间 */
RUNNING_EXT uint8_t cur_auto;            /* 当前ir 自动换挡 */
RUNNING_EXT uint16_t cur_arc_gear;       /* 当前电弧等级 或电流模式下的电流 */
RUNNING_EXT uint16_t cur_frequency_gear; /* 输出频率档位选择 1-9 */
RUNNING_EXT uint16_t cur_frequency;      /* 输出频率 */
RUNNING_EXT TEST_PORT cur_port;          /* 当前的端口 */
RUNNING_EXT WORK_PORT cur_work_port;     /* 当前的工作端口 */
RUNNING_EXT uint8_t cur_work_st;         /* 当前的工作端口状态 */
RUNNING_EXT uint8_t cur_auto;            /* 当前ir 自动换挡 */

RUNNING_EXT int8_t steps_con;	         /* 步间连续 */
RUNNING_EXT int8_t steps_pass;           /* 步间PASS */
RUNNING_EXT uint16_t cur_offset;         /* 当前偏移电流或电阻 */
RUNNING_EXT uint16_t cur_real_offset;    /* 当前的真是电流偏移 */
RUNNING_EXT uint8_t cur_vol_gear;        /* 当前ir电压档位 0-9 */

RUNNING_EXT uint8_t err_charge;          /* 充电错误 */
RUNNING_EXT uint8_t err_real;            /* 真实电流错误 */

RUNNING_EXT float cur_dac_k;             /* 当前DAC校准系数k */
RUNNING_EXT float cur_dac_b;             /* 当前DAC校准系数b */
RUNNING_EXT float cur_adc_k;             /* 当前ADC校准系数k */
RUNNING_EXT float cur_adc_b;             /* 当前ADC校准系数b */
RUNNING_EXT float cur_adc_cur_k;         /* 当前 电流ADC校准系数k */
RUNNING_EXT float cur_arc_k;             /* 当前ARC报警校准系数k */
RUNNING_EXT float cur_res_k;             /* 当前IR电阻挡位的校准系数k */

RUNNING_EXT  float vol_ch_step;	/* 电压缓变步进值 */
RUNNING_EXT  float vol_ch_base;	/* 电压变化的起始值 */
RUNNING_EXT  float vol_ch_target;	/* 电压变化的目标 */


extern CS_BOOL judge_road_work(ROAD_NUM_T road);
extern uint8_t get_work_roads(void);
extern void load_data(void);

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
