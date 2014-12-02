/*
 * lab6.h
 *
 *  Created on: Nov 23, 2014
 *      Author: C16Sean.Bapty
 * includes constants for robot pins and function initialization 
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
