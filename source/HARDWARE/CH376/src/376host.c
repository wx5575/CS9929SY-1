/* 用CH376操作HID设备，比如键盘，鼠标,兼容HID复合设备*/


#include <stdio.h>
#include <string.h>
#include "CH376INC.H"
#include "hid.h"

// 获取设备描述符
unsigned char SetupGetDevDescr[] = { 0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x12, 0x00 };
// 获取配置描述符
unsigned char SetupGetCfgDescr[] = { 0x80, 0x06, 0x00, 0x02, 0x00, 0x00, 0x09, 0x00 };
// 设置USB地址
unsigned char SetupSetUsbAddr[] = { 0x00, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 };
// 设置USB配置
//const	unsigned char code SetupSetUsbConfig[] = { 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// SET IDLE 
unsigned char SetupSetidle[]={0x21,0x0a,0x00,0x00,0x00,0x00,0x00,0x00};        
// 获取HID 报告描述符 
unsigned char SetupGetHidDes[]={0x81,0x06,0x00,0x22,0x00,0x00,0x81,0x00};    
// SET REPORT 
unsigned char SetupSetReport[]={0x21,0x09,0x00,0x02,0x00,0x00,0x01,0x00};     


////单片机总线方式操作CH376
//UINT8V	xdata	CH376_CMD_PORT	_at_ 0xBDF1; /* CH376命令端口的I/O地址 */
//UINT8V	xdata	CH376_DAT_PORT	_at_ 0xBCF0; /* CH376数据端口的I/O地址 */


//#define CH376_INT_WIRE   INT0       //定义CH376中断引脚，可省略；

struct _Device_Atti{
  	UINT8 Device_connect;      //设备连接状态 1：连接，0：断开
  	UINT8 Device_compat;       //0:非复合设备,1表示复合设备  
  	UINT8 Cfg_value;           //设备配置描述符中配置值
  struct _Device{ 
  	UINT8 Device_type;	       //设备类型 1:键盘，2:鼠标
  	UINT8 Device_inf;	       //设备接口号 默认为0  最多支持两个接口设备
  	UINT8 Device_endp;         //设备端点地址 最多支持一个端点
  	UINT8 Device_size;         //设备端点大小 
  	UINT16 Device_report_len;   //设备报表长度
	UINT8 tog;				   //端点的同步标志
  }Device[2];
}Device_Atti = { 0 };

UINT8 receive_mode = 0x00,send_mode = 0x00;
UINT8 idata, data_buf[120];
UINT8 status;
UINT8   flag_config_2; //第二次获取配置描述符标志

//**********************************************
//*	NAME:    mDelay2uS( )									 
//*	FUCTION: 延时2us子函数									 
//*	输入参数：无
//* 输出参数：无									 
//*	说明：    根据单片机的主频进行调整									 
//**********************************************

static void mDelay0_5uS( void )  /* 至少延时0.5uS,根据单片机主频调整 */
{
	UINT8 i;
    
	for (i = 0; i < 10; i++);
}

void mDelay2uS( )
{
    mDelay0_5uS();
}

//**********************************************
//*	NAME:    mDelay50mS( )									 
//*	FUCTION: 延时50ms子函数									 
//*	输入参数：无
//* 输出参数：无									 
//*	说明：    根据单片机的主频进行调整									 
//**********************************************

void mDelay50mS( )
{
    mDelay0_5uS();
}

//**********************************************
//*	NAME:    CH376_WR_CMD_PORT( UINT8 cmd )									 
//*	FUCTION: 写CH376命令子函数									 
//*	输入参数：8位命令码
//* 输出参数：无									 
//*	说明：对于速度较快的单片机，则需要1.5uS延时									 
//**********************************************

void CH376_WR_CMD_PORT( UINT8 cmd )
{
	xWriteCH376Cmd(cmd);
}

//**********************************************
//*	NAME:    CH376_WR_DAT_PORT( UINT8 dat )									 
//*	FUCTION: 写CH376数据子函数									 
//*	输入参数：8位数据
//* 输出参数：无									 
//*	说明：对于速度较快的单片机，则需要0.6uS延时									 
//**********************************************

void CH376_WR_DAT_PORT( UINT8 dat )
{
	xWriteCH376Data(dat);
}

