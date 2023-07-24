#ifndef __AT24C02_H__
#define __AT24C02_H__

//ͷ�ļ�
#include "stm32f4xx.h"
#include "systick.h"
#include "uart.h"
#include "iic.h"

//�궨��

/*
 * ��  �ܣ���ҳ�洢���ݣ�1ҳ8�ֽڣ�
 * ��  ����
 *		addr��0x00--0xF
 *		pBuf��
 */
#define AT24C02_WRITE_PAGE(addr, pBuf) AT24C02_WriteData(addr, pBuf, 8)
#define AT24C02_READ_PAGE(addr, pBuf) AT24C02_ReadData(addr, pBuf, 8)

//��������

/*
 * ��  �ܣ�AT24C02ʵ��ҳд 
 * ��  ����
 *		��
 * ����ֵ��
 *		��
 */
void AT24C02_Init(void);


/*
 * ��  �ܣ�AT24C02ʵ��ҳд 
 * ��  ����
 *		addr	--->������д�� AT24C02 �� addr �����ַ��
 * 		pBuf	--->����Ҫд������ݵ��׵�ַ
 * 		length	--->��Ҫд����ֽ���
 * ����ֵ��
 *		ʧ��	--->-1
 * 		�ɹ�	--->�ɹ�д����ֽ���
 */
uint8_t AT24C02_WriteData(uint8_t addr , uint8_t *pBuf , uint8_t length);


/*
 * ��  �ܣ�AT24C02ʵ�������
 *		addr	--->���� AT24C02 ��ȡ���ݵĵĵ�ַ
 * 		pBuf	--->��ȡ����������Ҫ��ŵ��ڴ������׵�ַ
 * 		length	--->Ҫ��ȡ���ֽ���
 * ����ֵ��
 *		ʧ��	---> -1
 * 		�ɹ�	--->  0
 */
uint8_t AT24C02_ReadData(uint8_t addr , uint8_t *pBuf , uint8_t length);


#endif
