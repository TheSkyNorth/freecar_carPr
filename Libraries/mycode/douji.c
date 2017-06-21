#include "douji.h"
#include "headfile.h"
#include "math.h"

#define LOW_PWM 2080
#define HIGH_PWM 2520


struct pid
{
  double P;
  double I;
  double D;
  double out;
};
static double pre_err=0,err=0;
struct pid douji_pid={
  .P = 6,
  .D = 1
};
 
uint8 ser_con(double *image_ang,double *hmc_ang,double *err_adjust)
{
  uint32 pwm_out;
  //*image_ang = *image_ang - *err_adjust;
  angle_adjust(hmc_ang,err_adjust);
  //err = (*image_ang - *hmc_ang + 180)%180;
  get_err_ang(hmc_ang,image_ang,&err);
//  if(*image_ang < *hmc_ang)
//  {
//    err = 0- err;
//  }
  
  douji_pid.out = 2290 + douji_pid.P*err+ \
    douji_pid.D * (err - pre_err);
  
  pwm_out = (uint32)douji_pid.out;
  pre_err = err;
  printf("image_angle:%f hmc_angle:%f  err_angle:%f \n",*image_ang,*hmc_ang,err);
  //ÏÞ·ù¿ØÖÆ
  if(pwm_out < LOW_PWM) {pwm_out = LOW_PWM;}
  if(pwm_out > HIGH_PWM){pwm_out = HIGH_PWM;}
  
  ftm_pwm_duty(ftm1,ftm_ch1,pwm_out);
  return 1;

}

uint8 get_err_ang(double *angle1,double *angle2,double *result)
{
  double angle = 0,bu_angle = 0;
  angle = *angle1 - *angle2;
  if(fabs(angle) > 180)
  {
    bu_angle = 360.0 - fabs(angle);
    if(angle > 0)
    {
      bu_angle = 0.0 - bu_angle;
    }
    *result = bu_angle;
  }
  else
  {
    *result = angle;
  }
  return 1;
}
/*
  angle = angle - adjust_num
*/
uint8 angle_adjust(double *angle,double *adjust_num)
{
  /*
  if(*angle >= *adjust_num)
  {
    *angle = *angle - *adjust_num;
  }else
  {
    *angle = 360 - *adjust_num + *angle;
  
  }
  */
  
  if(*adjust_num > 0)
  {
    *angle = *angle + *adjust_num;
    if(*angle > 360)
    {
      *angle = *angle - 360;
    }
    
  }else
  {
    *angle = *angle +*adjust_num;
    if(*angle < 0)
    {
      *angle = 360 + *angle;
    }
  }
  return 1;
}





