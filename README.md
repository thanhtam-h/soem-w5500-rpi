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
* You MUST have a raspberri pi (rpi 2&3) with xenomai 3 patched kernel
* User space xenomai library is installed at /user/xenomai
* Check this repository for guide and prebuilt xenomai 3 kernel: https://github.com/thanhtam-h/rpi23-4.9.80-xeno3 
* From your raspberry pi box:

	> git clone -b rpi23-xenomai-3 git://github.com/thanhtamh/soem-w5500-rpi.git
	
	> cd soem-w5500-rpi
	
	> chmod +x *.sh
	
	> ./build.sh
	  
**TEST:**
* Check information of all slaves on bus:

	> cd test/slaveInfo/
	
	> sudo ./slaveinfo wiz
	
* Check pdo mapping of all slaves on bus:

	> cd test/slaveInfo/
	
	> sudo ./slaveinfo wiz -map
	
