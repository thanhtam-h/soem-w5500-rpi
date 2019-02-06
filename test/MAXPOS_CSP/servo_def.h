/**
 * SOEM EtherCAT CiA402 example
 *
 * Author: Ho Tam - thanhtam.h[at]gmail.com 
 */

#ifndef _SERVO_DEF_
#define _SERVO_DEF_

#include <stdint.h>

#define _BV(bit) 				(1 << (bit)) 
#define bit_is_set(val, bit) 	(val & _BV(bit))
#define bit_is_clear(val, bit) 	(!(val & _BV(bit)))
#define sbi(val,bit) 			((val) |= _BV(bit)) 
#define cbi(val,bit) 			((val) &= ~_BV(bit))

#define STATUSWORD_READY_TO_SWITCH_ON_BIT 		0
#define STATUSWORD_SWITCHED_ON_BIT 				1
#define STATUSWORD_OPERATION_ENABLE_BIT 		2
#define STATUSWORD_FAULT_BIT 					3
#define STATUSWORD_VOLTAGE_ENABLE_BIT 			4
#define STATUSWORD_QUICK_STOP_BIT 				5
#define STATUSWORD_SWITCH_ON_DISABLE_BIT 		6
#define STATUSWORD_NO_USED_WARNING_BIT 			7
#define STATUSWORD_ELMO_NOT_USED_BIT 			8
#define STATUSWORD_REMOTE_BIT 					9
#define STATUSWORD_TARGET_REACHED_BIT 			10
#define STATUSWORD_INTERNAL_LIMIT_ACTIVE_BIT	11

/**
 * Control CoE FSM(Finite State Machine)
 */
#define EC_CONTROL_COMMAND					(0x02)
#define CONTROL_COMMAND_DIABLE_VOLTAGE		(0X0000)
#define CONTROL_COMMAND_QUICK_STOP			(0x0002)
#define CONTROL_COMMAND_SHUTDOWN			(0x0006)
#define CONTROL_COMMAND_SWITCH_ON			(0x0007)
#define CONTROL_COMMAND_ENABLE_OPERATION	(0x000f)
#define CONTROL_COMMAND_SWITCH_ON_ENABLE_OPERATION	(0x000f)
#define CONTROL_COMMAND_DISABLE_OPERATION	(0x0007)
#define CONTROL_COMMAND_FAULT_RESET			(0x0080)

/**
 * Operation mode
 */
#define EC_OP_MODE							(0x03)

#define OP_MODE_NO_MODE						(0x00)
#define OP_MODE_PROFILE_POSITION			(0X01)
#define OP_MODE_VELOCITY					(0X02)
#define OP_MODE_PROFILE_VELOCITY			(0X03)
#define OP_MODE_TORQUE_PROFILE				(0X04)
#define OP_MODE_HOMING						(0X06)
#define OP_MODE_INTERPOLATED_POSITION		(0X07)
#define OP_MODE_CYCLIC_SYNC_POSITION		(0X08)
#define OP_MODE_CYCLIC_SYNC_VELOCITY		(0X09)
#define OP_MODE_CYCLIC_SYNC_TORQUE			(0X0a)

#define SERVO_TIMEOUT						(50000)

typedef int32_t  INT32;
typedef uint32_t UINT32;
typedef int16_t  INT16;
typedef uint16_t UINT16;
typedef uint8_t UINT8;

#ifndef PI
#define PI	(3.14159265359)
#define PI2	(6.28318530718)
#define RAG2DEG		57.2957795131
#define DEG2RAD		0.01745329252
#endif

#ifdef __cplusplus
extern "C"{
#endif
int ServoOn_GetCtrlWrd(uint16_t StatusWord, uint16_t *ControlWord);
#ifdef __cplusplus
}
#endif

#endif //_SERVO_DEF_

