#ifndef _DATA_TRANSFER_H
#define	_DATA_TRANSFER_H

//#include "stm32f4xx.h"
#include "headfile.h"

#define u8 uint8_t 
#define u16 int16_t 


typedef struct 
{
		u8 send_version;
		u8 send_status;
		u8 send_senser;
		u8 send_pid1;
		u8 send_pid2;
		u8 send_pid3;
		u8 send_pid4;
		u8 send_pid5;
		u8 send_pid6;
		u8 send_rcdata;
		u8 send_offset;
		u8 send_motopwm;
		u8 send_power;

}dt_flag_t;

extern dt_flag_t f;

void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8);
void ANO_DT_Send_Senser(u16 a_x,u16 a_y,u16 a_z,u16 g_x,u16 g_y,u16 g_z,u16 m_x,u16 m_y,u16 m_z,uint32 bar);


#endif