//**********************************************
//*	NAME:    CH376_RD_DAT_PORT( void )									 
//*	FUCTION: 读CH376数据子函数									 
//*	输入参数：无
//* 输出参数：8位数据									 
//*	说明：对于速度较快的单片机，则需要0.6uS延时									 
//**********************************************

UINT8 CH376_RD_DAT_PORT( void )
{
  	return xReadCH376Data();
}


//**********************************************
//*	NAME:    StdioInit( void )									 
//*	FUCTION: 单片机串口初始化，用于检测程序									 
//*	输入参数：无
//* 输出参数：无									 
//*	说明： 单片机主频24M 使用定时器1 波特率9600									 
//**********************************************

void StdioInit( void )
{

}

//**********************************************
//*	NAME:    Set_USB_Mode( UINT8 mode )									 
//*	FUCTION: 设置CH376的工作模式 0x06 为主机模式									 
//*	输入参数：模式代码
//* 输出参数：操作状态 TRUE：成功，FALSE失败									 
//*	说明： 设置CH376的工作模式									 
//**********************************************

UINT8 Set_USB_Mode( UINT8 mode )
{  
	UINT8 i;
    
	CH376_WR_CMD_PORT( CMD_SET_USB_MODE );
	CH376_WR_DAT_PORT( mode );
    
	receive_mode = send_mode = 0x00;  //主机端复位USB数据同步标志 
    
    //等待设置模式操作完成,不超过30uS 
	for( i=0; i!=100; i++ ) 
	{    
		if ( CH376_RD_DAT_PORT()==CMD_RET_SUCCESS )
            return( TRUE );  //成功 
	}
    
	return( FALSE );//CH376出错,例如芯片型号错或者处于串口方式或者不支持
}

//*****************************************************
//*	NAME:    set_freq(void)									 
//*	FUCTION: 设置CH376的进入低速模式									 
//*	输入参数：无
//* 输出参数：无									 
//*	说明： 对于鼠标键盘等低速设备，要先设置ch376为低速模式									 
//******************************************************

void set_freq(void)
{	
	CH376_WR_CMD_PORT(0x0b); // 切换使375B进入低速模式 
	CH376_WR_DAT_PORT(0x17);
	CH376_WR_DAT_PORT(0xd8);
}

//*****************************************************
//*	NAME:     RD_USB_DATA( UINT8 *buf )									 
//*	FUCTION:  从CH376的端点缓冲区读取接收到的数据									 
//*	输入参数: 数据缓冲区的地址
//* 输出参数：返回接收的数据长度									 
//*	说明：    从CH376的主机端点缓冲区中读取接收到的数据									 
//******************************************************

UINT8 RD_USB_DATA( UINT8 *buf )
{  
	UINT8 i, len;
    
	CH376_WR_CMD_PORT( CMD01_RD_USB_DATA0 );// 从CH37X读取数据块 
	len=CH376_RD_DAT_PORT();// 后续数据长度 
    
	for ( i=0; i!=len; i++ )
        *buf++ = CH376_RD_DAT_PORT();
    
	return( len );
}

//*****************************************************
//*	NAME:     WR_USB_DATA( UINT8 len, UINT8 *buf )									 
//*	FUCTION:  往CH376的端点缓冲区写入数据块								 
//*	输入参数: 要写入数据块的长度，写入数据缓冲区的地址
//* 输出参数：无									 
//*	说明：    往CH376的主机端点缓冲区中写入要发送的数据块									 
//******************************************************

void WR_USB_DATA( UINT8 len, UINT8 *buf )
{  
	CH376_WR_CMD_PORT( CMD10_WR_HOST_DATA );// 向CH376的端点缓冲区写入准备发送的数据
	CH376_WR_DAT_PORT( len );// 后续数据长度, len不能大于64
    
	while( len-- )
        CH376_WR_DAT_PORT( *buf++ );
}


//*****************************************************
//*	NAME:     issue_token(UINT8 endptog, UINT8 endp_and_pid )								 
//*	FUCTION:  执行USB事务								 
//*	输入参数: 同步标志，端点号和令牌
//* 输出参数：无									 
//*	说明：    高4位目的端点号, 低4位令牌PID									 
//******************************************************

