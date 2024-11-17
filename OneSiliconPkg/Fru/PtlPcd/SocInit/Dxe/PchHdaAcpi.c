/** @file
  Initializes the PCH HD Audio ACPI Tables.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/AcpiTable.h>
#include "PchInit.h"
#include <HdAudioConfig.h>
#include <Library/PcdLib.h>
#include <Protocol/PchPolicy.h>
#include <HdAudioConfig.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/HdaRegs.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchInfoLib.h>
#include <Library/CpuPlatformLib.h>
#include <Register/Cpuid.h>
#include <Library/HobLib.h>

STATIC CONST UINT32 NhltSignaturesTable[] = {
  SIGNATURE_32 ('N', 'H', 'L', 'T'),
  SIGNATURE_32 ('N', 'H', 'L', '1'),
  SIGNATURE_32 ('N', 'H', 'L', '2'),
  SIGNATURE_32 ('N', 'H', 'L', '3'),
  SIGNATURE_32 ('N', 'H', 'L', '4'),
  SIGNATURE_32 ('N', 'H', 'L', '5'),
  SIGNATURE_32 ('N', 'H', 'L', '6'),
  SIGNATURE_32 ('N', 'H', 'L', '7')
  };

/**
  Retrieves address of NHLT table from XSDT/RSDT.

  @param[in]                NhltSignature    Nhlt signature

  @retval NHLT_ACPI_TABLE*  Pointer to NHLT table if found
  @retval NULL              NHLT could not be found
**/
EFI_ACPI_DESCRIPTION_HEADER *
LocateNhltAcpiTable (
  UINT32                  NhltSignature
  )
{
  EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_POINTER  *Rsdp;
  EFI_ACPI_DESCRIPTION_HEADER                   *Xsdt;
  EFI_ACPI_DESCRIPTION_HEADER                   *Nhlt;
  UINTN                                         Index;
  UINT64                                        Data64;
  EFI_STATUS                                    Status;
  Rsdp  = NULL;
  Xsdt  = NULL;
  Nhlt  = NULL;

  ///
  /// Find the AcpiSupport protocol returns RSDP (or RSD PTR) address.
  ///
  DEBUG ((DEBUG_INFO, "LocateNhltAcpiTable() Start\n"));

  Status = EfiGetSystemConfigurationTable (&gEfiAcpiTableGuid, (VOID *) &Rsdp);
  if (EFI_ERROR (Status) || (Rsdp == NULL)) {
    DEBUG ((DEBUG_ERROR, "EFI_ERROR or Rsdp == NULL\n"));
    return NULL;
  }

  Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->XsdtAddress;
  if (Xsdt == NULL || Xsdt->Signature != EFI_ACPI_6_5_EXTENDED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
    // If XSDT has not been found, check RSDT
    Xsdt = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Rsdp->RsdtAddress;
    if (Xsdt == NULL || Xsdt->Signature != EFI_ACPI_6_5_ROOT_SYSTEM_DESCRIPTION_TABLE_SIGNATURE) {
      DEBUG ((DEBUG_ERROR, "XSDT/RSDT == NULL or wrong signature\n"));
      return NULL;
    }
  }

  for (Index = sizeof (EFI_ACPI_DESCRIPTION_HEADER); Index < Xsdt->Length; Index = Index + sizeof (UINT64)) {
    Data64  = *(UINT64 *) ((UINT8 *) Xsdt + Index);
    Nhlt    = (EFI_ACPI_DESCRIPTION_HEADER *) (UINTN) Data64;
    if (Nhlt != NULL && Nhlt->Signature == NhltSignature) {
      break;
    }
    Nhlt = NULL;
  }

  if (Nhlt == NULL || Nhlt->Signature != NhltSignature) {
    DEBUG ((DEBUG_ERROR, "Nhlt == NULL or wrong signature\n"));
    return NULL;
  }

  DEBUG ((DEBUG_INFO, "Found NhltTable, Address = 0x%016lx\n", Nhlt));

  return Nhlt;
}

