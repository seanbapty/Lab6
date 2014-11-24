#include <msp430.h>
#include "lab6.h"

void main(void) {
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer
    initPWM();

	moveForward();
    __delay_cycles(5000000);
    moveBackward();

    while(1){ }

} // end main
