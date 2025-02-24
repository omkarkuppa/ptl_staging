/** @file
  BIOS Guard Driver implements the BIOS Guard Host Controller Compatibility Interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#include "BiosGuardServices.h"
#include "BiosGuardServicesDebug.h"
#include <MemInfoHob.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Register/GenerationMsr.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/WdtCommonLib.h>
#include <IntelRcStatusCode.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PerformanceLib.h>
#include <Protocol/SmmSwDispatch2.h>

///
/// Global variables
///
GLOBAL_REMOVE_IF_UNREFERENCED BIOSGUARD_PROTOCOL                *mBiosGuardProtocol;
GLOBAL_REMOVE_IF_UNREFERENCED BIOSGUARD_HOB                     *mBiosGuardHobPtr;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_BOOT_MODE                     mBootMode;
GLOBAL_REMOVE_IF_UNREFERENCED BIOSGUARD_NVS_AREA_PROTOCOL       *mBiosGuardNvsAreaProtocol = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED volatile UINT8                    mWaitTriggerAbort;
GLOBAL_REMOVE_IF_UNREFERENCED volatile UINT32                   mApThreadCount;

//
// Private GUID for BIOS Guard initializes
//
extern EFI_GUID                          gBiosGuardHobGuid;

/**
  This function is triggered by the BIOS update tool Interface with an IO trap. It executes
  BIOS Guard protocol execute with the true flag indicating that there is an update package
  in the DPR region of memory.
  This call is not triggered if PCH IO Trap Protocol is not located or
  BIOS Guard protocol is Null or unable to locate.

  @param[in] DispatchHandle      Not used
  @param[in] CallbackContext     Not used
**/
VOID
EFIAPI
BiosGuardUpdateBios (
  IN EFI_HANDLE                             DispatchHandle,
  IN CONST VOID                             *CallbackContext,
  IN OUT VOID                               *CommBuffer,
  IN OUT UINTN                              *CommBufferSize
  )
{
  DEBUG ((DEBUG_ERROR, "Entering BiosGuardUpdateBios IO trap SMI handler\n"));

  ///
  /// Invoke BIOS Guard Services for updating BIOS
  ///
  if (mBiosGuardProtocol == NULL) {
    DEBUG ((DEBUG_ERROR, "mBiosGuardProtocol is NULL\n"));
    return;
  }

  mBiosGuardProtocol->Execute (
                        mBiosGuardProtocol,
                        TRUE
                        );

  if (mBootMode == BOOT_ON_FLASH_UPDATE) {
    ///
    /// A complete BGUP is provided for Capsule and Tool updates. Therefore, the BGUP header should not
    /// be overwritten in the event another erase or write operation occurs. The status from the IO Trap
    /// is alternately placed at the BGUP Certificate address.
    ///
    CopyMem ((UINT64 *) mBgupCertificate, &mBiosGuardFullStatus, sizeof (UINT64));
  } else {
    ///
    /// Based on the interface defined for Tools implementation, the status from the IO Trap needs to be
    /// placed at the address of the BGUP, essentially overwriting the header.
    /// The BGUP Header, along with the Script, Data and BGUP Certificate are written by the tools into DPR memory,
    /// therefore, we can safely overwrite the information in that address after execution as the next time tools does an
    /// update operation, it will pass the complete package providing the proper BGUP Header
    ///
    CopyMem (mBiosGuardUpdatePackagePtr, &mBiosGuardFullStatus, sizeof (UINT64));
  }

}

/**
  This method registers and sets up the IOTRAP and NVS area for the BIOS Guard tools interface

**/
VOID
EFIAPI
BiosGuardToolsInterfaceInit (
  VOID
  )
{
  EFI_STATUS                                Status;
  EFI_HANDLE                                Handle;
  EFI_SMM_SW_REGISTER_CONTEXT               SwContext;
  EFI_SMM_SW_DISPATCH2_PROTOCOL            *SwDispatch;

  ///
  /// Locate SMM SW DISPATCH Interface Protocol
  ///
  Status = gSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID **) &SwDispatch);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gEfiSmmSwDispatch2ProtocolGuid Interface Protocol was not located\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  ///
  /// Locate BIOS Guard SMM Protocol
  ///
  Status = gSmst->SmmLocateProtocol (&gSmmBiosGuardProtocolGuid, NULL, (VOID **) &mBiosGuardProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gSmmBiosGuardProtocolGuid Interface Protocol was not located\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  if (mBiosGuardProtocol != NULL) {
    ///
    /// Register BIOS Guard SW SMI handler
    ///
    SwContext.SwSmiInputValue = (UINTN)-1;
    Handle = NULL;
    DEBUG ((DEBUG_ERROR, "Register SW SMI handler for BiosGuardUpdateBios Function\n"));
    Status = SwDispatch->Register (
                          SwDispatch,
                          (EFI_SMM_HANDLER_ENTRY_POINT2) BiosGuardUpdateBios,
                          &SwContext,
                          &Handle
                          );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Unable to Register SW SMI handler for BiosGuardUpdateBios Function\n"));
      ASSERT_EFI_ERROR (Status);
      return;
    }

    mBiosGuardNvsAreaProtocol->Area->BiosGuardMemAddress     = mBiosGuardMemAddress;
    DEBUG ((DEBUG_INFO, "mBiosGuardNvsAreaProtocol BiosGuardMemAddress = %x\n", mBiosGuardNvsAreaProtocol->Area->BiosGuardMemAddress));
    mBiosGuardNvsAreaProtocol->Area->BiosGuardMemSize        = (UINT8) RShiftU64 (mBiosGuardMemSize, 20);
    DEBUG ((DEBUG_INFO, "mBiosGuardNvsAreaProtocol BiosGuardMemSize = %x\n", mBiosGuardNvsAreaProtocol->Area->BiosGuardMemSize));
    mBiosGuardNvsAreaProtocol->Area->BiosGuardSwSmi          = (UINT8) SwContext.SwSmiInputValue;
    DEBUG ((DEBUG_INFO, "mBiosGuardNvsAreaProtocol BiosGuardSwSmi = %x\n", mBiosGuardNvsAreaProtocol->Area->BiosGuardSwSmi));

    mBiosGuardHobPtr->BiosGuardMemAddress                    = mBiosGuardNvsAreaProtocol->Area->BiosGuardMemAddress;
    DEBUG ((DEBUG_INFO, "mBiosGuardNvsAreaProtocol BiosGuardMemAddress = %x\n", mBiosGuardNvsAreaProtocol->Area->BiosGuardMemAddress));
    mBiosGuardHobPtr->BiosGuardMemSize                       = mBiosGuardNvsAreaProtocol->Area->BiosGuardMemSize;
    DEBUG ((DEBUG_INFO, "mBiosGuardNvsAreaProtocol BiosGuardMemSize = %x\n", mBiosGuardNvsAreaProtocol->Area->BiosGuardMemSize));
    mBiosGuardHobPtr->BiosGuardSwSmi                         = mBiosGuardNvsAreaProtocol->Area->BiosGuardSwSmi;
    DEBUG ((DEBUG_INFO, "mBiosGuardNvsAreaProtocol BiosGuardSwSmi = %x\n", mBiosGuardNvsAreaProtocol->Area->BiosGuardSwSmi));
  }

  return;
}

