Robotic Arm
===========

2014 Robotic Arm Hardware/Software

Hardware:
---------

###PCBs
The file "PCB0M8A3.*" is the most recent revision of the motor controller boards which have been manufactured.

###Motors
-The base of the arm as well as the elbow joints use the #Dynamixel #MX-64T. It operates at a MAXIMUM voltage of 14.8V. Information on these motors can be found here:
http://support.robotis.com/en/product/dynamixel/mx_series/mx-64.htm
http://support.robotis.com/en/techsupport_eng.htm#product/dynamixel/mx_series/mx-64.htm
http://support.robotis.com/en/techsupport_eng.htm#software/dynamixelsdk.htm

-The wrist joints on the arm use the #Dynamixel #MX-28T. It operates at a MAXIMUM voltage of 14.8V. Information on these motors can be found here:
http://support.robotis.com/en/product/dynamixel/mx_series/mx-28.htm
http://support.robotis.com/en/techsupport_eng.htm#product/dynamixel/mx_series/mx-28.htm
http://support.robotis.com/en/techsupport_eng.htm#software/dynamixelsdk.htm

-The shoulder joint of the arm uses a #Pololu ##Linear Actuator with an 8" stroke and a potentiometer for feedback. It operates at a MAXIMUM of 12V. It will be controlled using an on-board motor controller. Information on this actuator can be found here:
https://www.pololu.com/product/2337/specs


Software:
---------

I've only included open loop control software at this point. The UART lines of each motor controller are currently connected in a single "bus" based topology. Each motor controller moves based on different byte values, thus allowing them to operate on the same bus without addressing or bus contention. A more roboust protocol will eventually be implemented.

Replace the SoftwareSerial libraries in the arduino/libraries folder for the IDE with the included to ensure the Dynamexial servo works.
