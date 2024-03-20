/* 
 * File:   dehumi.h
 * Author: jjy99
 *
 * Created on 2021�� 3�� 15�� (��), ���� 1:40
 */

#ifndef DEHUMI_H
#define	DEHUMI_H

#ifdef	__cplusplus
extern "C" {
#endif

#define IR_PWR          0x44BBC03F		// ��������
#define IR_WIND         0x44BB50AF		// ��ǳ
#define IR_TURBO        0x44BBB847		// ����
#define IR_RESERVE      0x44BBF807		// ����
#define IR_UP           0x44BB9867		// �ø�
#define IR_DOWN         0x44BB7887		// ����
#define IR_DEHUMI       0x44BBB04F		// ����

#define ERR_OVERWATER   0x0001			// ����ħ ����
#define ERR_HIGHPRESURE 0x0002			// ��а��� ����
#define ERR_LOWPRESURE  0x0004			// ���а��� ����
#define ERR_EVAFREEZ    0x0008			// ����(���߱�)���� ����
#define ERR_EVATEMPER   0x0010			// ���� ���� ����
#define ERR_ROOMHUMI    0x0020			// ���� ���� ����
#define ERR_ROOMTEMPER  0x0040			// �ǳ��µ� ���� ����
#define ERR_COMM485     0x0080			// 485 ��ſ���

    enum {
        STOP_MODE = 0, RUN_MODE
    };

    enum {
        DEHUMI = 0, WIND
    }; // �������� ���
    // DISPLAY MODE
    // �ù�/��ǳ/���� ����, �������༳��, �������༳��, ����µ�����, �ǳ��µ�����, �����µ�����
    // ��������µ�����, �ɼǼ��� C1-8

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

