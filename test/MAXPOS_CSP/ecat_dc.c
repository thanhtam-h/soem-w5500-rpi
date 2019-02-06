/* 
 * SOEM EtherCAT exmaple
 * Ported to raspberry pi by Ho Tam - thanhtam.h[at]gmail.com 
 */
 
#include <stdlib.h>
#include <signal.h>
#include "ecat_dc.h"

//convert float, double to fixed point
void double_to_fixed(double f_input, int32_t *pValue, int32_t *pBase)
{
	if (f_input<1.0)
	{
		(*pBase)=15;
		(*pValue)=(int32_t) (32768.0*f_input);
	}
	else if (f_input<2.0)
	{
		(*pBase)=14;
		(*pValue)=(int32_t) (16384.0*f_input);
	}
	else if (f_input<4.0)
	{
		(*pBase)=13;
		(*pValue)=(int32_t) (8192.0*f_input);
	}
	else if (f_input<8.0)
	{
		(*pBase)=12;
		(*pValue)=(int32_t) (4096.0*f_input);
	}
	else if (f_input<16.0)
	{
		(*pBase)=11;
		(*pValue)=(int32_t) (2048.0*f_input);
	}
	else if (f_input<32.0)
	{
		(*pBase)=10;
		(*pValue)=(int32_t) (1024.0*f_input);
	}
	else if (f_input<64.0)
	{
		(*pBase)=9;
		(*pValue)=(int32_t) (512.0*f_input);
	}
	else if (f_input<128.0)
	{
		(*pBase)=8;
		(*pValue)=(int32_t) (256.0*f_input);
	}
	else if (f_input<256.0)
	{
		(*pBase)=7;
		(*pValue)=(int32_t) (128.0*f_input);
	}
	else if (f_input<512.0)
	{
		(*pBase)=6;
		(*pValue)=(int32_t) (64.0*f_input);
	}
	else if (f_input<1024.0)
	{
		(*pBase)=5;
		(*pValue)=(int32_t) (32.0*f_input);
	}
	else if (f_input<2048.0)
	{
		(*pBase)=4;
		(*pValue)=(int32_t) (16.0*f_input);
	}
	else if (f_input<4096.0)
	{
		(*pBase)=3;
		(*pValue)=(int32_t) (8.0*f_input);
	}
	else if (f_input<81928.0)
	{
		(*pBase)=2;
		(*pValue)=(int32_t) (4.0*f_input);
	}
	else if (f_input<16384.0)
	{
		(*pBase)=1;
		(*pValue)=(int32_t) (2.0*f_input);
	}
	else if (f_input<32768.0)
	{
		(*pBase)=0;
		(*pValue)=(int32_t) (1.0*f_input);
	}
}




#define PI_SAT_VAL	50000

int32_t  _pPart=0, _iPart=0;
int32_t   _sync_err=0, _sync_pre_err=0;

//double Kp=0.1, Ki=0.0005;
double Kp=0.1, Ki=0.001;

//new style, PI compensation
long long dc_pi_sync(long long  reftime, long long  cycletime, int32_t shift_time)
{
	long long  adj_time=0;
	int32_t iKp=0, iKp_Base, iKi=0, iKi_Base;
	double_to_fixed(Kp, &iKp, &iKp_Base);
	double_to_fixed(Ki, &iKi, &iKi_Base);
	
	_sync_err = (reftime - shift_time) % cycletime;
	if(_sync_err> (cycletime /2)) { _sync_err= _sync_err - cycletime; }

	_pPart=_sync_err*iKp;
	_iPart+=(_sync_err+_sync_pre_err)*iKi;
	_sync_pre_err=_sync_err;

	adj_time = -(_pPart>>iKp_Base) - (_iPart>>iKi_Base);

//	if (adj_time>PI_SAT_VAL) adj_time=PI_SAT_VAL;
//	if (adj_time<-PI_SAT_VAL) adj_time=-PI_SAT_VAL;

	return adj_time;
	
}

