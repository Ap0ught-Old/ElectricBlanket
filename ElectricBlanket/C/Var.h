#ifndef __VAR_H
#define __VAR_H
EXTERN _t_bits maiFlagA;
EXTERN _t_bits maiFlagB;

EXTERN _t_bits LEDG;
EXTERN _t_bits LEDH;

//sys
EXTERN u16 sysTime;
EXTERN unsigned char Time60Min;

EXTERN unsigned char adcsp;
EXTERN unsigned char adcsele;
EXTERN unsigned char Time1MinCnt;
EXTERN unsigned char G_Input_Flag[CONFIG_IN_NUM];	//�������ؼ�ֵ

EXTERN unsigned char WorkMode;		//����ģʽ

EXTERN u8 HeartMode;
EXTERN u8 ErrType;
EXTERN unsigned char adjDiv;

EXTERN unsigned char hotDelay;
EXTERN unsigned char hotTmp;
EXTERN unsigned short clrTime;

EXTERN unsigned char delayWate;

EXTERN unsigned char ntcErroL;
EXTERN unsigned char ntcErroH;
EXTERN unsigned char runErroCnt;
EXTERN unsigned char adjStep;
EXTERN u8 ModeScnt;
EXTERN u8 NoZeroTime;				//�޹����ź�ʱ��

EXTERN unsigned char txBuf;
EXTERN unsigned char txSp;
EXTERN unsigned char txBit;


#endif