/**
  Sets NVS ACPI variables for HDAS._DSM and SNDW._DSD accordingly to policy.

  @param[in]                NhltAcpiTableAddress
  @param[in]                NhltAcpiTableLength
  @param[in]                *HdAudioConfigHob
  @param[in]                *HdAudioDxeConfig
**/
VOID
UpdateHdaAcpiData (
  IN CONST HDAUDIO_CONFIG_HOB     *HdAudioConfigHob,
  IN CONST HDAUDIO_DXE_CONFIG     *HdAudioDxeConfig
  )
{
  EFI_ACPI_DESCRIPTION_HEADER   *NhltTable;
  UINT8                         NhltSignatureIndex;

  mPchNvsAreaProtocol.Area->XTAL = HdAudioConfigHob->AudioXtal;
  DEBUG ((DEBUG_INFO, "XTAL NVS Value: %d Hz.\n", mPchNvsAreaProtocol.Area->XTAL));

  for (NhltSignatureIndex = 0; NhltSignatureIndex < ARRAY_SIZE (NhltSignaturesTable); NhltSignatureIndex++) {
    NhltTable = LocateNhltAcpiTable (NhltSignaturesTable[NhltSignatureIndex]);
    if (NhltTable == NULL) {
      mPchNvsAreaProtocol.Area->NHLA[NhltSignatureIndex] = (UINT64) NULL;
      mPchNvsAreaProtocol.Area->NHLL[NhltSignatureIndex] = (UINT32) 0;
    } else {
      DEBUG ((DEBUG_INFO, "UpdateHdaAcpiData():\n NHLT Address = 0x%016lx, Length = 0x%08x\n", (UINT64) (UINTN) NhltTable, (UINT32) NhltTable->Length));
      mPchNvsAreaProtocol.Area->NHLA[NhltSignatureIndex] = (UINT64) (UINTN) NhltTable;
      mPchNvsAreaProtocol.Area->NHLL[NhltSignatureIndex] = (UINT32) NhltTable->Length;
    }
  }

  mPchNvsAreaProtocol.Area->ADFM = HdAudioDxeConfig->DspFeatureMask;

  if (HdAudioConfigHob->DspEnable || HdAudioConfigHob->DspUaaCompliance == FALSE) {
    mPchNvsAreaProtocol.Area->SWQ0 = HdAudioConfigHob->AudioLinkSndw0 ? 0 : BIT1;
    mPchNvsAreaProtocol.Area->SWQ1 = HdAudioConfigHob->AudioLinkSndw1 ? 0 : BIT1;
    mPchNvsAreaProtocol.Area->SWQ2 = HdAudioConfigHob->AudioLinkSndw2 ? 0 : BIT1;
    mPchNvsAreaProtocol.Area->SWQ3 = HdAudioConfigHob->AudioLinkSndw3 ? 0 : BIT1;
    mPchNvsAreaProtocol.Area->SWQ4 = HdAudioConfigHob->AudioLinkSndw4 ? 0 : BIT1;
  } else {
    mPchNvsAreaProtocol.Area->SWQ0 = BIT1;
    mPchNvsAreaProtocol.Area->SWQ1 = BIT1;
    mPchNvsAreaProtocol.Area->SWQ2 = BIT1;
    mPchNvsAreaProtocol.Area->SWQ3 = BIT1;
    mPchNvsAreaProtocol.Area->SWQ4 = BIT1;
  }

  mPchNvsAreaProtocol.Area->SML0 = HdAudioConfigHob->AudioLinkSndw0 ? BIT0 : 0;
  mPchNvsAreaProtocol.Area->SML1 = HdAudioConfigHob->AudioLinkSndw1 ? BIT0 : 0;
  mPchNvsAreaProtocol.Area->SML2 = (HdAudioConfigHob->AudioLinkSndwLanes2 || HdAudioConfigHob->AudioLinkSndw2 ? BIT0 : 0) |
                                   (HdAudioConfigHob->AudioLinkSndwLanes2 >= HdaSndwMultilaneTwoDataLanes     ? BIT1 : 0) |
                                   (HdAudioConfigHob->AudioLinkSndwLanes2 >= HdaSndwMultilaneThreeDataLanes   ? BIT2 : 0) |
                                   (HdAudioConfigHob->AudioLinkSndwLanes2 == HdaSndwMultilaneFourDataLanes    ? BIT3 : 0);
  mPchNvsAreaProtocol.Area->SML3 = (HdAudioConfigHob->AudioLinkSndwLanes3 || HdAudioConfigHob->AudioLinkSndw3 ? BIT0 : 0) |
                                   (HdAudioConfigHob->AudioLinkSndwLanes3 >= HdaSndwMultilaneTwoDataLanes     ? BIT1 : 0) |
                                   (HdAudioConfigHob->AudioLinkSndwLanes3 >= HdaSndwMultilaneThreeDataLanes   ? BIT2 : 0) |
                                   (HdAudioConfigHob->AudioLinkSndwLanes3 == HdaSndwMultilaneFourDataLanes    ? BIT3 : 0);
  // Last instance always uses three lanes when enabled
  mPchNvsAreaProtocol.Area->SML4 = HdAudioConfigHob->AudioLinkSndw4 ? (BIT0 | BIT1 | BIT2) : 0;

  mPchNvsAreaProtocol.Area->SWMC = GetPchHdaMaxSndwLinkNum ();
  mPchNvsAreaProtocol.Area->SWCS = (UINT8) HdAudioConfigHob->SoundWireClockSelect;

  mPchNvsAreaProtocol.Area->ACS0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].AutonomousClockStop;
  mPchNvsAreaProtocol.Area->ACS1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].AutonomousClockStop;
  mPchNvsAreaProtocol.Area->ACS2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].AutonomousClockStop;
  mPchNvsAreaProtocol.Area->ACS3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].AutonomousClockStop;
  mPchNvsAreaProtocol.Area->ACS4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].AutonomousClockStop;

  mPchNvsAreaProtocol.Area->DAI0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].DataOnActiveIntervalSelect;
  mPchNvsAreaProtocol.Area->DAI1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].DataOnActiveIntervalSelect;
  mPchNvsAreaProtocol.Area->DAI2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].DataOnActiveIntervalSelect;
  mPchNvsAreaProtocol.Area->DAI3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].DataOnActiveIntervalSelect;
  mPchNvsAreaProtocol.Area->DAI4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].DataOnActiveIntervalSelect;

  mPchNvsAreaProtocol.Area->DOD0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].DataOnDelaySelect;
  mPchNvsAreaProtocol.Area->DOD1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].DataOnDelaySelect;
  mPchNvsAreaProtocol.Area->DOD2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].DataOnDelaySelect;
  mPchNvsAreaProtocol.Area->DOD3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].DataOnDelaySelect;
  mPchNvsAreaProtocol.Area->DOD4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].DataOnDelaySelect;

  mPchNvsAreaProtocol.Area->DAE0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].DataOnActiveIntervalExtSelect;
  mPchNvsAreaProtocol.Area->DAE1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].DataOnActiveIntervalExtSelect;
  mPchNvsAreaProtocol.Area->DAE2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].DataOnActiveIntervalExtSelect;
  mPchNvsAreaProtocol.Area->DAE3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].DataOnActiveIntervalExtSelect;
  mPchNvsAreaProtocol.Area->DAE4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].DataOnActiveIntervalExtSelect;

  mPchNvsAreaProtocol.Area->DAX0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].DataOnActiveIntervalExt2Select;
  mPchNvsAreaProtocol.Area->DAX1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].DataOnActiveIntervalExt2Select;
  mPchNvsAreaProtocol.Area->DAX2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].DataOnActiveIntervalExt2Select;
  mPchNvsAreaProtocol.Area->DAX3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].DataOnActiveIntervalExt2Select;
  mPchNvsAreaProtocol.Area->DAX4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].DataOnActiveIntervalExt2Select;

  mPchNvsAreaProtocol.Area->DOE0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].DataOnDelayExtSelect;
  mPchNvsAreaProtocol.Area->DOE1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].DataOnDelayExtSelect;
  mPchNvsAreaProtocol.Area->DOE2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].DataOnDelayExtSelect;
  mPchNvsAreaProtocol.Area->DOE3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].DataOnDelayExtSelect;
  mPchNvsAreaProtocol.Area->DOE4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].DataOnDelayExtSelect;

  mPchNvsAreaProtocol.Area->DOX0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].DataOnDelayExt2Select;
  mPchNvsAreaProtocol.Area->DOX1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].DataOnDelayExt2Select;
  mPchNvsAreaProtocol.Area->DOX2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].DataOnDelayExt2Select;
  mPchNvsAreaProtocol.Area->DOX3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].DataOnDelayExt2Select;
  mPchNvsAreaProtocol.Area->DOX4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].DataOnDelayExt2Select;

  mPchNvsAreaProtocol.Area->CLE0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].ClockLoopbackEnableSndw;
  mPchNvsAreaProtocol.Area->CLE1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].ClockLoopbackEnableSndw;
  mPchNvsAreaProtocol.Area->CLE2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].ClockLoopbackEnableSndw;
  mPchNvsAreaProtocol.Area->CLE3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].ClockLoopbackEnableSndw;
  mPchNvsAreaProtocol.Area->CLE4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].ClockLoopbackEnableSndw;

  mPchNvsAreaProtocol.Area->CLT0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].ClockLoopbackDelaySelectSndw;
  mPchNvsAreaProtocol.Area->CLT1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].ClockLoopbackDelaySelectSndw;
  mPchNvsAreaProtocol.Area->CLT2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].ClockLoopbackDelaySelectSndw;
  mPchNvsAreaProtocol.Area->CLT3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].ClockLoopbackDelaySelectSndw;
  mPchNvsAreaProtocol.Area->CLT4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].ClockLoopbackDelaySelectSndw;

  mPchNvsAreaProtocol.Area->CLS0 = (UINT8)HdAudioDxeConfig->SndwConfig[0].ClockLoopbackSourceSndw;
  mPchNvsAreaProtocol.Area->CLS1 = (UINT8)HdAudioDxeConfig->SndwConfig[1].ClockLoopbackSourceSndw;
  mPchNvsAreaProtocol.Area->CLS2 = (UINT8)HdAudioDxeConfig->SndwConfig[2].ClockLoopbackSourceSndw;
  mPchNvsAreaProtocol.Area->CLS3 = (UINT8)HdAudioDxeConfig->SndwConfig[3].ClockLoopbackSourceSndw;
  mPchNvsAreaProtocol.Area->CLS4 = (UINT8)HdAudioDxeConfig->SndwConfig[4].ClockLoopbackSourceSndw;

  mPchNvsAreaProtocol.Area->HDBO =  (UINT8)HdAudioDxeConfig->HdaDiscBtOffload.HdaDiscBtOffEnabled;
  mPchNvsAreaProtocol.Area->HDBO |= (UINT8)HdAudioDxeConfig->HdaDiscBtOffload.HdaDiscBtOffSspLink << 8;
  DEBUG ((DEBUG_INFO, "HDBO NVS Value: %x\n", mPchNvsAreaProtocol.Area->HDBO));
}

