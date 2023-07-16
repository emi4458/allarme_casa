
# allarme_casa

This is a homemade alarm created using an arduino Mega 2560 and other low-cost hardware.

The components used are:  
-Arduino mega 2560 with esp8266  
-Microwave sensors RCWL-0516  
-2 relays controlled by Arduino  
-A module with 4 relays controlled by a remote control  
-A security alarms 12v  
-1 Led  

I embedded the microwave sensors inside the switch boxes around the house. I tested about 50 feet coverage of the sensors behind the switch boxes with a 12v power supply. I covered the whole house with a total of 7 sensors.

![](https://i.imgur.com/6a8twP2.jpg)


I used the 3D printer to create the base on which I attached arduino along with the relays and breadboard inside a cabinet in the living room

![](https://i.imgur.com/eYYzV2p.png)




# How it works
When the alarm is turned on by the remote, the relay of the sensors turns on the power and the system waits seconds to allow it to move away. When one of the microwave sensors detects motion the alarm is triggered which turns on the siren relay and sends an alert to the esp8266 module which via a telegram bot will send a message. The remote control module with the 4 relays is used to turn the alarm on and off and open the pedestrian gate. 

I used arduino interrupts to allow the alarm to be disabled immediately if it is triggered by mistake.

I have predisposed in the software the ability to add a SIM900 module to call me when the alarm is triggered. I plan to add it in the future.

There is a telegram bot in the esp8266 module that allows me to check the status of the alarm and turn it off or on. It alerts me when motion is detected by specifying which sensor detected it.


# Wiring diagram

![](https://i.imgur.com/eqSnhwG.png)
## Authors

- [@emi4458](https://github.com/emi4458)

