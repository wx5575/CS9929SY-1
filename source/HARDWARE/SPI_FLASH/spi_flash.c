
/**
  ******************************************************************************
  * @file    spi_flash.c
  * @author  王鑫
  * @version V1.0.0
  * @date    2017.4.18
  * @brief   SPI串口FLASH驱动
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "spi_flash.h"

#include "stdio.h"
#include "string.h"

/* Private typedef -----------------------------------------------------------*/
#define SPI_FLASH_PageSize    0x200

/* Private define ------------------------------------------------------------*/
#define WRITE      0x82  /* Write to Memory instruction */
#define READ       0xE8  /* Read from Memory instruction */
#define RDSR       0xD7  /* Read Status Register instruction  */
#define RDID       0x9F  /* Read identification */
#define PE         0x81  /* Page Erase instruction */
#define BE1        0xC7  /* Bulk Erase instruction */
#define BE2        0x94  /* Bulk Erase instruction */
#define BE3        0x80  /* Bulk Erase instruction */
#define BE4        0x9A  /* Bulk Erase instruction */

#define BUSY_Flag  0x80 /* Ready/busy status flag */

#define Dummy_Byte 0xA5

/**************************** 移植改动硬件 *********************************/
#define         FLASH_RCC_AHB1Periph_GPIO   RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG
/* WP */
#define         FLASH_WP_GPIO       GPIOA
#define         FLASH_WP_PIN        GPIO_Pin_12
/* MOSI */
#define         FLASH_MOSI_GPIO     GPIOG
#define         FLASH_MOSI_PIN      GPIO_Pin_7
/* MISO */ 
#define         FLASH_MISO_GPIO     GPIOA
#define         FLASH_MISO_PIN      GPIO_Pin_11
/* CLK */
#define         FLASH_CLK_GPIO      GPIOG
#define         FLASH_CLK_PIN       GPIO_Pin_8

/* CS1 D3 */
#define         FLASH_CS1_GPIO      GPIOA
#define         FLASH_CS1_PIN       GPIO_Pin_8
/* CS2 D4 */
#define         FLASH_CS2_GPIO      GPIOA
#define         FLASH_CS2_PIN       GPIO_Pin_9
/* CS3 D5 */
#define         FLASH_CS3_GPIO      GPIOA
#define         FLASH_CS3_PIN       GPIO_Pin_10
/* CS4 D6 */
#define         FLASH_CS4_GPIO      GPIOA
#define         FLASH_CS4_PIN       GPIO_Pin_15

/*************************************************************************/


#define FLASH_WP_EN()       GPIO_ResetBits(FLASH_WP_GPIO, FLASH_WP_PIN)
#define FLASH_WP_DIS()      GPIO_SetBits(FLASH_WP_GPIO, FLASH_WP_PIN)

#define FLASH_CAL_WP_EN()   GPIO_ResetBits(FLASH_CAL_WP_GPIO, FLASH_CAL_WP_PIN)
#define FLASH_CAL_WP_DIS()  GPIO_SetBits(FLASH_CAL_WP_GPIO, FLASH_CAL_WP_PIN)

#define FLASH_CS1_EN()      GPIO_ResetBits(FLASH_CS1_GPIO,   FLASH_CS1_PIN)
#define FLASH_CS2_EN()      GPIO_ResetBits(FLASH_CS2_GPIO,   FLASH_CS2_PIN)
#define FLASH_CS3_EN()      GPIO_ResetBits(FLASH_CS3_GPIO,   FLASH_CS3_PIN)
#define FLASH_CS4_EN()      GPIO_ResetBits(FLASH_CS4_GPIO,   FLASH_CS4_PIN)

#define FLASH_CS1_DIS()     GPIO_SetBits(FLASH_CS1_GPIO, FLASH_CS1_PIN)
#define FLASH_CS2_DIS()     GPIO_SetBits(FLASH_CS2_GPIO, FLASH_CS2_PIN)
#define FLASH_CS3_DIS()     GPIO_SetBits(FLASH_CS3_GPIO, FLASH_CS3_PIN)
#define FLASH_CS4_DIS()     GPIO_SetBits(FLASH_CS4_GPIO, FLASH_CS4_PIN)

