/**
  ******************************************************************************
  * @file    module_manage.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.7.10
  * @brief   模块管理
  ******************************************************************************
  */

#pragma  diag_suppress 186 //消除186号编译警告
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
#include "os.h"
#include "start_stop_key.h"


/* Private typedef -----------------------------------------------------------*/

/**
  * @brief  通信指令定义
  */
enum{
    GET_MODULE_INF = 1,///<获取模块信息用来联机用
    SET_MODULE_NUM = 2,///<设置模块的编号，对于多路同步测试仪要告诉每个模块在系统中的编号,用这个编号来判断 \
                    是否参与测试工作
    FORMAT_DATA     = 3,///<格式化数据
    SET_CONFIG_PARAMETER    = 4,///<设置配置参数
    
    START_SLAVE_TEST        = 10,///<启动从机测试
    STOP_SLVAE_TEST         = 11,///<停止从机测试
    GET_SLAVE_TEST_DATA     = 12,///<获取测试数据
    TEST_OVER_SIGN_H        = 13,///<控制从机测试完成信息输出高电平
    TEST_OVER_SIGN_L        = 14,///<控制从机测试完成信号输出低电平
    GET_SLAVE_TEST_TIME     = 15,///<获取从机测试时间
    
    SLAVE_NEW_FILE          = 30,///<新建文件
    SLAVE_EDIT_FILE         = 31,///<设置文件
    SLAVE_READ_FILE         = 32,///<读取文件
    SLAVE_DEL_FILE          = 33,///<删除文件
    SLAVE_SAVE_FILE         = 34,///<保存文件
    SLAVE_LOAD_FILE         = 35,///<加载文件作为当前文件
    SLAVE_CLEAR_ALL_FILES   = 36,///<清空所有文件
    
    SLAVE_INSERT_STEP       = 40,///<插入步骤
    SLAVE_LOAD_STEP         = 41,///<加载步骤作为当前步骤
    SLAVE_EDIT_STEP         = 42,///<设置步骤
    SLAVE_READ_STEP         = 43,///<读取步骤
    SLAVE_DEL_STEP          = 44,///<删除步骤
    SLAVE_PRE_STEP          = 45,///<前移步骤
    SLAVE_BACK_STEP         = 46,///<后移步骤
    SLAVE_SWAP_STEP         = 47,///<交换步骤
    
    SLAVE_READ_RES          = 50,///<读取一条结果
    SLAVE_CLEAR_RES         = 51,///<清空结果
    SLAVE_READ_LAST         = 52,///<读取最近一次测试的结果
    
    SLAVE_ENTER_CAL_ST      = 60,///<从机进入校准状态
    SLAVE_EXIT_CAL_ST       = 61,///<退出校准状态
    SLAVE_CAL_ST            = 62,///<校准状态查询
    SLAVE_CAL_MODE          = 63,///<校准模式查询
    SLAVE_CAL_POINT_RANGE   = 64,///<当前校准点量程查询
    SLAVE_CAL_START         = 65,///<启动校准输出
    SLAVE_CAL_STOP          = 66,///<停止校准输出
    SLAVE_CAL_MEASURE_VALUE = 67,///<设置校准测量值
    GET_MODULE_CAL_POINTS   = 68,///<获取模块的校准总个数
    LOAD_CAL_POINT          = 69,///<加载校准点作为当前校准点
    QUERY_CAL_POINT_INF     = 70,///<查询校准点信息
    
};


/**
  * @brief  地址偏移，第一路0，第二路16，第三路32，第四路48
  */
uint8_t addr_offset[] =
{
    COM1_BROADCAST_ADDR,///<第1路串口的广播地址
    COM2_BROADCAST_ADDR,///<第2路串口的广播地址
    COM3_BROADCAST_ADDR,///<第3路串口的广播地址
    COM4_BROADCAST_ADDR ///<第4路串口的广播地址
};

/* Private function prototypes -----------------------------------------------*/
CS_ERR com_send_cmd_data(MODULE_ADDR_T addr, uint8_t cmd, uint8_t *data, uint32_t len);
static COM_NUM get_com_num(MODULE_ADDR_T *addr, CS_ERR *err);

/**
  * @brief  根据串口编号获取偏移地址
  * @param  [in] com_num 串口编号
  * @retval 偏移地址
  */
uint8_t get_com_offset_add(COM_NUM com_num)
{
    return addr_offset[com_num];
}
/**
  * @brief  获取串口结构信息
  * @param  [in] 串口编号
  * @retval 串口结构信息
  */
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
/**
  * @brief  更新模块地址信息，每当获取到新的模块信息时，都需要更新模块信息
  * @param  无
  * @retval 无
  */
