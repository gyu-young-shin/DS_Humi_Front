#include <htc.h>
#include <pic16F946.h>
#include "main.h"
#include "Eeprom.h"

#ifndef _XTAL_FREQ
 // Unless already defined assume 4MHz system frequency
 // This definition is required to calibrate __delay_us( ) and __delay_ms( )
 #define _XTAL_FREQ 8000000
#endif

TE2PDataRec Flashdatarec;
uint8_t e2p_data, *pdata;
//===========================================================================================
void EEPROM_Init(void)
{
    uint8_t i;

    e2p_data = Eeprom_Read(E2P_START_ADDRESS);
    
    if(e2p_data != 0xAA)
    {
        Flashdatarec.FA_code = 0xAA;
        Flashdatarec.e2p_flg_oper = 0;               // ������
        Flashdatarec.e2p_dehumi_cali = 0;           // ������ ���� (-9% - +9%)
        Flashdatarec.e2p_sethumi = 50;              // ���� ����
        Flashdatarec.e2p_senuse_onoff = 1;        // �������� ��� ����
        Flashdatarec.e2p_jesang_temp = -1;       // ���� �߻� �µ�
        Flashdatarec.e2p_fanoff_delay = 1;          // FAN OFF ��������
        Flashdatarec.e2p_fandelay_time = 1;       // FAN OFF ���� �ð� ����
        Flashdatarec.e2p_E7_check = 1;              // �ø����� E7 ���� ���
        Flashdatarec.e2p_jesang_reltemp = 5;        // ���� ���� �µ�
        Flashdatarec.e2p_jesang_reltime = 3;        // ���� ���� �ð�

        E2pData_Write();
    }
    else
    {
        pdata = (uint8_t *)(&Flashdatarec);

        for(i=0; i<sizeof(TE2PDataRec); i++)
        {
            *pdata = Eeprom_Read(E2P_START_ADDRESS + i);
            pdata++;
        }
        
        if(Flashdatarec.e2p_sethumi > 90)
        {
            Flashdatarec.FA_code = 0xAA;
            Flashdatarec.e2p_flg_oper = 0;               // ������
            Flashdatarec.e2p_dehumi_cali = 0;           // ������ ���� (-9% - +9%)
            Flashdatarec.e2p_sethumi = 50;              // ���� ����
            Flashdatarec.e2p_senuse_onoff = 1;        // �������� ��� ����
            Flashdatarec.e2p_jesang_temp = -1;       // ���� �߻� �µ�
            Flashdatarec.e2p_fanoff_delay = 1;          // FAN OFF ��������
            Flashdatarec.e2p_fandelay_time = 1;       // FAN OFF ���� �ð� ����
            Flashdatarec.e2p_E7_check = 1;              // �ø����� E7 ���� ���
            Flashdatarec.e2p_jesang_reltemp = 5;        // ���� ���� �µ�
            Flashdatarec.e2p_jesang_reltime = 3;        // ���� ���� �ð�

            E2pData_Write();
        }
    }
}

void E2pData_Write(void)
{
    uint8_t i, *pdata;

    pdata = (uint8_t *)(&Flashdatarec);

    for(i=0; i<sizeof(TE2PDataRec); i++)
    {
        Eeprom_Write(E2P_START_ADDRESS + i, *pdata);
        pdata++;
        __delay_ms(100);
    }
}


uint8_t Eeprom_Read(uint8_t addr)
{
	uint8_t data;
	
	GIE     = 0;         
	EEPGD   = 0;
	EEADR   = addr;
	EECON1  &= 0x3F;
	RD      = 1;
	data  = EEDATA;
	GIE     = 1;
        __delay_ms(10);
	return data;
} 

void Eeprom_Write(uint8_t addr, uint8_t wdata)
{
	while(WR);
	GIE     = 0;         

	EEIF    = 0;
	WREN    = 0;

	EEADR   = addr;
	EEDATA  = wdata;
	EECON1  &= 0x3F;
	wdata   = 0;
	WREN    = 1;            // enable writes
	EECON2  = 0x55;         // required sequence for EEPROM update
	EECON2  = 0xAA;
	WR=1;
	WREN    = 0;

	GIE     = 1;
}



