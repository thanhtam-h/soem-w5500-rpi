/* 
 * SOEM EtherCAT exmaple
 * Ported to raspberry pi by Ho Tam - thanhtam.h[at]gmail.com 
 */
 

#ifndef _PDO_DEF_
#define _PDO_DEF_

#include "servo_def.h"

//MAXPOS PDO mapping

typedef union _mapping_obj{
	uint32_t obj;
	struct {
		uint16_t index;
		uint8_t subindex;
		uint8_t size;
	};
} mapping_obj;

//0x1600 RxPDO
typedef struct PACKED
{
	UINT16	ControlWord;		//0x6040
	INT32	TargetPosition;		//0x607A
	INT32	PositionOffset;		//0x60B0
	INT32	VelocityOffset;		//0x60B1
	INT16	TorqueOffset;		//0x60B2
	UINT8	ModeOfOperation;	//0x6060
	UINT32	PhysicalOutput;		//0x60FE
	UINT16	TouchProbeFunction;	//0x60B8
}MAXPOS_DRIVE_RxPDO_t;

//0x1A00 TxPDO
typedef struct PACKED
{
	UINT16	StatusWord;					//0x6041
	INT32	PositionActualValue;		//0x6064
	INT32	VelocityActualValue;		//0x606C
	INT16	TorqueActualValue;			//0x6077
	UINT8	ModeOfOperationDisplay;		//0x6061
	UINT32	DigitalInput;				//0x60FD
	UINT16	TouchProbeStatus;			//0x60B9
	INT32	TouchProbePosition1;		//0x60BA
	INT32	TouchProbePosition2;		//0x60BB
}MAXPOS_DRIVE_TxPDO_t;


typedef struct _MAXPOS_ServoDrive
{
	MAXPOS_DRIVE_RxPDO_t 	OutParam;
	MAXPOS_DRIVE_TxPDO_t 	InParam;
} MAXPOS_ServoDrive_t;

typedef struct _MAXPOS_Drive_pt
{
	MAXPOS_DRIVE_RxPDO_t 	*ptOutParam;
	MAXPOS_DRIVE_TxPDO_t 	*ptInParam;
} MAXPOS_Drive_pt;


#endif //_PDO_DEF_

