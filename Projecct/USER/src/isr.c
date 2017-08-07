/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：179029047
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr.c
 * @brief      		中断函数库
 * @company	   		成都逐飞科技有限公司
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.2 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/



#include "isr.h"

#define PB1_IN gpio_get(B1)
#define PA28_IN gpio_get(A28)    
#define PA29_IN gpio_get(A29)
#define RNU_SPEED 200

float ultrasonic_longth = 0;
uint16_t ultrasonic_us = 0;

void   Ultrasonic_Delay(uint32_t n)  //超声波延时us级函数
{
  int i,j;
  for(i=0;i<n;i++)
  {
    for(j=0;j<13;j++);   //   13   100M   1us
                         //   28   180M   1us
                         //   32   200M   1us
  }
}uint8_t ult_obs = 0; //超声波避障标志,距离标志
   
   


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PROTA中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当A口启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------

uint8_t ult_flag=0;//0用第一个超声波，1用第二个，2用第三个
uint8_t ult_time = 0;
void PORTA_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	//PORTA->ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	//PORTA_FLAG_CLR(A1);
        
   ultrasonic_us=0;
   
   if(PORTA->ISFR & (1<<28))//A28触发中断
   {
    while(PA28_IN==1)
     {
        ultrasonic_us++;
        Ultrasonic_Delay(1);
        if(ultrasonic_us>2000)     //超出测试距离清0
        {
        
          break;
        }            
      }   
     ultrasonic_longth = ((float)ultrasonic_us*340)/20000;
    
    if(ultrasonic_longth == 0) //检测为0.00，多半是误判
    {
      ultrasonic_longth = 100;
    }
    
    if(ultrasonic_longth < 28)
     {
      ult_obs = 1;
      printf("ult2: %f \n",ultrasonic_longth);
     }
     else
     {
      ult_obs = 0;
     }
     
     //PORTA_FLAG_CLR(B1);
    //printf("ult2: %f \n",ultrasonic_longth);
    ult_flag=1;//置超声音波标志位，开启下一个
      //printf(" 2 \n");
     PORTA_FLAG_CLR(A28);
     
   }
   
    if(PORTA->ISFR & (1<<29))//A29触发中断
   {
    while(PA29_IN==1)
     {
        ultrasonic_us++;
        Ultrasonic_Delay(1);
        if(ultrasonic_us>2000)     //超出测试距离清0
        {
        
          break;
        }            
      }   
     ultrasonic_longth = ((float)ultrasonic_us*340)/20000;
    
    if(ultrasonic_longth == 0) //检测为0.00，多半是误判
    {
      ultrasonic_longth = 100;
    }
     
    if(ultrasonic_longth < 28)
     {
      ult_obs = 1;
      printf("ult3: %f \n",ultrasonic_longth);
     }
     else
     {
      ult_obs = 0;
      //ultrasonic_start_2(); //第2个没有检测到，检测到了，测不开启。开启下一个超声音波测距
     }
     
     //PORTA_FLAG_CLR(B1);
   // printf("ult3: %f \n",ultrasonic_longth);
    ult_flag = 2;
     // printf(" 3 \n");
     PORTA_FLAG_CLR(A29);
     
   }

}

void PORTB_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	//PORTB->ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	
   ultrasonic_us=0;
   if(PORTB->ISFR & 0x2)
   {
    while(PB1_IN==1)
     {
        ultrasonic_us++;
        Ultrasonic_Delay(1);
        if(ultrasonic_us>2000)     //超出测试距离清0
        {
        
          break;
        }            
      }   
     ultrasonic_longth = ((float)ultrasonic_us*340)/20000;
    
    if(ultrasonic_longth == 0) //检测为0.00，多半是误判
    {
      ultrasonic_longth = 100;
    }
     
    if(ultrasonic_longth < 28)
     {
      ult_obs = 1;
      printf("ult 1 :%f \n",ultrasonic_longth);
     }
     else
     {
      ult_obs = 0;
      //ultrasonic_start_1(); //第一个没有检测到，检测到了，测不开启。开启下一个超声音波测距
     }
     
     //PORTA_FLAG_CLR(B1);
   // printf("ult 1 :%f \n",ultrasonic_longth);
    ult_flag = 0;
      //printf(" 1 \n");
     PORTB_FLAG_CLR(B1);
     
   }
   
}


void PORTC_IRQHandler(void)
{
    //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
	PORTC->ISFR = 0xffffffff;
	//使用我们编写的宏定义清除发生中断的引脚
	//PORTC_FLAG_CLR(C1);
	VSYNC();
}


void DMA0_IRQHandler(void)
{
	//DMA_IRQ_CLEAN(DMA_CH0);
	row_finished();
	
}
uint16 speed_count;
float speed_flag = 0;
extern uint32 nrf_recive_flag;
extern uint8_t start_car_flag;
uint8 pit_count=0,pit1_200ms_flag;
uint8 un_recive_flag = 0;//200ms没有接收到数据标志
uint8_t run_flag =0;
uint8_t direction_flag=0;//为0不动，为1向前走，为2向后走。

