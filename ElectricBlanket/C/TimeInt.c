#include "base.h"
#define EXTERN extern
#include "Var.h"


/*********************************************************************************************************
** 函数名称: ISR_tmr2
** 功能描述: 定时器2中断服务函数，控制水泵出水量
** 输　入: 	 
** 输　出 :  无
** 全局变量: 无							  
** 调用模块: 无
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
** 函数名称: ISR_tmr0
** 功能描述: 定时器0中断服务函数，用于响应可控硅过零计时
** 输　入: 	 
** 输　出 :  无
** 全局变量: 无							  
** 调用模块: 无
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
** 函数名称: ISR_tmr0
** 功能描述: 定时器0中断服务函数，中断周期：220us
** 输　入: 	 
** 输　出 :  无
** 全局变量: 无							  
** 调用模块: 无
********************************************************************************************************/

#define INIT_NO_DETECT_TIME	3
unsigned char Time500Cnt = 0,Time1SCnt = 0,Time100Cnt = 0;
void __attribute((interrupt(0x1c))) ISRTime0Base(void)
{
	static u8 TaskADCCnt = 0;
	
	NoZeroTime++;				//过零计数时间
	TaskADCCnt++;
	if(TaskADCCnt > 25)			//100ms采样响应比较合适，试过200ms发现关闭后温度上升快
	{
		TaskADCCnt = 0;
		TaskADCRdyFlag = true;
	}
	if((Time100Cnt ++) >= 25)
	{
		Time100Cnt = 0;
		Flag100ms = !Flag100ms;
	}
	
	if(Time500Cnt++ >= 122)		//500ms计时
	{
		Time500Cnt = 0;
		Flag500ms = true;
	//	LED_RED = !LED_RED;
	}

	if(Time1SCnt++ >= 240)		//1S计时
	{
		Time1SCnt = 0;
		Time1MinCnt++;
		NoZeroTime++;
		Flag1S = true;
		if(Time1MinCnt > INIT_NO_DETECT_TIME)
		{
			FlagStartDetect = true;				
		}
		if(Time1MinCnt >= 60)	//计分
		{
			Time1MinCnt = 0;
			Time60Min++;
		}
		if(ModeScntFlag)	//长按计时
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
** 函数名称: ISR_int0
** 功能描述: 外部中断0，检测到过零信号后，启动定时2ms
** 输　入: 	 
** 输　出 :  无
** 全局变量: 无							  
** 调用模块: 无
********************************************************************************************************/
const u8 Table_1[]={1,2};		//60HZ	//低档	//7.69HZ	

void __attribute((interrupt(0x04))) ISR_int0 (void)
{
    _int0f=0;
    NoZeroTime = 0;
    ZeroFlag  = false;
	if(EnThyOutFlag && (WorkMode == RUN_MODE))			//工作模式可以加热
	{
		IO_HEATTR = true;					//打开可控硅
		//**setup value of timer 0 int for off pulse 
		_t0on = false;						// turn off timer 0 to change timer config
		_t0af = false;_t0ae = 1;			// enable TM0 Comparator A match interrupt control; clr TM0 Comparator A match interrupt req flg.
		_tm0al = 0xfa;		               	// set TM0 CCRA=250; Timer interval = 250/125kHz=2ms
		_tm0ah = 0x00; 	                    
		_t0on = true;						// turn on Timer0 and reset the timer count
		
		
				
		ZeroHalfFlag = false;
		HeaterFlag = true;			//此时是正半波，可以检测电流判断是否断开
	}
	else						//关闭
	{
		_t0on = false;							// turn off timer 0 to change timer config		
		IO_HEATTR = false;		//关闭可控硅
		HeaterFlag = false;		//此时是正半波，可以检测电流判断是否断开
	}  
	
	//计算波峰到达时间触发采样
	_t2on = false;						// turn off timer 0 to change timer config
	_t2af = false;_t2ae = 1;			// enable TM0 Comparator A match interrupt control; clr TM0 Comparator A match interrupt req flg.
	_tm2al = 0x06;		               	// set TM0 CCRA=0x0206; Timer interval = 0x0206/125kHz=4.3ms
	_tm2ah = 0x02; 	                    
	_t2on = true;						// turn on Timer0 and reset the timer count
	  
}