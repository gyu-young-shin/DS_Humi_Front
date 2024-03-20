/* 
 * File:   dehumi.h
 * Author: jjy99
 *
 * Created on 2021년 3월 15일 (월), 오후 1:40
 */

#ifndef DEHUMI_H
#define	DEHUMI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define IR_PWR          0x44BBC03F		// 운전정지
#define IR_WIND         0x44BB50AF		// 송풍
#define IR_TURBO        0x44BBB847		// 강약
#define IR_RESERVE      0x44BBF807		// 예약
#define IR_UP           0x44BB9867		// 올림
#define IR_DOWN         0x44BB7887		// 내림
#define IR_DEHUMI       0x44BBB04F		// 제습

#define ERR_OVERWATER   0x0001			// 물넘침 에러
#define ERR_HIGHPRESURE 0x0002			// 고압감지 에러
#define ERR_LOWPRESURE  0x0004			// 저압감지 에러
#define ERR_EVAFREEZ    0x0008			// 에바(증발기)동결 에러
#define ERR_EVATEMPER   0x0010			// 에바 센서 에러
#define ERR_ROOMHUMI    0x0020			// 습도 센서 에러
#define ERR_ROOMTEMPER  0x0040			// 실내온도 센서 에러
#define ERR_COMM485     0x0080			// 485 통신에러

    enum {
        STOP_MODE = 0, RUN_MODE
    };

    enum {
        DEHUMI = 0, WIND
    }; // 운전선택 모드
    // DISPLAY MODE
    // 냉방/송풍/제습 운전, 켜짐예약설정, 꺼짐예약설정, 토출온도설정, 실내온도설정, 제습온도설정
    // 에바토출온도보기, 옵션설정 C1-8

    enum {
        DISP_WAIT = 0, DISP_OPERATION,
        DISP_ON_RESERVE, DISP_OFF_RESERVE, DISP_TURBO_SETTEMP, DISP_ROOM_TEMP,
        DISP_DEHUMI_SET, DISP_EVAROOM_TEMP, DISP_OPTION
    };

    void IR_Receive_Proc(void);
    void InputCheck_Proc(void);
    void Output_Proc(void);
    void Disp_OnOffReservTime(uint8_t onoff);
    void Error_Disp(void);
    void Error_Recovery_Proc(void);
    void Disp_Segment(void);
    void Stop_Proc(void);
    void InputKey_Proc(void);
    void Disp_Temper(void);
    void Disp_EvaTemper(void);
    void Disp_Humidity(void);
    void IrRxProc(void);
    void Control_Proc(void);
    void Disp_BlowLevel(void);
    void Init_Variable(void);
    void Disp_Version(void);
    void Animation_Blowfan(void);
    void Animation_Dehumi(void);



#ifdef	__cplusplus
}
#endif

#endif	/* DEHUMI_H */

