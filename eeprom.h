/* 
 * File:   eeprom.h
 * Author: jjy99
 *
 * Created on 2021년 3월 15일 (월), 오후 1:43
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
	uint8_t    	e2p_flg_oper;       	// 운전중
	int8_t		e2p_dehumi_cali;    	// 습도차 보정 (-9% - +9%)
	uint8_t    	e2p_sethumi;            // 설정 습도
	uint8_t    	e2p_senuse_onoff;   	// 습도센서 사용 유무
	int8_t    	e2p_jesang_temp;        // 제상 발생 온도
	uint8_t    	e2p_fanoff_delay;   	// FAN OFF 지연설정
	uint8_t    	e2p_fandelay_time;      // FAN OFF 지연 시간 설정
	uint8_t    	e2p_E7_check;	     	// 냉매저압 E7 검출 기능
	uint8_t    	e2p_jesang_reltemp; 	// 제상 해제 온도
	uint8_t    	e2p_jesang_reltime; 	// 제상 해제 시간
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

