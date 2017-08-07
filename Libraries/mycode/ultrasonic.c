#include "ultrasonic.h"
#include "headfile.h"


void   Ultrasonic_Delay(uint32_t n);  //超声波延时us级函数


void ultrasonic_init(void)
{
  gpio_init(B0,GPO,0);//Tx
  
  port_init(B1,IRQ_RISING | ALT1 | PULLDOWN );
  
  
  
  set_irq_priority(PORTB_IRQn,2);						//设置优先级
  enable_irq(PORTB_IRQn);
  
  gpio_init(A27,GPO,0);//Tx  
  port_init(A28,IRQ_RISING | ALT1 | PULLDOWN );

  gpio_init(A25,GPO,0);//Tx  
  port_init(A29,IRQ_RISING | ALT1 | PULLDOWN );

  set_irq_priority(PORTA_IRQn,1);						//设置优先级
  enable_irq(PORTA_IRQn);
  

}

void ultrasonic_start(void)
{
  //gpio_init(A27,GPO,0);//Tx
  gpio_set(B0,1);
  Ultrasonic_Delay(12);
  gpio_set(B0,0);

}

void ultrasonic_start_1(void)
{
  //gpio_init(A27,GPO,0);//Tx
  gpio_set(A27,1);
  Ultrasonic_Delay(12);
  gpio_set(A27,0);

}

void ultrasonic_start_2(void)
{
  //gpio_init(A27,GPO,0);//Tx
  gpio_set(A25,1);
  Ultrasonic_Delay(12);
  gpio_set(A25,0);

}