/**
  Entry point for the BIOS Guard protocol driver.

  @param[in] ImageHandle         Image handle of this driver.
  @param[in] SystemTable         Global system service table.

  @retval EFI_SUCCESS            Initialization complete.
  @retval EFI_ERROR              Driver exits abnormally.
  @retval EFI_OUT_OF_RESOURCES   Do not have enough resources to initialize the driver.
  @retval EFI_INVALID_PARAMETER  Invalid parameter.
  @retval EFI_NOT_FOUND          CPU Data HOB not available.
**/
EFI_STATUS
EFIAPI
InstallBiosGuardProtocol (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                                Status;
  BIOSGUARD_INSTANCE                        *BiosGuardInstance;
  VOID                                      *BiosGuardProtocolAddr;
  BOOLEAN                                   BiosGuardToolsInterface;
  MSR_PLAT_FRMW_PROT_CTRL_REGISTER          PlatFrmwProtCtrl;

  PERF_INMODULE_BEGIN ("InstallBiosGuardProtocol");
  BiosGuardToolsInterface  = FALSE;

  ///
  /// Allocate pool for BIOS Guard protocol instance
  ///
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    sizeof (BIOSGUARD_INSTANCE),
                    (VOID **) &BiosGuardInstance
                    );

  if ((EFI_ERROR (Status)) || (BiosGuardInstance == NULL)) {
    DEBUG ((DEBUG_ERROR, "Unable to allocate pool for BIOS Guard protocol instance\n"));
    return Status;
  }

  ZeroMem ((VOID *) BiosGuardInstance, sizeof (BIOSGUARD_INSTANCE));
  BiosGuardInstance->Handle = NULL;
  BiosGuardProtocolAddr     = NULL;

  ///
  /// Initialize BIOS Guard protocol instance if BIOS Guard is supported and enabled
  ///
  if (IsPfatEnabled ()) {
    PlatFrmwProtCtrl.Uint64 = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if (PlatFrmwProtCtrl.Bits.PfatEnable == 1) {
      mBootMode = GetBootModeHob ();

      ///
      /// Initialize the BIOS Guard protocol instance
      ///
      Status = BiosGuardProtocolConstructor (BiosGuardInstance);
      if (EFI_ERROR (Status)) {
        gSmst->SmmFreePool (BiosGuardInstance);
        return Status;
      }
      BiosGuardProtocolAddr = &(BiosGuardInstance->BiosGuardProtocol);

      ///
      /// Initialize tools support only if we are in Legacy flow
      ///
      if (mBiosGuardUpdatePackageInTseg == FALSE) {
        BiosGuardToolsInterface = TRUE;
      }
    } else {
      DEBUG ((DEBUG_INFO, "BIOS Guard Feature supported but disabled\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "BIOS Guard Feature is not supported\n"));
  }

  ///
  /// Install the SMM BIOSGUARD_PROTOCOL interface
  ///
  Status = gSmst->SmmInstallProtocolInterface (
                    &(BiosGuardInstance->Handle),
                    &gSmmBiosGuardProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    BiosGuardProtocolAddr
                    );
  if (EFI_ERROR (Status)) {
    gSmst->SmmFreePool (BiosGuardInstance);
  } else {
    if (BiosGuardToolsInterface == TRUE) {
      ///
      /// Locate BIOS Guard Nvs
      ///
      Status = gBS->LocateProtocol (&gBiosGuardNvsAreaProtocolGuid, NULL, (VOID **) &mBiosGuardNvsAreaProtocol);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "gBiosGuardNvsAreaProtocolGuid Interface Protocol was not located\n"));
        gSmst->SmmFreePool (BiosGuardInstance);
        ASSERT_EFI_ERROR (Status);
        return Status;
      }

      BiosGuardToolsInterfaceInit ();
    }
  }
 
  PERF_INMODULE_END ("InstallBiosGuardProtocol");
  return Status;
}

