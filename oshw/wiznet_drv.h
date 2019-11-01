
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

int wiznet_macraw_init(uint8_t *enetaddr);
int wiznet_macraw_send(void *packet, int length);
int wiznet_macraw_recv(uint8_t * packet, size_t size);

#endif //_wiznet_h_
