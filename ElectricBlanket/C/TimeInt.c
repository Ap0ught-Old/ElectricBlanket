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
	TaskChkLoadRdy = true;
	_t2on = false;							// turn off timer 0 to change timer config		

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
	if(IO_HEATTR)
	{											// off the pump if Triac is ON					
		_t0on = false;							// turn off timer 0 to change timer config
		IO_HEATTR = false;
		_tm0al=0x13;		               		// set TM0 CCRA=875; Timer interval = 875/125kHz=7ms
		_tm0ah=0x03; 	                    
		_t0on = true;							// turn on Timer0 and reset the timer count
	}
	else if (!ZeroHalfFlag)
	{											// on the pump pulse before next zero cross
		_t0on = false;							// turn off timer 0 to change timer config
		IO_HEATTR = true;
		ZeroHalfFlag = true;					// 
		_tm0al=0xfa;		                	// set TM0 CCRA=250; Timer interval = 250/125kHz=2ms
		_tm0ah=0x00; 	                    
		_t0on = true;							// turn on Timer0 and reset the timer count
	}
	_t0af=0;									// clr TM0 Comparator A match interrupt req flg.		
}
/*********************************************************************************************************
** ��������: ISR_tmr0
** ��������: ��ʱ��0�жϷ��������ж����ڣ�220us
** �䡡��: 	 
** �䡡�� :  ��
** ȫ�ֱ���: ��							  
** ����ģ��: ��
********************************************************************************************************/

#define INIT_NO_DETECT_TIME	3
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
		NoZeroTime++;
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
    NoZeroTime = 0;
    ZeroFlag  = false;
	if(EnThyOutFlag && (WorkMode == RUN_MODE))			//����ģʽ���Լ���
	{
		IO_HEATTR = true;					//�򿪿ɿع�
		//**setup value of timer 0 int for off pulse 
		_t0on = false;						// turn off timer 0 to change timer config
		_t0af = false;_t0ae = 1;			// enable TM0 Comparator A match interrupt control; clr TM0 Comparator A match interrupt req flg.
		_tm0al = 0xfa;		               	// set TM0 CCRA=250; Timer interval = 250/125kHz=2ms
		_tm0ah = 0x00; 	                    
		_t0on = true;						// turn on Timer0 and reset the timer count
		
		
				
		ZeroHalfFlag = false;
		HeaterFlag = true;			//��ʱ�����벨�����Լ������ж��Ƿ�Ͽ�
	}
	else						//�ر�
	{
		_t0on = false;							// turn off timer 0 to change timer config		
		IO_HEATTR = false;		//�رտɿع�
		HeaterFlag = false;		//��ʱ�����벨�����Լ������ж��Ƿ�Ͽ�
	}  
	
	//���㲨�嵽��ʱ�䴥������
	_t2on = false;						// turn off timer 0 to change timer config
	_t2af = false;_t2ae = 1;			// enable TM0 Comparator A match interrupt control; clr TM0 Comparator A match interrupt req flg.
	_tm2al = 0x06;		               	// set TM0 CCRA=0x0206; Timer interval = 0x0206/125kHz=4.3ms
	_tm2ah = 0x02; 	                    
	_t2on = true;						// turn on Timer0 and reset the timer count
	  
}