/**
  Initialize BIOS Guard protocol instance.

  @param[in] BiosGuardInstance  Pointer to BiosGuardInstance to initialize

  @retval EFI_SUCCESS           The protocol instance was properly initialized
  @retval EFI_NOT_FOUND         BIOS Guard Binary module was not found.
  @retval EFI_OUT_OF_RESOURCES  Allocated memory could not be freed.
  @retval EFI_INVALID_PARAMETER Invalid BIOS Guard Binary module buffer size
**/
EFI_STATUS
BiosGuardProtocolConstructor (
  BIOSGUARD_INSTANCE *BiosGuardInstance
  )
{
  EFI_STATUS                      Status;
  BGPDT                           *Bgpdt;
  UINTN                           NumPages;
  EFI_PHYSICAL_ADDRESS            Addr;
  EFI_PHYSICAL_ADDRESS            BiosGuardModule;
  DPR_INFO_HOB                    *DprInfoHobPtr;
  UINT32                          BiosGuardModuleSize;
  MEMORY_PLATFORM_DATA_HOB        *MemInfoHob;
  UINT8                           TotalDprSizeMB;

  NumPages                      = BIOSGUARD_MEMORY_PAGES + BIOSGUARD_ALIGNMENT_IN_PAGES;
  BiosGuardModuleSize           = 0;
  Addr                          = 0;
  mBiosGuardUpdatePackageInTseg = FALSE;
  MemInfoHob                    = NULL;
  TotalDprSizeMB                = 0;

  DEBUG ((DEBUG_INFO, "BiosGuardProtocolConstructor\n"));

  mBiosGuardHobPtr = GetFirstGuidHob (&gBiosGuardHobGuid);
  if (mBiosGuardHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "BIOS Guard HOB not available\n"));
    return EFI_NOT_FOUND;
  }

  if (mBiosGuardHobPtr->BiosGuardModulePtr == (EFI_PHYSICAL_ADDRESS) NULL) {
    return EFI_NOT_FOUND;
  }

  BiosGuardModuleSize  = mBiosGuardHobPtr->BiosGuardModuleSize;
  DEBUG ((DEBUG_INFO, "BIOS Guard Module Size: %x\n", BiosGuardModuleSize));
  if (BiosGuardModuleSize == 0) {
    return EFI_NOT_FOUND;
  }

  ///
  /// Allocate memory buffer for BIOS Guard Module
  ///
  Status = gSmst->SmmAllocatePages (AllocateAnyPages, EfiRuntimeServicesData, NumPages, &Addr);
  if (EFI_ERROR (Status) || Addr == (EFI_PHYSICAL_ADDRESS) NULL) {
    DEBUG ((DEBUG_ERROR, "Allocation of buffer for BIOS Guard Module failed.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Align address to 512K.
  ///
  BiosGuardModule  = Addr &~(ALIGN_512KB - 1);
  BiosGuardModule  = BiosGuardModule < Addr ? (BiosGuardModule + ALIGN_512KB) : BiosGuardModule;

  ///
  /// Checking if the size of pages to free is not zero
  ///
  if (Addr != BiosGuardModule) {
    ///
    /// Free all allocated pages till start of the aligned memory address
    ///
    Status = gSmst->SmmFreePages (
                      Addr,
                      EFI_SIZE_TO_PAGES (BiosGuardModule - Addr)
                      );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Freeing of allocated pages till start of the aligned memory address failed.\n"));
      return EFI_OUT_OF_RESOURCES;
    }
  }

  ///
  /// Checking if the size of pages to free is not zero
  ///
  if (((Addr + (ALIGN_512KB * 2)) - (BiosGuardModule + ALIGN_512KB)) != 0) {
    ///
    /// Free all allocated pages after the end of the aligned memory address
    ///
    Status = gSmst->SmmFreePages (
                      BiosGuardModule + ALIGN_512KB,
                      EFI_SIZE_TO_PAGES ((Addr + (ALIGN_512KB * 2)) - (BiosGuardModule + ALIGN_512KB))
                      );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Freeing of allocated pages after the end of the aligned memory address failed.\n"));
      return EFI_OUT_OF_RESOURCES;
    }
  }

  ///
  /// Copy BIOS Guard Module into prepared buffer.
  ///

  if(!IsSimicsEnvironment ()){
    if (BiosGuardModuleSize != *(UINT32*) (mBiosGuardHobPtr->BiosGuardModulePtr + BIOSGUARD_MODULE_SIZE_OFFSET)) {
      DEBUG ((DEBUG_ERROR, "Invalid BIOS Guard Binary module size set at BIOS Guard Module Binary size parameter.\n"));
      return EFI_INVALID_PARAMETER;
    }
  }

  CopyMem ((VOID*) BiosGuardModule, (VOID*) mBiosGuardHobPtr->BiosGuardModulePtr, BiosGuardModuleSize);

  DprInfoHobPtr = GetFirstGuidHob (&gDprInfoHobGuid);
  if (DprInfoHobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "DPR Info HOB not available\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// Allocate pool for BGPDT Data
  ///
  Status = gSmst->SmmAllocatePool (
                    EfiRuntimeServicesData,
                    mBiosGuardHobPtr->Bgpdt.BgpdtSize,
                    (VOID **) &Bgpdt
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to Allocate pool for BGPDT Data\n"));
    return Status;
  }

  CopyMem (Bgpdt, &mBiosGuardHobPtr->Bgpdt, mBiosGuardHobPtr->Bgpdt.BgpdtSize);

  ///
  /// Determine if BGUP is to be placed in TSEG
  /// If the size allocated to BIOS Guard in DPR is 0, BGUP will be stored in TSEG
  /// Otherwise, BGUP will use the memory allocated within DPR
  ///
  if (mBiosGuardHobPtr->BiosGuardMemSize == 0) {
    mBiosGuardUpdatePackageInTseg = TRUE;
  }

  ///
  /// Legacy runtime flow or Capsule Update based flow, allocate BIOS Guard variables in DPR
  ///
  if (mBiosGuardUpdatePackageInTseg == FALSE) {
    ///
    /// First, initialize the BIOS Guard memory address in the pre-allocated space within DPR
    ///
    MemInfoHob = (MEMORY_PLATFORM_DATA_HOB *) GetFirstGuidHob (&gSiMemoryPlatformDataGuid);
    if (MemInfoHob == NULL) {
      DEBUG ((DEBUG_ERROR, "MemInfoHob not available\n"));
      gSmst->SmmFreePool (Bgpdt);
      return EFI_NOT_FOUND;
    }
    //Check, this TotalDprSizeMB is adding TXT and BiosGuard DPR, samplecode was in MemoryInit.c, CalculateTotalDprMemorySize
    TotalDprSizeMB              += mBiosGuardHobPtr->BiosGuardMemSize;
    TotalDprSizeMB              += DprInfoHobPtr->DprDirectory[EnumDprDirectoryTxt].Size;
    mBiosGuardMemAddress        = (EFI_PHYSICAL_ADDRESS) ((MemInfoHob->Data.TsegBase - TotalDprSizeMB) << 20);
    mBiosGuardMemSize           = (UINT32) LShiftU64 (mBiosGuardHobPtr->BiosGuardMemSize, 20);

    ///
    /// The BGUP uses the majority of the space within the allocated region and uses the initial block of memory
    ///
    mBiosGuardUpdatePackagePtr  = (BGUP *) mBiosGuardMemAddress;

    ///
    /// A subset of the memory is allotted for the space required for the BIOS Guard certificate after the BGUP
    ///
    mBgupCertificate            = (EFI_PHYSICAL_ADDRESS) (mBiosGuardMemAddress + mBiosGuardMemSize - BGUPC_MEMORY_OFFSET);

    ///
    /// A final allocation is made for the BIOS Guard Log at the end of the buffer after the certificate
    ///
    mBiosGuardLogPtr            = (BIOSGUARD_LOG *) (mBiosGuardMemAddress + mBiosGuardMemSize - BIOSGUARD_LOG_MEMORY_OFFSET);
  } else {
    ///
    /// Non-BIOS update flow, allocate BGUP pointer within TSEG
    ///
    mBiosGuardMemSize = BGUP_TSEG_BUFFER_SIZE;
    NumPages = mBiosGuardMemSize / EFI_PAGE_SIZE;
    Status = gSmst->SmmAllocatePages (
                      AllocateAnyPages,
                      EfiRuntimeServicesData,
                      NumPages,
                      &mBiosGuardMemAddress
                      );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Memory for BGUP not allocated in SMM!\n"));
      gSmst->SmmFreePool (Bgpdt);
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    ///
    /// During runtime, the TSEG memory is only allocated for BGUP
    ///
    mBiosGuardUpdatePackagePtr  = (BGUP *) mBiosGuardMemAddress;

    ///
    /// Initialize the Certificate memory and Log memory to NULL as it isn't available in the standard path
    ///
    mBgupCertificate = (EFI_PHYSICAL_ADDRESS) NULL;
    mBiosGuardLogPtr = NULL;
  }

  ///
  /// Save the Log & BGUP headers into a temporary location so it can be used for re-initialization of the log between BiosGuardProtocolExecute calls
  ///
  CopyMem (&mBiosGuardLogTemp, &mBiosGuardHobPtr->BiosGuardLog, sizeof (BIOSGUARD_LOG));
  CopyMem (&mBiosGuardBgupHeaderTemp, &mBiosGuardHobPtr->BgupHeader, sizeof (BGUP_HEADER));

  ///
  /// Initialize the BIOS Guard Update Package with the package header and zero out the rest of the buffer
  ///
  ZeroMem (mBiosGuardUpdatePackagePtr, mBiosGuardMemSize);
  CopyMem (&mBiosGuardUpdatePackagePtr->BgupHeader, &mBiosGuardHobPtr->BgupHeader, sizeof (BGUP_HEADER));
  mBiosGuardUpdatePackagePtr->BgupHeader.ScriptSectionSize  = 0;
  mBiosGuardUpdatePackagePtr->BgupHeader.DataSectionSize    = 0;
  mBiosGuardUpdateCounter = 0;

  ///
  /// Set Begin command
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] = BIOSGUARD_COMMAND_BEGIN;

  ///
  /// Initialize the BIOS Guard protocol instance
  ///
  BiosGuardInstance->Signature                                      = BIOSGUARD_SIGNATURE;
  BiosGuardInstance->BiosGuardProtocol.Write                        = BiosGuardProtocolWrite;
  BiosGuardInstance->BiosGuardProtocol.Erase                        = BiosGuardProtocolBlockErase;
  BiosGuardInstance->BiosGuardProtocol.Execute                      = BiosGuardProtocolExecute;
  BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardModule]        = BiosGuardModule;
  BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardModule]        |= LShiftU64 (BIOSGUARD_DIRECTORY_BIOSGUARD_MODULE_ENTRY, 56);
  BiosGuardInstance->BiosGuardDirectory[EnumBgpdt]                  = (EFI_PHYSICAL_ADDRESS) Bgpdt;
  BiosGuardInstance->BiosGuardDirectory[EnumBgpdt]                  |= LShiftU64 (BIOSGUARD_DIRECTORY_BGPDT_ENTRY, 56);
  BiosGuardInstance->BiosGuardDirectory[EnumBgup]                   = (EFI_PHYSICAL_ADDRESS) mBiosGuardUpdatePackagePtr;
  BiosGuardInstance->BiosGuardDirectory[EnumBgup]                   |= LShiftU64 (BIOSGUARD_DIRECTORY_BGUP_ENTRY, 56);
  BiosGuardInstance->BiosGuardDirectory[EnumBgupCertificate]        = 0;
  BiosGuardInstance->BiosGuardDirectory[EnumBgupCertificate]        |= LShiftU64 (BIOSGUARD_DIRECTORY_UNDEFINED_ENTRY, 56);
  BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardLog]           = 0;
  BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardLog]           |= LShiftU64 (BIOSGUARD_DIRECTORY_UNDEFINED_ENTRY, 56);
  BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardDirectoryEnd]  = 0;
  BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardDirectoryEnd]  |= LShiftU64 (BIOSGUARD_DIRECTORY_END_MARKER, 56);

  return EFI_SUCCESS;
}

