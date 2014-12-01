Lab6 Basic Robot Movement
====

#Objectives
This lab is designed to provide you with experience using the pulse-width modulation features of the MSP430. You will need to program the MSP430 to generate pulse-width-modulated (PWM) waveforms to control the speed / direction of your robot's motors. In this lab, you will make your robot move forward, backwards, a small (< 45 degree) turn left/right, and a large (> 45 dgree) turn left/right.

#Preliminary Design
Prior to begining work on the lab, it was useful to think about the problem and layout how the MSP430 will be hooked up to the motor driver chip and therefore the electric motors. The electric motors will operate if there is a potential difference accross the motor. Therefore, each motor must have two inputs so that this difference can be created. Additionally, there are two motors on the robot and they are independently controlled to perform right/left hand turns. A rough schematic of what pins can be used on the MSP and the motor driver chip (SN754410) was drawn prior to wiring the connections or writing software.

![alt tag](https://raw.githubusercontent.com/seanbapty/Lab6/master/IMG_0313.JPG)

The top two drawings show how the motor will be hooked up to the SN754410 and the MSP430. The bottom waveform is a sample PWM for motor A and B. These waveforms were drawn so that I could think through if the motor would be active at each stage and therefore figure out how to implement the PWM.

It must be noted that in order for an engine to move, the enable pin must be set to high per the motor driver datasheet.
#Software Flowchart/Algorithms
There were no software flowcharts designed for this lab, however, it was determined that a header would declare constants to more "usable" names, an implementation file would contain the robot movement functions, and a main file would manage the order of movement.
#Hardware Schematic
Based on the prelab schematic and the motor SN754410 data sheet connection table, it was fairly easy to connect the SN754410.

![alt tag](https://raw.githubusercontent.com/seanbapty/Lab6/master/motordriver%20layout.JPG)

Additionally, in order to reduce the complexity of the wiring and therefore signal noise/distortion, the MSP430 was wired in-circuit. This was accomplised by referencing the datasheet on the course website.

![alt tag](https://raw.githubusercontent.com/seanbapty/Lab6/master/MSP430%20in%20circuit.jpg)
#Well-Formated Code
The first task in software development was pseudo-code. 
```
declare constants such as which pin does what
initialize MSP430 pins based on hardware schematic
 main 
     call each movement fucntion
implementation
  initilization function
  forward--activates right and left motor forward by creating appropirate potential difference
  backward--opposite as forward
  turn right--right wheel forward, left backward
  turn left--opposite of right
  small right--turn right for less time (smaller __delay_cycles(XXXXX)
  small left--opposite
```
##Header
The purpose of the header was to declare the pins as meaningful constants, and all the appropriate functions.
```
/*
 * lab6.h
 *
 *  Created on: Nov 23, 2014
 *      Author: C16Sean.Bapty
 */
#include <msp430.h>
#ifndef LAB6_H_
#define LAB6_H_

#define L_SET_OUT			P2DIR |= BIT0			// define left motor enable pin as output and
#define L_EN				P2OUT |= BIT0			// declare "on" and "off" settings for motor
#define L_DIS				P2OUT &= ~BIT0

#define L_DIRECTION_SET		P2DIR |= BIT1			// Sets left motor to output
#define L_BACKWARD			P2OUT &= ~BIT1			// used to determine if the left motor is going
#define L_FORWARD			P2OUT |= BIT1			// forward or backward

#define L_PWM				BIT2					// left pulse width modulation

#define R_DIRECTION_SET		P2DIR |= BIT3			// right motor forward or backward?
#define R_BACKWARD			P2OUT |= BIT3
#define R_FORWARD			P2OUT &= ~BIT3

#define R_PWM				BIT4					// right pulse width modulation

#define R_SET_OUT			P2DIR |= BIT5			// same as enable for left but right motor
#define R_EN				P2OUT |= BIT5
#define R_DIS				P2OUT &= ~BIT5


#define STANDARD_DELAY		900000					// guess and check delay set
#define SHORT_DELAY			STANDARD_DELAY/2		// used for small turns and whenever else a small delay is desired

#define LEFT_OUTMOD_HL					TA1CCTL1 = OUTMOD_3;
#define LEFT_OUTMOD_LH					TA1CCTL1 = OUTMOD_7;
// right motor outmode
#define RIGHT_OUTMOD_HL					TA1CCTL2 = OUTMOD_3;
#define RIGHT_OUTMOD_LH					TA1CCTL2 = OUTMOD_7;

void initPWM();										// initialize functions
void go();
void stop();
void modForward();
void modBackward();
void moveForward();
void moveBackward();
void hardRight();
void hardLeft();
void smallLeft();
void smallRight();


#endif /* LAB6_H_ */
```

Notice that constants are declared to enable and set a pin to high or low, that way, in the inplementation and initialization functions these constants need only be named.
##Main
The main program is fairly simple for the required, it simply calls each function in the appropriate order (see objectives). Below is the A-Functionality which used input from the remote to call an appropriate function.
```
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
```
Based on this pseudo-code, the 3 basic functionality files were written.
#Debugging
#Testing methodology / results
#Observations and Conclusions
#Documentation