#define FLASH_DI_HIGH()     GPIO_SetBits(FLASH_MOSI_GPIO, FLASH_MOSI_PIN)
#define FLASH_DI_LOW()		GPIO_ResetBits(FLASH_MOSI_GPIO, FLASH_MOSI_PIN)

#define FLASH_CLK_HIGH()	GPIO_SetBits(FLASH_CLK_GPIO, FLASH_CLK_PIN)
#define FLASH_CLK_LOW()     GPIO_ResetBits(FLASH_CLK_GPIO, FLASH_CLK_PIN)

#define FLASH_DI()			GPIO_ReadInputDataBit(FLASH_MISO_GPIO, FLASH_MISO_PIN)

uint8_t chipselect;
#define CHIP_SIZE   4
static SFLASH_INFO  at45db161_info[CHIP_SIZE];


/**
  * @brief  FLASH片选设置
  * @param  [in] n n 1、2、3
  * @retval 无
  */
void flash_cs_set_at45db161(char n)
{
	switch(n)
	{
		case 1:
			chipselect = 1;
			break;
		case 2:
			chipselect = 2;
			break;
		case 3:
			chipselect = 3;
			break;
		case 4:
			chipselect = 4;
			break;
	}
}

/**
  * @brief  FLASH片选使能
  * @param  无
  * @retval 无
  */
static void flash_cs_en_at45db161(void)
{
    FLASH_CS1_DIS();
    FLASH_CS2_DIS();
    FLASH_CS3_DIS();
    FLASH_CS4_DIS();
    
	switch(chipselect)
	{
		case 1:
            FLASH_CS1_EN();
			break;
		case 2:
            FLASH_CS2_EN();
			break;
		case 3:
            FLASH_CS3_EN();
			break;
		case 4:
            FLASH_CS4_EN();
			break;
	}
}

/**
  * @brief  FLASH片选去使能
  * @param  无
  * @retval 无
  */
static void flash_cs_dis_at45db161(void)
{
	FLASH_CS1_DIS();
	FLASH_CS2_DIS();
	FLASH_CS3_DIS();
	FLASH_CS4_DIS();
}

/**
  * @brief  FLASH写保护上锁
  * @param  无
  * @retval 无
  */
static void at45_flash_lock(void)
{
    FLASH_WP_EN();
}

/**
  * @brief  FLASH写保护解锁
  * @param  无
  * @retval 无
  */
static void at45_flash_unlock(void)
{
    FLASH_WP_DIS();
}

/**
  * @brief  FLASH初始化管脚
  * @param  无
  * @retval 无
  */
static void at45_gpio_init(void)
{
	/*************************    定义初始化结构变量    *************************/
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/*************************    开启外设时钟    *************************/
	RCC_AHB1PeriphClockCmd(FLASH_RCC_AHB1Periph_GPIO,ENABLE); //开启GPIOB时钟
    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;  //输入
    
    /* MISO */
	GPIO_InitStructure.GPIO_Pin = FLASH_MISO_PIN; //PB8 9引脚设置
	GPIO_Init(FLASH_MISO_GPIO, &GPIO_InitStructure); //GPIOB8 9设置
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //输入
    
	/* MOSI */
	GPIO_InitStructure.GPIO_Pin = FLASH_MOSI_PIN;
	GPIO_Init(FLASH_MOSI_GPIO, &GPIO_InitStructure);
    
    /* WP */
	GPIO_InitStructure.GPIO_Pin = FLASH_WP_PIN;
	GPIO_Init(FLASH_WP_GPIO, &GPIO_InitStructure);
    
    /* CLK */
	GPIO_InitStructure.GPIO_Pin = FLASH_CLK_PIN;
	GPIO_Init(FLASH_CLK_GPIO, &GPIO_InitStructure);
    
    /* CS1 */
	GPIO_InitStructure.GPIO_Pin = FLASH_CS1_PIN;
	GPIO_Init(FLASH_CS1_GPIO, &GPIO_InitStructure);
    
    /* CS2 */
	GPIO_InitStructure.GPIO_Pin = FLASH_CS2_PIN;
	GPIO_Init(FLASH_CS2_GPIO, &GPIO_InitStructure);
    
    /* CS3 */
	GPIO_InitStructure.GPIO_Pin = FLASH_CS3_PIN;
	GPIO_Init(FLASH_CS3_GPIO, &GPIO_InitStructure);
    
    /* CS4 */
	GPIO_InitStructure.GPIO_Pin = FLASH_CS4_PIN;
	GPIO_Init(FLASH_CS4_GPIO, &GPIO_InitStructure);
    
	flash_cs_dis_at45db161();
}

