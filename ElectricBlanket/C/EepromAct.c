#include "base.h"
#define EXTERN extern
#include "Var.h"
unsigned char InitEepRom(void)
{
/*	unsigned char i,j;
    _eea = 0x11;
    _mp1 = 0x40;
    _bp = 1;
    asm("set %0.1":"=m"(_iar1));
    asm("set %0.0":"=m"(_iar1));
    while(_iar1&0x01) ;
    _iar1 = 0;
    _bp = 0;
    WaterTmpIndex = _eed;
	i = WaterTmpIndex;
	if(WaterTmpIndex > 4)
		return 1;

    _eea = 0x12;
    _mp1 = 0x40;
    _bp = 1;
    asm("set %0.1":"=m"(_iar1));
    asm("set %0.0":"=m"(_iar1));
    while(_iar1&0x01) ;
    _iar1 = 0;
    _bp = 0;
    WaterMlIndex = _eed;
    i ^= WaterMlIndex;
	if(WaterMlIndex > 4)
		return 1;
	
    _eea = 0x13;
    _mp1 = 0x40;
    _bp = 1;
    asm("set %0.1":"=m"(_iar1));
    asm("set %0.0":"=m"(_iar1));
    while(_iar1 & 0x01) ;
    _iar1 = 0;
    _bp = 0;
 	i ^= _eed;	
	if(_eed > 1)
		return 1;
    if(_eed == 0)  
    	LEDBGyuan = false;
    else  
    	LEDBGyuan = true;

    _eea = 0x14;
    _mp1 = 0x40;
    _bp = 1;
    asm("set %0.1":"=m"(_iar1));
    asm("set %0.0":"=m"(_iar1));
    while(_iar1 & 0x01) ;
    _iar1 = 0;
    _bp = 0;
    if(_eed != i)  
    	return 1;
    else
    	return 0;*/
	return 0;    	
}

void WriteEepRom(void)
{
/*	unsigned char i,m,n;

    for(i=0;i<3;i++)
    {
        _eea = 0x11;			//保存水温
        m = WaterTmpIndex;
        _eed = m;
        _mp1 = 0x40;
        _bp = 1;
        _emi = 0;
        asm("set %0.3":"=m"(_iar1));
        asm("set %0.2":"=m"(_iar1));
        _emi = 1;
        while(_iar1 & 0x04) ;
        _iar1 = 0;
        _bp = 0;

        _eea = 0x12;		//保存水量
        n = WaterMlIndex;
        m ^= n;
        _eed = n;
        _mp1 = 0x40;
        _bp = 1;
        _emi = 0;
        asm("set %0.3":"=m"(_iar1));
        asm("set %0.2":"=m"(_iar1));
        _emi = 1;
        while(_iar1 & 0x04) ;
        _iar1 = 0;
        _bp = 0;
    
        _eea = 0x13;		//保存模式
        n = (u8)LEDBGyuan;
        m ^= n;
        _eed = n;
        _mp1 = 0x40;
        _bp = 1;
        _emi = 0;
        asm("set %0.3":"=m"(_iar1));
        asm("set %0.2":"=m"(_iar1));
        _emi = 1;
        while(_iar1 & 0x04) ;
        _iar1 = 0;
        _bp = 0;        
    
        _eea=0x14;			//保存校验值
        _eed=m;
        _mp1=0x40;
        _bp=1;
        _emi=0;
        asm("set %0.3":"=m"(_iar1));
        asm("set %0.2":"=m"(_iar1));
        _emi=1;
        while(_iar1 & 0x04) ;
        _iar1=0;
        _bp=0;
        */
/*
        _eea=0x11;
        _mp1=0x40;
        _bp=1;
        asm("set %0.1":"=m"(_iar1));
        asm("set %0.0":"=m"(_iar1));
        while(_iar1 & 0x01) ;
        _iar1 = 0;
        _bp = 0;
        k = _eed;
        k <<= 8;
    
        _eea=0x12;
        _mp1=0x40;
        _bp=1;
        asm("set %0.1":"=m"(_iar1));
        asm("set %0.0":"=m"(_iar1));
        while(_iar1&0x01) ;
        _iar1=0;
        _bp=0;
        k+=_eed;
        
        _eea=0x13;
        _mp1=0x40;
        _bp=1;
        asm("set %0.1":"=m"(_iar1));
        asm("set %0.0":"=m"(_iar1));
        while(_iar1&0x01) ;
        _iar1=0;
        _bp=0;
        k+=_eed;            

 //       if(cleanCnt == k) 
 //       	break;
//        cleanCnt=k;
    }*/
}

void PowerDownFunc(void)
{
/*    if(NoZeroTime > 12)	//After 50 ms ,enter power down mode 
    {
		if(!PowerDnFlag) 
		{
		    PowerDnFlag = true;
			WriteEepRom();
			//保存水温 水量 模式
		}
    }
    else
    {
     	if(PowerDnFlag)   
		{
            if(NoZeroTime  < 5)	//过零信号恢复 
            	NoZeroTime = false;  	
		}
    }*/
}
