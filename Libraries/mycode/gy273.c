#include "gy273.h"
#include "headfile.h"
#include "math.h"



/**
*��������GY273Init
*���ܣ���ʼ��gy273
**/
void Gy273Init(void)
{
 // uint8 reg=0;
//IIC��ʼ��
  
  //IIC_init();
  i2c_init(i2c0,400*1000);
  
//ģ���ʼ��
  //simiic_write_reg(SLAVEADRESS,MODE,SINGLE);
  //reg = simiic_read_reg(SLAVEADRESS,MODE,IIC);
  
  i2c_write_reg(i2c0,SLAVEADRESS,MODE,CONTINUOUS);
  //reg = i2c_read_reg(i2c0,SLAVEADRESS,MODE);
  
 // i--;
}
/**

*������ ��Multiple_read_HMC5883
*���ܣ���ȡx,y,z�Ĵ�����ֵ
������buff  �洢���� ����6��

**/


uint8 Read_HMC5883(uint8 *buff,double *angle)
{ 
  //if(sizeof(buff) < 6 | sizeof(angle) < 3)
 // { 
 //   return 0;
 // }
 // uint8 i;
  int x,y,z;
  
 // i2c_write_reg(i2c0,SLAVEADRESS,MODE,CONTINUOUS);
  
  buff[0] = i2c_read_reg(i2c0,SLAVEADRESS,DATAREGISTER0);
  buff[1] = i2c_read_reg(i2c0,SLAVEADRESS,DATAREGISTER1);
  buff[2] = i2c_read_reg(i2c0,SLAVEADRESS,DATAREGISTER2);
  buff[3] = i2c_read_reg(i2c0,SLAVEADRESS,DATAREGISTER3);
  buff[4] = i2c_read_reg(i2c0,SLAVEADRESS,DATAREGISTER4);
  buff[5] = i2c_read_reg(i2c0,SLAVEADRESS,DATAREGISTER5);
  
  x=buff[0] << 8 | buff[1]; //Combine MSB and LSB of X Data output register  �����Чλ
  y=buff[4] << 8 | buff[5]; //Combine MSB and LSB of Z Data output register
  //z=buff[2] << 8 | buff[3]; //Combine MSB and LSB of Y Data output register
  
  if(x>0x7fff)x-=0xffff;	  
  if(y>0x7fff)y-=0xffff;	 
  
  //printf("%d %d %d ",x,y,z);
  
  angle[0] = atan2((double)y,(double)x) * (180 / PI)+180; //����XYƽ��Ƕ�
  //angle[1] = atan2((double)z,(double)x) * (180 / 3.14159265) + 180; //����XZƽ��Ƕ�
  //angle[2] = atan2((double)y,(double)z) * (180 / 3.14159265) + 180; //����ZYƽ��Ƕ�
  
  //angle[0]*=100;
  //angle[1]*=10;
 // angle[2]*=10;
  return 1;
  
}