/**
  Initialize Intel High Definition Audio ACPI Tables

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_LOAD_ERROR          ACPI table cannot be installed
  @retval EFI_UNSUPPORTED         ACPI table not set because DSP is disabled
**/
EFI_STATUS
PchHdAudioAcpiInit (
  VOID
  )
{
  EFI_STATUS                    Status;
  UINT64                        HdaPciBase;
  CONST HDAUDIO_CONFIG_HOB      *HdAudioConfigHob;
  PCH_POLICY_PROTOCOL           *PchPolicy;
  HDAUDIO_DXE_CONFIG            *HdAudioDxeConfig;

  DEBUG ((DEBUG_INFO, "PchHdAudioAcpiInit() Start\n"));

  HdAudioConfigHob = mPchHdaConfigHob;

  ///
  /// Get PCH Policy Protocol
  ///
  Status = gBS->LocateProtocol (&gPchPolicyProtocolGuid, NULL, (VOID **)&PchPolicy);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Get HD Audio DXE Config Block
  ///
  Status = GetConfigBlock ((VOID *)PchPolicy, &gHdAudioDxeConfigGuid, (VOID *)&HdAudioDxeConfig);
  ASSERT_EFI_ERROR (Status);

  HdaPciBase = HdaPciCfgBase ();

  if ((PciSegmentRead16 (HdaPciBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) || (HdAudioConfigHob->DspEnable == FALSE)) {
    // Do not set ACPI tables if HDAudio is Function disabled or DSP is disabled
    DEBUG ((DEBUG_INFO, "AudioDSP: Non-HDAudio ACPI Table (NHLT) not set!\n"));
    return EFI_UNSUPPORTED;
  }

  UpdateHdaAcpiData (HdAudioConfigHob, HdAudioDxeConfig);

  DEBUG ((DEBUG_INFO, "PchHdAudioAcpiInit() End - Status = %r\n", Status));
  return Status;
}