void update_module_addr_inf(void)
{
    int32_t i = 0;
    int32_t j = 0;
    MODULE_ADDR_T addr = 0;
    uint8_t offset_addr = 0;
    COM_STRUCT* com_inf = NULL;
    uint8_t id = 0;
    COM_NUM com_num;
    uint8_t n = 0;
    
    memset(&roads_flag, 0, sizeof(roads_flag));
    
    for(i = 0; i < MASTER_ADDR_RANGE; i++)
    {
        if(i == COM1_BROADCAST_ADDR
            || i == COM2_BROADCAST_ADDR
            || i == COM3_BROADCAST_ADDR
            || i == COM4_BROADCAST_ADDR)
        {
            continue;
        }
        
        id = module_inf_pool[i].module_inf.id;//id范围是1-15
        
        if(id != SLAVE_BROADCAST_ADDR && id <= SLAVE_ADDR_MAX)
        {
            com_num = module_inf_pool[i].com_num;
            n = roads_flag.flag[com_num].count;
            com_inf = get_com_inf(com_num);
            offset_addr = get_com_offset_add(module_inf_pool[i].com_num);
            addr = offset_addr + id;
            roads_flag.flag[com_num].road_buf[n] = addr;
            roads_flag.flag[com_num].count++;
            syn_test_port[j].com = com_inf;//初始化串口信息
            syn_test_port[j].road_index = j + 1;//初始化路号
            syn_test_port[j].addr = addr;
            syn_test_port[j].offset_addr = offset_addr;
            
            module_inf_pool[i].road_num = j + 1;
            j++;
        }
    }
}

/**
  * @brief  获取从机模块信息通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void set_module_inf(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    module_inf_pool[index_addr].com_num = com_num;
    memcpy(&module_inf_pool[index_addr].module_inf, data, sizeof(MODULE_INF));
    update_module_addr_inf();
}
/**
  * @brief  获取从机校准点总数通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void get_module_cal_points(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    memcpy(&cur_module_cal_points, data, sizeof(cur_module_cal_points));
}
/**
  * @brief  查询从机校准点信息通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void query_cal_point_inf(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    CAL_POINT_INF *inf = (void*)data;
    
    memcpy(&cal_point_inf_pool[inf->index], inf, sizeof(CAL_POINT_INF));
}
/**
  * @brief  从机启动校准通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_cal_start(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    ROAD_NUM_T road;
    
    road = get_road_index_num(index_addr);
    
    syn_test_port[road - 1].test_st = ROAD_TESTING;
}

/**
  * @brief  从机停止校准通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_cal_stop(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    ROAD_NUM_T road;
    
    road = get_road_index_num(index_addr);
    syn_test_port[road - 1].test_st = ROAD_STOPPING;
}


/**
  * @brief  从机新建文件通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_new_file(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}
/**
  * @brief  从机编辑文件通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_edit_file(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}
/**
  * @brief  从机读取文件通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_read_file(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}
/**
  * @brief  从机删除文件通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_del_file(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}
/**
  * @brief  从机保存文件通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_save_file(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}
/**
  * @brief  从机加载文件通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_load_file(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}
/**
  * @brief  从机清空所有文件通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_clear_all_files(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}




/**
  * @brief  从机插入测试步骤通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_insert_step(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}

/**
  * @brief  从机加载测试步骤通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_load_step(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}

/**
  * @brief  从机删除测试步骤通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_del_step(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}

/**
  * @brief  从机读取测试步骤通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_read_step(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}
/**
  * @brief  从机前移测试步骤通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_pre_step(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}
/**
  * @brief  从机后移测试步骤通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_back_step(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}

/**
  * @brief  从机交换测试步骤通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void slave_swap_step(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
}


/**
  * @brief  获取测试数据通信指令返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void get_slave_test_data(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    ROAD_NUM_T road;
    
    road = get_road_index_num(index_addr);
    memcpy(&syn_test_port[road - 1].test_data, data, sizeof(UN_COMM_TEST_DATA));
}

/**
  * @brief  测试完成信息置低通信返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void get_slave_test_time(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    
}

/**
  * @brief  测试完成信息置低通信返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void test_over_sign_l(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    comm_syn_sign = 1;
}

/**
  * @brief  测试完成信息置高通信返回处理函数
  * @param  [in] com_num 串口编辑
  * @param  [in] index_addr 索引地址
  * @param  [in] data 返回数据
  * @retval 无
  */
