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
  * @brief  串口扫描模块状态定义
  */
typedef enum{
    SEND_STOP,///<停止扫描
    SEND_START,///<启动扫描
    SEND_OVER,///<启动扫描
    SEND_INIT,///<扫描初始化值
}SEND_MODULE_ST;

typedef CS_ERR (*SEND_CMD_FUN)(uint8_t addr, uint8_t*data, uint32_t len);

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


void register_send_cmd_fun(SEND_CMD_FUN fun)
{
    send_cmd_fun = fun;
}
void init_send_module_connect(void)
{
    register_send_cmd_fun(send_module_connect);
}
void init_send_set_road_num(void)
{
    register_send_cmd_fun(com_module_set_road_num);
}
/**
  * @brief  扫描模块公共接口
  * @param  扫描模块的结构信息
  * @retval 无
  */
static void send_module_x(SEND_MODULE_T *inf, uint8_t *data, uint32_t len)
{
    CS_ERR err = CS_ERR_SEND_FAIL;
    
    /* 扫描状态已启动 */
    if(inf->status == SEND_START)
    {
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
            inf->cur_addr = inf->next_addr;
            inf->next_addr = inf->roads_flag->road_buf[inf->index];
            inf->index++;
            
            if(inf->index > inf->roads_flag->count)
            {
                inf->status = SEND_OVER;
            }
        }
    }
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  提供给用户层循环调用的扫描函数
  * @param  无
  * @retval 无
  */
void send_all_module(uint8_t *data, uint32_t len)
{
    send_module_x(&com1_send_cmd, data, len);
    send_module_x(&com2_send_cmd, data, len);
    send_module_x(&com3_send_cmd, data, len);
    send_module_x(&com4_send_cmd, data, len);
}
/**
  * @brief  启动4路串口进行模块扫描
  * @param  无
  * @retval 无
  */
void start_send_all_module(void)
{
    com1_send_cmd.status = SEND_START;
    com2_send_cmd.status = SEND_START;
    com3_send_cmd.status = SEND_START;
    com4_send_cmd.status = SEND_START;
    
    com1_send_cmd.next_addr = com1_send_cmd.roads_flag->road_buf[0];
    com2_send_cmd.next_addr = com2_send_cmd.roads_flag->road_buf[0];
    com3_send_cmd.next_addr = com3_send_cmd.roads_flag->road_buf[0];
    com4_send_cmd.next_addr = com4_send_cmd.roads_flag->road_buf[0];
    
    com1_send_cmd.index = 0;
    com2_send_cmd.index = 0;
    com3_send_cmd.index = 0;
    com4_send_cmd.index = 0;
}

/**
  * @brief  停止4路串口进行模块扫描
  * @param  无
  * @retval 无
  */
void stop_send_all_module(void)
{
    com1_send_cmd.status = SEND_STOP;
    com2_send_cmd.status = SEND_STOP;
    com3_send_cmd.status = SEND_STOP;
    com4_send_cmd.status = SEND_STOP;
}

/**
  * @brief  查询第1路串口扫描是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL com1_send_is_over(void)
{
    return com1_send_cmd.status == SEND_OVER? CS_TRUE:CS_FALSE;
}
/**
  * @brief  查询第2路串口扫描是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL com2_send_is_over(void)
{
    return com2_send_cmd.status == SEND_OVER? CS_TRUE:CS_FALSE;
}

/**
  * @brief  查询第3路串口扫描是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL com3_send_is_over(void)
{
    return com3_send_cmd.status == SEND_OVER? CS_TRUE:CS_FALSE;
}

/**
  * @brief  查询第4路串口扫描是是否结束
  * @param  无
  * @retval CS_FALSE 未结束 CS_TRUE 结束
  */
CS_BOOL com4_send_is_over(void)
{
    return com4_send_cmd.status == SEND_OVER? CS_TRUE:CS_FALSE;
}
/**
  * @brief  查询全部串口扫描是是否结束
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
  * @brief  获取串口1当前的扫描地址
  * @param  无
  * @retval 当前扫描地址
  */
uint8_t get_com1_send_addr(void)
{
    return com1_send_cmd.cur_addr;
}
/**
  * @brief  获取串口2当前的扫描地址
  * @param  无
  * @retval 当前扫描地址
  */
uint8_t get_com2_send_addr(void)
{
    return com2_send_cmd.cur_addr;
}

/**
  * @brief  获取串口3当前的扫描地址
  * @param  无
  * @retval 当前扫描地址
  */
uint8_t get_com3_send_addr(void)
{
    return com3_send_cmd.cur_addr;
}
/**
  * @brief  获取串口4当前的扫描地址
  * @param  无
  * @retval 当前扫描地址
  */
uint8_t get_com4_send_addr(void)
{
    return com4_send_cmd.cur_addr;
}

/**
  * @brief  用户层调用针对指定地址连接模块. 在开机时使用，来连接历史记录中的模块
  * @param  [in] addr 指定的连接地址
  * @retval 发送连接命令的状态 
  *     CS_ERR_SEND_FAIL 发送失败
  *     CS_ERR_SEND_SUCCESS 发送成功
  */
//CS_ERR connect_module(uint8_t addr)
//{
//	OS_ERR os_err;
//    CS_ERR err = CS_ERR_SEND_FAIL;//初始化为发送失败
//    uint8_t count = 0;
//    uint8_t RETRY_TIMES = 3;//重试次数
//    
//    wait_com_comm_idle(addr);//等待串口进入
//    
//    do{
//        err = com_module_connect(addr);//发送联机指令
//        
//        /* 发送成功就退出去 */
//        if(err == CS_ERR_SEND_SUCCESS)
//        {
//            break;
//        }
//        
//        count++;
//        OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&os_err);
//    }while(count < RETRY_TIMES);
//    
//    return err;
//}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
