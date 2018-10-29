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
    InitSys();				//系统初始化
    InitRam();				//RAM初始化

	PowerOnTips();			//上电提示

    while(1)
    {
        GCC_CLRWDT();		//清看门狗
        Func();				//功能实现
		if(TaskADCRdyFlag)
		{
			TaskADCRdyFlag = false;
	        GetAdcDat();		//NTC传感器数据读取
		}
		if(TaskKeyRdy)
		{
			TaskKeyRdy = false;
	        GetSwitch();	//读取状态
	        GetKey();		//读取键值
		}
		PowerDownFunc();	//掉电检测无过零保存设置参数       
    }
}
