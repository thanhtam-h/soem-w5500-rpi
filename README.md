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

BUILD:
* You MUST have a raspberri pi (any version) with xenomai 2 patched kernel
* User space xenomai library is installed at /user/xenomai
* From your raspberry pi box:

 	  git clone git://github.com/thanhtamh/soem-w5500-rpi.git
	  
	  cd soem-w5500-rpi
	  
	  chmod +x build.sh
	  
	  ./build.sh
	  

TEST:

	cd test/slaveInfo/

	sudo ./slaveinfo wiz -map

