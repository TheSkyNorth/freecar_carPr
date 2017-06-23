#ifndef _GY273_H
#define _GY273_H


#include "headfile.h"
/** HMC5883L �Ĵ������� **/
#define SLAVEADRESS 0x1E //�豸��ַ
#define MODE    0x02    //ģʽ�Ĵ���

#define CONTINUOUS 0x00 //continuous mode         
#define SINGLE  0x01    //single mode

#define DATAREGISTER0       0x03    
#define DATAREGISTER1       0x04
#define DATAREGISTER2       0x05
#define DATAREGISTER3       0x06
#define DATAREGISTER4       0x07
#define DATAREGISTER5       0x08

//#define PI  3.14159265

#define HMC5883I2C i2c0


uint8 Read_HMC5883(uint8 *buff,double *angle);
void Gy273Init(void);

#endif