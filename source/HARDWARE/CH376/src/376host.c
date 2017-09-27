/* ��CH376����HID�豸��������̣����,����HID�����豸*/


#include <stdio.h>
#include <string.h>
#include "CH376INC.H"
#include "hid.h"

// ��ȡ�豸������
unsigned char SetupGetDevDescr[] = { 0x80, 0x06, 0x00, 0x01, 0x00, 0x00, 0x12, 0x00 };
// ��ȡ����������
unsigned char SetupGetCfgDescr[] = { 0x80, 0x06, 0x00, 0x02, 0x00, 0x00, 0x09, 0x00 };
// ����USB��ַ
unsigned char SetupSetUsbAddr[] = { 0x00, 0x05, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00 };
// ����USB����
//const	unsigned char code SetupSetUsbConfig[] = { 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// SET IDLE 
unsigned char SetupSetidle[]={0x21,0x0a,0x00,0x00,0x00,0x00,0x00,0x00};        
// ��ȡHID ���������� 
unsigned char SetupGetHidDes[]={0x81,0x06,0x00,0x22,0x00,0x00,0x81,0x00};    
// SET REPORT 
unsigned char SetupSetReport[]={0x21,0x09,0x00,0x02,0x00,0x00,0x01,0x00};     


////��Ƭ�����߷�ʽ����CH376
//UINT8V	xdata	CH376_CMD_PORT	_at_ 0xBDF1; /* CH376����˿ڵ�I/O��ַ */
//UINT8V	xdata	CH376_DAT_PORT	_at_ 0xBCF0; /* CH376���ݶ˿ڵ�I/O��ַ */


//#define CH376_INT_WIRE   INT0       //����CH376�ж����ţ���ʡ�ԣ�

struct _Device_Atti{
  	UINT8 Device_connect;      //�豸����״̬ 1�����ӣ�0���Ͽ�
  	UINT8 Device_compat;       //0:�Ǹ����豸,1��ʾ�����豸  
  	UINT8 Cfg_value;           //�豸����������������ֵ
  struct _Device{ 
  	UINT8 Device_type;	       //�豸���� 1:���̣�2:���
  	UINT8 Device_inf;	       //�豸�ӿں� Ĭ��Ϊ0  ���֧�������ӿ��豸
  	UINT8 Device_endp;         //�豸�˵��ַ ���֧��һ���˵�
  	UINT8 Device_size;         //�豸�˵��С 
  	UINT16 Device_report_len;   //�豸������
	UINT8 tog;				   //�˵��ͬ����־
  }Device[2];
}Device_Atti = { 0 };

UINT8 receive_mode = 0x00,send_mode = 0x00;
UINT8 idata, data_buf[120];
UINT8 status;
UINT8   flag_config_2; //�ڶ��λ�ȡ������������־

//**********************************************
//*	NAME:    mDelay2uS( )									 
//*	FUCTION: ��ʱ2us�Ӻ���									 
//*	�����������
//* �����������									 
//*	˵����    ���ݵ�Ƭ������Ƶ���е���									 
//**********************************************

static void mDelay0_5uS( void )  /* ������ʱ0.5uS,���ݵ�Ƭ����Ƶ���� */
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
//*	FUCTION: ��ʱ50ms�Ӻ���									 
//*	�����������
//* �����������									 
//*	˵����    ���ݵ�Ƭ������Ƶ���е���									 
//**********************************************

void mDelay50mS( )
{
    mDelay0_5uS();
}

//**********************************************
//*	NAME:    CH376_WR_CMD_PORT( UINT8 cmd )									 
//*	FUCTION: дCH376�����Ӻ���									 
//*	���������8λ������
//* �����������									 
//*	˵���������ٶȽϿ�ĵ�Ƭ��������Ҫ1.5uS��ʱ									 
//**********************************************

void CH376_WR_CMD_PORT( UINT8 cmd )
{
	xWriteCH376Cmd(cmd);
}

//**********************************************
//*	NAME:    CH376_WR_DAT_PORT( UINT8 dat )									 
//*	FUCTION: дCH376�����Ӻ���									 
//*	���������8λ����
//* �����������									 
//*	˵���������ٶȽϿ�ĵ�Ƭ��������Ҫ0.6uS��ʱ									 
//**********************************************

void CH376_WR_DAT_PORT( UINT8 dat )
{
	xWriteCH376Data(dat);
}

