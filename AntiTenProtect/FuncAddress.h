#pragma once
#include "struct.h"
ULONG OriginObCreateObjectType;
ULONG OriginObCreateObject;
ULONG OriginObInsertObject;
//
ULONG OriginPspProcessDelete;
ULONG OriginPspThreadDelete;
ULONG OriginPsGetNextProcess;
ULONG OriginPsTerminateProcess;
//
ULONG OriginDbgkpDeleteObject;
ULONG OriginDbgkpMarkProcessPeb;
ULONG OriginDbgkpWakeTarget;
//
ULONG OriginExAcquireFastMutex;
ULONG OriginExReleaseFastMutex;
//
ULONG OriginKeStackAttachProcess;
ULONG OriginKeUnStackAttachProcess;