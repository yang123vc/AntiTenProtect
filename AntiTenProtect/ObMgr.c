#include "ObMgr.h"
//patch 以下这个函数吧，ObReferenceObjectByHandle
BOOL ObMgrInit()
{

}

NTSTATUS __stdcall codevObCreateObjectType(
	__in PUNICODE_STRING TypeName,
	__in POBJECT_TYPE_INITIALIZER ObjectTypeInitializer,
	__in_opt PSECURITY_DESCRIPTOR SecurityDescriptor,
	__out POBJECT_TYPE *ObjectType
	)
{
	OBCREATEOBJECTYPTE pfn_ObCreateObjectType;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return STATUS_UNSUCCESSFUL;
	}
	pfn_ObCreateObjectType = (OBCREATEOBJECTYPTE)(OriginObCreateObjectType - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	
	return pfn_ObCreateObjectType(TypeName,ObjectTypeInitializer,SecurityDescriptor,ObjectType);
}
//////////////////////////////////////////////////////////////////////////
NTSTATUS __stdcall codevObCreateObject(
	__in KPROCESSOR_MODE ProbeMode,
	__in POBJECT_TYPE ObjectType,
	__in POBJECT_ATTRIBUTES ObjectAttributes,
	__in KPROCESSOR_MODE OwnershipMode,
	__inout_opt PVOID ParseContext,
	__in ULONG ObjectBodySize,
	__in ULONG PagedPoolCharge,
	__in ULONG NonPagedPoolCharge,
	__out PVOID *Object
	)
{
	OBCREATEOBJECT pfn_ObCreateObject;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return STATUS_UNSUCCESSFUL;
	}
	pfn_ObCreateObject = (OBCREATEOBJECT)(OriginObCreateObject - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_ObCreateObject(ProbeMode,
		ObjectType,
		ObjectAttributes,
		OwnershipMode,
		ParseContext,
		ObjectBodySize,
		PagedPoolCharge,
		NonPagedPoolCharge,
		Object);
}
//////////////////////////////////////////////////////////////////////////
NTSTATUS __stdcall codevObInsertObject(
	IN PVOID            Object,
	IN PACCESS_STATE    PassedAccessState OPTIONAL,
	IN ACCESS_MASK      DesiredAccess,
	IN ULONG            AdditionalReferences,
	OUT PVOID           *ReferencedObject OPTIONAL,
	OUT PHANDLE         Handle
	)
{
	OBINSERTOBJECT pfn_ObInsertObject;
	if (!MmIsAddressValidEx((PVOID)SystemKernelModuleBase) ||
		!MmIsAddressValidEx(ReloadNtosImageBase))
	{
		return STATUS_UNSUCCESSFUL;
	}
	pfn_ObInsertObject = (OBINSERTOBJECT)(OriginObInsertObject - SystemKernelModuleBase + (ULONG)ReloadNtosImageBase);
	return pfn_ObInsertObject(Object,PassedAccessState,DesiredAccess,AdditionalReferences,ReferencedObject,Handle);
}
//这个好麻烦啊
NTSTATUS __stdcall codevObReferenceObjectByHandle(
	__in HANDLE Handle,
	__in ACCESS_MASK DesiredAccess,
	__in_opt POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__out PVOID *Object,
	__out_opt POBJECT_HANDLE_INFORMATION HandleInformation
	)
{

}