void test_over_sign_h(COM_NUM com_num, uint8_t index_addr, uint8_t *data)
{
    CS_BOOL res = CS_FALSE;
    SYN_TEST_PORT_INF *inf;
    CS_ERR err;
    
    inf = get_road_inf(index_addr, &err);
    
    if(err != CS_ERR_NONE)
    {
        return;
    }
    
    if(res == CS_FALSE)
    {
        res = road2_test_over();
        
        if(res == CS_TRUE)
        {
            inf->road_num = 2;
        }
    }
    
    if(res == CS_FALSE)
    {
        res = road1_test_over();
        
        if(res == CS_TRUE)
        {
            inf->road_num = 1;
        }
    }
    
    if(res == CS_FALSE)
    {
        res = road3_test_over();
        
        if(res == CS_TRUE)
        {
            inf->road_num = 3;
        }
    }
    
    if(res == CS_FALSE)
    {
        res = road4_test_over();
        
        if(res == CS_TRUE)
        {
            inf->road_num = 4;
        }
    }
    
    comm_syn_sign = 1;
}

/**
  * @brief  根据路号获取模块测试数据
  * @param  [in] road 路号
  * @param  [out] test_data 输出测试数据
  * @retval 获取到的模块测试数据
  */
UN_COMM_TEST_DATA* get_road_test_data(ROAD_NUM_T road, UN_COMM_TEST_DATA *test_data)
{
    uint8_t total_roads = 0;
    int32_t i = 0;
    
    total_roads = get_total_roads_num();
    
    for(i = 0; i < total_roads; i++)
    {
        if(syn_test_port[i].road_num == road)
        {
            break;
        }
    }
    
    if(i == total_roads)
    {
        return NULL;
    }
    
    memcpy(test_data, &syn_test_port[i].test_data, sizeof(UN_COMM_TEST_DATA));
    
    return &syn_test_port[i].test_data;
}

/**
  * @brief  根据路号获取模块测试数据
  * @param  [in] road 路号
  * @param  [out] test_data 输出测试数据
  * @retval 获取到的模块测试数据
  */
COM_FRAME* get_road_test_data_fpga(ROAD_NUM_T road, COM_FRAME *test_data)
{
    uint8_t total_roads = 0;
    int32_t i = 0;
    
    total_roads = get_total_roads_num();
    
    for(i = 0; i < total_roads; i++)
    {
        if(syn_test_port[i].road_num == road)
        {
            break;
        }
    }
    
    if(i == total_roads)
    {
        return NULL;
    }
    
    memcpy(test_data, &syn_test_port[i].com_frame, sizeof(COM_FRAME));
    
    return &syn_test_port[i].com_frame;
}
#define TEST_DATA_FPGA_ROAD1_CMD   0x02000000
#define TEST_DATA_FPGA_ROAD2_CMD   0x02100000
#define TEST_DATA_FPGA_ROAD3_CMD   0x02200000
#define TEST_DATA_FPGA_ROAD4_CMD   0x02300000
void module_delay_ms(uint32_t time)
{
    uint32_t i = 0;
    
    
    while(time--)
    {
        for(i = 0; i < 16 * 1000; i++)
        {
        }
    }
}
void read_one_road_test_data_fpga(ROAD_NUM_T road)
{
    uint32_t cmd_buf[]={
        TEST_DATA_FPGA_ROAD1_CMD,
        TEST_DATA_FPGA_ROAD2_CMD,
        TEST_DATA_FPGA_ROAD3_CMD,
        TEST_DATA_FPGA_ROAD4_CMD,
    };
    COM_FRAME *frame = &syn_test_port[road - 1].com_frame;
    
    ReadDataFromFPGA(cmd_buf[road - 1], sizeof(COM_FRAME), (u8 *)frame);
}
void read_test_data_fpga(void)
{
    read_one_road_test_data_fpga(INDEX_ROAD_1);
//    module_delay_ms(10);
    read_one_road_test_data_fpga(INDEX_ROAD_2);
//    module_delay_ms(10);
    read_one_road_test_data_fpga(INDEX_ROAD_3);
//    module_delay_ms(10);
    read_one_road_test_data_fpga(INDEX_ROAD_4);
}
/**
  * @brief  根据路号获取模块测试状态
  * @param  [in] road 路号
  * @retval 获取到的模块测试状态
  */
ROAD_TEST_ST read_road_test_status(ROAD_NUM_T road)
{
    return syn_test_port[road - 1].test_st;
}
/* Public functions ---------------------------------------------------------*/
/**
  * @brief  根据路号获取模块地址
  * @param  [in] road 路号
  * @param  [in] err 错误码 
  *         @arg CS_ERR_NONE 成功获取到模块地址
  *         @arg CS_ERR_ROAD_INVALTD 传入的路号非法，系统中没有对应的模块地址
  * @retval 获取到的模块地址
  */