/**
  Check to see if the executing thread is BSP

  @retval TRUE   Executing thread is BSP
  @retval FALSE  Executing thread is AP
**/
BOOLEAN
BiosGuardIsBsp (
  VOID
  )
{
  MSR_IA32_APIC_BASE_REGISTER Msr;

  Msr.Uint64  = AsmReadMsr64 (MSR_IA32_APIC_BASE);
  return (BOOLEAN) (Msr.Bits.BSP == 1);
}

/**
  This service invokes the BIOS Guard Binary.
  All AP threads are present and released by BSP to trigger MSR 0x116.

  Set MSR 0x115 with BIOS Guard DIRECTORY Address.
  Trigger MSR 0x116 to invoke BIOS Guard Binary.
  Read MSR 0x115 to get BIOS Guard Binary Status.

  @param[in] BiosGuardInstance              Pointer to BiosGuardInstance to initialize
**/
VOID
EFIAPI
BiosGuardModuleExecuteBsp (
  IN VOID          *BiosGuardInstancePtr
  )
{
  BIOSGUARD_INSTANCE *BiosGuardInstance;
  UINT8              RetryTimeOut;
  PERF_INMODULE_BEGIN ("BiosGuardModuleExecute");
  BiosGuardInstance = BiosGuardInstancePtr;

  //
  // BSP wait for APs to check-in with timeout. If AP is in Wait for SIPI, it won't check-in.
  //
  RetryTimeOut = 0;

  while ((mApThreadCount < (gSmst->NumberOfCpus - 1)) && (RetryTimeOut != BIOSGUARD_AP_SAFE_RETRY_LIMIT)) {
    MicroSecondDelay (BIOSGUARD_WAIT_PERIOD);
    RetryTimeOut++;
  }

  //
  // Signal if all APs checked-in in or not.
  //
  mWaitTriggerAbort = RetryTimeOut != BIOSGUARD_AP_SAFE_RETRY_LIMIT ? BIOSGUARD_MODULE_EXECUTE_TRIGGER :  BIOSGUARD_MODULE_EXECUTE_ABORT;

  //
  // If not all APs check-in, then abort.
  //
  if (mWaitTriggerAbort == BIOSGUARD_MODULE_EXECUTE_ABORT) {
    return;
  }

  //
  // BSP wait for all APs to signal they are invoking BIOS Guard binary. BSP will invoke last.
  //
  while (mApThreadCount > 0) {
    CpuPause ();
  }

  //
  // Delay to allow last AP to enter BIOS Guard before BSP
  //
  MicroSecondDelay (BIOSGUARD_WAIT_PERIOD);

  //
  // Initialize signal to Wait, so caller will know when signal has changed, and System BSP (which maybe different than SMM BSP) has exited BIOS Guard binary.
  //
  mWaitTriggerAbort = BIOSGUARD_MODULE_EXECUTE_WAIT;

  ///
  /// BSP will then write final Trigger to invoke BIOS Guard Binary
  ///
  AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_TRIG_PARAM, (UINT64) BiosGuardInstance->BiosGuardDirectory);
  AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_TRIGGER, 0);
  ///
  /// Read MSR_PLAT_FRMW_PROT_TRIG_PARAM to get BIOS Guard Binary status
  ///
  BiosGuardInstance->MsrValue = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_TRIG_PARAM);

  //
  // Signal that System BSP is exiting and that BIOS Guard Binary status has been updated.
  //
  mWaitTriggerAbort = BIOSGUARD_MODULE_EXECUTE_TRIGGER;
  PERF_INMODULE_END ("BiosGuardModuleExecute");
}

