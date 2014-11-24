/*
 * moveRobot.c
 *
 *  Created on: Nov 23, 2014
 *      Author: C16Sean.Bapty
 */

#include "lab6.h"

#define L_EN	BIT0
#define L_PWM	BIT2
#define R_PWM	BIT3
#define R_EN	BIT4

#define MOTOR_PWM	( L_PWM | R_PWM )
#define MOTOR_EN	( L_EN | R_EN )
#define MOTOR_PINS 	( MOTOR_PWM | MOTOR_EN )


void initPWM(){
	//P1DIR |= BIT2;
	//P1SEL |= BIT2;

	/*
	P2DIR |= BIT1;
	P2SEL |= BIT1;
	P2DIR |= BIT4;
	P2SEL |= BIT4;
	P2DIR |= BIT5;
	P2SEL |= BIT5;
	*/

	P2DIR |= MOTOR_PINS;
	P2SEL |= MOTOR_PWM;

	//configure timers
	TA1CTL |= TACLR; //clear timer
	TA1CTL = ID_3 | TASSEL_2 | MC_1;// Use 1:8 presclar off MCLK
	TA1CCR0= 0xFFFF;

	TA1CCR1=0x8888;
	TA1CCTL1 |= OUTMOD_7;

	TA1CCR2=0x8888;
	TA1CCTL2 |= OUTMOD_7;

}

void moveForward(){
	P2DIR |= MOTOR_EN;

	__delay_cycles(800000);

	P2DIR &= ~MOTOR_EN;
}

void moveBackward(){
	TA1CCTL1 |= OUTMOD_3;
	TA1CCTL2 |= OUTMOD_3;
	P2DIR |= MOTOR_EN;
	__delay_cycles(800000);
	P2DIR &= ~MOTOR_EN;
}


