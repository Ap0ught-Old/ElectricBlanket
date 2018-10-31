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
    while(1)
    {
        GCC_CLRWDT();		//清看门狗
        Func();				//功能实现
		if(TaskADCRdyFlag)
		{
			TaskADCRdyFlag = false;
	        GetPTCValu();		//PTC传感器数据读取
	        if(NoZeroTime > 50)
	        {
	        	ZeroFlag = true;
	        }
		}
		if(TaskKeyRdy)
		{
			TaskKeyRdy = false;
	        GetSwitch();	//读取状态
	        GetKey();		//读取键值
		}
		PowerDownFunc();	//掉电检测无过零保存设置参数
		if(TaskChkLoadRdy)  //负载任务就绪     
		{
			TaskChkLoadRdy = false;
			GetLoadValu();
		}
    }
}
