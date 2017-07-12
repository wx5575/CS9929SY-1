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
#include "USART2.H"
#include "USART3.H"
#include "UART4.H"
#include "mem_alloc.h"
#include "tim3.h"


CS_ERR com_send_cmd_data(uint8_t addr, uint8_t cmd, uint8_t *data, uint32_t len);


uint8_t get_com_offset_add(COM_NUM com_num)
{
    uint8_t addr_offset[4] = {0, 16, 32, 48};//地址偏移，第一路0，第二路16，第三路32，第四路48
    
    return addr_offset[com_num];
}
COM_STRUCT* get_com_inf(COM_NUM com_num)
{
    COM_STRUCT *p_com = NULL;
    
    switch(com_num)
    {
        case ROAD1_COM:
            p_com = &com1;
            break;
        case ROAD2_COM:
            p_com = &com2;
            break;
        case ROAD3_COM:
            p_com = &com3;
            break;
        case ROAD4_COM:
            p_com = &com4;
            break;
    }
    
    return p_com;
}
void update_module_addr_flag(void)
{
    int32_t i = 0;
    int32_t j = 0;
    uint8_t addr = 0;
    uint8_t offset_addr = 0;
    COM_STRUCT* com_inf = NULL;
    uint8_t id = 0;
    
    for(i = 0; i < 64; i++)
    {
        if(i == 0 || i == 16 || i == 32 || i == 48 || i > 64)
        {
            continue;
        }
        
        id = road_inf_pool[i].module_inf.id;//id范围是1-15
        
        if(id != 0 && id < 16)
        {
            com_inf = get_com_inf(road_inf_pool[i].com_num);
            offset_addr = get_com_offset_add(road_inf_pool[i].com_num);
            addr = offset_addr + id;
            roads_flag.road_buf[j] = addr;
            syn_test_port[j].com = com_inf;
            j++;
        }
    }
    
    roads_flag.count = j;
}
void set_module_inf(COM_NUM com_num, uint8_t index, uint8_t *data)
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
void com_receive_dispose(COM_NUM com_num, uint8_t *data, uint32_t len)
{
    uint16_t *p_crc;
    uint16_t crc_val;
    FRAME_T *frame = (void*)data;
    uint8_t index = 0;
    uint8_t addr_offset[4] = {0, 16, 32, 48};//地址偏移，第一路0，第二路16，第三路32，第四路48
    
    
    /* CRC校验 */
    p_crc = (uint16_t *)&data[len - 2];
    crc_val = get_crc16(data, len - 2);
    
    /* 校验失败放弃解析 */
    if(*p_crc != crc_val)
    {
        return;
    }
    
    index = addr_offset[com_num] + frame->addr;
    
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

COM_NUM get_com_num(uint8_t addr, CS_ERR *err)
{
    COM_NUM com_num;
    
    *err = CS_ERR_NONE;
    
    if(addr > 0 && addr < 16)
    {
        com_num = ROAD1_COM;
    }
    else if(addr > 16 && addr < 32)
    {
        com_num = ROAD2_COM;
    }
    else if(addr > 32 && addr < 48)
    {
        com_num = ROAD3_COM;
    }
    else if(addr > 48 && addr < 64)
    {
        com_num = ROAD4_COM;
    }
    else
    {
        *err = CS_ERR_COMM_ADDR_INVALTD;
    }
    
    return com_num;
}

CS_ERR com_module_connect(uint8_t addr)
{
    return com_send_cmd_data(addr, GET_MODULE_INF, NULL, 0);
}
CS_ERR com_send_cmd_data(uint8_t addr, uint8_t cmd, uint8_t *data, uint32_t len)
{
    FRAME_T *frame;
    ROAD_INF *inf;
    uint8_t frame_len = 0;
    COM_STRUCT *com_inf;
    CS_ERR err;
    COM_NUM com_num;
    
    com_num = get_com_num(addr, &err);//获取串口号
    
    if(err != CS_ERR_NONE)
    {
        return CS_ERR_SEND_FAIL;
    }
    
    com_inf = get_com_inf(com_num);
    
    if(com_inf->status != MODULE_COMM_IDLE)
    {
        return CS_ERR_COM_BUSY;
    }
    
    inf = &road_inf_pool[addr];
    
    frame = (void*)inf->buf;
    
    frame->addr = inf->module_inf.id;
    frame->fun_num = cmd;
    frame->st = 0;
    memcpy(frame->data, data, len);
    frame_len = len + sizeof(FRAME_T) - 4;//减4是去掉数据指针的长度4字节
    add_crc_to_send_data((uint8_t*)frame, frame_len);
    frame_len += 2;
    
    com_inf->send_fun((uint8_t *)frame, frame_len);
    com_inf->set_wait_ack_timeout();
    com_inf->status = MODULE_COMM_SEND;
    
    return CS_ERR_SEND_SUCCESS;
}
void init_module_manage_env(void)
{
    register_tim3_server_fun(usart1_judge_timout);
    register_tim3_server_fun(usart2_judge_timout);
    register_tim3_server_fun(usart3_judge_timout);
    register_tim3_server_fun(uart4_judge_timout);
    register_tim3_server_fun(com1_wait_ack_timeout);
    register_tim3_server_fun(com2_wait_ack_timeout);
    register_tim3_server_fun(com3_wait_ack_timeout);
    register_tim3_server_fun(com4_wait_ack_timeout);
    road_inf_pool = malloc_ex_mem(sizeof(ROAD_INF) * 64);
}

void module_comm_task(void)
{
    com1_comm_status_machine();
    com2_comm_status_machine();
    com3_comm_status_machine();
    com4_comm_status_machine();
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
