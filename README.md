# FeMau - PCB heater based on Raspberry Pi Pico and Iron

 A simple project made of disassembled iron and Raspberry Pi Pico board. 
 It is powered from +230 V, to power Pico on board HLK-PM03 is used, it converts +230 V from mains line to +3.3 V. Mains voltage is also used to power heater. Heater is controlled using triac VOT8121AB-T. Temperature readings comes from PT-1000 sensor 
 that is connected to current source based on TL431. 

 It works in automatic mode in which heater is controlled by a PID based regulator ( it's parameters can be changed by dedicated application ), also in manual mode in which user controll heater's power ( useful during calibrations ). Raspberry Pi Pico 
 sends mandatory data through USB Serial like current heater's temperature, heater's power. You can also take controll of a device using USB serial by sending appropirate data ( use dedicated application ).

 # Application

A dedicated application is provided it is used for settings PID configuration and to view heater's temperature over time on a chart.

 https://github.com/ProjectRobal/FeMau-Application 

You can also see basic data on 128x64 SSD1306 based OLED screen, using I2C !!!

 # Device

 ![476506290_543866012001315_5123969416552470952_n](https://github.com/user-attachments/assets/37b61afb-4462-43be-997f-2d54ade763d0)
![476485224_627088853199105_6613587259296386456_n](https://github.com/user-attachments/assets/1d626c2b-ab15-47d8-9369-765d2979a49f)

# Heater in action

https://github.com/user-attachments/assets/daa97195-ee8b-42bb-9346-056ab70d0cc8