void issue_token(UINT8 endptog, UINT8 endp_and_pid )
{  
	CH376_WR_CMD_PORT( CMD2H_ISSUE_TKN_X );
	CH376_WR_DAT_PORT( endptog );
	CH376_WR_DAT_PORT( endp_and_pid );
	mDelay2uS();
}


//*****************************************************
//*	NAME:     wait_interrupt( )								 
//*	FUCTION:  等待中断，并且获取中断状态								 
//*	输入参数: 无
//* 输出参数：中断状态									 
//*	说明：    CH376操作完成中断(INT#低电平)   									 
//******************************************************

UINT8 wait_interrupt( ) 
{  
	Wait376Interrupt();
	CH376_WR_CMD_PORT(CMD_GET_STATUS); 
	return( CH376_RD_DAT_PORT() );
}

//*****************************************************
//*	NAME:     Get_Dev_Descr( )								 
//*	FUCTION:  获取设备描述符								 
//*	输入参数: 无
//* 输出参数：成功返回1，否则返回0									 
//*	说明：    该程序采用外置固件模式获取设备描述符   									 
//******************************************************

UINT8 Get_Dev_Descr( )
{	
    UINT8 descr_len;
	UINT8  *p=data_buf;

	send_mode=0x00;
	WR_USB_DATA(8,SetupGetDevDescr);

	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_SETUP );
	status = wait_interrupt();
//	printf("status1=%02x\n",(unsigned short)status);
	if(status==USB_INT_SUCCESS) //SETUP阶段操作成功
	{
		receive_mode = 0x80;
	}
	else
    {
        return(0);
    }

	issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN );
	status=wait_interrupt();
    
	if(status==USB_INT_SUCCESS) //DATA阶段操作成功
	{
		descr_len=data_buf[0]-RD_USB_DATA(data_buf);	
		
		while( descr_len > 0 )
		{	
			receive_mode ^= 0x80;
			p+=0x08;
			issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);
			status=wait_interrupt();
            
			if(status==USB_INT_SUCCESS) //DATA阶段操作成功
				descr_len-=RD_USB_DATA(p);	
			else
            {
                return(0);
            }
		}
	}
	else
    {
        return(0);
    }

	send_mode=0x40;
	WR_USB_DATA(0,SetupGetDevDescr);
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_OUT);
	status=wait_interrupt();
    
    //状态阶段操作成功
	if(status==USB_INT_SUCCESS) 
    {
		return(1);
    }
	else
    {
        return(0);
    }
}

//*****************************************************
//*	NAME:     Get_Cfg_Descr( PUINT8 buf )								 
//*	FUCTION:  获取配置描述符								 
//*	输入参数: 接收缓冲区地址
//* 输出参数：成功返回1，否则返回0									 
//*	说明：    该程序采用外置固件模式获取配置描述符   									 
//******************************************************

UINT8 Get_Cfg_Descr( PUINT8 buf )
{
	unsigned char descr_len;
	unsigned char *p = data_buf;
    
	send_mode=0x00;
	WR_USB_DATA(8,buf);
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_SETUP);
    status=wait_interrupt();
    
	if(status==USB_INT_SUCCESS) //SETUP阶段操作成功
	{
		receive_mode=0x80;
	}
	else
    {
        return(0);
    }
    
	issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);
    status=wait_interrupt();
    
	if(status==USB_INT_SUCCESS) //DATA阶段操作成功
	{	
	    receive_mode ^= 0x80;
		if(flag_config_2) //第二次获取设备的配置描述符
        {
	         descr_len = data_buf[2] - RD_USB_DATA(data_buf);
        }
		else
        {
            descr_len = data_buf[0] - RD_USB_DATA(data_buf);
        }
        
		while(descr_len > 0)
		{	
			p+=0x08;
			issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);status=wait_interrupt();
            
			if(status==USB_INT_SUCCESS) //DATA阶段操作成功
            {
                receive_mode ^= 0x80;
                descr_len-=RD_USB_DATA(p);	
            }
			else
            {
                return(0);
            }
		}
	}
	else
    {
        return(0);
    }
    
	send_mode = 0x40;
	WR_USB_DATA(0,SetupGetCfgDescr);
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_OUT);status=wait_interrupt();
	if(status==USB_INT_SUCCESS) //状态阶段操作成功
    {
		return(1);
    }
	else
    {
        return(0);
    }
}