//**********************************************
//*	NAME:    CH376_RD_DAT_PORT( void )									 
//*	FUCTION: ��CH376�����Ӻ���									 
//*	�����������
//* ���������8λ����									 
//*	˵���������ٶȽϿ�ĵ�Ƭ��������Ҫ0.6uS��ʱ									 
//**********************************************

UINT8 CH376_RD_DAT_PORT( void )
{
  	return xReadCH376Data();
}


//**********************************************
//*	NAME:    StdioInit( void )									 
//*	FUCTION: ��Ƭ�����ڳ�ʼ�������ڼ�����									 
//*	�����������
//* �����������									 
//*	˵���� ��Ƭ����Ƶ24M ʹ�ö�ʱ��1 ������9600									 
//**********************************************

void StdioInit( void )
{

}

//**********************************************
//*	NAME:    Set_USB_Mode( UINT8 mode )									 
//*	FUCTION: ����CH376�Ĺ���ģʽ 0x06 Ϊ����ģʽ									 
//*	���������ģʽ����
//* �������������״̬ TRUE���ɹ���FALSEʧ��									 
//*	˵���� ����CH376�Ĺ���ģʽ									 
//**********************************************

UINT8 Set_USB_Mode( UINT8 mode )
{  
	UINT8 i;
    
	CH376_WR_CMD_PORT( CMD_SET_USB_MODE );
	CH376_WR_DAT_PORT( mode );
    
	receive_mode = send_mode = 0x00;  //�����˸�λUSB����ͬ����־ 
    
    //�ȴ�����ģʽ�������,������30uS 
	for( i=0; i!=100; i++ ) 
	{    
		if ( CH376_RD_DAT_PORT()==CMD_RET_SUCCESS )
            return( TRUE );  //�ɹ� 
	}
    
	return( FALSE );//CH376����,����оƬ�ͺŴ���ߴ��ڴ��ڷ�ʽ���߲�֧��
}

//*****************************************************
//*	NAME:    set_freq(void)									 
//*	FUCTION: ����CH376�Ľ������ģʽ									 
//*	�����������
//* �����������									 
//*	˵���� ���������̵ȵ����豸��Ҫ������ch376Ϊ����ģʽ									 
//******************************************************

void set_freq(void)
{	
	CH376_WR_CMD_PORT(0x0b); // �л�ʹ375B�������ģʽ 
	CH376_WR_DAT_PORT(0x17);
	CH376_WR_DAT_PORT(0xd8);
}

//*****************************************************
//*	NAME:     RD_USB_DATA( UINT8 *buf )									 
//*	FUCTION:  ��CH376�Ķ˵㻺������ȡ���յ�������									 
//*	�������: ���ݻ������ĵ�ַ
//* ������������ؽ��յ����ݳ���									 
//*	˵����    ��CH376�������˵㻺�����ж�ȡ���յ�������									 
//******************************************************

UINT8 RD_USB_DATA( UINT8 *buf )
{  
	UINT8 i, len;
    
	CH376_WR_CMD_PORT( CMD01_RD_USB_DATA0 );// ��CH37X��ȡ���ݿ� 
	len=CH376_RD_DAT_PORT();// �������ݳ��� 
    
	for ( i=0; i!=len; i++ )
        *buf++ = CH376_RD_DAT_PORT();
    
	return( len );
}

//*****************************************************
//*	NAME:     WR_USB_DATA( UINT8 len, UINT8 *buf )									 
//*	FUCTION:  ��CH376�Ķ˵㻺����д�����ݿ�								 
//*	�������: Ҫд�����ݿ�ĳ��ȣ�д�����ݻ������ĵ�ַ
//* �����������									 
//*	˵����    ��CH376�������˵㻺������д��Ҫ���͵����ݿ�									 
//******************************************************

void WR_USB_DATA( UINT8 len, UINT8 *buf )
{  
	CH376_WR_CMD_PORT( CMD10_WR_HOST_DATA );// ��CH376�Ķ˵㻺����д��׼�����͵�����
	CH376_WR_DAT_PORT( len );// �������ݳ���, len���ܴ���64
    
	while( len-- )
        CH376_WR_DAT_PORT( *buf++ );
}


