/* 
 * File:   buzzer.h
 * Author: jjy99
 *
 * Created on 2021년 3월 15일 (월), 오후 1:37
 */

#ifndef BUZZER_H
#define	BUZZER_H
#include "Main.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define	BASETIME	10      //4msec        //10
/*
#define O6_DO       1048
#define O6_RE       1176
#define O6_MI       1320
#define O6_FA       1396
#define O6_SO       1568
#define O6_RA       1760
#define O6_CI       1980
#define O7_DO       2096
#define O7_RE       2352
#define O7_MI       2640
#define O7_FA       2792
#define O7_SO       3136
*/
#define O6_DO       118
#define O6_DOb      112
#define O6_RE       105
#define O6_REb      100
#define O6_MI       94
#define O6_FA       89
#define O6_FAb      83
#define O6_SO       79
#define O6_SOb      74
#define O6_RA       70
#define O6_RAb      66
#define O6_CI       62
#define O7_DO       59
#define O7_DOb      55
#define O7_RE       52
#define O7_REb      49
#define O7_MI       46
#define O7_FA       44
#define O7_FAb      41
#define O7_SO       39
#define O7_SOb      36
#define O7_RA       34
#define O7_RAb      32
#define O7_CI       30
#define O8_DO       29
#define O8_DOb      28
#define O8_RE       26

#define O5_RE        O7_RAb
/*
#define	Tempo_0		0x00
#define Tempo_h8    0x01        //1
#define Tempo_h4    0x02        //2
#define Tempo_h2    0x03        //4
#define Tempo_1     0x04        //8
#define Tempo_1h2   0x05        //12
#define Tempo_2     0x06        //16
#define	Tempo_2h2   0x07        //20
#define Tempo_3     0x08        //24
#define	Tempo_3h2   0x09        //28
#define Tempo_4     0x0a        //32
#define Tempo_4h2   0x0b        //36
#define	Tempo_5     0x0c        //40
#define Tempo_5h2   0x0e        //44
#define Tempo_6     0x0f        //48
*/
//****     쉬는 박자    ***********//
#define	CTempo_h8	0x10        //1
#define CTempo_h4   0x20        //2
#define CTempo_h2	0x30        //4
#define CTempo_1    0x40        //8
#define CTempo_1h2  0x50        //12
#define CTempo_2    0x60        //16
#define CTempo_2h2  0x70        //20
#define CTempo_3    0x80        //24
#define CTempo_3h2  0x90        //28
#define CTempo_4    0xa0        //32
#define CTempo_4h2  0xb0        //36
#define CTempo_5    0xc0        //40
#define CTempo_5h2	0xe0        //44
#define CTempo_6    0xf0        //48

void Buzzer_Sequence(void);
void Play_Buzzer(uint8_t play_num);
void Buzzer_Stop(void);
void PWM_Config(uint16_t cur_period);




#ifdef	__cplusplus
}
#endif

#endif	/* BUZZER_H */

