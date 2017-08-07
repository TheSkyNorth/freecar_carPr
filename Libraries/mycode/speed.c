#include "speed.h"
#include "headfile.h"

extern uint16 speed_count;
struct PID_ST{
  float p;
  float i;
  float d;
  float a1;
  float a2;
  float a3;
};
struct PID_ST sp_pid={
  .p = 19,
  .i = 8,
  .d = 0
};

void pid_init(void)
{
  sp_pid.a1 = sp_pid.p + sp_pid.i+sp_pid.d;
  sp_pid.a2 = sp_pid.d - sp_pid.p;
  sp_pid.a3 = 2*sp_pid.d;
}
float P = 19,I = 8,D = 0;

float err,pre_err=0,pre_2_err=0;

float pwm_out;
uint32 motor_pwm;
static uint8 pid_init_flag = 0;
void speed_set(float speed_set)
{
  float pid_out = 0;
  if(!pid_init_flag)
  {
    pid_init_flag = 1;
    pid_init();
  }
  err = speed_set - (float)speed_count;
  /*
  pid_out = P*(err - pre_err) + \
    I*err +\
      D*(err - 2*pre_2_err+pre_err); 
  */
  pid_out = sp_pid.a1*err+\
    sp_pid.a2*pre_err+\
      sp_pid.a3*pre_2_err;              //¼ÆËãpidout
  
  pre_2_err = pre_err;
  pre_err = err;//±£´æÎó²îÖµ
  
  pwm_out = pid_out + (float)motor_pwm;
  
  if(pwm_out >9900)
  {
    pwm_out = 9900;
  }
  if(pwm_out < -9900)
  {
    pwm_out = -9900;
  }
  if(pwm_out >=0)
  {
    motor_pwm = (uint32)pwm_out;
    ftm_pwm_duty(ftm2,ftm_ch1,motor_pwm);
  }
  else
  {
    pwm_out = 0-pwm_out;
    motor_pwm = (uint32)pwm_out;
    ftm_pwm_duty(ftm2,ftm_ch0,motor_pwm);
    
  }
  
}









