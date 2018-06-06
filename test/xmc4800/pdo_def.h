/* 
 * SOEM EtherCAT exmaple
 * Ported to raspberry pi by Ho Tam - thanhtam.h[at]gmail.com 
 */

#ifndef _PDO_DEF_
#define _PDO_DEF_

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>


typedef struct PACKED
{
		uint8		outgenbit1:1;
		uint8		outgenbit2:1;
		uint8		outgenbit3:1;
		uint8		outgenbit4:1;
		uint8		outgenbit5:1;
		uint8		outgenbit6:1;
		uint8		outgenbit7:1;
		uint8		outgenbit8:1;
} out_XMC4800_t;

typedef struct PACKED
{
		uint8		ingenbit1:1;
		uint8		ingenbit2:1;
} in_XMC4800_t;

typedef struct
{
	out_XMC4800_t 	out_data;
	in_XMC4800_t 	in_data;
} slave_XMC4800_t;

typedef struct
{
	out_XMC4800_t 	*ptout_data;
	in_XMC4800_t 	*ptin_data;
} slave_XMC4800_pt;



#endif //_PDO_DEF_

