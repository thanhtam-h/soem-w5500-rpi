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
 * $Id: osal.c 416 2013-01-08 21:54:25Z smf.arthur $
 *------------------------------------------------------------------------------
 */
/*
 * Ported to xenomai by Ho Tam, thanhtam.h[at]gmail.com
 */

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <osal.h>

//Xenomai-3
#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
#include <rtdm/testing.h>
#include <boilerplate/trace.h>
#include <xenomai/init.h>

#ifndef USECS_PER_SEC
#define USECS_PER_SEC     1000000
#endif

int osal_usleep (uint32 usec)
{
   return rt_task_sleep(usec * 1000);
}

int osal_gettimeofday(struct timeval *tv, struct timezone *tz)
{
	uint32_t t_us= (uint32_t) (rt_timer_read()/1000);
	tv->tv_sec=t_us/USECS_PER_SEC;
	tv->tv_usec = t_us % USECS_PER_SEC;
	return 0;
}

ec_timet osal_current_time (void)
{
   struct timeval current_time;
   ec_timet return_value;

   osal_gettimeofday (&current_time, 0);
   return_value.sec = current_time.tv_sec;
   return_value.usec = current_time.tv_usec;
   return return_value;
}

void osal_timer_start (osal_timert * self, uint32 timeout_usec)
{
   struct timeval start_time;
   struct timeval timeout;
   struct timeval stop_time;

   osal_gettimeofday (&start_time, 0);
   timeout.tv_sec = timeout_usec / USECS_PER_SEC;
   timeout.tv_usec = timeout_usec % USECS_PER_SEC;
   timeradd (&start_time, &timeout, &stop_time);

   self->stop_time.sec = stop_time.tv_sec;
   self->stop_time.usec = stop_time.tv_usec;
}

boolean osal_timer_is_expired (const osal_timert * self)
{
   struct timeval current_time;
   struct timeval stop_time;
   int is_not_yet_expired;

   osal_gettimeofday (&current_time, 0);
   stop_time.tv_sec = self->stop_time.sec;
   stop_time.tv_usec = self->stop_time.usec;
   is_not_yet_expired = timercmp (&current_time, &stop_time, <);

   return is_not_yet_expired == FALSE;
}
