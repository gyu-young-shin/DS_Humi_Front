/* 
 * File:   Main.c
 * Author: jjy99
 *
 * Created on 2021년 3월 15일 (월), 오후 1:32
 * PIC16F946
 */

#include <htc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
#include "Rs485.h"
#include "Segment.h"
#include "Timer.h"
#include "buzzer.h"
#include "dehumi.h"
#include "eeprom.h"
#include "Testmode.h"


#ifdef DEBUG_MODE
__CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_OFF & IESO_OFF & FCMEN_OFF & DEBUG_ON);
#else
__CONFIG(FOSC_INTOSCIO & WDTE_OFF & PWRTE_ON & MCLRE_ON & CP_ON & CPD_ON & BOREN_ON & IESO_OFF & FCMEN_OFF & DEBUG_OFF);
#endif

#ifndef _XTAL_FREQ
 // Unless already defined assume 4MHz system frequency
 // This definition is required to calibrate __delay_us( ) and __delay_ms( )
 #define _XTAL_FREQ 8000000
#endif

/*
 * 
 */
//===========================================================================================
extern  bit     flg_dehumi_execute;
extern  bit     flg_e2p_save;
extern  bit     flg_testmode;
extern  bit     flg_toggle_500ms;;
extern  bit 	flg_short_circuit;

extern  ICON_TYPE   icon_led;
extern uint8_t  key_pushed[KEY_COUNT];
//===========================================================================================
void PortInit(void) 
{
    PORTA = 0b00000000;
    TRISA = 0b11000000;
    ANSEL = 0;			

    PORTB = 0b00001100;	//led off
    TRISB = 0b00000001;

    PORTC = 0b00000000;
    TRISC = 0b10000111;

    PORTD = 0b00000000;
    TRISD = 0b00000000;

    PORTE = 0b00000000;
    TRISE = 0b11111110;


    TRISF = 0b00000000;
    PORTF = 0b00000000;

    PORTG = 0b00000000;
    TRISG = 0b00000000;

    RS485_DE = 0;
    BACKLIGHT = 0;
}


void SfrInit(void) 
{
    OSCCON = 0b01110101;					// 8MHz, HTS, SCS ON

    OPTION_REG = 0b01000001;				// PULL-UP:ENABLE, PRESCALER:TIMER0()1 : 4
    INTCON = 0b01100000;					// Timer 0 Enable

    PIE1 = 0b00000000;					// PIE1 EEIE ADIE RCIE TXIE SSPIE CCP1IE TMR2IE TMR1IE

    PIE2 = 0b00000000;						// No Use
    PIR1 = 0b00000000;						// No Use
    PIR2 = 0b00000000;						// No Use
    PCON = 0b00000000;						// POR SET, BOR=ENABLE

    CMCON0 = 0x07;						//Comparators off. CxIN pins are configured as digital I/O
    CMCON1 = 0x00;						// No Use	

    TMR0 = 161;
    T0IE = 0;
    T1CON = 0b00010101;    
    
    TMR1H = 0xFC;
    TMR1L = 0x17;
    TMR1IF = 0;
    T2CON   = 0b00000111;                                   //--- TOUTPS3 TOUTPS2 TOUTPS1 TOUTPS0 TMR2ON T2CKPS1 T2CKPS0
    
    TMR2    = 0;                                       
    PR2     = 50;         
    //RC1/T1OSI/CCP2(1)/P2A(1)
    CCP1CON = 0x00;                 //CCPx CONTROL REGISTER         -- -- DC2B1 DC2B0 CCP2M3 CCP2M2 CCP2M1 CCP2M0
    CCP2CON = 0x00;

    INTCON |= 0b01100000;	//0b00110000;           //GIE PEIE T0IE INTE RBIE(1,3) T0IF(2) INTF RBIF
    INTF = 1;
}

void LcdInit(void) 
{
    LCDCON = 0b10010011;	//LCD Driver Enable,VLCD pins are enabled,1/4 (COM<3:0>)
    LCDPS  = 0b00000010;		

    LCDSE0 = 0b10110000;	// 7~0		
    LCDSE1 = 0b10010000;	// 15~8
    LCDSE2 = 0b00110000;	// 23~16
    LCDSE3 = 0b00000000;	// 31~24
    LCDSE4 = 0b11110111;	// 39~32
    LCDSE5 = 0b00000011;	// 41~40
}

void main(void)
{
    PortInit();	
    SfrInit();
    LcdInit();	
    Rs485_Init();
    EEPROM_Init();
    
    PEIE= 1;
    T0IE = 1;
    TMR1IE = 1;
    GIE = 1;	
    
    Play_Buzzer(3);
    BACKLIGHT = 1;
    
    Disp_Version();
    SEG_UpDate();
    __delay_ms(1500);
    BACKLIGHT = 0;
    Key_Scan();

	if(key_pushed[RESERVE_KEY] && key_pushed[DOWN_KEY])             // 단축모드
	{
        flg_short_circuit = 1;
        Play_Buzzer(1);
	}

	if(key_pushed[STRONG_KEY] && key_pushed[WEAK_KEY])              // 테스트 모드
	{
		flg_testmode = 1;
	}

    Init_Variable();
    
	if(flg_testmode)
	{
        Seg_data_set(0, 24);        	//'t'
        Seg_data_set(1, 14);        	//'E'
        Seg_data_set(2, 5);             //'5'
        Seg_data_set(3, 24);			//'t'
        SEG_UpDate();
        BACKLIGHT = 1;
		__delay_ms(1000);
		
		while(1)
		{
			Timer_Proc();
            SEG_UpDate();
			Rs485_proc();			
			Test_Proc();
		}
	}
    
    while(1)
    {
        Timer_Proc();
        SEG_UpDate();
        
        Rs485_proc();
        Disp_BlowLevel();
        Animation_Blowfan();
        Animation_Dehumi();
        
        if(flg_dehumi_execute)
        {
            flg_dehumi_execute = 0;

            InputCheck_Proc();
            Error_Recovery_Proc();
            IR_Receive_Proc();
            InputKey_Proc();
            Disp_Segment();
            Control_Proc();
            Output_Proc();

            if(flg_e2p_save)
            {
                flg_e2p_save = 0;
                E2pData_Write();
            }
        }
    }
}

