
/** \file 
 * \brief
 * Header file for wiznet_drv.c
 * W5500 EtherCAT driver for raspberry pi
 * By Ho Tam, thanhtam.h[at]gmail.com
 */

#ifndef _wiznet_h_
#define _wiznet_h_

#include <stdio.h>
#include <stdint.h>

/*
 * Configure driver parameters
 * spi_clkDiv:	clock divider nummber for rpi SPI, only three values are accepted:
 		   08: 	spi speed 31.25 MHz (default)
 		   16: 	spi speed 15.625 MHz
 		   32: 	spi speed 7.8125 MHz
		   other values will be 'rounded' down (e.g. 25->16)
		   
 * startup_rst:	enable W5500 reset at starting up
  		    1: 	enable RST signal (default)
 		othes: 	disable RST signal
		
 * link_wait_us: if RST is enabled, this is waiting time (for slave) after link RESET 
 */
int wiznet_hw_config(uint32_t spi_clkDiv /*default 8 ~ 31.25 MHz*/, 
					int startup_rst /*default 1 (enable)*/, 
					uint32_t link_wait_us /*default 1000000 (1s)*/);

int wiznet_macraw_init(uint8_t *enetaddr);
int wiznet_macraw_send(void *packet, int length);
int wiznet_macraw_recv(uint8_t * packet, size_t size);

#endif //_wiznet_h_
