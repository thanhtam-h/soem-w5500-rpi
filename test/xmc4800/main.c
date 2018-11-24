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
 * Port for Raspberry pi by Ho Tam - thanhtam.h[at]gmail.com
 */

 #include <stdlib.h>
#include <signal.h>

#include <sys/mman.h>

#include <alchemy/task.h>
#include <alchemy/timer.h>
#include <alchemy/sem.h>
//#include <rtdm/testing.h>
#include <boilerplate/trace.h>
#include <xenomai/init.h>

#include "ethercattype.h"
#include "nicdrv.h"
#include "ethercatbase.h"
#include "ethercatmain.h"
#include "ethercatdc.h"
#include "ethercatcoe.h"
#include "ethercatfoe.h"
#include "ethercatconfig.h"
#include "ethercatprint.h"
#include "wiznet_drv.h"

#include "pdo_def.h"

#define NSEC_PER_SEC 			1000000000
#define EC_TIMEOUTMON 500

#define x_USE_DC
#define x_WRITE_MODEOP_SDO

#define NUMOFXMC	1

slave_XMC4800_pt	xmc_module_pt[NUMOFXMC];


unsigned int cycle_ns = 1000000; /* 1 ms */

int print_enable=0;

char IOmap[4096];
//char IOmap[1024];
pthread_t thread1;
int expectedWKC;
boolean needlf;
volatile int wkc;
boolean inOP;
uint8 currentgroup = 0;

RT_TASK demo_task;
RT_TASK print_task;
RT_TASK eccheck_task;
//RT_MUTEX mutex_desc;

RTIME now, previous;
long ethercat_time=0, worst_time=0;
char ecat_ifname[32]="wiznet";
int run=1;
int LED_Byte_idx=8, BTN_Byte_idx=8; //HoTam, for XMC4800 kit
uint8 input_bnt1=0;
uint8 input_bnt2=0;
int sys_ready=0;

int recv_fail_cnt=0;

double gt=0;
/// TO DO: This is user-code.
double sine_amp=5000, f=1, period;

