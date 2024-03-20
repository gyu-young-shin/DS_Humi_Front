#include <htc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
#include "Segment.h"
#include "Buzzer.h"
#include "Dehumi.h"
#include "Eeprom.h"
#include "Testmode.h"


bit         flg_startout = 0;
uint8_t     step_num = 0;
uint8_t     menu_num = 0;
uint8_t     led_num = 0;
uint32_t	iconled_num = 0;
uint8_t     seg_num = 0;
uint32_t    test_timeout;
//===============================================================================================
extern bit          RxOk;                               // IR 수신 데이터 수신 완료 플레그
extern uint8_t      ir_timeout;
extern uint32_t     Rcv_IrData;
extern uint8_t      menu_key[KEY_COUNT];
ICON_TYPE           icon_led;
SEG_TYPE            seg_led[4] = {0x00,};
SEG_TYPE            simul_led = 0;

extern bit	COMM_COMP;
extern bit	COMM_SOL;
extern bit	COMM_EVAFAN;
extern bit	COMM_EVA_HIGHLOW;
//extern bit	COMM_COMPFAN;

extern bit	COMM_WATER_OVER;
extern bit	COMM_HIGH_PRESS;
extern bit	COMM_LOW_PRESS;
extern bit	COMM_REMOTE;
extern bit	COMM_DIP_SW1;
extern bit	COMM_DIP_SW2;
extern bit	COMM_DIP_SW3;
extern bit	COMM_DIP_SW4;

extern int8_t       comm_cur_temper;
extern int8_t       comm_eva_temper;
extern uint8_t      comm_cur_humi;

//===============================================================================================
void Test_Proc(void)
{
	if(menu_key[OPERSTOP_KEY])           // 운전 정지
	{
		AllOff_Output();
		if(flg_startout)
			flg_startout = 0;
		else
		{
			step_num = 0;
			seg_num = 0;
			led_num = 1;
			flg_startout = 1;
		}
		
		Play_Buzzer(1);
		
		menu_key[OPERSTOP_KEY] = 0;
		menu_num = 0;
	}
	else if(menu_key[RESERVE_KEY])			// Room, Eva Temper Disp
	{
		Play_Buzzer(1);
		
		if(flg_startout)
		{
			flg_startout = 0;
			AllOff_Output();
		}
		menu_key[RESERVE_KEY] = 0;
		menu_num = 1;
	}
	else if(menu_key[BLOW_KEY])			// Humi Disp
	{
		Play_Buzzer(1);
		
		if(flg_startout)
		{
			flg_startout = 0;
			AllOff_Output();
		}
		menu_key[BLOW_KEY] = 0;
		menu_num = 2;
	}
	else if(menu_key[DEHUMI_KEY])		// DI Input
	{
		Play_Buzzer(1);
		
		if(flg_startout)
		{
			flg_startout = 0;
			AllOff_Output();
		}
		menu_key[DEHUMI_KEY] = 0;
		menu_num = 3;
	}
	else if(menu_key[STRONG_KEY])	// DI 2 
	{
		Play_Buzzer(1);
		
		if(flg_startout)
		{
			flg_startout = 0;
			AllOff_Output();
		}
		menu_key[STRONG_KEY] = 0;
		menu_num = 4;
	}
	else if(menu_key[WEAK_KEY])			// IR Disp
	{
		Play_Buzzer(1);
		
		if(flg_startout)
		{
			flg_startout = 0;
			AllOff_Output();
		}
		menu_key[WEAK_KEY] = 0;
		menu_num = 5;
	}
	else if(menu_key[UP_KEY] || menu_key[DOWN_KEY])
	{
		Play_Buzzer(1);
		menu_key[UP_KEY] = 0;
        menu_key[DOWN_KEY] = 0;
	}

	switch(menu_num)
	{
		case 0:						// LED & Relay Out
			if(flg_startout)
				DO_Output();
			else
			{
				seg_led[0].byte = Dis_0;
				seg_led[1].byte = Dis_u;
				seg_led[2].byte = Dis_t;
				seg_led[3].byte = Dis_null;
			}
			break;
		case 1:						// Disp Temperature 
			Disp_Temper();			// 왼쪽에 표시
			Disp_TestEvaTemper();		// 오른쪽에 표시
            icon_led.Bit.curhumi_char = 0;
            icon_led.Bit.curhumi_percent = 0;
			break;
		case 2:						// 습도표시
			Disp_Humidity();		// 왼쪽에 표시
            icon_led.Bit.curhumi_char = 1;
            seg_led[2].byte = Dis_null;
            seg_led[3].byte = Dis_null;
			break;
		case 3:						// DI Disp
			Disp_DI1();
            icon_led.Bit.curhumi_char = 0;
            icon_led.Bit.curhumi_percent = 0;
			break;
		case 4:						// DI Disp
			Disp_DI2();
            icon_led.Bit.curhumi_char = 0;
            icon_led.Bit.curhumi_percent = 0;
			break;
		case 5:						// Ir Disp
			Disp_Ir();
            icon_led.Bit.curhumi_char = 0;
            icon_led.Bit.curhumi_percent = 0;
			break;
		default:
			break;
	}
}
//-----------------------------------------------------------
void DO_Output(void)
{
	switch(step_num)
	{
		case 0:
			if(test_timeout == 0)
			{
				seg_led[seg_num].byte |= led_num;		// Segment 4개
				if(led_num == 0x40)
				{
                    seg_num++;
                    if(seg_num >= 4)
                        step_num++;
					led_num = 1;
				}
				else
					led_num <<= 1;
				test_timeout = 10;			// 200ms
			}
			break;
		case 1:
			if(test_timeout == 0)
			{
				simul_led.byte |= led_num;		// wind simulation led
				if(led_num == 0x40)
				{
                    step_num++;
					led_num = 1;
                    iconled_num = 1;
				}
				else
					led_num <<= 1;
				test_timeout = 10;			// 200ms
			}
			break;
		case 2:
			if(test_timeout == 0)
			{
				icon_led.byte |= iconled_num;		// Icon Led
				if(iconled_num == 0x040000)
				{
					step_num = 3;
					iconled_num = 1;
				}
				else
					iconled_num <<= 1;
				test_timeout = 10;
			}
			break;
		case 3:								// 송풍 LED
			if(test_timeout == 0)
			{
				switch(led_num)
				{
					case 1:
						COMM_COMP = 1;
						break;
					case 2:
						COMM_SOL = 1;
						break;
					case 3:
                        COMM_EVA_HIGHLOW = 0;
						COMM_EVAFAN = 1;
						break;
					case 4:
                        COMM_EVA_HIGHLOW = 1;
						COMM_EVAFAN = 1;
						break;
				}
                
				if(led_num == 4)
				{
					step_num = 4;
					led_num = 1;
				}
				else
				{
					led_num++;
				}
				test_timeout = 100;
			}
			break;
		default:
			break;
	}
}