/**
  This service invokes the BIOS Guard Binary.
  All AP threads are present and released by BSP to trigger MSR 0x116.

  Set MSR 0x115 with BIOS Guard DIRECTORY Address.
  Trigger MSR 0x116 to invoke BIOS Guard Binary.
  Read MSR 0x115 to get BIOS Guard Binary Status.

  @param[in] BiosGuardInstance              Pointer to BiosGuardInstance to initialize
**/
VOID
EFIAPI
BiosGuardModuleExecuteAp (
  IN VOID          *BiosGuardInstancePtr
  )
{
  BIOSGUARD_INSTANCE *BiosGuardInstance;
  BiosGuardInstance = BiosGuardInstancePtr;

  //
  // AP check-in and wait for BSP signal.
  //
  InterlockedIncrement (&mApThreadCount);
  while (mWaitTriggerAbort == BIOSGUARD_MODULE_EXECUTE_WAIT) {
    CpuPause ();
  }

  //
  // Signal that AP is invoking BIOS Guard binary.
  //
  InterlockedDecrement (&mApThreadCount);

  //
  // Invoke BIOS Guard binary.
  //
  AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_TRIG_PARAM, (UINT64) BiosGuardInstance->BiosGuardDirectory);
  AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_TRIGGER, 0);
  return;

}

