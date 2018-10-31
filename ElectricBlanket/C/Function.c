
#include "base.h"
#define EXTERN extern
#include "Var.h"

void Delay(void)
{
	u8 i;
	for(i=20;i>0;i--);
}
#define HIGH_STA 1
#define LOW_STA 2
u8 IOZeroSta = 0,IOZeroStaBak = 0;

void OutCtrl(void)
{
/*	if(EnThyOutFlag)	//查询控制输出
	{
		if(IO_ZERO)
		{
			Delay();
			if(IO_ZERO)
			{
				IOZeroSta = HIGH_STA;		//高电平
				if(IOZeroSta != IOZeroStaBak)
				{
					IOZeroStaBak = IOZeroSta;
					ZeroHalfFlag = true;
				}				
				HeaterFlag = true;			//此时是正半波，可以检测电流判断是否断开
			}
		}
		else
		{
			IOZeroSta = LOW_STA;
			if(IOZeroSta != IOZeroStaBak)
			{
				IOZeroStaBak = IOZeroSta;
				ZeroHalfFlag = true;
			}
			HeaterFlag = false;			
		}
		if(ZeroHalfFlag)						//输出脉冲控制
		{
			ZeroHalfFlag = false;
			IO_HEATTR = true;
			_t0af = false;_t0ae = 1;			// enable TM0 Comparator A match interrupt control; clr TM0 Comparator A match interrupt req flg.
			_tm0al = 0xfa;		               	// set TM0 CCRA=250; Timer interval = 250/125kHz=2ms
			_tm0ah = 0x00; 	                    
			_t0on = true;						// turn on Timer0 and reset the timer count			
		}
	}
	else
	{
		IO_HEATTR = false;		
		HeaterFlag = false;			
	}*/
}

#define EN_FLICK	0	//不加热闪烁，置1，不加热时不闪烁，清零

void Func(void)
{
	if(ShortFlag)		//PTC保护报警
	{
		if(WorkMode != ALARM_MODE)
		{
			WorkMode = ALARM_MODE;	//进入报警状态
			ErrType = ERR_SHORT;
		}
	}

	if(NoLoadFlag)		//无负载，或负载断开报警
	{
		if(WorkMode != ALARM_MODE)
		{
			WorkMode = ALARM_MODE;	//进入报警状态
			ErrType = ERR_NO_LOAD;
		}
	}

	if(TRShortFlag)
	{
		if(WorkMode != ALARM_MODE)
		{
			WorkMode = ALARM_MODE;	//进入报警状态
			ErrType = ERR_TR_SHORT;
		}		
	}
	
	if(ZeroFlag)
	{
		if(WorkMode != ALARM_MODE)
		{
			WorkMode = ALARM_MODE;	//进入报警状态
			ErrType = ERR_NO_ZERO;
		}		
	}

    switch(WorkMode)
    {
	    case POWERON_MODE:
    		LED_YEL_ON;
    		LED_ORG_ON;
    		LED_RED_ON;
    		if(Flag1S)						//点亮500ms    		
    		{
	    		WorkMode = RDY_MODE;		//进入待机模式    		
	    		LED_YEL_OFF;
	    		LED_ORG_OFF;
	    		LED_RED_OFF;    		
    		}
    		HeaterFlag = false;
	        break;
	    case RDY_MODE:
    		LED_YEL_OFF;
    		LED_ORG_OFF;
    		LED_RED_OFF;    		
	    	HeaterFlag = false;
	    	if(G_Input_Flag[KEY_MODE] == 1)		//加热模式变换
	    	{
	    		G_Input_Flag[KEY_MODE] = 0;
	    		HeartMode = LOW_40C;			//初始加热模式 40度
	    		WorkMode = RUN_MODE;			//关闭输出
	    		Time60Min = 0;					//清除时间累积值	
	    	}
	        break;
	    case RUN_MODE:    
	    	switch(HeartMode)					//加热指示
	    	{
	    		case LOW_40C:
#if EN_FLICK > 0	    		
	    			if((Flag100ms)||(EnThyOutFlag))
	    			{
		    			LED_YEL_ON;
	    			}
	    			else
	    			{
	    				LED_YEL_OFF;
	    			}
#else
	    			LED_YEL_ON;
#endif	    			
		    		LED_ORG_OFF;
		    		LED_RED_OFF;
	    			break;
	    		case LOW_50C:
		    		LED_YEL_OFF;
#if EN_FLICK > 0	    				    		
	    			if((Flag100ms)||(EnThyOutFlag))
	    			{
		    			LED_ORG_ON;
	    			}
	    			else
	    			{
	    				LED_ORG_OFF;
	    			}
#else
		    			LED_ORG_ON;
#endif	    					    				    		
		    		LED_RED_OFF;
	    			break;
	    		case LOW_60C:
		    		LED_YEL_OFF;
		    		LED_ORG_OFF;
#if EN_FLICK > 0	    				    				    		
	    			if((Flag100ms)||(EnThyOutFlag))	//达到温度后快闪
	    			{
		    			LED_RED_ON;
	    			}
	    			else
	    			{
	    				LED_RED_OFF;
	    			}
#else
		    		LED_RED_ON;					
#endif	    				
	    			break;
	    		default:
	    			break;
	    	}
	    	if(G_Input_Flag[KEY_MODE] == 1)			//加热模式变换
	    	{
	    		G_Input_Flag[KEY_MODE] = 0;
	    		HeartMode++;
	    		if(HeartMode > LOW_60C)
	    		{
	    			HeartMode = LOW_40C;
	    		}
	    		Time60Min = 0;
	    	}
	    	OutCtrl();								//控制输出
	    	
	    	if(Time60Min >= WORK_TIME_MAX)			//最大时间限制
	    	{
	    		Time60Min = 0;
	    		WorkMode = RDY_MODE;				//关闭输出
	    	}
	    	if(G_Input_Flag[KEY_MODE] == LONG_KEY)	//长按关机
	    	{
	    		G_Input_Flag[KEY_MODE] = 0;
	    		WorkMode = RDY_MODE;				//关闭输出
	    	}
	    	
	        break;
		case ALARM_MODE:
			IO_HEATTR = false;						//关断可控硅
			if(Flag500ms)
			{
				Flag500ms = false;					//报警500ms闪亮
				if(ErrType == ERR_SHORT)
				{
					LED_RED_REVERSE;
				}
				if(ErrType == ERR_NO_LOAD)
				{
					LED_ORG_REVERSE;
				}
				if(ErrType == ERR_TR_SHORT)
				{
					LED_RED_REVERSE;
					LED_ORG_REVERSE;
					FUSE_OPEN;	//烧保险丝					
				}
				if(ErrType == ERR_NO_ZERO)
				{
					LED_RED_REVERSE;
					LED_ORG_REVERSE;
					LED_YEL_REVERSE;					
				}
			}
	}
}