MODULE_ADDR_T get_module_addr(ROAD_INDEX road, CS_ERR *err)
{
    int32_t i = 0;
    
    for(i = 0; i < SYN_MAX_ROADS; i++)
    {
        if(syn_test_port[i].road_num == road)
        {
            *err = CS_ERR_NONE;
            return syn_test_port[i].addr;
        }
    }
    
    *err = CS_ERR_ROAD_INVALTD;
    
    return 0;
}
/**
  * @brief  根据路号获取模块地址
  * @param  [in] road 路号
  * @param  [in] err 错误码 
  *         @arg CS_ERR_NONE 成功获取到模块地址
  *         @arg CS_ERR_ROAD_INVALTD 传入的路号非法，系统中没有对应的模块地址
  * @retval 获取到的模块地址
  */
MODULE_ADDR_T get_module_index_addr(ROAD_INDEX road, CS_ERR *err)
{
    int32_t i = 0;
    
    for(i = 0; i < SYN_MAX_ROADS; i++)
    {
        if(syn_test_port[i].road_index == road)
        {
            *err = CS_ERR_NONE;
            return syn_test_port[i].addr;
        }
    }
    
    *err = CS_ERR_ROAD_INVALTD;
    
    return 0;
}
/**
  * @brief  获取测试路的信息
  * @param  [in] addr 测试路的模块地址(全局地址)
  * @param  [in] err 错误码 
  *         @arg CS_ERR_NONE 成功获取到模块地址
  *         @arg CS_ERR_ROAD_INVALTD 传入的路号非法，系统中没有对应的模块地址
  * @retval 获取到的模块信息结构
  */
SYN_TEST_PORT_INF* get_road_inf(MODULE_ADDR_T addr, CS_ERR *err)
{
    CS_INDEX i = 0;
    MODULE_ADDR_T module_addr;
    
    for(i = 0; i < SYN_MAX_ROADS; i++)
    {
        module_addr = syn_test_port[i].addr;
        
        if(module_addr == addr)
        {
            *err = CS_ERR_NONE;
            return &syn_test_port[i];
        }
    }
    
    *err = CS_ERR_ROAD_INVALTD;
    
    return NULL;
}

/**
  * @brief  第1路测试完成判断
  * @param  无
  * @retval 判断测试完成的结果
  *         @arg CS_TRUE 测试完成
  *         @arg CS_FALSE 测试未完成
  */
CS_BOOL road1_test_over(void)
{
    return ROAD1_TEST_OVER_PIN == TEST_OVER_Y? CS_TRUE : CS_FALSE;
}

/**
  * @brief  第2路测试完成判断
  * @param  无
  * @retval 判断测试完成的结果
  *         @arg CS_TRUE 测试完成
  *         @arg CS_FALSE 测试未完成
  */
CS_BOOL road2_test_over(void)
{
    return ROAD2_TEST_OVER_PIN == TEST_OVER_Y? CS_TRUE : CS_FALSE;
}
/**
  * @brief  第3路测试完成判断
  * @param  无
  * @retval 判断测试完成的结果
  *         @arg CS_TRUE 测试完成
  *         @arg CS_FALSE 测试未完成
  */
CS_BOOL road3_test_over(void)
{
    return ROAD3_TEST_OVER_PIN == TEST_OVER_Y? CS_TRUE : CS_FALSE;
}
/**
  * @brief  第4路测试完成判断
  * @param  无
  * @retval 判断测试完成的结果
  *         @arg CS_TRUE 测试完成
  *         @arg CS_FALSE 测试未完成
  */
CS_BOOL road4_test_over(void)
{
    return ROAD4_TEST_OVER_PIN == TEST_OVER_Y? CS_TRUE : CS_FALSE;
}

/**
  * @brief  第1路测试报警判断
  * @param  无
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL road1_test_alarm(void)
{
    return ROAD1_TEST_ALARM_PIN == TEST_ALARM_Y? CS_TRUE : CS_FALSE;
}

/**
  * @brief  第2路测试报警判断
  * @param  无
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL road2_test_alarm(void)
{
    return ROAD2_TEST_ALARM_PIN == TEST_ALARM_Y? CS_TRUE : CS_FALSE;
}

/**
  * @brief  第3路测试报警判断
  * @param  无
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL road3_test_alarm(void)
{
    return ROAD3_TEST_ALARM_PIN == TEST_ALARM_Y? CS_TRUE : CS_FALSE;
}
/**
  * @brief  第4路测试报警判断
  * @param  无
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL road4_test_alarm(void)
{
    return ROAD4_TEST_ALARM_PIN == TEST_ALARM_Y? CS_TRUE : CS_FALSE;
}


/**
  * @brief  公共指令集指令处理
  * @param  [in] com_num 串口编号
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL com_cmd_dispose(COM_NUM com_num, uint8_t index_addr, FRAME_T *frame)
{
    CS_BOOL res = CS_TRUE;
    
    /* 指令解析 */
    switch(frame->fun_code)
    {
        case GET_MODULE_INF:
            set_module_inf(com_num, index_addr, frame->data);
            break;
        case SET_MODULE_NUM:
            break;
        case FORMAT_DATA:
            break;
        case SET_CONFIG_PARAMETER:
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}


