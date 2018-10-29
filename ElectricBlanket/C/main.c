//products mode:   MW
//functional description: 
//MCU mode:        HT66F018
//MCU frequency:   8MHz
//designer:        xj.jiang
//data:    2018/7/27 10:58
//record:

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "base.h"
#define EXTERN
#define VAR_DEFINE
#include "Var.h"
void PowerDownFunc(void);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void main()
{
    InitSys();				//ϵͳ��ʼ��
    InitRam();				//RAM��ʼ��

	PowerOnTips();			//�ϵ���ʾ

    while(1)
    {
        GCC_CLRWDT();		//�忴�Ź�
        Func();				//����ʵ��
		if(TaskADCRdyFlag)
		{
			TaskADCRdyFlag = false;
	        GetAdcDat();		//NTC���������ݶ�ȡ
		}
		if(TaskKeyRdy)
		{
			TaskKeyRdy = false;
	        GetSwitch();	//��ȡ״̬
	        GetKey();		//��ȡ��ֵ
		}
		PowerDownFunc();	//�������޹��㱣�����ò���       
    }
}