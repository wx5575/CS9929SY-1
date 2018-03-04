/**
  ******************************************************************************
  * @file    cs99xx_sdcard_manage.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.10.8
  * @brief   sdcard应用层代码
  ******************************************************************************
  */
#include "cs99xx_sdcard_manage.h"
#include "spi_flash.h"
#include "os.h"
#include "string.h"
#include "ff.h"

void read_one_result_sd(uint32_t result_count, RESULT_INF *res, CS_ERR *err)
{
    FRESULT fresult;
    FIL f;
    uint32_t one_res_size;
    uint32_t real_res_size;
    
    fresult = f_open (&f, (const char*)"\\ROOT\\RESULT\\RESULT.BIN", FA_READ | FA_OPEN_ALWAYS);
    *err = CS_ERR_NONE;
    
    if(fresult != FR_OK)
    {
        *err = CS_ERR_READ_RES_FAIL;
        goto end;
    }
    
    one_res_size = sizeof(RESULT_INF);
    
    fresult = f_lseek (&f, (result_count - 1) * one_res_size);
    
    if(fresult != FR_OK)
    {
        *err = CS_ERR_READ_RES_FAIL;
        goto end;
    }
    
    fresult = f_read (&f, res, one_res_size, &real_res_size);
    
    if(fresult != FR_OK)
    {
        *err = CS_ERR_READ_RES_FAIL;
        goto end;
    }
    
    end:
    f_close(&f);
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
