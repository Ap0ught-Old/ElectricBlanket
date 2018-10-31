#include "base.h"
#define EXTERN extern
#include "Var.h"

typedef struct
{
	u16 Min;
	u16 Max;
}__TEMP;

#define PVC_PTC			1		//0 PVC 1 PTC

#define MIN_VALU		500		//��Сֵ�����������·���ܵ��·Ŵ��������,С��0.4V������˵��358����

#define BASE_40C		1391	//37
#define MID_40C			1502
#define HIGH_40C		1468	//41


#define BASE_50C		1584	//47
#define MID_50C			1642
#define HIGH_50C		1661	//51

#define BASE_60C		1793	//58
#define MID_60C			1830
#define HIGH_60C		1923	//65

#define SHORT_VALU		100

unsigned short k;

 __TEMP TempArray[3] = 
{
	{BASE_40C,HIGH_40C},
	{BASE_50C,HIGH_50C},
	{BASE_60C,HIGH_60C}
};


void GetPTCValu(void)
{
	unsigned char i;
	unsigned short min,max;
	volatile static unsigned short adcBuf[10] __attribute__ ((at(0x1c0)));

	_adcr0 = ADC_PTC1;			//�л�PTC������
	STAR_ADC();
	while(_eocb);
    
    k = _adrh;
    k <<= 8;
    k += _adrl;
    if(adcsp < 10) 
    	i = adcsp;
    else  
    	i = adcsp - 10;
    adcBuf[i] = k;
    if(adcsp < 11) 				//�ɼ�10������ 
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
        	min = adcBuf[i];
        if(max < adcBuf[i])  
        	max = adcBuf[i];
        k += adcBuf[i]; 
    }
    k -= min; k -= max; k >>= 3;			//����10�Σ�ȥ�����ֵ����Сֵ��Ȼ��ƽ��
    
	if(k > TempArray[HeartMode].Max)		//�������ֵ��
	{
		EnThyOutFlag = false;
	}
	if(k < TempArray[HeartMode].Min)		//С����Сֵ��
	{
		EnThyOutFlag = true;	
	}
	if((k < MIN_VALU)&&(FlagStartDetect == true))	//�͵�ѹ����������LM358���ˣ�����ʱAD��׼����ʱһ��ʱ��
	{
		ShortFlag = true;
	}    
    
}

void GetLoadValu(void)
{
	unsigned char i;
	unsigned short min,max;
	volatile static unsigned short adc1Buf[10] __attribute__ ((at(0x1d5)));
	
	_adcr0 = ADC_CURRENT;	//�л����ش�����
	STAR_ADC();
	while(_eocb);	
    k = _adrh;
    k <<= 8;
    k += _adrl;
    if(AdcCnt < 10) 
    	i = AdcCnt;
    else  
    	i = AdcCnt - 10;
    adc1Buf[i] = k;
    if(AdcCnt < 11) 	//�ɼ�10������ 
    {
    	AdcCnt++; 
    	return;
    }
    AdcCnt = 0;
    k = adc1Buf[0];
    min = k;
    max = k;
    for(i=1;i<10;i++)
    {
        if(min > adc1Buf[i])  
        	min = adc1Buf[i];
        if(max < adc1Buf[i])  
        	max=adc1Buf[i];
        k += adc1Buf[i]; 
    }
    k -= min; k -= max; k >>= 3;			//����10�Σ�ȥ�����ֵ����Сֵ��Ȼ��ƽ��

    if(FlagStartDetect)						//�ϵ�X�벻���ж�
    {
    	if(HeaterFlag)						//����״̬�����źţ��жϸ��ضϿ�
    	{
	        if(k > SHORT_VALU)				//�и��أ�0.5V
	        {
	        	ShortCnt1 = 0;
	        	NoLoadFlag = false;	
	        }
	        else
	        {
	        	ShortCnt1++;
	        	if(ShortCnt1 >= 200)		//һ�����������ź��ж�Ϊ�޸���
	        	{
	        		NoLoadFlag = true;
	        	}
	        }
    	}
    	else								//û�м���ʱ������ź�˵���ɿع��·��
    	{
	        if(k < SHORT_VALU)				//û�в���				
	        {
	        	ShortCnt2 = 0;
	        	TRShortFlag = false;	
	        }
	        else
	        {
	        	ShortCnt2++;
	        	if(ShortCnt2 >= 10)			//һ�����������ź��ж�Ϊ�޸���
	        	{
	        		TRShortFlag = true;
	        	}
	        }
    		
    	}
    }	
}
/*
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
    if(adcsp < 11) 	//�ɼ�10������ 
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
    k -= min; k -= max; k >>= 3;			//����10�Σ�ȥ�����ֵ����Сֵ��Ȼ��ƽ��

    adcsele++;
    if(adcsele & 0x01)     					//ADC_HOT
    {
        _adcr0 = ADC_PTC1;					//�л�PTC������
        STAR_ADC();
        if(k > TempArray[HeartMode].Max)	//�������ֵ��
        {
        	EnThyOutFlag = false;
        }
        if(k < TempArray[HeartMode].Min)	//С����Сֵ��
        {
        	EnThyOutFlag = true;	
        }
		if((k < MIN_VALU)&&(FlagStartDetect == true))	//�͵�ѹ����������LM358���ˣ�����ʱAD��׼����ʱһ��ʱ��
		{
			ShortFlag = true;
		}
		
    }
    else //ADC_WATER
    {
        _adcr0=ADC_CURRENT;				//�л�����������ͨ��
        STAR_ADC();
        if((HeaterFlag)&&(FlagStartDetect))					//������ȸߵ�ƽʱ���ж�
        {
	        if(k > SHORT_VALU)			//�и���
	        {
	        	ShortCnt = 0;
	        	NoLoadFlag = false;	
	        }
	        else
	        {
	        	ShortCnt++;
	        	if(ShortCnt >= 2000)		//һ�����������ź��ж�Ϊ�޸���
	        	{
	        		//NoLoadFlag = true;
	        		NoLoadFlag = false;
	        	}
	        }
        }
    }
}*/

