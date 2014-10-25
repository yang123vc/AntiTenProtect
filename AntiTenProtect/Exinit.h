#pragma once
#include "struct.h"
#include "Ke.h"
#include "CommonFunc.h"
#include "FuncAddress.h"

extern BYTE* ReloadNtosImageBase;
extern ULONG SystemKernelModuleBase;

typedef VOID (FASTCALL *EXACQUIREFASTMUTEX) (
	__inout PFAST_MUTEX FastMutex
	);
typedef VOID (FASTCALL *EXRELEASEFASTMUTEX)(
	__inout PFAST_MUTEX FastMutex
	);


VOID FASTCALL codevExAcquireFastMutex (
	__inout PFAST_MUTEX FastMutex
	);
VOID
	FASTCALL
	codevExReleaseFastMutex (
	__inout PFAST_MUTEX FastMutex
	);
int ExSystemExceptionFilter( VOID );