/**
  Display condition associated with  error code returned by the module

  @param[in] BiosGuardStatus  Error code returned by the module
**/
VOID
EFIAPI
BiosGuardMatchStringToStatusCode (
  IN UINT16 BiosGuardStatus,
  IN UINT16 BiosGuardAdditionalData
  )
{
  switch (BiosGuardStatus) {
    case BIOSGUARD_ERR_OK:
      break;
    case BIOSGUARD_ERR_UNSUPPORTED_CPU:
      DEBUG ((DEBUG_ERROR, "Unsupported CPU -"));
      BiosGuardMatchDetailsStringToUnsupportedCpu(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_BAD_DIRECTORY:
      DEBUG ((DEBUG_ERROR, "Bad directory -"));
      BiosGuardMatchDetailsStringToBadDirectory(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_BAD_BGPDT:
      DEBUG ((DEBUG_ERROR, "Bad BGPDT -"));
      BiosGuarMatchDetailsStringToBadBgpdt(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_BAD_BGUP:
      DEBUG ((DEBUG_ERROR, "Bad BGUP -"));
      BiosGuardMatchDetailsStringToBadBgup(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_SCRIPT_SYNTAX:
      DEBUG ((DEBUG_ERROR, "Script syntax\n"));
      break;
    case  BIOSGUARD_ERR_UNDEFINED_FLASH_OBJECT:
      DEBUG ((DEBUG_ERROR, "Undefined flash object\n"));
      break;
    case  BIOSGUARD_ERR_INVALID_LINE:
      DEBUG ((DEBUG_ERROR, "Invalid line\n"));
      break;
    case  BIOSGUARD_ERR_BAD_BGUPC:
      DEBUG ((DEBUG_ERROR, "Bad BGUPC -"));
      BiosGuardMatchDetailsStringToBadBgupc(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_BAD_SVN:
      DEBUG ((DEBUG_ERROR, "Bad SVN -"));
      BiosGuardMatchDetailsStringToBadSvn(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_UNEXPECTED_OPCODE:
      DEBUG ((DEBUG_ERROR, "Unexpected opcode\n"));
      break;
    case  BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG:
      DEBUG ((DEBUG_ERROR, "Invalid address range config -"));
      BiosGuardMatchDetailsStringToInvalidAddrRangeConf(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_GENERAL_CONFIG_ERROR:
      DEBUG ((DEBUG_ERROR, "General error config -"));
      BiosGuardMatchDetailsStringToGeneralConfigError(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_EC_OPERATION_FAILED:
      DEBUG ((DEBUG_ERROR, "EC operation failed -"));
      BiosGuardMatchDetailsStringToEcOperaionFailed(BiosGuardAdditionalData);
      break;
    case  BIOSGUARD_ERR_UNSIGNED_B0_STORE:
      DEBUG ((DEBUG_ERROR, "Unsigned B0 store \n"));
      break;
    case  BIOSGUARD_ERR_FT_UPDATE_NOT_ALLOWED:
      DEBUG ((DEBUG_ERROR, "FT update not allowed \n"));
      break;
    case  BIOSGUARD_ERR_RANGE_VIOLATION:
      DEBUG ((DEBUG_ERROR, "Range valiolation \n"));
      break;
    case  BIOSGUARD_ERR_SFAM_VIOLATION:
      DEBUG ((DEBUG_ERROR, "SFAM violation \n"));
      break;
    case  BIOSGUARD_ERR_OVERFLOW:
      DEBUG ((DEBUG_ERROR, "Overflow \n"));
      break;
    case  BIOSGUARD_ERR_EXEC_LIMIT:
      DEBUG ((DEBUG_ERROR, "Exec limit \n"));
      break;
    case  BIOSGUARD_ERR_INTERNAL_ERROR:
      DEBUG ((DEBUG_ERROR, "Internal error -"));
      BiosGuardMatchDetailsStringToInternalError(BiosGuardAdditionalData);
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Error unknown\n"));
      break;
  }

}

/**
  Match EFI status to error code returned by the module

  @param[in] BiosGuardStatus  Error code returned by the module

  @retval EFI_SUCCESS            Successfully completed flash operation.
  @retval EFI_INVALID_PARAMETER  The parameters specified are not valid.
  @retval EFI_UNSUPPORTED        The CPU or SPI memory is not supported.
  @retval EFI_DEVICE_ERROR       Device error, command aborts abnormally.
**/
EFI_STATUS
EFIAPI
BiosGuardEvaluateStatusCode (
  IN UINT16 BiosGuardStatus
  )
{
  EFI_STATUS Status;

  switch (BiosGuardStatus) {
    case BIOSGUARD_ERR_OK:
      Status = EFI_SUCCESS;
      break;

    case BIOSGUARD_ERR_RANGE_VIOLATION:
    case BIOSGUARD_ERR_SFAM_VIOLATION:
    case BIOSGUARD_ERR_EXEC_LIMIT:
    case BIOSGUARD_ERR_INTERNAL_ERROR:
    case BIOSGUARD_ERR_EC_OPERATION_FAILED:
      Status = EFI_DEVICE_ERROR;
      break;

    case BIOSGUARD_ERR_UNSUPPORTED_CPU:
    case BIOSGUARD_ERR_UNDEFINED_FLASH_OBJECT:
    case BIOSGUARD_ERR_LAUNCH_FAIL:
      Status = EFI_UNSUPPORTED;
      break;

    case BIOSGUARD_ERR_UNSIGNED_B0_STORE:
    case BIOSGUARD_ERR_FT_UPDATE_NOT_ALLOWED:
      Status = EFI_SECURITY_VIOLATION;
      break;

    default:

    case BIOSGUARD_ERR_BAD_DIRECTORY:
    case BIOSGUARD_ERR_BAD_BGPDT:
    case BIOSGUARD_ERR_BAD_BGUP:
    case BIOSGUARD_ERR_SCRIPT_SYNTAX:
    case BIOSGUARD_ERR_INVALID_LINE:
    case BIOSGUARD_ERR_BAD_BGUPC:
    case BIOSGUARD_ERR_BAD_SVN:
    case BIOSGUARD_ERR_UNEXPECTED_OPCODE:
    case BIOSGUARD_ERR_OVERFLOW:
    case BIOSGUARD_ERR_INVALID_ADDRESS_RANGE_CONFIG:
    case BIOSGUARD_ERR_GENERAL_CONFIG_ERROR:
      Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}

/**
  Parse Bios Guard invocation result from mailbox MSR 0x115 to dedicated structure

  @param[in] BiosGuardInstance               Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in, out] BiosGuardInvocationResult  Pointer to the BIOSGUARD_INVOCATION_RESULT structure.
**/
VOID
EFIAPI
BiosGuardParseInvocationResult (
  IN BIOSGUARD_INSTANCE *BiosGuardInstance,
  IN BIOSGUARD_INVOCATION_RESULT *BiosGuardInvocationResult
  )
{
  BiosGuardInvocationResult->BiosGuardStatus = (UINT16) RShiftU64 (
    (BiosGuardInstance->MsrValue & LShiftU64 (V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_MASK,
                                    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_OFFSET)),
    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_STATUS_OFFSET
    );
  BiosGuardInvocationResult->BiosGuardAdditionalData = (UINT16) RShiftU64 (
    (BiosGuardInstance->MsrValue & LShiftU64 (V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_MASK,
                                    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_OFFSET)),
    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_DATA_OFFSET
    );
  BiosGuardInvocationResult->BiosGuardTerminalLine = (UINT16) RShiftU64 (
    (BiosGuardInstance->MsrValue & LShiftU64 (V_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_MASK,
                                    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_OFFSET)),
    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_TERMINAL_OFFSET
    );
  BiosGuardInvocationResult->BiosGuardSE = (UINT8) RShiftU64 (
    (BiosGuardInstance->MsrValue & B_MSR_PLAT_FRMW_PROT_TRIG_PARAM_SE),
    N_MSR_PLAT_FRMW_PROT_TRIG_PARAM_SE_OFFSET
    );
}

/**
  Print Bios Guard invocation result from dedicated structure

  @param[in] BiosGuardInvocationResult  Pointer to the BIOSGUARD_INVOCATION_RESULT structure.
**/
VOID
EFIAPI
BiosGuardPrintInvocationResult (
  IN BIOSGUARD_INVOCATION_RESULT *BiosGuardInvocationResult
  )
{
  DEBUG ((DEBUG_ERROR, "BIOS Guard Status            = 0x%X\n", BiosGuardInvocationResult->BiosGuardStatus));
  DEBUG ((DEBUG_ERROR, "BIOS Guard Additional Data   = 0x%X\n", BiosGuardInvocationResult->BiosGuardAdditionalData));
  DEBUG ((DEBUG_ERROR, "BIOS Guard Terminal Line     = 0x%X\n", BiosGuardInvocationResult->BiosGuardTerminalLine));
  DEBUG ((DEBUG_ERROR, "BIOS Guard SE                = 0x%X\n", BiosGuardInvocationResult->BiosGuardSE));
  BiosGuardMatchStringToStatusCode(BiosGuardInvocationResult->BiosGuardStatus, BiosGuardInvocationResult->BiosGuardAdditionalData);
}

/**
  This service will write BIOSGUARD_DIRECTORY MSR and invoke the BIOS Guard Module by writing to PLAT_FRMW_PROT_TRIGGER MSR for writing/erasing to flash.
  BIOS should invoke BIOSGUARD_PROTOCOL.Write() or BIOSGUARD_PROTOCOL.Erase() function prior to calling BIOSGUARD_PROTOCOL.Execute() for flash writes/erases (except for IoTrapBasedRequest).
  Write()/Erase() function will render BIOS Guard script during execution.
  Execute() function will implement the following steps:
  1. Update BIOS Guard directory with address of BGUP.
  2. All the AP's except the BSP thread are put to sleep.
  3. BIOS Guard module is invoked from BSP to execute desired operation.
  If IoTrapBasedRequest flag is set to true, BGUP (BGUP Header + BIOS Guard Script + Update data) is part of data that is passed to SMI Handler. SMI Handler invokes BIOS Guard module to process the update.
  This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This                Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] IoTrapBasedRequest  Flag to indicate flash update is requested from OS runtime via IO Trap (i.e. a Tool)

  @retval EFI_SUCCESS            Successfully completed flash operation.
  @retval EFI_INVALID_PARAMETER  The parameters specified are not valid.
  @retval EFI_UNSUPPORTED        The CPU or SPI memory is not supported.
  @retval EFI_DEVICE_ERROR       Device error, command aborts abnormally.
  @retval EFI_BAD_BUFFER_SIZE    DataSectionSize in BGUP header exceeds the size of BIOS Guard update data buffer
  @retval EFI_OUT_OF_RESOURCES   Command queue exceeds the size of BIOS Guard data buffer
**/
EFI_STATUS
EFIAPI
BiosGuardProtocolExecute (
  IN BIOSGUARD_PROTOCOL *This,
  IN BOOLEAN            IoTrapBasedRequest
  )
{
  EFI_STATUS         Status;
  BIOSGUARD_INSTANCE *BiosGuardInstance;
  UINTN              Index;
  BOOLEAN            DisallowedUpdate;
  BOOLEAN            OcWdtEnabled;
  BOOLEAN            ReloadOcWdt;
  UINT8              RetryTrigger;
  UINT64             DataSectionSizeCopy;
  UINT32             ScriptSectionSize;
  BIOSGUARD_INVOCATION_RESULT   BiosGuardInvocationResult;

  ///
  /// Initialize local variables
  ///
  BiosGuardInvocationResult.BiosGuardStatus         = BIOSGUARD_ERR_OK;
  BiosGuardInvocationResult.BiosGuardAdditionalData = BIOSGUARD_ERR_OK;
  BiosGuardInvocationResult.BiosGuardTerminalLine   = BIOSGUARD_ERR_OK;
  BiosGuardInvocationResult.BiosGuardSE             = BIOSGUARD_ERR_OK;
  DisallowedUpdate        = FALSE;
  OcWdtEnabled            = FALSE;
  ReloadOcWdt             = FALSE;
  Status                  = EFI_SUCCESS;
  RetryTrigger            = 0;

  DEBUG ((DEBUG_INFO, "BiosGuardProtocolExecute\n"));
  PERF_INMODULE_BEGIN ("BiosGuardProtocolExecute");
  BiosGuardInstance = BIOSGUARD_INSTANCE_FROM_BIOSGUARDPROTOCOL (This);

  ///
  /// Prior to execution of the BIOS Guard module, reinitialize the BIOS Guard Log area & BIOS Guard Binary Return Status
  ///
  if (mBiosGuardLogPtr != NULL) {
    ZeroMem (mBiosGuardLogPtr, BIOSGUARD_LOG_MEMORY_SIZE);
    CopyMem (mBiosGuardLogPtr, &mBiosGuardLogTemp, sizeof (BIOSGUARD_LOG));
  }
  BiosGuardInstance->MsrValue = BIOSGUARD_ERR_LAUNCH_FAIL;

  if (IoTrapBasedRequest == FALSE) {
    ///
    /// If Update Package has been created by the BIOS during POST then complete the script
    /// and create update Pkg
    ///

    //Check mBiosGuardUpdateCounter to prevent potential overflow
    if (mBiosGuardUpdateCounter > BIOSGUARD_BUFFER_SIZE_WITH_NEW_RECORDS(1)) {
      DEBUG ((DEBUG_ERROR, "Buffer overflow while placing the data from the caller into the global BIOS Guard Update data\n"));
      Status = EFI_OUT_OF_RESOURCES;
      goto BiGProtocolExecuteExit;
    }

    ///
    /// First, finalize the script by adding the "End" command
    ///
    mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++]  = BIOSGUARD_COMMAND_END;

    ///
    /// Save the script section size
    ///
    ScriptSectionSize = mBiosGuardUpdateCounter * 8;
    mBiosGuardUpdatePackagePtr->BgupHeader.ScriptSectionSize = ScriptSectionSize;

    ///
    /// Verify the DataSectionSize
    ///
    DataSectionSizeCopy = mBiosGuardUpdatePackagePtr->BgupHeader.DataSectionSize;
    if ((((UINT64) ScriptSectionSize) + DataSectionSizeCopy) >= BGUP_TSEG_BUFFER_SIZE) {
      DEBUG ((DEBUG_ERROR, "Buffer overflow while placing the data from the caller into the global BIOS Guard Update data\n"));
      Status =  EFI_BAD_BUFFER_SIZE;
      goto BiGProtocolExecuteExit;
    }

    ///
    /// Copy the BIOS Guard Update Data member variable into the BGUP buffer directly after the "End" command
    ///
    CopyMem (
      &mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter],
      &mBiosGuardUpdateData,
      DataSectionSizeCopy
      );
  } else {
    ///
    /// If the Update Package was retrieved from the OS via Global NVS and IO Trap then require it to be signed
    ///
    if (mBiosGuardUpdatePackagePtr->BgupHeader.PkgAttributes) {
      BiosGuardInstance->BiosGuardDirectory[EnumBgupCertificate]  = mBgupCertificate;
      BiosGuardInstance->BiosGuardDirectory[EnumBgupCertificate] |= LShiftU64 (BIOSGUARD_DIRECTORY_BGUP_CERTIFICATE_ENTRY, 56);
      BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardLog]     = (EFI_PHYSICAL_ADDRESS) mBiosGuardLogPtr;
      BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardLog]    |= LShiftU64 (BIOSGUARD_DIRECTORY_BIOSGUARD_LOG_ENTRY, 56);
    } else {
      /// BIOS Updates will not be allowed to be passed through when there is no certificate required
      DisallowedUpdate = TRUE;
    }
  }

  ///
  /// BIOS Guard should prevent Overclocking Watchdog Timer timeout during script execution by disabling it.
  /// BIOS Guard module execution is prevented when Overclocking Watchdog Timer is locked and enabled.
  ///
  if (IsWdtEnabled ()) {
    OcWdtEnabled = TRUE;

    if (!IsWdtLocked ()) {
      ///
      /// Disable Overclocking Watchdog Timer
      ///
      WdtDisable ();
      OcWdtEnabled = FALSE;

      ///
      /// Overclocking Watchdog Timer will be reloaded after BIOS Guard module execution
      ///
      ReloadOcWdt  = TRUE;
    }
  }

  do {
    if ((RetryTrigger != 0) && (BiosGuardInvocationResult.BiosGuardAdditionalData != BIOSGUARD_ERR_OK)) {
      DEBUG ((DEBUG_INFO, "BiosGuardProtocolExecute Retry: %x of 3\n", RetryTrigger));
    }

    if (!DisallowedUpdate && !OcWdtEnabled) {
      mWaitTriggerAbort = BIOSGUARD_MODULE_EXECUTE_WAIT;
      mApThreadCount    = 0;

      ///
      /// Prior to launching the BIOS Guard Binary on the BSP, each of the APs must launch the BIOS Guard binary so that
      /// they can be accounted for and placed in sleep by the BIOS Guard binary.
      ///
      for (Index = 0; Index < gSmst->NumberOfCpus; Index++) {
        if (Index != gSmst->CurrentlyExecutingCpu) {
          Status = gSmst->SmmStartupThisAp (BiosGuardModuleExecuteAp, Index, (VOID *) BiosGuardInstance);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "AP unable to launch BIOS Guard Module Binary. Index = %d and NumberofCpus = %d\n", Index, gSmst->NumberOfCpus));
            ASSERT_EFI_ERROR (Status);
            goto BiGProtocolExecuteExit;
          }
        }
      }

      BiosGuardModuleExecuteBsp ((VOID *) BiosGuardInstance);

      ///
      /// Reload Overclocking Watchdog Timer if it was enabled prior to BIOS Guard module execution
      ///
      if (ReloadOcWdt) {
        WdtReloadAndStart (OC_WDT_TIMEOUT_VALUE);
      }

      if (mWaitTriggerAbort != BIOSGUARD_MODULE_EXECUTE_ABORT) {

        //
        // Wait for System BSP to complete and BIOS Guard Status is valid.
        //
        while (mWaitTriggerAbort == BIOSGUARD_MODULE_EXECUTE_WAIT) {
          CpuPause ();
        }

        BiosGuardParseInvocationResult(BiosGuardInstance, &BiosGuardInvocationResult);
        Status = BiosGuardEvaluateStatusCode(BiosGuardInvocationResult.BiosGuardStatus);
      } else {
        Status = EFI_DEVICE_ERROR;
      }
    } else {
      Status = EFI_UNSUPPORTED;
    }

    if ((EFI_ERROR (Status)) || (BiosGuardInvocationResult.BiosGuardAdditionalData != BIOSGUARD_ERR_OK)) {
      if (BiosGuardInstance->MsrValue == (UINT64) BiosGuardInstance->BiosGuardDirectory) {
        DEBUG ((DEBUG_ERROR, "BIOS Guard ACM failed to launch - MSR still holding module entry point\n"));
      } else {
        BiosGuardPrintInvocationResult(&BiosGuardInvocationResult);
      }
      if (DisallowedUpdate) {
        DEBUG ((DEBUG_ERROR, "BIOS Guard Disallowed Update\n"));
      }
      if (mWaitTriggerAbort == BIOSGUARD_MODULE_EXECUTE_ABORT) {
        DEBUG ((DEBUG_ERROR, "AP Timeout\n"));
      }
      if (OcWdtEnabled) {
        DEBUG ((DEBUG_ERROR, "Overclocking Watchdog Timer is enabled and locked\n"));
      }

      RetryTrigger++;
    }
  } while ((BiosGuardInvocationResult.BiosGuardAdditionalData != BIOSGUARD_ERR_OK) && (RetryTrigger < 4));

  mBiosGuardFullStatus = BiosGuardInstance->MsrValue;

BiGProtocolExecuteExit:
  BiosGuardInstance->BiosGuardDirectory[EnumBgupCertificate] = 0;
  BiosGuardInstance->BiosGuardDirectory[EnumBgupCertificate] |= LShiftU64 (BIOSGUARD_DIRECTORY_UNDEFINED_ENTRY, 56);
  BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardLog]    = 0;
  BiosGuardInstance->BiosGuardDirectory[EnumBiosGuardLog]    |= LShiftU64 (BIOSGUARD_DIRECTORY_UNDEFINED_ENTRY, 56);
  ZeroMem (mBiosGuardUpdatePackagePtr, mBiosGuardMemSize - BIOSGUARD_LOG_MEMORY_OFFSET);
  CopyMem (&mBiosGuardUpdatePackagePtr->BgupHeader, &mBiosGuardBgupHeaderTemp, sizeof (BGUP_HEADER));
  mBiosGuardUpdatePackagePtr->BgupHeader.ScriptSectionSize    = 0;
  mBiosGuardUpdatePackagePtr->BgupHeader.DataSectionSize      = 0;
  mBiosGuardUpdateCounter = 0;

  ///
  /// Prep for the next script execution by adding the Begin command
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] = BIOSGUARD_COMMAND_BEGIN;

  PERF_INMODULE_END ("BiosGuardProtocolExecute");
  return Status;
}

