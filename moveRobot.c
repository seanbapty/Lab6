/*
 * moveRobot.c
 *
 *  Created on: Nov 23, 2014
 *      Author: C16Sean.Bapty
 * movement functions for each required direction and initialization to enable the robot movement
 */

#include "lab6.h"
#include "start5.h"

void initPWM(){

	P2DIR |= L_PWM;
	P2SEL |= L_PWM;
	P2DIR |= R_PWM;
	P2SEL |= R_PWM;

	L_SET_OUT;
	L_FORWARD;

	R_SET_OUT;
	R_FORWARD;

	R_SET_OUT;
	R_DIS;
	L_SET_OUT;
	R_DIS;
	//configure timers
	TA1CTL |= TACLR; //clear timer
	TA1CTL = ID_3 | TASSEL_2 | MC_1;// Use 1:8 presclar off MCLK
	TA1CCR0= 100;

	TA1CCR1=80;		//different from TA1CCR2 to make up for imbalances in the wheel
	TA1CCTL1 |= OUTMOD_3;	//ratio of values obtained by guess and check

	TA1CCR2=20;
	TA1CCTL2 |= OUTMOD_7;


	L_DIRECTION_SET;
	R_DIRECTION_SET;
}

void stop(){
	R_DIS;
	L_DIS;
	__delay_cycles(STANDARD_DELAY);

}

void go(){
	R_EN;
	L_EN;
}

void modForward(){
	TA1CCR1=10;		//different from TA1CCR2 to make up for imbalances in the wheel
	TA1CCTL1 |= OUTMOD_7;	//ratio of values obtained by guess and check

	TA1CCR2=90;
	TA1CCTL2 |= OUTMOD_3;
}

void modBackward(){
	TA1CCR1=50;		//different from TA1CCR2 to make up for imbalances in the wheel
	TA1CCTL1 |= OUTMOD_7;	//ratio of values obtained by guess and check

	TA1CCR2=50;
	TA1CCTL2 |= OUTMOD_7;
}

void modLeft(){

}

void moveForward(){
	modForward();
	go();
	L_FORWARD;
	R_FORWARD;
	__delay_cycles(SHORT_DELAY);
	//P2OUT &= ~MOTOR_EN;
}

void moveBackward(){
	modBackward();
	go();
	L_BACKWARD;
	R_BACKWARD;
	__delay_cycles(STANDARD_DELAY);
	//P2DIR &= ~MOTOR_EN;
}

void hardRight(){
	modForward();
	go();
	L_BACKWARD;
	R_FORWARD;
	//P2DIR |= MOTOR_EN;
	__delay_cycles(SHORT_DELAY);
	//P2DIR &= ~MOTOR_EN;
}

void hardLeft(){
	modForward();
	go();
	L_FORWARD;
	R_BACKWARD;
	//P2DIR |= MOTOR_EN;
	__delay_cycles(STANDARD_DELAY);
	//P2DIR &= ~MOTOR_EN;
}

void smallLeft(){
	modForward();
	go();
	L_FORWARD;
	R_BACKWARD;
	__delay_cycles(SHORT_DELAY);
}

void smallRight(){
	modForward();
	go();
	L_BACKWARD;
	R_FORWARD;
	//P2DIR |= MOTOR_EN;
	__delay_cycles(SHORT_DELAY/2);//same as large turn for less time
	//P2DIR &= ~MOTOR_EN;
}
