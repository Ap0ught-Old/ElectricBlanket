#include "base.h"
#define EXTERN extern
#include "Var.h"


#define	DATA_COMMAND	0X40
#define	DISP_COMMAND	0x80
#define	ADDR_COMMAND	0XC0

//�����������ʾ����
//const unsigned char tab[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
//                           0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00,0x40};

const unsigned char tab[]={0x5F,0x12,0x2f,0x37,0x72,0x75,0x7D,0x13,
                           0x7F,0x77,0x7b,0x7C,0x4d,0x3e,0x6d,0x69,0x00,0x20};
volatile static unsigned char LEDDispBuf[8]  __attribute__ ((at(0x1e5)));
/*
unsigned char TM1638_Read(void)					//�����ݺ���
{
	unsigned char i;
	unsigned char temp=0;
	TM1638_DIO=1;	//����Ϊ����
	for(i=0;i<8;i++)
	{
		temp>>=1;
		TM1638_CLK=0;
		if(TM1638_DIO)
			temp|=0x80;
		TM1638_CLK=1;
	}
	return temp;
}*/

void TM1638_Write(unsigned char	DATA)			//д���ݺ���
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		TM1638_CLK=0;
		if(DATA&0X01)
			TM1638_DIO=1;
		else
			TM1638_DIO=0;
		DATA>>=1;
		TM1638_CLK=1;
	}
}

void Write_COM(unsigned char cmd)		//����������
{
	TM1638_STB=0;
	TM1638_Write(cmd);
	TM1638_STB=1;
}
void Write_DATA(unsigned char add,unsigned char DATA)		//ָ����ַд������
{
	Write_COM(0x44);
	TM1638_STB=0;
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	TM1638_STB=1;
}

void DigitalErrorDisp(u8 Index)
{
	u8 i,j = 0;
	LEDDispBuf[5] = Index;
	LEDDispBuf[6] = 0x0e;
	LEDDispBuf[7] = 0x10;
	
	LEDDispBuf[2] = 0x11;
	LEDDispBuf[3] = 0x11;
	LEDDispBuf[4] = 0x11;		
	for(i=0;i<12;i++)    	
	{
		Write_DATA(i,tab[LEDDispBuf[7-j]]);
		i++;
		j++;
	}	
}

const u8 WaterMlTab[]={15,25,35,50};
const u8 WaterTmpTab[]={45,65,85,100};

void DigitalDisp(void)
{
	u8 i;
	static u8 j __attribute__ ((at(0x1Fe)));
	j = 0;
	LEDDispBuf[5] = 0x10;
	LEDDispBuf[6] = 0x10;
	LEDDispBuf[7] = 0x10;
	if(WorkMode != RDY_MODE)
	{
		i = WaterTmpTab[WaterTmpIndex];
		if(i >= 100)
		{
			LEDDispBuf[7] = i/100;
		}
		LEDDispBuf[6] = i/10%10;	
		LEDDispBuf[5] = i%10;			
	}
	else
	{
		if(hotTmp >= 100)
		{
			LEDDispBuf[7] = hotTmp/100;
		}
		if(hotTmp >= 10)
		{
			
			LEDDispBuf[6] = hotTmp/10%10;
			LEDDispBuf[5] = hotTmp%10;	
		}
		else
		{
			LEDDispBuf[5] = hotTmp;
		}
	}
	LEDDispBuf[2] = 0x10;
	LEDDispBuf[3] = 0x10;
	LEDDispBuf[4] = 0x10;
	if(WorkMode != RDY_MODE)
	{
		LEDDispBuf[4] = WaterMlTab[WaterMlIndex]/10;
		LEDDispBuf[3] = WaterMlTab[WaterMlIndex]%10;	
		LEDDispBuf[2] = 0;	
	}

	for(i=0;i<12;i++)    	
	{
		Write_DATA(i,tab[LEDDispBuf[7-j]]);
		i++;
		j++;
	}	
}


