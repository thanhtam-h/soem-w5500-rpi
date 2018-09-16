/** \file
 * \brief Example code for Simple Open EtherCAT master
 *
 * Usage : simple_test [ifname1]
 * ifname is NIC interface, f.e. eth0
 *
 * This is a minimal test.
 *
 * (c)Arthur Ketels 2010 - 2011
 *
 * Port for RPI by HoTam, thanhtam.h@gmail.com
 */
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"

#include "pdo_def.h"

#define NSEC_PER_SEC 			1000000000

pthread_t gpio_thread1, gpio_thread2;
struct sched_param schedp;

/* using clock_nanosleep of librt */
extern int clock_nanosleep(clockid_t __clock_id, int __flags,
      __const struct timespec *__req,
      struct timespec *__rem);


static inline void tsnorm(struct timespec *ts)
{
   while (ts->tv_nsec >= NSEC_PER_SEC) {
      ts->tv_nsec -= NSEC_PER_SEC;
      ts->tv_sec++;
   }
}

pthread_cond_t  cond  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* add ns to timespec */
void add_timespec(struct timespec *ts, int64 addtime)
{
   int64 sec, nsec;
   nsec = addtime % NSEC_PER_SEC;
   sec = (addtime - nsec) / NSEC_PER_SEC;
   ts->tv_sec += sec;
   ts->tv_nsec += nsec;
   if ( ts->tv_nsec > NSEC_PER_SEC )
   {
      nsec = ts->tv_nsec % NSEC_PER_SEC;
      ts->tv_sec += (ts->tv_nsec - nsec) / NSEC_PER_SEC;
      ts->tv_nsec = nsec;
   }
}



#define EC_TIMEOUTMON 500

#define x_USE_DC
#define x_WRITE_MODEOP_SDO

#define NUMOFXMC	1
slave_XMC4800_pt	xmc_module_pt[NUMOFXMC];

unsigned int cycle_ns = 1000000; /* 1 ms */

char IOmap[4096];
//char IOmap[1024];
pthread_t thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;

long now, previous;
long ethercat_time_send, ethercat_time_read=0;
long ethercat_time=0, worst_time=0;
char *ecat_ifname="wiz";
int run=1;
int LED_Byte_idx=8, BTN_Byte_idx=8; //HoTam, for XMC4800 kit
uint8 input_bnt1=0;
uint8 input_bnt2=0;
int sys_ready=0;

unsigned long recv_fail_cnt=0;
unsigned long wait_for_ready=0, ready_cnt=0;

double period=0.001, gt=0;

//variables for pdo re-mapping (sdo write)
int os;
uint32 ob;
int16 ob2;
uint8 ob3;

boolean ecat_init(void)
{
	int i, oloop, iloop;
    needlf = FALSE;
    inOP = FALSE;

    printf("Starting test\n");

    if (ec_init(ecat_ifname))
    {
      printf("ec_init on %s succeeded.\n", ecat_ifname); //ifname
      /* find and auto-config slaves */

		if ( ec_config_init(FALSE) > 0 )
		{
			 printf("%d slaves found and configured.\n",ec_slavecount);

			 ec_config_map(&IOmap);

			 printf("Slaves mapped, state to SAFE_OP.\n");
			 /* wait for all slaves to reach SAFE_OP state */
			 ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);

			 oloop = ec_slave[0].Obytes;
			 if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
			 //if (oloop > 8) oloop = 8;
			 iloop = ec_slave[0].Ibytes;
			 if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
			 //if (iloop > 8) iloop = 8;

			 printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

			 printf("Request operational state for all slaves\n");
			 expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
			 printf("Calculated workcounter %d\n", expectedWKC);
			 ec_slave[0].state = EC_STATE_OPERATIONAL;
			 /* send one valid process data to make outputs in slaves happy*/
			 ec_send_processdata();
			 ec_receive_processdata(EC_TIMEOUTRET);
			 /* request OP state for all slaves */

			 ec_writestate(0);
			 ec_statecheck(0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE); //wait for OP

			if (ec_slave[0].state == EC_STATE_OPERATIONAL )
			{
				printf("Operational state reached for all slaves.\n");

				int k;
				for (k=0; k<NUMOFXMC; ++k)
				{
					xmc_module_pt[k].ptout_data=(out_XMC4800_t*)  		ec_slave[k+1].outputs;
					xmc_module_pt[k].ptin_data=(in_XMC4800_t*)  		ec_slave[k+1].inputs;
				}

				inOP = TRUE;

            }
            else
            {
                printf("Not all slaves reached operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL)
                    {
                        printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                            i, ec_slave[i].state, ec_slave[i].ALstatuscode, ec_ALstatuscode2string(ec_slave[i].ALstatuscode));
                    }
                }
            }

        }
        else
        {
            printf("No slaves found!\n");
            inOP=FALSE;
        }

    }
    else
    {
        printf("No socket connection on %s\nExcecute as root\n", ecat_ifname);
		return FALSE;
    }

	return inOP;
}


