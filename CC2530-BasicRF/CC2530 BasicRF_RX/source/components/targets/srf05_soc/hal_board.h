/***********************************************************************************

  Filename:     hal_board.h

  Description:  SmartRF05 board with CCxxxxEM.

***********************************************************************************/

#ifndef HAL_BOARD_H
#define HAL_BOARD_H

#include "hal_cc8051.h"
#include "hal_defs.h"
#include "hal_mcu.h"
#include "ioCC2530.h"

#define BSP_CONFIG_CLOCK_MHZ                32



/***********************************************************************************
 * CONSTANTS
 */

#define KEY1 P1_1      //KEY1为P1.1口控制
#define KEY2 P1_2      //
#define KEY3 P1_3      //

#define Red_LED     P1_4    //P14连接红色LED
#define Yellow_LED  P1_5    //P15连接黄色LED
#define Blue_LED    P1_6    //P16连接蓝色LED

// Board properties
#define BOARD_NAME                     "SRF05EB"

#define HAL_BOARD_IO_UART_RTS_PORT          0
#define HAL_BOARD_IO_UART_RTS_PIN           5


// Debounce
#define HAL_DEBOUNCE(expr)    { int i; for (i=0; i<500; i++) { if (!(expr)) i = 0; } }

/***********************************************************************************
 * MACROS
 */


#define HAL_RTS_SET()       MCU_IO_SET_HIGH(HAL_BOARD_IO_UART_RTS_PORT, \
    HAL_BOARD_IO_UART_RTS_PIN)
#define HAL_RTS_CLR()       MCU_IO_SET_LOW(HAL_BOARD_IO_UART_RTS_PORT, \
    HAL_BOARD_IO_UART_RTS_PIN)
#define HAL_RTS_DIR_OUT()   MCU_IO_OUTPUT(HAL_BOARD_IO_UART_RTS_PORT, \
    HAL_BOARD_IO_UART_RTS_PIN, 1)



#define HAL_PROCESS()

/***********************************************************************************
 * FUNCTION PROTOTYPES
 */
void halBoardInit(void);
void Initial_IO();

#endif
