#include "headfile.h"
#include "road.h"

uint8_t hand_ser_flag = 0; //0--��1---��
uint8_t hand_speed_flag = 0;//0---ͣ��1---��

void hand_nrf_data(uint8 *buff)
{
  if(buff[0] = 1)
  {
    if(buff[1] == 'f') hand_speed_flag = 1;
    else if(buff[1] == 's') hand_speed_flag = 0;
    else if(buff[1] == 'b') hand_speed_flag = 2;
    else if(buff[1] == 'r') hand_ser_flag = 1;
    else if(buff[1] == 'l') hand_ser_flag = 0;
    else if(buff[1] == 'r') hand_ser_flag = 2;
    
  }

}