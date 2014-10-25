#include "PsProcessThread.h"
POBJECT_TYPE codevPsProcessType= NULL;//自己创建两个对象类型
POBJECT_TYPE codevPsThreadType= NULL;

VOID  codevPspProcessDelete(
	IN PVOID Object)
{
	PSPPROCESSDELETE pfn_PspProcessDelete;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return;
	}
	pfn_PspProcessDelete = (PSPPROCESSDELETE)(OriginPspProcessDelete - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_PspProcessDelete(Object);
}
//
VOID codevPspThreadDelete(
	IN PVOID Object)
{
	PSPTHREADDELETE pfn_PspThreadDelete;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return;
	}
	pfn_PspThreadDelete = (PSPTHREADDELETE)(OriginPspThreadDelete - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_PspThreadDelete(Object);
}
BOOL codevPsInitialize()
{
	OBJECT_TYPE_INITIALIZER ObjectTypeInitializer;
	UNICODE_STRING NameString;
	// Initialize the common fields of the Object Type Prototype record
	//

	RtlZeroMemory (&ObjectTypeInitializer, sizeof (ObjectTypeInitializer));
	ObjectTypeInitializer.Length = sizeof (ObjectTypeInitializer);
	ObjectTypeInitializer.SecurityRequired = TRUE;
	ObjectTypeInitializer.PoolType = NonPagedPool;
	ObjectTypeInitializer.InvalidAttributes = OBJ_PERMANENT |
		OBJ_EXCLUSIVE |
		OBJ_OPENIF;


	//
	// Create Object types for Thread and Process Objects.
	//

	RtlInitUnicodeString (&NameString, L"codevProcess");
	ObjectTypeInitializer.DefaultPagedPoolCharge = PSP_PROCESS_PAGED_CHARGE;
	ObjectTypeInitializer.DefaultNonPagedPoolCharge = PSP_PROCESS_NONPAGED_CHARGE;
	ObjectTypeInitializer.DeleteProcedure = codevPspProcessDelete;
	ObjectTypeInitializer.ValidAccessMask = PROCESS_ALL_ACCESS;
	ObjectTypeInitializer.GenericMapping = PspProcessMapping;

	if (!NT_SUCCESS (codevObCreateObjectType (&NameString,
		&ObjectTypeInitializer,
		(PSECURITY_DESCRIPTOR) NULL,
		&codevPsProcessType))) {
			return FALSE;
	}

	RtlInitUnicodeString (&NameString, L"codevThread");
	ObjectTypeInitializer.DefaultPagedPoolCharge = PSP_THREAD_PAGED_CHARGE;
	ObjectTypeInitializer.DefaultNonPagedPoolCharge = PSP_THREAD_NONPAGED_CHARGE;
	ObjectTypeInitializer.DeleteProcedure = codevPspThreadDelete;
	ObjectTypeInitializer.ValidAccessMask = THREAD_ALL_ACCESS;
	ObjectTypeInitializer.GenericMapping = PspThreadMapping;

	if (!NT_SUCCESS (codevObCreateObjectType (&NameString,
		&ObjectTypeInitializer,
		(PSECURITY_DESCRIPTOR) NULL,
		&codevPsThreadType))) {
			return FALSE;
	}
}
//
PEPROCESS __stdcall codevPsGetNextProcess(
	IN PEPROCESS Process
	)
{
	PSGETNEXTPROCESS pfn_PsGetNextProcess;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return NULL;
	}
	pfn_PsGetNextProcess = (PSGETNEXTPROCESS)(OriginPsGetNextProcess - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_PsGetNextProcess(Process);
}
//
NTSTATUS __stdcall codevPsTerminateProcess(
	PEPROCESS Process,
	NTSTATUS Status
	)
{
	PSTERMINATEPROCESS pfn_PsTerminateProcess;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return STATUS_UNSUCCESSFUL;
	}
	pfn_PsTerminateProcess = (PSTERMINATEPROCESS)(OriginPsTerminateProcess - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_PsTerminateProcess(Process,Status);
}