//*****************************************************
//*	NAME:     issue_token(UINT8 endptog, UINT8 endp_and_pid )								 
//*	FUCTION:  ִ��USB����								 
//*	�������: ͬ����־���˵�ź�����
//* �����������									 
//*	˵����    ��4λĿ�Ķ˵��, ��4λ����PID									 
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
//*	FUCTION:  �ȴ��жϣ����һ�ȡ�ж�״̬								 
//*	�������: ��
//* ����������ж�״̬									 
//*	˵����    CH376��������ж�(INT#�͵�ƽ)   									 
//******************************************************

UINT8 wait_interrupt( ) 
{  
	Wait376Interrupt();
	CH376_WR_CMD_PORT(CMD_GET_STATUS); 
	return( CH376_RD_DAT_PORT() );
}

//*****************************************************
//*	NAME:     Get_Dev_Descr( )								 
//*	FUCTION:  ��ȡ�豸������								 
//*	�������: ��
//* ����������ɹ�����1�����򷵻�0									 
//*	˵����    �ó���������ù̼�ģʽ��ȡ�豸������   									 
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
	if(status==USB_INT_SUCCESS) //SETUP�׶β����ɹ�
	{
		receive_mode = 0x80;
	}
	else
    {
        return(0);
    }

	issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN );
	status=wait_interrupt();
    
	if(status==USB_INT_SUCCESS) //DATA�׶β����ɹ�
	{
		descr_len=data_buf[0]-RD_USB_DATA(data_buf);	
		
		while( descr_len > 0 )
		{	
			receive_mode ^= 0x80;
			p+=0x08;
			issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);
			status=wait_interrupt();
            
			if(status==USB_INT_SUCCESS) //DATA�׶β����ɹ�
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
    
    //״̬�׶β����ɹ�
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
//*	FUCTION:  ��ȡ����������								 
//*	�������: ���ջ�������ַ
//* ����������ɹ�����1�����򷵻�0									 
//*	˵����    �ó���������ù̼�ģʽ��ȡ����������   									 
//******************************************************

UINT8 Get_Cfg_Descr( PUINT8 buf )
{
	unsigned char descr_len;
	unsigned char *p = data_buf;
    
	send_mode=0x00;
	WR_USB_DATA(8,buf);
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_SETUP);
    status=wait_interrupt();
    
	if(status==USB_INT_SUCCESS) //SETUP�׶β����ɹ�
	{
		receive_mode=0x80;
	}
	else
    {
        return(0);
    }
    
	issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);
    status=wait_interrupt();
    
	if(status==USB_INT_SUCCESS) //DATA�׶β����ɹ�
	{	
	    receive_mode ^= 0x80;
		if(flag_config_2) //�ڶ��λ�ȡ�豸������������
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
            
			if(status==USB_INT_SUCCESS) //DATA�׶β����ɹ�
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
	if(status==USB_INT_SUCCESS) //״̬�׶β����ɹ�
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
//*	FUCTION:  �򵥵ķ��������������е������Ϣ��������								 
//*	�������: ��
//* �����������									 
//*	˵����    �����豸�Ľӿں�,�˵��ַ,������,֧�ָ����豸   									 
//******************************************************

void parse_config_descr( )
{
    Device_Atti.Device_connect = 1; //��ʾ�豸����
    Device_Atti.Cfg_value = ((PUSB_CFG_DESCR)data_buf)->bConfigurationValue;  //���������������е�����ֵ

    if(((PUSB_CFG_DESCR)data_buf)->bNumInterfaces==1) //ֻ��һ���ӿ��豸
    {
        Device_Atti.Device_compat = 0; //�Ǹ����豸

 #define HID_Dev  ((PHID_DEVICE)data_buf)
    
        Device_Atti.Device[0].Device_type =  HID_Dev->hid_itf_descr.bInterfaceProtocol;	//�豸���ͣ�0x01 ���̣�0x02 ���
        Device_Atti.Device[0].Device_inf  =  HID_Dev->hid_itf_descr.bInterfaceNumber;	//�豸�Ľӿں�
        Device_Atti.Device[0].Device_endp =  HID_Dev->endp_descr.bEndpointAddress;		//�豸�˵��ַ
        Device_Atti.Device[0].Device_size =  HID_Dev->endp_descr.wMaxPacketSize;		//�豸�˵��С
        Device_Atti.Device[0].Device_report_len =  (HID_Dev->hid_class_descr.wDescriptorLength>>8)|(HID_Dev->hid_class_descr.wDescriptorLength<<8);	   //�����ȣ���С�����ݸ�ʽת��
    }
    //2���ӿ��豸
    else if(((PUSB_CFG_DESCR)data_buf)->bNumInterfaces==2)
    {
        Device_Atti.Device_compat = 1;//�����豸

 #define HID_Dev1  ((PHID_COMPOSITE_DEVICE1)data_buf)
    
        Device_Atti.Device[0].Device_type =  HID_Dev1->hid_itf_descr1.bInterfaceProtocol;
        Device_Atti.Device[0].Device_inf  =  HID_Dev1->hid_itf_descr1.bInterfaceNumber;
        Device_Atti.Device[0].Device_endp =  HID_Dev1->endp_descr1.bEndpointAddress;
        Device_Atti.Device[0].Device_size =  HID_Dev1->endp_descr1.wMaxPacketSize;
        Device_Atti.Device[0].Device_report_len =  (HID_Dev1->hid_class_descr1.wDescriptorLength>>8)|(HID_Dev1->hid_class_descr1.wDescriptorLength<<8);	 //�����ȣ���С�����ݸ�ʽת��
  
        Device_Atti.Device[1].Device_type =  HID_Dev1->hid_itf_descr2.bInterfaceProtocol;
        Device_Atti.Device[1].Device_inf  =  HID_Dev1->hid_itf_descr2.bInterfaceNumber;
        Device_Atti.Device[1].Device_endp =  HID_Dev1->endp_descr2.bEndpointAddress;
        Device_Atti.Device[1].Device_size =  HID_Dev1->endp_descr2.wMaxPacketSize;
        Device_Atti.Device[1].Device_report_len =  (HID_Dev1->hid_class_descr2.wDescriptorLength>>8)|(HID_Dev1->hid_class_descr2.wDescriptorLength<<8);	 //�����ȣ���С�����ݸ�ʽת��

    }
}


//*****************************************************
//*	NAME:     set_config( UINT8 cfg )								 
//*	FUCTION:  ����USB�豸								 
//*	�������: ����ֵ
//* �������������״̬ �ɹ�����0x14									 
//*	˵����    ������ֵȡ��������������   									 
//******************************************************

UINT8 set_config( UINT8 cfg )
{  
	CH376_WR_CMD_PORT( CMD_SET_CONFIG );  
	CH376_WR_DAT_PORT( cfg );
    
	return( wait_interrupt() );
}

//*****************************************************
//*	NAME:     set_idle( UINT8 inf )								 
//*	FUCTION:  ����HID�豸��IDLE								 
//*	�������: �ӿں�
//* �������������״̬ �ɹ�����1									 
//*	˵����       									 
//******************************************************

UINT8 set_idle( UINT8 inf )
{
	send_mode=0x00;
	memcpy(data_buf,SetupSetidle,8);
	data_buf[4] = inf;
	WR_USB_DATA(8,data_buf);//SETUP��������8�ֽ�
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_SETUP);
	status=wait_interrupt();
	if(status==USB_INT_SUCCESS)//SETUP�׶β����ɹ�
	{
		receive_mode=0x80;
		issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN );status=wait_interrupt();
		if(status==USB_INT_SUCCESS)//״̬�׶β����ɹ�
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
//*	FUCTION:  ��ȡHID���豸�ı���������								 
//*	�������: �ӿںţ�������
//* �������������״̬ �ɹ�����1									 
//*	˵����    ��ȡHID��ı���������ȡ��HID����������   									 
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
	if(status==USB_INT_SUCCESS)//SETUP�׶β����ɹ�
	{	
	    receive_mode = 0x80;	
	}
	else return(0);
	issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);status=wait_interrupt();
	if(status==USB_INT_SUCCESS)//DATA�׶β����ɹ�
	{	
	   
	   RD_USB_DATA( p );
		descr_len -= 0x08;//ʣ�����������ȼ���
		while(descr_len>0)
		{	
			receive_mode ^= 0x80;
			p+=0x08;
			issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);status=wait_interrupt();
			if(status==USB_INT_SUCCESS)// DATA�׶β����ɹ�
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
    
	if(status==USB_INT_SUCCESS)//״̬�׶β����ɹ� 
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
//*	FUCTION:  ���ڼ����豸����������ñ���								 
//*	�������: ��
//* �������������״̬ �ɹ�����1									 
//*	˵����    ����ͨ�����ñ�����������ָʾ��   									 
//******************************************************

UINT8 set_report( )
{	
	send_mode=0x00;
	WR_USB_DATA(8,SetupSetReport);//SETUP��������8�ֽ�
	issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_SETUP);status=wait_interrupt();
	if(status==USB_INT_SUCCESS)//SETUP�׶β����ɹ�
	{	
	    send_mode ^= 0x40;
		data_buf[0]=0x01;
		WR_USB_DATA(1,data_buf);
		issue_token(send_mode,( 0 << 4 ) | DEF_USB_PID_OUT);
        status=wait_interrupt();
        
		if(status==USB_INT_SUCCESS)//DATA�׶β����ɹ�
		{	
		    receive_mode=0x80;
			issue_token(receive_mode,( 0 << 4 ) | DEF_USB_PID_IN);status=wait_interrupt();
			if(status==USB_INT_SUCCESS)//״̬�׶β����ɹ�
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
//*	FUCTION:  ��ȡHID���豸������								 
//*	�������: ���豸��ͬ����־���˵��
//* �������������״̬									 
//*	˵����    ֧�ָ����豸�Ĳ���  									 
//******************************************************

UINT8 get_int_in(UINT8 tog,UINT8 endp_int)
{
    UINT8 s,t,j;
    tog = tog ? 0x80 : 0x00;
    issue_token(tog,( endp_int << 4 ) | DEF_USB_PID_IN);
    s = wait_interrupt( );
    
    if(s == USB_INT_SUCCESS)
    {
        t = RD_USB_DATA(data_buf); //�����ж϶˵����ݳ���һ��Ϊ8�ֽڣ����Ϊ4�ֽ�
        
        if( endp_int == Device_Atti.Device[0].Device_endp )
            Device_Atti.Device[0].tog = Device_Atti.Device[0].tog ? FALSE : TRUE;
        else 
            Device_Atti.Device[1].tog = Device_Atti.Device[1].tog ? FALSE : TRUE;
    }
    
    return s;
}


//*****************************************************
//*	NAME:     Reset_Device( )								 
//*	FUCTION:  ��λUSB�豸�����ҽ�CH376���ó�����ģʽ								 
//*	�������: ��
//* �����������									 
//*	˵����    USB�淶��δҪ��USB�豸�������븴λ���豸��
//*           ��ЩUSB�豸ҲҪ���ڲ��������ȸ�λ���ܹ���  									 
//******************************************************

void Reset_Device( )
{	
	Set_USB_Mode( 7 );  													//��λUSB�豸,CH376��USB�ź��ߵ�D+��D-����͵�ƽ
	mDelaymS(10);
	Set_USB_Mode( 6 );  													//������λ����CH376���ó�����ģʽ
	while ( wait_interrupt()!=USB_INT_CONNECT );  							//�ȴ���λ֮����豸���ٴ���������

}

//*****************************************************
//*	NAME:     set_addr( UINT8 addr )								 
//*	FUCTION:  ����USB�豸��ַ����������USB������Ҫ������USB�豸��ַ								 
//*	�������: ��ֵַ(1~127)
//* �������������״̬									 
//*	˵����    
//******************************************************

UINT8 set_addr( UINT8 addr ) { 
	UINT8 status;
	CH376_WR_CMD_PORT( CMD_SET_ADDRESS );  
	CH376_WR_DAT_PORT( addr );  
	status=wait_interrupt(); //�ȴ�CH376�������
	if ( status==USB_INT_SUCCESS ) { //�����ɹ�
		CH376_WR_CMD_PORT( CMD_SET_USB_ADDR ); //����USB�����˵�USB��ַ
		CH376_WR_DAT_PORT( addr ); //��Ŀ��USB�豸�ĵ�ַ�ɹ��޸ĺ�,Ӧ��ͬ���޸������˵�USB��ַ
	}
	return( status );
}

//**********************************************
//*	NAME:    main( void )									 
//*	FUCTION: ͨ��CH376�����������Լ�����HID�豸������									 
//*	�����������
//* �����������									 
//*	˵����ֻ������ͨ��USB���̣�֧�ָ����豸
//*       ���̣���֧�ִ���HUB�ӿڵ�USB����									 
//**********************************************

void mainx( void )
{
    UINT16 i;
    UINT8 s;

    //�����������ⵥƬ����CH376Ӳ�����ӣ��Լ���дʱ���Ƿ���ȷ  
    CH376_WR_CMD_PORT( 0x06 );
    CH376_WR_DAT_PORT( 0x65 );
    s = CH376_RD_DAT_PORT( );
    
    Set_USB_Mode( 6 ); //����USB����ģʽ, ����豸����CH37X, ��ô5��6����
    set_freq( ); //ʹ376�������ģʽ
 
    while(1)
    {
        while ( wait_interrupt()!=USB_INT_CONNECT ); 
        mDelaymS(50);
        Reset_Device(); //��λusb�豸
        mDelaymS(50);
        set_freq( ); //ʹ376�������ģʽ
        mDelaymS(200);
        mDelaymS(200);
        mDelaymS(200);
        mDelaymS(200);

        //���õ�ַ		
        set_addr(5); 
        mDelaymS(200);
        mDelaymS(200);
        mDelaymS(200);

    //��ȡ�豸������
    Get_Dev_Descr();


    //��ȡ����������
    Get_Cfg_Descr( SetupGetCfgDescr );
    
    s = ((PUSB_CFG_DESCR)data_buf)->wTotalLengthL;
    memcpy( data_buf,SetupGetCfgDescr,8);
    data_buf[6] = s;

    flag_config_2 = 1;
    Get_Cfg_Descr( data_buf );
    
    flag_config_2 = 0;
    //�����������������豸��Ϣ
    parse_config_descr( );


    //����USB����   
    s = set_config( Device_Atti.Cfg_value );

    //����HID���豸��IDLE
    s = set_idle( Device_Atti.Device[0].Device_inf );
    
   if(s == 1)
   {
//    printf("\n set idle success \n");
   }
   else
   {
//       printf("\n set idle error \n");
   }

//��ȡHID���豸�ı���������
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
////�����HID���豸�Ǽ��̣���������ñ���
//	printf( "\n" );
//    printf( "Device_Atti.Device[0].Device_type =%02x\n",(unsigned short)Device_Atti.Device[0].Device_type );
	if(Device_Atti.Device[0].Device_type == 0x01)
	{
		s = set_report( );
//		if(s==1) printf("\n set report success \n");
//		else printf("\n set report error \n");
	}
    
//	printf( "Device_Atti.Device_compat =%02x\n",(unsigned short)Device_Atti.Device_compat );

//������豸�Ǹ����豸������Ҫ��������һ���ӿ��豸
#if 1
	if(Device_Atti.Device_compat == 1)
	{
//����IDLE
	 s = set_idle( Device_Atti.Device[1].Device_inf );
//      if(s == 1) 
//   	  printf("\n set idle success \n");
//     else printf("\n set idle error %02x \n",(UINT16)s);

//��ȡ����������
     s = get_report_descr_ex( Device_Atti.Device[1].Device_inf,Device_Atti.Device[1].Device_report_len);
//	 printf("s=%02x \n",(unsigned short)s); 
      if( s == 1)
	   {
//	    for(i=0;i!=Device_Atti.Device[1].Device_report_len;i++)
//	    printf(" %02x ",(UINT16)data_buf[i]);	 
	   }

//������豸�Ǽ��̣���������ñ���
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

 	CH376_WR_CMD_PORT( CMD20_SET_RETRY );//����CH376���Դ���
	CH376_WR_DAT_PORT( 0x25 );
	CH376_WR_DAT_PORT( 0xC0 );//Ϊ�˱�֤�����Զ��ڲ�����������Ҫ�������޴�����(0xc0),�����ڸ����豸��
    //����USBתPS2���豸�����ϼ��̣�����Ҫ����(0x00)


	Device_Atti.Device[0].tog = FALSE;//�����豸��ͬ����־��Ĭ��0
 	Device_Atti.Device[1].tog = FALSE;

   while(1)
   {
//��ȡ�豸1������
   	s = get_int_in( Device_Atti.Device[0].tog,Device_Atti.Device[0].Device_endp);

//������豸�Ƿ����豸������Ҫ��ȡ�豸2������
	if( Device_Atti.Device_compat )
	{
	 s = get_int_in( Device_Atti.Device[1].tog,Device_Atti.Device[1].Device_endp);
	}

//�����Ǽ���豸���״̬
	CH376_WR_CMD_PORT(CMD01_TEST_CONNECT );			//����豸���
	 s = CH376_RD_DAT_PORT( );
	 if(s == USB_INT_DISCONNECT )
	 {
      Device_Atti.Device_connect = 0;
//	    Device_Atti = {0};
	  break;										//�豸�Ͽ�
   	 }
   }
 }

}