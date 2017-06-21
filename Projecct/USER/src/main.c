/********************************************
逐飞科技 总钻风-摄像头  历程
Designed by Fly Sir
软件版本:V1.1
最后更新:2016年5月3日
相关信息参考下列地址：
淘宝店：https://seekfree.taobao.com/
------------------------------------
软件版本： IAR 7.2 or MDK 5.17
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
*********************************************/  
#include "headfile.h"

static double adjust_angle=-90;

int main(void)
{
  
  
    uint8 gy273buff[8]={0};
    double angle[5]={0};
    double image_angle;
    uint8 nrf_buff[32]={0};
    int i,j;
     get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
    //gpio_init(C13,GPO,0);
   //gpio_set(C13,1);
    my_delay(9999999);                                     //上电延时以便于摄像头初始化
    uart_init (uart0, 115200);                          //初始换串口与电脑通信
   // camera_init();
    NRF_Dev_Init();
    Gy273Init();
    uart_putchar(uart0,'a');
    //ftm_pwm_init(ftm1,ftm_ch1,100,1000);//a13
    //ftm_pwm_init(ftm2,ftm_ch0,100,2290);//B18
    //ftm_pwm_init(ftm2,ftm_ch1,80000,180);//B19
    
    for(;;)
	{     
          while(NRF_Rece_Packet(nrf_buff)==0);
         // uart_putbuff(uart0,nrf_buff+1,1);
          if(nrf_buff[0] == 0x09)
          {
            //get_nrf_data(nrf_buff,&image_angle);//图像角度
           // Read_HMC5883(gy273buff,angle); //地磁计角度
            //printf("image_angle:%f hmc_angle:%f  err_angle:%f \n",image_angle,angle[0],image_angle-angle[0]-55.828147);
           // printf("地磁角：angle0:    %f \n",angle[0]);
            //ser_con(&image_angle,&angle[0],&adjust_angle);
          }
          //Read_HMC5883(gy273buff,angle);      
          //printf("angle0:    %f \n",angle[0]);
         // my_delay(9999999);
       
          
	}
 
  
  
}

