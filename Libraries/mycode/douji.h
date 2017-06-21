#ifndef _DOUJI_H_
#define _DOUJI_H_
#include "headfile.h"
uint8 ser_con(double *image_ang,double *hmc_ang,double *err_adjust);
uint8 get_err_ang(double *angle1,double *angle2,double *result);
uint8 angle_adjust(double *angle,double *adjust_num);
#endif



