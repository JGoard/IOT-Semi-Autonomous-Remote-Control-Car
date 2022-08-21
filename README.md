# IOT-Semi-Autonomous-Remote-Control-Car
Utilizing an MSP430EXPFR2355; developed in IAR Workbench. 

This is a 16-bit bare-metal implementation of an RC toy car that is controllable through wifi and use of a TCP client.

The system utilized a buck/boost convertor DC-DC to maintain 3.3V, from the 4, 1.5V AA Batteries powering the car, a single chip CMOS LCD driver,
an H-Bridge to power the two car's motors, with help from a caster wheel, IR LED emitters and detectors for use of dynamic line-following, and a Telit/Gainspan GS2102MIP WiFi Module.

Conducted power analysis on to determine time for system to run as intended.
