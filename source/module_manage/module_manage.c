/**
  ******************************************************************************
  * @file    module_manage.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.10
  * @brief   模块管理
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#define MODULE_GLOBALS
#include "module_manage.h"
#include "string.h"
#include "crc.h"
#include "USART1.H"
#include "USART2.H"
#include "USART3.H"
#include "UART4.H"
#include "mem_alloc.h"


/* Private typedef -----------------------------------------------------------*/

enum{
    GET_MODULE_INF = 1,
};
uint8_t addr_offset[] =
{
    COM1_BROADCAST_ADDR,///<第1路串口的广播地址
    COM2_BROADCAST_ADDR,///<第2路串口的广播地址
    COM3_BROADCAST_ADDR,///<第3路串口的广播地址
    COM4_BROADCAST_ADDR ///<第4路串口的广播地址
};//地址偏移，第一路0，第二路16，第三路32，第四路48

/* Private function prototypes -----------------------------------------------*/
CS_ERR com_send_cmd_data(uint8_t addr, uint8_t cmd, uint8_t *data, uint32_t len);
static COM_NUM get_com_num(uint8_t *addr, CS_ERR *err);


uint8_t get_com_offset_add(COM_NUM com_num)
{
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
    
    for(i = 0; i < MASTER_ADDR_RANGE; i++)
    {
        if(i == COM1_BROADCAST_ADDR
            || i == COM2_BROADCAST_ADDR
            || i == COM3_BROADCAST_ADDR
            || i == COM4_BROADCAST_ADDR)
        {
            continue;
        }
        
        id = road_inf_pool[i].module_inf.id;//id范围是1-15
        
        if(id != SLAVE_BROADCAST_ADDR && id <= SLAVE_ADDR_MAX)
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

/* Public functions ---------------------------------------------------------*/
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
    COM_STRUCT *com_inf;
    
    /* CRC校验 */
    p_crc = (uint16_t *)&data[len - CRC_LEN];
    crc_val = get_crc16(data, len - CRC_LEN);
    
    com_inf = get_com_inf(com_num);
    
    /* 校验失败放弃解析 */
    if(*p_crc != crc_val)
    {
        com_inf->comm_cannot_connect = CS_ERR_COMM_CRC_ERR;
        return;
    }
    
    /* 从机通信状态错误 */
    if(frame->st != COMM_ST_NO_ERROR)
    {
        com_inf->comm_cannot_connect = CS_ERR_COMM_SLAVE_ERR;
        return;
    }
    
    index = addr_offset[com_num] + frame->addr;
    
    /* 指令解析 */
    switch(frame->fun_code)
    {
        case GET_MODULE_INF:
            set_module_inf(com_num, index, frame->data);
            break;
    }
}

/**
  * @brief  向发送数据添加CRC校验
  * @param  [in] data 数据
  * @param  [in] len 数据帧长度
  * @retval 无
  */
void add_crc_to_send_data(uint8_t *data, uint32_t len)
{
    uint16_t crc_val;
    uint16_t *p_u16;
    
    crc_val = get_crc16(data, len);
    p_u16 = (uint16_t *)&data[len];
    *p_u16 = crc_val;
}

/**
  * @brief  获取串口状态
  * @param  [in] addr 通信地址
  * @retval 串口状态
  */
MODULE_COMM_STATUS get_com_st(uint8_t addr)
{
    COM_STRUCT *com_inf;
    CS_ERR err;
    COM_NUM com_num;
    
    com_num = get_com_num(&addr, &err);//获取串口号
    com_inf = get_com_inf(com_num);
    
    return com_inf->status;
}

/**
  * @brief  获取串口是否在空闲状态
  * @param  [in] addr 通信地址
  * @retval 0 不在空闲状态 1 在空闲状态
  */
uint8_t com_comm_is_idle(uint8_t addr)
{
    MODULE_COMM_STATUS st;
    
    st = get_com_st(addr);
    
    return st == MODULE_COMM_IDLE;
}
/**
  * @brief  根据提供的地址来计算出串串口编辑
  * @param  [in] addr 通信地址
  * @param  [out] err 错误码
  * @retval 串口编号
  */
static COM_NUM get_com_num(uint8_t *addr, CS_ERR *err)
{
    COM_NUM com_num;
    
    *err = CS_ERR_NONE;
    
    /* 0-15是第1路串口的地址范围 */
    if(*addr > COM1_BROADCAST_ADDR
        && *addr <= (COM1_BROADCAST_ADDR + SLAVE_ADDR_MAX))
    {
        com_num = ROAD1_COM;
        *addr -= COM1_BROADCAST_ADDR;
    }
    /* 16-31是第2路串口的地址范围 */
    else if(*addr >= COM2_BROADCAST_ADDR
        && *addr <= (COM2_BROADCAST_ADDR + SLAVE_ADDR_MAX))
    {
        com_num = ROAD2_COM;
        *addr -= COM2_BROADCAST_ADDR;
    }
    /* 32-47是第3路串口的地址范围 */
    else if(*addr >= COM3_BROADCAST_ADDR
            && *addr <= (COM3_BROADCAST_ADDR + SLAVE_ADDR_MAX))
    {
        com_num = ROAD3_COM;
        *addr -= COM3_BROADCAST_ADDR;
    }
    /* 48-63是第4路串口的地址范围 */
    else if(*addr >= COM4_BROADCAST_ADDR
        && *addr <= (COM4_BROADCAST_ADDR + SLAVE_ADDR_MAX))
    {
        com_num = ROAD4_COM;
        *addr -= COM4_BROADCAST_ADDR;
    }
    /* 地址非法 */
    else
    {
        *err = CS_ERR_COMM_ADDR_INVALTD;
    }
    
    return com_num;
}
/**
  * @brief  发送通信模块的连接指令
  * @param  [in] addr 通信地址
  * @retval 发送结果
  */
CS_ERR com_module_connect(uint8_t addr)
{
    return com_send_cmd_data(addr, GET_MODULE_INF, NULL, 0);
}
/**
  * @brief  发送命令给从机
  * @param  [in] addr 通信地址
  * @param  [in] cmd 功能码
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  *     CS_ERR_SEND_FAIL 通信地址非法
  *     CS_ERR_COM_BUSY 串口发送的上一条指令还没完成——发送后要有返回数据才能发送下一条命令
  *     CS_ERR_SEND_SUCCESS  发送成功
  */
CS_ERR com_send_cmd_data(uint8_t addr, uint8_t cmd, uint8_t *data, uint32_t len)
{
    FRAME_T *frame;
    ROAD_INF *inf;
    uint8_t frame_len = 0;
    COM_STRUCT *com_inf;
    CS_ERR err;
    COM_NUM com_num;
    
    com_num = get_com_num(&addr, &err);//获取串口号
    
    /* 如果地址非法就返回 */
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
    
    frame->addr = addr;
    frame->fun_code = cmd;
    frame->st = 0;
    memcpy(frame->data, data, len);
    frame_len = len + FRAME_HEAD_SIZE;
    add_crc_to_send_data((uint8_t*)frame, frame_len);
    frame_len += CRC_LEN;
    
    com_inf->send_fun(com_inf, (uint8_t *)frame, frame_len);
    com_inf->set_wait_ack_timeout(com_inf);
    com_inf->status = MODULE_COMM_SEND;//状态机进入发送状态
    
    return CS_ERR_SEND_SUCCESS;
}
/**
  * @brief  初始化模块管理的环境
  * @param  无
  * @retval 无
  */
void init_module_manage_env(void)
{
    /* 初始化各路串口的运行环境 */
    com1.init_com_env(&com1);
    com2.init_com_env(&com2);
    com3.init_com_env(&com3);
    com4.init_com_env(&com4);
    
    /* 为模块信息分配内存 */
    road_inf_pool = malloc_ex_mem(sizeof(ROAD_INF) * MASTER_ADDR_RANGE);//分配内存，不会再释放了
    
    /* 初始化分配到的内存 */
    if(road_inf_pool != NULL)
    {
        memset(road_inf_pool, 0, sizeof(ROAD_INF) * MASTER_ADDR_RANGE);
    }
}

/**
  * @brief  清空模块信息
  * @param  无
  * @retval 无
  */
void clear_module_inf(void)
{
    memset(&roads_flag, 0 , sizeof(roads_flag));
    memset(road_inf_pool, 0, sizeof(ROAD_INF) * MASTER_ADDR_RANGE);
}
/**
  * @brief  初始化模块管理的环境
  * @param  无
  * @retval 无
  */
void module_comm_task(void)
{
    com_comm_status_machine(&com1);
    com_comm_status_machine(&com2);
    com_comm_status_machine(&com3);
    com_comm_status_machine(&com4);
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