/**
  * @brief  源指令集指令处理
  * @param  [in] com_num 串口编号
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL source_cmd_dispose(COM_NUM com_num, uint8_t index_addr, FRAME_T *frame)
{
    CS_BOOL res = CS_TRUE;
    
    /* 指令解析 */
    switch(frame->fun_code)
    {
        case START_SLAVE_TEST:
            break;
        case STOP_SLVAE_TEST:
            break;
        case GET_SLAVE_TEST_DATA:
            get_slave_test_data(com_num, index_addr, frame->data);
            break;
        case TEST_OVER_SIGN_H:
            test_over_sign_h(com_num, index_addr, frame->data);
            break;
        case TEST_OVER_SIGN_L:
            test_over_sign_l(com_num, index_addr, frame->data);
            break;
        case GET_SLAVE_TEST_TIME:
            get_slave_test_time(com_num, index_addr, frame->data);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}

/**
  * @brief  文件指令集指令处理
  * @param  [in] com_num 串口编号
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL file_cmd_dispose(COM_NUM com_num, uint8_t index_addr, FRAME_T *frame)
{
    CS_BOOL res = CS_TRUE;
    
    /* 指令解析 */
    switch(frame->fun_code)
    {
        case SLAVE_NEW_FILE:
            slave_new_file(com_num, index_addr, frame->data);
            break;
        case SLAVE_EDIT_FILE:
            slave_edit_file(com_num, index_addr, frame->data);
            break;
        case SLAVE_READ_FILE:
            slave_read_file(com_num, index_addr, frame->data);
            break;
        case SLAVE_DEL_FILE:
            slave_del_file(com_num, index_addr, frame->data);
            break;
        case SLAVE_SAVE_FILE:
            slave_save_file(com_num, index_addr, frame->data);
            break;
        case SLAVE_LOAD_FILE:
            slave_load_file(com_num, index_addr, frame->data);
            break;
        case SLAVE_CLEAR_ALL_FILES:
            slave_clear_all_files(com_num, index_addr, frame->data);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}

/**
  * @brief  步骤指令集指令处理
  * @param  [in] com_num 串口编号
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL step_cmd_dispose(COM_NUM com_num, uint8_t index_addr, FRAME_T *frame)
{
    CS_BOOL res = CS_TRUE;
    
    /* 指令解析 */
    switch(frame->fun_code)
    {
        case SLAVE_INSERT_STEP:
            slave_insert_step(com_num, index_addr, frame->data);
            break;
        case SLAVE_LOAD_STEP:
            slave_load_step(com_num, index_addr, frame->data);
            break;
        case SLAVE_READ_STEP:
            slave_read_step(com_num, index_addr, frame->data);
            break;
        case SLAVE_DEL_STEP:
            slave_del_step(com_num, index_addr, frame->data);
            break;
        case SLAVE_PRE_STEP:
            slave_pre_step(com_num, index_addr, frame->data);
            break;
        case SLAVE_BACK_STEP:
            slave_back_step(com_num, index_addr, frame->data);
            break;
        case SLAVE_SWAP_STEP:
            slave_swap_step(com_num, index_addr, frame->data);
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}

/**
  * @brief  结果指令集指令处理
  * @param  [in] com_num 串口编号
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL res_cmd_dispose(COM_NUM com_num, uint8_t index_addr, FRAME_T *frame)
{
    CS_BOOL res = CS_TRUE;
    
    /* 指令解析 */
    switch(frame->fun_code)
    {
        case GET_MODULE_INF:
            set_module_inf(com_num, index_addr, frame->data);
            break;
        case SET_MODULE_NUM:
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}

/**
  * @brief  校准指令集指令处理
  * @param  [in] com_num 串口编号
  * @retval 判断测试报警的结果
  *         @arg CS_TRUE 测试发生报警异常
  *         @arg CS_FALSE 测试未发生报警异常
  */
