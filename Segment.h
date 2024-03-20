/* 
 * File:   Segment.h
 * Author: jjy99
 *
 * Created on 2021년 3월 15일 (월), 오후 1:45
 */

#ifndef SEGMENT_H
#define	SEGMENT_H
#include "Main.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define 	Dis_0		0b00111111	//0
#define 	Dis_1		0b00000110	//1
#define 	Dis_2		0b01011011	//2
#define 	Dis_3		0b01001111	//3
#define 	Dis_4		0b01100110	//4
#define 	Dis_5		0b01101101	//5
#define 	Dis_6		0b01111101	//6
#define 	Dis_7		0b00100111	//7
#define 	Dis_8		0b01111111	//8
#define 	Dis_9		0b01101111	//9
#define 	Dis_A		0b01110111	//A
#define 	Dis_b		0b01111100	//b
#define 	Dis_C		0b00111001	//C
#define 	Dis_d		0b01011110	//d
#define 	Dis_E		0b01111001	//E
#define 	Dis_F		0b01110001	//F
#define 	Dis_h		0b01110100	//h
#define 	Dis_H		0b01110110	//H
#define 	Dis_L		0b00111000	//L
#define 	Dis_n		0b01010100	//n
#define 	Dis_O		0b00111111	//O
#define 	Dis_o		0b01011100	//o
#define 	Dis_P       0b01110011	//P
#define 	Dis_r		0b01010000	//r
#define 	Dis_g		0b01101111	//g
#define 	Dis_u		0b00011100	//u
#define 	Dis_S		0b11101101      //S
#define 	Dis_t		0b01111000	//t
#define 	Dis_I		0b00000110	//I
#define 	Dis_d		0b01011110	//d
#define 	Dis_Y		0b01101110	//Y
#define 	Dis_Bar     0b01000000	//-
#define 	Dis_null	0b00000000	//Null
#define 	Dis_minus1	0b01000110
    
typedef union 
{
    uint32_t 	byte;
    struct 
    {
        uint8_t	curhumi_char:		1;      // 현재습도 글자
        uint8_t	curhumi_percent:    1;      // 현재습도 %
        uint8_t	sethumi_char:		1;      // 설정습도 글자
        uint8_t	sethumi_percent:	1;      // 설정습도 %
        uint8_t	time_char:			1;      // 시간 글자
        uint8_t	icon_reserve:		1;      // 예약 아이콘 & 글자
        uint8_t	icon_blow_char:		1;      // 송풍 아이콘 글자
        uint8_t	icon_dehumi_char:	1;      // 제습 아이콘 글자

		uint8_t	icon_blowfan2:		1;      // 송풍 바람개비 2시			
		uint8_t	icon_blowfan3:		1;      // 송풍 바람개비 3시			
		uint8_t	icon_blowfan12:		1;      // 송풍 바람개비 12시			
		uint8_t	icon_blowfandot:	1;      // 송풍 바람개비 점
		uint8_t	icon_dehumi_fan:    1;      // 제습 왼쪽 바람개비			
        uint8_t	icon_dehumi_drop1:	1;      // 제습 물방울 왼쪽
        uint8_t	icon_dehumi_drop2:  1;      // 제습 물방울 오른쪽	 
        uint8_t	icon_emg_oper:  	1;      // 비상운전 아이콘 & 글자

		uint8_t	icon_check:  		1;      // 점검 아이콘 & 글자
        uint8_t	icon_overwater:		1;      // 물넘침 아이콘 & 글자
        uint8_t	icon_comp:          1;      // COMP 아이콘 & 글자 
        uint8_t	dummi1:             1;
        uint8_t	dummi2:             1;			
        uint8_t	dummi3:             1;
        uint8_t	dummi4:             1; 
        uint8_t	dummi5:             1;

        uint8_t	dummi6:             1;			
        uint8_t	dummi7:             1;
        uint8_t	dummi8:             1;			
        uint8_t	dummi9:             1;
        uint8_t	dummi10:            1; 
        uint8_t	dummi11:            1;
        uint8_t	dummi12:            1;			
        uint8_t	dummi13:            1;
        
    } Bit;
} ICON_TYPE;  

typedef union 
{
    uint8_t             byte;
    struct 
    {
        uint8_t	bit_a:              1; //bit0		
        uint8_t	bit_b:              1;
        uint8_t	bit_c:              1;		
        uint8_t	bit_d:              1;

        uint8_t	bit_e:              1;
        uint8_t	bit_f:              1;
        uint8_t	bit_g:				1;
        uint8_t	bit_h:				1;	//bit7

    } Bit;
} SEG_TYPE; 

void Seg_data_set(uint8_t segnum, uint8_t seg_data);
void SEG_UpDate(void);



#ifdef	__cplusplus
}
#endif

#endif	/* SEGMENT_H */

