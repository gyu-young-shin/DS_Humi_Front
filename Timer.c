#include <htc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
#include "Segment.h"
#include "Buzzer.h"
#include "Dehumi.h"
#include "Timer.h"
#include "Rs485.h"
#include "Eeprom.h"

#ifndef _XTAL_FREQ
 // Unless already defined assume 4MHz system frequency
 // This definition is required to calibrate __delay_us( ) and __delay_ms( )
 #define _XTAL_FREQ 8000000
#endif

//===========================================================================================
bit     flg_toggle_500ms;
bit     flg_dehumi_execute;
bit     flg_keyscan;
bit     flg_485_excute;
bit	 flg_blink_check = 0;

uint8_t callback_1ms = 0;         	// Timer1 Callback count
uint8_t callback_10ms = 0;
uint8_t callback_100ms = 0;
uint8_t callback_1sec = 0;
uint8_t keyblink_count = 0;
uint8_t Rs_485timeout = 0;

uint8_t  menu_key[KEY_COUNT] = {0,};
uint8_t  keyin_buf[KEY_COUNT][6];
uint8_t  key_pushed[KEY_COUNT] = {0,};
uint8_t  key_continued[KEY_COUNT] = {0,};
uint8_t  key_cont_count[KEY_COUNT] = {0,};
uint8_t  key_idx = 0;

//===========================================================================================
extern bit      flg_e2p_save;
extern bit      urx2_comp;
extern bit      tx2_restart;
extern bit      flg_485_send;

extern uint8_t  urx2_buf[URX2_LEN];
extern uint8_t  utx2_buf[URX2_LEN];
extern uint8_t  send_count;
extern uint8_t  send_idx;

extern uint16_t     urx2_count;
extern uint16_t     urx2_tout;

extern SEG_TYPE    seg_led[4];


extern bit	flg_blink;
extern bit 	flg_offreserve;
extern bit 	flg_onreserve;
extern bit 	flg_evafan_on;
extern bit 	flg_mainloop;
extern bit	flg_blowfan_on;

extern uint8_t  optkey_timeout;
extern uint8_t  key_updown_timeout;
extern uint8_t  key_timeout;
extern uint8_t  disp_mode;

extern uint8_t  e2p_save_timeout;
extern uint8_t  ir_timeout;
extern uint8_t  level_timeout;
extern uint8_t  water_timeout;
extern uint8_t  highp_timeout;
extern uint8_t  lowp_timeout;
extern uint8_t  eva_detect_timeout;
extern uint8_t  evatemp_timeout;
extern uint8_t  keycont_timeout;
extern uint8_t  warning_timeout;
extern uint8_t  ani_timeout;
extern uint8_t  ani_dehumi_timeout;

extern uint16_t     offreserve_timeout;
extern uint16_t     onreserve_timeout;
extern uint16_t     evafan_offtime;
extern uint16_t     evadelay_timeout;
extern uint16_t     humidity_timeout;
extern uint16_t     roomtemp_timeout;
extern uint16_t     eva_error_timeout;
extern uint16_t     comm_timeout;
extern uint16_t     error_code;
//extern uint16_t 	high_press_timeout;
//extern uint16_t 	low_press_timeout;

extern uint16_t     compdelay_timeout;
extern TE2PDataRec 	Flashdatarec;
extern uint8_t      set_cur_humi;
extern uint32_t     test_timeout;
//extern uint8_t      cur_chk_timeout;
extern uint8_t      curtemp_error_timeout;
//===========================================================================================
void interrupt ISR(void)
{
    uint8_t data_char;
    
    //------------------------------------------------------------------
    if(RCIF)                                    // RS485 RX Interrupt
    {			
        RCIF = 0;
        if(RCSTA & 0x06)            // Fram Error, Overrun Error 
        {
            CREN = 0;
            CREN = 1;                   // Continuous Receive Enable bit
        }

        data_char = RCREG;   
        
        if((!urx2_comp) && (urx2_count < URX2_LEN))
        {
            urx2_buf[urx2_count++] = data_char;
            urx2_tout = 3;
        }
        RCIE = 1;
    }
    //------------------------------------------------------------------
    if(TXIF && TXIE)                                    // RS485 TX Interrupt
    {
        if(send_count) 							
        {
            TXREG = utx2_buf[send_idx];
            send_count--;
            send_idx++;
        }
        else 
        {
            tx2_restart = 0;
            TXIE = 0;
            Rs_485timeout = 3;
        }
        TXIF = 0;
    }
    //------------------------------------------------------------------
    
    if(T0IF)                                    //	200us
    {
        TMR0 = 161;												
        T0IF = 0;

        IrRxProc();
    }

    // Timer1
    if(TMR1IF)					// 1ms
    {
        TMR1H = 0xFC;
        TMR1L = 0x17;
        TMR1IF = 0;

        callback_1ms++;

        if(urx2_tout)
        {
                urx2_tout--;
                if(urx2_tout == 0)
                    urx2_comp = 1;
        }
        
        if(Rs_485timeout)
        {
            Rs_485timeout--;
            if(Rs_485timeout == 0)
                RS485_DE = 0;
        }
        
        if(callback_1ms >= 10)      // 10ms
        {
            callback_1ms = 0;
            callback_10ms++;

            if(ir_timeout > 0)
                ir_timeout--;

            if(level_timeout)
                level_timeout--;

            if(keycont_timeout)
                keycont_timeout--;
            
            if(ani_timeout)
                ani_timeout--;
            
            if(ani_dehumi_timeout)
                ani_dehumi_timeout--;
            
            if(test_timeout)
                test_timeout--;
            
            flg_keyscan = 1;
            Buzzer_Sequence();      // Buzzer
        }
    }
}

