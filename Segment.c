#include <htc.h>
#include "Segment.h"

ICON_TYPE   icon_led;
SEG_TYPE    seg_led[4] = {0x00,};
SEG_TYPE    simul_led = 0;

const uint16_t digit[] =
{
    Dis_0,   Dis_1,  Dis_2, Dis_3, Dis_4, Dis_5, Dis_6, Dis_7,         //0~7
    Dis_8,   Dis_9,  Dis_A, Dis_b, Dis_C, Dis_d, Dis_E, Dis_F,         //8~15
    Dis_Bar, Dis_n,  Dis_null, Dis_L, Dis_o, Dis_H, Dis_I, Dis_h,       //16~23
    Dis_t,   Dis_r,  Dis_u, Dis_P, Dis_minus1                               //24
};
//===========================================================================================
void Seg_data_set(uint8_t segnum, uint8_t seg_data)			// 0,1,2,3
{
	if(segnum > 3)
		return;
	seg_led[segnum].byte = digit[seg_data];
}

void SEG_UpDate(void) 
{
	//FND1 ��ġ 	
	SEG20COM0 = seg_led[0].Bit.bit_a; 	//a
	SEG20COM2 = seg_led[0].Bit.bit_b; 	//b
	SEG20COM1 = seg_led[0].Bit.bit_c; 	//c
	SEG20COM3 = seg_led[0].Bit.bit_d; 	//d
	SEG21COM1 = seg_led[0].Bit.bit_e; 	//e
	SEG21COM0 = seg_led[0].Bit.bit_f; 	//f
	SEG21COM2 = seg_led[0].Bit.bit_g; 	//g

	//FND2 ��ġ
	SEG37COM0 = seg_led[1].Bit.bit_a; 	//a
	SEG37COM2 = seg_led[1].Bit.bit_b; 	//b
	SEG37COM1 = seg_led[1].Bit.bit_c; 	//c
	SEG37COM3 = seg_led[1].Bit.bit_d; 	//d
	SEG36COM1 = seg_led[1].Bit.bit_e; 	//e
	SEG36COM0 = seg_led[1].Bit.bit_f; 	//f
	SEG36COM2 = seg_led[1].Bit.bit_g; 	//g
			
	//FND3 ��ġ 	
	SEG41COM0 = seg_led[2].Bit.bit_a; 	//a
	SEG41COM2 = seg_led[2].Bit.bit_b; 	//b
	SEG41COM1 = seg_led[2].Bit.bit_c; 	//c
	SEG41COM3 = seg_led[2].Bit.bit_d; 	//d
	SEG40COM1 = seg_led[2].Bit.bit_e; 	//e
	SEG40COM0 = seg_led[2].Bit.bit_f; 	//f
	SEG40COM2 = seg_led[2].Bit.bit_g; 	//g

	//FND4 ��ġ
	SEG33COM0 = seg_led[3].Bit.bit_a; 	//a
	SEG33COM2 = seg_led[3].Bit.bit_b; 	//b
	SEG33COM1 = seg_led[3].Bit.bit_c; 	//c
	SEG33COM3 = seg_led[3].Bit.bit_d; 	//d
	SEG32COM1 = seg_led[3].Bit.bit_e; 	//e
	SEG32COM0 = seg_led[3].Bit.bit_f; 	//f
	SEG32COM2 = seg_led[3].Bit.bit_g; 	//g
			

	// Blow Simulatino Icon 
	SEG39COM0 = simul_led.Bit.bit_a; 	//T1    
	SEG39COM2 = simul_led.Bit.bit_b; 
	SEG39COM1 = simul_led.Bit.bit_c; 
	SEG38COM1 = simul_led.Bit.bit_d; 
	SEG38COM3 = simul_led.Bit.bit_e; 
	SEG39COM3 = simul_led.Bit.bit_f; 
	SEG40COM3 = simul_led.Bit.bit_g; 	//T7

	//DIGIT ICON DATA 	
	SEG36COM3 = icon_led.Bit.curhumi_char;          // ������� ����
	SEG38COM0 = icon_led.Bit.curhumi_percent;       // ������� %
	SEG32COM3 = icon_led.Bit.sethumi_char;          // �������� ����
	SEG34COM0 = icon_led.Bit.sethumi_percent;       // �������� %
	SEG34COM2 = icon_led.Bit.time_char;        		// �ð� ����

	//STATE ICON DATA 		
	SEG5COM2 = icon_led.Bit.icon_reserve;           // ���� ������ & ���� 0
	SEG4COM2 = icon_led.Bit.icon_blow_char;         // ��ǳ ������ ���� 0
	SEG15COM0 = icon_led.Bit.icon_dehumi_char;      // ���� ������ ���� 0

	SEG4COM3 = icon_led.Bit.icon_blowfan12;         // ��ǳ �ٶ����� 12�� 0
	SEG5COM1 = icon_led.Bit.icon_blowfan2;          // ��ǳ �ٶ����� 2�� 0
	SEG5COM3 = icon_led.Bit.icon_blowfan3;          // ��ǳ �ٶ����� 3�� 0
	SEG4COM1 = icon_led.Bit.icon_blowfandot;       // ��ǳ �ٶ����� �� 0
	SEG15COM2 = icon_led.Bit.icon_dehumi_fan;       // ���� ���� �ٶ����� 0
	SEG15COM1 = icon_led.Bit.icon_dehumi_drop1;     // ���� ����� ����	0
	SEG15COM3 = icon_led.Bit.icon_dehumi_drop2;     // ���� ����� ������ 0
	SEG12COM1 = icon_led.Bit.icon_emg_oper;         // ������ ������ & ���� 0
	SEG12COM3 = icon_led.Bit.icon_check;            // ���� ������ & ����  0
	SEG21COM3 = icon_led.Bit.icon_overwater;		// ����ħ ������ & ���� 0
	SEG34COM1 = icon_led.Bit.icon_comp;             // COMP ������ & ���� 0
}


