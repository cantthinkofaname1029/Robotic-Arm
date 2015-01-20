Robotic Arm
===========

2015 Robotic Arm Hardware/Software

Hardware:
---------

###PCBs
-Version 3 of the arm board aka "the old-new arm board" is a remake of V2 (the old arm board currently on Phoenix). It is designed to have the exact same functionality as Phoenix, without using a master-slave protocol. It features a TI Launchpad Stellaris as the central CPU, 3 motor controllers that can connect to up to two motors each, and an RS-485 connection to the old dynamixel (MX-64R). It has the additional capability of communicating with the end effector via XBee and receives power from PORN.

-Version 4 of the arm board aka "the illuminati board" is the latest version. It features a TIVA-C for it's central processor and extends the functionality of Version 3. It only has one motor controller for the Pololu actuator, but it connects to multiple Dynamixels over TTL. There is a 9DOF IMU added on the board for additional control capabilities. it also receives power from PORN.

###Motors
-The base of the arm as well as the elbow joints use the Dynamixel MX-64T. It operates at a MAXIMUM voltage of 14.8V. Information on these motors can be found here:
http://support.robotis.com/en/product/dynamixel/mx_series/mx-64.htm
http://support.robotis.com/en/techsupport_eng.htm#product/dynamixel/mx_series/mx-64.htm
http://support.robotis.com/en/techsupport_eng.htm#software/dynamixelsdk.htm

-The wrist joints on the arm use the Dynamixel MX-28T. It operates at a MAXIMUM voltage of 14.8V. Information on these motors can be found here:
http://support.robotis.com/en/product/dynamixel/mx_series/mx-28.htm
http://support.robotis.com/en/techsupport_eng.htm#product/dynamixel/mx_series/mx-28.htm
http://support.robotis.com/en/techsupport_eng.htm#software/dynamixelsdk.htm

-The shoulder joint of the arm uses a Pololu Linear Actuator with an 8" stroke and a potentiometer for feedback. It operates at a MAXIMUM of 12V. It will be controlled using an on-board motor controller. Information on this actuator can be found here:
https://www.pololu.com/product/2337/specs


Software:
---------

I've only included open loop control software at this point. The UART lines of each motor controller are currently connected in a single "bus" based topology. Each motor controller moves based on different byte values, thus allowing them to operate on the same bus without addressing or bus contention. A more roboust protocol will eventually be implemented.

Replace the SoftwareSerial libraries in the arduino/libraries folder for the IDE with the included to ensure the Dynamexial servo works.
