#include "iwdg.h"

//�趨�������Ź��ĳ�ʱʱ��Ϊ1�룬����Ҫ��1����ι����1�룺��32KHz��Ƶ128������װ��ֵΪ250����ֵ���ɹ�ʽ��������ģ�
//��1/32000��*128*250=1s
void iwdg_init(uint8_t IWDG_Prescaler, uint16_t Reload)
{
	//�����IWDG_PR��IWDG_RLR�Ĵ�����д����
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	 
	//����Ԥ��Ƶϵ��
	IWDG_SetPrescaler(IWDG_Prescaler);
	
	//������װ��ֵ
	IWDG_SetReload(Reload);
	
	//�����������Ź�
	IWDG_Enable();

	//ι������
	IWDG_ReloadCounter();
}
