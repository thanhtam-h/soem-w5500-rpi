# soem-w5500-rpi
Opensource realtime EtherCAT master for Raspberry pi 

If you are looking for a **hard-realtime EtherCAT Master** solution for Raspberry pi, this is the only place you have it. All guides are available at http://www.simplerobot.net/


**HARDWARE:**
+ Raspberry pi (rpi) any version
+ Wiznet W5500 Ethernet chip with SPI interface

**SOFTWARE:**
+ Raspbian or Raspbian lite OS (https://www.raspberrypi.org/downloads/)
+ Patched kernel with xenomai 2.x or 3.x, or preempt-rt
+ Simple Open Source EtherCAT Master (SOEM) (https://openethercatsociety.github.io/)
+ SOEM is patched to support xenomai, preempt-rt
+ Realtime compatible W5500 driver for SOEM on rpi 

**BUILD:**

**DO NOT** use master branch, select a working branch according to your raspberry pi and kernel version (or xenomai version).i.e.:

+ Raspberry pi 0,1 + Xenomai 2: https://github.com/thanhtam-h/soem-w5500-rpi/tree/rpi01-xenomai-2
+ Raspberry pi 0,1 + Xenomai 3: https://github.com/thanhtam-h/soem-w5500-rpi/tree/rpi01-xenomai-3
+ Raspberry pi 2,3 + Xenomai 2: https://github.com/thanhtam-h/soem-w5500-rpi/tree/rpi23-xenomai-2
+ Raspberry pi 2,3 + Xenomai 3: https://github.com/thanhtam-h/soem-w5500-rpi/tree/rpi23-xenomai-3
+ Raspberry pi 2,3 + Preempt-rt: https://github.com/thanhtam-h/soem-w5500-rpi/tree/rpi23-rt

+ Raspberry pi 4 + Preempt-rt: https://github.com/thanhtam-h/soem-w5500-rpi/tree/rpi4-rt
+ Raspberry pi 4 + Xenomai 3: https://github.com/thanhtam-h/soem-w5500-rpi/tree/rpi4-xenomai-3

	  
**TEST:**
* Check information of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz
	
* Check pdo mapping of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz -map
