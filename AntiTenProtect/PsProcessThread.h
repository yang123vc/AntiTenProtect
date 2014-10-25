#pragma once
#include "struct.h"
#include "ObMgr.h"
#include "CommonFunc.h"
extern BYTE* ReloadNtosImageBase;
extern ULONG SystemKernelModuleBase;

#define PSP_PROCESS_PAGED_CHARGE    (PAGE_SIZE)
//#define PSP_PROCESS_NONPAGED_CHARGE (sizeof(EPROCESS))
#define PSP_PROCESS_NONPAGED_CHARGE (0x258)//xp系统eprocess大小，这里省事了，囧！

#define PSP_THREAD_PAGED_CHARGE     (0)
//#define PSP_THREAD_NONPAGED_CHARGE  (sizeof(ETHREAD))
#define PSP_THREAD_NONPAGED_CHARGE  (0x255)
//
#define PROCESS_TERMINATE         (0x0001)  // winnt
#define PROCESS_CREATE_THREAD     (0x0002)  // winnt
#define PROCESS_SET_SESSIONID     (0x0004)  // winnt
#define PROCESS_VM_OPERATION      (0x0008)  // winnt
#define PROCESS_VM_READ           (0x0010)  // winnt
#define PROCESS_VM_WRITE          (0x0020)  // winnt
// begin_ntddk begin_wdm begin_ntifs
#define PROCESS_DUP_HANDLE        (0x0040)  // winnt
// end_ntddk end_wdm end_ntifs
#define PROCESS_CREATE_PROCESS    (0x0080)  // winnt
#define PROCESS_SET_QUOTA         (0x0100)  // winnt
#define PROCESS_SET_INFORMATION   (0x0200)  // winnt
#define PROCESS_QUERY_INFORMATION (0x0400)  // winnt
#define PROCESS_SET_PORT          (0x0800)
#define PROCESS_SUSPEND_RESUME    (0x0800)  // winnt
const GENERIC_MAPPING PspProcessMapping = {
	STANDARD_RIGHTS_READ |
	PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
	STANDARD_RIGHTS_WRITE |
	PROCESS_CREATE_PROCESS | PROCESS_CREATE_THREAD |
	PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_DUP_HANDLE |
	PROCESS_TERMINATE | PROCESS_SET_QUOTA |
	PROCESS_SET_INFORMATION | PROCESS_SET_PORT,
	STANDARD_RIGHTS_EXECUTE |
	SYNCHRONIZE,
	PROCESS_ALL_ACCESS
};
//
#define THREAD_TERMINATE               (0x0001)  // winnt
// end_ntddk end_wdm end_ntifs
#define THREAD_SUSPEND_RESUME          (0x0002)  // winnt
#define THREAD_ALERT                   (0x0004)
#define THREAD_GET_CONTEXT             (0x0008)  // winnt
#define THREAD_SET_CONTEXT             (0x0010)  // winnt
// begin_ntddk begin_wdm begin_ntifs
#define THREAD_SET_INFORMATION         (0x0020)  // winnt
// end_ntddk end_wdm end_ntifs
#define THREAD_QUERY_INFORMATION       (0x0040)  // winnt
// begin_winnt
#define THREAD_SET_THREAD_TOKEN        (0x0080)
#define THREAD_IMPERSONATE             (0x0100)
#define THREAD_DIRECT_IMPERSONATION    (0x0200)
const GENERIC_MAPPING PspThreadMapping = {
	STANDARD_RIGHTS_READ |
	THREAD_GET_CONTEXT | THREAD_QUERY_INFORMATION,
	STANDARD_RIGHTS_WRITE |
	THREAD_TERMINATE | THREAD_SUSPEND_RESUME | THREAD_ALERT |
	THREAD_SET_INFORMATION | THREAD_SET_CONTEXT,
	STANDARD_RIGHTS_EXECUTE |
	SYNCHRONIZE,
	THREAD_ALL_ACCESS
};

typedef VOID (__stdcall *PSPPROCESSDELETE)(IN PVOID Object);
typedef VOID (__stdcall *PSPTHREADDELETE)(IN PVOID Object);

typedef PEPROCESS (__stdcall *PSGETNEXTPROCESS)(
	IN PEPROCESS Process
	);
typedef NTSTATUS (__stdcall *PSTERMINATEPROCESS)(
	PEPROCESS Process,
	NTSTATUS Status
	);
PEPROCESS __stdcall codevPsGetNextProcess(
	IN PEPROCESS Process
	);
NTSTATUS __stdcall codevPsTerminateProcess(
	PEPROCESS Process,
	NTSTATUS Status
	);