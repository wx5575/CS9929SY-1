

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
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB |RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOB��GPIOEʱ��
    
    //PE0,PE1��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//PE0Ϊclk,PE1ΪCS
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIO
    
    GPIO_SetBits(GPIOE,GPIO_Pin_0 | GPIO_Pin_1);							//
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PE2ΪMISO						
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//PB9ΪMOSI
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
    
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

/* ��������fpga_write
 * ����  ����fpgaд���������һ�ֽ�
 * ����  ������
 * ���  ������ֵ
 */
//**************************************************************
static void fpga_write(u32 SendData,u8 frame_length)
{
    u8  ShiftCounter;
    u32 DataBuffer;

    SPI_CS_DIS();
    fpga_delay_ms(1);
    SPI_CS_EN();																				//CS=0;ƬѡCPLD
    fpga_delay_ms(1);

    for(ShiftCounter=0;ShiftCounter<frame_length;ShiftCounter++)
    {
        DataBuffer 		 = SendData;
        DataBuffer     = DataBuffer & 0x00000001;//ȡ�������λ
        SendData       = SendData >>1;//��������һλ 
        SPI_CLK_HIGH();//ʱ���ø�
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

        SPI_CLK_LOW();//����д��FPGA
        fpga_delay_ms(1);
        
    }
    
    SPI_CS_DIS();//CS=1;ȡ��Ƭѡ
    fpga_delay_ms(1);
}

//��CPLD�ж�����
void ReadDataFromFPGA(u32 SendData, u8 bytes, u8 *buf)
{
    u8  ShiftCounter;
    u8  DataBuffer=0;
    u32 SendDataBuffer;
    u32 i = 0;
    u8  frame_length =26;//һ֡����26bit
    
    SendDataBuffer = SendData;//Ҫ���͵�����
    fpga_write(SendDataBuffer, frame_length);//��FPGA���Ͷ�����
    
    SPI_CS_EN(); //Ƭѡ��Ч
    fpga_delay_ms(1);
    DataBuffer = 0;
	
	for(i = 0; i < bytes; i++)
	{
		DataBuffer = 0;
		
		for(ShiftCounter=0;ShiftCounter < 8;ShiftCounter++)
		{
			DataBuffer = DataBuffer << 1;//����һλ
            
			SPI_CLK_HIGH();//ʱ���ø�
			fpga_delay_ms(1);
            
			SPI_CLK_LOW();
			fpga_delay_ms(1);
			
			if(SPI_MISO_DI() == 1)
			{
				DataBuffer = DataBuffer | 0x0001;//��λ��1
				fpga_delay_ms(1);
			}
		}
        
		buf[i] = DataBuffer;
	}
    
	SPI_CS_DIS();
	fpga_delay_ms(1);
}




/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