/**
  * @brief  发送  FLASH 的数据
  * @param  [in] data 8bit数据
  * @retval 无
  */
static void at45_send_byte(unsigned char data)
{
    uint8_t   i;
    
    for (i = 8; i != 0; i-- )
    {
        FLASH_CLK_LOW();
        
        if (data & 0x80)
        {
            FLASH_DI_HIGH();
        }
        else
        {
            FLASH_DI_LOW();
        }
        
        FLASH_CLK_HIGH();
        data <<= 1;
    }
}

/**
  * @brief  读 FLASH 的数据
  * @param  无
  * @retval 读出的数据8bit
  */
static uint8_t at45_read_byte(void)
{
    uint8_t   i, data = 0;
    
    for (i = 8; i != 0; i--)
    {
        FLASH_CLK_LOW();
        data <<= 1;
        FLASH_CLK_HIGH();
        
        data |= FLASH_DI();
    }
    
    return data;
}

/**
  * @brief  等待flash写完成
  * @param  无
  * @retval 无
  */
static void SPI_FLASH_WaitForWriteEnd(void)
{
    u8 FLASH_Status = 0;
    
    flash_cs_en_at45db161();
    at45_send_byte(RDSR);
    
    do
    {
        FLASH_Status = at45_read_byte();
    }while ((FLASH_Status & BUSY_Flag) == RESET);
    
    flash_cs_dis_at45db161();
}

/**
  * @brief  写扇区保护寄存器
  * @param  无
  * @retval 无
  */
static void SPI_FLASH_PROTE_ENABLE(void)
{
    uint8_t bytes[4] = {0X3D, 0X2A, 0X7F, 0XA9};//写扇区保护寄存器
    int32_t i = 0;
    
    SPI_FLASH_WaitForWriteEnd();
    
	flash_cs_en_at45db161();
    
    for(i = 0; i < 4; i++)
    {
        at45_send_byte(bytes[i]);
    }
    
	flash_cs_dis_at45db161();
}

/**
  * @brief  写扇区去保护寄存器
  * @param  无
  * @retval 无
  */
static void SPI_FLASH_PROTE_DISABLE(void)
{
    uint8_t bytes[4] = {0X3D, 0X2A, 0X7F, 0X9A};//写扇区保护寄存器
    int32_t i = 0;
    
    SPI_FLASH_WaitForWriteEnd();
    
	flash_cs_en_at45db161();
    
    for(i = 0; i < 4; i++)
    {
        at45_send_byte(bytes[i]);
    }
    
	flash_cs_dis_at45db161();
}

/**
  * @brief  FLASH页擦除
  * @param  [in] PageAddr 页地址
  * @retval 无
  */
void SPI_FLASH_PageErase(uint32_t PageAddr)
{
    at45_flash_unlock();
    SPI_FLASH_WaitForWriteEnd();
	flash_cs_en_at45db161();
	
	at45_send_byte(0x81);
	
	at45_send_byte((PageAddr & 0xFF0000) >> 16);
	at45_send_byte((PageAddr & 0xFF00) >> 8);
	at45_send_byte(PageAddr & 0xFF);
	
	flash_cs_dis_at45db161();
	SPI_FLASH_WaitForWriteEnd();
    
    at45_flash_lock();
}

