/** @file
  The Itss Soc Library for initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiItssLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/PcdInfoLib.h>
#include <Library/P2sbPrivateLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <Register/P2sbRegs.h>
#include <Register/ItssRegs.h>
#include <Register/PchRegs.h>
#include <Ppi/SiPolicy.h>
#include <ItssHandle.h>
#include <GpioV2ControllerInterface.h>
#include <Register/GpioAcpiDefines.h>
#include <ConfigBlock/PchGeneralConfig.h>
#include <Library/PchPciBdfLib.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>
#include <Library/GpioHelpersLib.h>

extern PCH_DEV_INT_INFO       mSocPDeviceInterruptInfo[];
extern UINT32                 mSocPDeviceInterruptInfoSize;
extern PCH_DEV_INT_INFO       mSocHDeviceInterruptInfo[];
extern UINT32                 mSocHDeviceInterruptInfoSize;

/**
  Get Interrupt Info data

  @param[out] Size       Current size of the interrupt info

  @retval     Pointer to Interrupt Info data on current SOC
**/
CONST PCH_DEV_INT_INFO*
GetInterruptInfoData (
  OUT UINT32                   *Size
  )
{
  if (PtlIsPcdP()) {
    *Size = mSocPDeviceInterruptInfoSize;
    return mSocPDeviceInterruptInfo;
  } else {
      *Size = mSocHDeviceInterruptInfoSize;
      return mSocHDeviceInterruptInfo;
  }
}

//
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST PCH_PIRX_REGISTER mPirxRegister[] = {
  //Dev, PirIndex
  { 31, 0 },  // D31 LPC/eSPI, SMBUS, SPI, GbE, TraceHub
  { 6,  1 },  // D06 PCIe
  { 28, 2 },  // D28 PCIe
  { 22, 4 },  // D22 HECI,IDER,KT
  { 20, 5 },  // D20 xHCI
  { 31, 6 },  // D31 ACE
  { 16, 7 }   // D16 THC
};

/**
  Get PIRx register data table

  @param[out] Size       Current size of the PIRx register table (number of PIRx registers)

  @retval Pointer to PIRx register data table
**/
CONST PCH_PIRX_REGISTER*
EFIAPI
GetPirxRegData (
  OUT UINT32         *Size
  )
{
  *Size = ARRAY_SIZE (mPirxRegister);
  return mPirxRegister;
}

/**
  Check for GPIO IRQ conflicts

  @param[in]   ItssHandle  Pointer to ITSS handle
  @param[in]   IrqUsage    Pointer to array that represents IRQs in use

  @retval      FALSE       No conflicts found with the GPIO IRQ configuration
  @retval      TRUE        A conflict was found with the GPIO IRQ configuration
**/
BOOLEAN
EFIAPI
ItssCheckGpioConflicts (
  IN ITSS_HANDLE    *ItssHandle,
  IN UINT32         *IrqUsage
  )
{
  GPIOV2_SERVICES   *GpioServices;
  GPIOV2_INTERFACE  *GpioInterface;
  UINT32            CommunityIndex;
  UINT32            GroupIndex;
  UINT32            PadIndex;
  GPIOV2_PAD_OWN    Ownership;
  BOOLEAN           GpiIe;
  UINT32            GpioIrqNumber;
  EFI_STATUS        Status;

  if (GpioOverrideLevel1Enabled ()) {
    return FALSE;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices failed (Status: %d)\n", __FUNCTION__, Status));
    ASSERT (FALSE);
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    for (GroupIndex = 0; GroupIndex < GpioInterface->Private.Communities[CommunityIndex].GroupsNum; GroupIndex++) {
      for (PadIndex = 0; PadIndex < GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadsNum; PadIndex++) {

        Status = GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex], &Ownership);
        if (EFI_ERROR (Status) || (Ownership != GpioV2PadOwnHost)) {
          continue;
        }

        Status = GpioInterface->Public.GetGpiIe (&(GpioInterface->Public), GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex], &GpiIe);
        if (EFI_ERROR (Status) || (GpiIe != TRUE)) {
          //
          // This pad is not routed to IOxAPIC, move to next one
          //
          continue;
        }

        Status = GpioInterface->Public.GetPadIoApicIrqNum (&(GpioInterface->Public), GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex], &GpioIrqNumber);
        if (EFI_ERROR (Status)) {
          continue;
        }
        if (((IrqUsage[GpioIrqNumber >> 5] >> (GpioIrqNumber % 32)) & BIT0) == 0x0) {
          //
          // If IRQ is not yet used, update IrqUsage[] table
          //
          IrqUsage[GpioIrqNumber >> 5]|= 0x1 << (GpioIrqNumber  % 32);
        } else {
          //
          // If IRQ is reported to be used then assert, as those interrupts
          // cannot be shareable
          //
          DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: For Pad %d.%d.%d IRQ%d is already in use!\n",
                  CommunityIndex, GroupIndex, PadIndex,
                  GpioIrqNumber));
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