//*****************************************************
//*	NAME:     parse_config_descr( )								 
//*	FUCTION:  简单的分析配置描述符中的相关信息，并保存								 
//*	输入参数: 无
//* 输出参数：无									 
//*	说明：    保存设备的接口号,端点地址,报表长度,支持复合设备   									 
//******************************************************

void parse_config_descr( )
{
    Device_Atti.Device_connect = 1; //表示设备连接
    Device_Atti.Cfg_value = ((PUSB_CFG_DESCR)data_buf)->bConfigurationValue;  //保留配置描述符中的配置值

    if(((PUSB_CFG_DESCR)data_buf)->bNumInterfaces==1) //只有一个接口设备
    {
        Device_Atti.Device_compat = 0; //非复合设备

 #define HID_Dev  ((PHID_DEVICE)data_buf)
    
        Device_Atti.Device[0].Device_type =  HID_Dev->hid_itf_descr.bInterfaceProtocol;	//设备类型：0x01 键盘，0x02 鼠标
        Device_Atti.Device[0].Device_inf  =  HID_Dev->hid_itf_descr.bInterfaceNumber;	//设备的接口号
        Device_Atti.Device[0].Device_endp =  HID_Dev->endp_descr.bEndpointAddress;		//设备端点地址
        Device_Atti.Device[0].Device_size =  HID_Dev->endp_descr.wMaxPacketSize;		//设备端点大小
        Device_Atti.Device[0].Device_report_len =  (HID_Dev->hid_class_descr.wDescriptorLength>>8)|(HID_Dev->hid_class_descr.wDescriptorLength<<8);	   //报表长度，大小端数据格式转换
    }
    //2个接口设备
    else if(((PUSB_CFG_DESCR)data_buf)->bNumInterfaces==2)
    {
        Device_Atti.Device_compat = 1;//复合设备

 #define HID_Dev1  ((PHID_COMPOSITE_DEVICE1)data_buf)
    
        Device_Atti.Device[0].Device_type =  HID_Dev1->hid_itf_descr1.bInterfaceProtocol;
        Device_Atti.Device[0].Device_inf  =  HID_Dev1->hid_itf_descr1.bInterfaceNumber;
        Device_Atti.Device[0].Device_endp =  HID_Dev1->endp_descr1.bEndpointAddress;
        Device_Atti.Device[0].Device_size =  HID_Dev1->endp_descr1.wMaxPacketSize;
        Device_Atti.Device[0].Device_report_len =  (HID_Dev1->hid_class_descr1.wDescriptorLength>>8)|(HID_Dev1->hid_class_descr1.wDescriptorLength<<8);	 //报表长度，大小端数据格式转换
  
        Device_Atti.Device[1].Device_type =  HID_Dev1->hid_itf_descr2.bInterfaceProtocol;
        Device_Atti.Device[1].Device_inf  =  HID_Dev1->hid_itf_descr2.bInterfaceNumber;
        Device_Atti.Device[1].Device_endp =  HID_Dev1->endp_descr2.bEndpointAddress;
        Device_Atti.Device[1].Device_size =  HID_Dev1->endp_descr2.wMaxPacketSize;
        Device_Atti.Device[1].Device_report_len =  (HID_Dev1->hid_class_descr2.wDescriptorLength>>8)|(HID_Dev1->hid_class_descr2.wDescriptorLength<<8);	 //报表长度，大小端数据格式转换

    }
}


//*****************************************************
//*	NAME:     set_config( UINT8 cfg )								 
//*	FUCTION:  配置USB设备								 
//*	输入参数: 配置值
//* 输出参数：操作状态 成功返回0x14									 
//*	说明：    该配置值取自配置描述符中   									 
//******************************************************

UINT8 set_config( UINT8 cfg )
{  
	CH376_WR_CMD_PORT( CMD_SET_CONFIG );  
	CH376_WR_DAT_PORT( cfg );
    
	return( wait_interrupt() );
}

//*****************************************************
//*	NAME:     set_idle( UINT8 inf )								 
//*	FUCTION:  设置HID设备的IDLE								 
//*	输入参数: 接口号
//* 输出参数：操作状态 成功返回1									 
//*	说明：       									 
//******************************************************

