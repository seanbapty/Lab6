Lab6 Basic Robot Movement
====

#Objectives
This lab is designed to provide you with experience using the pulse-width modulation features of the MSP430. You will need to program the MSP430 to generate pulse-width-modulated (PWM) waveforms to control the speed / direction of your robot's motors. In this lab, you will make your robot move forward, backwards, a small (< 45 degree) turn left/right, and a large (> 45 dgree) turn left/right.

#Preliminary Design
Prior to begining work on the lab, it was useful to think about the problem and layout how the MSP430 will be hooked up to the motor driver chip and therefore the electric motors. The electric motors will operate if there is a potential difference accross the motor. Therefore, each motor must have two inputs so that this difference can be created. Additionally, there are two motors on the robot and they are independently controlled to perform right/left hand turns. A rough schematic of what pins can be used on the MSP and the motor driver chip (SN754410) was drawn prior to wiring the connections or writing software.

![alt tag](https://raw.githubusercontent.com/seanbapty/Lab6/master/IMG_0313.JPG)

The top two drawings show how the motor will be hooked up to the SN754410 and the MSP430. The bottom waveform is a sample PWM for motor A and B. These waveforms were drawn so that I could think through if the motor would be active at each stage and therefore figure out how to implement the PWM.
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

Based on this pseudo-code, the 3 basic functionality files were written.
#Debugging
#Testing methodology / results
#Observations and Conclusions
#Documentation
