/** @file
  Framework PEIM to PchMeUma

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PerformanceLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiMeLib.h>
#include <Library/HobLib.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/Wdt.h>
#include <Register/HeciRegs.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <PchResetPlatformSpecific.h>
#include <Library/ConfigBlockLib.h>
#include <IntelRcStatusCode.h>
#include <Library/PeiPcieRpLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/MeInitLib.h>
#include <MeHandle.h>
#include <MeDefines.h>
#include <PciSbdf.h>
#include <ImrConfigHob.h>
#include <IndustryStandard/Pci30.h>

//
// ME FW communication timeout value definitions
//
#define CPURV_TIMEOUT_MULTIPLIER  50         // 50 ms
#define STALL_1_MILLISECOND       1000

/**
  This procedure will issue a Non-Power Cycle, Power Cycle, or Global Rest.
  It should not return.

  @param[in] ResetType            Type of reset to be issued.
**/
VOID
PerformReset (
  IN UINT8        ResetType
  )
{
  EFI_STATUS      Status;
  WDT_PPI         *Wdt;
  EFI_RESET_TYPE  Reset;
  PCH_RESET_DATA  ResetData;
  UINTN           ResetDataSize;

  Wdt           = NULL;
  ResetDataSize = 0;

  ///
  /// Locate WDT PPI for access to Wdt->AllowKnownReset()
  ///
  Status = PeiServicesLocatePpi (
             &gWdtPpiGuid,
             0,
             NULL,
             (VOID **) &Wdt
             );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Clear CF9GR of PCH to indicate Host reset
  /// Make sure CF9 Without Resume Well Reset is disabled
  ///
  PmcDisableCf9GlobalReset (PmcGetPwrmBase ());
  PmcDisableCf9ResetWithoutResumeWell ();

  switch (ResetType) {
    case DID_ACK_NON_PCR:
      DEBUG ((DEBUG_INFO, "ME FW DID ACK has requested a Non Power Cycle Reset.\n"));
      Reset = EfiResetWarm;
      break;

    case DID_ACK_PCR:
      DEBUG ((DEBUG_INFO, "ME FW DID ACK has requested a Power Cycle Reset.\n"));
      Reset = EfiResetCold;
      Wdt->ReloadAndStart (2);
      break;

    case DID_ACK_GRST:
      DEBUG ((DEBUG_INFO, "ME FW DID Ack requested a global reset.\n"));
      Reset = EfiResetPlatformSpecific;
      CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
      StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
      ResetDataSize = sizeof (PCH_RESET_DATA);
      break;

    default:
      return;
  }

  Wdt->AllowKnownReset ();
  (*GetPeiServicesTablePointer ())->ResetSystem2 (Reset, EFI_SUCCESS, ResetDataSize, &ResetData);
}

/**
  This procedure will enforce the BIOS Action that was requested by ME FW
  as part of the DRAM Init Done message.

  @param[in] BiosAction           CSME requested BIOS Action
**/
VOID
HandleMeBiosAction (
  IN UINT8                        BiosAction
  )
{
  switch (BiosAction) {
    case DID_ACK_NON_PCR:
    case DID_ACK_PCR:
    case DID_ACK_GRST:
      PerformReset (BiosAction);
      break;

    default:
      DEBUG ((DEBUG_INFO, "BIOS continues to POST.\n"));
      break;
  }
}

//
// RC Private Function Declarations
//