void LEDTips(void)
{
	
	if(LEDBTemp)
		LEDDispBuf[1] |= MASK_LEDTEMPON;
	else
		LEDDispBuf[1] &= MASK_LEDTEMPOFF;
	if(LEDBWaterML)
		LEDDispBuf[1] |= MASK_LEDWATERMLON;
	else
		LEDDispBuf[1] &= MASK_LEDWATERMLOFF;
	if(LEDBWaterOut)
		LEDDispBuf[1] |= MASK_LEDWATEROUTON;
	else
		LEDDispBuf[1] &= MASK_LEDWATEROUTOFF;
	if(LEDBLockRed)
		LEDDispBuf[1] |= MASK_LEDLOCKREDON;
	else
		LEDDispBuf[1] &= MASK_LEDLOCKREDOFF;
	if(LEDBLOCKBlue)
		LEDDispBuf[1] |= MASK_LEDLOCKBLUEON;
	else
		LEDDispBuf[1] &= MASK_LEDLOCKBLUEOFF;
		
	if(LEDBUV)
		LEDDispBuf[0] |= MASK_LEDBUVON;
	else
		LEDDispBuf[0] &= MASK_LEDBUVOFF;
	if(LEDBOC)
		LEDDispBuf[0] |= MASK_LEDBOCON;
	else
		LEDDispBuf[0] &= MASK_LEDBOCOFF;
	if(LEDBML)
		LEDDispBuf[0] |= MASK_LEDBMLON;
	else
		LEDDispBuf[0] &= MASK_LEDBMLOFF;
	if(WorkMode != RDY_MODE)
	{
		if(LEDBGyuan)
		{
			LEDDispBuf[0] |= MASK_LEDBGYON;
			LEDDispBuf[0] &= MASK_LEDBPYOFF;
		}
		else
		{
			LEDDispBuf[0] &= MASK_LEDBGYOFF;
			LEDDispBuf[0] |= MASK_LEDBPYON;
		}
	}
	else
	{
		LEDDispBuf[0] &= MASK_LEDBPYOFF;
		LEDDispBuf[0] &= MASK_LEDBGYOFF;
	}
																						
	Write_DATA(12,LEDDispBuf[1]);
	Write_DATA(13,0);
	Write_DATA(14,LEDDispBuf[0]);
	Write_DATA(15,0);
}

//TM1638��ʼ������
void TM1638Init(void)
{
	unsigned char i;
	Write_COM(0x88);       //���� (0x88-0x8f)8�����ȿɵ�
	Write_COM(0x40);       //���õ�ַ�Զ���1
	TM1638_STB = 0;		   //
	TM1638_Write(0xc0);    //������ʼ��ַ

	for(i=0;i<16;i++)	   //����16���ֽڵ�����
		TM1638_Write(0x00);
	TM1638_STB = 1;
}

/*
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
	TM1638_STB=0;
	TM1638_Write(0x42);		           //����ɨ���� ����
	for(i=0;i<4;i++)		
		c[i]=TM1638_Read();
	TM1638_STB=1;					           //4���ֽ����ݺϳ�һ���ֽ�
	for(i=0;i<4;i++)
		key_value|=c[i]<<i;
	for(i=0;i<8;i++)
		if((0x01<<i) == key_value)
			break;
	return i;
}*/

