#ifndef _STRUCT_H_
#define _STRUCT_H_
/***************************************************************/
#include <ntddk.h>
#include <ntimage.h>// This is the include file that describes all image structures.
/***************************************************************/
typedef BOOLEAN BOOL;
typedef ULONG DWORD;
typedef DWORD *PDWORD;
typedef USHORT WORD;
typedef UCHAR BYTE;
typedef BYTE *PBYTE;
typedef unsigned int UINT;
/***************************************************************/
typedef struct _AUX_ACCESS_DATA {
	PPRIVILEGE_SET PrivilegesUsed;
	GENERIC_MAPPING GenericMapping;
	ACCESS_MASK AccessesToAudit;
	ACCESS_MASK MaximumAuditMask;
	ULONG Unknown[41];
} AUX_ACCESS_DATA, *PAUX_ACCESS_DATA;

//////////////////////////////////////////////////////////////////////////
typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY InLoadOrderLinks;
	LIST_ENTRY InMemoryOrderLinks;
	LIST_ENTRY InInitializationOrderLinks;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY HashLinks;
		struct {
			PVOID SectionPointer;
			ULONG CheckSum;
		};
	};
	union {
		struct {
			ULONG TimeDateStamp;
		};
		struct {
			PVOID LoadedImports;
		};
	};
	PVOID EntryPointActivationContext;

	PVOID PatchInformation;

} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;
/***************************************************************/
typedef struct _SERVICE_DESCRIPTOR_TABLE {
	/*
	* Table containing cServices elements of pointers to service handler
	* functions, indexed by service ID.
	*/
	PULONG   ServiceTable;
	/*
	* Table that counts how many times each service is used. This table
	* is only updated in checked builds.
	*/
	PULONG  CounterTable;
	/*
	* Number of services contained in this table.
	*/
	ULONG   TableSize;
	/*
	* Table containing the number of bytes of parameters the handler
	* function takes.
	*/
	PUCHAR  ArgumentTable;
} SERVICE_DESCRIPTOR_TABLE, *PSERVICE_DESCRIPTOR_TABLE;
//声明系统描述表
extern PSERVICE_DESCRIPTOR_TABLE    KeServiceDescriptorTable;
//////////////////////////////////////////////////////////////////////////
typedef struct _OBJECT_TYPE_INITIALIZER {
	USHORT Length;
	BOOLEAN UseDefaultObject;
	BOOLEAN CaseInsensitive;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccessMask;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	BOOLEAN MaintainTypeList;
	POOL_TYPE PoolType;
	ULONG DefaultPagedPoolCharge;
	ULONG DefaultNonPagedPoolCharge;
	PVOID DumpProcedure;
	PVOID OpenProcedure;
	PVOID CloseProcedure;
	PVOID DeleteProcedure;
	PVOID ParseProcedure;
	PVOID SecurityProcedure;
	PVOID QueryNameProcedure;
	PVOID OkayToCloseProcedure;
} OBJECT_TYPE_INITIALIZER, *POBJECT_TYPE_INITIALIZER;
//////////////////////////////////////////////////////////////////////////
//
//
// Define the debug object thats used to attatch to processes that are being debugged.
//
#define DEBUG_OBJECT_DELETE_PENDING (0x1) // Debug object is delete pending.
#define DEBUG_OBJECT_KILL_ON_CLOSE  (0x2) // Kill all debugged processes on close

//XP下的结构
typedef struct _DEBUG_OBJECT {
	//
	// Event thats set when the EventList is populated.
	//
	KEVENT EventsPresent;
	//
	// Mutex to protect the structure
	//
	FAST_MUTEX Mutex;
	//
	// Queue of events waiting for debugger intervention
	//
	LIST_ENTRY EventList;
	//
	// Flags for the object
	//
	ULONG Flags;
} DEBUG_OBJECT, *PDEBUG_OBJECT;




#endif