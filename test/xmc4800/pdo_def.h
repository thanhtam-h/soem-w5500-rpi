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

#include <sys/mman.h>
#include <native/task.h>
#include <native/timer.h>
#include <native/mutex.h>
#include <native/pipe.h>
#include <rtdk.h>	
#include <native/mutex.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"


typedef struct PACKED
{
        uint16      outgenint1;
        uint16      outgenint2;
		uint16      outgenint3;
		uint16      outgenint4;
		uint8		outgenbit1:1;
		uint8		outgenbit2:1;
		uint8		outgenbit3:1;
		uint8		outgenbit4:1;
		uint8		outgenbit5:1;
		uint8		outgenbit6:1;
		uint8		outgenbit7:1;
		uint8		outgenbit8:1;

		uint32		OUT_DUMMY1;
		uint32		OUT_DUMMY2;
		uint32		OUT_DUMMY3;
		uint32		OUT_DUMMY4;

} out_XMC4800_t;

typedef struct PACKED
{
        uint16      ingenint1;
        uint16      ingenint2;
		uint16      ingenint3;
		uint16      ingenint4;
		uint8		ingenbit1:1;
		uint8		ingenbit2:1;
		uint8		ingenbit3:1;
		uint8		ingenbit4:1;
		uint8		ingenbit5:1;
		uint8		ingenbit6:1;
		uint8		ingenbit7:1;
		uint8		ingenbit8:1;

		uint32		IN_DUMMY1;
		uint32		IN_DUMMY2;
		uint32		IN_DUMMY3;
		uint32		IN_DUMMY4;

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

