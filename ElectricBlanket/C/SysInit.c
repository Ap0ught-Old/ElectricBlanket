#include "base.h"
#define EXTERN extern
#include "Var.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InitSys(void)
{
//	unsigned char i;
// System clock setting
	_smod = 0b11100001;					// fsys=fH=8MHz; IDLE mode Disable
		// Watchdog Timer Setting  
	_wdtc = 0b01010101;					// set WDT Timeout period= 2^16/fsub=2^16/32.768kHz=2sec; Enable WDT

    _lvrc = 0b00110011; //2.55V
    _cpc = 0x08;
//io
    _pa = 0x00;
    _pac = 0x19;
    _papu = 0x7E; 
    FUSE_CLOSE;
    _pb = 0x10;
    _pbc  = 0b00000111;
    _pbpu = 0b11111000;	//检测过零信号不能中断

    _pc    = 0x07;
    _pcc   = 0x01;
    _pcpu  = 0x07;
	IO_HEATTR = false;						//关断可控硅    
  
//adc init
    _acerl = 0b00001010;
    _adcr0 = ADC_PTC1;
    _adcr1 = 0x06;
    STAR_ADC();
//time2 init
    _t2cp = 1;
    _tm2c0 = 0b00110000;
    _tm2c1 = 0b10101000;
    _tm2al = 0;
    _tm2ah = 0;
    _tm2rp = 0b00100000;   //0~8192 682us tp2 pwm
    _t2on=1;
    _mf1e=1;
    _t2pe=1;
//time1 init
    _t1cp = 0;
    _tm1c0 = 0b00010000;
    _tm1c1 = 0b10101000;
    _tm1al = 0;
    _tm1ah = 0;
    _tm1rpl = 0xff;
    _tm1rph = 0x03;
    _t1on=1;
    //_mf1e=0;
    //_t1pe=1;
#ifdef DEBUG_SWITCH
//time0 init 208us  4800
    _tm0al=0xc0;
    _tm0ah=0x09;
#else
//time0  500us
    _tm0al=0x70;
    _tm0ah=0x17;
#endif
	// ***Timer0 Setting***
	_tm0c0=0b00110000;                  // TM0 counter clock=fH/64=8MHz/64=125kHz; TM0 counter Off
	_tm0c1=0b11000001;                  // TM0 in timer mode; 
	
	_tm0al=0xfa;		                // set TM0 CCRA=250; Timer interval = 250/125kHz=2ms
	_tm0ah=0x00; 	                    
	_mf0e=1;							// Enable multi-function 0 for Timer0 interrupt
//int
// Ext Int 1 Setting
	_int0s0 = 1;_int0s1 = 0;				// Set rising edge interrupt for INT1
    //_integ=0x03;
    _int0e = true;  
	// ***Time Base0&1 Setting***
	_tbc=0b11110101;					// enable Time Base, fTB=fsys/4=2MHz, select TB0 Time-out period=8192/fTB=4.096ms; 
										// select TB1 Time-out period=32768/fTB=1sec; 
	_tb0e=1;							// enable Time Base0 interrupt control   
    _emi = 1;    
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void InitRam(void)
{
    adcsp=0;
    adcsele=0;

    WorkMode = POWERON_MODE;	
    HeartMode = LOW_40C; 				//初始加热模式：
	G_Input_Flag[KEY_MODE] = 0;
	Flag500ms = false;
	Flag1S = false;
	Time60Min = 0;
	FlagStartDetect = false;
	TaskKeyRdy = false;
	HeaterFlag = false;
	Time1MinCnt = 0;
	EnThyOutFlag = false;
	TRShortFlag = false;
	ShortFlag = false;
	NoLoadFlag = false;
	ZeroFlag = false;
	
	ShortCnt1 = 0;
	ShortCnt2 = 0;
	AdcCnt = 0;
	NoZeroTime = 0;
}
