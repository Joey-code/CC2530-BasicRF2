/****************************************************************************
* �� �� ��: main.c
* ��    ��: ���ô��ڵ������ֲ����ʣ�115200bps 8N1
*           ���ڵ������ָ�CC2530���ַ���ʱ��������᷵�ؽ��յ����ַ���
ע�⣬����������'��'Ϊ������,һ��������50���ַ�  
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
char RxData[SIZE];        //�洢�����ַ���

/****************************************************************************
* ��    ��: DelayMS()
* ��    ��: �Ժ���Ϊ��λ��ʱ
* ��ڲ���: msec ��ʱ������ֵԽ����ʱԽ��
* ���ڲ���: ��
****************************************************************************/
void DelayMS(uint msec)
{ 
    uint i,j;
    
    for (i=0; i<msec; i++)
        for (j=0; j<1070; j++);
}

/****************************************************************************
* ��    ��: InitUart()
* ��    ��: ���ڳ�ʼ������
* ��ڲ���: ��
* ���ڲ���: ��
****************************************************************************/
void InitUart(void)
{ 
    PERCFG = 0x00;           //������ƼĴ��� USART 0��IOλ��:0ΪP0��λ��1 
    P0SEL = 0x0c;            //P0_2,P0_3�������ڣ����蹦�ܣ�
    P2DIR &= ~0xC0;          //P0������ΪUART0
    
    U0CSR |= 0x80;           //����ΪUART��ʽ
    U0GCR |= 11;				       
    U0BAUD |= 216;           //��������Ϊ115200
    UTX0IF = 0;              //UART0 TX�жϱ�־��ʼ��λ0
    U0CSR |= 0x40;           //������� 
    IEN0 |= 0x84;            //�����ж���������ж�  
}


#pragma vector = URX0_VECTOR 
__interrupt void UART0_ISR(void) 
{ 
    URX0IF = 0;       // ���жϱ�־ 
    RxBuf = U0DBUF;                           
}

void Key()
{
  if(KEY1==0)
  {        
      Red_LED=!Red_LED;
      
       basicRfSendPacket(ADDR, pTxData, APP_PAYLOAD_LENGTH);  // sent data  
      halMcuWaitMs(300);     //����ȥ��
      LCD_write_english_string(5,1,"  already send ");  //��ʾ�ַ���
  } 
 
}

/****************************************************************************
* ������ں���
****************************************************************************/
void main(void)
{	
    CLKCONCMD &= ~0x40;                        //����ϵͳʱ��ԴΪ32MHZ����
    while(CLKCONSTA & 0x40);                   //�ȴ������ȶ�Ϊ32M
    CLKCONCMD &= ~0x47;                        //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ  
    
        basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = ADDR; 
    
    halBoardInit();   //��ʼ����Ƭ��
    Initial_IO();     //��ʼ��IO   
   initial_lcd();
   
    clear_screen();      //clear all dots
    LCD_write_english_string(1,1,"Welcome to ES1 ");  //��ʾӢ���ַ���
    memset(RxData, 0, SIZE);
     
     halMcuWaitMs(100);  // ��ʱ100����
    InitUart();  
    //���ô��ڳ�ʼ������   
   if(halRfInit()==FAILED) {
      HAL_ASSERT(FALSE);
    }    
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }   
   
    basicRfReceiveOff();   //������

   while(1) 
   {      
            if(RxBuf != 0) 
            {                 
                if((RxBuf != '#')&&(count < 50))//��'��'Ϊ������,һ��������50���ַ�            
                    RxData[count++] = RxBuf; 
                else
                {
                    if(count >= 50)             //�ж����ݺϷ��ԣ���ֹ���
                    {
                        count = 0;              //������0
                        memset(RxData, 0, SIZE);//��ս��ջ�����
                    }
                    else
                      
                      for(i=0;i<count;i++) pTxData[i]=RxData[i];
                       LCD_write_english_string(2,1,RxData);  //��ʾӢ���ַ���
                     memset(RxData, 0, SIZE);//��ս��ջ�����
                     count = 0;
                }
                RxBuf  = 0;
            }
         Key();
        halMcuWaitMs(1);  // ��ʱ1����   
   }
            
}
