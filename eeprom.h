/* 
 * File:   eeprom.h
 * Author: jjy99
 *
 * Created on 2021�� 3�� 15�� (��), ���� 1:43
 */

#ifndef EEPROM_H
#define	EEPROM_H
#include "Main.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define E2P_START_ADDRESS	100

typedef struct
{
	uint8_t     FA_code;        		// Offset Address
	uint8_t    	e2p_flg_oper;       	// ������
	int8_t		e2p_dehumi_cali;    	// ������ ���� (-9% - +9%)
	uint8_t    	e2p_sethumi;            // ���� ����
	uint8_t    	e2p_senuse_onoff;   	// �������� ��� ����
	int8_t    	e2p_jesang_temp;        // ���� �߻� �µ�
	uint8_t    	e2p_fanoff_delay;   	// FAN OFF ��������
	uint8_t    	e2p_fandelay_time;      // FAN OFF ���� �ð� ����
	uint8_t    	e2p_E7_check;	     	// �ø����� E7 ���� ���
	uint8_t    	e2p_jesang_reltemp; 	// ���� ���� �µ�
	uint8_t    	e2p_jesang_reltime; 	// ���� ���� �ð�
	uint8_t    	pads[2]; 				
} TE2PDataRec;

void EEPROM_Init(void);
uint8_t Eeprom_Read(uint8_t addr);
void Eeprom_Write(uint8_t addr, uint8_t wdata);
void E2pData_Write(void);



#ifdef	__cplusplus
}
#endif

#endif	/* EEPROM_H */

