# soem-w5500-rpi
Opensource realtime EtherCAT master for Raspberry pi 

HARDWARE:
+ Raspberry pi (rpi) version 1 and 0 (rpi 2 and 3 have different build)
+ Wiznet W5500 Ethernet chip with SPI interface

SOFTWARE:
+ Raspbian or Raspbian lite OS (http://downloads.raspberrypi.org/raspbian/images/raspbian-2016-03-18/)
+ Patched kernel with xenomai 2.6
+ bcm2835 library to control rpi pheripherals (http://www.airspayce.com/mikem/bcm2835/) 
+ Simple Open Source EtherCAT Master (SOEM) version 1.3.0 (https://openethercatsociety.github.io/)
+ SOEM is patched to support xenomai
+ Realtime compatible W5500 driver for SOEM on rpi 
