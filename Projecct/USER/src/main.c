/********************************************
目标核心： MK60DN512VLL10
============================================
MT9V032接线定义：
------------------------------------ 
    模块管脚            单片机管脚
    SDA(51的RX)         C17
    SCL(51的TX)         C16
	场中断(VSY)         
\
    像素中断(PCLK)      PTC18
    数据口(D0-D7)       PTC8-PTC15 

    TFT1.8液晶                
    SCL                 PTA15
    SDA                 PTA16        
    RES                 PTB17    
    DC                  PTB16
	CS					PTA14
============================================

分辨率是                188*120
摄像头参数设置可以到    SEEKFREE-->h_file-->SEEKFREE_MT9V032.h

总钻风-摄像头测试步骤：
1.下载程序到开发板
2.插上串口线或者USB转TTL
3.接好MT9V032模块接线
4.打开上位机设置好参数，即可观看

车上部分

*********************************************/  
#include "headfile.h"

static double adjust_angle=-112;
extern float speed_flag;
extern Node head[10];

//float32_t a=0.153654,b=1.56954,sum=0;
uint32 nrf_recive_flag = 0;
extern uint8 un_recive_flag;
extern uint8_t ult_obs;
uint8_t start_car_flag=0;
extern uint8_t direction_flag;
extern uint8_t ult_flag;
extern float ultrasonic_longth;


extern uint8_t hand_ser_flag ; //0--左。1---右
extern uint8_t hand_speed_flag;//0---停，1---跑


void back_delay(uint32_t t)
{
  uint32_t i,j;
  for(i = 0;i < t;i++)
  {
    for(j = 0;j<100;j++);
  }
}

