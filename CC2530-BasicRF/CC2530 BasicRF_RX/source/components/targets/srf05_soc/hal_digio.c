//-------------------出厂测试程序------------------//
/*                  Creat By ES                       */
/*           http://es-taobao.taobao.com          */
/******************************************************
测试内容：   测试板子所有模块是否工作正常
1.三个LED显示
2.三个按键操作
3.液晶显示
4.串口通信
5.无线通信

实验操作：
把本程序分别烧写到每一个模块中并运行
模块一上电，红色和黄色LED每隔0.5秒闪亮一次，LCD显示"Wecome to ES"
模块每隔一段时间向电脑发送字符串“Waiting for Key”
当检测到按键，向PC发送一段代码”Key Detect“
当检测到附近有其他模块，自动进行连接并实现通信，蓝色LED闪亮

注意：液晶显示需要在本店铺购买液晶模块 JLX12864
使用串口调试助手请将波特率设定为115200，并选择合适
串口号，不要勾选HEX显示。测试本程序首先需要在电脑安装
PL2302 USB转串口驱动，并使用USB线连接模块和电脑
******************************************************/

// INCLUDES
//
#include <hal_assert.h>
#include <hal_board.h>
#include "JLX12864.h"
#include <hal_int.h>
#include "hal_mcu.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include<stdio.h>

/***********************************************************************************
* CONSTANTS
*/
#define RF_CHANNEL                25      // 2.4 GHz RF channel
#define PAN_ID                0x2007
#define ADDR                  0x2520
#define APP_PAYLOAD_LENGTH        1

/***********************************************************************************/

static uint8 pRxData[APP_PAYLOAD_LENGTH];
static uint8 pTxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;

uint16 count=0;


/*****************************************
 串口初始化函数：初始化串口 UART0		
*****************************************/
void InitUART0(void)
{
{
    CLKCONCMD &= ~0x40; // 设置系统时钟源为 32MHZ晶振
    while(CLKCONSTA & 0x40);                     // 等待晶振稳定 
    CLKCONCMD &= ~0x47;                          // 设置系统主时钟频率为 32MHZ

    PERCFG = 0x00;        //位置1 P0口 
    P0SEL |= 0x0c;        //P0_2,P0_3,P0_4,P0_5用作串口,第二功能 
    P2DIR &= ~0XC0;      //P0 优先作为UART0 ，优先级
 
    U0CSR |= 0x80;       //UART 方式 
    U0GCR |= 8;         //U0GCR与U0BAUD配合     
    U0BAUD |= 59;       // 波特率设为115200 
    UTX0IF = 0;          //UART0 TX 中断标志初始置位1  （收发时候）
    U0CSR |= 0X40;       //允许接收 
    IEN0 |= 0x84;        // 开总中断，接收中断    
}			  //开总中断，接收中断

}

/****************************************************************
   串口发送字符串函数						
****************************************************************/
void UartTX_Send_String(char *Data,int len)
{
  int j;
  for(j=0;j<len;j++)
  {
    U0DBUF = *Data++;
    while(UTX0IF == 0);
    UTX0IF = 0;
  }
     U0DBUF = 0x0A;        // 换行
    while(UTX0IF == 0);
      UTX0IF = 0;
}

void Key()
{
  if(KEY1==0)
  {        
      Red_LED=!Red_LED;
      //UartTX_Send_String("S1 detected",11);
      printf("<G>刷卡成功，卡号为05674175\n");
      halMcuWaitMs(300);     //按键去抖
      LCD_write_english_string(5,1,"   Waiting for Key1 ");  //显示字符串
  } 
  if(KEY2==0)
  {        
      Yellow_LED=!Yellow_LED;
      UartTX_Send_String("S2 detected",11);
      halMcuWaitMs(300);     //按键去抖
      LCD_write_english_string(5,1,"   Waiting for Key2 ");  //显示字符串
  }
  if(KEY3==0)
  {        
      Blue_LED=!Blue_LED;
      UartTX_Send_String("S3 detected",11);
      halMcuWaitMs(300);     //按键去抖
      LCD_write_english_string(5,1,"   Waiting for Key3 ");  //显示字符串
  } 
}

void main(void)
{
    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = ADDR; 
    
    halBoardInit();   //初始化单片机
    Initial_IO();     //初始化IO   
    initial_lcd();
    clear_screen();      //clear all dots
    LCD_write_english_string(1,1,"    Welcome  ");  //显示英文字符串
  
    halMcuWaitMs(100);  // 延时100毫秒
    InitUART0();
   

    if(halRfInit()==FAILED) {
      HAL_ASSERT(FALSE);
    }    
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }   
   
    basicRfReceiveOn();   //开接收
    
       while (1) 
       {        
        if (basicRfPacketIsReady())   // receive data
        {   
          if(basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL)>0)
          {    
           
                Blue_LED=!Blue_LED;
                   LCD_write_english_string(1,1,pRxData);  //显示英文字符串
             
            }
          }
               
        Key();
        halMcuWaitMs(1);  // 延时1毫秒         
    }
    
}


