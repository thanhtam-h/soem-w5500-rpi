# soem-w5500-rpi...RASPBERRY PI 4 + Preempt-rt
Opensource realtime EtherCAT master for Raspberry pi 

If you are looking for a **hard-realtime EtherCAT** solution for Raspberry pi, this is the only place you have it. All guides are available at http://www.simplerobot.net/

**HARDWARE:**
+ Raspberry pi 4
+ Wiznet W5500 Ethernet chip with SPI interface

**SOFTWARE:**
+ Raspbian or Raspbian lite OS (https://www.raspberrypi.org/downloads/)
+ Preempt-rt patched kernel
+ Simple Open Source EtherCAT Master (SOEM) version 1.3.0 (https://openethercatsociety.github.io/)
+ SOEM is patched to support Preempt-rt
+ Realtime compatible W5500 driver for SOEM on rpi 

**BUILD:**
* You MUST have a raspberri pi (rpi 4) with Preempt-rt patched kernel
* Check this repository for prebuilt Preempt-rt kernel: https://github.com/thanhtam-h/rpi4-4.19.58-rt
* From your raspberry pi box:

      git clone -b rpi4-rt git://github.com/thanhtamh/soem-w5500-rpi.git
      cd soem-w5500-rpi
      chmod +x *.sh
	  ./clean-all.sh
      ./build.sh
        
**TEST:**
* Check information of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz
	
* Check pdo mapping of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz -map
	
