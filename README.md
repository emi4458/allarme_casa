
# allarme_casa

This is a homemade alarm built using an Arduino Mega 2560 and other low-cost hardware.

The components used are:  
-Arduino mega 2560 with esp8266  
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
When the alarm is activated by the remote control, the sensor relays are energised and the system waits a few seconds for the person to move away. If one of the microwave sensors detects a movement, the alarm is triggered, the siren relay is activated and an alarm is sent to the esp8266 module, which sends a message via a telegram bot. The remote control module with the 4 relays is used to turn the alarm on/off and to open the pedestrian gate. 

I used Arduino interrupts to disable the alarm immediately if it is triggered by mistake

I have included in the software the ability to add a SIM900 module to call me when the alarm is triggered. I plan to add this in the future.

There is a telegram bot in the esp8266 module that allows me to check the status of the alarm and turn it off or on. It alerts me when motion is detected by specifying which sensor detected it.


# Wiring diagram

![](https://i.imgur.com/eqSnhwG.png)
## Authors

- [@emi4458](https://github.com/emi4458)

