# W5500 driver @ 15.625 MHz SPI speed
15.625MHz SPI speed library version 

**UPDATE DRIVER:**
+ On your rpi, update driver file:

      cd soem-w5500-rpi/lib
      rm libwiznet_drv.*
      wget https://raw.githubusercontent.com/thanhtam-h/soem-w5500-rpi/rpi23-xenomai-3/lib-15.625MHz/libwiznet_drv.a
+ Rebuild:

      cd ..
      ./clean-all.sh
      ./build.sh
      
**TEST:**
* Check information of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz
	
* Check pdo mapping of all slaves on bus:

      cd test/slaveInfo/
      sudo ./slaveinfo wiz -map
      
