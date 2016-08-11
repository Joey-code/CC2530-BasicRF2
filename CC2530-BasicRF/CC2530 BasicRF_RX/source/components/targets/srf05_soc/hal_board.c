/***********************************************************************************
  Filename:     hal_board.c

  Description:  HAL board peripherals library for the
                SmartRF05EB + CC2430EM/CC2531EM/CC2510/CC1110 platforms

***********************************************************************************/

/***********************************************************************************
* INCLUDES
*/
#include "hal_types.h"
#include "hal_defs.h"
#include "hal_digio.h"
#include "hal_int.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include "clock.h"

#define SPI_CLOCK_POL_LO       0x00
#define SPI_CLOCK_POL_HI       0x80
#define SPI_CLOCK_PHA_0        0x00
#define SPI_CLOCK_PHA_1        0x40
#define SPI_TRANSFER_MSB_FIRST 0x20
#define SPI_TRANSFER_MSB_LAST  0x00

#define KEY1 P1_1      //KEY1ΪP1.1�ڿ���
#define KEY2 P1_2      //
#define KEY3 P1_3      //

#define Red_LED     P1_4    //P14���Ӻ�ɫLED
#define Yellow_LED  P1_5    //P15���ӻ�ɫLED
#define Blue_LED    P1_6    //P16������ɫLED


/****************************
     ������ʼ������
*****************************/
void Initial_IO(void)
{
	P0DIR |= 0xfd;     //P0_1Ϊ���룬�������Ŷ���Ϊ���
        P1DIR |= 0xff;     //P1����Ϊ���
        P1=0x0f;           //����ʹ�ܣ���ֹ����LED

} 

void halBoardInit(void)
{
    halMcuInit();
    halIntOn();
}