/**
  * @brief  擦除FLASH扇区保护寄存器
  * @param  void
  * @retval 无
  */
static void SPI_FLASH_Erase_Sector_Protection_Reg(void)
{
    uint8_t bytes[4] = {0X3D, 0X2A, 0X7F, 0XCF};//写扇区保护寄存器
    int32_t i = 0;
    
    at45_flash_unlock();
    SPI_FLASH_WaitForWriteEnd();
    
	flash_cs_en_at45db161();
    
    for(i = 0; i < 4; i++)
    {
        at45_send_byte(bytes[i]);
    }
    
	flash_cs_dis_at45db161();
	SPI_FLASH_WaitForWriteEnd();
    
    at45_flash_lock();
}
/**
  * @brief  写FLASH扇区保护寄存器
  * @param  void
  * @retval 无
  */
static void SPI_FLASH_Write_Sector_Protection_Reg(uint8_t cmd)
{
    uint8_t bytes[4] = {0X3D, 0X2A, 0X7F, 0XFC};//写扇区保护寄存器
    int32_t i = 0;
    uint8_t prote_st = cmd;
    
    at45_flash_unlock();
    SPI_FLASH_WaitForWriteEnd();
    
	flash_cs_en_at45db161();
    
    for(i = 0; i < 4; i++)
    {
        at45_send_byte(bytes[i]);
    }
    
    for(i = 0; i < 16; i++)
    {
        at45_send_byte(prote_st);
    }
    
	flash_cs_dis_at45db161();
	SPI_FLASH_WaitForWriteEnd();
    at45_flash_lock();
}
/**
  * @brief  读FLASH扇区保护寄存器
  * @param  void
  * @retval 无
  */