/**
  Configures the P2Sb IoAPIC Control Register

  @param[in]   ItssHandle    Pointer to ITSS handle

  @retval      None
**/
VOID
EFIAPI
ItssP2SbConfigureIoApic (
  IN  ITSS_HANDLE           *ItssHandle
  )
{
  P2SbConfigureIoApic (ItssHandle->P2SbPcieConfigAccess,
                       ItssHandle->ItssPrivateConfig->IoApicAddress);
}

/**
  Get P2SB access interface for ITSS

  @param[out]  P2SbAccess          Pointer to P2SB Register Access structure
  @param[in]   P2SbCtrl            Pointer to P2SB Controller
**/
VOID
PtlPcdGetItssP2SbController (
  OUT P2SB_SIDEBAND_REGISTER_ACCESS *P2SbAccess,
  IN  P2SB_CONTROLLER               *P2SbCtrl
  )
{
  EFI_STATUS                    Status;
  P2SB_PORT_16_ID               P2SBPid;
  P2SBPid.Pid16bit = PTL_SID_F2_PID_ITSS;
  Status = PtlPcdGetP2SbController (P2SbCtrl, P2SBPid);
  ASSERT_EFI_ERROR (Status);

  BuildP2SbSidebandAccess (
    P2SbCtrl,
    P2SBPid.PortId.LocalPid,
    0,
    P2SbPrivateConfig,
    P2SbMmioAccess,
    FALSE,
    P2SbAccess
  );
}

