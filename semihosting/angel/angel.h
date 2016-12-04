#ifndef ANGEL_H
#define ANGEL_H

enum ANGEL_SWI {
	SYS_OPEN	= 0x01,
	SYS_CLOSE,
	SYS_WRITEC,
	SYS_WRITE0,
	SYS_WRITE,
	SYS_READ,
	SYS_READC,
	SYS_ISERROR,
	SYS_ISTTY,
	SYS_SEEK,
	SYS_FLEN	= 0x0C,
	SYS_TMPNAM,
	SYS_REMOVE,
	SYS_RENAME,
	SYS_CLOCK,
	SYS_TIME,
	SYS_SYSTEM,
	SYS_ERRNO,
	SYS_GET_CMDLINE	= 0x15,
	SYS_HEAPINFO,
	angel_SWIreason_EnterSVC,
	angel_SWIreason_ReportException,
	SYS_ELAPSED	= 0x30,
	SYS_TICKFREQ,
};

enum ANGEL_SWI_REPORT {
	ADP_Stopped_BranchThroughZer0	= 0x20000,
	ADP_Stopped_UndefinedInstr	= 0x20001,
	ADP_Stopped_SoftwareInterrupt	= 0x20002,
	ADP_Stopped_PrefetchAbort	= 0x20003,
	ADP_Stopped_DataAbort		= 0x20004,
	ADP_Stopped_AddressException	= 0x20005,
	ADP_Stopped_IRQ 		= 0x20006,
	ADP_Stopped_FIQ			= 0x20007,
	ADP_Stopped_BreakPoint		= 0x20020,
	ADP_Stopped_WatchPoint		= 0x20021,
	ADP_Stopped_StepComplete	= 0x20022,
	ADP_Stopped_RunTimeErrorUnknown	= 0x20023,
	ADP_Stopped_InternalError	= 0x20024,
	ADP_Stopped_UserInterruption	= 0x20025,
	ADP_Stopped_ApplicationExit	= 0x20026,
	ADP_Stopped_StackOverflow	= 0x20027,
	ADP_Stopped_DivisionByZero	= 0x20028,
	ADP_Stopped_OSSpecific		= 0x20029,
};

void ANGEL_CALL(enum ANGEL_SWI type, void *param);

#endif	// ANGEL_H

