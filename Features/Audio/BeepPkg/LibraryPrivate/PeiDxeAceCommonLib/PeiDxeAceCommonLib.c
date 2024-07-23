/** @file
  Ace Common Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <Base.h>
#include <Uefi.h>
#include <IndustryStandard/Pci30.h>
#include <Protocol/PciIo.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Register/AceRegs.h>

#define HDA_RESET_WAIT_PERIOD             256

#define V_ACE_BCC_NON_UAA_DSP_EN          0x4
#define V_ACE_SCC_NON_UAA_DSP_EN          0x1
#define V_ACE_BCC_UAA_DSP_EN              0x4
#define V_ACE_SCC_UAA_DSP_EN              0x3

GLOBAL_REMOVE_IF_UNREFERENCED UINT64 SupportedHdaDidValue[] = {
  0x7E28,
  0x7E29,
  0x7E2A,
  0x7E2B,
  0x7E2C,
  0x7E2D,
  0x7E2E,
  0x7E2F,
  0x7F50,
  0x7F51,
  0x7F52,
  0x7F53,
  0x7F54,
  0x7F55,
  0x7F56,
  0x7F57,
  0xA828,
  0xA829,
  0xA82A,
  0xA82B,
  0xA82C,
  0xA82D,
  0xA82E,
  0xA82F,
  0x8E28,
  0x7728,
  0x7729,
  0x772A,
  0x772B,
  0x772C,
  0x772D,
  0x772E,
  0x772F,
  0xE428,
  0xE429,
  0xE42A,
  0xE42B,
  0xE42C,
  0xE42D,
  0xE42E,
  0xE42F,
  0xE328,
  0xE329,
  0xE32A,
  0xE32B,
  0xE32C,
  0xE32D,
  0xE32E,
  0xE32F
};

/**
  Check if passed instance of PciIo is HDA device.

  @param[in] EFI_PCI_IO_PROTOCOL      Instance of PciIo of a PCI device

  @retval TRUE                        HDA PCI Device supported
  @retval FALSE                       HDA PCI Device not suppoorted
**/
BOOLEAN
IsHdaDeviceSupported (
  IN EFI_PCI_IO_PROTOCOL              *PciIo
  )
{
  UINT8                               Index;
  PCI_TYPE00                          PciData;
  EFI_STATUS                          Status;

  Status = PciIo->Pci.Read (
            PciIo,
            EfiPciIoWidthUint8,
            0,
            sizeof (PciData.Hdr),
            &PciData.Hdr
            );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if ((((PciData.Hdr.ClassCode[2] == V_ACE_BCC_NON_UAA_DSP_EN) && (PciData.Hdr.ClassCode[1] == V_ACE_SCC_NON_UAA_DSP_EN)) ||
       ((PciData.Hdr.ClassCode[2] == V_ACE_BCC_UAA_DSP_EN) && (PciData.Hdr.ClassCode[1] == V_ACE_SCC_UAA_DSP_EN))) &&
        (PciData.Hdr.VendorId == 0x8086)) {
    for (Index = 0; Index < ARRAY_SIZE (SupportedHdaDidValue); Index++) {
      if (SupportedHdaDidValue[Index] == PciData.Hdr.DeviceId) {
        DEBUG ((DEBUG_INFO, "HDA Class Code: [BCC:SCC:PI] = [%x:%x:%x]\n", PciData.Hdr.ClassCode[2], PciData.Hdr.ClassCode[1], PciData.Hdr.ClassCode[0]));
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Polling the Status bit.

  @param[in] StatusReg            The register address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
EFI_STATUS
StatusPolling (
  IN      UINTN           StatusReg,
  IN      UINT32          PollingBitMap,
  IN      UINT32          PollingData
  )
{
  UINT32  LoopTime;
  UINT32  Reg32;
  Reg32 = 0;

  for (LoopTime = 0; LoopTime < HDA_MAX_LOOP_TIME; LoopTime++) {
    Reg32 = MmioRead32 (StatusReg);
    if ((Reg32 & PollingBitMap) == PollingData) {
      break;
    } else {
      MicroSecondDelay (HDA_WAIT_PERIOD);
    }
  }

  if (LoopTime >= HDA_MAX_LOOP_TIME) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  Polling the Status bit.
  Maximum polling time (us) equals HDA_MAX_LOOP_TIME * HDA_WAIT_PERIOD.

  @param[in] StatusReg            The register address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
EFI_STATUS
StatusPolling16 (
  IN      UINTN           StatusReg,
  IN      UINT16          PollingBitMap,
  IN      UINT16          PollingData
  )
{
  UINT32  LoopTime;

  for (LoopTime = 0; LoopTime < HDA_MAX_LOOP_TIME; LoopTime++) {
    if ((MmioRead16 (StatusReg) & PollingBitMap) == PollingData) {
      break;
    } else {
      MicroSecondDelay (HDA_WAIT_PERIOD);
    }
  }

  if (LoopTime >= HDA_MAX_LOOP_TIME) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}


/**
  Checks HD Audio Controller reset state.

  @param[in] HdaBar             Memory Space Base Address

  @return BOOLEAN               TRUE if controller is in reset, FALSE otherwise
**/
BOOLEAN
IsHdaControllerReset (
  IN  UINTN      HdaBar
  )
{
  ///
  /// Read RESET# bit - GCTL.CRSTB (offset 08h, BIT0)
  /// TRUE:  controller in reset state (CRSTB = 0b)
  /// FALSE: controller out of reset state (CRSTB = 1b)
  ///
  return (MmioRead32 ((UINTN) (HdaBar + R_HDA_MEM_GCTL)) & B_HDA_MEM_GCTL_CRST) ? FALSE : TRUE;
}

/**
  Function gets DSP out of reset and confirms that DSP is alive

  @param[out]  HdaBar             Pointer to DSP BAR

  @retval EFI_SUCCESS             DSP is out of reset
  @retval EFI_UNSUPPORTED         HDA is not enabled
  @retval EFI_DEVICE_ERROR        Polling CPA bit timed out, DSP is not out of reset
**/
EFI_STATUS
GetDspOutOfReset (
  OUT UINTN    HdaBar
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "%a: set SPA, waiting for CPA.\n", __FUNCTION__));

  ///
  /// Set DSP2C0CTL.SPA = 1 to get DSP subsystem out of reset.
  ///
  MmioOr32 ((UINTN) (HdaBar + R_HDA_MEM2_DSP2C0CTL), B_HDA_MEM2_DSP2C0CTL_SPA);

  ///
  /// Wait until DSP2C0CTL.CPA = 1 to confirm DSP subsystem is alive.
  ///
  Status = StatusPolling (
             (UINTN) (HdaBar + R_HDA_MEM2_DSP2C0CTL),
             (UINT32) B_HDA_MEM2_DSP2C0CTL_CPA,
             (UINT32) B_HDA_MEM2_DSP2C0CTL_CPA);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "DSP stalled! Status = %r\n", Status));
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a: CPA is set, DSP out of reset. Status = %r\n", __FUNCTION__, Status));

  return EFI_SUCCESS;
}


/**
  Function resets DSP and confirms that DSP is in reset state

  @param[out]  HdaBar             DSP BAR

  @retval EFI_SUCCESS             DSP is in reset
  @retval EFI_DEVICE_ERROR        Polling CPA bit timed out, DSP is still out of reset
**/
EFI_STATUS
ResetDsp (
  OUT UINTN    HdaBar
  )
{
  EFI_STATUS Status;

  //
  // Set DSP2C0CTL.SPA = 0 to get DSP subsystem to reset
  //
  MmioAnd32 ((UINTN) (HdaBar + R_HDA_MEM2_DSP2C0CTL), (UINT32) (~B_HDA_MEM2_DSP2C0CTL_SPA));

  DEBUG ((DEBUG_INFO, "%a: SPA cleared, waiting for CPA.\n", __FUNCTION__));

  //
  // Wait until DSP2C0CTL.CPA = 0 to confirm DSP subsystem is not alive.
  //
  Status = StatusPolling (
             (UINTN) (HdaBar + R_HDA_MEM2_DSP2C0CTL),
             (UINT32) B_HDA_MEM2_DSP2C0CTL_CPA,
             (UINT32) ~(B_HDA_MEM2_DSP2C0CTL_CPA));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a: CPA is cleared, DSP is in reset.\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Function sets HDA BAR (if not set) and enables MMIO access

  @param[in, out] HdaBar            Pointer to HDA base address
  @param[out]     IsTemporaryBar    Pointer to variable that keeps the information whether temporary BAR was set

  @retval EFI_SUCCESS               MMIO access enabled successfully
  @retval EFI_UNSUPPORTED           HDA device is not available
**/
EFI_STATUS
EnableHdaMmioAccess (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN OUT UINTN            *HdaBar,
  OUT    BOOLEAN          *IsTemporaryBar
  )
{
  UINT16                  Reg16;
  UINT32                  Reg32;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));

  *IsTemporaryBar = FALSE;


  PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_DEVICE_ID_OFFSET, 1, &Reg16);
  if (Reg16 == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "HDA is not enabled in this phase.\n"));
    return EFI_UNSUPPORTED;
  }

  PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_HDALBA, 1, HdaBar);
  if ((*HdaBar & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
    PciIo->Pci.Read (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_HDAUBA, 1, &Reg32);
    *HdaBar |= LShiftU64 ((UINT64) Reg32, 32);
  }
  *HdaBar &= ~((UINTN) (0x3FFF));

  if (*HdaBar == 0) {
    PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_COMMAND_OFFSET, 1, &Reg16);
    Reg16 &= ~EFI_PCI_COMMAND_MEMORY_SPACE;
    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint16, PCI_COMMAND_OFFSET, 1, &Reg16);

    DEBUG ((DEBUG_WARN, "HDA Bar isn't programmed in this phase, set HDA bar to temporary value.\n"));
    *HdaBar = (UINTN) PcdGet32 (PcdSiliconInitTempMemBaseAddr);

    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_HDALBA,  1, HdaBar);
    Reg32 = 0;
    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_HDAUBA,  1, &Reg32);
    Reg32 = 0;
    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_ADSPLBA, 1, &Reg32);
    Reg32 = 0;
    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_ADSPUBA, 1, &Reg32);

    *IsTemporaryBar = TRUE;
  }

  //
  // Enable memory map access
  //
  PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_COMMAND_OFFSET, 1, &Reg16);
  Reg16 |= EFI_PCI_COMMAND_MEMORY_SPACE;
  PciIo->Pci.Write (PciIo, EfiPciIoWidthUint16, PCI_COMMAND_OFFSET, 1, &Reg16);

  DEBUG ((DEBUG_INFO, "%a () - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Sets HD Audio Controller Reset state.
  In reset all state machines, memory mapped registers and links are driven to reset values.

  @param[in] HdaHandle              Pointer to Hda controller configuration structure
  @param[in] AssertResetSignal      TRUE to enter (assert) reset, FALSE to exit (de-assert) reset

  @retval EFI_SUCCESS               Controller changed reset state successfully
  @retval EFI_TIMEOUT               Polling RESET# bit timed out
**/
EFI_STATUS
AceSetControllerReset (
  IN  UINTN    HdaBar,
  IN  BOOLEAN  AssertResetSignal
  )
{
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if (AssertResetSignal) {
    ///
    /// Enter reset state: clear RESET# bit - GCTL.CRSTB (offset 08h, BIT0) = 0b
    ///
    MmioAnd32 (HdaBar + R_HDA_MEM_GCTL, (UINT32) ~(B_HDA_MEM_GCTL_CRST));
  } else {
    ///
    /// Exit reset state: set RESET# bit - GCTL.CRSTB (offset 08h, BIT0) = 1b
    ///
    MmioOr32 (HdaBar + R_HDA_MEM_GCTL, (UINT32) B_HDA_MEM_GCTL_CRST);
  }

  ///
  /// For some combo card that will need this delay because each codec has different latency to come out from RESET.
  /// This delay can make sure all codecs will be recognized by BIOS after RESET sequence.
  /// Additional delay might be required to allow codec coming out of reset prior to subsequent operations,
  /// please contact your codec vendor for detail. When clearing this bit and setting it afterward,
  /// BIOS must ensure that minimum link timing requirements (minimum RESET# assertion time, etc.) are met.
  ///
  MicroSecondDelay (HDA_RESET_WAIT_PERIOD);

  ///
  /// Poll RESET# bit until it reads back as 1 if RESET# de-asserted or 0 if asserted
  ///
  Status = StatusPolling (HdaBar + R_HDA_MEM_GCTL,  B_HDA_MEM_GCTL_CRST, (BOOLEAN) !AssertResetSignal);

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

VOID
DisableHdaDspMmioAccess (
  IN EFI_PCI_IO_PROTOCOL  *PciIo,
  IN BOOLEAN              IsTemporaryBar
  )
{
  UINT16    Reg16;
  UINT32    Reg32;

  //
  // Disable memory map access
  //
  PciIo->Pci.Read (PciIo, EfiPciIoWidthUint16, PCI_COMMAND_OFFSET, 1, &Reg16);
  Reg16 &= ~EFI_PCI_COMMAND_MEMORY_SPACE;
  PciIo->Pci.Write (PciIo, EfiPciIoWidthUint16, PCI_COMMAND_OFFSET, 1, &Reg16);

  //
  // Clear DSP and HDA BARs
  //
  if (IsTemporaryBar == TRUE) {
    Reg32 = 0;
    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_HDALBA,  1, &Reg32);
    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_HDAUBA,  1, &Reg32);
    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_ADSPLBA, 1, &Reg32);
    PciIo->Pci.Write (PciIo, EfiPciIoWidthUint32, R_HDA_CFG_ADSPUBA, 1, &Reg32);
  }
}
