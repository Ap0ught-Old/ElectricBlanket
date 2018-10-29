#include "base.h"
#define EXTERN extern
#include "Var.h"
void SelfDeal(void)
{

    delayWate=0;
    hotDelay=0;
    WorkMode=0;

    while(1)
    {
        GCC_CLRWDT();
        GetAdcDat();



//5ms cycle~~~~~~~~~~~~~~~~~~~~~
        GetSwitch();
        GetKey();
        
//100ms cycle~~~~~~~~~~~~~~~~~~~~~
        if(hotDelay<10)
        {
            hotDelay++;
//            if(hotDelay==10)   ledDat=0x00;
//            else  ledDat=0x1f;
            continue;
        }
    }
}