//-----------------------------------------------------------------------------------
void Timer_Proc(void)
{
    if(flg_keyscan)
    {
        flg_keyscan = 0;
        Key_Scan();
    }
    
    if(callback_10ms >= 10)		// 100ms
    {
        callback_10ms = 0;
        callback_100ms++;

        flg_dehumi_execute = 1;
        
        if(key_timeout)
            key_timeout--;
        
        if(e2p_save_timeout)
        {
            e2p_save_timeout--;
            if(e2p_save_timeout == 0)
                flg_e2p_save = 1;
        }

        if(key_updown_timeout)
        {
        	key_updown_timeout--;
        	keyblink_count = 0;
        	
        	if(key_updown_timeout == 0)
        	{
        		flg_blink = 1;
        		keyblink_count = 2;
        	}
        }
		
        if(water_timeout)			// ¸¸¼ö Ã¼Å© Å¸ÀÓ¾Æ¿ô
            water_timeout--;

        if(evatemp_timeout)
            evatemp_timeout--;

        if(warning_timeout)
            warning_timeout--;
        
        if(eva_detect_timeout)		// ¿¡¹Ù µ¿°á Å¸ÀÓ¾Æ¿ô
            eva_detect_timeout--;

//        if(cur_chk_timeout)
//            cur_chk_timeout--;
        
        if(curtemp_error_timeout)
            curtemp_error_timeout--; 
        
        flg_485_excute = !flg_485_excute;
        if(flg_485_excute)
            flg_485_send = 1;
        
        if(!(callback_100ms % 5))
        {
            flg_toggle_500ms = !flg_toggle_500ms;
            
            RB2 = !RB2;
            
            if(keyblink_count)
            {
                if(flg_toggle_500ms == 0)			// ²¨Áü
                    flg_blink_check = 1;
                else
                {
                    if(flg_blink_check)
                    {
                        keyblink_count--;
                        if(keyblink_count ==0)
                        {
                            Flashdatarec.e2p_sethumi = set_cur_humi;
                            flg_blink = 0;
                            flg_e2p_save = 1;			// E2p Save
                            disp_mode = DISP_OPERATION;
                        }
                    }
                }
            }
        }
    }


    if(callback_100ms >= 10)	// 1sec
    {
        callback_100ms = 0;
        callback_1sec++;

        if(optkey_timeout)
            optkey_timeout--;

        if(highp_timeout)			// °í¾Ð¿¡·¯ Å¸ÀÓ¾Æ¿ô
            highp_timeout--;

        if(lowp_timeout)			// Àú¾Ð¿¡·¯ Å¸ÀÓ¾Æ¿ô
            lowp_timeout--;

        if(evafan_offtime)
        {
            evafan_offtime--;
            if(evafan_offtime == 0)
                flg_evafan_on = 0;                                      // ¼ÛÇ³ÆÒ OFF
        }

        if(evadelay_timeout)
            evadelay_timeout--;

        if(humidity_timeout)
            humidity_timeout--;

        if(roomtemp_timeout)
            roomtemp_timeout--;

        if(eva_error_timeout)
            eva_error_timeout--;

        if(comm_timeout)
        {
            comm_timeout--;
            if(comm_timeout == 0)
                error_code |= ERR_COMM485;
        }

        if(compdelay_timeout)
            compdelay_timeout--;
        
//        if(high_press_timeout)
//            high_press_timeout--;
//
//        if(low_press_timeout)
//            low_press_timeout--;
        
    }

    if(callback_1sec >= 60)				// 1ºÐ
    {
        callback_1sec  = 0;

        if(flg_offreserve)
        {
            if(offreserve_timeout)
                offreserve_timeout--;
        }

        if(flg_onreserve)
        {
            if(onreserve_timeout)
                onreserve_timeout--;
        }
    }
}
//-------------------------------------------------------------------------------------------
void Key_Scan(void)
{
    uint8_t i, j, result_var;

    keyin_buf[RESERVE_KEY][key_idx] = RESERVE_BTN;
    keyin_buf[UP_KEY][key_idx] = UP_BTN;
    keyin_buf[DOWN_KEY][key_idx] = DOWN_BTN;
    keyin_buf[STRONG_KEY][key_idx] = STRONG_BTN;
    keyin_buf[DEHUMI_KEY][key_idx] = DEHUMI_BTN;
    keyin_buf[OPERSTOP_KEY][key_idx] = OPERSTOP_BTN;
    keyin_buf[BLOW_KEY][key_idx] = BLOW_BTN;
    keyin_buf[WEAK_KEY][key_idx] = WEAK_BTN;

    key_idx++;
    if(key_idx >= 6)
        key_idx = 0;

    for(i=0; i < KEY_COUNT; i++)
    {
        result_var = 1;
        
        for(j=0; j<6; j++)
        {
        	if(keyin_buf[i][j])
        	{
        		result_var = 0;
        		break;
        	}
        }
        
        if(result_var)
        {
            if(key_pushed[i] == 0)                        // Key Pushed Flag Ã³À½ÀÏ °æ¿ì
            {
                key_pushed[i] = 1;
                key_cont_count[i] = 0;
            }
            else
            {
                if(key_continued[i] == 0)
                {
                    if (key_cont_count[i] < 50)                                // 500ms
                        key_cont_count[i]++;
                    else
                    {
                        key_continued[i] = 1;
                        key_cont_count[i] = 0;
                    }
                }
            }
        }
        else
        {
            if(key_pushed[i])
                menu_key[i] = 1;
            key_pushed[i] = 0;
            key_continued[i] = 0;
            key_cont_count[i] = 0;
        }
    }
}
//---------------------------------------------------------------------------------------------------