void AllOff_Output(void)
{
	uint8_t i;
	
	for(i=0; i < 4; i++)		// All Sement and led Off
		seg_led[i].byte = 0;

    icon_led.byte = 0;
    simul_led.byte = 0;
    seg_num = 0;
    led_num = 1;
    iconled_num = 0;
    
    COMM_COMP = 0;
    COMM_SOL = 0;
    COMM_EVAFAN = 0;
    COMM_EVA_HIGHLOW = 0;
}

void Disp_Ir(void)
{
    uint8_t temp;
	
    seg_led[0].byte = Dis_1;
    seg_led[1].byte = Dis_r;
	
    if(RxOk)
    {
        if(ir_timeout == 0)
        {
            Play_Buzzer(1);
			
			temp = (Rcv_IrData & 0xFF) / 16;
			Seg_data_set(2, temp);

            temp = (Rcv_IrData & 0xFF) % 16;
			Seg_data_set(3, temp);

            ir_timeout = 20;
        }
        RxOk = 0;
    }
}
// 에바온도 표시
void Disp_TestEvaTemper(void)
{
    uint8_t temp;
    uint8_t flg_minus;
    int tmp_var;
    
    if(comm_eva_temper >= 70)
    {
        Seg_data_set(2, 21);            // H
        Seg_data_set(3, 22);            // i and dot
    }
    else if(comm_eva_temper <= -19)
    {
        Seg_data_set(2, 19);            // L
        Seg_data_set(3, 20);            // o and dot
    }
    else
    {
        flg_minus = 0;
        if(comm_eva_temper < 0)
        {
            tmp_var = comm_eva_temper * -1;
            flg_minus = 1;
        }
        else
            tmp_var = comm_eva_temper;
        
        temp = tmp_var / 10;
        if(flg_minus)
        {
          if(temp == 0)
              Seg_data_set(2, 16);            // Bar
          else
              Seg_data_set(2, 28);            // Bar
        }
        else
        {
          if(temp == 0)
              Seg_data_set(2, 18);            // Blank
          else
              Seg_data_set(2, temp);
        }
        
        temp = tmp_var % 10;
        Seg_data_set(3, temp);
    }
}
//----------------------------------------------
void Disp_DI1(void)
{
	if(COMM_HIGH_PRESS)
		Seg_data_set(0, 1);
    else
        Seg_data_set(0, 0);

	if(COMM_LOW_PRESS)
		Seg_data_set(1, 1);
    else
        Seg_data_set(1, 0);

	if(COMM_WATER_OVER)
		Seg_data_set(2, 1);
    else
        Seg_data_set(2, 0);

	if(COMM_REMOTE)
		Seg_data_set(3, 1);
    else
        Seg_data_set(3, 0);
}

void Disp_DI2(void)
{
	if(COMM_DIP_SW1)
		Seg_data_set(0, 1);
    else
        Seg_data_set(0, 0);

	if(COMM_DIP_SW2)
		Seg_data_set(1, 1);
    else
        Seg_data_set(1, 0);

	if(COMM_DIP_SW3)
		Seg_data_set(2, 1);
    else
        Seg_data_set(2, 0);

	if(COMM_DIP_SW4)
		Seg_data_set(3, 1);
    else
        Seg_data_set(3, 0);
}
