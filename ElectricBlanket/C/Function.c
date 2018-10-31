
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
/*	if(EnThyOutFlag)	//��ѯ�������
	{
		if(IO_ZERO)
		{
			Delay();
			if(IO_ZERO)
			{
				IOZeroSta = HIGH_STA;		//�ߵ�ƽ
				if(IOZeroSta != IOZeroStaBak)
				{
					IOZeroStaBak = IOZeroSta;
					ZeroHalfFlag = true;
				}				
				HeaterFlag = true;			//��ʱ�����벨�����Լ������ж��Ƿ�Ͽ�
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
		if(ZeroHalfFlag)						//����������
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

#define EN_FLICK	0	//��������˸����1��������ʱ����˸������

void Func(void)
{
	if(ShortFlag)		//PTC��������
	{
		if(WorkMode != ALARM_MODE)
		{
			WorkMode = ALARM_MODE;	//���뱨��״̬
			ErrType = ERR_SHORT;
		}
	}

	if(NoLoadFlag)		//�޸��أ����ضϿ�����
	{
		if(WorkMode != ALARM_MODE)
		{
			WorkMode = ALARM_MODE;	//���뱨��״̬
			ErrType = ERR_NO_LOAD;
		}
	}

	if(TRShortFlag)
	{
		if(WorkMode != ALARM_MODE)
		{
			WorkMode = ALARM_MODE;	//���뱨��״̬
			ErrType = ERR_TR_SHORT;
		}		
	}
	
	if(ZeroFlag)
	{
		if(WorkMode != ALARM_MODE)
		{
			WorkMode = ALARM_MODE;	//���뱨��״̬
			ErrType = ERR_NO_ZERO;
		}		
	}

    switch(WorkMode)
    {
	    case POWERON_MODE:
    		LED_YEL_ON;
    		LED_ORG_ON;
    		LED_RED_ON;
    		if(Flag1S)						//����500ms    		
    		{
	    		WorkMode = RDY_MODE;		//�������ģʽ    		
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
	    	if(G_Input_Flag[KEY_MODE] == 1)		//����ģʽ�任
	    	{
	    		G_Input_Flag[KEY_MODE] = 0;
	    		HeartMode = LOW_40C;			//��ʼ����ģʽ 40��
	    		WorkMode = RUN_MODE;			//�ر����
	    		Time60Min = 0;					//���ʱ���ۻ�ֵ	
	    	}
	        break;
	    case RUN_MODE:    
	    	switch(HeartMode)					//����ָʾ
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
	    			if((Flag100ms)||(EnThyOutFlag))	//�ﵽ�¶Ⱥ����
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
	    	if(G_Input_Flag[KEY_MODE] == 1)			//����ģʽ�任
	    	{
	    		G_Input_Flag[KEY_MODE] = 0;
	    		HeartMode++;
	    		if(HeartMode > LOW_60C)
	    		{
	    			HeartMode = LOW_40C;
	    		}
	    		Time60Min = 0;
	    	}
	    	OutCtrl();								//�������
	    	
	    	if(Time60Min >= WORK_TIME_MAX)			//���ʱ������
	    	{
	    		Time60Min = 0;
	    		WorkMode = RDY_MODE;				//�ر����
	    	}
	    	if(G_Input_Flag[KEY_MODE] == LONG_KEY)	//�����ػ�
	    	{
	    		G_Input_Flag[KEY_MODE] = 0;
	    		WorkMode = RDY_MODE;				//�ر����
	    	}
	    	
	        break;
		case ALARM_MODE:
			IO_HEATTR = false;						//�ضϿɿع�
			if(Flag500ms)
			{
				Flag500ms = false;					//����500ms����
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
					FUSE_OPEN;	//�ձ���˿					
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