/**
  This service fills BIOS Guard script buffer for flash writes.
  BIOS should invoke this function prior to calling BIOSGUARD_PROTOCOL.Execute() with all the relevant data required for flash write.
  This function will not invoke BIOS Guard Module, only create script required for writing to flash.
  This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This           Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] Offset         This value specifies the offset from the start of the SPI Flash component.
  @param[in] DataByteCount  Number of bytes in the data portion.
  @param[in] Buffer         Pointer to caller-allocated buffer containing the data sent.

  @retval EFI_SUCCESS            Successfully filled the BIOS Guard script buffer.
  @retval EFI_BAD_BUFFER_SIZE    DataSectionSize in BGUP header exceeds the size of BIOS Guard script buffer
  @retval EFI_OUT_OF_RESOURCES   Command queue exceeds the size of BIOS Guard data buffer
**/
EFI_STATUS
EFIAPI
BiosGuardProtocolWrite (
  IN BIOSGUARD_PROTOCOL *This,
  IN UINTN              Offset,
  IN UINT32             DataByteCount,
  IN OUT UINT8          *Buffer
  )
{
  UINT64 DataSectionSizeCopy = ((UINT64) mBiosGuardUpdatePackagePtr->BgupHeader.DataSectionSize);

  //Check mBiosGuardUpdateCounter to prevent potential overflow
  if (mBiosGuardUpdateCounter > BIOSGUARD_BUFFER_SIZE_WITH_NEW_RECORDS(4)) {
    DEBUG ((DEBUG_ERROR, "Buffer overflow while placing the data from the caller into the global BIOS Guard Update data\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Set Buffer Offset Index immediate command
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] =
    (LShiftU64 ((UINTN) DataSectionSizeCopy, 32)) |
    (LShiftU64 (BIOSGUARD_B0_INDEX, 16)) |
    BIOSGUARD_COMMAND_SET_BUFFER_INDEX;
  ///
  /// Set Flash Index immediate command
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] = (LShiftU64 (Offset, 32)) | (LShiftU64 (BIOSGUARD_F0_INDEX, 16)) | BIOSGUARD_COMMAND_SET_FLASH_INDEX;
  ///
  /// Write to Flash Index from Buffer Offset Index with specific Size command
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] = (LShiftU64 (DataByteCount, 32)) | (LShiftU64 (BIOSGUARD_B0_INDEX, 24)) | (LShiftU64 (BIOSGUARD_F0_INDEX, 16)) | BIOSGUARD_COMMAND_WRITE_IMM;

  ///
  /// Read hardware status
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] = (LShiftU64 (BIOSGUARD_IF_INDEX, 16)) | BIOSGUARD_COMMAND_RD_STS;

  ///
  /// Verify the DataSectionSize
  ///
  if ((DataSectionSizeCopy + ((UINT64) DataByteCount)) >= BGUP_TSEG_BUFFER_SIZE) {
    DEBUG ((DEBUG_ERROR, "Buffer overflow while placing the data from the caller into the global BIOS Guard Update data\n"));
    return EFI_BAD_BUFFER_SIZE;
  }

  ///
  /// Place the data from the caller into the global BIOS Guard Update data
  ///
  CopyMem (&mBiosGuardUpdateData[DataSectionSizeCopy], Buffer, DataByteCount);

  ///
  /// Update the size of the data section to match the input data size
  ///
  mBiosGuardUpdatePackagePtr->BgupHeader.DataSectionSize = ((UINT32) DataSectionSizeCopy) + DataByteCount;

  return EFI_SUCCESS;
}

