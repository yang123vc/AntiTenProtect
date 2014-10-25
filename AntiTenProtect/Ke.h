#pragma once
#include "struct.h"
#include "CommonFunc.h"
#include "FuncAddress.h"
extern BYTE* ReloadNtosImageBase;
extern ULONG SystemKernelModuleBase;

typedef struct _KAPC_STATE {
	LIST_ENTRY ApcListHead[MaximumMode];
	PRKPROCESS Process;//struct _KPROCESS *Process¾ÍÊÇ²»ÅöEPROCESS
	BOOLEAN KernelApcInProgress;
	BOOLEAN KernelApcPending;
	BOOLEAN UserApcPending;
} KAPC_STATE, *PKAPC_STATE, *PRKAPC_STATE;


typedef VOID (__stdcall *KESTACKATTACHPROCESS)(
	__inout PRKPROCESS Process,
	__out PRKAPC_STATE ApcState
	);
typedef VOID (__stdcall *KEUNSTACKATTACHPROCESS)(
	__in PRKAPC_STATE ApcState
	);
VOID
	__stdcall
	codevKeStackAttachProcess (
	__inout PRKPROCESS Process,
	__out PRKAPC_STATE ApcState
	);
VOID
	__stdcall
	codevKeUnstackDetachProcess (
	__in PRKAPC_STATE ApcState
	);
#define KeGetPreviousMode() *(PCHAR)((ULONG)KeGetCurrentThread + 0x140)