#include <msp430.h>
#include "lab6.h"
//#include "moveRobot.c"

void main(void) {
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer
    initPWM();								// initialize ports and timer
while(1){									// continuously perform required functionality sequence
	moveForward();
    stop();

    moveBackward();
    stop();

    hardRight();
    stop();

    hardLeft();
    stop();

    smallRight();
    stop();

    smallLeft();
    stop();


}

} // end main
