#include "douji.h"
#include "headfile.h"
#include "math.h"

#define LOW_PWM 1430
#define HIGH_PWM 1790
#define MID_PWM 1630
//#define DEBUG
//#define DSPDA
struct pid
{
  double P;
  double D;
  double out;
};
static double pre_err=0,err=0;
struct pid douji_pid={
  .P = 6,
  .D = 1
};
 /*
  image_ang----飞机上的图像角
  hmc_ang-----飞机上的hmc角
  air_hmc-----飞机上的hmc角

*/
uint8 ser_con(double *image_ang,double *hmc_ang,double *air_hmc,double *err_adjust)
{
  uint32 pwm_out;
#ifdef DEBUG
  
  printf("before adjust air_img:%f  air_hmc:%f car_hmc:%f \n",*image_ang,*air_hmc,*hmc_ang);
#endif
  //*image_ang = *image_ang - *err_adjust;
  angle_adjust(hmc_ang,err_adjust);
  //err = (*image_ang - *hmc_ang + 180)%180;
  get_err_ang(hmc_ang,image_ang,&err);
//  if(*image_ang < *hmc_ang)
//  {
//    err = 0- err;
//  }
  

    douji_pid.out = MID_PWM + douji_pid.P*err+ \
    douji_pid.D * (err - pre_err);
    
    pwm_out = (uint32)douji_pid.out;  
  
  
  pre_err = err;
#ifdef DEBUG
  
  printf("after_adjust--image_angle:%f hmc_angle:%f  err_angle:%f \n",*image_ang,*hmc_ang,err);
#endif
//限幅控制
  
  
  if(pwm_out < LOW_PWM) {pwm_out = LOW_PWM;}
  if(pwm_out > HIGH_PWM){pwm_out = HIGH_PWM;}
  
  ftm_pwm_duty(ftm1,ftm_ch1,pwm_out);
  return 1;

}

uint8 get_err_ang(double *angle1,double *angle2,double *result)
{
  double angle = 0,bu_angle = 0;
#ifdef DSPDA
   float32_t ang1 = (float32_t)*angle1,\
             ang2 = (float32_t)*angle2,\
             ret;
   arm_sub_f32(&ang1,&ang2,&ret,0);
   *result = (double)ret;
   
#endif
#ifndef DSPDA
  angle = *angle1 - *angle2;
#endif 
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


void ca_dis(Node *pointa,Node *pointb,float *dis)
{
 // float dis=0;
  
  *dis = (pointa->x-pointb->x)*(pointa->x-pointb->x)+(pointa->y-pointb->y)*(pointa->y-pointb->y);
#ifdef DEBUG  
  printf("distancd: %f  \n",dis);
#endif
  //return dis;
}


