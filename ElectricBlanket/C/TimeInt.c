#include "base.h"
#define EXTERN extern
#include "Var.h"


/*********************************************************************************************************
** ��������: ISR_tmr2
** ��������: ��ʱ��2�жϷ�����������ˮ�ó�ˮ��
** �䡡��: 	 
** �䡡�� :  ��
** ȫ�ֱ���: ��							  
** ����ģ��: ��
********************************************************************************************************/
void __attribute((interrupt(0x10))) ISR_tmr2 (void)
{
//	_pc0 = !_pc0;
//	_pb4 = !_pb4;	
    _mf1f = 0;
    _t2pf = 0;
    _t1pf = 0;
    //if(mfPumpW==0)
    if(1)
    {
        _t2cp = 0;
        //_tm2al = 0;
        //_tm2ah = 0;
        _pb3 = 0;
    }
    else
    {
//        _tm2al = (unsigned char)pumpPwm;
//        _tm2ah = (unsigned char)(pumpPwm>>8);
        _t2cp = 1;
    }

}

/*********************************************************************************************************
** ��������: ISR_tmr0
** ��������: ��ʱ��0�жϷ�������������Ӧ�ɿع�����ʱ
** �䡡��: 	 
** �䡡�� :  ��
** ȫ�ֱ���: ��							  
** ����ģ��: ��
********************************************************************************************************/
void __attribute((interrupt(0x0c))) ISR_tmr0 (void)
{
	// off the pump pulse if triac is ON
	// on the pump pulse if triac is off before zero cross
	if(IO_HEATTR == 0)
	{											// off the pump if Triac is ON					
		_t0on = false;							// turn off timer 0 to change timer config
		IO_HEATTR = true;

	}						// clr TM0 Comparator A match interrupt req flg.	
		_t0on = false;							// turn off timer 0 to change timer config
		IO_HEATTR = false;
	
}
/*********************************************************************************************************
** ��������: ISR_tmr0
** ��������: ��ʱ��0�жϷ��������ж����ڣ�220us
** �䡡��: 	 
** �䡡�� :  ��
** ȫ�ֱ���: ��							  
** ����ģ��: ��
********************************************************************************************************/
//static _BUZZER	Buzzer __attribute__ ((at(0x1f2)));

void PowerOnTips(void)
{
}
#define INIT_NO_DETECT_TIME	5
unsigned char Time500Cnt = 0,Time1SCnt = 0,Time100Cnt = 0;
void __attribute((interrupt(0x1c))) ISRTime0Base(void)
{
	static u8 TaskADCCnt = 0;
	
	NoZeroTime++;				//�������ʱ��
	TaskADCCnt++;
	if(TaskADCCnt > 25)			//100ms������Ӧ�ȽϺ��ʣ��Թ�200ms���ֹرպ��¶�������
	{
		TaskADCCnt = 0;
		TaskADCRdyFlag = true;
	}
	if((Time100Cnt ++) >= 25)
	{
		Time100Cnt = 0;
		Flag100ms = !Flag100ms;
	}
	
	if(Time500Cnt++ >= 122)		//500ms��ʱ
	{
		Time500Cnt = 0;
		Flag500ms = true;
	//	LED_RED = !LED_RED;
	}

	if(Time1SCnt++ >= 240)		//1S��ʱ
	{
		Time1SCnt = 0;
		Time1MinCnt++;
		Flag1S = true;
		if(Time1MinCnt > INIT_NO_DETECT_TIME)
		{
			FlagStartDetect = true;				
		}
		if(Time1MinCnt >= 60)	//�Ʒ�
		{
			Time1MinCnt = 0;
			Time60Min++;
		}
		if(ModeScntFlag)	//������ʱ
		{
			ModeScnt++;
		}
	}
	if(WorkMode != POWERON_MODE)
		TaskKeyRdy = true;
}


void __attribute((interrupt(0x20))) ISRTime1Base(void)
{

}

/*********************************************************************************************************
** ��������: ISR_int0
** ��������: �ⲿ�ж�0����⵽�����źź�������ʱ2ms
** �䡡��: 	 
** �䡡�� :  ��
** ȫ�ֱ���: ��							  
** ����ģ��: ��
********************************************************************************************************/
const u8 Table_1[]={1,2};		//60HZ	//�͵�	//7.69HZ	

void __attribute((interrupt(0x04))) ISR_int0 (void)
{
    _int0f=0;
/*	if(WaterOutFlag)			//���Գ�ˮ
	{
		IO_HEATTR = true;		//�򿪼̵���
	}
	else						//�ر�
	{
		_t0on = false;							// turn off timer 0 to change timer config		
		IO_HEATTR = true;		//�رտɿع�
	}*/
}