#pragma once

#define PACKET_HEADER "EXTRBEXT"

enum PacketClassEnumerator : unsigned short
{
	HEADER = 0,
	COMMAND,
	STATUS,
	CONFIGURATION
	//...
};

enum GlobalStatusType : unsigned short
{
	STATUS_IDLE = 0,		//System is on, idling
	STATUS_PREPARING,		//System is preparing for process start
	STATUS_READY,			//System is ready to start process
	STATUS_PROCESS_RUNNING,	//Process has started
	STATUS_ERROR
};

enum GlobalCommandEnumerator : unsigned short
{
	GET_STATUS = 0,		//Get system status
	GET_PROCESS_DATA,	//Get full settings of system 
	SET_CONFIGURATION
	//...
};

struct CommandInStructure {
	GlobalCommandEnumerator command;
};
struct ConfigurationInStructure {
	GlobalCommandEnumerator command;
};
struct StatusOutStructure {
	GlobalStatusType	status;
	unsigned short		statusCode;
};
struct ProcessOutStructure {
	StatusOutStructure status;
	//Extruder specific data
	//...
};

typedef CompleteDataPacket<StatusOutStructure, PacketClassEnumerator> StatusDataPacket;
typedef CompleteDataPacket<ProcessOutStructure, PacketClassEnumerator> ProcessDataPacket;

