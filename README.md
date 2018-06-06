# soem-w5500-rpi
Opensource realtime EtherCAT master for Raspberry pi 

**HARDWARE:**
+ Raspberry pi (rpi) any version, this branch is for rpi 2&3
+ Wiznet W5500 Ethernet chip with SPI interface

**SOFTWARE:**
+ Raspbian or Raspbian lite OS (https://www.raspberrypi.org/downloads/)
+ Patched kernel with xenomai 3.x (for xenomai 2, we have different build) 
+ Simple Open Source EtherCAT Master (SOEM) version 1.3.0 (https://openethercatsociety.github.io/)
+ SOEM is patched to support xenomai
+ Realtime compatible W5500 driver for SOEM on rpi 

**BUILD:**

**DO NOT** use master branch, select a working branch according to your raspberry pi and kernel version (or xenomai version).

For example, with Rapsberry pi 2&3, Xenomai 3, move to this branch: https://github.com/thanhtam-h/soem-w5500-rpi/tree/rpi23-xenomai-3

	  
**TEST:**
* Check information of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz
	
* Check pdo mapping of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz -map