void main(void)
{
  
  
    uint8 gy273buff[8]={0};
    float car_lig_dis=0;
    double angle[5]={0};
    double air_angle[3];
    double *image_angle = &air_angle[0];
    double *hmc_air_angle = &air_angle[1];
    uint8 nrf_buff[32]={0};
    int i,j;
     get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
    
      DisableInterrupts;
     //gpio_init(C13,GPO,0);
   //gpio_set(C13,1);
    my_delay(9999999);                                     //上电延时以便于摄像头初始化
   
    uart_init (uart0, 115200);                          //初始换串口与电脑通信
   // camera_init();
    NRF_Dev_Init();
    Gy273Init(); 
    
    lptmr_pulse_init(lpt0_alt2,0xFFFF,LPT_Rising);     //PTA19 触发 上升沿
  
    pit_init_ms(pit0,20);//设置采集中断，20ms
    pit_init_ms(pit1,100);//pit1的计时中断，100ms
    enable_irq(PIT0_IRQn);	//打开pit0的中断开关
    set_irq_priority(pit0,3);
    enable_irq(PIT1_IRQn);	//打开pit0的中断开关
    set_irq_priority(pit1,4);
    
    ultrasonic_init(); //超声波初始化
    //EnableInterrupts;		//打开总的中断开关
    
    uart_putstr(uart0,"uart right!\n");
    ftm_pwm_init(ftm1,ftm_ch1,100,1630);//a13
    ftm_pwm_init(ftm2,ftm_ch1,80000,0); 
    ftm_pwm_init(ftm2,ftm_ch0,80000,0); 
    //while(1);
   
    //pit_delay_ms(pit1,4000);
    /*
    uint32 ser_pwm = 1630;
    while(1)
    {
      ser_pwm = 1630;
      while(NRF_Rece_Packet(nrf_buff)==0)
      {
          //ftm_pwm_duty(ftm1,ftm_ch1,ser_pwm);
      }
      
        hand_nrf_data(nrf_buff);
        if(hand_ser_flag == 0 )
        {
          hand_ser_flag = 3;
          ser_pwm = ser_pwm - 500;
        }
        else if(hand_ser_flag == 1)
        {
          hand_ser_flag = 3;
          ser_pwm = ser_pwm + 500;
        }
        else if(hand_ser_flag == 2)
        {
          hand_ser_flag = 3;
          ser_pwm = 1630;
        }
         if(ser_pwm < 1430) {ser_pwm = 1430;}
        if(ser_pwm > 1790){ser_pwm = 1790;}
  
        ftm_pwm_duty(ftm1,ftm_ch1,ser_pwm);
        
        if(hand_speed_flag == 0)
        {
          ftm_pwm_init(ftm2,ftm_ch1,80000,0); 
          ftm_pwm_init(ftm2,ftm_ch0,80000,0); 
        }
        else if(hand_speed_flag == 1)
        {
          ftm_pwm_init(ftm2,ftm_ch1,80000,3000); 
          ftm_pwm_init(ftm2,ftm_ch0,80000,0); 
         // speed_set(200);
        }
        else if(hand_speed_flag == 2)
        {
          ftm_pwm_init(ftm2,ftm_ch1,80000,0); 
          ftm_pwm_init(ftm2,ftm_ch0,80000,2800); 
        }
      
      
    }*/
    for(;;)
	{   
         // arm_add_f32(&a,&b,&sum,1);
         
          if(NRF_Rece_Packet(nrf_buff))
          {
            if(nrf_buff[0] == 0x16)
            {
              nrf_recive_flag++;
              get_nrf_data(nrf_buff,air_angle);//图像角度以及飞机上的hmc角度
              ger_nrf_point(nrf_buff,head);
              Read_HMC5883(gy273buff,angle); //地磁计角度
              //printf("image_angle:%f hmc_angle:%f  err_angle:%f \n",image_angle,angle[0],image_angle-angle[0]-55.828147);
             // printf("地磁角：angle0:    %f \n",angle[0]);
                          
              ca_dis(&head[0],&head[1],&car_lig_dis);//计算图像点距离
              //printf("dis:%f\n",car_lig_dis);
              if(car_lig_dis < 200)//收到数据，图像距离小于定值，开启超声音波
              {
                
                if(ult_flag==2)//2号检测完了
                {
                  ultrasonic_start(); //发送启动信号
                  ult_flag=3;
                }
                else if(ult_flag==0)//0号检测完了
                {
                  ultrasonic_start_1(); //发送启动信号
                  ult_flag=3;
                }
                else if(ult_flag==1)//1号检测完了
                {
                  ultrasonic_start_2(); //发送启动信号
                  ult_flag=3;
                }
                
                 // ult_obs=1; //超声波不好使，强制进入停车标志
                
              }
              if(direction_flag == 1)//要向前走，打舵
              {
                ser_con(image_angle,&angle[0],hmc_air_angle,&adjust_angle); 
              }
              if(car_lig_dis>400)
              {
                start_car_flag = 1;
              }
              head[0].x = 0;
              head[0].y = 0;
              head[1].x = 0;
              head[1].y = 0;
              
            }
          }
          else if(un_recive_flag)//200ms内没有收到数据
          {
            /*
                if(ult_flag==2)//2号检测完了
                {
                  ultrasonic_start(); //发送启动信号
                  ult_flag=3;
                }
                else if(ult_flag==0)//0号检测完了
                {
                  ultrasonic_start_1(); //发送启动信号
                  ult_flag=3;
                }
                else if(ult_flag==1)//1号检测完了
                {
                  ultrasonic_start_2(); //发送启动信号
                  ult_flag=3;
                }
            */
            ult_obs=1;
            ultrasonic_longth = 100;
          }
          
          //printf("un_recive_flag:&d\n",un_recive_flag);
          if(direction_flag == 2) //倒车
          {
            DisableInterrupts;
            ftm_pwm_init(ftm1,ftm_ch1,100,1630);//a13 舵机
            
            ftm_pwm_init(ftm2,ftm_ch0,80000,4000); 
            ftm_pwm_init(ftm2,ftm_ch1,80000,0); 
            back_delay(50000);
            
            speed_set(0);
             EnableInterrupts;
            direction_flag = 1;//倒车结束，向前走
          }
          if(direction_flag == 0) //停车
          {
           // speed_set(0);
            ftm_pwm_init(ftm2,ftm_ch1,80000,0); 
            ftm_pwm_init(ftm2,ftm_ch0,80000,0); 
            back_delay(1000);//等待停稳
          }
          
         
          
          
          //Read_HMC5883(gy273buff,angle);      
          //printf("angle0:    %f \n",angle[0]);
         // my_delay(9999999);
       
          
	}
 
  
  
}

