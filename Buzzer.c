#include <htc.h>
#include "main.h"
#include "buzzer.h"

uint8_t  	on_delay = 0;
uint8_t  	off_delay = 0;
uint8_t     bs_num = 99;
uint8_t  	beep_num = 0;
uint16_t  	*psound_data;
uint16_t   	sound_size;

const uint8_t  TempoValue[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 48, 96, 144, 192, 240};

const uint16_t  STBL_PwrConnect[6][2] = {
  {O6_MI, 0x21}, 
  {O6_FA, 0x21},
  {O6_SO, 0x21},
  {O6_RA, 0x21},
  {O6_CI, 0x21},
  {O7_DO, 0x4F}
};

const uint16_t  STBL_PwrOn[3][2] = {
      {O7_DO, 0x22},
      {O7_MI, 0x22},
      {O7_SO, 0x1F}
};

const uint16_t  STBL_PwrOff[3][2] = {
      {O7_SO, 0x46},
      {O7_MI, 0x46},
      {O7_DO, 0x2F}
};

const uint16_t  STBL_Not[3][2] = {
      {O6_CI, 0x12},
      {O6_CI, 0x15},
      {O6_CI, 0x1f}
};

const uint16_t  STBL_Button[1][2] = {
      {O7_FA, 0x1f}
};



//==============================================================================
void Buzzer_Sequence(void)
{
     uint16_t cur_period;

     switch(bs_num)
     {
          case 0:
              if(sound_size > 0)
              {
                  cur_period = *(psound_data + beep_num);		// 주파수
                  PWM_Config(cur_period);
                  beep_num++;
                  cur_period = *(psound_data + beep_num);		// 쉼표 기간
                  on_delay = TempoValue[cur_period >> 4];		// 앞
                  off_delay = TempoValue[cur_period & 0x0F];	// 뒤

                  BUZZERCONTROL = 1; 		// buzzer on
                  bs_num = 1;
                  sound_size--;
              }
              else
              {
                  Buzzer_Stop();
                  bs_num = 99;
              }
              break;
          case 1:
              if(on_delay > 0)
                  on_delay--;
              else
              {
                  BUZZERCONTROL = 0; 				// buzzer off
                  bs_num = 2;
              }
              break;
          case 2:                                      // Echo
              if(off_delay > 0)
                  off_delay--;
              else
              {
                  beep_num++;
                  bs_num = 0;
              }
              break;
          default:
              break;
     }
}

void Play_Buzzer(uint8_t play_num)
{
	BUZZERCONTROL = 0; 				// buzzer off

    switch(play_num)
    {
        case 0:
            psound_data = (uint16_t *)(STBL_Not);
            sound_size = sizeof(STBL_Not) / 4;
            break;
        case 1:
            psound_data = (uint16_t *)(STBL_Button);
            sound_size = sizeof(STBL_Button) / 4;
            break;
        case 2:
            psound_data = (uint16_t *)(STBL_PwrOff);
            sound_size = sizeof(STBL_PwrOff) / 4;
            break;
        case 3:
            psound_data = (uint16_t *)(STBL_PwrOn);
            sound_size = sizeof(STBL_PwrOn) / 4;
            break;
        case 4:
            psound_data = (uint16_t *)(STBL_PwrConnect);
            sound_size = sizeof(STBL_PwrConnect) / 4;
            break;
        default:
            break;
    }
    beep_num = 0;
    bs_num = 0;
}

void Buzzer_Stop(void)
{
    CCP2CON=0;
    BUZZERCONTROL = 0; 				// buzzer off
    TMR2ON = 0;  
    BUZZER = 0;
    TMR2IE = 0;
}

void PWM_Config(uint16_t cur_period)
{
	CCP2CON = 0b00001111; 
	PR2 = cur_period; 
	CCPR2L = cur_period >> 1;
	TMR2 = 0;					// Timer2 Clear
	TMR2ON = 1; 
}


