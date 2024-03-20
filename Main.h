/* 
 * File:   Main.h
 * Author: jjy99
 *
 * Created on 2021년 3월 15일 (월), 오후 1:36
 */

#ifndef __MAIN_H
#define	__MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif
typedef  unsigned char   	uint8_t;
typedef  unsigned int    	uint16_t;
typedef  unsigned long int 	uint32_t;
typedef  signed char   		int8_t;
typedef  signed int    		int16_t;
typedef  signed long int 	int32_t;
    

#define KEY_COUNT           8

#define DEHUMI_BTN          RE1
#define UP_BTN              RE6
#define DOWN_BTN            RE5
#define STRONG_BTN          RE7
#define RESERVE_BTN         RA7
#define OPERSTOP_BTN        RE4
#define BLOW_BTN            RE2
#define WEAK_BTN            RA6

#define RS485_DE            RC5
#define BUZZERCONTROL       RD1
#define BUZZER              RD2
#define REMOCONRXPIN        RB0
#define BACKLIGHT           RB1


enum {OPERSTOP_KEY = 0, RESERVE_KEY, BLOW_KEY, DEHUMI_KEY, STRONG_KEY, WEAK_KEY, UP_KEY, DOWN_KEY};




#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

