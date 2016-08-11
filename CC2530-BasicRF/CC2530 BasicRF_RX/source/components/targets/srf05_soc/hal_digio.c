//-------------------�������Գ���------------------//
/*                  Creat By ES                       */
/*           http://es-taobao.taobao.com          */
/******************************************************
�������ݣ�   ���԰�������ģ���Ƿ�������
1.����LED��ʾ
2.������������
3.Һ����ʾ
4.����ͨ��
5.����ͨ��

ʵ�������
�ѱ�����ֱ���д��ÿһ��ģ���в�����
ģ��һ�ϵ磬��ɫ�ͻ�ɫLEDÿ��0.5������һ�Σ�LCD��ʾ"Wecome to ES"
ģ��ÿ��һ��ʱ������Է����ַ�����Waiting for Key��
����⵽��������PC����һ�δ��롱Key Detect��
����⵽����������ģ�飬�Զ��������Ӳ�ʵ��ͨ�ţ���ɫLED����

ע�⣺Һ����ʾ��Ҫ�ڱ����̹���Һ��ģ�� JLX12864
ʹ�ô��ڵ��������뽫�������趨Ϊ115200����ѡ�����
���ںţ���Ҫ��ѡHEX��ʾ�����Ա�����������Ҫ�ڵ��԰�װ
PL2302 USBת������������ʹ��USB������ģ��͵���
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
 ���ڳ�ʼ����������ʼ������ UART0		
*****************************************/
void InitUART0(void)
{
{
    CLKCONCMD &= ~0x40; // ����ϵͳʱ��ԴΪ 32MHZ����
    while(CLKCONSTA & 0x40);                     // �ȴ������ȶ� 
    CLKCONCMD &= ~0x47;                          // ����ϵͳ��ʱ��Ƶ��Ϊ 32MHZ

    PERCFG = 0x00;        //λ��1 P0�� 
    P0SEL |= 0x0c;        //P0_2,P0_3,P0_4,P0_5��������,�ڶ����� 
    P2DIR &= ~0XC0;      //P0 ������ΪUART0 �����ȼ�
 
    U0CSR |= 0x80;       //UART ��ʽ 
    U0GCR |= 8;         //U0GCR��U0BAUD���     
    U0BAUD |= 59;       // ��������Ϊ115200 
    UTX0IF = 0;          //UART0 TX �жϱ�־��ʼ��λ1  ���շ�ʱ��
    U0CSR |= 0X40;       //������� 
    IEN0 |= 0x84;        // �����жϣ������ж�    
}			  //�����жϣ������ж�

}

/****************************************************************
   ���ڷ����ַ�������						
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
     U0DBUF = 0x0A;        // ����
    while(UTX0IF == 0);
      UTX0IF = 0;
}

void Key()
{
  if(KEY1==0)
  {        
      Red_LED=!Red_LED;
      //UartTX_Send_String("S1 detected",11);
      printf("<G>ˢ���ɹ�������Ϊ05674175\n");
      halMcuWaitMs(300);     //����ȥ��
      LCD_write_english_string(5,1,"   Waiting for Key1 ");  //��ʾ�ַ���
  } 
  if(KEY2==0)
  {        
      Yellow_LED=!Yellow_LED;
      UartTX_Send_String("S2 detected",11);
      halMcuWaitMs(300);     //����ȥ��
      LCD_write_english_string(5,1,"   Waiting for Key2 ");  //��ʾ�ַ���
  }
  if(KEY3==0)
  {        
      Blue_LED=!Blue_LED;
      UartTX_Send_String("S3 detected",11);
      halMcuWaitMs(300);     //����ȥ��
      LCD_write_english_string(5,1,"   Waiting for Key3 ");  //��ʾ�ַ���
  } 
}

void main(void)
{
    // Config basicRF
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = TRUE;
    basicRfConfig.myAddr = ADDR; 
    
    halBoardInit();   //��ʼ����Ƭ��
    Initial_IO();     //��ʼ��IO   
    initial_lcd();
    clear_screen();      //clear all dots
    LCD_write_english_string(1,1,"    Welcome  ");  //��ʾӢ���ַ���
  
    halMcuWaitMs(100);  // ��ʱ100����
    InitUART0();
   

    if(halRfInit()==FAILED) {
      HAL_ASSERT(FALSE);
    }    
    if(basicRfInit(&basicRfConfig)==FAILED) {
      HAL_ASSERT(FALSE);
    }   
   
    basicRfReceiveOn();   //������
    
       while (1) 
       {        
        if (basicRfPacketIsReady())   // receive data
        {   
          if(basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL)>0)
          {    
           
                Blue_LED=!Blue_LED;
                   LCD_write_english_string(1,1,pRxData);  //��ʾӢ���ַ���
             
            }
          }
               
        Key();
        halMcuWaitMs(1);  // ��ʱ1����         
    }
    
}


