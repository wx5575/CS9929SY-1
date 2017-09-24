
#include "ui_com/com_ui_info.h"
#include "IMAGE.H"
#include "ff.h"
#include "mem_alloc.h"

IMAGE_Handle create_logo_imagex(WM_HWIN hWin)
{
    IMAGE_Handle handle;
    uint8_t *bmpBuffer;
    FRESULT res;
    GUI_HMEM hmem;
    FIL f;
    uint32_t width = 0;
    uint32_t height = 0;
    
    res = f_open (&f, "\\ROOT\\IMAGE\\mlogo.bmp", FA_READ);
    
    if(res == FR_OK)
    {
//        bmpBuffer = malloc_ex_mem(f.fsize);
        
        hmem = GUI_ALLOC_AllocZero(f.fsize);
        bmpBuffer = GUI_ALLOC_h2p(hmem);
        
        if(bmpBuffer != NULL)
        {
            f_read(&f, bmpBuffer, f.fsize, NULL);
            width = *(uint32_t*)&bmpBuffer[18];
            height = *(uint32_t*)&bmpBuffer[18+4];
            
            handle = IMAGE_CreateUser(10, 10, width, height, hWin,
                WM_CF_SHOW, 0, id_base++, 0);
            
            IMAGE_SetBMP(handle, bmpBuffer, f.fsize);
//            free_ex_mem(bmpBuffer);
//            GUI_ALLOC_Free(hmem);
            bmpBuffer = NULL;
        }
        
        f_close(&f);
    }
    
    return handle;
}

IMAGE_Handle create_logo_image(WM_HWIN hWin)
{
    IMAGE_Handle handle;
    uint8_t *bmpBuffer;
    FRESULT res;
    GUI_HMEM hmem;
    FIL f;
    uint32_t width = 0;
    uint32_t height = 0;
    
    res = f_open (&f, "\\ROOT\\IMAGE\\mlogo.bmp", FA_READ);
    
    if(res == FR_OK)
    {
//        bmpBuffer = malloc_ex_mem(f.fsize);
        
        hmem = GUI_ALLOC_AllocZero(f.fsize);
        bmpBuffer = GUI_ALLOC_h2p(hmem);
        
        if(bmpBuffer != NULL)
        {
            f_read(&f, bmpBuffer, f.fsize, NULL);
            width = *(uint32_t*)&bmpBuffer[18];
            height = *(uint32_t*)&bmpBuffer[18+4];
            
            handle = IMAGE_CreateUser((800-width) / 2, (480 - height) / 2, width, height, hWin,
                WM_CF_SHOW, 0, id_base++, 0);
            
            IMAGE_SetBMP(handle, bmpBuffer, f.fsize);
//            free_ex_mem(bmpBuffer);
//            GUI_ALLOC_Free(hmem);
            bmpBuffer = NULL;
        }
        
        f_close(&f);
    }
    
    return handle;
}

IMAGE_Handle create_slogo_image(WM_HWIN hWin)
{
    IMAGE_Handle handle;
    uint8_t *bmpBuffer;
    FRESULT res;
    FIL f;
    int32_t width = 0;
    int32_t height = 0;
    GUI_HMEM hmem;
    uint32_t data_offset = 0;
    
    res = f_open (&f, "\\ROOT\\IMAGE\\slogo.bmp", FA_READ);
    
    if(res == FR_OK)
    {
        bmpBuffer = malloc_ex_mem(f.fsize);
//        hmem = GUI_ALLOC_AllocZero(f.fsize);
//        bmpBuffer = GUI_ALLOC_h2p(hmem);
        
        if(bmpBuffer != NULL)
        {
            f_read(&f, bmpBuffer, f.fsize, NULL);
            data_offset = *(uint32_t*)&bmpBuffer[10];
            width = *(int32_t*)&bmpBuffer[18];
            height = *(int32_t*)&bmpBuffer[18+4];
            if(height < 0)
            {
                height = -1 * height;
            }
//            RA8875_DrawBMP(10, 10, height, width, &bmpBuffer[data_offset]);
            handle = IMAGE_CreateUser(10, 10, width, height, hWin,
                WM_CF_SHOW, 0, id_base++, 0);
            
            IMAGE_SetBMP(handle, bmpBuffer, f.fsize);
//            free_ex_mem(bmpBuffer);
//            GUI_ALLOC_Free(hmem);
            bmpBuffer = NULL;
        }
        
        f_close(&f);
    }
    
    return handle;
}

