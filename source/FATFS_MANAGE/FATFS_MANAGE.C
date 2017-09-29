
#include "string.h"
#include "stdio.h"
#include "sys_level.h"
#include "ff.h"
#include "FATFS_MANAGE.H"

/**
  * @brief  移除一个文件夹，包括其本身和其子文件夹，子文件
  * @brief  用来移除一个文件夹及其子文件夹、子文件，但不能移除已经打开的对象。   
  * @param  [in] path 指向要移除文件夹路径字符串
  * @retval @arg FR_OK(0)：           函数成功   
            @arg FR_NO_FILE：         无法找到文件或目录   
            @arg FR_NO_PATH：         无法找到路径   
            @arg FR_INVALID_NAME：    文件名非法   
            @arg FR_INVALID_DRIVE：   驱动器号非法   
            @arg FR_DENIED：          函数由于以下原因被拒绝：   
                    对象属性为只读；  
                    目录下非空；  
                    当前目录。  
            @arg FR_NOT_READY：       磁盘驱动器无法工作，由于驱动器中没有媒体或
            @arg FR_WRITE_PROTECTED： 媒体写保护   
            @arg FR_DISK_ERR：        函数失败由于磁盘运行的一个错误   
            @arg FR_INT_ERR：         函数失败由于一个错误的 FAT 结构或内部错误
            @arg FR_NOT_ENABLED：     逻辑驱动器没有工作区   
            @arg FR_NO_FILESYSTEM：   驱动器上没有合法的 FAT 卷   
            @arg FR_LOCKED：          函数被拒由于文件共享机制（_FS_SHARE）   
*/
FRESULT my_deldir(uint8_t *path)
{  
    FRESULT res;  
    DIR   dir;     /* 文件夹对象 */ //36  bytes  
    FILINFO fno;   /* 文件属性 */   //32  bytes  
    uint8_t file[_MAX_LFN + 2] = {0};  
#if _USE_LFN  
    uint8_t lname[_MAX_LFN + 2] = {0};  
#endif  
      
#if _USE_LFN  
    fno.lfsize = _MAX_LFN;  
    fno.lfname = lname;    //必须赋初值  
#endif
    //打开文件夹
    res = f_opendir(&dir, (const char*)path);  
    
    //持续读取文件夹内容
    while((res == FR_OK) && (FR_OK == f_readdir(&dir, &fno)))  
    {  
        //若是"."或".."文件夹，跳过  
        if(0 == strlen(fno.fname))          break;      //若读到的文件名为空  
        if(0 == strcmp(fno.fname, "."))     continue;   //若读到的文件名为当前文件夹  
        if(0 == strcmp(fno.fname, ".."))    continue;   //若读到的文件名为上一级文件夹  
        
        memset(file, 0, sizeof(file));
#if _USE_LFN
        sprintf((char*)file, "%s/%s", path, (*fno.lfname) ? fno.lfname : fno.fname);  
#else
        sprintf((char*)file, "%s/%s", path, fno.fname);  
#endif
        /* 若是文件夹，递归删除 */
        if (fno.fattrib & AM_DIR)  
        {
            res = my_deldir(file);
        }
        /* 若是文件，直接删除 */
        else
        {
            res = f_unlink((const char*)file);  
        }  
    }  
    
    //删除本身
    if(res == FR_OK)
    {
        res = f_unlink((const char*)path);
    }
    
    return res;
}
/**
  * @brief  在SDCARD中创建多级目录，目录分隔符必须为 "\\"
  * @param  [in] path 文件路径
  * @retval 创建结果
  */
FRESULT my_mkdir(uint8_t *path)
{
    uint8_t len = 0;
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;
    uint8_t dir_buf[10][20] = {0};
    uint8_t full_path[100] = {0};
    FRESULT fresult;
    
    len = strlen((const char*)path);
    
    for(i = 0; i < len; i++)
    {
        if(path[i] == '\\')
        {
            j++;
            k = 0;
            continue;
        }
        
        dir_buf[j][k++] = path[i];
    }
    
    for(i = 1; i <= j; i++)
    {
        sprintf((char*)full_path, "%s\\%s",full_path, dir_buf[i]);
        fresult = f_mkdir((const char*)full_path);
        
        if(fresult == FR_EXIST)
        {
            continue;
        }
        
        if(fresult != FR_OK)
        {
            break;
        }
    }
    
    return fresult;
}
