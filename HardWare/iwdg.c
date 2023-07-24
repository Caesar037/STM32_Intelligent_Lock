#include "iwdg.h"

//设定独立看门狗的超时时间为1秒，即需要在1秒内喂狗（1秒：对32KHz分频128，且重装载值为250，数值是由公式计算得来的）
//（1/32000）*128*250=1s
void iwdg_init(uint8_t IWDG_Prescaler, uint16_t Reload)
{
	//解除对IWDG_PR和IWDG_RLR寄存器的写访问
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	 
	//设置预分频系数
	IWDG_SetPrescaler(IWDG_Prescaler);
	
	//设置重装载值
	IWDG_SetReload(Reload);
	
	//开启独立看门狗
	IWDG_Enable();

	//喂狗函数
	IWDG_ReloadCounter();
}
