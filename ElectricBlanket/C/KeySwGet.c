#include "base.h"
#define EXTERN extern
#include "Var.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
volatile static unsigned char old_in_status[4]  __attribute__ ((at(0x1ee)));
volatile static unsigned char in_count[4]  __attribute__ ((at(0x1f2)));


void GetKey(void)
{
    u8 i;
    u8 new_status;
	static u8 LongTimeCnt __attribute__ ((at(0x1dc)));

    for(i = 0; i < CONFIG_IN_NUM; i++)
    {
    	if(i == 0)
        	new_status = KEY;

        if(old_in_status[i] != new_status)
            in_count[i] ++;
        else
            in_count[i] = 0;

        if(in_count[i] > 20)
        {
            if(new_status == 0)
            {
                G_Input_Flag[i] = 1;
            }
            else
            {
                G_Input_Flag[i] = 2;
            }
        	old_in_status[i] = new_status;
        }        
    }
    if(old_in_status[KEY_MODE] == 0)
    {
    	LongTimeCnt++;
    	if(LongTimeCnt >= 250)
    	{
    		G_Input_Flag[KEY_MODE] = LONG_KEY;
    	}
    }
    else
    {
    	LongTimeCnt = 0;
    }
}


void GetSwitch(void)
{
	static unsigned char swCntA = 0;
	if(FlagStartDetect)	//检测保护电平需要在上电几秒稳定后
	{
	    if(PROTECT)
	    {
	        if(swCntA < 200) 
	        	swCntA++;
	        else 
	        	ShortFlag = true;
	    }
	    else
	    {
	        if(swCntA>0) 
	        	swCntA--;
	        else 
	        	ShortFlag = false;
	    }
	}
}