static void SPI_FLASH_Read_Sector_Protection_Reg(uint8_t *buf)
{
    uint8_t bytes[4] = {0X32, 0X2A, 0X7F, 0XFC};//写扇区保护寄存器
    int32_t i = 0;
    
    SPI_FLASH_WaitForWriteEnd();
    
	flash_cs_en_at45db161();
    
    for(i = 0; i < 4; i++)
    {
        at45_send_byte(bytes[i]);
    }
    
    for(i = 0; i < 16; i++)
    {
        buf[i] = at45_read_byte();
    }
    
	flash_cs_dis_at45db161();
	SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  擦除整片flash
  * @param  无
  * @retval 无
  */
void erase_chip_spi_flash(void)
{
    uint8_t bytes[4] = {0XC7, 0X94, 0X80, 0X9A};//写扇区保护寄存器
    int32_t i = 0;
    
    at45_flash_unlock();
    SPI_FLASH_WaitForWriteEnd();
    
	flash_cs_en_at45db161();
    
    for(i = 0; i < 4; i++)
    {
        at45_send_byte(bytes[i]);
    }
    
	flash_cs_dis_at45db161();
	
    SPI_FLASH_WaitForWriteEnd();
    at45_flash_lock();
}

/**
  * @brief  FLASH 写扇区
  * @param  [in] pBuffer 数据缓冲区
  * @param  [in] SectorAddr 扇区地址
  * @param  [in] NumByteToWrite 要写入的字节数
  * @retval 无
  */
static void SPI_FLASH_WriteSector(u8* pBuffer, u32 SectorAddr, u16 NumByteToWrite)
{
	uint32_t  uiPageAddress;
	uint8_t *bytes;
	uint32_t page_addr = 0;
	uint32_t inter_addr = 0;
	uint32_t page_size = SPI_FLASH_PAGE_SIZE;
	uint32_t addr = 0;
    
	uiPageAddress = (uint32_t)SectorAddr;
	
	inter_addr = uiPageAddress % page_size;
	page_addr = uiPageAddress / page_size;
	addr =  inter_addr | (page_addr << PAGE_ADDR_OFFSET_BITS);
	bytes = (uint8_t*)&addr;
    
    at45_flash_unlock();
    SPI_FLASH_WaitForWriteEnd();
    
	flash_cs_en_at45db161();
    
	at45_send_byte(WRITE);
	at45_send_byte(bytes[2]);
	at45_send_byte(bytes[1]);
	at45_send_byte(bytes[0]);
    
	while (NumByteToWrite--)
	{
		at45_send_byte(*pBuffer++);
	}
    
	flash_cs_dis_at45db161();
	SPI_FLASH_WaitForWriteEnd();
    at45_flash_lock();
}

/**
  * @brief  FLASH 读扇区
  * @param  [in] pBuffer 数据缓冲区
  * @param  [in] SectorAddr 扇区地址
  * @param  [in] NumByteToWrite 要读的字节数
  * @retval 无
  */
static void SPI_FLASH_ReadSector(u8* pBuffer, u32 SectorAddr, u16 NumByteToRead)
{
	uint32_t  uiPageAddress;
	uint8_t *bytes;
	uint32_t page_addr = 0;
	uint32_t inter_addr = 0;
	uint32_t page_size = SPI_FLASH_PAGE_SIZE;
	uint32_t addr = 0;
    
	uiPageAddress = (uint32_t)SectorAddr;
	
	inter_addr = uiPageAddress % page_size;
	page_addr = uiPageAddress / page_size;
	addr =  inter_addr | (page_addr << PAGE_ADDR_OFFSET_BITS);
	bytes = (uint8_t*)&addr;
    
    SPI_FLASH_WaitForWriteEnd();
	flash_cs_en_at45db161();
    
	at45_send_byte(READ);
	
	at45_send_byte(bytes[2]);
	at45_send_byte(bytes[1]);
	at45_send_byte(bytes[0]);
    
	at45_send_byte(Dummy_Byte);
	at45_send_byte(Dummy_Byte);
	at45_send_byte(Dummy_Byte);
	at45_send_byte(Dummy_Byte);
    
	while (NumByteToRead--)
	{
        *pBuffer++ = at45_read_byte();
	}
    
	flash_cs_dis_at45db161();
}


/**
  * @brief  FLASH 读ID
  * @param  无
  * @retval FLASH ID(32bit)
  */
static u32 SPI_FLASH_ReadID(void)
{
  u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0, Temp3;

  /* Select the FLASH: Chip Select low */
  flash_cs_en_at45db161();

  /* Send "RDID " instruction */
  at45_send_byte(0x9F);

  /* Read a byte from the FLASH */
  Temp0 = at45_read_byte();

  /* Read a byte from the FLASH */
  Temp1 = at45_read_byte();

  /* Read a byte from the FLASH */
  Temp2 = at45_read_byte();

  /* Read a byte from the FLASH */
  Temp3 = at45_read_byte();
  
  /* Deselect the FLASH: Chip Select high */
  flash_cs_dis_at45db161();

  Temp = (Temp0 << 24) | (Temp1 << 16) | (Temp2<<8) | Temp3;

  return Temp;
}

/**
  * @brief  写入一页并对写入的数据进行检查
  * @param  [in] buf 数据缓存
  * @param  [in] addr 地址页对齐的
  * @retval 无
  */
static void write_page_and_check_ok(uint8_t *buf, uint32_t addr)
{
    static uint8_t buf_cmp[SPI_FLASH_PAGE_SIZE];
    uint32_t page_size = SPI_FLASH_PAGE_SIZE;
    uint32_t retry_count = 0;
    
    do
    {
        SPI_FLASH_WriteSector(buf, addr, page_size);
        SPI_FLASH_ReadSector(buf_cmp, addr, page_size);
        if(++retry_count > 5)
        {
            break;
        }
    }while(0 != memcmp(buf, buf_cmp, page_size));
}

/**
  * @brief  缓冲写，地址是2Mb内任意地址，
  * @param  [in] pBuffer 数据缓存
  * @param  [in] WriteAddr 要写入的地址
  * @param  [in] NumByteToWrite 写入的字节数只要空间足够的任意字节数
  * @retval 无
  */
void at45_buffer_write(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite)
{
	uint32_t num_of_p_first = 0;//第一页个数
	uint32_t num_of_p_n = 0;//整页数
	uint32_t num_of_p_last = 0;//最后一页个数
    uint32_t addr_of_p_first = 0;//第一页的首地址
	uint32_t addr = 0;
    uint32_t page_size = SPI_FLASH_PAGE_SIZE;
    static uint8_t buf[SPI_FLASH_PAGE_SIZE];
	uint32_t remain_f_p = 0;/* 第一页可用的数据个数 */
    
    int32_t i = 0;
    uint32_t start;
    uint32_t first_addr_of_page;//页的首地址
	uint32_t num_byte = NumByteToWrite;
    
    if(page_size == 0)
    {
        return;
    }
    
    addr_of_p_first = WriteAddr / page_size * page_size;
	addr = WriteAddr % page_size;//写入地址是每页的第几位
    
	remain_f_p = (addr==0? 0:(page_size - addr));//在开始的一页还可以写入的个数
    
	/* 确定第一页的数据字节数 */
	if(num_byte > remain_f_p)
	{
		num_of_p_first = remain_f_p;
	}
	else
	{
		num_of_p_first = num_byte;
	}
	
    if(num_byte > remain_f_p)
    {
        num_of_p_n = (num_byte - remain_f_p) / page_size;//要读的整页数
        num_of_p_last = (num_byte - remain_f_p) % page_size;//最后一页的个数
    }
    
    first_addr_of_page = addr_of_p_first;
    
    /* 第一页 */
    if(num_of_p_first > 0)
    {
        SPI_FLASH_ReadSector(buf, first_addr_of_page, page_size);
        
        start = addr;
        
        for(i = 0; i < num_of_p_first; i++)
        {
            buf[start + i] = *pBuffer++;
        }
        
        write_page_and_check_ok(buf, first_addr_of_page);
        
        first_addr_of_page += page_size;
    }
    /* 中间n页 */
    if(num_of_p_n > 0)
    {
        for(i = 0; i < num_of_p_n; i++)
        {
            write_page_and_check_ok(pBuffer, first_addr_of_page);
            pBuffer += page_size;
            first_addr_of_page += page_size;
        }
    }
    /* 最后一页 */
    if(num_of_p_last > 0)
    {
        SPI_FLASH_ReadSector(buf, first_addr_of_page, page_size);
        
        for(i = 0; i < num_of_p_last; i++)
        {
            buf[i] = *pBuffer++;
        }
        
        write_page_and_check_ok(buf, first_addr_of_page);
    }
}

/**
  * @brief  缓冲读，地址是2Mb内任意地址，
  * @param  [in] pBuffer 数据缓存
  * @param  [in] WriteAddr 要读的地址
  * @param  [in] NumByteToRead 读的字节数只要空间足够的任意字节数
  * @retval 无
  */
void at45_buffer_read(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	uint32_t num_of_p_first = 0;//第一页个数
	uint32_t num_of_p_n = 0;//整页数
	uint32_t num_of_p_last = 0;//最后一页个数
    uint32_t addr_of_p_first = 0;//第一页的首地址
	uint32_t addr = 0;
    uint32_t page_size = SPI_FLASH_PAGE_SIZE;
    static uint8_t buf[SPI_FLASH_PAGE_SIZE];
	uint32_t remain_f_p = 0;/* 第一页可用的数据个数 */
	uint32_t num_bytes = NumByteToRead;
    
    int32_t i = 0;
    uint32_t start;
    uint32_t first_addr_of_page;//页地址
    
    if(page_size == 0)
    {
        return;
    }
    
    addr_of_p_first = ReadAddr / page_size * page_size;
	addr = ReadAddr % page_size;//写入地址是每页的第几位
    
	remain_f_p = (addr==0? 0:(page_size - addr));//在开始的一页还可以写入的个数
    
	/* 确定第一页的数据字节数 */
	if(num_bytes > remain_f_p)
	{
		num_of_p_first = remain_f_p;
	}
	else
	{
		num_of_p_first = num_bytes;
	}
	
    if(num_bytes > remain_f_p)
    {
        num_of_p_n = (num_bytes - remain_f_p) / page_size;//要读的整页数
        num_of_p_last = (num_bytes - remain_f_p) % page_size;//最后一页的个数
    }
    
    first_addr_of_page = addr_of_p_first;
    
    /* 第一页 */
    if(num_of_p_first > 0)
    {
        SPI_FLASH_ReadSector(buf, first_addr_of_page, page_size);
        
        start = addr;
        
        for(i = 0; i < num_of_p_first; i++)
        {
            *pBuffer++ = buf[start + i];
        }
        
        first_addr_of_page += page_size;
    }
    /* 中间n页 */
    if(num_of_p_n > 0)
    {
        for(i = 0; i < num_of_p_n; i++)
        {
            SPI_FLASH_ReadSector(pBuffer, first_addr_of_page, page_size);
            pBuffer += page_size;
            first_addr_of_page += page_size;
        }
    }
    /* 最后一页 */
    if(num_of_p_last > 0)
    {
        SPI_FLASH_ReadSector(buf, first_addr_of_page, page_size);
        
        for(i = 0; i < num_of_p_last; i++)
        {
            *pBuffer++ = buf[i];
        }
    }
}
void xxxx(void)
{
    SPI_FLASH_PROTE_ENABLE();
    SPI_FLASH_PROTE_DISABLE();
    SPI_FLASH_Erase_Sector_Protection_Reg();
    SPI_FLASH_Write_Sector_Protection_Reg(0);
    SPI_FLASH_Read_Sector_Protection_Reg(0);
}


/**
  * @brief  测试FLASH
  * @param  无
  * @retval 无
  */
void test_flash(void)
{
    int i = 0;
    uint8_t temp = 0;
    uint8_t buf[100];
    
    FLASH_WP_DIS();
    
    if(temp == 0)
    {
        SPI_FLASH_Erase_Sector_Protection_Reg();
        SPI_FLASH_Read_Sector_Protection_Reg(buf);
        SPI_FLASH_Write_Sector_Protection_Reg(0xff);
    }
    else
    {
        FLASH_WP_EN();
    }
    
    SPI_FLASH_Read_Sector_Protection_Reg(buf);
    FLASH_WP_DIS();
    
    for(i = 0; i < 1024; i++)
    {
        buf[i] = i+temp;
    }
    
    at45_buffer_write(buf, 0, 1024);
    at45_buffer_read(buf, 1, 1024);
}

/**
  * @brief  初始化FLASH
  * @param  无
  * @retval 无
  */
void init_flash(void)
{
    uint8_t buf[20];
    int32_t i = 0;
    
	at45_gpio_init();
    
    for(i = 0; i < CHIP_SIZE; i++)
    {
        flash_cs_set_at45db161(1 + i);
        
        at45db161_info[i].ChipID = SPI_FLASH_ReadID();
        
        if(at45db161_info[i].ChipID == 0x1F260000)
        {
            strcpy(at45db161_info[i].ChipName, "AT45DB161");
            at45db161_info[i].TotalSize = 2 * 1024 * 1024; /* 总容量 = 2M */
            at45db161_info[i].PageSize = SPI_FLASH_PAGE_SIZE; /* 页面大小 = 512B */
            
            SPI_FLASH_Read_Sector_Protection_Reg(buf);
            
            if(buf[0] != 0xff)
            {
                SPI_FLASH_Erase_Sector_Protection_Reg();
    //             SPI_FLASH_Write_Sector_Protection_Reg(0);
            }
        }
    }
}

/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
