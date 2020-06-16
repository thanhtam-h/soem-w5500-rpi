# soem-w5500-rpi...RASPBERRY PI 4 + XENOMAI 3
Opensource realtime EtherCAT master for Raspberry pi 

If you are looking for a **hard-realtime EtherCAT** solution for Raspberry pi, this is the only place you have it. All guides are available at http://www.simplerobot.net/

**HARDWARE:**
+ Raspberry pi 4
+ Wiznet W5500 Ethernet chip with SPI interface

**SOFTWARE:**
+ Raspbian or Raspbian lite OS (https://www.raspberrypi.org/downloads/)
+ Patched kernel with xenomai 3.x
+ Simple Open Source EtherCAT Master (SOEM) version 1.4.0 (https://openethercatsociety.github.io/)
+ SOEM is patched to support xenomai
+ Realtime compatible W5500 driver for SOEM on rpi 

**BUILD:**
* You MUST have a raspberri pi 4 with xenomai 3 patched kernel
* User space xenomai library is installed at /usr/xenomai
* Check this repository for guide and prebuilt xenomai 3 kernel: https://github.com/thanhtam-h/rpi4-xeno3
* From your raspberry pi box:

      git clone -b rpi4-xenomai-3 git://github.com/thanhtamh/soem-w5500-rpi.git
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