IMAGE_Handle create_main_image(WM_HWIN hWin)
{
    IMAGE_Handle handle;
    uint8_t *bmpBuffer;
    FRESULT res;
    FIL f;
    uint32_t width = 0;
    uint32_t height = 0;
    
    res = f_open (&f, "\\ROOT\\IMAGE\\main.bmp", FA_READ);
    
    if(res == FR_OK)
    {
        bmpBuffer = malloc_ex_mem(f.fsize);
        
        if(bmpBuffer != NULL)
        {
            f_read(&f, bmpBuffer, f.fsize, NULL);
            width = *(uint16_t*)&bmpBuffer[1];
            height = *(uint16_t*)&bmpBuffer[1+2];
            
//            handle = IMAGE_CreateUser((800-width) / 2, (480 - height) / 2, width, height, hWin,
//                WM_CF_SHOW, 0, id_base++, 0);
            handle = IMAGE_CreateUser(0, 0, 600, 360, hWin,
                WM_CF_SHOW, 0, id_base++, 0);
            
            IMAGE_SetBMP(handle, bmpBuffer, f.fsize);
//            IMAGE_SetGIF(handle, bmpBuffer, f.fsize);
//            free_ex_mem(bmpBuffer);
            bmpBuffer = NULL;
        }
        
        f_close(&f);
    }
    
    return handle;
}

IMAGE_Handle create_www_qr_code_image(WM_HWIN hWin)
{
    IMAGE_Handle handle;
    uint8_t *bmpBuffer;
    FRESULT res;
    FIL f;
    uint32_t width = 0;
    uint32_t height = 0;
    
    res = f_open (&f, "\\ROOT\\IMAGE\\QRCODE\\cswww.bmp", FA_READ);
    
    if(res == FR_OK)
    {
        bmpBuffer = malloc_ex_mem(f.fsize);
        
        if(bmpBuffer != NULL)
        {
            f_read(&f, bmpBuffer, f.fsize, NULL);
            width = *(uint32_t*)&bmpBuffer[18];
            height = *(uint32_t*)&bmpBuffer[18+4];
            
            handle = IMAGE_CreateUser(20, 300, width, height, hWin,
                WM_CF_SHOW, 0, id_base++, 0);
//            handle = IMAGE_CreateUser(0, 0, 600, 360, hWin,
//                WM_CF_SHOW, 0, id_base++, 0);
            
            IMAGE_SetBMP(handle, bmpBuffer, f.fsize);
//            IMAGE_SetGIF(handle, bmpBuffer, f.fsize);
//            free_ex_mem(bmpBuffer);
            bmpBuffer = NULL;
        }
        
        f_close(&f);
    }
    
    return handle;
}
uint8_t count_flag;
uint32_t count_run_time;
void count_fun_run_time(void)
{
    if(count_flag)
    {
        count_run_time++;
    }
}
void start_count_run_time(void)
{
    count_flag = 1;
    count_run_time = 0;
}
void stop_count_run_time(void)
{
    count_flag = 0;
}

IMAGE_Handle create_wts_qr_code_image(WM_HWIN hWin)
{
    IMAGE_Handle handle;
    uint8_t *bmpBuffer;
    FRESULT res;
    FIL f;
    uint32_t width = 0;
    uint32_t height = 0;
    
    res = f_open (&f, "\\ROOT\\IMAGE\\cswts.bmp", FA_READ);
    
    if(res == FR_OK)
    {
        bmpBuffer = malloc_ex_mem(f.fsize);
        
        if(bmpBuffer != NULL)
        {
            f_read(&f, bmpBuffer, f.fsize, NULL);
            width = *(uint32_t*)&bmpBuffer[18];
            height = *(uint32_t*)&bmpBuffer[18+4];
            
            handle = IMAGE_CreateUser(220, 300, width, height, hWin,
                WM_CF_SHOW, 0, id_base++, 0);
//            handle = IMAGE_CreateUser(0, 0, 600, 360, hWin,
//                WM_CF_SHOW, 0, id_base++, 0);
            
            IMAGE_SetBMP(handle, bmpBuffer, f.fsize);
//            IMAGE_SetGIF(handle, bmpBuffer, f.fsize);
//            free_ex_mem(bmpBuffer);
            bmpBuffer = NULL;
        }
        
        f_close(&f);
    }
    
    return handle;
}

IMAGE_Handle create_test_image(WM_HWIN hWin)
{
    IMAGE_Handle handle;
    uint8_t *bmpBuffer;
    FRESULT res;
    FIL f;
    uint32_t width = 0;
    uint32_t height = 0;
    
    register_tim3_server_fun(count_fun_run_time);
    start_count_run_time();
    res = f_open (&f, "\\ROOT\\IMAGE\\test.bmp", FA_READ);
    
    if(res == FR_OK)
    {
        bmpBuffer = malloc_ex_mem(f.fsize);
        
        if(bmpBuffer != NULL)
        {
            f_read(&f, bmpBuffer, f.fsize, NULL);
    stop_count_run_time();
            width = *(uint32_t*)&bmpBuffer[18];
            height = *(uint32_t*)&bmpBuffer[18+4];
            
            handle = IMAGE_CreateUser(0, 30, width, height, hWin,
                WM_CF_SHOW, 0, id_base++, 0);
            
            IMAGE_SetBMP(handle, bmpBuffer, f.fsize);
//            IMAGE_SetGIF(handle, bmpBuffer, f.fsize);
            free_ex_mem(bmpBuffer);
            bmpBuffer = NULL;
        }
        
        f_close(&f);
    }
    unregister_tim3_server_fun(count_fun_run_time);
    
    return handle;
}