/**
  This procedure will determine whether or not the CPU was replaced
  during system power loss or via dynamic fusing.
  Calling this procedure could result in a warm reset (if ME FW is requesting one).

  @param[in] FfsHeader            Not used.
  @param[out] ForceFullTraining   When set = 0x1, MRC will be forced to perform a full
                                  memory training cycle.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
CpuReplacementCheck (
  IN VOID                         *FfsHeader,
  OUT UINT8                       *ForceFullTraining
  )
{
  PCI_SBDF                        HeciSbdf;
  HECI_GS_SHDW_REGISTER           MeFwsts2;
  EFI_STATUS                      Status;
  UINT32                          Timeout;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi;
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;
  ME_HANDLE                       *MeHandle;

  Timeout         = 0;

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HeciSbdf.PciSegmentLibAddress = MeHandle->Callbacks->GetCsmeHeciPciCfgBase (HECI1);
  if (PciSegmentRead16 (HeciSbdf.PciSegmentLibAddress + PCI_DEVICE_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "HECI not present! Skip detection!\n"));
    return EFI_SUCCESS;
  }

  //
  // Get Policy settings through the SiPreMemPolicyPpi PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  if (MePeiPreMemConfig->SkipCpuReplacementCheck) {
    DEBUG ((DEBUG_INFO, "Skip CPU replacement check by policy\n"));
    return EFI_SUCCESS;
  }

  MeFwsts2.ul = PciSegmentRead32 (HeciSbdf.PciSegmentLibAddress + R_ME_CFG_HFS_2);
  DEBUG ((DEBUG_INFO, "MeFwsts2 = %x.\n", MeFwsts2.ul));
  PERF_INMODULE_BEGIN ("CpuReplacementCheck");
  //
  // Poll 50 ms on CPU Replaced Valid bit
  //
  while ((((MeFwsts2.r.CpuReplacedValid == 0) && Timeout < CPURV_TIMEOUT_MULTIPLIER))
      && (MePeiPreMemConfig->DisableCpuReplacedPolling != 1)) {
    MicroSecondDelay (STALL_1_MILLISECOND);
    MeFwsts2.ul = PciSegmentRead32 (HeciSbdf.PciSegmentLibAddress + R_ME_CFG_HFS_2);
    Timeout++;
  }
  PERF_INMODULE_END ("CpuReplacementCheck");

  DEBUG ((DEBUG_INFO, "CpuReplacedValid = %x, ", MeFwsts2.r.CpuReplacedValid));
  DEBUG ((DEBUG_INFO, "CpuReplacedStatus = %x, ", MeFwsts2.r.CpuReplacedSts));
  DEBUG ((DEBUG_INFO, "WarmRstReqForDF = %x.\n", MeFwsts2.r.WarmRstReqForDF));

  if (Timeout >= CPURV_TIMEOUT_MULTIPLIER || MeFwsts2.r.CpuReplacedValid == 0) {
    DEBUG ((DEBUG_WARN, "Timeout occurred, the CPU Replacement Valid Bit is not set.\n"));
    *ForceFullTraining = 1;
  } else {
    if (MeFwsts2.r.CpuReplacedValid == 1) {
      if (MeFwsts2.r.WarmRstReqForDF == 1) {
        //
        // Clear DISB and Issue a Non-Power Cycle Reset
        //
        PmcClearDramInitScratchpad ();
        PerformReset (DID_ACK_NON_PCR);
        // System should not reach here
      }

      if ((MeFwsts2.r.CpuReplacedSts == 1) && (MeFwsts2.r.WarmRstReqForDF == 0)) {
        *ForceFullTraining = 1;
      }
    }
  }

  return Status;
}

/**
  This procedure will configure the ME Host General Status register,
  indicating that DRAM Initialization is complete and ME FW may
  begin using the allocated ME UMA space.

  @param[in] FfsHeader            Pointer to the FFS file header
  @param[in] InitStat             H_GS[27:24] Status
  @param[in] ImrBaseLow           Base address for IMR region (Low DWORD)
  @param[in] ImrBaseHigh          Base address for IMR region (High DWORD)

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
MeConfigDidReg (
  IN VOID                         *FfsHeader,
  IN UINT8                        InitStat,
  IN UINT32                       ImrBaseLow,
  IN UINT32                       ImrBaseHigh
  )
{
  HECI_FWS_REGISTER               MeHfs;
  EFI_STATUS                      Status;
  SI_PREMEM_POLICY_PPI            *SiPreMemPolicyPpi;
  UINT8                           MeReturnValue;
  UINT8                           BiosAction;
  ME_PEI_PREMEM_CONFIG            *MePeiPreMemConfig;
  VOID                            *HobPtr;
  UINT32                          DidSent;
  ME_HANDLE                       *MeHandle;
  PCI_SBDF                        HeciSbdf;

  //PostCodeWithDescription (0xE01, "ME-BIOS: DID Entry.");
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_DID_ENTRY_START); //PostCode(0xE01)
  DEBUG ((DEBUG_INFO, "ME-BIOS: DID Entry.\n"));

  BiosAction = 0;
  DidSent    = 0;
  HobPtr     = NULL;

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  HeciSbdf.PciSegmentLibAddress = MeHandle->Callbacks->GetCsmeHeciPciCfgBase (HECI1);
  ///
  /// Get Policy settings through the SiPreMemPolicyPpi PPI
  ///
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: DID Exit - Error by SiPreMemPolicyPpi not found, Status: %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_DID_EXIT_ERR_SIPREMEMPPIFAIL); //PostCode (0xE81)
    return Status;
  }

  MeHfs.ul = PciSegmentRead32 (HeciSbdf.PciSegmentLibAddress + R_ME_CFG_HFS);
  DEBUG ((DEBUG_INFO, "ME FW Status: 0x%08x\n", MeHfs.ul));

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMePeiPreMemConfigGuid, (VOID *) &MePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Entered ME DRAM Init Done procedure.\n"));

  if (MePeiPreMemConfig->DidInitStat != 0) {
    InitStat = (UINT8) (UINTN) MePeiPreMemConfig->DidInitStat - 1;
  }
  DEBUG ((DEBUG_INFO, "InitStat: %x\n", InitStat));

  PERF_INMODULE_BEGIN ("HeciDID");
  //
  // Send DID using HECI message
  //
  DEBUG ((DEBUG_INFO, " Sending DID as MKHI message\n"));
  Status = PeiHeciSendDid (
             ImrBaseLow,
             ImrBaseHigh,
             InitStat,
             (UINT8) HeciSbdf.Fields.Bus,
             &MeReturnValue,
             &BiosAction
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Sending DID failed! Error: %r\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "ME DRAM Init Done ACK received.\n"));
    DEBUG ((DEBUG_INFO, "HFS read after DID ACK: %x\n", MeHfs.r));

    DidSent = 1;
    HobPtr = BuildGuidDataHob (&gMeDidSentHobGuid, &DidSent, sizeof (DidSent));
    ASSERT (HobPtr != 0);
  }
  PERF_INMODULE_END ("HeciDID");

  DEBUG ((DEBUG_INFO, "BiosAction = %x\n", BiosAction));
  HandleMeBiosAction (BiosAction);

  //
  // The 'Status' local variable still keeps value of PeiHeciSendDid function.
  // Use it to update POSTCODE.
  //
  if (!EFI_ERROR (Status)) {
    //PostCodeWithDescription (0x0E21, "ME-BIOS: DID Exit - Success.");
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_DID_EXIT_SUCCESS); //PostCode(0xE21)
    DEBUG ((DEBUG_INFO, "ME-BIOS: DID Exit - Success.\n"));
  } else {
    DEBUG ((DEBUG_ERROR, "ME-BIOS: DID Exit - Error Status: %r\n", Status));
    //PostCodeWithDescription (0x0EA1, "ME-BIOS: DID Exit - Error by fail to send DID message.");
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_DID_EXIT_ERR_DIDMSGSENDFAIL); //PostCode(0xEA1)
    DEBUG ((DEBUG_INFO, "ME-BIOS: DID Exit - Error by fail to send DID message.\n"));
  }

  return EFI_SUCCESS;
}

/**
  This procedure will send the information to CSME about the IMRs which need to be
  enabled and retrieve the information about total size of memory and alignment
  required for IMRs.

  @param[out] ImrsSize            Total IMR size in MB.
  @param[out] Alignment           Required address alignment in MB.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES    ME UMA Size outside of a 0MB-128MB
  @retval Others                  An error has occured.
**/
EFI_STATUS
MeImrConfig (
  OUT UINT32        *ImrsSize,
  OUT UINT32        *ImrAlignment
  )
{
  UINT8             MeReturnValue;
  HECI_FWS_REGISTER MeHfs;
  IMR_CONFIG_HOB    *ImrConfigHob;
  VOID              *HobPtr;
  UINT32            Size;
  UINT32            Alignment;
  EFI_STATUS        Status;
  ME_HANDLE         *MeHandle;

  DEBUG ((DEBUG_INFO, "IMR Configuration start\n"));

  HobPtr = GetFirstGuidHob (&gImrConfigGuid);
  if (HobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "No IMR_CONFIG HOB\n"));
    return EFI_INVALID_PARAMETER;
  }
  ImrConfigHob = (IMR_CONFIG_HOB*) GET_GUID_HOB_DATA (HobPtr);

  Status = GetMeHandle (&MeHandle);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  MeHfs.ul = PciSegmentRead32 (MeHandle->Callbacks->GetCsmeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);
  DEBUG ((DEBUG_INFO, "ME FW Status: 0x%08x\n", MeHfs.ul));

  DEBUG ((DEBUG_INFO, "BiosImrDisableMask0 = %x\n", ImrConfigHob->BiosImrDisableMask0));
  DEBUG ((DEBUG_INFO, "BiosImrDisableMask1 = %x\n", ImrConfigHob->BiosImrDisableMask1));
  DEBUG ((DEBUG_INFO, "PCIe IMR Size       = %x\n", ImrConfigHob->PcieImrSize));

  Status = PeiHeciGetImrSizeMsg (
             ImrConfigHob->BiosImrDisableMask0,
             ImrConfigHob->BiosImrDisableMask1,
             ImrConfigHob->PcieImrSize << 20,   // PCIe IMR Size in bytes
             &MeReturnValue,
             &Size,                             // ImrsSize in bytes
             &Alignment                         // Alignment in bytes
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Requested CSE IMR size = %d Bytes\nLargest CSE IMR size = %d Bytes\n", Size, Alignment));

  //
  // If ImrsSize is not a power of two, adjust ImrsSize by rounding it to the next power of 2
  //
  if ((Size > 0) && (GetPowerOfTwo32 (Size) < Size)) {
    Size = GetPowerOfTwo32 (Size) << 1;
  }

  *ImrsSize     = Size >> 20;           // Value in MB
  *ImrAlignment = Alignment >> 20;      // Value in MB

  //
  // round up MeStolenSize/ImrAlignment
  //
  if (((*ImrsSize) << 20) < Size) {
    // when ME request has small amount of memory (in less than 1MB area) add 1MB to the size
    (*ImrsSize)++;
    DEBUG ((DEBUG_INFO, "Rounding requested CSE IMR size\n"));
  }
  if (((*ImrAlignment) << 20) < Alignment) {
    // when ME request has small amount of memory (in less than 1MB area) add 1MB to the alignment
    (*ImrAlignment)++;
    DEBUG ((DEBUG_INFO, "Rounding requested CSE IMR alignment\n"));
  }

  DEBUG ((
    DEBUG_INFO,
    "Requested CSE IMR size (rounded up) = %d MB\nLargest CSE IMR size = %d MB\n",
    *ImrsSize,
    *ImrAlignment
    ));

  //
  // ME UMA Size outside of a 0MB-128MB range is not defined.
  //
  if (*ImrsSize > 0x80) {
    return EFI_OUT_OF_RESOURCES;
  }

  return EFI_SUCCESS;
}
