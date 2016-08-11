/***********************************************************************************
  Filename:     hal_assert.c
    
  Description:  Assert functionality
    
***********************************************************************************/

/***********************************************************************************
* INCLUDES
*/
#include "hal_assert.h"
#include "hal_mcu.h"
#include "hal_defs.h"


/***********************************************************************************
* GLOBAL FUNCTIONS
*/

/***********************************************************************************
* @fn          halAssertHandler
*
* @brief       Logic to handle an assert.
*
* @param       none
*
* @return      none
***********************************************************************************
*/
void halAssertHandler(void)
{
    // execute code that handles asserts 
    // blink all leds
    while(TRUE){
//        halLedToggle(1);
        halMcuWaitMs(50);
    }
}

