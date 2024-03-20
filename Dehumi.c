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

bit     RxOk = 0;                               // IR 수신 데이터 수신 완료 플레그
bit     NowIrIn;                                 // IR 수신포트  현재 비트 상태
bit     flg_e2p_save = 0;
//bit     flg_blowfan_on = 0;			// COMP FAN
//bit	flg_blowfan_highlow = 0;            // EVA FAN 강 약
bit  	flg_waterchk = 0;
bit 	waterlevel_sensor_error = 0;
bit 	flg_highpresurechk = 0;
bit 	flg_lowpresurechk = 0;
bit 	flg_eva_freez_chk = 0;
bit 	eva_freez_detect = 0;
bit 	flg_evafan_on = 0;
bit 	flg_solvalve_on = 0;
bit 	evafan_mode = 0;						// 송풍팬 모드 1:강 0:약
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
bit 	flg_remote_controle = 0;			// Dip S/W 2		원격제어 선택 : OFF(해제), ON(선택) / 초기설정 OFF
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

uint32_t    IrRxBuf = 0;               // IR 수신 데이터(바이트) 버퍼
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
    if(flg_remote_controle)		// 원격제어 
    {
        if(COMM_REMOTE)			// 외부 스위치 Open
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
                case IR_DOWN:					// 내림
                    menu_key[DOWN_KEY] = 1;
                    break;
                case IR_UP:					// 올림
                    menu_key[UP_KEY] = 1;
                    break;
                case IR_PWR:					// 운전 정지
                    menu_key[OPERSTOP_KEY] = 1;
                    break;
                case IR_WIND:					// 송풍
                    menu_key[BLOW_KEY] = 1;
                    break;
                case IR_TURBO:					// 강/약
                    if(evafan_mode)             // 강이면
                        menu_key[WEAK_KEY] = 1;
                    else
                        menu_key[STRONG_KEY] = 1;
                    break;
                case IR_RESERVE:				// 예약
                    menu_key[RESERVE_KEY] = 1;
                    break;
                case IR_DEHUMI:				// 제습
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
// 입력 Check 처리
void InputCheck_Proc(void)
{
	//----------------------------------------------------   만수위(물넘침) 레벨 Check
	if(COMM_WATER_OVER && COMM_DIP_SW3)				// level 감지 dip s/w off 시
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
	//----------------------------------------------------   고압감지 에러
	if(COMM_HIGH_PRESS && (op_mode == RUN_MODE) && (op_kind == DEHUMI))										// level 감지
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
	//----------------------------------------------------   저압감지 에러
	if(COMM_LOW_PRESS && (op_mode == RUN_MODE) && (op_kind == DEHUMI) && Flashdatarec.e2p_E7_check)										// level 감지
	{
		if(flg_lowpresurechk == 0)
		{
            if(flg_short_circuit)
                lowp_timeout = 2;   							// 저압감지에러 시간 120초
            else
                lowp_timeout = 120;   							// 저압감지에러 시간 120초
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
    
	//----------------------------------------------------   에바 온도 에러
	if(((comm_eva_temper <= -20) || (comm_eva_temper >= 70)) && (op_mode == RUN_MODE) && (op_kind == DEHUMI))// -20 도 이하
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
	//----------------------------------------------------   에바동결 에러
	if((comm_eva_temper <= Flashdatarec.e2p_jesang_temp) && (evatemp_sensor_error == 0) && (op_mode == RUN_MODE) && (op_kind == DEHUMI))		// 제상 진입 온도 비교 초기 -1
	{
		if(flg_eva_freez_chk == 0)
		{
			flg_eva_freez_chk = 1;
			eva_detect_timeout = 35;							// 3 초
		}
		else
		{
			if(eva_detect_timeout == 0)
				eva_freez_detect = 1;						// 에바 동결 선언
		}
	}
	else
	{
		flg_eva_freez_chk = 0;
	}
	//----------------------------------------------------   룸 습도 센서 에러
	if(comm_cur_humi <= 0)                                  // 0%이하 또는 100% 이상    2021-07-21 고습도 에러 삭제
		humi_sensor_error = 1;
	else
		humi_sensor_error = 0;
	//----------------------------------------------------   룸 온도 센서 에러
	if(comm_cur_temper <= -19) // -15 도 이하
	{
		comm_cur_temper = -19;
        if(Flashdatarec.e2p_senuse_onoff)
            roomtemp_sensor_error = 1;
        else
            roomtemp_sensor_error = 0;
	}
	else if(comm_cur_temper >= 80)  // 80도 이				// 온도 에러를 Check 한다.
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
// 출력 처리
void Output_Proc(void)
{
    if(flg_evafan_on || flg_force_comp_on)                                   // 송풍팬 ON
    {
        if(evafan_mode)					// 송풍팬 모드 1:강 0:약
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


    if(flg_force_comp_on)					// 강제 콤프 출력
    {
        COMM_COMP = 1;
    }
    else if(flg_comp_on)					// 콤프 ON
    {
        if(compdelay_timeout == 0)                  // 콤프 지연시간
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
void Disp_OnOffReservTime(uint8_t onoff)					// 켜짐/ 꺼짐 예약 시간 표시
{
	uint8_t hour;
	
	if(onoff)
		temp16_var = onreserve_timeout;						// 분단위 
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
        icon_led.Bit.icon_check = 1;                // 점검 LED
    else
        icon_led.Bit.icon_check = 0;

    if(error_code & ERR_HIGHPRESURE)        // 고압감지 에러
    {
        Seg_data_set(1, 6);            	// 6
        Seg_data_set(2, 21);           	// H
        Seg_data_set(3, 27);            // P
    }
    else if(error_code & ERR_LOWPRESURE)	// 저압감지 에러
    {
        Seg_data_set(1, 7);            	// 7
        Seg_data_set(2, 19);           	// L
        Seg_data_set(3, 27);            // P
    }
    else if(error_code & ERR_ROOMHUMI)		// 습도 센서 에러
    {
        Seg_data_set(1, 1);            	// 1
        Seg_data_set(2, 5);            	// S
        Seg_data_set(3, 21);            // H
    }
    else if(error_code & ERR_ROOMTEMPER)	// 실내온도 에러
    {
        Seg_data_set(1, 2);            	// 2
        Seg_data_set(2, 5);            	// S
        Seg_data_set(3, 24);            // t
    }
    else if(error_code & ERR_EVATEMPER)		// 에바 센서 에러
    {
        Seg_data_set(1, 3);            	// 3
        Seg_data_set(2, 5);            	// S
        Seg_data_set(3, 14);            // E
    }
    else if(error_code & ERR_EVAFREEZ)		// 에바(증발기)동결 에러
    {
        Seg_data_set(1, 16);            // '-'
        Seg_data_set(2, 14);            // E
        Seg_data_set(3, 15);            // F
    }
    else if(error_code & ERR_OVERWATER)		// 물넘침 에러
    {
        Seg_data_set(1, 4);            	// 4
        Seg_data_set(2, 15);           	// F
        Seg_data_set(3, 26);            // u
        icon_led.Bit.icon_overwater = 1;
    }
    else if(error_code & ERR_COMM485)		// RS485 통신 에러
    {
        Seg_data_set(1, 8);            	// 8
        Seg_data_set(2, 12);           	// C
        Seg_data_set(3, 12);            // C
    }
}
//------------------------------------------------------------------------------------
void Error_Recovery_Proc(void)
{
    if(error_code & ERR_OVERWATER)				// 물넘침 에러
    {
        if(!COMM_WATER_OVER)					// level 감지	 Close
        {
            error_code &= ~ERR_OVERWATER;
            icon_led.Bit.icon_overwater = 0;
        }
    }

    if(error_code & ERR_HIGHPRESURE)			// 고압감지 에러
    {
        if(COMM_HIGH_PRESS == 0)				// Close
        {
            error_code &= ~ERR_HIGHPRESURE;
            //highpress_error = 0;
        }
    }
	
    if(error_code & ERR_LOWPRESURE)				// 저압감지 에러
    {
        if(COMM_LOW_PRESS == 0)					// Close
        {
            error_code &= ~ERR_LOWPRESURE;
            lowpress_error = 0;
        }
    }

    if(error_code & ERR_EVAFREEZ)				// 에바(증발기)동결 에러
    {
        if(comm_eva_temper >= Flashdatarec.e2p_jesang_reltemp)	// 에바동결 해지 온도 이상이면
        {
            error_code &= ~ERR_EVAFREEZ;
            evafreez_error = 0;
        }
    }

    if(error_code & ERR_EVATEMPER)			// 에바 센서 에러
    {
        if(evatemp_sensor_error == 0)		// 에바온도 센서 에러가 복구되면
            error_code &= ~ERR_EVATEMPER;
    }

    if(error_code & ERR_ROOMHUMI)			// 룸 습도 센서 에러
    {
        if(humi_sensor_error == 0)		// 룸 습도 센서 에러가 복구 되면
            error_code &= ~ERR_ROOMHUMI;
    }

    if(error_code & ERR_ROOMTEMPER)			// 실내온도 센서 에러
    {
        if(roomtemp_sensor_error == 0)		// 룸온도 센서 에러가 복구 되면
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
        Seg_data_set(0, 15);           					// 'F' 표시
        Seg_data_set(1, opt_num);

        icon_led.Bit.curhumi_percent = 0;
        icon_led.Bit.curhumi_char = 0;

        icon_led.Bit.sethumi_percent = 0;
        icon_led.Bit.sethumi_char = 0;
        icon_led.Bit.time_char = 0;
        
        switch(opt_num)
        {
            case 1:										// 습도차 보정 (-9% - +9%)
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
            case 2:										// 습도센서 사용 유무
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
            case 3:										// 제상모드 진입 온도설정. (제상진입온도 초기값 -1도), (범위 : -5도 ~ 7도)
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
            case 4:										 // FAN OFF 지연설정
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
            case 5:										//  FAN OFF 지연 시간 설정
                temp = Flashdatarec.e2p_fandelay_time / 10;
                if(temp == 0)
                    Seg_data_set(2, 18);            // Blank
                else
                    Seg_data_set(2, temp);

                temp = Flashdatarec.e2p_fandelay_time % 10;
                Seg_data_set(3, temp);
                break;
            case 6:										//   냉매저압 E7 검출 기능
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
            case 7:										// 제상 해제 온도
                temp = Flashdatarec.e2p_jesang_reltemp / 10;
                if(temp == 0)
                    Seg_data_set(2, 18);                    // Blank
                else
                    Seg_data_set(2, temp);

                temp = Flashdatarec.e2p_jesang_reltemp % 10;
                Seg_data_set(3, temp);
                break;
            case 8:										// 제상 해제 시간
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
    else if(error_code > 0)                             // Error 표시
    {
        Error_Disp();
    }
    else
    {
        icon_led.Bit.icon_check = 0;                    // 점검 LED

        if(flg_offreserve || flg_onreserve)				// 꺼짐, 켜짐 예약
            icon_led.Bit.icon_reserve = 1;              // 예약 LED
        else
            icon_led.Bit.icon_reserve = 0;                      

        switch(disp_mode)
        {
            case DISP_WAIT:					  // Stop 모드
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
                
                if(flg_onreserve)					  // 켜짐 예약이 존재
                    Disp_OnOffReservTime(1);                // 켜짐 예약 시간 표시
                else
                {
                    Seg_data_set(2, 18);
                    Seg_data_set(3, 18);
                    icon_led.Bit.time_char = 0;
                }
                icon_led.Bit.sethumi_char = 0;
                icon_led.Bit.sethumi_percent = 0;
                break;
            case DISP_OPERATION:                       		// 제습/송풍 운전
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
                else if(flg_offreserve)							// 꺼짐 예약이 존재
                {
                    Disp_OnOffReservTime(0);				// 꺼짐 예약 시간 표시
                    icon_led.Bit.sethumi_char = 0;
                    icon_led.Bit.sethumi_percent = 0;
                }
                else
                {
                    switch(op_kind)
                    {
                        case DEHUMI:							// 제습
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
                        case WIND:								// 송풍
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
            case DISP_DEHUMI_SET:                               // 제습온도설정
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
            case DISP_ON_RESERVE:                                     // 켜짐예약
            case DISP_OFF_RESERVE:					// 꺼짐예약
                Seg_data_set(0, 21);           				// 'H.' 표시
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
                    if(temp)						// 나머지가 있다면
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
    //			case DISP_EVAROOM_TEMP:                               // 에바토출온도보기
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

    if(op_kind == WIND)										// 송풍운전시 
    {
        evafan_offtime = 0;
        flg_evafan_on = 0;									// 바로 정지
    }
    else
    {
        if(Flashdatarec.e2p_fanoff_delay)                   // F4 옵션
        {
            if((evafan_offtime == 0) && flg_evafan_on)
            {
                if(Flashdatarec.e2p_fandelay_time == 0)
                    flg_evafan_on = 0;									// 바로 정지
                else
                {
                    if(flg_short_circuit)
                        evafan_offtime = Flashdatarec.e2p_fandelay_time;
                    else
                        evafan_offtime = Flashdatarec.e2p_fandelay_time * 60;		// Delay 이후 송풍팬 정지
                }
            }
        }
        else
        {
            evafan_offtime = 0;
            flg_evafan_on = 0;									// 바로 정지
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
// 키 입력 처리
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
                    onreserve_timeout = onreserve_settime * 60;			// 분으로 환산
                
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
    if(flg_remote_controle)		// 원격제어 
    {
        if(COMM_REMOTE)			// 외부 스위치 Open
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
    if(key_continued[UP_KEY] && key_continued[DOWN_KEY] && (error_code & (ERR_EVATEMPER | ERR_ROOMHUMI)))					// 비상운전 올림 + 내림 3초
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

                op_kind = DEHUMI;			// 제습 모드
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
    if(op_mode == STOP_MODE)			// 정지모드
    {
        if(menu_key[OPERSTOP_KEY] && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0))      // 운전 Key
        {
            if(disp_mode == DISP_ON_RESERVE)		// 디스플레이 모드가 예약 일경우
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
        else if((menu_key[RESERVE_KEY]) && (menu_key[DEHUMI_KEY] == 0) && (key_pushed[DEHUMI_KEY] == 0) && (error_code == 0))		// 예약 키 (켜짐예약)
        {
            if((disp_mode == DISP_WAIT) || (disp_mode == DISP_ON_RESERVE))
            {
                if(disp_mode == DISP_ON_RESERVE)		// 디스플레이 모드가 예약 일경우
                {	
                    disp_mode = prev_disp_mode;
                    flg_onreserve = 0;
                    onreserve_settime = 0;
                }
                else								// 디스플레이 모드가 예약이 아니라면
                {
                    prev_disp_mode = disp_mode;
                    disp_mode = DISP_ON_RESERVE;
                    onreserve_settime = 3;					// 3 시간
                    optkey_timeout = 5;
                }
            }
            Play_Buzzer(1);						// Button
        }
        else if((menu_key[DOWN_KEY] || key_continued[DOWN_KEY]) && (key_pushed[UP_KEY] == 0) && 
                (key_pushed[DEHUMI_KEY] == 0) && ((disp_mode == DISP_ON_RESERVE) || (disp_mode == DISP_OPTION)))				// Down 키
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

                if(disp_mode == DISP_ON_RESERVE)		// 디스플레이 모드가 예약 일경우
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
                (key_pushed[DEHUMI_KEY] == 0) && ((disp_mode == DISP_ON_RESERVE) || (disp_mode == DISP_OPTION)))				// Up 키
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

                if(disp_mode == DISP_ON_RESERVE)		// 디스플레이 모드가 예약 일경우
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
    else											// 운전모드
    {
        if(key_continued[DEHUMI_KEY] && key_continued[UP_KEY] && (menu_key[RESERVE_KEY] == 0))	// 제습 + 올림 키	Option Mode
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
        else if(menu_key[OPERSTOP_KEY])      			// 운전
        {
            if(disp_mode == DISP_OFF_RESERVE)		// 디스플레이 모드가 예약 일경우
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
                e2p_save_timeout = 10;											// 1초 후 저장
                Play_Buzzer(1);						
            }
            else
            {
                Stop_Proc();
                Play_Buzzer(2);						// Power Off
            }
        }
        else if(menu_key[DEHUMI_KEY] && (menu_key[RESERVE_KEY] == 0)  && (disp_mode != DISP_EVAROOM_TEMP) && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0) && (error_code == 0))				// 제습 키
        {
            if(disp_mode == DISP_OPTION)
            {
                if(flg_keyall_off)
                {
                    optkey_timeout = 10;
                    opt_num++;
                    if(opt_num > 8)
                        disp_mode = prev_disp_mode;
                    e2p_save_timeout = 10;												// 1초 후 저장
                    Play_Buzzer(1);						// Button
                }
            }
            else if(op_kind == WIND)
            {
                op_kind = DEHUMI;			// 제습 모드
                Play_Buzzer(1);						
            }
            else
                Play_Buzzer(0);						
        }
        else if(menu_key[BLOW_KEY] && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0)  && (error_code == 0) && (disp_mode != DISP_OPTION))				// 송풍 키
        {
            if((disp_mode != DISP_ROOM_TEMP) && (disp_mode != DISP_EVAROOM_TEMP))
            {
                if(op_kind == DEHUMI)
                {
                    op_kind = WIND;			// 송풍 모드
                    Play_Buzzer(1);						
                }
                else
                    Play_Buzzer(0);						
            }
        }
        else if(menu_key[STRONG_KEY] && (key_pushed[WEAK_KEY] == 0) && (key_pushed[UP_KEY] == 0) && (key_pushed[DOWN_KEY] == 0)  
                && (disp_mode != DISP_ROOM_TEMP) && (error_code == 0) && (disp_mode != DISP_OPTION))				// 강 키
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
                && (disp_mode != DISP_ROOM_TEMP) && (error_code == 0) && (disp_mode != DISP_OPTION))					// 약 키
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
                && (key_pushed[DOWN_KEY] == 0)  && (error_code == 0) && (disp_mode != DISP_OPTION))					// 예약 키 (꺼짐예약)
        {
            if(disp_mode == DISP_OFF_RESERVE)		// 디스플레이 모드가 예약 일경우
            {	
                disp_mode = DISP_OPERATION;
                flg_offreserve = 0;
                offreserve_settime = 0;
            }
            else								// 디스플레이 모드가 예약이 아니라면
            {
                disp_mode = DISP_OFF_RESERVE;
                offreserve_settime = 3;
                optkey_timeout = 5;
            }
            Play_Buzzer(1);						// Button
        }
        //--------------------------------------------------------------------------------------------------------------------
        else if(key_continued[DEHUMI_KEY] && key_continued[DOWN_KEY] && (disp_mode != DISP_OPTION))				// 에바 온도 표시 제습 + 내림 3초
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
        else if(key_continued[STRONG_KEY] && key_continued[WEAK_KEY] && (disp_mode != DISP_OPTION))			// 현재 온도 표시 강 + 약 3초
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
        else if(key_continued[DEHUMI_KEY] && key_continued[RESERVE_KEY])			// COMP 강제동작 예약 + 제습 3초
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
        else if((menu_key[DOWN_KEY] || key_continued[DOWN_KEY]) && (disp_mode != DISP_EVAROOM_TEMP) && (key_pushed[UP_KEY] == 0) && (key_pushed[DEHUMI_KEY] == 0) && (error_code == 0))				// Down 키
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

                if(disp_mode == DISP_OFF_RESERVE)		// 디스플레이 모드가 예약 일경우
                {
                    if(offreserve_settime > 1)	
                        offreserve_settime--;	
                    optkey_timeout = 5;
                }
                else if(disp_mode == DISP_OPTION)
                {
                    optkey_timeout = 10;
                    e2p_save_timeout = 10;												// 1초 후 저장
                    if(flg_keyall_off)
                    {
                        switch(opt_num)
                        {
                            case 1:										// 습도차 보정 (-9% - +9%)
                                if(Flashdatarec.e2p_dehumi_cali > -9)
                                    Flashdatarec.e2p_dehumi_cali--;
                                break;
                            case 2:										// 습도센서 사용 유무
                                Flashdatarec.e2p_senuse_onoff = !Flashdatarec.e2p_senuse_onoff;
                                break;
                            case 3:										// 제상 발생 온도
                                if(Flashdatarec.e2p_jesang_temp > -17)
                                    Flashdatarec.e2p_jesang_temp--;
                                break;
                            case 4:										// FAN OFF 지연설정
                                Flashdatarec.e2p_fanoff_delay = !Flashdatarec.e2p_fanoff_delay;
                                break;
                            case 5:										// FAN OFF 지연 시간 설정
                                if(Flashdatarec.e2p_fandelay_time > 0)
                                    Flashdatarec.e2p_fandelay_time--;	// 1분 단위
                                break;
                            case 6:										// 냉매저압 E7 검출 기능
                                Flashdatarec.e2p_E7_check = !Flashdatarec.e2p_E7_check;
                                break;
                            case 7:										// 제상 해제 온도 (0 - 17C)
                                if(Flashdatarec.e2p_jesang_reltemp > 0)
                                    Flashdatarec.e2p_jesang_reltemp--;
                                break;
                            case 8:										// 제상 해제 시간 (1 - 10분)
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
                    switch(op_kind)				// 운전 선택모드
                    {
                        case DEHUMI:			// 제습운전
                            if(set_cur_humi > 30)	// 30-90        습도
                                set_cur_humi--;
                            // eeprom save 필요
                            disp_mode = DISP_DEHUMI_SET;
                            flg_blink = 0;
                            key_updown_timeout = 20;				// 2 sec
                            break;
                        case WIND:				// 송풍운전
                            break;
                        default:
                            break;

                    }
                }
            }
        }
        else if((menu_key[UP_KEY] || key_continued[UP_KEY]) && (key_pushed[DOWN_KEY] == 0) && (key_pushed[DEHUMI_KEY] == 0) && (error_code == 0))				// Up 키
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

                if(disp_mode == DISP_OFF_RESERVE)		// 디스플레이 모드가 예약 일경우
                {
                    if(offreserve_settime < 10)
                        offreserve_settime++;
                    optkey_timeout = 5;	
                }
                else if(disp_mode == DISP_OPTION)
                {
                    optkey_timeout = 10;
                    e2p_save_timeout = 10;												// 1초 후 저장
                    if(flg_keyall_off)
                    {
                        switch(opt_num)
                        {
                            case 1:										// 습도차 보정 (-9% - +9%)
                                if(Flashdatarec.e2p_dehumi_cali < 9)
                                    Flashdatarec.e2p_dehumi_cali++;
                                break;
                            case 2:										// 습도센서 사용 유무
                                Flashdatarec.e2p_senuse_onoff = !Flashdatarec.e2p_senuse_onoff;
                                break;
                            case 3:										// 제상 발생 온도
                                if(Flashdatarec.e2p_jesang_temp < 0)
                                    Flashdatarec.e2p_jesang_temp++;
                                break;
                            case 4:										// FAN OFF 지연설정
                                Flashdatarec.e2p_fanoff_delay = !Flashdatarec.e2p_fanoff_delay;
                                break;
                            case 5:										// FAN OFF 지연 시간 설정
                                if(Flashdatarec.e2p_fandelay_time < 10)
                                    Flashdatarec.e2p_fandelay_time++;	// 1분 단위
                                break;
                            case 6:										// 냉매저압 E7 검출 기능
                                Flashdatarec.e2p_E7_check = !Flashdatarec.e2p_E7_check;
                                break;
                            case 7:										// 제상 해제 온도 (0 - 17C)
                                if(Flashdatarec.e2p_jesang_reltemp < 17)
                                    Flashdatarec.e2p_jesang_reltemp++;
                                break;
                            case 8:										// 제상 해제 시간 (1 - 10분)
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
                    switch(op_kind)				// 운전 선택모드
                    {
                        case DEHUMI:			// 제습운전
                            if(set_cur_humi < 90)	// 10-90
                                set_cur_humi++;
                            // eeprom save 필요
                            disp_mode = DISP_DEHUMI_SET;
                            flg_blink = 0;
                            key_updown_timeout = 20;				// 2 sec
                            break;
                        case WIND:				// 송풍운전
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
// 현재온도 표시
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

// 에바온도 표시
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
// 습도 표시
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

                if(Irmask == 0x01)		// 마지막이면
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
        //----------------------------------------------------------------   예약
        if(flg_offreserve)				// 꺼짐 예약이 있다
        {
            if(offreserve_timeout == 0)	// 예약 시간이 되었다면
            {
                menu_key[OPERSTOP_KEY] = 1;	// 운전
                flg_offreserve = 0;
            }
        }

        //----------------------------------------------------------------  운전
        if((error_code == 0) && (eva_freez_detect == 0))
        {
            switch(op_kind)										// 운전 선택모드
            {
                case DEHUMI:									// 제습운전
                    if(!((comm_cur_temper <= 15) && (Flashdatarec.e2p_senuse_onoff)))
                    {
                        curtemp_error_timeout = 15;                             // 1초
                        
                        if(comm_cur_humi >= (Flashdatarec.e2p_sethumi + 3))  	// 현재 습도가 설정습도 + 3이상일 경우
                        {
                            flg_comp_on = 1;										// Comp ON
                            evafan_offtime = 0;
                            flg_evafan_on = 1;                                      // 송풍팬 ON
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
                                    flg_evafan_on = 1;                                      // 송풍팬 ON
                                }
                            }
                        }
                        
                        if(Flashdatarec.e2p_fanoff_delay == 0)                      // F4 Off시 운전시 계속 가동
                        {
                            evafan_offtime = 0;
                            flg_evafan_on = 1;                                      // 송풍팬 ON
                        }
                    }
                    else    // 제습운전 동작 안됨
                    {
                        if(curtemp_error_timeout == 0)
                            menu_key[OPERSTOP_KEY] = 1;	// 운전 OFF
                    }
                    break;
                case WIND:										// 송풍운전
                    evafan_offtime = 0;
                    flg_evafan_on = 1;							// 송풍팬 ON
                    flg_comp_on = 0;							// Comp OFF
                    curtemp_error_timeout = 15; 
                    break;
                default:
                    break;
            }
        }

        //----------------------------------------------------------------  고압 에러
        if(highpress_detect)					// 고압감지 이면
        {
            flg_comp_on = 0;				// Comp OFF
            evafan_offtime = 0;
            flg_evafan_on = 1;

            error_code |= ERR_HIGHPRESURE;
        }
        //----------------------------------------------------------------  저압감지
        if(lowpress_error)					// 저압에러 이면
        {
            flg_comp_on = 0;				// Comp OFF
            evafan_offtime = 0;
            flg_evafan_on = 1;
            error_code |= ERR_LOWPRESURE;
        }
        //----------------------------------------------------------------  에바온도 센서 에러
        if(evatemp_sensor_error)				
        {
            error_code |= ERR_EVATEMPER;
            if(flg_emergency_oper)
            {
                compdelay_timeout = 0;
                flg_comp_on = 1;
                evafan_offtime = 0;
                flg_evafan_on = 1;
                
                if(COMM_DIP_SW1)					// Dip S/W OFF (일반용)
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
                        evadelay_timeout = 240;				// 4분
                    flg_evatemp_first = 1;
                }
                else
                {
                    if(evadelay_timeout == 0)
                    {
                        if((op_mode == RUN_MODE) && (flg_emergency_oper == 0))
                        {
                            menu_key[OPERSTOP_KEY] = 1;	// 운전 OFF
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
        //----------------------------------------------------------------  에바 동결 에러
        if(eva_freez_detect && (evatemp_sensor_error == 0))					// 에바 동결 감지
        {
            if(COMM_DIP_SW1)					// Dip S/W OFF (일반용)
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
            flg_evafan_on = 1;					// 송풍팬 동작

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
                if((comm_eva_temper >= Flashdatarec.e2p_jesang_reltemp)	&& (eva_error_timeout == 0))    // 에바동결 해지 온도 이상이면
                {
                    eva_freez_detect = 0;
                    flg_solvalve_on = 0;

                    if(op_kind == DEHUMI)			// 제습모드이면
                        flg_comp_on = 1;				// Comp ON

                    flg_evafan_on = 1;                          // 송풍팬 동작
                }
            }
        }
        else
            flg_evafreez_detect_first = 0;
        //--------------------------------------------------------------------------------		
    }
    else	// STOP_MODE
    {
        if(flg_onreserve)				// 켜짐 예약이 있다
        {
            if(onreserve_timeout == 0)	// 예약 시간이 되었다면
            {
                menu_key[OPERSTOP_KEY] = 1;	// 운전
                flg_onreserve = 0;
            }
        }
    }
    //----------------------------------------------------------------  만수위(물넘침) 에러
    if(waterlevel_sensor_error)
    {
        if(flg_overwater_first)
        {
            error_code |= ERR_OVERWATER;

            flg_overwater_first = 0;
            flg_comp_on = 0;										// Comp OFF
            evafan_offtime = 0;
            flg_evafan_on = 0;										// 송풍팬 OFF
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
                flg_evafan_on = 1;                                      // 송풍팬 ON
            }
            Buzzer_Stop();
        }
    }
    //----------------------------------------------------------------  습도 센서 에러
    if(humi_sensor_error || roomtemp_sensor_error)                                               
    {
        curtemp_error_timeout = 15; 
        
        if(flg_emergency_oper)
        {
            compdelay_timeout = 0;
            flg_comp_on = 1;
            evafan_offtime = 0;
            flg_evafan_on = 1;
            
            if(COMM_DIP_SW1)					// Dip S/W OFF (일반용)
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
                    humidity_timeout = 240;                 // 4분
                flg_roomhumi_first = 1;
                error_code |= ERR_ROOMHUMI;            
            }
            else
            {
                if(humidity_timeout == 0)
                {
                    if((op_mode == RUN_MODE) && (flg_emergency_oper == 0))
                    {
                        menu_key[OPERSTOP_KEY] = 1;	// 운전 OFF
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
    //----------------------------------------------------------------  룸온도 센서 에러
    /*
    if(roomtemp_sensor_error)				// 룸온도 센서 에러
    {
        error_code |= ERR_ROOMTEMPER;
        if(flg_emergency_oper == 0)
            flg_comp_on = 0;				// Comp OFF

        if(flg_roomtemp_first == 0)
        {
            flg_comp_on = 0;				// Comp OFF
            evafan_offtime = 0;	
            flg_evafan_on = 1;			
            roomtemp_timeout = 240;			// 4분
            flg_roomtemp_first = 1;
        }
        else
        {
            if(roomtemp_timeout == 0)
            {
                if(op_mode == RUN_MODE)
                {
                    menu_key[OPERSTOP_KEY] = 1;	// 운전 OFF
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
        compdelay_timeout = 180;			// comp 정지 후 재가동 지연시간 3분 초단위로 변환
    evafan_mode = 1;                // 송풍팬 모드 전원인가 후 강으로 동작
}

void Disp_Version(void)
{
    Seg_data_set(0, 13);     //'d'
    Seg_data_set(1, 21);     //'H'
    Seg_data_set(2, 1);
    Seg_data_set(3, 9);
}

