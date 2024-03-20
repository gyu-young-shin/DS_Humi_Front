#include <htc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
#include "Segment.h"
#include "Buzzer.h"
#include "Dehumi.h"
#include "Eeprom.h"

//===========================================================================================
bit	COMM_COMP;
bit	COMM_SOL;
bit	COMM_EVAFAN;
bit	COMM_EVA_HIGHLOW;
bit	COMM_COMPFAN;

bit	COMM_WATER_OVER;
bit	COMM_HIGH_PRESS;
bit	COMM_LOW_PRESS;
bit	COMM_REMOTE;
bit	COMM_DIP_SW1;
bit	COMM_DIP_SW2;
bit	COMM_DIP_SW3;
bit	COMM_DIP_SW4;

bit     RxOk = 0;                               // IR ���� ������ ���� �Ϸ� �÷���
bit     NowIrIn;                                 // IR ������Ʈ  ���� ��Ʈ ����
bit     flg_e2p_save = 0;
//bit     flg_blowfan_on = 0;			// COMP FAN
//bit	flg_blowfan_highlow = 0;            // EVA FAN �� ��
bit  	flg_waterchk = 0;
bit 	waterlevel_sensor_error = 0;
bit 	flg_highpresurechk = 0;
bit 	flg_lowpresurechk = 0;
bit 	flg_eva_freez_chk = 0;
bit 	eva_freez_detect = 0;
bit 	flg_evafan_on = 0;
bit 	flg_solvalve_on = 0;
bit 	evafan_mode = 0;						// ��ǳ�� ��� 1:�� 0:��
bit 	flg_comp_on = 0;
//bit 	highpress_error = 0;
bit 	highpress_detect = 0;
bit 	lowpress_error = 0;
bit 	evatemp_sensor_error = 0;
bit 	humi_sensor_error = 0;
bit 	roomtemp_sensor_error = 0;
bit     flg_evatemp_chk = 0;
bit 	flg_option_start = 0;
bit 	flg_keyall_off = 0;
bit 	flg_cont_proc = 0;
bit     flg_force_comp_on = 0;
bit     flg_emergency_oper = 0;
bit 	flg_onreserve = 0;
bit 	flg_offreserve = 0;
bit 	flg_blink = 0;
bit 	evafreez_error = 0;
bit 	flg_remote_controle = 0;			// Dip S/W 2		�������� ���� : OFF(����), ON(����) / �ʱ⼳�� OFF
bit 	flg_evafreez_detect_first = 0;
bit     flg_eva_error_first = 0;
bit 	flg_overwater_first = 0;
bit 	flg_evatemp_first = 0;
bit 	flg_roomhumi_first = 0;
bit 	flg_roomtemp_first = 0;
bit 	flg_short_circuit = 0;
bit    flg_testmode = 0;
bit    flg_highpress_detect_first = 0;
bit    flg_lowpress_detect_first = 0;
//bit    flg_roomtemp_first = 0;

int8_t      comm_cur_temper = 25;
int8_t      comm_out_temper = 25;
int8_t      comm_eva_temper = 25;
uint8_t     comm_cur_humi = 25;
uint8_t      set_cur_humi = 25;

uint8_t     blow_level = 0;
uint8_t     level_timeout = 0;
uint8_t 	water_timeout = 0;
uint8_t 	highp_timeout = 0;
uint8_t 	lowp_timeout = 0;
uint8_t 	eva_detect_timeout = 0;
uint8_t 	opt_num = 1;
uint8_t 	key_timeout = 0;
uint8_t 	optkey_timeout = 0;
uint8_t 	prev_disp_mode = DISP_WAIT;
uint8_t 	onreserve_settime = 0;
uint8_t 	offreserve_settime = 0;
uint8_t 	keycont_timeout = 0;
uint8_t 	op_kind = DEHUMI;
uint8_t 	key_updown_timeout = 0;
uint8_t 	evatemp_timeout = 0;
uint8_t 	op_mode = STOP_MODE;
uint8_t 	disp_mode = DISP_WAIT;
uint8_t 	e2p_save_timeout = 0;
uint8_t 	warning_timeout = 0;
uint8_t 	fan_step = 0;
uint8_t 	ani_timeout = 0;
uint8_t 	dehumi_step = 0;
uint8_t 	ani_dehumi_timeout = 0;
uint8_t 	curtemp_error_timeout = 0;

uint16_t 	compdelay_timeout = 180;
uint16_t 	onreserve_timeout = 0;
uint16_t 	offreserve_timeout = 0;
uint16_t 	temp16_var = 0;
uint16_t 	error_code = 0;
uint16_t 	evafan_offtime = 0;
uint16_t 	evadelay_timeout = 0;
uint16_t 	waterdelay_timeout = 0;
uint16_t 	humidity_timeout = 0;
uint16_t 	roomtemp_timeout = 0;
uint16_t 	eva_error_timeout = 0;
uint16_t 	high_press_timeout = 0;
uint16_t 	low_press_timeout = 0;

uint32_t    IrRxBuf = 0;               // IR ���� ������(����Ʈ) ����
uint32_t    Irmask;
uint32_t    Rcv_IrData = 0;
uint8_t     IrStep_Num = 0;
uint16_t    bitcount = 0;
uint8_t     ir_timeout = 0;


//===========================================================================================
extern bit         flg_485_send;
extern bit         urx2_comp;
extern bit         tx2_restart;
extern bit         flg_toggle_500ms;

extern uint8_t  menu_key[KEY_COUNT];
extern uint8_t  key_pushed[KEY_COUNT];
extern uint8_t  key_continued[KEY_COUNT];
extern SEG_TYPE    simul_led;
extern ICON_TYPE   icon_led;
extern SEG_TYPE    seg_led[4];

extern TE2PDataRec Flashdatarec;
//===========================================================================================
void IR_Receive_Proc(void)
{
    if(flg_remote_controle)		// �������� 
    {
        if(COMM_REMOTE)			// �ܺ� ����ġ Open
        {
            RxOk = 0;
            return;
        }
    }

    if(RxOk)
    {
        if(ir_timeout == 0)
        {
            switch(Rcv_IrData)
            {
                case IR_DOWN:					// ����
                    menu_key[DOWN_KEY] = 1;
                    break;
                case IR_UP:					// �ø�
                    menu_key[UP_KEY] = 1;
                    break;
                case IR_PWR:					// ���� ����
                    menu_key[OPERSTOP_KEY] = 1;
                    break;
                case IR_WIND:					// ��ǳ
                    menu_key[BLOW_KEY] = 1;
                    break;
                case IR_TURBO:					// ��/��
                    if(evafan_mode)             // ���̸�
                        menu_key[WEAK_KEY] = 1;
                    else
                        menu_key[STRONG_KEY] = 1;
                    break;
                case IR_RESERVE:				// ����
                    menu_key[RESERVE_KEY] = 1;
                    break;
                case IR_DEHUMI:				// ����
                   	menu_key[DEHUMI_KEY] = 1;
                    break;
                default:
                    break;
            }
            ir_timeout = 20;
        }
        RxOk = 0;
    }
}
//------------------------------------------------------------------------------------
// �Է� Check ó��
void InputCheck_Proc(void)
{
	//----------------------------------------------------   ������(����ħ) ���� Check
	if(COMM_WATER_OVER && COMM_DIP_SW3)				// level ���� dip s/w off ��
	{
		if(flg_waterchk == 0)
		{
			water_timeout = 30;				// 3sec
			flg_waterchk = 1;
		}
		else
		{
			if(water_timeout == 0)
				waterlevel_sensor_error = 1;
		}
	}
	else
	{
		water_timeout = 0;
		flg_waterchk = 0;
		waterlevel_sensor_error = 0;
	}
	//----------------------------------------------------   ��а��� ����
	if(COMM_HIGH_PRESS && (op_mode == RUN_MODE) && (op_kind == DEHUMI))										// level ����
	{
		if(flg_highpresurechk == 0)
		{
            if(flg_short_circuit)
                highp_timeout = 1;								// 60 sec
            else
                highp_timeout = 60;								// 60 sec
			flg_highpresurechk = 1;
		}
		else
		{
			if(highp_timeout == 0)
				highpress_detect = 1;
		}
	}
	else
	{
		highp_timeout = 0;
		flg_highpresurechk = 0;
		highpress_detect = 0;
	}
	//----------------------------------------------------   ���а��� ����
	if(COMM_LOW_PRESS && (op_mode == RUN_MODE) && (op_kind == DEHUMI) && Flashdatarec.e2p_E7_check)										// level ����
	{
		if(flg_lowpresurechk == 0)
		{
            if(flg_short_circuit)
                lowp_timeout = 2;   							// ���а������� �ð� 120��
            else
                lowp_timeout = 120;   							// ���а������� �ð� 120��
			flg_lowpresurechk = 1;
		}
		else
		{
			if(lowp_timeout == 0)
				lowpress_error = 1;
		}
	}
	else
	{
		lowp_timeout = 0;
		flg_lowpresurechk = 0;
		lowpress_error = 0;
	}
    
	//----------------------------------------------------   ���� �µ� ����
	if(((comm_eva_temper <= -20) || (comm_eva_temper >= 70)) && (op_mode == RUN_MODE) && (op_kind == DEHUMI))// -20 �� ����
	{
		if(flg_evatemp_chk == 0)
		{
            if(flg_short_circuit)
                evatemp_timeout = 1;
            else
                evatemp_timeout = 30;
			flg_evatemp_chk = 1;
		}
		else
		{
			if(evatemp_timeout == 0)
				evatemp_sensor_error = 1;
		}
	}
	else
	{
		evatemp_sensor_error = 0;
		flg_evatemp_chk = 0;
	}
	//----------------------------------------------------   ���ٵ��� ����
	if((comm_eva_temper <= Flashdatarec.e2p_jesang_temp) && (evatemp_sensor_error == 0) && (op_mode == RUN_MODE) && (op_kind == DEHUMI))		// ���� ���� �µ� �� �ʱ� -1
	{
		if(flg_eva_freez_chk == 0)
		{
			flg_eva_freez_chk = 1;
			eva_detect_timeout = 35;							// 3 ��
		}
		else
		{
			if(eva_detect_timeout == 0)
				eva_freez_detect = 1;						// ���� ���� ����
		}
	}
	else
	{
		flg_eva_freez_chk = 0;
	}
	//----------------------------------------------------   �� ���� ���� ����
	if(comm_cur_humi <= 0)                                  // 0%���� �Ǵ� 100% �̻�    2021-07-21 ����� ���� ����
		humi_sensor_error = 1;
	else
		humi_sensor_error = 0;
	//----------------------------------------------------   �� �µ� ���� ����
	if(comm_cur_temper <= -19) // -15 �� ����
	{
		comm_cur_temper = -19;
        if(Flashdatarec.e2p_senuse_onoff)
            roomtemp_sensor_error = 1;
        else
            roomtemp_sensor_error = 0;
	}
	else if(comm_cur_temper >= 80)  // 80�� ��				// �µ� ������ Check �Ѵ�.
	{
		comm_cur_temper = 80;
        if(Flashdatarec.e2p_senuse_onoff)
            roomtemp_sensor_error = 1;
        else
            roomtemp_sensor_error = 0;
	}
	else
		roomtemp_sensor_error = 0;
}

