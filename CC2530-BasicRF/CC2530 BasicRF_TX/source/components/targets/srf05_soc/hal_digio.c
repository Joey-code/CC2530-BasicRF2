/****************************************************************************
* 文 件 名: main.c
* 描    述: 设置串口调试助手波特率：115200bps 8N1
*           串口调试助手给CC2530发字符串时，开发板会返回接收到的字符串
注意，发送数据以'＃'为结束符,一次最多接收50个字符  
****************************************************************************/
#include <ioCC2530.h>
#include <string.h>
#include "JLX12864.h"
#include <hal_assert.h>
#include <hal_board.h>
#include "JLX12864.h"
#include <hal_int.h>
#include "hal_mcu.h"
#include "hal_rf.h"
#include "basic_rf.h"



/***********************************************************************************
* CONSTANT*/
#define RF_CHANNEL                25      // 2.4 GHz RF channel
#define PAN_ID                0x2007
#define ADDR                  0x2520
#define APP_PAYLOAD_LENGTH        1

/***********************************************************************************/
#define UART0_RX    1
#define UART0_TX    2
#define SIZE       51


static uint8 pRxData[APP_PAYLOAD_LENGTH];
static uint8 pTxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;

char RxBuf;
char UartState;
char count,i;
char RxData[SIZE];        //存储发送字符串

/****************************************************************************
* 名    称: DelayMS()
* 功    能: 以毫秒为单位延时
* 入口参数: msec 延时参数，值越大，延时越久
* 出口参数: 无
****************************************************************************/
void DelayMS(uint msec)
{ 
    uint i,j;
    
    for (i=0; i<msec; i++)
        for (j=0; j<1070; j++);
}

/****************************************************************************
* 名    称: InitUart()
* 功    能: 串口初始化函数
* 入口参数: 无
* 出口参数: 无
****************************************************************************/
void InitUart(void)
{ 
    PERCFG = 0x00;           //外设控制寄存器 USART 0的IO位置:0为P0口位置1 
    P0SEL = 0x0c;            //P0_2,P0_3用作串口（外设功能）
    P2DIR &= ~0xC0;          //P0优先作为UART0
    
    U0CSR |= 0x80;           //设置为UART方式
    U0GCR |= 11;				       
    U0BAUD |= 216;           //波特率设为115200
    UTX0IF = 0;              //UART0 TX中断标志初始置位0
    U0CSR |= 0x40;           //允许接收 
    IEN0 |= 0x84;            //开总中断允许接收中断  
}


#pragma vector = URX0_VECTOR 
__interrupt void UART0_ISR(void) 
{ 
    URX0IF = 0;       // 清中断标志 
    RxBuf = U0DBUF;                           
}

void Key()
{
  if(KEY1==0)
  {        
      Red_LED=!Red_LED;
      
       basicRfSendPacket(ADDR, pTxData, APP_PAYLOAD_LENGTH);  // sent data  
      halMcuWaitMs(300);     //按键去抖
      LCD_write_english_string(5,1,"  already send ");  //显示字符串
  } 
 
}

/****************************************************************************
* 程序入口函数
****************************************************************************/
void main(void)
{	
    CLKCONCMD &= ~0x40;                        //设置系统时钟源为32MHZ晶振
    while(CLKCONSTA & 0x40);                   //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;                        //设置系统主时钟频率为32MHZ  
    
        basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = ADDR; 
    
    halBoardInit();   //初始化单片机
    Initial_IO();     //初始化IO   
   initial_lcd();
   
    clear_screen();      //clear all dots
    LCD_write_english_string(1,1,"Welcome to ES1 ");  //显示英文字符串
    memset(RxData, 0, SIZE);
     
     halMcuWaitMs(100);  // 延时100毫秒
    InitUart();  
    //调用串口初始化函数   
   if(halRfInit()==FAILED) {
      HAL_ASSERT(FALSE);
    }    
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }   
   
    basicRfReceiveOff();   //开接收

   while(1) 
   {      
            if(RxBuf != 0) 
            {                 
                if((RxBuf != '#')&&(count < 50))//以'＃'为结束符,一次最多接收50个字符            
                    RxData[count++] = RxBuf; 
                else
                {
                    if(count >= 50)             //判断数据合法性，防止溢出
                    {
                        count = 0;              //计数清0
                        memset(RxData, 0, SIZE);//清空接收缓冲区
                    }
                    else
                      
                      for(i=0;i<count;i++) pTxData[i]=RxData[i];
                       LCD_write_english_string(2,1,RxData);  //显示英文字符串
                     memset(RxData, 0, SIZE);//清空接收缓冲区
                     count = 0;
                }
                RxBuf  = 0;
            }
         Key();
        halMcuWaitMs(1);  // 延时1毫秒   
   }
            
}
