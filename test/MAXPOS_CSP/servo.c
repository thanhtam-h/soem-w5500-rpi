/**
 * SOEM EtherCAT CiA402 example
 * Author: Ho Tam - thanhtam.h[at]gmail.com 
 *
 * Description: implementation of CiA402 state machine for Servo On 
 * Input: current status word read from servo drive
 * Output: next control word or command will be sent to servo drive in next cycle
 * Return value: drive has been in Operation or not
 */

#include "servo_def.h"

int ServoOn_GetCtrlWrd(uint16_t StatusWord, uint16_t *ControlWord)
{
	int  _enable=0;
	if (bit_is_clear(StatusWord, STATUSWORD_OPERATION_ENABLE_BIT)) //Not ENABLED yet
	{
		if (bit_is_clear(StatusWord, STATUSWORD_SWITCHED_ON_BIT)) //Not SWITCHED ON yet
		{
			if (bit_is_clear(StatusWord, STATUSWORD_READY_TO_SWITCH_ON_BIT)) //Not READY to SWITCH ON yet
			{
				if (bit_is_set(StatusWord, STATUSWORD_FAULT_BIT)) //FAULT exist
				{
					(*ControlWord)=0x80;	//FAULT RESET command
				}
				else //NO FAULT
				{
					(*ControlWord)=0x06;	//SHUTDOWN command (transition#2)
				}
			}
			else //READY to SWITCH ON
			{
				(*ControlWord)=0x07;	//SWITCH ON command (transition#3)
			}
		}
		else //has been SWITCHED ON
		{
			(*ControlWord)=0x0F;	//ENABLE OPETATION command (transition#4)
			_enable=1;
		}
	}
	else //has been ENABLED
	{
		(*ControlWord)=0x0F;	//maintain OPETATION state
		_enable=1;
	}
	return _enable;;
}
