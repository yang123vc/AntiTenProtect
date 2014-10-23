#ifndef _NTOS_H_
#define _NTOS_H_
#include "struct.h"
#include "KernelReload.h"
//
BYTE* ReloadNtosImageBase;
ULONG SystemKernelModuleBase;
ULONG SystemKernelModuleSize;
WCHAR *SystemKernelFilePath;
extern PDRIVER_OBJECT 	g_MyDriverObject;//自身驱动的基址
NTSTATUS ReloadNtos(PDRIVER_OBJECT   DriverObject);




#endif