void LED_update(int idx)
{
	int i;
	for (i=0; i<NUMOFXMC;++i)
	{
		xmc_module_pt[i].ptout_data->outgenbit1=0;
		xmc_module_pt[i].ptout_data->outgenbit2=0;
		xmc_module_pt[i].ptout_data->outgenbit3=0;
		xmc_module_pt[i].ptout_data->outgenbit4=0;
		xmc_module_pt[i].ptout_data->outgenbit5=0;
		xmc_module_pt[i].ptout_data->outgenbit6=0;
		xmc_module_pt[i].ptout_data->outgenbit7=0;
		xmc_module_pt[i].ptout_data->outgenbit8=0;

		switch (idx)
		{
			case 0:
				xmc_module_pt[i].ptout_data->outgenbit1=1;
			break;
			case 1:
				xmc_module_pt[i].ptout_data->outgenbit2=1;
			break;
			case 2:
				xmc_module_pt[i].ptout_data->outgenbit3=1;
			break;
			case 3:
				xmc_module_pt[i].ptout_data->outgenbit4=1;
			break;
			case 4:
				xmc_module_pt[i].ptout_data->outgenbit5=1;
			break;
			case 5:
				xmc_module_pt[i].ptout_data->outgenbit6=1;
			break;
			case 6:
				xmc_module_pt[i].ptout_data->outgenbit7=1;
			break;
			case 7:
				xmc_module_pt[i].ptout_data->outgenbit8=1;
			break;
		}
	}

}



// main task
void demo_run(void *arg)
{
	unsigned long led_loop_cnt=0;
	unsigned long ready_cnt=0;
	int LED_idx=0;
	int LED_Step=1;
	
	struct sched_param param;
	param.sched_priority = 90; //set maximum priority
	printf("Main thread using realtime, priority: %d\n",param.sched_priority);
	if(sched_setscheduler(0, SCHED_FIFO, &param)==-1){
		 perror("sched_setscheduler failed1");
		 exit(-1);
	}
	
	struct timespec   ts;
    struct timeval    tp;
	long rc;
    long ht;

	int64 cycletime;

	if (ecat_init()==FALSE)
	{
		run =0;
		printf("fail\n");
		return;	//all initialization stuffs here
	}

	usleep(1e3);

   rc =  gettimeofday(&tp, NULL);

    /* Convert from timeval to timespec */
   ts.tv_sec  = tp.tv_sec;
   ht = (tp.tv_usec / 1000) + 1; /* round to nearest ms */
   ts.tv_nsec = ht * 1000000;
   
   cycletime = cycle_ns; /* cycletime in ns */

	while (run)
	{

		add_timespec(&ts, cycletime);
		/* wait untill next shot */
		rc = pthread_cond_timedwait(&cond, &mutex, &ts);
		/* do the stuff */

		rc =  gettimeofday(&tp, NULL);
		previous=tp.tv_usec;		//before send-receive ethercat frame
		ec_send_processdata();
		wkc = ec_receive_processdata(EC_TIMEOUTRET);
		if (wkc<3*(NUMOFXMC)) recv_fail_cnt++;
		rc =  gettimeofday(&tp, NULL);
		now =  tp.tv_usec;			//after send-receive ethercat frame

	   ethercat_time = (long) (now - previous);
	   if (sys_ready)
		   if (worst_time<ethercat_time) worst_time=ethercat_time;

		ready_cnt++;
		if (ready_cnt>=1000)
		{
			ready_cnt=6000;
			sys_ready=1;
		}

		if (sys_ready)
		{
			if (++led_loop_cnt>=100)
			{
				led_loop_cnt=0;
				LED_update(LED_idx);
				LED_idx+=LED_Step;
				if (LED_idx>=7) LED_Step=-1;
				if (LED_idx<1) LED_Step=1;
			}
			gt+=period;
		}
		

	}//while (run)

	//
	usleep(2e5);

	printf("End simple test, close socket\n");

	 printf("Request safe operational state for all slaves\n");
	 ec_slave[0].state = EC_STATE_SAFE_OP;
	 /* request SAFE_OP state for all slaves */
	 ec_writestate(0);
	 /* wait for all slaves to reach state */
	 ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE);
	 ec_slave[0].state = EC_STATE_PRE_OP;
	 /* request SAFE_OP state for all slaves */
	 ec_writestate(0);
	 /* wait for all slaves to reach state */
	 ec_statecheck(0, EC_STATE_PRE_OP,  EC_TIMEOUTSTATE);

	 ec_close();

}