CS_BOOL cal_cmd_dispose(COM_NUM com_num, uint8_t index_addr, FRAME_T *frame)
{
    CS_BOOL res = CS_TRUE;
    
    /* 指令解析 */
    switch(frame->fun_code)
    {
        case GET_MODULE_CAL_POINTS:
            get_module_cal_points(com_num, index_addr, frame->data);
            break;
        case SLAVE_ENTER_CAL_ST:
            break;
        case QUERY_CAL_POINT_INF:
            query_cal_point_inf(com_num, index_addr, frame->data);
            break;
        case SLAVE_EXIT_CAL_ST:
            break;
        case SLAVE_CAL_ST:
            break;
        case SLAVE_CAL_MODE:
            break;
        case SLAVE_CAL_POINT_RANGE:
            break;
        case SLAVE_CAL_START:
            slave_cal_start(com_num, index_addr, frame->data);
            break;
        case SLAVE_CAL_STOP:
            slave_cal_stop(com_num, index_addr, frame->data);
            break;
        case SLAVE_CAL_MEASURE_VALUE:
            break;
        default:
            res = CS_FALSE;
            break;
    }
    
    return res;
}
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
    uint8_t index_addr = 0;
    COM_STRUCT *com_inf;
    CS_BOOL res;
    
    
    if(len <= CRC_LEN)
    {
        return;
    }
    
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
    
    index_addr = addr_offset[com_num] + frame->addr;
    
    /* 公共指令处理 */
    res = com_cmd_dispose(com_num, index_addr, frame);
    
    /* 源指令处理 */
    if(res == CS_FALSE)
    {
        res = source_cmd_dispose(com_num, index_addr, frame);
    }
    
    /* 文件指令处理 */
    if(res == CS_FALSE)
    {
        res = file_cmd_dispose(com_num, index_addr, frame);
    }
    
    /* 步骤指令处理 */
    if(res == CS_FALSE)
    {
        res = step_cmd_dispose(com_num, index_addr, frame);
    }
    
    /* 结果指令处理 */
    if(res == CS_FALSE)
    {
        res = res_cmd_dispose(com_num, index_addr, frame);
    }
    
    /* 校准指令处理 */
    if(res == CS_FALSE)
    {
        res = cal_cmd_dispose(com_num, index_addr, frame);
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
MODULE_COMM_STATUS get_com_st(MODULE_ADDR_T addr)
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
  * @retval CS_FALSE 不在空闲状态 CS_TRUE 在空闲状态
  */
CS_BOOL com_comm_is_idle(MODULE_ADDR_T addr)
{
    MODULE_COMM_STATUS st;
    
    st = get_com_st(addr);
    
    return st == MODULE_COMM_IDLE? CS_TRUE:CS_FALSE;
}

/**
  * @brief  等待串口进入空闲状态
  * @param  [in] addr 指定的模块地址
  * @retval 无
  */
void wait_com_comm_idle(MODULE_ADDR_T addr)
{
	OS_ERR os_err;
    CS_BOOL flag = CS_FALSE;
    
    while(CS_TRUE != flag)
    {
        flag = com_comm_is_idle(addr);
        OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&os_err);
    }
}

/**
  * @brief  根据提供的地址来计算出串串口编辑
  * @param  [in] addr 通信地址
  * @param  [out] err 错误码
  * @retval 串口编号
  */
static COM_NUM get_com_num(MODULE_ADDR_T *addr, CS_ERR *err)
{
    COM_NUM com_num;
    
    *err = CS_ERR_NONE;
    
    /* 0-15是第1路串口的地址范围 */
    if(*addr >= COM1_BROADCAST_ADDR
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
  * @brief  根据通信地址来查找模块的索引编号
  * @param  [in] addr 通信地址
  * @retval 模块路编号
  */
ROAD_NUM_T get_road_index_num(MODULE_ADDR_T addr)
{
    int32_t i = 0;
    ROAD_NUM_T road_num = 0;
    
    for(i = 0; i < SYN_MAX_ROADS; i++)
    {
        if(addr == syn_test_port[i].addr)
        {
            road_num = syn_test_port[i].road_index;
        }
    }
    
    return road_num;
}
/**
  * @brief  根据通信地址来查找模块的编号
  * @param  [in] addr 通信地址
  * @retval 模块路编号
  */
ROAD_NUM_T get_road_num(MODULE_ADDR_T addr)
{
    int32_t i = 0;
    ROAD_NUM_T road_num = 0;
    
    for(i = 0; i < SYN_MAX_ROADS; i++)
    {
        if(addr == syn_test_port[i].addr)
        {
            road_num = syn_test_port[i].road_num;
        }
    }
    
    return road_num;
}
/**
  * @brief  发送通信模块的连接指令
  * @param  [in] addr 通信地址
  * @retval 发送结果
  */
CS_ERR com_module_connect(MODULE_ADDR_T addr)
{
    return com_send_cmd_data(addr, GET_MODULE_INF, NULL, 0);
}

/**
  * @brief  发送通信模块的连接指令
  * @param  [in] addr 通信地址
  * @param  [in] data 指令数据
  * @param  [in] len 指令数据长度
  * @retval 发送结果
  */
CS_ERR send_module_connect(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, GET_MODULE_INF, NULL, 0);
}
/**
  * @brief  发送格式化存储指令
  * @param  [in] addr 通信地址
  * @param  [in] data 指令数据
  * @param  [in] len 指令数据长度
  * @retval 发送结果
  */
CS_ERR send_format_data(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, FORMAT_DATA, data, len);
}
/**
  * @brief  发送配置参数指令
  * @param  [in] addr 通信地址
  * @param  [in] data 指令数据
  * @param  [in] len 指令数据长度
  * @retval 发送结果
  */