/**
  This service fills BIOS Guard script buffer for erasing blocks in flash.
  BIOS should invoke this function prior to calling BIOSGUARD_PROTOCOL.Execute() with all the relevant data required for flash erase.
  This function will not invoke BIOS Guard module, only create script required for erasing each block in the flash.
  This function would be called by runtime driver, please do not use any MMIO macro here.

  @param[in] This     Pointer to the BIOSGUARD_PROTOCOL instance.
  @param[in] Offset   This value specifies the offset from the start of the SPI Flash component.
**/
VOID
EFIAPI
BiosGuardProtocolBlockErase (
  IN BIOSGUARD_PROTOCOL *This,
  IN UINTN              Offset
  )
{

  //Check mBiosGuardUpdateCounter to prevent potential overflow
  if (mBiosGuardUpdateCounter > BIOSGUARD_BUFFER_SIZE_WITH_NEW_RECORDS(3)) {
    DEBUG ((DEBUG_ERROR, "Buffer overflow while placing the data from the caller into the global BIOS Guard Update data\n"));
    return;
  }

  ///
  /// Set Flash Index immediate command
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] = (LShiftU64 (Offset, 32)) | (LShiftU64 (BIOSGUARD_F0_INDEX, 16)) | BIOSGUARD_COMMAND_SET_FLASH_INDEX;

  ///
  /// Erase Flash Index command
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] = (LShiftU64 (BIOSGUARD_F0_INDEX, 16)) | BIOSGUARD_COMMAND_ERASE_BLK;

  ///
  /// Read hardware status
  ///
  mBiosGuardUpdatePackagePtr->BgupBuffer[mBiosGuardUpdateCounter++] = (LShiftU64 (BIOSGUARD_IF_INDEX, 16)) | BIOSGUARD_COMMAND_RD_STS;

  ///
  /// No change necessary to the data section size
  ///
  mBiosGuardUpdatePackagePtr->BgupHeader.DataSectionSize += 0;

  return;
}
