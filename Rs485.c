#include <htc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
#include "Segment.h"
#include "Buzzer.h"
#include "Timer.h"
#include "Rs485.h"
#include "Dehumi.h"
#include "Eeprom.h"


#ifndef _XTAL_FREQ
 // Unless already defined assume 4MHz system frequency
 // This definition is required to calibrate __delay_us( ) and __delay_ms( )
 #define _XTAL_FREQ 8000000
#endif
//===========================================================================================
bit         flg_485_send;
bit         urx2_comp;
bit         tx2_restart;

uint8_t     urx2_buf[URX2_LEN];
uint8_t     utx2_buf[UTX2_LEN];
uint8_t     send_count = 0;
uint8_t     send_idx = 0;
//int8_t      rs485_cur_temper = 25;
//int8_t      compare_cur_temper = 25;
//uint8_t     cur_chk_timeout = 0;

uint16_t 	urx2_count = 0;
uint16_t	urx2_tout = 0;
uint16_t	comm_timeout = 10;
//===========================================================================================
extern bit	COMM_COMP;
extern bit	COMM_SOL;
extern bit	COMM_EVAFAN;
extern bit	COMM_EVA_HIGHLOW;
extern bit	COMM_COMPFAN;

extern bit	COMM_WATER_OVER;
extern bit	COMM_HIGH_PRESS;
extern bit	COMM_LOW_PRESS;
extern bit	COMM_REMOTE;
extern bit	COMM_DIP_SW1;
extern bit	COMM_DIP_SW2;
extern bit	COMM_DIP_SW3;
extern bit	COMM_DIP_SW4;

extern bit 	flg_remote_controle;

extern int8_t       comm_cur_temper;
extern int8_t       comm_out_temper;
extern int8_t       comm_eva_temper;
extern uint8_t      comm_cur_humi;
extern uint16_t    error_code;
extern TE2PDataRec 	Flashdatarec;

extern uint16_t MakeCCITT16(uint8_t *data, uint32_t len);
//===========================================================================================
void Rs485_Init(void) 
{
    TXSTA = 0x20;                                           // Clock Source Select, 9bit, Transmit enable, Sync
    RCSTA = 0x90;                                           // serial port enable, 수신 가능

    SPBRG = 12;                                              // 9600bps
    TXIE = 0;                                                   // 송신 인터럽트
    RCIE = 1;                                                   // 수신 인터럽트
    
    TXEN = 1;
    SYNC = 0;
    SPEN = 1;
}

void Rs485_proc(void) 
{
	uint16_t Rcv_Crc, cmp_Crc;
	
	if(flg_485_send && (tx2_restart == 0))
	{
		flg_485_send = 0;
		
		utx2_buf[0] = 0x99;
		utx2_buf[1] = (COMM_COMP << 7) | (COMM_SOL << 6) | (COMM_EVAFAN << 5) | 
                                    (COMM_EVA_HIGHLOW << 4) | (COMM_COMPFAN << 3);
		utx2_buf[2] = 100;			// Comp 출력 비 100%
		
		cmp_Crc = MakeCCITT16(utx2_buf, 3);
		utx2_buf[3] = (uint8_t)((cmp_Crc & 0xFF00) >> 8);
		utx2_buf[4] = (uint8_t)(cmp_Crc & 0x00FF);
        
        tx2_restart = 1;
        send_idx = 0;
        send_count = 5;
        RS485_DE = 1;
        TXIF = 0;
        TXIE = 1;
	}

	if(urx2_comp)
	{
            Rcv_Crc = MakeCCITT16(urx2_buf, urx2_count - 2);	// CRC를 남기고 모두 계산한다.
            cmp_Crc = (uint16_t)(urx2_buf[urx2_count-2] << 8) | urx2_buf[urx2_count-1];

            if((Rcv_Crc == cmp_Crc) && (urx2_buf[0] == 0x88))
            {
                comm_timeout = 10;					// 10 sec
                error_code &= ~ERR_COMM485;
                
                comm_cur_temper = (int8_t)(urx2_buf[1]);
                comm_out_temper  = (int8_t)(urx2_buf[2]);
                comm_eva_temper = (int8_t)(urx2_buf[3]);

//                if(compare_cur_temper == rs485_cur_temper)      // 온도의 급격한 변화를 막기 위해 1초동안 비교한다.
//                {
//                    if(cur_chk_timeout == 0)
//                        comm_cur_temper = rs485_cur_temper;
//                }
//                else
//                {
//                    cur_chk_timeout = 15;                       // 1초
//                    compare_cur_temper = rs485_cur_temper;
//                }
                
                comm_cur_humi = (int8_t)urx2_buf[4] + Flashdatarec.e2p_dehumi_cali;

                COMM_WATER_OVER = (urx2_buf[5] & 0x80) ? 1:0;
                COMM_HIGH_PRESS = (urx2_buf[5] & 0x40) ? 1:0;
                COMM_LOW_PRESS = (urx2_buf[5] & 0x20) ? 1:0;
                COMM_REMOTE  = (urx2_buf[5] & 0x10) ? 1:0;
                COMM_DIP_SW1 = (urx2_buf[5] & 0x08) ? 1:0;
                COMM_DIP_SW2 = (urx2_buf[5] & 0x04) ? 1:0;
                COMM_DIP_SW3 = (urx2_buf[5] & 0x02) ? 1:0;
                COMM_DIP_SW4 = (urx2_buf[5] & 0x01) ? 1:0;

                flg_remote_controle = !COMM_DIP_SW2;
                memset(urx2_buf, 0, URX2_LEN);
            }
            urx2_count = 0;
            urx2_comp = 0;
	}

/*
    comm_timeout = 10;                                                  // 10 sec
    error_code &= ~ERR_COMM485;

    comm_cur_temper = 25;
    comm_out_temper = 27;
    comm_eva_temper = 12;
//			temp_humi = (int8_t)urx2_buf[4] + Flashdatarec.e2p_dehumi_cali;
//			if(temp_humi < 1)
//				comm_cur_humi = 1;
//			else
    comm_cur_humi = 30;

    COMM_WATER_OVER = 0;
    COMM_HIGH_PRESS = 0;
    COMM_LOW_PRESS = 0;
    COMM_REMOTE  = 0;
    COMM_DIP_SW1 = 0;
    COMM_DIP_SW2 = 1;
    COMM_DIP_SW3 = 0;
    COMM_DIP_SW4 = 0;

    flg_remote_controle = !COMM_DIP_SW2;
*/    
}
//------------------------------------------------------------------------------------------