CS_ERR send_set_config_parameter(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SET_CONFIG_PARAMETER, data, len);
}
/**
  * @brief  发送查询校准点总个数指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_query_cal_points(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, GET_MODULE_CAL_POINTS, NULL, 0);
}
/**
  * @brief  发送查询校准点信息指令
  * @param  [in] addr 通信地址
  * @param  [in] data 指令数据
  * @param  [in] len 指令数据长度
  * @retval 发送结果
  */
CS_ERR send_query_cal_point_inf(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, QUERY_CAL_POINT_INF, data, len);
}
/**
  * @brief  发送从机进入校准状态指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_enter_cal_st(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_ENTER_CAL_ST, NULL, 0);
}

/**
  * @brief  发送启动校准指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_start_cal(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_CAL_START, NULL, 0);
}
/**
  * @brief  发送停止校准指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_stop_cal(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_CAL_STOP, NULL, 0);
}
/**
  * @brief  发送设置测量值指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_set_measure_value(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_CAL_MEASURE_VALUE, data, len);
}
/**
  * @brief  发送加载校准点指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_load_cur_point(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, LOAD_CAL_POINT, data, len);
}
/**
  * @brief  发送从机退出校准状态指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_exit_cal_st(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_EXIT_CAL_ST, NULL, 0);
}
/**
  * @brief  发送获取从机测试数据指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_get_test_data(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, GET_SLAVE_TEST_DATA, NULL, 0);
}
/**
  * @brief  发送控制从机测试完成信号输出高电平
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_test_over_sign_h(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, TEST_OVER_SIGN_H, NULL, 0);
}
/**
  * @brief  发送控制从机测试完成信号输出低电平
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_test_over_sign_l(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, TEST_OVER_SIGN_L, NULL, 0);
}
/**
  * @brief  发送控制从机测试完成信号输出低电平
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_get_slave_test_time(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, GET_SLAVE_TEST_TIME, data, len);
}
/**
  * @brief  发送启动从机进行测试指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_start_test(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, START_SLAVE_TEST, NULL, 0);
}
/**
  * @brief  发送从机新建文件指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_new_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_NEW_FILE, data, len);
}
/**
  * @brief  发送从机读取文件指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_read_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_READ_FILE, data, len);
}
/**
  * @brief  发送从机编辑文件指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_edit_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_EDIT_FILE, data, len);
}
/**
  * @brief  发送从机清空所有文件指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_clear_all_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_EDIT_FILE, data, len);
}
/**
  * @brief  发送从机删除文件指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_del_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_DEL_FILE, data, len);
}
/**
  * @brief  发送保存文件指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_save_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_SAVE_FILE, data, len);
}
/**
  * @brief  发送加载文件指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_load_file(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_LOAD_FILE, data, len);
}
/**
  * @brief  发送清空所有文件指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_slave_clear_all_files(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_CLEAR_ALL_FILES, data, len);
}

/**
  * @brief  发送插入步骤指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_insert_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_INSERT_STEP, data, len);
}
/**
  * @brief  发送编辑步骤指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_edit_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_EDIT_STEP, data, len);
}

/**
  * @brief  发送加载步骤指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_load_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_LOAD_STEP, data, len);
}
/**
  * @brief  发送删除步骤指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_del_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_DEL_STEP, data, len);
}
/**
  * @brief  发送交换步骤指令
  * @param  [in] addr 通信地址
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  */
CS_ERR send_swap_step(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    return com_send_cmd_data(addr, SLAVE_SWAP_STEP, data, len);
}
/**
  * @brief  发送设置模块编号指令
  * @param  [in] addr 通信地址
  * @retval 发送结果
  */
CS_ERR com_module_set_road_num(MODULE_ADDR_T addr, uint8_t *data, uint32_t len)
{
    ROAD_NUM_T road_num = 0;
    
    road_num = get_road_num(addr);
    
    if(road_num == 0)
    {
        return CS_ERR_COMM_ADDR_INVALTD;
    }
    
    return com_send_cmd_data(addr, SET_MODULE_NUM, &road_num, sizeof(road_num));
}

