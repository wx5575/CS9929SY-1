
#include "ui_com/com_ui_info.h"
#include "IMAGE.H"
#include "ff.h"
#include "mem_alloc.h"

void create_logo_image(WM_HWIN hWin)
{
    IMAGE_Handle handle;
    uint8_t *bmpBuffer;
    FRESULT res;
    FIL f;
    
    res = f_open (&f, "/2.bmp", FA_READ);
    
    if(res == FR_OK)
    {
        bmpBuffer = malloc_ex_mem(f.fsize);
        
        if(bmpBuffer != NULL)
        {
            handle = IMAGE_CreateUser(0, 0, 628, 480, hWin,
                WM_CF_LATE_CLIP | WM_CF_SHOW, 0, id_base++, 0);
            
            f_read(&f, bmpBuffer, f.fsize, NULL);
            IMAGE_SetBMP(handle, bmpBuffer, f.fsize);
            free_ex_mem(bmpBuffer);
            bmpBuffer = NULL;
        }
        
        f_close(&f);
    }
}