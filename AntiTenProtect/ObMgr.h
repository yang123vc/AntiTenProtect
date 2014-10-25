#pragma once
#include "struct.h"
#include "FuncAddress.h"
#include "CommonFunc.h"
extern BYTE* ReloadNtosImageBase;
extern ULONG SystemKernelModuleBase;
typedef NTSTATUS (__stdcall *OBCREATEOBJECTYPTE)(
	__in PUNICODE_STRING TypeName,
	__in POBJECT_TYPE_INITIALIZER ObjectTypeInitializer,
	__in_opt PSECURITY_DESCRIPTOR SecurityDescriptor,
	__out POBJECT_TYPE *ObjectType
	);
//此函数与2k3相同,那就直接用重载的吧
typedef NTSTATUS (__stdcall *OBCREATEOBJECT)(
	__in KPROCESSOR_MODE ProbeMode,
	__in POBJECT_TYPE ObjectType,
	__in POBJECT_ATTRIBUTES ObjectAttributes,
	__in KPROCESSOR_MODE OwnershipMode,
	__inout_opt PVOID ParseContext,
	__in ULONG ObjectBodySize,
	__in ULONG PagedPoolCharge,
	__in ULONG NonPagedPoolCharge,
	__out PVOID *Object
	);
//此函数与2k3相同,那就直接用重载的吧
typedef NTSTATUS (__stdcall *OBINSERTOBJECT)(
	IN PVOID            Object,
	IN PACCESS_STATE    PassedAccessState OPTIONAL,
	IN ACCESS_MASK      DesiredAccess,
	IN ULONG            AdditionalReferences,
	OUT PVOID           *ReferencedObject OPTIONAL,
	OUT PHANDLE         Handle
	);
typedef NTSTATUS (__stdcall *OBREFERENCEOBJECTBYHANDLE)(
	__in HANDLE Handle,
	__in ACCESS_MASK DesiredAccess,
	__in_opt POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__out PVOID *Object,
	__out_opt POBJECT_HANDLE_INFORMATION HandleInformation
	);


NTSTATUS __stdcall codevObCreateObjectType(
	__in PUNICODE_STRING TypeName,
	__in POBJECT_TYPE_INITIALIZER ObjectTypeInitializer,
	__in_opt PSECURITY_DESCRIPTOR SecurityDescriptor,
	__out POBJECT_TYPE *ObjectType
	);
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
	);
//
NTSTATUS __stdcall codevObInsertObject(
	IN PVOID            Object,
	IN PACCESS_STATE    PassedAccessState OPTIONAL,
	IN ACCESS_MASK      DesiredAccess,
	IN ULONG            AdditionalReferences,
	OUT PVOID           *ReferencedObject OPTIONAL,
	OUT PHANDLE         Handle
	);
NTSTATUS __stdcall codevObReferenceObjectByHandle(
	__in HANDLE Handle,
	__in ACCESS_MASK DesiredAccess,
	__in_opt POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__out PVOID *Object,
	__out_opt POBJECT_HANDLE_INFORMATION HandleInformation
	);