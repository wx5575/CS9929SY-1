/**
  ******************************************************************************
  * @file    send_cmd.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   发送命令
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "send_cmd.h"
#include "os.h"

/* Private typedef -----------------------------------------------------------*/

/**
  * @brief  串口发送状态定义
  */
typedef enum{
    SEND_START,///<启动发送
    SEND_OVER,///<发送完成
    SEND_INIT,///<发送状态初始化值
}SEND_MODULE_ST;


SEND_CMD_FUN send_cmd_fun;
/**
  * @brief  串口扫描模块结构体定义  
  */
typedef struct{
    uint8_t status;///<发送状态
    uint8_t start_addr;///<起始地址
    uint8_t end_addr;///<结束地址
    uint8_t offset_addr;///<偏移地址 串口1的偏移地址是0 串口2的偏移地址是16 串口3是32 串口4是48
    _ROADS_FLAG *roads_flag;///<多路标志
    uint8_t next_addr;///<等待扫描的地址
    uint8_t cur_addr;///<当前的扫描地址
    uint8_t index;///<地址索引
}SEND_MODULE_T;

/* Private variables ---------------------------------------------------------*/
/**
  * @brief  串口1扫描模块结构信息定义
  */
static SEND_MODULE_T com1_send_cmd=
{
    SEND_INIT,///<发送状态
    1,///<起始地址
    SLAVE_ADDR_MAX,///<结束地址
    COM1_BROADCAST_ADDR,///<偏移地址
    &roads_flag.flag[ROAD1_COM],
};
/**
  * @brief  串口2扫描模块结构信息定义
  */
static SEND_MODULE_T com2_send_cmd=
{
    SEND_INIT,///<发送状态
    1,///<起始地址
    SLAVE_ADDR_MAX,///<结束地址
    COM2_BROADCAST_ADDR,///<偏移地址
    &roads_flag.flag[ROAD2_COM],
};
/**
  * @brief  串口3扫描模块结构信息定义
  */
static SEND_MODULE_T com3_send_cmd=
{
    SEND_INIT,///<发送状态
    1,///<起始地址
    SLAVE_ADDR_MAX,///<结束地址
    COM3_BROADCAST_ADDR,///<偏移地址
    &roads_flag.flag[ROAD3_COM],
};
/**
  * @brief  串口4扫描模块结构信息定义
  */
