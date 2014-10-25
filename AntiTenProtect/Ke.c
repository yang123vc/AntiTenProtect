#include "Ke.h"

VOID
__stdcall
	codevKeStackAttachProcess (
	__inout PRKPROCESS Process,
	__out PRKAPC_STATE ApcState
	)
{
	KESTACKATTACHPROCESS pfn_KeStackAttachProcess;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return;
	}
	pfn_KeStackAttachProcess = (KESTACKATTACHPROCESS)(OriginKeStackAttachProcess - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_KeStackAttachProcess(Process,ApcState);
}
//
VOID
__stdcall
	codevKeUnstackDetachProcess (
	__in PRKAPC_STATE ApcState
	)
{
	KEUNSTACKATTACHPROCESS pfn_KeUnstackDetachProcess;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return;
	}
	pfn_KeUnstackDetachProcess = (KEUNSTACKATTACHPROCESS)(OriginKeUnStackAttachProcess - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_KeUnstackDetachProcess(ApcState);
}