UINT8 set_idle( UINT8 inf )
{
	send_mode=0x00;
	memcpy(data_buf,SetupSetidle,8);
	data_buf[4] = inf;
	WR_USB_DATA(8,data_buf);//SETUP数据总是8字节
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_SETUP);
	status=wait_interrupt();
	if(status==USB_INT_SUCCESS)//SETUP阶段操作成功
	{
		receive_mode=0x80;
		issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN );status=wait_interrupt();
		if(status==USB_INT_SUCCESS)//状态阶段操作成功
		{
			if(RD_USB_DATA(data_buf)!=0)
            {
                return(0);
            }
		}
		else
        {
            return(0);
        }
	}
	else
    {
        return(0);
    }
    
	return(1);	
}


//*****************************************************
//*	NAME:     get_report_descr_ex( UINT8 inf,UINT16 len)								 
//*	FUCTION:  获取HID类设备的报表描述符								 
//*	输入参数: 接口号，报表长度
//* 输出参数：操作状态 成功返回1									 
//*	说明：    获取HID类的报表，报表长度取自HID类描述符中   									 
//******************************************************

UINT8 get_report_descr_ex( UINT8 inf,UINT16 len)		
{
	unsigned char descr_len;
	unsigned char *p=data_buf;
	unsigned char report_cou_temp=0;
	send_mode=0x00;
	memcpy(data_buf,SetupGetHidDes,8);
	data_buf[4] = inf;
	data_buf[6] = len+0x40;
	descr_len = len;					
	WR_USB_DATA(8,data_buf);		
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_SETUP);
    status=wait_interrupt();	
	if(status==USB_INT_SUCCESS)//SETUP阶段操作成功
	{	
	    receive_mode = 0x80;	
	}
	else return(0);
	issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);status=wait_interrupt();
	if(status==USB_INT_SUCCESS)//DATA阶段操作成功
	{	
	   
	   RD_USB_DATA( p );
		descr_len -= 0x08;//剩余描述符长度计算
		while(descr_len>0)
		{	
			receive_mode ^= 0x80;
			p+=0x08;
			issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);status=wait_interrupt();
			if(status==USB_INT_SUCCESS)// DATA阶段操作成功
			{	
			    report_cou_temp=RD_USB_DATA(p);
				if(report_cou_temp!=0x08)
                {
                    descr_len -= report_cou_temp;
                    break;
                }
				else
                {
                    descr_len -= 0x08; 
                }
			}
			else
            {
                return(0);
            }
		}
	}
	else
    {
        return(0);
    }
    
	send_mode=0x40;
	WR_USB_DATA(0,data_buf);
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_OUT);
    status = wait_interrupt();
    
	if(status==USB_INT_SUCCESS)//状态阶段操作成功 
    {
		return(1);	
    }
	else
    {
        return(0);
    }
}


//*****************************************************
//*	NAME:     set_report( )								 
//*	FUCTION:  对于键盘设备，则可以设置报表								 
//*	输入参数: 无
//* 输出参数：操作状态 成功返回1									 
//*	说明：    可以通过设置报表，点亮键盘指示灯   									 
//******************************************************

UINT8 set_report( )
{	
	send_mode=0x00;
	WR_USB_DATA(8,SetupSetReport);//SETUP数据总是8字节
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_SETUP);status=wait_interrupt();
	if(status==USB_INT_SUCCESS)//SETUP阶段操作成功
	{	
	    send_mode ^= 0x40;
		data_buf[0]=0x01;
		WR_USB_DATA(1,data_buf);
		issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_OUT);
        status=wait_interrupt();
        
		if(status==USB_INT_SUCCESS)//DATA阶段操作成功
		{	
		    receive_mode=0x80;
			issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);status=wait_interrupt();
			if(status==USB_INT_SUCCESS)//状态阶段操作成功
			{	
			    if(RD_USB_DATA(data_buf)!=0)
				return(0);
			}
			else return(0);
		}
		else return(0);
	}	
	else return(0);
	return(1);
}

//*****************************************************
//*	NAME:     get_int_in(UINT8 tog,UINT8 endp_int)								 
//*	FUCTION:  获取HID类设备的数据								 
//*	输入参数: 该设备的同步标志，端点号
//* 输出参数：操作状态									 
//*	说明：    支持复合设备的操作  									 
//******************************************************

