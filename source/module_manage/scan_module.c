
#include "scan_module.h"
#include "os.h"

typedef enum{

    SCAN_STOP,///<停止扫描
    SCAN_START,///<启动扫描
    SCAN_OVER,///<启动扫描
    SCAN_INIT,///<扫描初始化值
}SCAN_MODULE_ST;

typedef struct{
    uint8_t status;///<扫描状态
    uint8_t start_addr;///<起始地址
    uint8_t end_addr;///<结束地址
    uint8_t offset_addr;///<偏移地址 串口1的偏移地址是0 串口2的偏移地址是16 串口3是32 串口4是48
    uint8_t next_addr;///<等待扫描的地址
    uint8_t cur_addr;///<当前的扫描地址
}SCAN_MODULE_T;

static SCAN_MODULE_T com1_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    15,///<结束地址
    0,///<偏移地址
};
static SCAN_MODULE_T com2_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    15,///<结束地址
    16,///<偏移地址
};
static SCAN_MODULE_T com3_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    15,///<结束地址
    32,///<偏移地址
};
static SCAN_MODULE_T com4_scan_module=
{
    SCAN_INIT,///<扫描状态
    1,///<起始地址
    15,///<结束地址
    48,///<偏移地址
};



uint8_t get_com1_scan_addr(void)
{
    return com1_scan_module.cur_addr;
}
uint8_t get_com2_scan_addr(void)
{
    return com2_scan_module.cur_addr;
}

uint8_t get_com3_scan_addr(void)
{
    return com3_scan_module.cur_addr;
}
uint8_t get_com4_scan_addr(void)
{
    return com4_scan_module.cur_addr;
}

CS_ERR connect_module(uint8_t addr)
{
	OS_ERR os_err;
    CS_ERR err;
    uint8_t count = 0;
    uint8_t RETRY_TIMES = 3;
    
    do{
        err = com_module_connect(addr);
        count++;
        OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_PERIODIC,&os_err);
    }while(err != CS_ERR_SEND_SUCCESS && count < RETRY_TIMES);
    
    if(count == RETRY_TIMES && err != CS_ERR_SEND_SUCCESS)
    {
        return CS_ERR_SEND_FAIL;
    }
    
    return CS_ERR_SEND_SUCCESS;
}

uint8_t scan_module_x(SCAN_MODULE_T *inf)
{
    CS_ERR err;
    
    if(inf->status == SCAN_START)
    {
        err = com_module_connect(inf->next_addr + inf->offset_addr);
        
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
    
    return inf->status;
}

void scan_all_module(void)
{
    scan_module_x(&com1_scan_module);
    scan_module_x(&com2_scan_module);
    scan_module_x(&com3_scan_module);
    scan_module_x(&com4_scan_module);
}
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

void stop_scan_all_module(void)
{
    com1_scan_module.status = SCAN_STOP;
    com2_scan_module.status = SCAN_STOP;
    com3_scan_module.status = SCAN_STOP;
    com4_scan_module.status = SCAN_STOP;
}

uint8_t com1_scan_is_over(void)
{
    return com1_scan_module.status == SCAN_OVER;
}
uint8_t com2_scan_is_over(void)
{
    return com2_scan_module.status == SCAN_OVER;
}

uint8_t com3_scan_is_over(void)
{
    return com3_scan_module.status == SCAN_OVER;
}

uint8_t com4_scan_is_over(void)
{
    return com4_scan_module.status == SCAN_OVER;
}

