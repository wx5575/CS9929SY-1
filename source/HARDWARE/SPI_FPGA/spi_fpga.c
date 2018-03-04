

#include "spi_fpga.h"

#define			SPI_CS_EN()         GPIO_ResetBits(GPIOE,GPIO_Pin_1)
#define			SPI_CS_DIS()        GPIO_SetBits(GPIOE,GPIO_Pin_1)
#define			SPI_MOSI_HIGH()     GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define			SPI_MOSI_LOW()      GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define			SPI_CLK_HIGH()      GPIO_SetBits(GPIOE,GPIO_Pin_0)
#define			SPI_CLK_LOW()       GPIO_ResetBits(GPIOE,GPIO_Pin_0)
#define			SPI_MISO_DI()       GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)




void init_spi_fpga_gpio(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB |RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB、GPIOE时钟
    
    //PE0,PE1初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//PE0为clk,PE1为CS
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIO
    
    GPIO_SetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1);							//
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PE2为MISO						
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PB9为MOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
    
    GPIO_SetBits(GPIOB,GPIO_Pin_9);//
    SPI_CS_DIS();
}

static void fpga_delay_ms(u32 time)
{
//    u32 i = 0;
//    
//    while(time--)
//    {
//        for(i = 0; i < 16 * 1000; i++)
//        {
//        }
//    }
}

//*************************************************************

/* 函数名：fpga_write
 * 描述  ：往fpga写命令，并返回一字节
 * 输入  ：命令
 * 输出  ：返回值
 */
//**************************************************************
static void fpga_write(u32 SendData,u8 frame_length)
{
    u8  ShiftCounter;
    u32 DataBuffer;

    SPI_CS_DIS();
    fpga_delay_ms(1);
    SPI_CS_EN();																				//CS=0;片选CPLD
    fpga_delay_ms(1);

    for(ShiftCounter=0;ShiftCounter<frame_length;ShiftCounter++)
    {
        DataBuffer 		 = SendData;
        DataBuffer     = DataBuffer & 0x00000001;//取数据最低位
        SendData       = SendData >>1;//数据右移一位 
        SPI_CLK_HIGH();//时钟置高
        fpga_delay_ms(1);
        
        if(DataBuffer == 0x00000000)
        {
            SPI_MOSI_LOW();
            fpga_delay_ms(1);
        }
        else
        {
            SPI_MOSI_HIGH();
            fpga_delay_ms(1);
        }

        SPI_CLK_LOW();//数据写入FPGA
        fpga_delay_ms(1);
        
    }
    
    SPI_CS_DIS();//CS=1;取消片选
    fpga_delay_ms(1);
}

//从CPLD中读数据
void ReadDataFromFPGA(u32 SendData, u8 bytes, u8 *buf)
{
    u8  ShiftCounter;
    u8  DataBuffer=0;
    u32 SendDataBuffer;
    u32 i = 0;
    u8  frame_length =26;//一帧数据26bit
    
    SendDataBuffer = SendData;//要发送的数据
    fpga_write(SendDataBuffer, frame_length);//给FPGA发送读命令
    
    SPI_CS_EN(); //片选有效
    fpga_delay_ms(1);
    DataBuffer = 0;
	
	for(i = 0; i < bytes; i++)
	{
		DataBuffer = 0;
		
		for(ShiftCounter=0;ShiftCounter < 8;ShiftCounter++)
		{
			DataBuffer = DataBuffer << 1;//左移一位
            
			SPI_CLK_HIGH();//时钟置高
			fpga_delay_ms(1);
            
			SPI_CLK_LOW();
			fpga_delay_ms(1);
			
			if(SPI_MISO_DI() == 1)
			{
				DataBuffer = DataBuffer | 0x0001;//低位置1
				fpga_delay_ms(1);
			}
		}
        
		buf[i] = DataBuffer;
	}
    
	SPI_CS_DIS();
	fpga_delay_ms(1);
}




/************************ (C) COPYRIGHT 2017 长盛仪器 *****END OF FILE****/