UINT8 get_int_in(UINT8 tog,UINT8 endp_int)
{
    UINT8 s,t,j;
    tog = tog ? 0x80 : 0x00;
    issue_token(tog,( endp_int << 4 ) | DEF_USB_PID_IN);
    s = wait_interrupt( );
    
    if(s == USB_INT_SUCCESS)
    {
        t = RD_USB_DATA(data_buf); //键盘中断端点数据长度一般为8字节，鼠标为4字节
        
        if( endp_int == Device_Atti.Device[0].Device_endp )
            Device_Atti.Device[0].tog = Device_Atti.Device[0].tog ? FALSE : TRUE;
        else 
            Device_Atti.Device[1].tog = Device_Atti.Device[1].tog ? FALSE : TRUE;
    }
    
    return s;
}


//*****************************************************
//*	NAME:     Reset_Device( )								 
//*	FUCTION:  复位USB设备，并且将CH376设置成主机模式								 
//*	输入参数: 无
//* 输出参数：无									 
//*	说明：    USB规范中未要求USB设备插入后必须复位该设备，
//*           有些USB设备也要求在插入后必须先复位才能工作  									 
//******************************************************

void Reset_Device( )
{	
	Set_USB_Mode( 7 );  													//复位USB设备,CH376向USB信号线的D+和D-输出低电平
	mDelaymS(10);
	Set_USB_Mode( 6 );  													//结束复位，将CH376设置成主机模式
	while ( wait_interrupt()!=USB_INT_CONNECT );  							//等待复位之后的设备端再次连接上来

}

//*****************************************************
//*	NAME:     set_addr( UINT8 addr )								 
//*	FUCTION:  设置USB设备地址，并且设置USB主机端要操作的USB设备地址								 
//*	输入参数: 地址值(1~127)
//* 输出参数：操作状态									 
//*	说明：    
//******************************************************

UINT8 set_addr( UINT8 addr ) { 
	UINT8 status;
	CH376_WR_CMD_PORT( CMD_SET_ADDRESS );  
	CH376_WR_DAT_PORT( addr );  
	status=wait_interrupt(); //等待CH376操作完成
	if ( status==USB_INT_SUCCESS ) { //操作成功
		CH376_WR_CMD_PORT( CMD_SET_USB_ADDR ); //设置USB主机端的USB地址
		CH376_WR_DAT_PORT( addr ); //当目标USB设备的地址成功修改后,应该同步修改主机端的USB地址
	}
	return( status );
}

//**********************************************
//*	NAME:    main( void )									 
//*	FUCTION: 通过CH376操作鼠标键盘以及复合HID设备的数据									 
//*	输入参数：无
//* 输出参数：无									 
//*	说明：只适用普通的USB键盘，支持复合设备
//*       键盘，不支持带有HUB接口的USB键盘									 
//**********************************************

