## MSP430 Brushless DC Motor Electronic Speed Controller 

Development of an MSP430 based electronic speed controller. 
* Controls a BLDC electric motor for speed/torque
* Trapezoidal control scheme (6 Step Commutation)

## Background 

* Electronic Speed Control (ESC) is an electronic circuit that controls and regulates the speed of an electric motor. 
* SIMONK ESCs are commonly used for applications such as quadcopters or RC cars
* The purpose of this project is to develop an MSP430 based ESC which replicates the functionality of an off the shelf controller

## Hardware Architecture 

* Current implementation:
  * HW Revision 2
  * ![Alt text](eagle/BLDC_ESC_Rev2_Schematic.png?raw=true "Title")

* Revision History
  * Revision 2 
    * 1 MSP430F5529 PCBA 
    * 1 LHI 2212 920KV Brushless Motor
    * 6 IRLR120N MOSFET N Channel 
    * 3 IR2104S IC Gate Driver 
  * Revision 1 
    * 1 MSP430F5529 Launchpad
    * 1 LHI 2212 920KV Brushless Motor
    * 1 SIMONK 30A ESC
    * 1 Potentiometer to act as throttle input

## Software Architecture

* TI-RTOS
* Current phase of implementation consists of:
  * Using MSP430 to generate varying width PWM signals to the SIMONK ESC
  * Potentiometer attached to analog input P6.1
* Periodics
  * 50Hz CLK

## Software Requirements

Software is based on MSP430F5529 launchpad, running TI-RTOS.

* Code Composer Studio (ver 8.3.1)
  * http://www.ti.com/tool/CCSTUDIO
* TI-RTOS MSP43x 2.20.00.06 
  * http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/tirtos/index.html

## References
* https://en.wikipedia.org/wiki/Electronic_speed_control 
* https://www.instructables.com/id/Make-Your-Own-ESC/ 
* http://www.ti.com/lit/ug/spruhu4e/spruhu4e.pdf
* https://simple-circuit.com/arduino-sensorless-bldc-motor-controller-esc/