//------------------------------------------------------------------------------------
// ��� ó��
void Output_Proc(void)
{
    if(flg_evafan_on || flg_force_comp_on)                                   // ��ǳ�� ON
    {
        if(evafan_mode)					// ��ǳ�� ��� 1:�� 0:��
            COMM_EVA_HIGHLOW = 1;
        else
            COMM_EVA_HIGHLOW = 0;

        COMM_EVAFAN = 1;
    }
    else
    {
        COMM_EVAFAN = 0;
        COMM_EVA_HIGHLOW = 0;
    }

    if(flg_solvalve_on)
        COMM_SOL = 1;
    else
        COMM_SOL = 0;


    if(flg_force_comp_on)					// ���� ���� ���
    {
        COMM_COMP = 1;
    }
    else if(flg_comp_on)					// ���� ON
    {
        if(compdelay_timeout == 0)                  // ���� �����ð�
            COMM_COMP = 1;
    }
    else
    {
        if(COMM_COMP)
        {
            COMM_COMP = 0;
            if(flg_short_circuit)
                compdelay_timeout = 3;
            else
                compdelay_timeout = 180;			
        }
    }
/*   
    if(flg_blowfan_on)                                          // comp fan
        COMM_COMPFAN = 1;
    else
        COMM_COMPFAN = 0;
*/
    //------------------------------------------------
    if(COMM_COMP)
        icon_led.Bit.icon_comp = 1;                     // COMP LED
    else
    {
        if(flg_comp_on && (compdelay_timeout > 0))
        {
            if(flg_toggle_500ms)
                icon_led.Bit.icon_comp = 1;
            else
                icon_led.Bit.icon_comp = 0;
        }
        else
            icon_led.Bit.icon_comp = 0;
    }
    //------------------------------------------------
    if(flg_emergency_oper)
    {
        icon_led.Bit.icon_emg_oper = 1;
    }
    else
    {
        icon_led.Bit.icon_emg_oper = 0;
    }
    //------------------------------------------------
}
//-------------------------------------------------------------------------------------------
void Animation_Dehumi(void)
{
    if((op_mode == RUN_MODE) && (op_kind == DEHUMI) && (!((comm_cur_temper <= -15) && (Flashdatarec.e2p_senuse_onoff))))
    {
        icon_led.Bit.icon_dehumi_char = 1;
        icon_led.Bit.icon_dehumi_fan = 1;
        switch(dehumi_step)
        {
            case 0:
                if(ani_dehumi_timeout == 0)
                {
                    icon_led.Bit.icon_dehumi_drop1 = 1;
                    icon_led.Bit.icon_dehumi_drop2 = 0;
                    ani_dehumi_timeout = 50;
                    dehumi_step = 1;
                }
                break;
            case 1:
                if(ani_dehumi_timeout == 0)
                {
                    icon_led.Bit.icon_dehumi_drop1 = 1;
                    icon_led.Bit.icon_dehumi_drop2 = 1;
                    ani_dehumi_timeout =60;
                    dehumi_step = 2;
                }
                break;
            case 2:
                if(ani_dehumi_timeout == 0)
                {
                    icon_led.Bit.icon_dehumi_drop1 = 0;
                    icon_led.Bit.icon_dehumi_drop2 = 0;
                    ani_dehumi_timeout =50;
                    dehumi_step = 0;
                }
                break;
        }
    }
    else
    {
        icon_led.Bit.icon_dehumi_char = 0;        
        icon_led.Bit.icon_dehumi_drop1 = 0;
        icon_led.Bit.icon_dehumi_drop2 = 0;
        icon_led.Bit.icon_dehumi_fan = 0;
    }
}
//-------------------------------------------------------------------------------------------
void Animation_Blowfan(void)
{
    if(flg_evafan_on || flg_force_comp_on)
    {
        if(flg_short_circuit)
        {
            if(flg_toggle_500ms)
                icon_led.Bit.icon_blow_char = 1;
            else
                icon_led.Bit.icon_blow_char = 0;
        }
        else
            icon_led.Bit.icon_blow_char = 1;
        
        icon_led.Bit.icon_blowfandot = 1;
        
        switch(fan_step)
        {
            case 0:
                if(ani_timeout == 0)
                {
                    icon_led.Bit.icon_blowfan12 = 0;
                    icon_led.Bit.icon_blowfan2 = 1;
                    icon_led.Bit.icon_blowfan3 = 1;

                    if(evafan_mode)
                        ani_timeout = 8;
                    else
                        ani_timeout = 13;
                    fan_step = 1;
                }
                break;
            case 1:
                if(ani_timeout == 0)
                {
                    icon_led.Bit.icon_blowfan12 = 1;
                    icon_led.Bit.icon_blowfan2 = 0;
                    icon_led.Bit.icon_blowfan3 = 1;

                    if(evafan_mode)
                        ani_timeout = 8;
                    else
                        ani_timeout = 13;
                    fan_step = 2;
                }
                break;
            case 2:
                if(ani_timeout == 0)
                {
                    icon_led.Bit.icon_blowfan12 = 1;
                    icon_led.Bit.icon_blowfan2 = 1;
                    icon_led.Bit.icon_blowfan3 = 0;

                    if(evafan_mode)
                        ani_timeout = 8;
                    else
                        ani_timeout = 13;
                    fan_step = 0;
                }
                break;
        }
    }
    else
    {
        if(flg_short_circuit)
        {
            if(flg_toggle_500ms)
                icon_led.Bit.icon_blow_char = 1;
            else
                icon_led.Bit.icon_blow_char = 0;
        }
        else
            icon_led.Bit.icon_blow_char = 0;
        
        icon_led.Bit.icon_blowfandot = 0;
        icon_led.Bit.icon_blowfan12 = 0;
        icon_led.Bit.icon_blowfan2 = 0;
        icon_led.Bit.icon_blowfan3 = 0;
        fan_step = 0;
        ani_timeout = 0;
    }
}
//-------------------------------------------------------------------------------------------
void Disp_OnOffReservTime(uint8_t onoff)					// ����/ ���� ���� �ð� ǥ��
{
	uint8_t hour;
	
	if(onoff)
		temp16_var = onreserve_timeout;						// �д��� 
	else
		temp16_var = offreserve_timeout;
		
	hour = (temp16_var / 60);
    if(temp16_var % 60)
        hour++;
    
    if(hour > 10)
        hour = 10;
	if(hour >= 10)
        Seg_data_set(2, 1);
    else
        Seg_data_set(2, 18);
    
    hour %= 10;
    Seg_data_set(3, hour);
    
    icon_led.Bit.time_char = 1;
}
//----------------------------------------------------------------------------------
void Error_Disp(void)
{
    icon_led.Bit.curhumi_percent = 0;
    icon_led.Bit.curhumi_char = 0;
    
    icon_led.Bit.sethumi_percent = 0;
    icon_led.Bit.sethumi_char = 0;
    icon_led.Bit.time_char = 0;

    Seg_data_set(0, 14);                            // E
    if(flg_toggle_500ms)
        icon_led.Bit.icon_check = 1;                // ���� LED
    else
        icon_led.Bit.icon_check = 0;

    if(error_code & ERR_HIGHPRESURE)        // ��а��� ����
    {
        Seg_data_set(1, 6);            	// 6
        Seg_data_set(2, 21);           	// H
        Seg_data_set(3, 27);            // P
    }
    else if(error_code & ERR_LOWPRESURE)	// ���а��� ����
    {
        Seg_data_set(1, 7);            	// 7
        Seg_data_set(2, 19);           	// L
        Seg_data_set(3, 27);            // P
    }
    else if(error_code & ERR_ROOMHUMI)		// ���� ���� ����
    {
        Seg_data_set(1, 1);            	// 1
        Seg_data_set(2, 5);            	// S
        Seg_data_set(3, 21);            // H
    }
    else if(error_code & ERR_ROOMTEMPER)	// �ǳ��µ� ����
    {
        Seg_data_set(1, 2);            	// 2
        Seg_data_set(2, 5);            	// S
        Seg_data_set(3, 24);            // t
    }
    else if(error_code & ERR_EVATEMPER)		// ���� ���� ����
    {
        Seg_data_set(1, 3);            	// 3
        Seg_data_set(2, 5);            	// S
        Seg_data_set(3, 14);            // E
    }
    else if(error_code & ERR_EVAFREEZ)		// ����(���߱�)���� ����
    {
        Seg_data_set(1, 16);            // '-'
        Seg_data_set(2, 14);            // E
        Seg_data_set(3, 15);            // F
    }
    else if(error_code & ERR_OVERWATER)		// ����ħ ����
    {
        Seg_data_set(1, 4);            	// 4
        Seg_data_set(2, 15);           	// F
        Seg_data_set(3, 26);            // u
        icon_led.Bit.icon_overwater = 1;
    }
    else if(error_code & ERR_COMM485)		// RS485 ��� ����
    {
        Seg_data_set(1, 8);            	// 8
        Seg_data_set(2, 12);           	// C
        Seg_data_set(3, 12);            // C
    }
}
//------------------------------------------------------------------------------------
void Error_Recovery_Proc(void)
{
    if(error_code & ERR_OVERWATER)				// ����ħ ����
    {
        if(!COMM_WATER_OVER)					// level ����	 Close
        {
            error_code &= ~ERR_OVERWATER;
            icon_led.Bit.icon_overwater = 0;
        }
    }

    if(error_code & ERR_HIGHPRESURE)			// ��а��� ����
    {
        if(COMM_HIGH_PRESS == 0)				// Close
        {
            error_code &= ~ERR_HIGHPRESURE;
            //highpress_error = 0;
        }
    }
	
    if(error_code & ERR_LOWPRESURE)				// ���а��� ����
    {
        if(COMM_LOW_PRESS == 0)					// Close
        {
            error_code &= ~ERR_LOWPRESURE;
            lowpress_error = 0;
        }
    }

    if(error_code & ERR_EVAFREEZ)				// ����(���߱�)���� ����
    {
        if(comm_eva_temper >= Flashdatarec.e2p_jesang_reltemp)	// ���ٵ��� ���� �µ� �̻��̸�
        {
            error_code &= ~ERR_EVAFREEZ;
            evafreez_error = 0;
        }
    }

    if(error_code & ERR_EVATEMPER)			// ���� ���� ����
    {
        if(evatemp_sensor_error == 0)		// ���ٿµ� ���� ������ �����Ǹ�
            error_code &= ~ERR_EVATEMPER;
    }

    if(error_code & ERR_ROOMHUMI)			// �� ���� ���� ����
    {
        if(humi_sensor_error == 0)		// �� ���� ���� ������ ���� �Ǹ�
            error_code &= ~ERR_ROOMHUMI;
    }

    if(error_code & ERR_ROOMTEMPER)			// �ǳ��µ� ���� ����
    {
        if(roomtemp_sensor_error == 0)		// ��µ� ���� ������ ���� �Ǹ�
            error_code &= ~ERR_ROOMTEMPER;
    }
}
//----------------------------------------------------------------------------------
// 7 Segment & LED Display
void Disp_Segment(void)
{
    uint8_t flg_minus;
    int8_t temp;
    int tmp_var;

    if((op_mode == STOP_MODE) && (disp_mode == DISP_WAIT) && (flg_evafan_on == 0))
        BACKLIGHT = 0;
    else
        BACKLIGHT = 1;
    
    
    if(disp_mode == DISP_OPTION)
    {
        Seg_data_set(0, 15);           					// 'F' ǥ��
        Seg_data_set(1, opt_num);

        icon_led.Bit.curhumi_percent = 0;
        icon_led.Bit.curhumi_char = 0;

        icon_led.Bit.sethumi_percent = 0;
        icon_led.Bit.sethumi_char = 0;
        icon_led.Bit.time_char = 0;
        
        switch(opt_num)
        {
            case 1:										// ������ ���� (-9% - +9%)
                flg_minus = 0;
                if(Flashdatarec.e2p_dehumi_cali < 0)
                {
                    tmp_var = Flashdatarec.e2p_dehumi_cali * -1;
                    flg_minus = 1;
                }
                else
                    tmp_var = Flashdatarec.e2p_dehumi_cali;

                if(flg_minus)
                    Seg_data_set(2, 16);            	// Bar
                else
                    Seg_data_set(2, 18);

                Seg_data_set(3, tmp_var);
                break;
            case 2:										// �������� ��� ����
                if(Flashdatarec.e2p_senuse_onoff)
                {
                    Seg_data_set(2, 0);            		// '0'
                    Seg_data_set(3, 17);                	// 'n'
                }
                else
                {
                    Seg_data_set(2, 0);            		// '0'
                    Seg_data_set(3, 15);                	// 'F'
                }
                break;
            case 3:										// ������ ���� �µ�����. (�������Կµ� �ʱⰪ -1��), (���� : -5�� ~ 7��)
                flg_minus = 0;
                if(Flashdatarec.e2p_jesang_temp < 0)
                {
                    tmp_var = Flashdatarec.e2p_jesang_temp * -1;
                    flg_minus = 1;
                }
                else
                    tmp_var = Flashdatarec.e2p_jesang_temp;

                temp = tmp_var / 10;
                if(flg_minus)
                {
                    if(temp == 0)
                        Seg_data_set(2, 16);                        // Bar
                    else
                        Seg_data_set(2, 28);                        // Bar
                }
                else
                {
                    if(temp == 0)
                        Seg_data_set(2, 18);                        // Blank
                    else
                        Seg_data_set(2, temp);
                }

                temp = tmp_var % 10;
                Seg_data_set(3, temp);
                break;
            case 4:										 // FAN OFF ��������
                if(Flashdatarec.e2p_fanoff_delay)	
                {
                    Seg_data_set(2, 0);            		// '0'
                    Seg_data_set(3, 17);                	// 'n'
                }
                else
                {
                    Seg_data_set(2, 0);            		// '0'
                    Seg_data_set(3, 15);                    // 'F'
                }
                break;
            case 5:										//  FAN OFF ���� �ð� ����
                temp = Flashdatarec.e2p_fandelay_time / 10;
                if(temp == 0)
                    Seg_data_set(2, 18);            // Blank
                else
                    Seg_data_set(2, temp);

                temp = Flashdatarec.e2p_fandelay_time % 10;
                Seg_data_set(3, temp);
                break;
            case 6:										//   �ø����� E7 ���� ���
                if(Flashdatarec.e2p_E7_check)	
                {
                    Seg_data_set(2, 0);            		// '0'
                    Seg_data_set(3, 17);                    // 'n'
                }
                else
                {
                    Seg_data_set(2, 0);            		// '0'
                    Seg_data_set(3, 15);                    // 'F'
                }
                break;
            case 7:										// ���� ���� �µ�
                temp = Flashdatarec.e2p_jesang_reltemp / 10;
                if(temp == 0)
                    Seg_data_set(2, 18);                    // Blank
                else
                    Seg_data_set(2, temp);

                temp = Flashdatarec.e2p_jesang_reltemp % 10;
                Seg_data_set(3, temp);
                break;
            case 8:										// ���� ���� �ð�
                temp = Flashdatarec.e2p_jesang_reltime / 10;
                if(temp == 0)
                    Seg_data_set(2, 18);                    // Blank
                else
                    Seg_data_set(2, temp);

                temp = Flashdatarec.e2p_jesang_reltime % 10;
                Seg_data_set(3, temp);
                break;
            default:
                break;
        }
        return;
    }
    //----------------------------------------------------------------------------------------------
    if(disp_mode == DISP_ROOM_TEMP)
    {
        if(key_pushed[STRONG_KEY] && key_pushed[WEAK_KEY])
        {
            icon_led.Bit.curhumi_percent = 0;
            icon_led.Bit.curhumi_char = 0;

            Disp_Temper();
        }
        else
        {
            disp_mode = prev_disp_mode;
        }
    }
    else if(disp_mode == DISP_EVAROOM_TEMP)
    {
        if(key_pushed[DEHUMI_KEY] && key_pushed[DOWN_KEY])
        {
            icon_led.Bit.curhumi_percent = 0;
            icon_led.Bit.curhumi_char = 0;

            Disp_EvaTemper();
        }
        else
        {
            disp_mode = prev_disp_mode;
        }
    }
    else if(error_code > 0)                             // Error ǥ��
    {
        Error_Disp();
    }
    else
    {
        icon_led.Bit.icon_check = 0;                    // ���� LED

        if(flg_offreserve || flg_onreserve)				// ����, ���� ����
            icon_led.Bit.icon_reserve = 1;              // ���� LED
        else
            icon_led.Bit.icon_reserve = 0;                      

        switch(disp_mode)
        {
            case DISP_WAIT:					  // Stop ���
                if((comm_cur_temper <= 15) && (Flashdatarec.e2p_senuse_onoff))
                {
                    Seg_data_set(0, 19);            // L
                    Seg_data_set(1, 20);            // o and dot
                    icon_led.Bit.curhumi_char = 0;
                    icon_led.Bit.curhumi_percent = 0;
                }
                else
                {
                    Disp_Humidity();
                    icon_led.Bit.curhumi_char = 1;
                }
                
                if(flg_onreserve)					  // ���� ������ ����
                    Disp_OnOffReservTime(1);                // ���� ���� �ð� ǥ��
                else
                {
                    Seg_data_set(2, 18);
                    Seg_data_set(3, 18);
                    icon_led.Bit.time_char = 0;
                }
                icon_led.Bit.sethumi_char = 0;
                icon_led.Bit.sethumi_percent = 0;
                break;
            case DISP_OPERATION:                       		// ����/��ǳ ����
                if((comm_cur_temper <= 15) && (Flashdatarec.e2p_senuse_onoff))
                {
                    Seg_data_set(0, 19);            // L
                    Seg_data_set(1, 20);            // o and dot
                    icon_led.Bit.curhumi_char = 0;
                    icon_led.Bit.curhumi_percent = 0;
                }
                else
                {
                    Disp_Humidity();
                    icon_led.Bit.curhumi_char = 1;
                }

                if(eva_freez_detect)
                {
                    Seg_data_set(2, 14);            	// E
                    Seg_data_set(3, 15);            	// F

                    icon_led.Bit.icon_check = 0; 
                    icon_led.Bit.sethumi_char = 0;
                    icon_led.Bit.sethumi_percent = 0;
                    icon_led.Bit.time_char = 0;
                }
                else if(flg_offreserve)							// ���� ������ ����
                {
                    Disp_OnOffReservTime(0);				// ���� ���� �ð� ǥ��
                    icon_led.Bit.sethumi_char = 0;
                    icon_led.Bit.sethumi_percent = 0;
                }
                else
                {
                    switch(op_kind)
                    {
                        case DEHUMI:							// ����
                            temp = set_cur_humi / 10;

                            if(temp == 0)
                                Seg_data_set(2, 18);
                            else
                                Seg_data_set(2, temp);

                            Seg_data_set(3, (set_cur_humi % 10));
                            icon_led.Bit.sethumi_char = 1;
                            
                            if(flg_remote_controle)
                            {
                                if(flg_toggle_500ms)
                                    icon_led.Bit.sethumi_percent = 1;
                                else
                                    icon_led.Bit.sethumi_percent = 0;
                            }
                            else
                                icon_led.Bit.sethumi_percent = 1;
                            break;
                        case WIND:								// ��ǳ
                            Seg_data_set(2, 18);
                            Seg_data_set(3, 18);
                            icon_led.Bit.sethumi_char = 0;
                            icon_led.Bit.sethumi_percent = 0;
                            break;
                        default:
                            break;
                    }
                    icon_led.Bit.time_char = 0;
                }
                break;
            case DISP_DEHUMI_SET:                               // �����µ�����
                Disp_Humidity();
                icon_led.Bit.sethumi_char = 1;
                
                if(flg_remote_controle)
                {
                    if(flg_toggle_500ms)
                        icon_led.Bit.sethumi_percent = 1;
                    else
                        icon_led.Bit.sethumi_percent = 0;
                }
                else
                    icon_led.Bit.sethumi_percent = 1;
                
                icon_led.Bit.time_char = 0;

                if((flg_blink == 0) || (flg_blink && flg_toggle_500ms))
                {
                    temp = set_cur_humi / 10;

                    if(temp == 0)
                        Seg_data_set(2, 18);
                    else
                        Seg_data_set(2, temp);

                    Seg_data_set(3, (set_cur_humi % 10));
                }
                else
                {
                    Seg_data_set(2, 18);
                    Seg_data_set(3, 18);
                }
                break;				
            case DISP_ON_RESERVE:                                     // ��������
            case DISP_OFF_RESERVE:					// ��������
                Seg_data_set(0, 21);           				// 'H.' ǥ��
                Seg_data_set(1, 18);
                
                icon_led.Bit.sethumi_char = 0;
                icon_led.Bit.sethumi_percent = 0;
                icon_led.Bit.curhumi_percent = 0;
                icon_led.Bit.curhumi_char = 0;
                icon_led.Bit.icon_reserve = 1;
                icon_led.Bit.time_char = 1;

                if(flg_toggle_500ms)
                {
                    if(disp_mode == DISP_ON_RESERVE)
                        temp16_var = onreserve_settime;
                    else
                        temp16_var = offreserve_settime;

                    temp = temp16_var / 10;
                    if(temp)						// �������� �ִٸ�
                        Seg_data_set(2, temp);
                    else
                        Seg_data_set(2, 18);


                    temp = temp16_var % 10;
                    Seg_data_set(3, temp);
                }
                else
                {
                    Seg_data_set(2, 18);
                    Seg_data_set(3, 18);
                }
                break;
    //			case DISP_EVAROOM_TEMP:                               // ��������µ�����
    //				Disp_Temper();
    //				Disp_EvaTemper();
    //				break;
            default:
                break;
        }
    }
}
//================================================================================================================
void Stop_Proc(void)
{
    op_mode = STOP_MODE;
    flg_force_comp_on = 0;

    flg_comp_on = 0;										// Comp OFF
    flg_solvalve_on = 0;

    if(op_kind == WIND)										// ��ǳ������ 
    {
        evafan_offtime = 0;
        flg_evafan_on = 0;									// �ٷ� ����
    }
    else
    {
        if(Flashdatarec.e2p_fanoff_delay)                   // F4 �ɼ�
        {
            if((evafan_offtime == 0) && flg_evafan_on)
            {
                if(Flashdatarec.e2p_fandelay_time == 0)
                    flg_evafan_on = 0;									// �ٷ� ����
                else
                {
                    if(flg_short_circuit)
                        evafan_offtime = Flashdatarec.e2p_fandelay_time;
                    else
                        evafan_offtime = Flashdatarec.e2p_fandelay_time * 60;		// Delay ���� ��ǳ�� ����
                }
            }
        }
        else
        {
            evafan_offtime = 0;
            flg_evafan_on = 0;									// �ٷ� ����
        }
    }

    flg_offreserve = 0;
    offreserve_timeout = 0;
    flg_emergency_oper = 0;
    //flg_forcekey_in = 0;

    disp_mode = DISP_WAIT;
    eva_error_timeout = 0;

    if(error_code)
        Error_Recovery_Proc();
    if(error_code == 0)
        Buzzer_Stop();
}
//===========================================================================================
// Ű �Է� ó��
void InputKey_Proc(void)
{
    if((disp_mode == DISP_OPTION) || (disp_mode == DISP_ON_RESERVE) || (disp_mode == DISP_OFF_RESERVE))
    {
        if(optkey_timeout == 0)
        {
            if(disp_mode == DISP_ON_RESERVE)
            {
                if(flg_short_circuit)
                    onreserve_timeout = onreserve_settime;
                else
                    onreserve_timeout = onreserve_settime * 60;			// ������ ȯ��
                
                flg_onreserve = 1;
                flg_blink = 0;
            }
            else if(disp_mode == DISP_OFF_RESERVE)
            {
                if(flg_short_circuit)
                    offreserve_timeout = offreserve_settime;
                else
                    offreserve_timeout = offreserve_settime * 60;
                
                flg_offreserve = 1;
                flg_blink = 0;
            }

            if(op_mode == RUN_MODE)
                disp_mode = DISP_OPERATION;
            else
                disp_mode = DISP_WAIT;
        }
    }
    //--------------------------------------------------------------------------------------------------------------------
    if(flg_remote_controle)		// �������� 
    {
        if(COMM_REMOTE)			// �ܺ� ����ġ Open
        {
            if(op_mode == RUN_MODE)
                menu_key[OPERSTOP_KEY] = 1;			// Power OFF
            else
                goto KEY_CLEAR;
        }
        else
        {
            if(op_mode == STOP_MODE)
                menu_key[OPERSTOP_KEY] = 1;			// Power ON
            else if(menu_key[OPERSTOP_KEY])
                goto KEY_CLEAR;
        }
    }
    
    //--------------------------------------------------------------------------------------------------------------------
    if(key_continued[UP_KEY] && key_continued[DOWN_KEY] && (error_code & (ERR_EVATEMPER | ERR_ROOMHUMI)))					// ������ �ø� + ���� 3��
    {
        if(flg_option_start == 0)
        {
            flg_option_start = 1;
            key_timeout = 25;            														// 3 SEC
            flg_cont_proc = 0;
        }
        else
        {
            if((key_timeout == 0) && (flg_cont_proc == 0))
            {
                flg_cont_proc = 1;
                flg_emergency_oper = 1;

                op_kind = DEHUMI;			// ���� ���
                op_mode = RUN_MODE;
                disp_mode = DISP_OPERATION;
                flg_onreserve = 0;
                flg_offreserve = 0;
                onreserve_settime = 0;
                offreserve_settime = 0;

                Play_Buzzer(1);																	
            }
        }
    }
    //--------------------------------------------------------------------------------------------------------------------
    if(op_mode == STOP_MODE)			// �������
    {
        if(menu_key[OPERSTOP_KEY] && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0))      // ���� Key
        {
            if(disp_mode == DISP_ON_RESERVE)		// ���÷��� ��尡 ���� �ϰ��
            {
                disp_mode = prev_disp_mode;
                flg_onreserve = 0;
                onreserve_settime = 0;
                Play_Buzzer(1);						
                flg_blink = 0;
            }
            else
            {
                if((error_code == 0) && (!((comm_cur_temper <= 15) && (Flashdatarec.e2p_senuse_onoff))))
                {
                    op_mode = RUN_MODE;
                    disp_mode = DISP_OPERATION;
                    flg_onreserve = 0;
                    onreserve_settime = 0;
                    curtemp_error_timeout = 15; 

                    Buzzer_Stop();
                    Play_Buzzer(3);				// Power On
                }
                else
                    Play_Buzzer(0);				
            }
        }
        else if((menu_key[RESERVE_KEY]) && (menu_key[DEHUMI_KEY] == 0) && (key_pushed[DEHUMI_KEY] == 0) && (error_code == 0))		// ���� Ű (��������)
        {
            if((disp_mode == DISP_WAIT) || (disp_mode == DISP_ON_RESERVE))
            {
                if(disp_mode == DISP_ON_RESERVE)		// ���÷��� ��尡 ���� �ϰ��
                {	
                    disp_mode = prev_disp_mode;
                    flg_onreserve = 0;
                    onreserve_settime = 0;
                }
                else								// ���÷��� ��尡 ������ �ƴ϶��
                {
                    prev_disp_mode = disp_mode;
                    disp_mode = DISP_ON_RESERVE;
                    onreserve_settime = 3;					// 3 �ð�
                    optkey_timeout = 5;
                }
            }
            Play_Buzzer(1);						// Button
        }
        else if((menu_key[DOWN_KEY] || key_continued[DOWN_KEY]) && (key_pushed[UP_KEY] == 0) && 
                (key_pushed[DEHUMI_KEY] == 0) && ((disp_mode == DISP_ON_RESERVE) || (disp_mode == DISP_OPTION)))				// Down Ű
        {
            if(key_continued[DOWN_KEY])
            {
                if(flg_option_start == 0)
                {
                    flg_option_start = 1;
                    if(keycont_timeout == 0)
                        keycont_timeout = 50;		// 500ms
                }
            }

            if(keycont_timeout == 0)
            {
                keycont_timeout = 20;
                Play_Buzzer(1);						// Button

                if(disp_mode == DISP_ON_RESERVE)		// ���÷��� ��尡 ���� �ϰ��
                {
                    if(onreserve_settime > 1)			
                        onreserve_settime--;

                    optkey_timeout = 5;
                }
                else
                    Play_Buzzer(0);						// Not
            }
        }
        else if((menu_key[UP_KEY] ||  key_continued[UP_KEY]) && (key_pushed[DOWN_KEY] == 0)  && 
                (key_pushed[DEHUMI_KEY] == 0) && ((disp_mode == DISP_ON_RESERVE) || (disp_mode == DISP_OPTION)))				// Up Ű
        {
            if(key_continued[UP_KEY])
            {
                if(flg_option_start == 0)
                {
                    flg_option_start = 1;
                    if(keycont_timeout == 0)
                        keycont_timeout = 50;		// 500ms
                }
            }

            if(keycont_timeout == 0)
            {
                keycont_timeout = 20;
                Play_Buzzer(1);					

                if(disp_mode == DISP_ON_RESERVE)		// ���÷��� ��尡 ���� �ϰ��
                {
                    if(onreserve_settime < 10)
                        onreserve_settime++;

                    optkey_timeout = 5;
                }
                else
                    Play_Buzzer(0);						// Button
            }
        }
        else if(menu_key[STRONG_KEY])
        {
            if(flg_evafan_on)
            {
                evafan_mode = 1;
                Play_Buzzer(1);						// Button
            }
            else
                Play_Buzzer(0);						// Not
        }
        else if(menu_key[WEAK_KEY])
        {
            if(flg_evafan_on)
            {
                evafan_mode = 0;
                Play_Buzzer(1);						// Button
            }
            else
                Play_Buzzer(0);						// Not
        }
        else if(menu_key[UP_KEY] || menu_key[DOWN_KEY] || menu_key[BLOW_KEY] )
            Play_Buzzer(0);							// Not
    }
    //-----------------------------------------------------------------------------------------------
    else											// �������
    {
        if(key_continued[DEHUMI_KEY] && key_continued[UP_KEY] && (menu_key[RESERVE_KEY] == 0))	// ���� + �ø� Ű	Option Mode
        {
            if(disp_mode != DISP_OPTION)
            {
                if(flg_option_start == 0)
                {
                    flg_option_start = 1;
                    key_timeout = 25;            // 5 SEC
                    flg_cont_proc = 0;
                }
                else
                {
                    if((key_timeout == 0) && (flg_cont_proc == 0))
                    {
                        flg_cont_proc = 1;
                        prev_disp_mode = disp_mode;
                        disp_mode = DISP_OPTION;
                        flg_keyall_off = 0;
                        opt_num = 1;
                        optkey_timeout = 10;										// key timeout
                        Play_Buzzer(1);												// Button
                    }
                }
            }
        }
        else if(menu_key[OPERSTOP_KEY])      			// ����
        {
            if(disp_mode == DISP_OFF_RESERVE)		// ���÷��� ��尡 ���� �ϰ��
            {
                disp_mode = DISP_OPERATION;
                flg_offreserve = 0;
                offreserve_settime = 0;
                Play_Buzzer(1);						// Button
                flg_blink = 0;
            }
            else if(disp_mode == DISP_OPTION)
            {
                disp_mode = prev_disp_mode;
                e2p_save_timeout = 10;											// 1�� �� ����
                Play_Buzzer(1);						
            }
            else
            {
                Stop_Proc();
                Play_Buzzer(2);						// Power Off
            }
        }
        else if(menu_key[DEHUMI_KEY] && (menu_key[RESERVE_KEY] == 0)  && (disp_mode != DISP_EVAROOM_TEMP) && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0) && (error_code == 0))				// ���� Ű
        {
            if(disp_mode == DISP_OPTION)
            {
                if(flg_keyall_off)
                {
                    optkey_timeout = 10;
                    opt_num++;
                    if(opt_num > 8)
                        disp_mode = prev_disp_mode;
                    e2p_save_timeout = 10;												// 1�� �� ����
                    Play_Buzzer(1);						// Button
                }
            }
            else if(op_kind == WIND)
            {
                op_kind = DEHUMI;			// ���� ���
                Play_Buzzer(1);						
            }
            else
                Play_Buzzer(0);						
        }
        else if(menu_key[BLOW_KEY] && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0)  && (error_code == 0) && (disp_mode != DISP_OPTION))				// ��ǳ Ű
        {
            if((disp_mode != DISP_ROOM_TEMP) && (disp_mode != DISP_EVAROOM_TEMP))
            {
                if(op_kind == DEHUMI)
                {
                    op_kind = WIND;			// ��ǳ ���
                    Play_Buzzer(1);						
                }
                else
                    Play_Buzzer(0);						
            }
        }
        else if(menu_key[STRONG_KEY] && (key_pushed[WEAK_KEY] == 0) && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0)  
                && (disp_mode != DISP_ROOM_TEMP) && (error_code == 0) && (disp_mode != DISP_OPTION))				// �� Ű
        {
            if(flg_evafan_on)
            {
                evafan_mode = 1;
                Play_Buzzer(1);						// Button
            }
            else
                Play_Buzzer(0);						// Button
        }
        else if(menu_key[WEAK_KEY] && (key_pushed[STRONG_KEY] == 0)  && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0)  
                && (disp_mode != DISP_ROOM_TEMP) && (error_code == 0) && (disp_mode != DISP_OPTION))					// �� Ű
        {
            if(flg_evafan_on)
            {
                evafan_mode = 0;
                Play_Buzzer(1);						// Button
            }
            else
                Play_Buzzer(0);						// Button
        }
        else if(menu_key[RESERVE_KEY] && (menu_key[DEHUMI_KEY] == 0) && (key_pushed[DEHUMI_KEY] == 0) && (key_pushed[UP_KEY] == 0) 
                && (key_pushed[DOWN_KEY] == 0)  && (error_code == 0) && (disp_mode != DISP_OPTION))					// ���� Ű (��������)
        {
            if(disp_mode == DISP_OFF_RESERVE)		// ���÷��� ��尡 ���� �ϰ��
            {	
                disp_mode = DISP_OPERATION;
                flg_offreserve = 0;
                offreserve_settime = 0;
            }
            else								// ���÷��� ��尡 ������ �ƴ϶��
            {
                disp_mode = DISP_OFF_RESERVE;
                offreserve_settime = 3;
                optkey_timeout = 5;
            }
            Play_Buzzer(1);						// Button
        }
        //--------------------------------------------------------------------------------------------------------------------
        else if(key_continued[DEHUMI_KEY] && key_continued[DOWN_KEY] && (disp_mode != DISP_OPTION))				// ���� �µ� ǥ�� ���� + ���� 3��
        {
            if(flg_option_start == 0)
            {
                flg_option_start = 1;
                key_timeout = 25;            														// 3 SEC
                flg_cont_proc = 0;
            }
            else
            {
                if((key_timeout == 0) && (flg_cont_proc == 0))
                {
                    flg_cont_proc = 1;
                    prev_disp_mode = disp_mode;
                    disp_mode = DISP_EVAROOM_TEMP;
                    Play_Buzzer(1);																	// Button
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------------
        else if(key_continued[STRONG_KEY] && key_continued[WEAK_KEY] && (disp_mode != DISP_OPTION))			// ���� �µ� ǥ�� �� + �� 3��
        {
            if(flg_option_start == 0)
            {
                flg_option_start = 1;
                key_timeout = 25;            														// 3 SEC
                flg_cont_proc = 0;
            }
            else
            {
                if((key_timeout == 0) && (flg_cont_proc == 0))
                {
                    flg_cont_proc = 1;
                    prev_disp_mode = disp_mode;
                    disp_mode = DISP_ROOM_TEMP;
                    Play_Buzzer(1);																	
                }
            }
        }
        //--------------------------------------------------------------------------------------------------------------------
        else if(key_continued[DEHUMI_KEY] && key_continued[RESERVE_KEY])			// COMP �������� ���� + ���� 3��
        {
            if(flg_option_start == 0)
            {
                flg_option_start = 1;
                key_timeout = 25;                                                                                                           // 3 SEC
                flg_cont_proc = 0;
            }
            else
            {
                if((key_timeout == 0) && (flg_cont_proc == 0))
                {
                    flg_cont_proc = 1;
                    flg_force_comp_on = 1;
                    //flg_forcekey_in = 1;
                    Play_Buzzer(1);																	
                }
            }
        }
        else if((menu_key[DOWN_KEY] || key_continued[DOWN_KEY]) && (disp_mode != DISP_EVAROOM_TEMP) && (key_pushed[UP_KEY] == 0) && (key_pushed[DEHUMI_KEY] == 0) && (error_code == 0))				// Down Ű
        {
            if(key_continued[DOWN_KEY])
            {
                if(flg_option_start == 0)
                {
                    flg_option_start = 1;
                    if(keycont_timeout == 0)
                        keycont_timeout = 50;		// 500ms
                }
            }

            if(keycont_timeout == 0)
            {
                keycont_timeout = 20;
                Play_Buzzer(1);						// Button

                if(disp_mode == DISP_OFF_RESERVE)		// ���÷��� ��尡 ���� �ϰ��
                {
                    if(offreserve_settime > 1)	
                        offreserve_settime--;	
                    optkey_timeout = 5;
                }
                else if(disp_mode == DISP_OPTION)
                {
                    optkey_timeout = 10;
                    e2p_save_timeout = 10;												// 1�� �� ����
                    if(flg_keyall_off)
                    {
                        switch(opt_num)
                        {
                            case 1:										// ������ ���� (-9% - +9%)
                                if(Flashdatarec.e2p_dehumi_cali > -9)
                                    Flashdatarec.e2p_dehumi_cali--;
                                break;
                            case 2:										// �������� ��� ����
                                Flashdatarec.e2p_senuse_onoff = !Flashdatarec.e2p_senuse_onoff;
                                break;
                            case 3:										// ���� �߻� �µ�
                                if(Flashdatarec.e2p_jesang_temp > -17)
                                    Flashdatarec.e2p_jesang_temp--;
                                break;
                            case 4:										// FAN OFF ��������
                                Flashdatarec.e2p_fanoff_delay = !Flashdatarec.e2p_fanoff_delay;
                                break;
                            case 5:										// FAN OFF ���� �ð� ����
                                if(Flashdatarec.e2p_fandelay_time > 0)
                                    Flashdatarec.e2p_fandelay_time--;	// 1�� ����
                                break;
                            case 6:										// �ø����� E7 ���� ���
                                Flashdatarec.e2p_E7_check = !Flashdatarec.e2p_E7_check;
                                break;
                            case 7:										// ���� ���� �µ� (0 - 17C)
                                if(Flashdatarec.e2p_jesang_reltemp > 0)
                                    Flashdatarec.e2p_jesang_reltemp--;
                                break;
                            case 8:										// ���� ���� �ð� (1 - 10��)
                                if(Flashdatarec.e2p_jesang_reltime > 1)
                                    Flashdatarec.e2p_jesang_reltime--;
                                break;
                            default:
                                break;
                        }
                    }
                }
                else
                {
                    switch(op_kind)				// ���� ���ø��
                    {
                        case DEHUMI:			// ��������
                            if(set_cur_humi > 30)	// 30-90        ����
                                set_cur_humi--;
                            // eeprom save �ʿ�
                            disp_mode = DISP_DEHUMI_SET;
                            flg_blink = 0;
                            key_updown_timeout = 20;				// 2 sec
                            break;
                        case WIND:				// ��ǳ����
                            break;
                        default:
                            break;

                    }
                }
            }
        }
        else if((menu_key[UP_KEY] || key_continued[UP_KEY]) && (key_pushed[DOWN_KEY] == 0) && (key_pushed[DEHUMI_KEY] == 0) && (error_code == 0))				// Up Ű
        {
            if(key_continued[UP_KEY])
            {
                if(flg_option_start == 0)
                {
                    flg_option_start = 1;
                    if(keycont_timeout == 0)
                        keycont_timeout = 50;		// 500ms
                }
            }

            if(keycont_timeout == 0)
            {
                keycont_timeout = 20;
                Play_Buzzer(1);						// Button

                if(disp_mode == DISP_OFF_RESERVE)		// ���÷��� ��尡 ���� �ϰ��
                {
                    if(offreserve_settime < 10)
                        offreserve_settime++;
                    optkey_timeout = 5;	
                }
                else if(disp_mode == DISP_OPTION)
                {
                    optkey_timeout = 10;
                    e2p_save_timeout = 10;												// 1�� �� ����
                    if(flg_keyall_off)
                    {
                        switch(opt_num)
                        {
                            case 1:										// ������ ���� (-9% - +9%)
                                if(Flashdatarec.e2p_dehumi_cali < 9)
                                    Flashdatarec.e2p_dehumi_cali++;
                                break;
                            case 2:										// �������� ��� ����
                                Flashdatarec.e2p_senuse_onoff = !Flashdatarec.e2p_senuse_onoff;
                                break;
                            case 3:										// ���� �߻� �µ�
                                if(Flashdatarec.e2p_jesang_temp < 0)
                                    Flashdatarec.e2p_jesang_temp++;
                                break;
                            case 4:										// FAN OFF ��������
                                Flashdatarec.e2p_fanoff_delay = !Flashdatarec.e2p_fanoff_delay;
                                break;
                            case 5:										// FAN OFF ���� �ð� ����
                                if(Flashdatarec.e2p_fandelay_time < 10)
                                    Flashdatarec.e2p_fandelay_time++;	// 1�� ����
                                break;
                            case 6:										// �ø����� E7 ���� ���
                                Flashdatarec.e2p_E7_check = !Flashdatarec.e2p_E7_check;
                                break;
                            case 7:										// ���� ���� �µ� (0 - 17C)
                                if(Flashdatarec.e2p_jesang_reltemp < 17)
                                    Flashdatarec.e2p_jesang_reltemp++;
                                break;
                            case 8:										// ���� ���� �ð� (1 - 10��)
                                if(Flashdatarec.e2p_jesang_reltime < 10)
                                    Flashdatarec.e2p_jesang_reltime++;
                                break;
                            default:
                                break;
                        }
                    }
                }
                else
                {
                    switch(op_kind)				// ���� ���ø��
                    {
                        case DEHUMI:			// ��������
                            if(set_cur_humi < 90)	// 10-90
                                set_cur_humi++;
                            // eeprom save �ʿ�
                            disp_mode = DISP_DEHUMI_SET;
                            flg_blink = 0;
                            key_updown_timeout = 20;				// 2 sec
                            break;
                        case WIND:				// ��ǳ����
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }
	
    if((!key_pushed[UP_KEY]) && (!key_pushed[DOWN_KEY]) && (!key_pushed[DEHUMI_KEY]) && (!key_pushed[BLOW_KEY]) && (!key_pushed[RESERVE_KEY])
            && (!key_pushed[STRONG_KEY]) && (!key_pushed[WEAK_KEY]))
    {
        flg_option_start = 0;
        flg_keyall_off = 1;
    }
      
        
KEY_CLEAR:
    menu_key[OPERSTOP_KEY] = 0;
    menu_key[DEHUMI_KEY] = 0;
    menu_key[RESERVE_KEY] = 0;
    menu_key[DOWN_KEY] = 0;
    menu_key[UP_KEY] = 0;
    menu_key[BLOW_KEY] = 0;
    menu_key[STRONG_KEY] = 0;
    menu_key[WEAK_KEY] = 0;
}

//--------------------------------------------------------------------------------------------------
// ����µ� ǥ��
void Disp_Temper(void)
{
    uint8_t temp;
    uint8_t flg_minus;
    int tmp_var;
    
    if(comm_cur_temper > 70)
    {
        Seg_data_set(0, 21);            // H
        Seg_data_set(1, 22);            // i and dot
    }
    else if(comm_cur_temper < -19)
    {
        Seg_data_set(0, 19);            // L
        Seg_data_set(1, 20);            // o and dot
    }
    else
    {
        flg_minus = 0;
        if(comm_cur_temper < 0)
        {
            tmp_var = comm_cur_temper * -1;
            flg_minus = 1;
        }
        else
            tmp_var = comm_cur_temper;
        
        temp = tmp_var / 10;
        if(flg_minus)
        {
          if(temp == 0)
              Seg_data_set(0, 16);            // Bar
          else
              Seg_data_set(0, 28);            // Bar
        }
        else
        {
          if(temp == 0)
              Seg_data_set(0, 18);            // Blank
          else
              Seg_data_set(0, temp);
        }
        
        temp = tmp_var % 10;
        Seg_data_set(1, temp);
    }
}
//--------------------------------------------------------------------------------------------------

// ���ٿµ� ǥ��
void Disp_EvaTemper(void)
{
    uint8_t temp;
    uint8_t flg_minus;
    int tmp_var;
    
    if(comm_eva_temper >= 70)
    {
        Seg_data_set(0, 21);            // H
        Seg_data_set(1, 22);            // i and dot
    }
    else if(comm_eva_temper <= -19)
    {
        Seg_data_set(0, 19);            // L
        Seg_data_set(1, 20);            // o and dot
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
              Seg_data_set(0, 16);            // Bar
          else
              Seg_data_set(0, 28);            // Bar
        }
        else
        {
          if(temp == 0)
              Seg_data_set(0, 18);            // Blank
          else
              Seg_data_set(0, temp);
        }
        
        temp = tmp_var % 10;
        Seg_data_set(1, temp);
    }
}
//--------------------------------------------------------------------------------------------------
// ���� ǥ��
void Disp_Humidity(void)
{
    uint8_t temp;
    int tmp_var;
    
    if(comm_cur_humi > 95)
    {
        Seg_data_set(0, 21);            // H
        Seg_data_set(1, 22);            // i and dot
    }
    else if(comm_cur_humi < 5)
    {
        Seg_data_set(0, 19);            // L
        Seg_data_set(1, 20);            // o and dot
    }
    else
    {
        tmp_var = comm_cur_humi;
        
        temp = tmp_var / 10;
        if(temp == 0)
            Seg_data_set(0, 18);            // Blank
        else
            Seg_data_set(0, temp);
        
        temp = tmp_var % 10;
        Seg_data_set(1, temp);
    }
	
    if(flg_remote_controle)
    {
        if(flg_toggle_500ms)
            icon_led.Bit.curhumi_percent = 1;
        else
            icon_led.Bit.curhumi_percent = 0;
    }
    else
        icon_led.Bit.curhumi_percent = 1;
}

//-------------------------------------------------------------------------------------------
void IrRxProc(void)				// Every 200us
{
    if(RxOk)
        return;

    NowIrIn = !REMOCONRXPIN;

    switch(IrStep_Num)
    {
        case 0:
            if(NowIrIn)						// Lead code Check
            {
                bitcount++;
                if(bitcount > 48)			// High:9ms (About 45count)
                    bitcount = 0;
            }
            else
            {
                if((bitcount >= 42) && (bitcount <= 48))
                {
                    bitcount = 1;
                    IrStep_Num = 1;
                }
                else
                    bitcount = 0;
            }
            break;
        case 1:
            if(NowIrIn == 0)						// Lead code Check
            {
                bitcount++;
                if(bitcount > 26)			// High:4.5ms (About 22.5count)
                {
                    bitcount = 0;
                    IrStep_Num = 0;
                }
            }
            else
            {
                if((bitcount >= 19) && (bitcount <= 26))
                {
                    bitcount = 1;
                    IrStep_Num = 2;
                    IrRxBuf = 0;
                    Irmask = 0x80000000;
                }
                else
                {
                    bitcount = 0;
                    IrStep_Num = 0;
                }
            }
            break;
        case 2:
            if(NowIrIn)						// First bit
            {
                bitcount++;
                if(bitcount > 5)			// High:0.56ms (About 2.8count)
                {
                    bitcount = 0;
                    IrStep_Num = 0;
                }
            }
            else
            {
                if((bitcount >= 1) && (bitcount <= 5))
                {
                    bitcount = 1;
                    IrStep_Num = 3;
                }
                else
                {
                    bitcount = 0;
                    IrStep_Num = 0;
                }
            }
            break;
        case 3:
            if(NowIrIn == 0)						// Second bit
            {
                bitcount++;
                if(bitcount > 11)			// High:0.56ms or 1.69ms (About 2.8count, 8.45count)
                {
                    if(Irmask == 0x01)		// Last bit
                    {
                        IrRxBuf |= Irmask;
                        IrStep_Num = 4;
                    }
                    else
                    {
                        bitcount = 0;
                        IrStep_Num = 0;
                    }
                }
            }
            else
            {
                if((bitcount >= 1) && (bitcount <= 5))		// bitcount 0
                {
                    bitcount = 1;
                    IrStep_Num = 2;
                }
                else if((bitcount >= 5) && (bitcount <= 11))		// bitcount 1
                {
                    bitcount = 1;
                    IrStep_Num = 2;
                    IrRxBuf |= Irmask;
                }
                else
                {
                    bitcount = 0;
                    IrStep_Num = 0;
                }

                if(Irmask == 0x01)		// �������̸�
                    IrStep_Num = 4;

                Irmask >>= 1;
            }
            break;
        case 4:
            Rcv_IrData = IrRxBuf;
            bitcount = 0;
            IrStep_Num = 0;
            RxOk = 1;
            break;
        default:
            break;
    }
}
//--------------------------------------------------------------------------------------------------
void Control_Proc(void)
{
    if(op_mode == RUN_MODE)
    {
        //----------------------------------------------------------------   ����
        if(flg_offreserve)				// ���� ������ �ִ�
        {
            if(offreserve_timeout == 0)	// ���� �ð��� �Ǿ��ٸ�
            {
                menu_key[OPERSTOP_KEY] = 1;	// ����
                flg_offreserve = 0;
            }
        }

        //----------------------------------------------------------------  ����
        if((error_code == 0) && (eva_freez_detect == 0))
        {
            switch(op_kind)										// ���� ���ø��
            {
                case DEHUMI:									// ��������
                    if(!((comm_cur_temper <= 15) && (Flashdatarec.e2p_senuse_onoff)))
                    {
                        curtemp_error_timeout = 15;                             // 1��
                        
                        if(comm_cur_humi >= (Flashdatarec.e2p_sethumi + 3))  	// ���� ������ �������� + 3�̻��� ���
                        {
                            flg_comp_on = 1;										// Comp ON
                            evafan_offtime = 0;
                            flg_evafan_on = 1;                                      // ��ǳ�� ON
                        }
                        else if(comm_cur_humi <= (Flashdatarec.e2p_sethumi - 3))
                        {
                            if(flg_comp_on)
                            {
                                flg_comp_on = 0;	// Comp OFF
                                if(Flashdatarec.e2p_fanoff_delay)
                                {
                                    if(flg_short_circuit)
                                        evafan_offtime = Flashdatarec.e2p_fandelay_time;
                                    else
                                        evafan_offtime = Flashdatarec.e2p_fandelay_time * 60;
                                    flg_evafan_on = 1;                                      // ��ǳ�� ON
                                }
                            }
                        }
                        
                        if(Flashdatarec.e2p_fanoff_delay == 0)                      // F4 Off�� ������ ��� ����
                        {
                            evafan_offtime = 0;
                            flg_evafan_on = 1;                                      // ��ǳ�� ON
                        }
                    }
                    else    // �������� ���� �ȵ�
                    {
                        if(curtemp_error_timeout == 0)
                            menu_key[OPERSTOP_KEY] = 1;	// ���� OFF
                    }
                    break;
                case WIND:										// ��ǳ����
                    evafan_offtime = 0;
                    flg_evafan_on = 1;							// ��ǳ�� ON
                    flg_comp_on = 0;							// Comp OFF
                    curtemp_error_timeout = 15; 
                    break;
                default:
                    break;
            }
        }

        //----------------------------------------------------------------  ��� ����
        if(highpress_detect)					// ��а��� �̸�
        {
            flg_comp_on = 0;				// Comp OFF
            evafan_offtime = 0;
            flg_evafan_on = 1;

            error_code |= ERR_HIGHPRESURE;
        }
        //----------------------------------------------------------------  ���а���
        if(lowpress_error)					// ���п��� �̸�
        {
            flg_comp_on = 0;				// Comp OFF
            evafan_offtime = 0;
            flg_evafan_on = 1;
            error_code |= ERR_LOWPRESURE;
        }
        //----------------------------------------------------------------  ���ٿµ� ���� ����
        if(evatemp_sensor_error)				
        {
            error_code |= ERR_EVATEMPER;
            if(flg_emergency_oper)
            {
                compdelay_timeout = 0;
                flg_comp_on = 1;
                evafan_offtime = 0;
                flg_evafan_on = 1;
                
                if(COMM_DIP_SW1)					// Dip S/W OFF (�Ϲݿ�)
                    flg_solvalve_on = 0;
                else
                    flg_solvalve_on = 1;				// Sol valve ON
            }
            else
            {
                flg_comp_on = 0;						// Comp OFF
                flg_solvalve_on = 0;

                if(flg_evatemp_first == 0)
                {
                    evafan_offtime = 0;
                    flg_evafan_on = 1;
                    if(flg_short_circuit)
                        evadelay_timeout = 4;
                    else
                        evadelay_timeout = 240;				// 4��
                    flg_evatemp_first = 1;
                }
                else
                {
                    if(evadelay_timeout == 0)
                    {
                        if((op_mode == RUN_MODE) && (flg_emergency_oper == 0))
                        {
                            menu_key[OPERSTOP_KEY] = 1;	// ���� OFF
                        }
                    }
                }
            }
        }
        else
        {
            if(flg_evatemp_first)
            {
                flg_evatemp_first = 0;
                evadelay_timeout = 0;
            }
        }
        //----------------------------------------------------------------  ���� ���� ����
        if(eva_freez_detect && (evatemp_sensor_error == 0))					// ���� ���� ����
        {
            if(COMM_DIP_SW1)					// Dip S/W OFF (�Ϲݿ�)
            {
                flg_comp_on = 0;					// Comp OFF
                flg_solvalve_on = 0;
            }
            else
            {
                flg_comp_on = 1;					// Comp ON
                flg_solvalve_on = 1;				// Sol valve ON
            }

            evafan_offtime = 0;
            flg_evafan_on = 1;					// ��ǳ�� ����

            if(flg_evafreez_detect_first == 0)
            {
                if(flg_short_circuit)
                    eva_error_timeout = Flashdatarec.e2p_jesang_reltime;
                else
                    eva_error_timeout = Flashdatarec.e2p_jesang_reltime * 60;
                flg_evafreez_detect_first = 1;
            }
            else
            {
                if((comm_eva_temper >= Flashdatarec.e2p_jesang_reltemp)	&& (eva_error_timeout == 0))    // ���ٵ��� ���� �µ� �̻��̸�
                {
                    eva_freez_detect = 0;
                    flg_solvalve_on = 0;

                    if(op_kind == DEHUMI)			// ��������̸�
                        flg_comp_on = 1;				// Comp ON

                    flg_evafan_on = 1;                          // ��ǳ�� ����
                }
            }
        }
        else
            flg_evafreez_detect_first = 0;
        //--------------------------------------------------------------------------------		
    }
    else	// STOP_MODE
    {
        if(flg_onreserve)				// ���� ������ �ִ�
        {
            if(onreserve_timeout == 0)	// ���� �ð��� �Ǿ��ٸ�
            {
                menu_key[OPERSTOP_KEY] = 1;	// ����
                flg_onreserve = 0;
            }
        }
    }
    //----------------------------------------------------------------  ������(����ħ) ����
    if(waterlevel_sensor_error)
    {
        if(flg_overwater_first)
        {
            error_code |= ERR_OVERWATER;

            flg_overwater_first = 0;
            flg_comp_on = 0;										// Comp OFF
            evafan_offtime = 0;
            flg_evafan_on = 0;										// ��ǳ�� OFF
        }

        if(warning_timeout == 0)
        {
            warning_timeout = 5;
            Play_Buzzer(0);
        }
    }
    else
    {
        if(flg_overwater_first == 0)
        {
            flg_overwater_first = 1;
            if(op_mode == RUN_MODE)
            {
                evafan_offtime = 0;                                
                flg_evafan_on = 1;                                      // ��ǳ�� ON
            }
            Buzzer_Stop();
        }
    }
    //----------------------------------------------------------------  ���� ���� ����
    if(humi_sensor_error || roomtemp_sensor_error)                                               
    {
        curtemp_error_timeout = 15; 
        
        if(flg_emergency_oper)
        {
            compdelay_timeout = 0;
            flg_comp_on = 1;
            evafan_offtime = 0;
            flg_evafan_on = 1;
            
            if(COMM_DIP_SW1)					// Dip S/W OFF (�Ϲݿ�)
                flg_solvalve_on = 0;
            else
                flg_solvalve_on = 1;				// Sol valve ON
        }
        else
        {
            flg_comp_on = 0;						// Comp OFF

            if(flg_roomhumi_first == 0)
            {
                evafan_offtime = 0;
                if(op_mode == RUN_MODE)
                    flg_evafan_on = 1;

                if(flg_short_circuit)
                    humidity_timeout = 4;
                else
                    humidity_timeout = 240;                 // 4��
                flg_roomhumi_first = 1;
                error_code |= ERR_ROOMHUMI;            
            }
            else
            {
                if(humidity_timeout == 0)
                {
                    if((op_mode == RUN_MODE) && (flg_emergency_oper == 0))
                    {
                        menu_key[OPERSTOP_KEY] = 1;	// ���� OFF
                    }
                }
            }
        }
    }
    else
    {
        if(flg_roomhumi_first)
        {
            flg_roomhumi_first = 0;
            humidity_timeout = 0;
        }
    }
    //----------------------------------------------------------------  ��µ� ���� ����
    /*
    if(roomtemp_sensor_error)				// ��µ� ���� ����
    {
        error_code |= ERR_ROOMTEMPER;
        if(flg_emergency_oper == 0)
            flg_comp_on = 0;				// Comp OFF

        if(flg_roomtemp_first == 0)
        {
            flg_comp_on = 0;				// Comp OFF
            evafan_offtime = 0;	
            flg_evafan_on = 1;			
            roomtemp_timeout = 240;			// 4��
            flg_roomtemp_first = 1;
        }
        else
        {
            if(roomtemp_timeout == 0)
            {
                if(op_mode == RUN_MODE)
                {
                    menu_key[OPERSTOP_KEY] = 1;	// ���� OFF
                    error_code |= ERR_ROOMTEMPER;
                }
            }
        }
    }
    else
    {
        if(flg_roomtemp_first)
        {
            flg_roomtemp_first = 0;
            roomtemp_timeout = 0;
        }
    }
    */
}
//-------------------------------------------------------------------------------------------
void Disp_BlowLevel(void)
{
    if(flg_evafan_on)
    {
        if(level_timeout == 0)
        {
            simul_led.byte |= (1<<blow_level);
            blow_level++;

            if(blow_level > 7)
            {
                simul_led.byte = 0;
                blow_level = 0;
            }
            
            if(blow_level == 7)
                level_timeout = 70;
            else
                level_timeout = 36;
        }
    }
    else
    {
        simul_led.byte = 0;
        blow_level = 0;
    }
}
//-------------------------------------------------------------------------------------------
void Init_Variable(void)
{
    set_cur_humi = Flashdatarec.e2p_sethumi;
    if(flg_short_circuit)
        compdelay_timeout = 3;
    else
        compdelay_timeout = 180;			// comp ���� �� �簡�� �����ð� 3�� �ʴ����� ��ȯ
    evafan_mode = 1;                // ��ǳ�� ��� �����ΰ� �� ������ ����
}

void Disp_Version(void)
{
    Seg_data_set(0, 13);     //'d'
    Seg_data_set(1, 21);     //'H'
    Seg_data_set(2, 1);
    Seg_data_set(3, 9);
}

