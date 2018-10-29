#ifndef __SS350_BASE
#define __SS350_BASE
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "HT66F018.h"

#define u8 unsigned char
#define u16 unsigned int


//***************************************************//
typedef unsigned char 	uchar; 				//8bit
typedef unsigned int	uint16; 			//16bit
typedef unsigned long 	uint32; 			//32bit

#define BitOn(x)	(1<<x)

#define true	1
#define false	0

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define DEBUG_SWITCH
//#define DEBUG_WATER
#define DEBUG_HOT_ON
//#define DEBUG_ZERO_ON
//#define DEBUG_SLEEP   //2015/11/24 11:33
#define DEBUG_TX

//~~~IO~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define PROTECT			_pb2	//保护

#define KEY				_pa3	//按键
#define KEY_PIN			3

#define IO_HEATTR		_pa5	//发热丝控制

#define IO_ZERO			_pc0	//过零检测

#define LED_RED			_pc1	//LED灯
#define LED_RED_ON		LED_RED = false
#define LED_RED_OFF		LED_RED = true
#define LED_RED_REVERSE	LED_RED = !LED_RED

#define LED_ORG			_pc2
#define LED_ORG_ON		LED_ORG = false
#define LED_ORG_OFF		LED_ORG = true
#define LED_ORG_REVERSE	LED_ORG = !LED_ORG

#define LED_YEL			_pa1
#define LED_YEL_ON		LED_YEL = false
#define LED_YEL_OFF		LED_YEL = true
#define LED_YEL_REVERSE	LED_YEL = !LED_YEL

#define ADC_PTC1        0x10	//PTC值
#define ADC_CURRENT	 	0x11	//保护电流值

#define LOW_40C         0
#define LOW_50C         1
#define LOW_60C         2

#define CONFIG_IN_NUM	0x01
#define KEY_MODE		0

#define WORK_TIME_MAX	60

#define POWERON_MODE    0
#define RDY_MODE        1
#define RUN_MODE        2
#define ALARM_MODE		3

#define LONG_KEY       	10


//#define PWM_MAX         6198
//#define PWM_MIN         4200
#define PWM_MAX         8000
#define PWM_MIN         2048
#define PWM_MID         5200
#define AdjA        22  //22  //55 //65     //5  20
#define AdjB        27 // 28  //75 //75    //3  16
#define AdjC        8  // 8   //25 //15      //0  2

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define STAR_ADC()    {_start=0; _start=1; _start=0;}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef struct {
	unsigned char b0 : 1;
	unsigned char b1 : 1;
	unsigned char b2 : 1;
	unsigned char b3 : 1;
	unsigned char b4 : 1;
	unsigned char b5 : 1;
	unsigned char b6 : 1;
	unsigned char b7 : 1;
} _t_bits;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~main.c~~~~~~~~~~~~~~~~~~~~
extern _t_bits maiFlagA;
#define PowerDnFlag 	maiFlagA.b0		//掉电标志
#define	ShortFlag       maiFlagA.b1		//短路标志
#define NoLoadFlag      maiFlagA.b2		//无负载检测
#define Flag500ms		maiFlagA.b3
#define EnThyOutFlag	maiFlagA.b4		//允许输出
#define HeaterFlag		maiFlagA.b5		//加热标志，用于检测是否有负载
#define NoZeroFlag		maiFlagA.b6		//无过零信号报警
#define FlagStartDetect maiFlagA.b7

extern _t_bits maiFlagB;
#define Flag1S	      maiFlagB.b0
#define TaskADCRdyFlag      maiFlagB.b1
#define mf500ms       maiFlagB.b3
#define Flag100ms  		maiFlagB.b4
#define mfPumpG       maiFlagB.b5
#define mfWaterErroL  maiFlagB.b6
#define mfSelfCheck   maiFlagB.b7

extern _t_bits LEDG;
#define LEDBTemp	  LEDG.b0
#define LEDBWaterML   LEDG.b1
#define LEDBWaterOut  LEDG.b2
#define LEDBLockRed   LEDG.b3
#define LEDBLOCKBlue  LEDG.b4
#define TaskKeyRdy	  LEDG.b5

extern _t_bits LEDH;
#define LEDBUV       	LEDH.b0
#define LEDBOC      	LEDH.b1
#define LEDBML     		LEDH.b2
#define LEDBGyuan    	LEDH.b3
#define ModeScntFlag  	LEDH.b4
#define ZeroHalfFlag	LEDH.b5
#define WaterOutFlag	LEDH.b6
#define TrInFlag		LEDH.b7

#define MASK_LEDTEMPON		0x01
#define MASK_LEDTEMPOFF		0xFE

#define MASK_LEDWATERMLON	0x02
#define MASK_LEDWATERMLOFF	0xFD

#define MASK_LEDLOCKBLUEON	0x08 
#define MASK_LEDLOCKBLUEOFF	0xF7 

#define MASK_LEDLOCKREDON	0x04
#define MASK_LEDLOCKREDOFF	0xFb

#define MASK_LEDWATEROUTON	0x10
#define MASK_LEDWATEROUTOFF	0xEF



#define MASK_LEDBUVON		0x01
#define MASK_LEDBUVOFF		0xFE

#define MASK_LEDBOCON		0x02
#define MASK_LEDBOCOFF		0xFD

#define MASK_LEDBPYON		0x08
#define MASK_LEDBPYOFF		0xF7

#define MASK_LEDBGYON		0x04
#define MASK_LEDBGYOFF		0xFb

#define MASK_LEDBMLON		0x10
#define MASK_LEDBMLOFF		0xEF

#define UV_TIME				60	//单位：秒


#define ERR_NO_ZERO			1
#define ERR_SHORT			2
#define ERR_NO_LOAD			3


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern void InitSys(void);
extern void InitRam(void);
extern void GetAdcDat(void);
extern void PumpAdj(void);
extern void GetSwitch(void);
extern void GetKey(void);
extern void Func(void);
extern void LedOut(void);
extern void WaterCtl(void);
extern void GetTemp(unsigned short dat);
extern unsigned char InitEepRom(void);
extern void SelfDeal(void);
extern void TM1638Init(void);
extern void LedDisplay(void);

extern void i2cStart(void);
extern void i2cStop(void);
extern void Ack(void);
extern void NoAck(void);
extern u8 TestAck(void);

extern void i2cWrite(uchar Data);
extern uchar i2cRead(void);
extern void DigitalDisp(void);
extern void LEDTips(void);
extern void DigitalErrorDisp(u8 Index);
void PowerOnTips(void);
extern void WriteEepRom(void);
extern void KeyBuzzerLong(void);
extern void KeyBuzzer(u8 Times);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#endif //__SS350_BASE
