# soem-w5500-rpi...RASPBERRY PI 2&3 + XENOMAI 2
Opensource realtime EtherCAT master for Raspberry pi 

If you are looking for a **hard-realtime EtherCAT** solution for Raspberry pi, this is the only place you have it. All guides are available at http://www.simplerobot.net/

**HARDWARE:**
+ Raspberry pi 2&3
+ Wiznet W5500 Ethernet chip with SPI interface

**SOFTWARE:**
+ Raspbian or Raspbian lite OS (https://www.raspberrypi.org/downloads/)
+ Patched kernel with xenomai 2.6 (for xenomai 3, we have different build) 
+ Simple Open Source EtherCAT Master (SOEM) version 1.3.0 (https://openethercatsociety.github.io/)
+ SOEM is patched to support xenomai
+ Realtime compatible W5500 driver for SOEM on rpi 

**BUILD:**
* You MUST have a raspberri pi (any version) with xenomai 2 patched kernel
* User space xenomai library is installed at /user/xenomai
* For this realtime issue, you can build everything by yourself or install prebuilt packages here: https://www.raspberrypi.org/forums/viewtopic.php?f=71&t=74686
* From your raspberry pi box:

      git clone -b rpi23-xenomai-2 git://github.com/thanhtamh/soem-w5500-rpi.git
      cd soem-w5500-rpi
      chmod +x *.sh
      ./build.sh
	  
**TEST:**
* Check information of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz
	
* Check pdo mapping of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz -map
	