/**
  Build an Itss handle for PTL SOC

  @param[in]     SiPolicy           SiPolicyPpi instance
  @param[out]    ItssHandle         Pointer to an initialized ITSS handle
  @param[in]     P2SbAccess         Pointer to P2SB Register Access structure
  @param[in]     P2SbCtrl           Pointer to P2SB Controller
  @param[in out] P2SbPcieAccess     Pointer to P2SB PCIe Register Access
  @param[in out] ItssPrivateConfig  Pointer to ITSS Private Config
  @param[in out] ItssCallback       Pointer to ITSS callback structure
**/
VOID
PtlPcdItssCreateHandle (
  IN      SI_POLICY_PPI                 *SiPolicy,
  OUT     ITSS_HANDLE                   *ItssHandle,
  IN      P2SB_SIDEBAND_REGISTER_ACCESS *P2SbAccess,
  IN      P2SB_CONTROLLER               *P2SbCtrl,
  IN OUT  PCI_CONFIG_SPACE_ACCESS       *P2SbPcieAccess,
  IN OUT  ITSS_PRIVATE_CONFIG           *ItssPrivateConfig,
  IN OUT  ITSS_CALLBACKS                *ItssCallBack
  )
{
  EFI_STATUS                    Status;
  PCH_INTERRUPT_CONFIG          *InterruptConfig;
  PCH_IOAPIC_CONFIG             *IoApicConfig;
  PCH_GENERAL_CONFIG            *PchGeneralConfig;
  UINT32                        PchDevIntInfoSize;
  UINT32                        ItssPirxRegDataSize;

  //
  // Get ITSS Public config data and PchGeneralConfig
  //
  Status = GetConfigBlock((VOID *)SiPolicy, &gIoApicConfigGuid, (VOID *)&IoApicConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *)SiPolicy, &gInterruptConfigGuid, (VOID *)&InterruptConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *)SiPolicy, &gPchGeneralConfigGuid, (VOID *)&PchGeneralConfig);
  ASSERT_EFI_ERROR(Status);

  //
  // ITSS SB Access initialization
  //
  ZeroMem (ItssPrivateConfig, sizeof (ITSS_PRIVATE_CONFIG));

  PtlPcdGetItssP2SbController (P2SbAccess, P2SbCtrl);
  ItssHandle->PciPcrAccess = &P2SbAccess->Access;
  PciConfigAccessInit (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, P2sbDevNumber (), P2sbFuncNumber (), P2SbPcieAccess);
  ItssHandle->P2SbPcieConfigAccess = (REGISTER_ACCESS *) P2SbPcieAccess;

  //
  // Get Device Interrupt Info Data
  //
  ItssPrivateConfig->PchDevIntInfoData      = GetInterruptInfoData (&PchDevIntInfoSize);
  ItssPrivateConfig->PchDevIntInfoSize      = PchDevIntInfoSize;

  //
  // Get Pirx Register Data Table
  //
  ItssPrivateConfig->ItssPirxRegData     = GetPirxRegData (&ItssPirxRegDataSize);
  ItssPrivateConfig->ItssPirxRegDataSize = ItssPirxRegDataSize;

  //
  // Initialize additional ITSS Private Config variables
  //
  ItssPrivateConfig->LegacyIoLowLatency  = PchGeneralConfig->LegacyIoLowLatency ? TRUE : FALSE;
  ItssPrivateConfig->UniqueIrqOverride   = 0xFF;
  ItssPrivateConfig->IoApicAddress     = PcdGet32 (PcdSiIoApicBaseAddress);
  ASSERT ((ItssPrivateConfig->IoApicAddress & 0xFFF00FFF) == 0xFEC00000);

  ItssHandle->InterruptConfig   = InterruptConfig;
  ItssHandle->IoApicConfig      = IoApicConfig;

  ItssCallBack->ItssCheckGpioConflicts   = ItssCheckGpioConflicts;
  ItssCallBack->ItssP2SbConfigureIoApic  = ItssP2SbConfigureIoApic;

  ItssHandle->ItssPrivateConfig = ItssPrivateConfig;
  ItssHandle->ItssCallBack = ItssCallBack;
}

/**
  Initialize ITSS Controller

  @param[in] SiPolicy  The Silicon Policy PPI instance
**/
VOID
PtlPcdItssInit (
  IN SI_POLICY_PPI  *SiPolicy
  )
{
  ITSS_HANDLE                     ItssHandle;
  P2SB_SIDEBAND_REGISTER_ACCESS   P2SbAccess;
  P2SB_CONTROLLER                 P2SbController;
  PCI_CONFIG_SPACE_ACCESS         P2SbPcieConfigAccess;
  ITSS_PRIVATE_CONFIG             ItssPrivateConfig;
  ITSS_CALLBACKS                  ItssCallback;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  PtlPcdItssCreateHandle (
    SiPolicy,
    &ItssHandle,
    &P2SbAccess,
    &P2SbController,
    &P2SbPcieConfigAccess,
    &ItssPrivateConfig,
    &ItssCallback
    );
  ItssInitIoApic (&ItssHandle);
  ItssConfigureInterrupts (&ItssHandle);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  Before any interrupt is enabled, set controller message enable.

  @param  None

  @retval None
**/
VOID
PtlPcdItssSetMasterMessageEnable (
  VOID
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS P2SbAccess;
  P2SB_CONTROLLER               P2SbCtrl;

  PtlPcdGetItssP2SbController (&P2SbAccess, &P2SbCtrl);
  //
  // Set ITSS.MMC, PCR[ITSS] + 3334h[0] = 1b.
  //
  P2SbAccess.Access.Write16 (&P2SbAccess.Access, R_ITSS_PCR_MMC, (UINT16)B_ITSS_PCR_MMC_MSTRMSG_EN);
}

