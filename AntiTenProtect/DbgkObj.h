#pragma once
#include "struct.h"
#include "Ke.h"
#include "Exinit.h"
#include "ObMgr.h"
#include "FuncAddress.h"
#include "PsProcessThread.h"
extern BYTE* ReloadNtosImageBase;
extern ULONG SystemKernelModuleBase;

extern BOOL bWin7;

extern POBJECT_TYPE codevPsProcessType;//自己创建两个对象类型
extern POBJECT_TYPE codevPsThreadType;

#define  DebugPortOffsetWin7 (0xec)
#define  DebugPortOffsetXp (0xbc)
#define  RundownProtectOffsetWin7 (0x80)
#define  RundownProtectOffsetXp		(0x80)
#define  PebOffsetWin7				(0x1b0)
#define  PebOffsetXp				(0x1b0)

#define DEBUG_KILL_ON_CLOSE  (0x1) // Kill all debuggees on last handle close

#define DEBUG_READ_EVENT        (0x0001)
#define DEBUG_PROCESS_ASSIGN    (0x0002)
#define DEBUG_SET_INFORMATION   (0x0004)
#define DEBUG_QUERY_INFORMATION (0x0008)
#define DEBUG_ALL_ACCESS     (STANDARD_RIGHTS_REQUIRED|SYNCHRONIZE|DEBUG_READ_EVENT|DEBUG_PROCESS_ASSIGN|\
	DEBUG_SET_INFORMATION|DEBUG_QUERY_INFORMATION)

//
#define LPC_CLIENT_ID CLIENT_ID64
#define LPC_SIZE_T ULONGLONG
#define LPC_PVOID ULONGLONG
#define LPC_HANDLE ULONGLONG

#define LPC_CLIENT_ID CLIENT_ID
#define LPC_SIZE_T SIZE_T
#define LPC_PVOID PVOID
#define LPC_HANDLE HANDLE

typedef struct _PORT_MESSAGE {
	union {
		struct {
			CSHORT DataLength;
			CSHORT TotalLength;
		} s1;
		ULONG Length;
	} u1;
	union {
		struct {
			CSHORT Type;
			CSHORT DataInfoOffset;
		} s2;
		ULONG ZeroInit;
	} u2;
	union {
		LPC_CLIENT_ID ClientId;
		double DoNotUseThisField;       // Force quadword alignment
	};
	ULONG MessageId;
	union {
		LPC_SIZE_T ClientViewSize;          // Only valid on LPC_CONNECTION_REQUEST message
		ULONG CallbackId;                   // Only valid on LPC_REQUEST message
	};
	//  UCHAR Data[];
} PORT_MESSAGE, *PPORT_MESSAGE;
//

typedef enum _DBGKM_APINUMBER {
	DbgKmExceptionApi,
	DbgKmCreateThreadApi,
	DbgKmCreateProcessApi,
	DbgKmExitThreadApi,
	DbgKmExitProcessApi,
	DbgKmLoadDllApi,
	DbgKmUnloadDllApi,
	DbgKmMaxApiNumber
} DBGKM_APINUMBER;
//
typedef struct _DBGKM_EXCEPTION {
	EXCEPTION_RECORD ExceptionRecord;
	ULONG FirstChance;
} DBGKM_EXCEPTION, *PDBGKM_EXCEPTION;
//
typedef struct _DBGKM_CREATE_THREAD {
	ULONG SubSystemKey;
	PVOID StartAddress;
} DBGKM_CREATE_THREAD, *PDBGKM_CREATE_THREAD;
//
typedef struct _DBGKM_CREATE_PROCESS {
	ULONG SubSystemKey;
	HANDLE FileHandle;
	PVOID BaseOfImage;
	ULONG DebugInfoFileOffset;
	ULONG DebugInfoSize;
	DBGKM_CREATE_THREAD InitialThread;
} DBGKM_CREATE_PROCESS, *PDBGKM_CREATE_PROCESS;
//
typedef struct _DBGKM_EXIT_THREAD {
	NTSTATUS ExitStatus;
} DBGKM_EXIT_THREAD, *PDBGKM_EXIT_THREAD;

typedef struct _DBGKM_EXIT_PROCESS {
	NTSTATUS ExitStatus;
} DBGKM_EXIT_PROCESS, *PDBGKM_EXIT_PROCESS;

typedef struct _DBGKM_LOAD_DLL {
	HANDLE FileHandle;
	PVOID BaseOfDll;
	ULONG DebugInfoFileOffset;
	ULONG DebugInfoSize;
	PVOID NamePointer;
} DBGKM_LOAD_DLL, *PDBGKM_LOAD_DLL;

typedef struct _DBGKM_UNLOAD_DLL {
	PVOID BaseAddress;
} DBGKM_UNLOAD_DLL, *PDBGKM_UNLOAD_DLL;
typedef struct _DBGKM_APIMSG {
	PORT_MESSAGE h;
	DBGKM_APINUMBER ApiNumber;
	NTSTATUS ReturnedStatus;
	union {
		DBGKM_EXCEPTION Exception;
		DBGKM_CREATE_THREAD CreateThread;
		DBGKM_CREATE_PROCESS CreateProcessInfo;
		DBGKM_EXIT_THREAD ExitThread;
		DBGKM_EXIT_PROCESS ExitProcess;
		DBGKM_LOAD_DLL LoadDll;
		DBGKM_UNLOAD_DLL UnloadDll;
	} u;
} DBGKM_APIMSG, *PDBGKM_APIMSG;
typedef struct _DEBUG_EVENT {
	LIST_ENTRY EventList;      // Queued to event object through this
	KEVENT ContinueEvent;
	CLIENT_ID ClientId;
	PEPROCESS Process;         // Waiting process
	PETHREAD Thread;           // Waiting thread
	NTSTATUS Status;           // Status of operation
	ULONG Flags;
	PETHREAD BackoutThread;    // Backout key for faked messages
	DBGKM_APIMSG ApiMsg;       // Message being sent
} DEBUG_EVENT, *PDEBUG_EVENT;
typedef VOID (__stdcall *DBGKPDELETEOBJECT) (
	IN  PVOID   Object
	);
typedef VOID (__stdcall *DBGKPCLOSEOBEJCT) (
	IN PEPROCESS Process,
	IN PVOID Object,
	IN ACCESS_MASK GrantedAccess,
	IN ULONG_PTR ProcessHandleCount,
	IN ULONG_PTR SystemHandleCount
	);
typedef VOID (__stdcall *DBGKPWAKETARGET)(IN PDEBUG_EVENT DebugEvent);