void PIT0_IRQHandler(void)
{
  PIT_FlAG_CLR(pit0);
  pit_count++;
  if(pit_count%10 == 0)
  {
    pit1_200ms_flag =1;
  }
  if(pit_count == 200)
  {
    pit_count = 0;
  }  
  if(ult_flag == 3)
  {
    ult_flag = pit_count%3;
  }
  if(pit1_200ms_flag)//200ms 进行一次
  {
    pit1_200ms_flag = 0;
    //printf("%d\n", nrf_recive_flag);
      
   if(nrf_recive_flag > 0) //200ms内有接收到数据
    {
      un_recive_flag = 0; 
      //speed_flag = 200;
      nrf_recive_flag = 0;
    }
   else //200ms内没有接收到数据
    {
      un_recive_flag = 1;
      //speed_flag = 0;
      nrf_recive_flag = 0;  
    } 
   
   if(run_flag)//车在动
   { 
       if(ultrasonic_longth !=0) //超声波测到了距离
       // if(1)
       {
         ultrasonic_longth = 0;
         if(ult_obs && (direction_flag == 1))//如果距离小于了定值
         {
            //停车
           ult_obs = 0;
           speed_flag =0;
           run_flag = 0;
           direction_flag = 0;//方向标志
         }
         else//距离没有小于定值
         {
            speed_flag = RNU_SPEED;
            run_flag = 1;
            direction_flag =1;
            
         }
       
       }
       else//并没有使用超声波检测
       {
          speed_flag = RNU_SPEED;
          run_flag = 1;
          direction_flag =1;
       }
   }
   else //车已经停住
   {
     if(start_car_flag && (direction_flag == 0))//检测到距离大于定值，开始启动车
     {
      start_car_flag = 0;
      speed_flag = RNU_SPEED;
      run_flag = 1;
      direction_flag =2;//倒车标志
     }
   }
   
   
   
  }
  speed_count = lptmr_pulse_get();
  lptmr_pulse_clean();
  //
  //ANO_DT_Send_MotoPWM(speed_count,0,0,0,0,0,0,0);
  if(direction_flag == 1 && run_flag)//向前走，启动pid调速度
  {
    speed_set(speed_flag);
  } 
    //printf("%d\n", speed_cout);
 
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART3中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当UART3启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void UART3_RX_TX_IRQHandler(void)
{
    if(UART3->S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
    {
        //用户需要处理接收数据

    }
    if(UART3->S1 & UART_S1_TDRE_MASK )                                    //发送数据寄存器空
    {
        //用户需要处理发送数据

    }
}


uint16_t pit1_time_flag = 0;
void PIT1_IRQHandler(void)
{
  PIT_FlAG_CLR(pit1);
  
  pit1_time_flag++;
}


/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了DMA0中断，然后就到下面去找哪个是DMA0的中断函数名称，找到后写一个该名称的函数即可
void DMA0_IRQHandler(void)
{
    ;
}
记得进入中断后清除标志位


DMA0_IRQHandler  
DMA1_IRQHandler  
DMA2_IRQHandler  
DMA3_IRQHandler  
DMA4_IRQHandler  
DMA5_IRQHandler  
DMA6_IRQHandler  
DMA7_IRQHandler  
DMA8_IRQHandler  
DMA9_IRQHandler  
DMA10_IRQHandler 
DMA11_IRQHandler 
DMA12_IRQHandler 
DMA13_IRQHandler 
DMA14_IRQHandler 
DMA15_IRQHandler 
DMA_Error_IRQHandler      
MCM_IRQHandler            
FTFL_IRQHandler           
Read_Collision_IRQHandler 
LVD_LVW_IRQHandler        
LLW_IRQHandler            
Watchdog_IRQHandler       
RNG_IRQHandler            
I2C0_IRQHandler           
I2C1_IRQHandler           
SPI0_IRQHandler           
SPI1_IRQHandler           
SPI2_IRQHandler           
CAN0_ORed_Message_buffer_IRQHandler    
CAN0_Bus_Off_IRQHandler                
CAN0_Error_IRQHandler                  
CAN0_Tx_Warning_IRQHandler             
CAN0_Rx_Warning_IRQHandler             
CAN0_Wake_Up_IRQHandler                
I2S0_Tx_IRQHandler                     
I2S0_Rx_IRQHandler                     
CAN1_ORed_Message_buffer_IRQHandler    
CAN1_Bus_Off_IRQHandler                
CAN1_Error_IRQHandler                  
CAN1_Tx_Warning_IRQHandler             
CAN1_Rx_Warning_IRQHandler             
CAN1_Wake_Up_IRQHandler                
Reserved59_IRQHandler                  
UART0_LON_IRQHandler                   
UART0_RX_TX_IRQHandler                 
UART0_ERR_IRQHandler                   
UART1_RX_TX_IRQHandler                 
UART1_ERR_IRQHandler  
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler  
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler  
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler  
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler  
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler 
RTC_IRQHandler 
RTC_Seconds_IRQHandler  
PIT0_IRQHandler  
PIT1_IRQHandler  
PIT2_IRQHandler  
PIT3_IRQHandler  
PDB0_IRQHandler  
USB0_IRQHandler  
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler  
ENET_Receive_IRQHandler
ENET_Error_IRQHandler  
Reserved95_IRQHandler  
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler 
LPTimer_IRQHandler 
Reserved102_IRQHandler 
PORTA_IRQHandler 
PORTB_IRQHandler 
PORTC_IRQHandler 
PORTD_IRQHandler 
PORTE_IRQHandler 
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler 
*/
                


