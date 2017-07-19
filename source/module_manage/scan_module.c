/**
  ******************************************************************************
  * @file    scan_module.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   扫描模块
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "scan_module.h"
#include "os.h"

/* Private typedef -----------------------------------------------------------*/

/**
  * @brief  串口扫描模块状态定义
  */
typedef enum{
    SCAN_STOP,///<停止扫描
    SCAN_START,///<启动扫描
    SCAN_OVER,///<启动扫描
    SCAN_INIT,///<扫描初始化值
}SCAN_MODULE_ST;

/**
  * @brief  串口扫描模块结构体定义
  */
typedef struct{
    uint8_t status;///<扫描状态
    uint8_t start_addr;///<起始地址
    uint8_t end_addr;///<结束地址
    uint8_t offset_addr;///<偏移地址 串口1的偏移地址是0 串口2的偏移地址是16 串口3是32 串口4是48
    uint8_t next_addr;///<等待扫描的地址
    uint8_t cur_addr;///<当前的扫描地址
}SCAN_MODULE_T;

/* Private variables ---------------------------------------------------------*/
/**
  * @brief  串口1扫描模块结构信息定义
  */
static SCAN_MODULE_T com1_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    SLAVE_ADDR_MAX,///<结束地址
    COM1_BROADCAST_ADDR,///<偏移地址
};
/**
  * @brief  串口2扫描模块结构信息定义
  */
static SCAN_MODULE_T com2_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    SLAVE_ADDR_MAX,///<结束地址
    COM2_BROADCAST_ADDR,///<偏移地址
};
/**
  * @brief  串口3扫描模块结构信息定义
  */
static SCAN_MODULE_T com3_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    SLAVE_ADDR_MAX,///<结束地址
    COM3_BROADCAST_ADDR,///<偏移地址
};
/**
  * @brief  串口4扫描模块结构信息定义
  */
static SCAN_MODULE_T com4_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    SLAVE_ADDR_MAX,///<结束地址
    COM4_BROADCAST_ADDR,///<偏移地址
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  扫描模块公共接口
  * @param  扫描模块的结构信息
  * @retval 无
  */
static void scan_module_x(SCAN_MODULE_T *inf)
{
    CS_ERR err;
    
    /* 扫描状态已启动 */
    if(inf->status == SCAN_START)
    {
        err = com_module_connect(inf->next_addr + inf->offset_addr);//发送联机指令
        
        if(err == CS_ERR_SEND_SUCCESS)
        {
            inf->cur_addr = inf->next_addr;
            inf->next_addr++;
            
            if(inf->next_addr > inf->end_addr)
            {
                inf->status = SCAN_OVER;
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
void scan_all_module(void)
{
    scan_module_x(&com1_scan_module);
    scan_module_x(&com2_scan_module);
    scan_module_x(&com3_scan_module);
    scan_module_x(&com4_scan_module);
}
/**
  * @brief  启动4路串口进行模块扫描
  * @param  无
  * @retval 无
  */
void start_scan_all_module(void)
{
    com1_scan_module.status = SCAN_START;
    com2_scan_module.status = SCAN_START;
    com3_scan_module.status = SCAN_START;
    com4_scan_module.status = SCAN_START;
    
    com1_scan_module.next_addr = com1_scan_module.start_addr;
    com2_scan_module.next_addr = com2_scan_module.start_addr;
    com3_scan_module.next_addr = com3_scan_module.start_addr;
    com4_scan_module.next_addr = com4_scan_module.start_addr;
}

/**
  * @brief  停止4路串口进行模块扫描
  * @param  无
  * @retval 无
  */
void stop_scan_all_module(void)
{
    com1_scan_module.status = SCAN_STOP;
    com2_scan_module.status = SCAN_STOP;
    com3_scan_module.status = SCAN_STOP;
    com4_scan_module.status = SCAN_STOP;
}

/**
  * @brief  查询第1路串口扫描是是否结束
  * @param  无
  * @retval 0 未结束 1 结束
  */
uint8_t com1_scan_is_over(void)
{
    return com1_scan_module.status == SCAN_OVER;
}
/**
  * @brief  查询第2路串口扫描是是否结束
  * @param  无
  * @retval 0 未结束 1 结束
  */
uint8_t com2_scan_is_over(void)
{
    return com2_scan_module.status == SCAN_OVER;
}

/**
  * @brief  查询第3路串口扫描是是否结束
  * @param  无
  * @retval 0 未结束 1 结束
  */
uint8_t com3_scan_is_over(void)
{
    return com3_scan_module.status == SCAN_OVER;
}

/**
  * @brief  查询第4路串口扫描是是否结束
  * @param  无
  * @retval 0 未结束 1 结束
  */
uint8_t com4_scan_is_over(void)
{
    return com4_scan_module.status == SCAN_OVER;
}

/**
  * @brief  获取串口1当前的扫描地址
  * @param  无
  * @retval 当前扫描地址
  */
uint8_t get_com1_scan_addr(void)
{
    return com1_scan_module.cur_addr;
}
/**
  * @brief  获取串口2当前的扫描地址
  * @param  无
  * @retval 当前扫描地址
  */
uint8_t get_com2_scan_addr(void)
{
    return com2_scan_module.cur_addr;
}

/**
  * @brief  获取串口3当前的扫描地址
  * @param  无
  * @retval 当前扫描地址
  */
uint8_t get_com3_scan_addr(void)
{
    return com3_scan_module.cur_addr;
}
/**
  * @brief  获取串口4当前的扫描地址
  * @param  无
  * @retval 当前扫描地址
  */
uint8_t get_com4_scan_addr(void)
{
    return com4_scan_module.cur_addr;
}

/**
  * @brief  用户层调用针对指定地址连接模块. 在开机时使用，来连接历史记录中的模块
  * @param  [in] addr 指定的连接地址
  * @retval 发送连接命令的状态 
  *     CS_ERR_SEND_FAIL 发送失败
  *     CS_ERR_SEND_SUCCESS 发送成功
  */
CS_ERR connect_module(uint8_t addr)
{
	OS_ERR os_err;
    CS_ERR err = CS_ERR_SEND_FAIL;//初始化为发送失败
    uint8_t count = 0;
    uint8_t RETRY_TIMES = 3;//重试次数
    
    wait_com_comm_idle(addr);//等待串口进入
    
    do{
        err = com_module_connect(addr);//发送联机指令
        
        /* 发送成功就退出去 */
        if(err == CS_ERR_SEND_SUCCESS)
        {
            break;
        }
        
        count++;
        OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&os_err);
    }while(count < RETRY_TIMES);
    
    return err;
}
/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
