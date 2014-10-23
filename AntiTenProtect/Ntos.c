#include "Ntos.h"

NTSTATUS ReloadNtos(PDRIVER_OBJECT   DriverObject)
{
	//NTSTATUS status = STATUS_UNSUCCESSFUL;
	if (!GetNtosInformation(&SystemKernelFilePath,&SystemKernelModuleBase,&SystemKernelModuleSize))
	{
		if (SystemKernelFilePath)
		{
			ExFreePool(SystemKernelFilePath);
		}
		return STATUS_UNSUCCESSFUL;
	}
	if (!PeReload(SystemKernelFilePath,SystemKernelModuleBase,&ReloadNtosImageBase,g_MyDriverObject))
	{
		if (SystemKernelFilePath)
		{
			ExFreePool(SystemKernelFilePath);
		}
		if (ReloadNtosImageBase)
		{
			ExFreePool(ReloadNtosImageBase);
		}
		return STATUS_UNSUCCESSFUL;
	}
	//这个申请的内核路径到底释放不是放呢？
	if (SystemKernelFilePath)
	{
		ExFreePool(SystemKernelFilePath);
	}
	//ntos重定位之后，reload模块中的ssdt表保存的还是原始表 

	return STATUS_SUCCESS;
}