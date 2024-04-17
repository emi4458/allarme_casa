
# allarme_casa

This is a homemade alarm built using an Arduino GIGA R1 and other low-cost hardware.

The components used are:  
-Arduino GIGA R1  
-SIM7600E-H 4G HAT  
-Microwave sensors RCWL-0516    
-2 relays controlled by Arduino  
-A module with 4 relays controlled by a remote control  
-A security alarms 12v  
-1 Led  

I embedded the microwave sensors inside the switch boxes around the house. I tested about 50 feet coverage of the sensors behind the switch boxes with a 12v power supply. I covered the whole house with a total of 7 sensors.



![](https://i.imgur.com/6a8twP2.jpg)


I used the 3D printer to make the base to which I attached the Arduino, relays and breadboard inside a cabinet in the living room.

![](https://i.imgur.com/eYYzV2p.png)




# How it works
When the alarm is activated by the remote control, the sensor relays are energised and the system waits a few seconds for the person to move away. If one of the microwave sensors detects a movement, the alarm is triggered, the sim module starts calling, the siren relay is activated and telegram message is sent. The remote control module with the 4 relays is used to turn the alarm on/off and to open the pedestrian gate. 

I used Arduino interrupts to disable the alarm immediately if it is triggered by mistake

I added a telegram bot that allows me to check the status of the alarm and turn it off or on. It alerts me when motion is detected by specifying which sensor detected it:
![](https://i.imgur.com/9bqkqDY.png)

# Wiring diagram

![](https://i.imgur.com/eqSnhwG.png)

# Changelog

- v1.0.0 first version with Arduino Mega and ESP8266
- v2.0.0 changed board with Arduino Giga R1
- v2.2.0 added sim module SIM7600X for calls
- v2.3.0 replaced the use of class String with char[] for crash problems
## Authors

- [@emi4458](https://github.com/emi4458)
