#include "base.h"
#define EXTERN extern
#include "Var.h"

#define vu8  volatile unsigned char
#define vu16 volatile unsigned int
#define vu32 volatile unsigned long

#define HIGH   	1
#define LOW   	0

#define SDA   	_pa3
#define SCL   	_pa1
#define SDA_C  	_pac3
#define SCL_C  	_pac1

#define SDA_PU  _papu3
#define SCL_PU  _papu1

#define SDA_HIGH   	SDA = HIGH
#define SDA_LOW   	SDA = LOW
#define SDA_INPUT   SDA_C = HIGH
#define SDA_OUTPUT  SDA_C = LOW

#define SCL_HIGH   	SCL = HIGH
#define SCL_LOW   	SCL = LOW
#define SCL_INPUT   SCL_C = HIGH
#define SCL_OUTPUT  SCL_C = LOW

vu8 	WAIT_SLAVE_ACK_F;

void _delay(u16 Cnt)
{
	for(;Cnt>0;Cnt--);
}

extern void sys_init(void)	;
extern void IIC_Start(void) ;
extern void IIC_Stop(void) ;
extern void SEND_Ack(void) ;
extern void SEND_NoAck(void) ;
extern vu8 Test_Ack(void) ;
extern void IIC_Write(vu8 Data) ;
extern vu8 IIC_Read(void) ;
extern void	byte_iic_read (void) ;

//////////////////////////////////////////////////
//������:IIC_START	�_ʼ��̖					//
//��  ��:										//
//��  ��:										//
//////////////////////////////////////////////////
void IIC_START()
{
	SCL_INPUT;
	SDA_INPUT;
	GCC_DELAY(10) ;
	SDA_LOW;
    SDA_OUTPUT;
	GCC_DELAY(10) ;
	SCL_LOW;
    SCL_OUTPUT;
    GCC_DELAY(10) ;	
}
//////////////////////////////////////////////////
//������:IIC_STOP	ֹͣ��̖					//
//��  ��:										//
//��  ��:										//
//////////////////////////////////////////////////
void IIC_STOP()
{
	SDA_LOW;
    SDA_OUTPUT;
    SDA_LOW;
    SDA_OUTPUT;
	GCC_DELAY(10) ;
	SCL_INPUT;   
	GCC_DELAY(10) ;
	SDA_INPUT;
	GCC_DELAY(10) ;
}
//////////////////////////////////////////////////
//������:WAIT_SLAVE_ACK	�ȴ��ęC����			//
//��  ��:										//
//��  ��:										//
//////////////////////////////////////////////////
vu8 WAIT_SLAVE_ACK(void)
{
	SCL_LOW;
	SCL_OUTPUT;
	SDA_INPUT;
	GCC_DELAY(10) ;
	SCL_INPUT;
	if(SDA==HIGH)
	{
		WAIT_SLAVE_ACK_F=1;			//�ęC�o����		
	}
	else
	{
		WAIT_SLAVE_ACK_F=0;			//�ęC����
	}  
    GCC_DELAY(10) ;
    while(!SCL);
    SCL_LOW;
	SCL_OUTPUT;       
    GCC_DELAY(10) ;
	return WAIT_SLAVE_ACK_F;	
}
//////////////////////////////////////////////////
//������:MASTER_SEND_ACK	���C�l�͑���		//
//��  ��:										//
//��  ��:										//
//////////////////////////////////////////////////
void MASTER_SEND_ACK(void)						
{	
	SCL_LOW;
	SCL_OUTPUT;
	SDA_INPUT;
	GCC_DELAY(10) ;
    SDA_LOW;
    SDA_OUTPUT;   
    GCC_DELAY(10) ;
    SCL_INPUT;
    GCC_DELAY(10) ;
	SCL_LOW;
	SCL_OUTPUT;       
    GCC_DELAY(10) ;
    SDA_INPUT;
    GCC_DELAY(10) ;
    GCC_DELAY(10) ;
		
}
//////////////////////////////////////////////////
//������:MASTER_SEND_NOACK	���C�l�͟o����		//
//��  ��:										//
//��  ��:										//
//////////////////////////////////////////////////
void MASTER_SEND_NOACK(void)								
{
	SDA_LOW;
    SDA_OUTPUT;
	SCL_LOW;
	SCL_OUTPUT;	
    GCC_DELAY(10) ;
    SDA_INPUT;
    GCC_DELAY(10) ;
    SCL_INPUT;
    GCC_DELAY(10) ;
    SCL_LOW;
    SCL_OUTPUT;    
    GCC_DELAY(10) ;
    SDA_LOW;
    SDA_OUTPUT;
    GCC_DELAY(10) ;
    GCC_DELAY(10) ;
}
//////////////////////////////////////////////////
//������:IIC_WRITE_BYTE		��һ�ֹ�			//
//��  ��:IIC_WRITE_DATA							//
//��  ��:										//
//////////////////////////////////////////////////
void IIC_WRITE_BYTE(vu8 IIC_WRITE_DATA)
{
	vu8 i;	
	
		for(i=0 ; i<8 ; i++)
		{	
			if(IIC_WRITE_DATA&0b10000000)
			{
				SDA_INPUT;
			}
			else
			{
				SDA_LOW;
				SDA_OUTPUT;			
			}		
		GCC_DELAY(10) ;
		SCL_INPUT;		
		GCC_DELAY(10) ;
		while(!SCL);
		SCL_LOW;
		SCL_OUTPUT;
		IIC_WRITE_DATA <<= 1;
		}
		
}
//////////////////////////////////////////////////
//������:IIC_READ_BYTE		�xһ�ֹ�			//
//��  ��:										//
//��  ��:IIC_READ_DATA							//
//////////////////////////////////////////////////
vu8 IIC_READ_BYTE(void)  
{ 
    vu8 i=8;
    vu8 IIC_READ_DATA=0;				

	for( i=0 ; i<8 ; i++)
	{
		SDA_INPUT;
		SCL_INPUT;
		GCC_DELAY(12) ;
		while(!SCL);
		IIC_READ_DATA <<= 1;
		if(SDA) 
		IIC_READ_DATA |= 0b00000001;
		SCL_LOW;
    	SCL_OUTPUT;
    	GCC_DELAY(10) ;
	}
	return IIC_READ_DATA;
}
//////////////////////////////////////////////////////////////////
//������:IIC_READ_ADDR_REG_MULTI	�x�ęCĳ��ַ���ֹ�			//
//��  ��:Slaveaddress,REG_ADDRESS,MULTI_REG_LEN,*REG_DATA		//
//��  ��:														//
//////////////////////////////////////////////////////////////////


void IIC_READ_ADDR_REG_MULTI()
{
	IIC_START();
	IIC_WRITE_BYTE(0xa0);		//���ęC��ַ
	WAIT_SLAVE_ACK();			//�ȴ��ęC����
	if(WAIT_SLAVE_ACK_F)			
	{
		IIC_STOP();				//�o�����˳�
		return;
	}
	IIC_START();
	IIC_WRITE_BYTE(0xa1);		//���ęC��ַ
	WAIT_SLAVE_ACK();			//�ȴ��ęC����
	if(WAIT_SLAVE_ACK_F)			
	{
		IIC_STOP();				//�o�����˳�
		return;
	}	

	KeyDat = IIC_READ_BYTE();	//�x�������Ĵ���
	MASTER_SEND_ACK();			//���C�l�͑���	
	IIC_READ_BYTE();			//�x�������Ĵ���	
	MASTER_SEND_NOACK();		//����һ�ֹ��x�����C�l�͟o����

	IIC_STOP();	
}