void print_run(void *arg)
{
	//RTIME now, previous=0;
	int i;
	unsigned long itime=0;
	unsigned long stick=0;
	struct timespec t;
	struct sched_param param;
	int32_t print_interval=100000000;	//100ms

	param.sched_priority = 40; //set maximum priority
	printf("Printing thread using realtime, priority: %d\n",param.sched_priority);
	if(sched_setscheduler(0, SCHED_FIFO, &param)==-1){
		 perror("sched_setscheduler failed1");
		 exit(-1);
	}
	
	 /* get current time */
	clock_gettime(0,&t);
	/* start after one second */
	t.tv_sec++;

	while (run)
	{
	   /* wait untill next shot */
	   clock_nanosleep(0, TIMER_ABSTIME, &t, NULL);
	   /* do the stuff */
	   
		if (inOP==TRUE)
		{
			
			if (!sys_ready)
			{
				if (stick==0)
					printf("waiting for system ready...\n");
				if (stick%10==0)
					printf("%lu\n", stick/10);
				stick++;
			}
			else
			{
				itime++;
				printf("Time=%06ld.%01ld, \e[32;1m fail=%ld\e[0m, ecat_T=%ld, maxT=%ld\n", itime/10, itime%10, recv_fail_cnt,  ethercat_time, worst_time);
				for(i=0; i<NUMOFXMC; ++i)
					printf("XMC#%i: Btn#1-2: %x\t%x\n", i+1, xmc_module_pt[i].ptin_data->ingenbit1, xmc_module_pt[i].ptin_data->ingenbit2);
				printf("\n");

			}

		}
	      /* calculate next shot */
	      t.tv_nsec+=print_interval;
	      tsnorm(&t);
	} //while (run)
}


void catch_signal(int sig)
{
	run=0;
	usleep(1e6);
	//exit(1);
}

int main(int argc, char *argv[])
{
	struct sched_param    param;
    int                   policy = SCHED_OTHER;

	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);

    printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");

	cycle_ns=1000000; // nanosecond
	period=((double) cycle_ns)/((double) NSEC_PER_SEC);	//period in second unit

	printf("use default adapter %s\n", ecat_ifname);

	int iret1;
	iret1 = pthread_create( &gpio_thread1, NULL, (void *) &demo_run, NULL);
	
	memset(&param, 0, sizeof(param));
	param.sched_priority = 40;
	iret1 = pthread_setschedparam(thread1, policy, &param);
	iret1 = pthread_create( &gpio_thread2, NULL, (void *) &print_run, NULL);

   	while (run)
	{
		usleep(10000);
	}


   printf("End program\n");
   return (0);
}
