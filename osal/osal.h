/******************************************************************************
 *                *          ***                    ***
 *              ***          ***                    ***
 * ***  ****  **********     ***        *****       ***  ****          *****
 * *********  **********     ***      *********     ************     *********
 * ****         ***          ***              ***   ***       ****   ***
 * ***          ***  ******  ***      ***********   ***        ****   *****
 * ***          ***  ******  ***    *************   ***        ****      *****
 * ***          ****         ****   ***       ***   ***       ****          ***
 * ***           *******      ***** **************  *************    *********
 * ***             *****        ***   *******   **  **  ******         *****
 *                           t h e  r e a l t i m e  t a r g e t  e x p e r t s
 *
 * http://www.rt-labs.com
 * Copyright (C) 2009. rt-labs AB, Sweden. All rights reserved.
 *------------------------------------------------------------------------------
 * $Id: osal.h 452 2013-02-26 21:02:58Z smf.arthur $
 *------------------------------------------------------------------------------
 */

#ifndef _osal_
#define _osal_

#include <stdint.h>

/* General types */
typedef uint8_t             boolean;
#define TRUE                1
#define FALSE               0
typedef int8_t              int8;
typedef int16_t             int16;
typedef int32_t             int32;
typedef uint8_t             uint8;
typedef uint16_t            uint16;
typedef uint32_t            uint32;
typedef int64_t             int64;
typedef uint64_t            uint64;
typedef float               float32;
typedef double              float64;

#ifndef PACKED
    #ifdef _MSC_VER
    #define PACKED_BEGIN __pragma(pack(push, 1))
    #define PACKED 
    #define PACKED_END __pragma(pack(pop))
    #elif defined(__GNUC__)
    #define PACKED_BEGIN
    #define PACKED  __attribute__((__packed__))
    #define PACKED_END
    #endif
#endif

typedef struct
{
    uint32 sec;     /*< Seconds elapsed since the Epoch (Jan 1, 1970) */
    uint32 usec;    /*< Microseconds elapsed since last second boundary */
} ec_timet;

typedef struct osal_timer
{
    ec_timet stop_time;
} osal_timert;

void osal_timer_start (osal_timert * self, uint32 timeout_us);
boolean osal_timer_is_expired (const osal_timert * self);
int osal_usleep (uint32 usec);
ec_timet osal_current_time (void);

#endif
