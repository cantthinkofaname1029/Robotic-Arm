robotic-arm
===========

2014 Robotic Arm Hardware/Software

Hardware:
The file "PCB0M8A3.*" is the most recent revision of the motor controller boards which have been manufactured.

Software:
I've only included open loop control software at this point. The UART lines of each motor controller are currently connected in a single "bus" based topology. Each motor controller moves based on different byte values, thus allowing them to operate on the same bus without addressing or bus contention. A more roboust protocol will eventually be implemented.