void mainx( void )
{
    UINT16 i;
    UINT8 s;

    //做测试命令，检测单片机和CH376硬件连接，以及读写时序是否正确  
    CH376_WR_CMD_PORT( 0x06 );
    CH376_WR_DAT_PORT( 0x65 );
    s = CH376_RD_DAT_PORT( );
    
    Set_USB_Mode( 6 ); //设置USB主机模式, 如果设备端是CH37X, 那么5和6均可
    set_freq( ); //使376进入低速模式
 
    while(1)
    {
        while ( wait_interrupt()!=USB_INT_CONNECT ); 
        mDelaymS(50);
        Reset_Device(); //复位usb设备
        mDelaymS(50);
        set_freq( ); //使376进入低速模式
        mDelaymS(200);
        mDelaymS(200);
        mDelaymS(200);
        mDelaymS(200);

        //设置地址		
        set_addr(5); 
        mDelaymS(200);
        mDelaymS(200);
        mDelaymS(200);

    //获取设备描述符
    Get_Dev_Descr();


    //获取配置描述符
    Get_Cfg_Descr( SetupGetCfgDescr );
    
    s = ((PUSB_CFG_DESCR)data_buf)->wTotalLengthL;
    memcpy( data_buf,SetupGetCfgDescr,8);
    data_buf[6] = s;

    flag_config_2 = 1;
    Get_Cfg_Descr( data_buf );
    
    flag_config_2 = 0;
    //保存配置描述符中设备信息
    parse_config_descr( );


    //设置USB配置   
    s = set_config( Device_Atti.Cfg_value );

    //设置HID类设备的IDLE
    s = set_idle( Device_Atti.Device[0].Device_inf );
    
   if(s == 1)
   {
//    printf("\n set idle success \n");
   }
   else
   {
//       printf("\n set idle error \n");
   }

//获取HID类设备的报表描述符
//printf("Device_Atti.Device[0].Device_inf=%02x \n",(unsigned short)Device_Atti.Device[0].Device_inf);
//printf("Device_Atti.Device[0].Device_report_len=%02x \n",(unsigned short)Device_Atti.Device[0].Device_report_len);
    s = get_report_descr_ex( Device_Atti.Device[0].Device_inf,Device_Atti.Device[0].Device_report_len);
//    printf("s=%02x \n",(unsigned short)s);
   
//    if( s == 1)
//	 {
//	   for(i=0;i!=Device_Atti.Device[0].Device_report_len;i++)
//	    printf(" %02x ",(UINT16)data_buf[i]);
//	 
//	 }
//	else printf(" report error %02x \n",(UINT16)s);
////如果该HID类设备是键盘，则可以设置报表
//	printf( "\n" );
//    printf( "Device_Atti.Device[0].Device_type =%02x\n",(unsigned short)Device_Atti.Device[0].Device_type );
	if(Device_Atti.Device[0].Device_type == 0x01)
	{
		s = set_report( );
//		if(s==1) printf("\n set report success \n");
//		else printf("\n set report error \n");
	}
    
//	printf( "Device_Atti.Device_compat =%02x\n",(unsigned short)Device_Atti.Device_compat );

//如果该设备是复合设备，则需要配置另外一个接口设备
#if 1
	if(Device_Atti.Device_compat == 1)
	{
//设置IDLE
	 s = set_idle( Device_Atti.Device[1].Device_inf );
//      if(s == 1) 
//   	  printf("\n set idle success \n");
//     else printf("\n set idle error %02x \n",(UINT16)s);

//获取报表描述符
     s = get_report_descr_ex( Device_Atti.Device[1].Device_inf,Device_Atti.Device[1].Device_report_len);
//	 printf("s=%02x \n",(unsigned short)s); 
      if( s == 1)
	   {
//	    for(i=0;i!=Device_Atti.Device[1].Device_report_len;i++)
//	    printf(" %02x ",(UINT16)data_buf[i]);	 
	   }

//如果该设备是键盘，则可以设置报表
//	printf("Device_Atti.Device[1].Device_type=%02x \n",(unsigned short)Device_Atti.Device[1].Device_type);
    if(Device_Atti.Device[1].Device_type == 0x01)
	 {
	  s = set_report( );
//       printf("s=%02x \n",(unsigned short)s); 
//	  if(s==1) printf("\n set report success \n");
//	  else printf("\n set report error \n");
 
 	 }
 
   }
#endif
//	printf("AT here\n");

 	CH376_WR_CMD_PORT( CMD20_SET_RETRY );//设置CH376重试次数
	CH376_WR_DAT_PORT( 0x25 );
	CH376_WR_DAT_PORT( 0xC0 );//为了保证兼容性对于部分鼠标键盘需要设置有限次重试(0xc0),而对于复合设备，
    //比如USB转PS2的设备，复合键盘，则不需要重试(0x00)


	Device_Atti.Device[0].tog = FALSE;//设置设备的同步标志，默认0
 	Device_Atti.Device[1].tog = FALSE;

   while(1)
   {
//获取设备1的数据
   	s = get_int_in( Device_Atti.Device[0].tog,Device_Atti.Device[0].Device_endp);

//如果该设备是符合设备，则需要获取设备2的数据
	if( Device_Atti.Device_compat )
	{
	 s = get_int_in( Device_Atti.Device[1].tog,Device_Atti.Device[1].Device_endp);
	}

//以下是检测设备插拔状态
	CH376_WR_CMD_PORT(CMD01_TEST_CONNECT );			//检测设备插拔
	 s = CH376_RD_DAT_PORT( );
	 if(s == USB_INT_DISCONNECT )
	 {
      Device_Atti.Device_connect = 0;
//	    Device_Atti = {0};
	  break;										//设备断开
   	 }
   }
 }

}