/*
void Write_oneLED(unsigned char num,unsigned char flag)	//��������һ��LED������numΪ��Ҫ���Ƶ�led��ţ�flagΪ0ʱϨ�𣬲�Ϊ0ʱ����
{
	if(flag)
		Write_DATA(2*num+1,1);
	else
		Write_DATA(2*num+1,0);
}  	*/
/*
void Write_allLED(unsigned char LED_flag)					//����ȫ��LED������LED_flag��ʾ����LED״̬
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		if(LED_flag&(1<<i))
			//Write_DATA(2*i+1,3);
			Write_DATA(2*i+1,1);
		else
			Write_DATA(2*i+1,0);
	}
}
*/
/*
void LedDisplay(void)
{
	unsigned char  i;
	unsigned char  SendData[8];
	for(i=0;i<8;i++)	   //����16���ֽڵ�����
		LEDDispBuf[i] = 5;

	SendData[0]=(tab[LEDDispBuf[0]]&0x01)+((tab[LEDDispBuf[1]]&0x01)<<1)+((tab[LEDDispBuf[2]]&0x01)<<2)+((tab[LEDDispBuf[3]]&0x01)<<3)+((tab[LEDDispBuf[4]]&0x01)<<4)
		+((tab[LEDDispBuf[5]]&0x01)<<5)+((tab[LEDDispBuf[6]]&0x01)<<6)+((tab[LEDDispBuf[7]]&0x01)<<7);

	SendData[1]=((tab[LEDDispBuf[0]]&0x02)>>1)+((tab[LEDDispBuf[1]]&0x02))+((tab[LEDDispBuf[2]]&0x02)<<1)+((tab[LEDDispBuf[3]]&0x02)<<2)+((tab[LEDDispBuf[4]]&0x02)<<3)
		+((tab[LEDDispBuf[5]]&0x02)<<4)+((tab[LEDDispBuf[6]]&0x02)<<5)+((tab[LEDDispBuf[7]]&0x02)<<6);

	SendData[2]=((tab[LEDDispBuf[0]]&0x04)>>2)+((tab[LEDDispBuf[1]]&0x04)>>1)+((tab[LEDDispBuf[2]]&0x04))+((tab[LEDDispBuf[3]]&0x04)<<1)+((tab[LEDDispBuf[4]]&0x04)<<2)
		+((tab[LEDDispBuf[5]]&0x04)<<3)+((tab[LEDDispBuf[6]]&0x04)<<4)+((tab[LEDDispBuf[7]]&0x04)<<5);

	SendData[3]=((tab[LEDDispBuf[0]]&0x08)>>3)+((tab[LEDDispBuf[1]]&0x08)>>2)+((tab[LEDDispBuf[2]]&0x08)>>1)+((tab[LEDDispBuf[3]]&0x08))+((tab[LEDDispBuf[4]]&0x08)<<1)
		+((tab[LEDDispBuf[5]]&0x08)<<2)+((tab[LEDDispBuf[6]]&0x08)<<3)+((tab[LEDDispBuf[7]]&0x08)<<4);

	SendData[4]=((tab[LEDDispBuf[0]]&0x10)>>4)+((tab[LEDDispBuf[1]]&0x10)>>3)+((tab[LEDDispBuf[2]]&0x10)>>2)+((tab[LEDDispBuf[3]]&0x10)>>1)+((tab[LEDDispBuf[4]]&0x10))
		+((tab[LEDDispBuf[5]]&0x10)<<1)+((tab[LEDDispBuf[6]]&0x10)<<2)+((tab[LEDDispBuf[7]]&0x10)<<3);

	SendData[5]=((tab[LEDDispBuf[0]]&0x20)>>5)+((tab[LEDDispBuf[1]]&0x20)>>4)+((tab[LEDDispBuf[2]]&0x20)>>3)+((tab[LEDDispBuf[3]]&0x20)>>2)+((tab[LEDDispBuf[4]]&0x20)>>1)
		+((tab[LEDDispBuf[5]]&0x20))+((tab[LEDDispBuf[6]]&0x20)<<1)+((tab[LEDDispBuf[7]]&0x20)<<2);

	SendData[6]=((tab[LEDDispBuf[0]]&0x40)>>6)+((tab[LEDDispBuf[1]]&0x40)>>5)+((tab[LEDDispBuf[2]]&0x40)>>4)+((tab[LEDDispBuf[3]]&0x40)>>3)+((tab[LEDDispBuf[4]]&0x40)>>2)
		+((tab[LEDDispBuf[5]]&0x40)>>1)+((tab[LEDDispBuf[6]]&0x40))+((tab[LEDDispBuf[7]]&0x40)<<1);

	SendData[7]=((tab[LEDDispBuf[0]]&0x80)>>7)+((tab[LEDDispBuf[1]]&0x80)>>6)+((tab[LEDDispBuf[2]]&0x80)>>5)+((tab[LEDDispBuf[3]]&0x80)>>4)+((tab[LEDDispBuf[4]]&0x80)>>3)
		+((tab[LEDDispBuf[5]]&0x80)>>2)+((tab[LEDDispBuf[6]]&0x80)>>1)+((tab[LEDDispBuf[7]]&0x80));

	Write_COM(0x8a);//����
	Write_COM(0x40); //д ��������
	TM1638_STB=0;
	TM1638_Write(0xc0);		//д��ַ����
	
	TM1638_Write(SendData[0]);
	TM1638_Write(0x80);
	TM1638_Write(SendData[1]);
	TM1638_Write(0x00);
	TM1638_Write(SendData[2]);
	TM1638_Write(0x00);
	TM1638_Write(SendData[3]);
	TM1638_Write(0x00);
	TM1638_Write(SendData[4]);
	TM1638_Write(0x00);
	TM1638_Write(SendData[5]);
	TM1638_Write(0x00);
	TM1638_Write(SendData[6]);
	TM1638_Write(0x00);
	TM1638_Write(SendData[7]);
	TM1638_Write(0x00);

	TM1638_STB=1;


}*/
