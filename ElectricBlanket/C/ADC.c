#include "base.h"
#define EXTERN extern
#include "Var.h"

typedef struct
{
	u16 Min;
	u16 Max;
}__TEMP;

#define PVC_PTC			1		//0 PVC 1 PTC

#define MIN_VALU		500		//最小值保护，如果短路可能导致放大器输出底,小于0.4V保护，说明358坏了

#define BASE_40C		1391	//37
#define MID_40C			1502
#define HIGH_40C		1468	//41


#define BASE_50C		1584	//47
#define MID_50C			1642
#define HIGH_50C		1661	//51

#define BASE_60C		1793	//58
#define MID_60C			1830
#define HIGH_60C		1923	//65

#define SHORT_VALU		30

unsigned short k;

 __TEMP TempArray[3] = 
{
	{BASE_40C,HIGH_40C},
	{BASE_50C,HIGH_50C},
	{BASE_60C,HIGH_60C}
};

void GetAdcDat(void)
{
	static u16 ShortCnt = 0;
	unsigned char i;
	unsigned short min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1c0)));

    if(_eocb)  
    	return;

    k = _adrh;
    k <<= 8;
    k += _adrl;
    if(adcsp < 10) 
    	i = adcsp;
    else  
    	i = adcsp-10;
    adcBuf[i] = k;
    if(adcsp < 11) 	//采集10次数据 
    {
    	adcsp++; 
    	return;
    }
    adcsp = 0;
    k = adcBuf[0];
    min = k;
    max = k;
    for(i=1;i<10;i++)
    {
        if(min > adcBuf[i])  
        	min=adcBuf[i];
        if(max < adcBuf[i])  
        	max=adcBuf[i];
        k += adcBuf[i]; 
    }
    k -= min; k -= max; k >>= 3;			//采样10次，去掉最大值和最小值，然后平均

    adcsele++;
    if(adcsele & 0x01)     					//ADC_HOT
    {
        _adcr0 = ADC_PTC1;					//切换PTC传感器
        STAR_ADC();
        if(k > TempArray[HeartMode].Max)	//大于最大值关
        {
        	EnThyOutFlag = false;
        }
        if(k < TempArray[HeartMode].Min)	//小于最小值开
        {
        	EnThyOutFlag = true;	
        }
		if((k < MIN_VALU)&&(FlagStartDetect == true))	//低电压保护，可能LM358坏了，开机时AD不准，延时一段时间
		{
			ShortFlag = true;
		}
		
    }
    else //ADC_WATER
    {
        _adcr0=ADC_CURRENT;				//切换电流传感器通道
        STAR_ADC();
        if((HeaterFlag)&&(FlagStartDetect))					//输出加热高电平时做判断
        {
	        if(k > SHORT_VALU)			//有负载
	        {
	        	ShortCnt = 0;
	        	NoLoadFlag = false;	
	        }
	        else
	        {
	        	ShortCnt++;
	        	if(ShortCnt >= 2000)		//一定次数的无信号判定为无负载
	        	{
	        		//NoLoadFlag = true;
	        		NoLoadFlag = false;
	        	}
	        }
        }
    }
}