static SEND_MODULE_T com4_send_cmd=
{
    SEND_INIT,///<发送状态
    1,///<起始地址
    SLAVE_ADDR_MAX,///<结束地址
    COM4_BROADCAST_ADDR,///<偏移地址
    &roads_flag.flag[ROAD4_COM],
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  注册发送函数
  * @param  [in] 发送函数
  * @retval 无
  */
void register_send_cmd_fun(SEND_CMD_FUN fun)
{
    send_cmd_fun = fun;
}

/**
  * @brief  向已知模块发送命令
  * @param  [in] inf 串口发送的结构信息
  * @param  [in] data 串口发送的数据
  * @param  [in] len 串口发送的数据长度
  * @retval 无
  */
static void send_cmd_to_known_module(SEND_MODULE_T *inf, uint8_t *data, uint32_t len)
{
    CS_ERR err = CS_ERR_SEND_FAIL;
    
    /* 扫描状态已启动 */
    if(inf->status == SEND_START)
    {
        /* 如果该路串口挂接的模块个数为0就结束发送 */
        if(inf->roads_flag->count == 0)
        {
            inf->status = SEND_OVER;
            return;
        }
        
        if(send_cmd_fun == NULL)
        {
            inf->status = SEND_OVER;
            return;
        }
        
        err = send_cmd_fun(inf->next_addr, data, len);
        
        /* 发送成功 或 通信地址不合法 继续发送下下个地址 */
        if(err == CS_ERR_SEND_SUCCESS || err == CS_ERR_COMM_ADDR_INVALTD)
        {
            inf->cur_addr = inf->next_addr;//更新当前发送地址为下一个地址
            inf->next_addr = inf->roads_flag->road_buf[inf->index];//更新下一个地址
            inf->index++;//地址索引加一
            
            if(inf->index >= inf->roads_flag->count)
            {
                inf->status = SEND_OVER;
            }
        }
    }
}

/**
  * @brief  启动4路串口进行模块扫描
  * @param  无
  * @retval 无
  */
static void start_send_cmd_to_all_module(void)
{
    /* 发送进入启动状态 */
    com1_send_cmd.status = SEND_START;
    com2_send_cmd.status = SEND_START;
    com3_send_cmd.status = SEND_START;
    com4_send_cmd.status = SEND_START;
    
    /* 加载各串口的第1个模块地址 */
    com1_send_cmd.next_addr = com1_send_cmd.roads_flag->road_buf[0];
    com2_send_cmd.next_addr = com2_send_cmd.roads_flag->road_buf[0];
    com3_send_cmd.next_addr = com3_send_cmd.roads_flag->road_buf[0];
    com4_send_cmd.next_addr = com4_send_cmd.roads_flag->road_buf[0];
    
    /* 清0发送地址索引 */
    com1_send_cmd.index = 0;
    com2_send_cmd.index = 0;
    com3_send_cmd.index = 0;
    com4_send_cmd.index = 0;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  提供给用户层调用
  * @param  无
  * @retval 无
  */
void send_cmd_to_all_module(uint8_t *data, uint32_t len, SEND_CMD_FUN fun)
{
    CS_BOOL flag = CS_FALSE;
    
    register_send_cmd_fun(fun);
    start_send_cmd_to_all_module();
    
    while(1)
    {
        send_cmd_to_known_module(&com1_send_cmd, data, len);
        send_cmd_to_known_module(&com2_send_cmd, data, len);
        send_cmd_to_known_module(&com3_send_cmd, data, len);
        send_cmd_to_known_module(&com4_send_cmd, data, len);
        
        flag = all_com_send_is_over();
        
        if(flag == CS_TRUE)
        {
            break;
        }
        
        OS_DELAY_ms(10);
    }
}
/**
  * @brief  查询第1路串口发送是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL com1_send_is_over(void)
{
    return com1_send_cmd.status == SEND_OVER? CS_TRUE:CS_FALSE;
}
/**
  * @brief  查询第2路串口发送是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL com2_send_is_over(void)
{
    return com2_send_cmd.status == SEND_OVER? CS_TRUE:CS_FALSE;
}

/**
  * @brief  查询第3路串口发送是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL com3_send_is_over(void)
{
    return com3_send_cmd.status == SEND_OVER? CS_TRUE:CS_FALSE;
}

/**
  * @brief  查询第4路串口发送是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL com4_send_is_over(void)
{
    return com4_send_cmd.status == SEND_OVER? CS_TRUE:CS_FALSE;
}
/**
  * @brief  查询全部串口发送是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL all_com_send_is_over(void)
{
    CS_BOOL flag = CS_FALSE;
    
    if(com1_send_cmd.status == SEND_OVER 
        && com2_send_cmd.status == SEND_OVER
        && com3_send_cmd.status == SEND_OVER
        && com4_send_cmd.status == SEND_OVER)
    {
        flag = CS_TRUE;
        register_send_cmd_fun(NULL);//注销发送函数
    }
    
    return flag;
}

/**
  * @brief  获取串口1当前的发送地址
  * @param  无
  * @retval 当前扫描地址
  */
MODULE_ADDR_T get_com1_send_addr(void)
{
    return com1_send_cmd.cur_addr;
}
/**
  * @brief  获取串口2当前的发送地址
  * @param  无
  * @retval 当前扫描地址
  */
MODULE_ADDR_T get_com2_send_addr(void)
{
    return com2_send_cmd.cur_addr;
}

/**
  * @brief  获取串口3当前的发送地址
  * @param  无
  * @retval 当前扫描地址
  */
MODULE_ADDR_T get_com3_send_addr(void)
{
    return com3_send_cmd.cur_addr;
}
/**
  * @brief  获取串口4当前的发送地址
  * @param  无
  * @retval 当前扫描地址
  */
MODULE_ADDR_T get_com4_send_addr(void)
{
    return com4_send_cmd.cur_addr;
}

CS_ERR send_cmd_to_one_module(ROAD_INDEX road, uint8_t *data, uint32_t len, SEND_CMD_FUN fun)
{
    MODULE_ADDR_T addr;
    CS_ERR err;
    
    addr = get_module_addr(road, &err);
    
    if(err != CS_ERR_NONE)
    {
        return err;
    }
    
    err = fun(addr, data, len);
    
    return err;
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
