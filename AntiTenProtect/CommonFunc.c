#include "CommonFunc.h"

#define SAFE_PROCESS_MAX 9
char SafeProcessList[][50] = {"svchost.exe","System","smss.exe","csrss.exe",
	"winlogon.exe","services.exe","lsass.exe","explorer.exe",
	"taskmgr.exe"};
//////////////////////////////////////////////////////////////////////////
__inline ULONG CR4()
{
	// mov eax, cr4
	__asm _emit 0x0F __asm _emit 0x20 __asm _emit 0xE0
}
VALIDITY_CHECK_STATUS MmIsAddressValidExNotPae(
	IN PVOID Pointer
	)
{
	VALIDITY_CHECK_STATUS  Return = VCS_INVALID;
	MMPTE* Pde;
	MMPTE* Pte;
	MMPTE pte;

	Pde = MiGetPdeAddress(Pointer);

	//KdPrint(("PDE is 0x%08x\n", Pde));
	if( Pde->u.Hard.Valid )
	{
		//KdPrint(("PDE entry is valid, PTE PFN=%08x\n", Pde->u.Hard.PageFrameNumber));

		Pte = MiGetPteAddress(Pointer);

		//KdPrint(("PTE is 0x%08x\n", Pte));
		if( Pte->u.Hard.Valid )
		{
			//KdPrint(("PTE entry is valid, PFN=%08x\n", Pte->u.Hard.PageFrameNumber));
			Return = VCS_VALID;
		}
		else
		{
			//
			// PTE is not valid
			//

			pte = *Pte;

			//KdPrint(("Got invalid PTE [%08x]: Proto=%d,Transition=%d,Protection=0x%x,PageFilePFN=0x%x\n",
			//	pte.u.Long,
			//	pte.u.Soft.Prototype,
			//	pte.u.Soft.Transition,
			//	pte.u.Soft.Protection,
			//	pte.u.Soft.PageFileHigh));

			if( pte.u.Long )
			{
				if( pte.u.Soft.Prototype == 1 )
				{
					//KdPrint(("PTE entry is not valid, points to prototype PTE.\n"));

					// more accurate check should be performed here for pointed prototype PTE!

					Return = VCS_PROTOTYPE;
				}
				else  // not a prototype PTE
				{
					if( pte.u.Soft.Transition != 0 )
					{
						//
						// This is a transition page. Consider it invalid.
						//

						//KdPrint(("PTE entry is not valid, points to transition page.\n"));

						Return = VCS_TRANSITION;
					}
					else if (pte.u.Soft.PageFileHigh == 0)
					{
						//
						// Demand zero page
						//

						//KdPrint(("PTE entry is not valid, points to demand-zero page.\n"));

						Return = VCS_DEMANDZERO;
					}
					else
					{
						//
						// Pagefile PTE
						//

						if( pte.u.Soft.Transition == 0 )
						{
							//KdPrint(("PTE entry is not valid, VA is paged out (PageFile offset=%08x)\n",
							//	pte.u.Soft.PageFileHigh));

							Return = VCS_PAGEDOUT;
						}
						else
						{
							//KdPrint(("PTE entry is not valid, Refault\n"));
						}
					}
				}
			}
			else
			{
				//KdPrint(("PTE entry is completely invalid\n"));
			}
		}
	}
	else
	{
		//KdPrint(("PDE entry is not valid\n"));
	}

	return Return;
}
VALIDITY_CHECK_STATUS MmIsAddressValidExPae(
	IN PVOID Pointer
	)
{
	VALIDITY_CHECK_STATUS Return = VCS_INVALID;
	MMPTE_PAE* Pde;
	MMPTE_PAE* Pte;
	MMPTE_PAE pte;

	Pde = MiGetPdeAddressPae(Pointer);

	//KdPrint(("PDE is at 0x%08x\n", Pde));
	if( Pde->u.Hard.Valid )
	{
		//KdPrint(("PDE entry is valid, PTE PFN=%08x\n", Pde->u.Hard.PageFrameNumber));

		if( Pde->u.Hard.LargePage != 0 )
		{
			//
			// This is a large 2M page
			//

			//KdPrint(("! PDE points to large 2M page\n"));

			Pte = Pde;
		}
		else
		{
			//
			// Small 4K page
			//

			// Get its PTE
			Pte  = MiGetPteAddressPae(Pointer);
		}

		//KdPrint(("PTE is at 0x%08x\n", Pte));
		if( Pte->u.Hard.Valid )
		{
			//KdPrint(("PTE entry is valid, PFN=%08x\n", Pte->u.Hard.PageFrameNumber));

			Return = VCS_VALID;
		}
		else
		{
			//
			// PTE is not valid
			//

			pte = *Pte;

			//KdPrint(("Got invalid PTE [%08x%08x]\n", pte.u.Long.HighPart, pte.u.Long.LowPart));

			if( pte.u.Long.LowPart == 0 )
			{
				//KdPrint(("PTE entry is completely invalid (page is not committed or is within VAD tree)\n"));
			}
			else
			{
				if( pte.u.Soft.Prototype == 1 )
				{
					// 					//KdPrint(("PTE entry is not valid, points to prototype PTE. Protection=%x[%s], ProtoAddress=%x\n",
					// 						(ULONG)pte.u.Proto.Protection,
					// 						MiPageProtectionString((UCHAR)pte.u.Proto.Protection),
					// 						(ULONG)pte.u.Proto.ProtoAddress));

					// more accurate check should be performed here for pointed prototype PTE!

					Return = VCS_PROTOTYPE;
				}
				else  // not a prototype PTE
				{
					if( pte.u.Soft.Transition != 0 )
					{
						//
						// This is a transition page.
						//

						// 						//KdPrint(("PTE entry is not valid, points to transition page. PFN=%x, Protection=%x[%s]\n",
						// 							(ULONG)pte.u.Trans.PageFrameNumber,
						// 							(ULONG)pte.u.Trans.Protection,
						// 							MiPageProtectionString((UCHAR)pte.u.Trans.Protection)));

						Return = VCS_TRANSITION;
					}
					else if (pte.u.Soft.PageFileHigh == 0)
					{
						//
						// Demand zero page
						//

						// 						//KdPrint(("PTE entry is not valid, points to demand-zero page. Protection=%x[%s]\n",
						// 							(ULONG)pte.u.Soft.Protection,
						// 							MiPageProtectionString((UCHAR)pte.u.Soft.Protection)));

						Return = VCS_DEMANDZERO;
					}
					else
					{
						//
						// Pagefile PTE
						//

						if( pte.u.Soft.Transition == 0 )
						{
							// 							//KdPrint(("PTE entry is not valid, VA is paged out. PageFile Offset=%08x, Protection=%x[%s]\n",
							// 								(ULONG)pte.u.Soft.PageFileHigh,
							// 								(ULONG)pte.u.Soft.Protection,
							// 								MiPageProtectionString((UCHAR)pte.u.Soft.Protection)));

							Return = VCS_PAGEDOUT;
						}
						else
						{
							//KdPrint(("PTE entry is not valid, Refault\n"));
						}
					}
				}
			}
		}
	}
	else
	{
		//KdPrint(("PDE entry is not valid\n"));
	}

	return Return;
}
VALIDITY_CHECK_STATUS MiIsAddressValidEx(
	IN PVOID Pointer
	)
{
	if( CR4() & PAE_ON ) {
		return MmIsAddressValidExPae(Pointer);
	}
	else {
		return MmIsAddressValidExNotPae(Pointer);
	}
}
BOOL MmIsAddressValidEx(
	IN PVOID Pointer
	)
{
	VALIDITY_CHECK_STATUS MmRet;
	ULONG ulTry;

	if (!ARGUMENT_PRESENT(Pointer) ||
		!Pointer){
		return FALSE;
	}
	/*
	//VCS_TRANSITION、VCS_PAGEDOUT内存居然是这样子~~擦~

	lkd> dd f8ad5ad8
	f8ad5ad8  ???????? ???????? ???????? ????????
	f8ad5ae8  ???????? ???????? ???????? ????????
	f8ad5af8  ???????? ???????? ???????? ????????
	f8ad5b08  ???????? ???????? ???????? ????????
	f8ad5b18  ???????? ???????? ???????? ????????
	f8ad5b28  ???????? ???????? ???????? ????????
	f8ad5b38  ???????? ???????? ???????? ????????
	f8ad5b48  ???????? ???????? ???????? ????????
	*/
	MmRet = MiIsAddressValidEx(Pointer);
	if (MmRet != VCS_VALID){
		return FALSE;
	}
	return TRUE;
}
/************************************************************************/
//对源地址的数据进行安全拷贝，再对拷贝后的数据进行操作
//
/************************************************************************/
NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size)
{
	PMDL  pSrcMdl, pDstMdl;
	PUCHAR pSrcAddress, pDstAddress;
	NTSTATUS st = STATUS_UNSUCCESSFUL;
	ULONG r;
	BOOL bInit = FALSE;

	pSrcMdl = IoAllocateMdl(SrcAddr, Size, FALSE, FALSE, NULL);
	if (MmIsAddressValidEx(pSrcMdl))
	{
		MmBuildMdlForNonPagedPool(pSrcMdl);
		pSrcAddress = (PUCHAR)MmGetSystemAddressForMdlSafe(pSrcMdl, NormalPagePriority);
		if (MmIsAddressValidEx(pSrcAddress))
		{
			pDstMdl = IoAllocateMdl(DstAddr, Size, FALSE, FALSE, NULL);
			if (MmIsAddressValidEx(pDstMdl))
			{
				__try
				{
					MmProbeAndLockPages(pDstMdl, KernelMode, IoWriteAccess);
					pDstAddress = (PUCHAR)MmGetSystemAddressForMdlSafe(pDstMdl, NormalPagePriority);
					if (MmIsAddressValidEx(pDstAddress))
					{
						RtlZeroMemory(pDstAddress,Size);
						RtlCopyMemory(pDstAddress, pSrcAddress, Size);
						st = STATUS_SUCCESS;
					}
					MmUnlockPages(pDstMdl);
				}
				__except(EXCEPTION_EXECUTE_HANDLER)
				{                 
					if (pDstMdl) MmUnlockPages(pDstMdl);

					if (pDstMdl) IoFreeMdl(pDstMdl);

					if (pSrcMdl) IoFreeMdl(pSrcMdl);

					return GetExceptionCode();
				}
				IoFreeMdl(pDstMdl);
			}
		}            
		IoFreeMdl(pSrcMdl);
	}
	return st;
}
/************************************************************************/
//判断是否是系统进程和调试器进程
//
/************************************************************************/
BOOL IsSystemProcess(PEPROCESS Eprocess)
{
	BOOL bRet = FALSE;
	ULONG i;
	PCHAR ProcessName = PsGetProcessImageFileName();
	if (!MmIsAddressValidEx(ProcessName))
	{
		return FALSE;
	}
	for (i = 0; i < SAFE_PROCESS_MAX; i++)
	{
		if (_stricmp(ProcessName,SafeProcessList[i]) == 0)
		{
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}
//
BOOL IsDebuggerProcess(PEPROCESS Eprocess)
{
	PCHAR ProcessName = PsGetProcessImageFileName();
	if (MmIsAddressValidEx(ProcessName))
	{
		if (_stricmp(ProcessName,"od.exe") == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}