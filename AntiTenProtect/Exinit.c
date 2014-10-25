#include "Exinit.h"


int
ExSystemExceptionFilter( VOID )
{
	return( KeGetPreviousMode() != KernelMode ? EXCEPTION_EXECUTE_HANDLER
		: EXCEPTION_CONTINUE_SEARCH
		);
}

VOID FASTCALL codevExAcquireFastMutex (
	__inout PFAST_MUTEX FastMutex
	)
{
	EXACQUIREFASTMUTEX pfn_ExAcquireFastMutex;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return;
	}
	pfn_ExAcquireFastMutex = (EXACQUIREFASTMUTEX)(OriginExAcquireFastMutex - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_ExAcquireFastMutex(FastMutex);
}
//
VOID
	FASTCALL
	codevExReleaseFastMutex (
	__inout PFAST_MUTEX FastMutex
	)
{
	EXRELEASEFASTMUTEX pfn_ExReleaseFastMutex;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return;
	}
	pfn_ExReleaseFastMutex = (EXRELEASEFASTMUTEX)(OriginExReleaseFastMutex - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_ExReleaseFastMutex(FastMutex);
}