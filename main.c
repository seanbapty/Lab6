// A functionality
// updates robots movement based on inputs from the remote control by calling functions in the implementation file
#include <msp430.h>
#include "lab6.h"
#include "start5.h"
//#include "moveRobot.c"

int8 newIrPacket = FALSE;
int16 packetData[48];
int8 packetIndex = 0;
int32 result = 0;

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;                 // stop the watchdog timer
	initPWM();								// initialize ports and timer
	initMSP430();							// initialize
	stop();
	while (1) {			// continuously perform required functionality sequence

		int32 checkValue = 0x80000000; //comparison value with a 1 set in the MSB
		if (packetIndex == 34) {	//start filling data at the appropriate time
			//newIrPacket = TRUE;
			int i = 2;
			for (i; i < 33; i++) { //32 bit data packets
				if (packetData[i] > 1500) {	//logic one sent from remote
					result |= checkValue;
				} else if (packetData[i] < 600) {//logic zero sent from remote
					result &= ~checkValue;
				}
				checkValue >>= 1; //rotate the bit right of the compare value
			}
			if (result == PWR) {    //button press defined in header
				moveForward();
				stop();

			} else if (result == ZER) {
				moveBackward();
				__delay_cycles(STANDARD_DELAY);
				stop();
			} else if (result == ONE) {
				moveForward();
				__delay_cycles(STANDARD_DELAY);
				stop();
			} else if (result == TWO) {
				hardRight();
				__delay_cycles(STANDARD_DELAY);
				stop();
			} else if (result == THR) {
				hardLeft();
				__delay_cycles(STANDARD_DELAY);
				stop();
			}
			//result=0;

			/* required functionality
			 * 	moveForward();
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
			 */
			initMSP430();
		}
	}
}	// end main

	void initMSP430() {

		IFG1 = 0; 					// clear interrupt flag1
		WDTCTL = WDTPW + WDTHOLD; 		// stop WD

		BCSCTL1 = CALBC1_8MHZ;
		DCOCTL = CALDCO_8MHZ;

		P2SEL &= ~BIT6;						// Setup P2.6 as GPIO not XIN
		P2SEL2 &= ~BIT6;						//set up pins to toggle
		P2DIR &= ~BIT6;
		P2IFG &= ~BIT6;						// Clear any interrupt flag
		P2IE |= BIT6;					// Enable PORT 2 interrupt on pin change

		HIGH_2_LOW;
		P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
		P1OUT &= ~(BIT0 | BIT6);			// An turn the LED off

		TA0CCR0 = 0x8000;					// create a 16mS roll-over period
		TACTL &= ~TAIFG;// clear flag before enabling interrupts = good practice
		TACTL = ID_3 | TASSEL_2 | MC_1;	// Use 1:1 presclar off MCLK and enable interrupts

		_enable_interrupt();
	}

#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

	__interrupt
	void pinChange(void) {

		int8 pin;
		int16 pulseDuration;			// The timer is 16-bits

		if (IR_PIN)
			pin = 1;
		else
			pin = 0;

		switch (pin) {					// read the current pin level
		case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;
			TACTL = 0;
			packetData[packetIndex++] = pulseDuration;
			LOW_2_HIGH; 				// Setup pin interrupr on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;				// time measurements are based at time 0
			TA0CCR0 = 0xFFFF;//set up time delay between when new data will be recieved
			TACTL &= ~TAIFG;
			TACTL = ID_3 | TASSEL_2 | MC_1 | TAIE;
			HIGH_2_LOW; 				// Setup pin interrupr on positive edge
			break;
		} // end switch

		P2IFG &= ~BIT6;	// Clear the interrupt flag to prevent immediate ISR re-entry

	} // end pinChange ISR

// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
	__interrupt
	void timerOverflow(void) {
		//newIrPacket = TRUE;
		//initMSP430();
		packetIndex = 0;
		TACTL &= ~TAIFG;
	}
