/***************************************************************************************
* AUTHOR : vLink
* DATE   : 2014-10-23
* MODULE : AntiTenProtect.C
* 
* Command: 
*	Source of IOCTRL Sample Driver
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2010 vLink.
****************************************************************************************/

//#######################################################################################
//# I N C L U D E S
//#######################################################################################

#include "AntiTenProtect.h"

//////////////////////////////////////////////////////////////////////////
BOOL bWin7 = FALSE;
PDRIVER_OBJECT 	g_MyDriverObject;//自身驱动的基址
PVOID g_MyDriverBase;
ULONG g_MyDriverSize;//自身驱动的大小

VOID
DriverUnload(IN PDRIVER_OBJECT pDriverObj)
{	

	return;
}
//#######################################################################################
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@				D R I V E R   E N T R Y   P O I N T						 @@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//#######################################################################################
NTSTATUS
DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	//记录自身驱动的信息
	g_MyDriverObject = pDriverObj;
	g_MyDriverBase = pDriverObj->DriverStart;
	g_MyDriverSize = pDriverObj->DriverSize;
	//Initialize Windows Version
	GetWindowsVersion();
	if (WinVersion == WINDOWS_VERSION_7_7000 ||
		WinVersion == WINDOWS_VERSION_7_7600_UP)
	{
		bWin7 = TRUE;
	}
	else if (WinVersion == WINDOWS_VERSION_XP)
	{
		bWin7 = FALSE;
	}
	else
	{
		//既不是win7也不是winxp
		goto DriverRet;
	}
	//测试
	//__asm int 3;
	status = ReloadNtos(g_MyDriverObject);
	if (status != STATUS_SUCCESS)
	{
		goto DriverRet;
	}
DriverRet:
	return status;
}