boolean ecat_init(void)
{
	int i, oloop, iloop;
    needlf = FALSE;
    inOP = FALSE;

    rt_printf("Starting simple test\n");

    if (ec_init(ecat_ifname))
    {
      rt_printf("ec_init on %s succeeded.\n", ecat_ifname); //ifname
      /* find and auto-config slaves */

		if ( ec_config_init(FALSE) > 0 )
		{
			 rt_printf("%d slaves found and configured.\n",ec_slavecount);

			 ec_config_map(&IOmap);

#ifdef _USE_DC
			 ec_configdc();
#endif
			 rt_printf("Slaves mapped, state to SAFE_OP.\n");
			 /* wait for all slaves to reach SAFE_OP state */
			 ec_statecheck(0, EC_STATE_SAFE_OP,  EC_TIMEOUTSTATE * 4);
#ifdef _USE_DC
			 //NEW, FOR DC----
			 /* configure DC options for every DC capable slave found in the list */
			 rt_printf("DC capable : %d\n",ec_configdc());
			 //---------------
#endif
			 oloop = ec_slave[0].Obytes;
			 if ((oloop == 0) && (ec_slave[0].Obits > 0)) oloop = 1;
			 //if (oloop > 8) oloop = 8;
			 iloop = ec_slave[0].Ibytes;
			 if ((iloop == 0) && (ec_slave[0].Ibits > 0)) iloop = 1;
			 //if (iloop > 8) iloop = 8;

			 rt_printf("segments : %d : %d %d %d %d\n",ec_group[0].nsegments ,ec_group[0].IOsegment[0],ec_group[0].IOsegment[1],ec_group[0].IOsegment[2],ec_group[0].IOsegment[3]);

			 rt_printf("Request operational state for all slaves\n");
			 expectedWKC = (ec_group[0].outputsWKC * 2) + ec_group[0].inputsWKC;
			 rt_printf("Calculated workcounter %d\n", expectedWKC);
			 ec_slave[0].state = EC_STATE_OPERATIONAL;
			 /* send one valid process data to make outputs in slaves happy*/
			 ec_send_processdata();
			 ec_receive_processdata(EC_TIMEOUTRET);
			 /* request OP state for all slaves */

			 ec_writestate(0);
			 ec_statecheck(0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE); //wait for OP

			if (ec_slave[0].state == EC_STATE_OPERATIONAL )
			{
				rt_printf("Operational state reached for all slaves.\n");

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
                rt_printf("Not all slaves reached operational state.\n");
                ec_readstate();
                for(i = 1; i<=ec_slavecount ; i++)
                {
                    if(ec_slave[i].state != EC_STATE_OPERATIONAL)
                    {
                        rt_printf("Slave %d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
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
        rt_printf("No socket connection on %s\nExcecute as root\n", ecat_ifname);
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
	RTIME now, previous;
	unsigned long led_loop_cnt=0;
	unsigned long ready_cnt=0;
	int LED_idx=0;
	int LED_Step=1;

	wiznet_hw_config(8, 1, 1000000); //select SPI-W5500 parameters, before ec_init
	if (ecat_init()==FALSE)
	{
		run =0;
		printf("fail\n");
		return;	//all initialization stuffs here
	}

	rt_task_sleep(1e6);
	rt_task_set_periodic(NULL, TM_NOW, cycle_ns);

	while (run)
	{
	   rt_task_wait_period(NULL); 	//wait for next cycle

	   previous = rt_timer_read();

	   ec_send_processdata();
	   wkc = ec_receive_processdata(EC_TIMEOUTRET);
	   if (wkc<3*(NUMOFXMC)) recv_fail_cnt++;

	   now = rt_timer_read();
	   ethercat_time = (long) now - previous;
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

	}

	rt_printf("End simple test, close socket\n");
	/* stop SOEM, close socket */
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
	int i;
	unsigned long itime=0;
	long stick=0;

	rt_task_set_periodic(NULL, TM_NOW, 1e8);

	while (run)
	{
		rt_task_wait_period(NULL); 	//wait for next cycle
		if (inOP==TRUE)
		{
			if (!sys_ready)
			{
				if (stick==0)
					rt_printf("waiting for system ready...\n");
				if (stick%10==0)
					rt_printf("%i\n", stick/10);
				stick++;
			}
			else
			{
				itime++;
				rt_printf("Time=%06d.%01d, \e[32;1m fail=%ld\e[0m, ecat_T=%ld, maxT=%ld\n", itime/10, itime%10, recv_fail_cnt,  ethercat_time/1000, worst_time/1000);
				for(i=0; i<NUMOFXMC; ++i)
					rt_printf("XMC#%i: Btn#1-2: %x\t%x\n", i+1, xmc_module_pt[i].ptin_data->ingenbit1, xmc_module_pt[i].ptin_data->ingenbit2);
				rt_printf("\n");

			}

		}
	}
}


void catch_signal(int sig)
{
	run=0;
	usleep(1e5);
	rt_task_delete(&demo_task);
	rt_task_delete(&print_task);
	exit(1);
}

int main(int argc, char *argv[])
{
	signal(SIGTERM, catch_signal);
	signal(SIGINT, catch_signal);

    printf("SOEM (Simple Open EtherCAT Master)\nSimple test\n");

	mlockall(MCL_CURRENT | MCL_FUTURE);

	cycle_ns=1000000; // nanosecond
	period=((double) cycle_ns)/((double) NSEC_PER_SEC);	//period in second unit

	printf("use default adapter %s\n", ecat_ifname);


  	rt_task_create(&demo_task, "SOEM_demo_task", 0, 90, 0 );
	rt_task_create(&print_task, "ec_printing", 0, 50, 0 );

	rt_task_start(&demo_task, &demo_run, NULL);
	rt_task_start(&print_task, &print_run, NULL);

   	while (run)
	{
		usleep(10000);
	}


   printf("End program\n");
   return (0);
}