/**
  * @brief  判断模块所挂的串口是否处于空闲状态
  * @param  [in] addr 模块通信地址
  * @retval 判断结果
  *        @arg CS_TRUE 串口在空闲状态
  *        @arg CS_FALSE 串口不在空闲状态
  */
CS_BOOL com_is_idle(MODULE_ADDR_T addr)
{
    COM_STRUCT *com_inf;
    CS_ERR err;
    COM_NUM com_num;
    
    com_num = get_com_num(&addr, &err);//获取串口号
    
    /* 如果地址非法就返回 */
    if(err != CS_ERR_NONE)
    {
        return CS_TRUE;
    }
    
    com_inf = get_com_inf(com_num);//根据串口号来获取串口信息
    
    /* 通信状态机只有在空闲状态时才允许发送下一条指令 */
    if(com_inf->status != MODULE_COMM_IDLE)
    {
        return CS_FALSE;
    }
    
    return CS_TRUE;
}
/**
  * @brief  发送命令给从机
  * @param  [in] addr 通信地址
  * @param  [in] cmd 功能码
  * @param  [in] data 数据
  * @param  [in] len 数据长度
  * @retval 发送结果
  *     CS_ERR_COMM_ADDR_INVALTD 通信地址非法
  *     CS_ERR_COM_BUSY 串口发送的上一条指令还没完成——发送后要有返回数据才能发送下一条命令
  *     CS_ERR_SEND_SUCCESS  发送成功
  */
CS_ERR com_send_cmd_data(MODULE_ADDR_T addr, uint8_t cmd, uint8_t *data, uint32_t len)
{
    FRAME_T *frame;
    uint32_t frame_len = 0;
    COM_STRUCT *com_inf;
    CS_ERR err;
    COM_NUM com_num;
    
    com_num = get_com_num(&addr, &err);//获取串口号
    
    /* 如果地址非法就返回 */
    if(err != CS_ERR_NONE)
    {
        return CS_ERR_COMM_ADDR_INVALTD;
    }
    
    com_inf = get_com_inf(com_num);//根据串口号来获取串口信息
    
    /* 通信状态机只有在空闲状态时才允许发送下一条指令 */
    if(com_inf->status != MODULE_COMM_IDLE)
    {
        return CS_ERR_COM_BUSY;
    }
    
    frame = (void*)com_inf->buf;
    
    frame->addr = addr;
    frame->fun_code = cmd;
    frame->st = 0;
    frame->data_len = len;
    memcpy(frame->data, data, len);
    frame_len = len + FRAME_HEAD_SIZE;
    add_crc_to_send_data((uint8_t*)frame, frame_len);
    frame_len += CRC_LEN;
    
    com_inf->send_fun(com_inf, (uint8_t *)frame, frame_len);
    com_inf->set_wait_ack_timeout(com_inf);
    
    /* 广播 */
    if(addr == 0)
    {
        com_inf->status = MODULE_COMM_SEND_BROADCAST;//状态机进入广播发送状态
    }
    else
    {
        com_inf->status = MODULE_COMM_SEND;//状态机进入发送状态
    }
    
    return CS_ERR_SEND_SUCCESS;
}
/**
  * @brief  初始化模块管理的环境
  * @param  无
  * @retval 无
  */
void init_module_manage_env(void)
{
    /* 为了避免函数多次调用 */
    if(module_inf_pool != NULL)
    {
        return;
    }
    
    /* 初始化各路串口的运行环境 */
    com1.init_com_env(&com1);
    com2.init_com_env(&com2);
    com3.init_com_env(&com3);
    com4.init_com_env(&com4);
    
    /* 为模块信息分配内存 */
    module_inf_pool = malloc_ex_mem(sizeof(ROAD_INF) * MASTER_ADDR_RANGE);//分配内存，不会再释放了
    
    /* 初始化分配到的内存 */
    if(module_inf_pool != NULL)
    {
        clear_module_inf();
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
    memset(module_inf_pool, 0, sizeof(ROAD_INF) * MASTER_ADDR_RANGE);
    memset(syn_test_port, 0, sizeof(syn_test_port));
}
/**
  * @brief  获取总的路数
  * @param  无
  * @retval 总的路数
  */
uint8_t get_total_roads_num(void)
{
    int32_t i =  0;
    
    for(i = 0; i < SYN_MAX_ROADS; i++)
    {
        if(syn_test_port[i].road_index == 0)
        {
            break;
        }
    }
    
    return i;
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
