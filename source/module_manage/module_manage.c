/**
  ******************************************************************************
  * @file    module_manage.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.10
  * @brief   模块管理
  ******************************************************************************
  */
#define MODULE_GLOBALS
#include "module_manage.h"
#include "string.h"
#include "crc.h"
#include "USART1.H"
#include "mem_alloc.h"
#include "tim3.h"

uint32_t com1_timeout;
uint32_t com2_timeout;
uint32_t com3_timeout;
uint32_t com4_timeout;
void com_receive_dispose(uint8_t com_num, uint8_t *frame, uint32_t len);
void com_send_cmd_data(uint8_t road, uint8_t cmd, uint8_t *data, uint32_t len);
void com_send_data(uint8_t com_num, uint8_t *data, uint32_t len);
typedef void (*COM_SEND_FUN)(uint8_t *, uint32_t);

COM_SEND_FUN com_send_fun[4]=
{
    usart1_send_data,//com1的发送函数
    
};
uint8_t *get_com1_frame(void)
{
    return usart1_get_receive_data();
}
uint32_t get_com1_frame_len(void)
{
    uint32_t len = 0;
    
    len = usart1_get_receive_data_count();
    usart1_clear_receive_data_count();
    
    return len;
}
uint8_t *get_com2_frame(void)
{
}
uint32_t get_com2_frame_len(void)
{
}  
uint8_t *get_com3_frame(void)
{
}
uint32_t get_com3_frame_len(void)
{
}
uint8_t *get_com4_frame(void)
{
}
uint32_t get_com4_frame_len(void)
{
}

void com_timeout_judge(void)
{
    uint8_t *frame;
    uint32_t len;
    uint8_t com_num = 0;
    
    if(get_usart1_receive_over_flag())
    {
        clear_usart1_receive_over_flag();
        com_num = 1;
        frame = get_com1_frame();
        len = get_com1_frame_len();
    }
    else if(com2_timeout == 0)
    {
        com_num = 2;
        frame = get_com2_frame();
        len = get_com2_frame_len();
    }
    else if(com3_timeout == 0)
    {
        com_num = 3;
        frame = get_com3_frame();
        len = get_com3_frame_len();
    }
    else if(com4_timeout == 0)
    {
        com_num = 4;
        frame = get_com4_frame();
        len = get_com4_frame_len();
    }
    
    if(com_num != 0)
    {
        com_receive_dispose(com_num, frame, len);
    }
}
void update_module_addr_flag(void)
{
    int32_t i = 0;
    int32_t j = 0;
    uint8_t addr = 0;
    
    for(i = 0; i < 64; i++)
    {
        if(i == 0 || i == 16 || i == 32 || i == 64 || i > 64)
        {
            continue;
        }
        
        addr = road_inf_pool[i].com_num - 1 + road_inf_pool[i].module_inf.id;
        roads_flag.road_buf[j++] = addr;
    }
    
    roads_flag.count = j;
}
void set_module_inf(uint8_t com_num, uint8_t index, uint8_t *data)
{
    road_inf_pool[index].com_num = com_num;
    memcpy(&road_inf_pool[index].module_inf, data, sizeof(MODULE_INF));
    update_module_addr_flag();
}
enum{
    GET_MODULE_INF = 1,
};
/**
  * @brief  串口1的接收处理函数
  * @param  [in] com_num 串口号
  * @param  [in] frame 数据帧
  * @param  [in] len 数据帧长度
  * @retval 无
  */
void com_receive_dispose(uint8_t com_num, uint8_t *data, uint32_t len)
{
    uint16_t *p_crc;
    uint16_t crc_val;
    FRAME_T *frame = (void*)data;
    uint8_t addr_offset[4] = {0, 16, 32, 48};
    uint8_t index = 0;
    
    /* CRC校验 */
    p_crc = (uint16_t *)&data[len - 2];
    crc_val = get_crc16(data, len - 2);
    
    /* 校验失败放弃解析 */
    if(*p_crc != crc_val)
    {
        return;
    }
    
    index = addr_offset[com_num - 1] + frame->addr;
    
    /* 指令解析 */
    switch(frame->fun_num)
    {
        case GET_MODULE_INF:
            set_module_inf(com_num, index, frame->data);
            break;
    }
}

ROAD_INF* get_road_inf(uint8_t road)
{
    uint8_t addr;
    
    if(road > roads_flag.count)
    {
        return NULL;
    }
    
    addr = roads_flag.road_buf[road-1];
    
    return &road_inf_pool[addr];
}
void add_crc_to_send_data(uint8_t *data, uint32_t len)
{
    uint16_t crc_val;
    uint16_t *p_u16;
    
    crc_val = get_crc16(data, len);
    p_u16 = (uint16_t *)&data[len];
    *p_u16 = crc_val;
}
void com_send_cmd_data(uint8_t road, uint8_t cmd, uint8_t *data, uint32_t len)
{
    FRAME_T *frame;
    ROAD_INF *inf;
    uint8_t frame_len = 0;
    
    inf = get_road_inf(road);
    
    if(inf == NULL)
    {
        return;
    }
    
    frame = (void*)inf->buf;
    
    frame->addr = inf->module_inf.id;
    frame->fun_num = cmd;
    frame->st = 0;
    memcpy(frame->data, data, len);
    frame_len = len + sizeof(FRAME_T) - 4;//减4是去掉数据指针的长度4字节
    add_crc_to_send_data((uint8_t*)frame, frame_len);
    frame_len += 2;
    com_send_data(inf->com_num, (uint8_t *)frame, frame_len);
}

void com_send_data(uint8_t com_num, uint8_t *data, uint32_t len)
{
    com_send_fun[com_num - 1](data, len);
}

void init_module_manage_env(void)
{
    register_tim3_server_fun(usart1_judge_timout);
    road_inf_pool = malloc_ex_mem(sizeof(ROAD_INF) * 64);
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
