/** @file
  This file contains routines that support PCI Express initialization

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
#include <PcieHelpersLib.h>
#include <Library/PcieHelperLib.h>
#include <PchPcieRpConfig.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>
#include <Register/PcieSipRegs.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/PchPcieRpRegs.h>

#include <IpCpcie.h>
#include <IpPcieRegs.h>
#include <IpWrapperCntxtInfoClient.h>

#define MSLV_BIT_OFFSET   0
#define MSLS_BIT_OFFSET  10
#define MNSLV_BIT_OFFSET 13
#define MNSLS_BIT_OFFSET 23

#define ASPM_L1_NO_LIMIT 0xFF
#define ASPM_L0s_NO_LIMIT 0x7
#define LINK_RETRAIN_WAIT_TIME 1000 // microseconds

#pragma pack(push, 1)
typedef union {
  struct {
    UINT32  RequesterCapable       : 1;
    UINT32  ResponderCapable       : 1;
    UINT32  RootCapable            : 1;
    UINT32  Reserved               : 5;
    UINT32  LocalClockGranularity  : 8;
    UINT32  Reserved2              : 16;
  } Bits;
  UINT32  Uint32;
} PTM_CAPS;

typedef union {
  struct {
    UINT32 Enable                 : 1;
    UINT32 RootSelect             : 1;
    UINT32 Reserved               : 6;
    UINT32 EffectiveGranularity   : 8;
    UINT32 Reserved2              : 16;
  } Bits;
  UINT32  Uint32;
} PTM_CTRL;

typedef union {
  struct {
  UINT32 MaxSnoopLatencyValue         : 10;
  UINT32 MaxSnoopLatencyScale         : 3;
  UINT32 MaxSnoopLatencyRequirement   : 1;
  UINT32 MaxNoSnoopLatencyValue       : 10;
  UINT32 MaxNoSnoopLatencyScale       : 3;
  UINT32 MaxNoSnoopLatencyRequirement : 1;
  UINT32 ForceOverride                : 1;
  UINT32 Reserved                     : 7;
  } Bits;
  UINT32  Uint32;
} LTR_OVR;

//
// This structure keeps in one place all data relevant to enabling L0s and L1.
// L0s latencies are encoded in the same way as in hardware registers. The only operation
// that will be performed on them is comparison
// L1 latencies are decoded to microseconds, because they will be used in subtractions and additions
//
typedef union {
  struct {
    UINT32  L0sSupported          : 1;
    UINT32  L1Supported           : 1;
    UINT32  L0sAcceptableLatency  : 3; // encoded as in hardware register
    UINT32  L1AcceptableLatencyUs : 8; // decoded to microseconds
    UINT32  LinkL0sExitLatency    : 3; // encoded as in hardware register
    UINT32  LinkL1ExitLatencyUs   : 8; // decoded to microseconds
    UINT32  Reserved0             : 8;
    } Bits;
    UINT32  Uint32;
} ASPM_CAPS;

typedef union {
  struct {
    UINT32  PmL12     : 1;
    UINT32  PmL11     : 1;
    UINT32  AspmL12   : 1;
    UINT32  AspmL11   : 1;
    UINT32  L1pss     : 1;
    UINT32  L1sses    : 1;
    UINT32  L1sseis   : 1;
    UINT32  Reserved0 : 1;
    UINT32  Cmrt      : 8;
    UINT32  TpoScale  : 2;
    UINT32  Reserved1 : 1;
    UINT32  TpoValue  : 5;
    UINT32  Reserved2 : 8;
  } Bits;
  UINT32  Uint32;
} L1SS_CAPS;

typedef union {
  struct {
    INT32      Reserved0           : 16;
    UINT32     CompleterSupported  : 1;
    UINT32     RequesterSupported  : 1;
    } Bits;
    UINT32 Uint32;
} TENBITTAG_CAPS;
#pragma pack(pop)

/**
  Get max payload size supported by device.

  @param[in] Sbdf   device's segment:bus:device:function coordinates

  @retval    Max payload size, encoded in the same way as in register (0=128b, 1=256b, etc)
**/
STATIC
UINT8
GetMps (
  SBDF Sbdf
  )
{
  return (PciSegmentRead16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCAP_OFFSET) & B_PCIE_DCAP_MPS);
}

/**
  Sets Maximum Payload Size to be used by device

  @param[in] Sbdf   device's segment:bus:device:function coordinates
  @param[in] Mps    Max payload size, encoded in the same way as in register (0=128b, 1=256b, etc)
**/
STATIC
VOID
SetMps (
  SBDF  Sbdf,
  UINT8  Mps
  )
{
  DEBUG ((DEBUG_INFO, "SetMps on %02x:%02x:%02x to %d\n", Sbdf.Bus,Sbdf.Dev,Sbdf.Func, Mps));
  PciSegmentAndThenOr16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCTL_OFFSET, (UINT16) ~B_PCIE_DCTL_MPS, Mps << N_PCIE_DCTL_MPS);
}

/**
  Enables LTR feature in given device

  @param[in] Sbdf            device's segment:bus:device:function coordinates
**/
STATIC
VOID
EnableLtr (
  SBDF Sbdf
  )
{
  if (Sbdf.PcieCap == 0) {
    return;
  }
  PciSegmentOr32 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCTL2_OFFSET, B_PCIE_DCTL2_LTREN);
}

/**
  Initializes Dev:Func numbers for use in FindNextPcieChild or FindNextLegalSbdf functions.

  @param[out] Sbdf            device's segment:bus:device:function coordinates
**/
STATIC
VOID
InitChildFinder (
  OUT SBDF *Sbdf
  )
{
  //
  // Initialize Dev/Func to maximum values, so that when FindNextLegalSbdf ()
  // is called on those input parameters, it will return 1st legal address (Dev 0 Func 0).
  //
  Sbdf->Dev = PCI_MAX_DEVICE;
  Sbdf->Func = PCI_MAX_FUNC;
}

/**
  Checks the device is a bridge and has non-zero secondary bus number assigned.
  If so, it returns TRUE and initializes ChildSbdf with such values that
  allow searching for devices on the secondary bus.
  ChildSbdf will be mangled even if this function returns FALSE.

  Legal bus assignment is assumed. This function doesn't check subordinate bus numbers of
  the the device it was called on or any bridges between it and root complex

  @param[in]  Sbdf       device's segment:bus:device:function coordinates
  @param[out] ChildSbdf  SBDF initialized in such way that calling FindNextPcieChild( ) on it will find all children devices

  @retval TRUE if device is a bridge and has a bus behind it; FALSE otherwise
**/
STATIC
BOOLEAN
HasChildBus (
  SBDF   Sbdf,
  SBDF   *ChildSbdf
  )
{
  UINT32 Data32;
  UINT64 Base;
  UINT8  SecondaryBus;

  ChildSbdf->Seg = Sbdf.Seg;
  InitChildFinder (ChildSbdf);

  Base = SbdfToBase (Sbdf);

  if (PciSegmentRead8 (Base + R_PCI_BCC_OFFSET) != PCI_CLASS_BRIDGE) {
    DEBUG ((DEBUG_INFO, "HasChildBus%02:%02:%02: no\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return FALSE;
  }
  Data32 = PciSegmentRead32 (Base + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET);
  SecondaryBus = (UINT8)((Data32 & B_PCI_BRIDGE_BNUM_SCBN) >> 8);
  ChildSbdf->Bus = SecondaryBus;
  if (SecondaryBus == 0) {
    DEBUG ((DEBUG_INFO, "HasChildBus%02x:%02x:%02x: no\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return FALSE;
  } else {
    DEBUG ((DEBUG_INFO, "HasChildBus%02x:%02x:%02x: yes, %x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func, SecondaryBus));
    return TRUE;
  }
}

/**
  Sets LTR limit in a device.

  @param[in] Base            device's base address
  @param[in] Ltr             LTR limit
**/
STATIC
VOID
SetLtrLimit (
  UINT64    Base,
  LTR_LIMIT Ltr
  )
{
  UINT16 LtrCapOffset;
  UINT16 Data16;

  LtrCapOffset = PcieBaseFindExtendedCapId (Base, R_PCIE_LTRECH_CID);
  if (LtrCapOffset == 0) {
    return;
  }
  Data16 = (UINT16)((Ltr.MaxSnoopLatencyValue << N_PCIE_LTRECH_MSLR_VALUE) | (Ltr.MaxSnoopLatencyScale << N_PCIE_LTRECH_MSLR_SCALE));
  PciSegmentWrite16(Base + LtrCapOffset + R_PCIE_LTRECH_MSLR_OFFSET, Data16);

  Data16 = (UINT16)((Ltr.MaxNoSnoopLatencyValue << N_PCIE_LTRECH_MNSLR_VALUE) | (Ltr.MaxNoSnoopLatencyScale << N_PCIE_LTRECH_MNSLR_SCALE));
  PciSegmentWrite16(Base + LtrCapOffset + R_PCIE_LTRECH_MNSLR_OFFSET, Data16);
}

/**
  Checks if given device supports Clock Power Management

  @param[in] Sbdf     segment:bus:device:function coordinates of a device

  @retval TRUE when device supports it, FALSE otherwise
**/
STATIC
BOOLEAN
IsCpmSupported (
  SBDF Sbdf
  )
{
  return !!(PciSegmentRead32 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_LCAP_OFFSET) & B_PCIE_LCAP_CPM);
}

/**
  Sets Enable Clock Power Management bit for given device

  @param[in] Sbdf            segment:bus:device:function coordinates of a device
**/
STATIC
VOID
EnableCpm (
  SBDF Sbdf
  )
{
  PciSegmentOr16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_LCTL_OFFSET, B_PCIE_LCTL_ECPM);
}

/**
  There are some devices whose implementation of L1 substates is partially broken. This function checks
  list of such devices and if one is found, overrides their L1ss-related capabilities

  @param[in]     Base       base address of device
  @param[in]     Override   table of devices that need override
  @param[in,out] L1ss       on entry, capabilities read from register; on exit, capabilities modified according ot override table
**/
STATIC
VOID
OverrideL1ssCaps (
  UINT64         Base,
  OVERRIDE_TABLE *Override,
  L1SS_CAPS      *L1ss
  )
{
  UINT16 DeviceId;
  UINT16 VendorId;
  UINT8  Revision;
  UINT32 Index;

  VendorId = PciSegmentRead16 (Base + PCI_VENDOR_ID_OFFSET);
  DeviceId = PciSegmentRead16 (Base + PCI_DEVICE_ID_OFFSET);
  Revision = PciSegmentRead8  (Base + PCI_REVISION_ID_OFFSET);

  for (Index = 0; Index < Override->Size; Index++) {
    if (((Override->Table[Index].OverrideConfig & PchPcieL1SubstatesOverride) == PchPcieL1SubstatesOverride) &&
        (Override->Table[Index].VendorId == VendorId) &&
        (Override->Table[Index].DeviceId == DeviceId) &&
        (Override->Table[Index].RevId == Revision || Override->Table[Index].RevId == 0xFF)) {

      L1ss->Bits.PmL12   &= !!(Override->Table[Index].L1SubstatesCapMask & B_PCIE_EX_L1SCAP_PPL12S);
      L1ss->Bits.PmL11   &= !!(Override->Table[Index].L1SubstatesCapMask & B_PCIE_EX_L1SCAP_PPL11S);
      L1ss->Bits.AspmL12 &= !!(Override->Table[Index].L1SubstatesCapMask & B_PCIE_EX_L1SCAP_AL12S);
      L1ss->Bits.AspmL11 &= !!(Override->Table[Index].L1SubstatesCapMask & B_PCIE_EX_L1SCAP_AL1SS);
      if (Override->Table[Index].L1sTpowerOnValue != 0) {
        L1ss->Bits.Cmrt = Override->Table[Index].L1sCommonModeRestoreTime;
        L1ss->Bits.TpoScale = Override->Table[Index].L1sTpowerOnScale;
        L1ss->Bits.TpoValue = Override->Table[Index].L1sTpowerOnValue;
      }
      return;
    }
  }
}

/**
  Returns L1 sub states capabilities of a device

  @param[in] Base   base address of a device

  @retval L1SS_CAPS structure filled with device's capabilities
**/
STATIC
L1SS_CAPS
GetL1ssCaps (
  UINT64         Base,
  OVERRIDE_TABLE *Override
  )
{
  L1SS_CAPS Capabilities = {0};
  UINT16    PcieCapOffset;
  UINT32    CapsRegister;

  PcieCapOffset = GetOverrideL1ssCapsOffset (Base, Override);
  if (PcieCapOffset == 0) {
    PcieCapOffset = PcieBaseFindExtendedCapId (Base, V_PCIE_EX_L1S_CID);
  }
  if (PcieCapOffset == 0) {
    return Capabilities;
  }
  CapsRegister = PciSegmentRead32 (Base + PcieCapOffset + R_PCIE_EX_L1SCAP_OFFSET);
  if (CapsRegister & B_PCIE_EX_L1SCAP_L1PSS) {
    //
    // Skip L1.1 checking since some device only indecate L1.2 support.
    // [1604452805]
    //
    Capabilities.Bits.PmL11    = !!(CapsRegister & B_PCIE_EX_L1SCAP_PPL11S);
    Capabilities.Bits.PmL12    = !!(CapsRegister & B_PCIE_EX_L1SCAP_PPL12S);
    Capabilities.Bits.AspmL12  = !!(CapsRegister & B_PCIE_EX_L1SCAP_AL12S);
    Capabilities.Bits.AspmL11  = !!(CapsRegister & B_PCIE_EX_L1SCAP_AL1SS);
    Capabilities.Bits.Cmrt     = (CapsRegister & B_PCIE_EX_L1SCAP_CMRT) >> N_PCIE_EX_L1SCAP_CMRT;
    Capabilities.Bits.TpoValue = (CapsRegister & B_PCIE_EX_L1SCAP_PTV) >> N_PCIE_EX_L1SCAP_PTV;
    Capabilities.Bits.TpoScale = (CapsRegister & B_PCIE_EX_L1SCAP_PTPOS) >> N_PCIE_EX_L1SCAP_PTPOS;
  }
  OverrideL1ssCaps (Base, Override, &Capabilities);
  return Capabilities;
}

/**
  Returns combination of two sets of L1 substate capabilities
  Given feature is supported by the link only if both sides support it
  Time parameters for link (Cmrt and Tpo) depend on the bigger value between two sides

  @param[in] L1ssA      L1 substate capabilities of first device
  @param[in] L1ssB      L1 substate capabilities of second device

  @retval Link's L1 substate capabilities
**/
STATIC
L1SS_CAPS
CombineL1ss (
  L1SS_CAPS L1ssA,
  L1SS_CAPS L1ssB
  )
{
  L1SS_CAPS Combined;

  Combined.Bits.PmL12   = L1ssA.Bits.PmL12 && L1ssB.Bits.PmL12;
  Combined.Bits.PmL11   = L1ssA.Bits.PmL11 && L1ssB.Bits.PmL11;
  Combined.Bits.AspmL12 = L1ssA.Bits.AspmL12 && L1ssB.Bits.AspmL12;
  Combined.Bits.AspmL11 = L1ssA.Bits.AspmL11 && L1ssB.Bits.AspmL11;
  Combined.Bits.Cmrt    = MAX (L1ssA.Bits.Cmrt, L1ssB.Bits.Cmrt);
  if (TpoToUs (L1ssA.Bits.TpoScale, L1ssA.Bits.TpoValue) > TpoToUs (L1ssB.Bits.TpoScale, L1ssB.Bits.TpoValue)) {
    Combined.Bits.TpoScale = L1ssA.Bits.TpoScale;
    Combined.Bits.TpoValue = L1ssA.Bits.TpoValue;
  } else {
    Combined.Bits.TpoScale = L1ssB.Bits.TpoScale;
    Combined.Bits.TpoValue = L1ssB.Bits.TpoValue;
  }
  return Combined;
}

/**
  Configures L1 substate feature in a device

  @param[in] Sbdf     segment:bus:device:function coordinates of a device
  @param[in] L1ss     configuration to be programmed
  @param[in] Override table of devices that require special handling
**/
STATIC
VOID
SetL1ss (
  SBDF           Sbdf,
  L1SS_CAPS      L1ss,
  OVERRIDE_TABLE *Override
  )
{
  UINT16    PcieCapOffset;
  UINT32    Ctrl1Register;
  UINT32    Ctrl2Register;
  UINT64    Base;

  Base = SbdfToBase (Sbdf);
  Ctrl1Register = 0;
  Ctrl2Register = 0;

  PcieCapOffset = GetOverrideL1ssCapsOffset (Base, Override);
  if (PcieCapOffset == 0) {
    PcieCapOffset = PcieBaseFindExtendedCapId (Base, V_PCIE_EX_L1S_CID);
  }
  if (PcieCapOffset == 0) {
    return;
  }
  Ctrl1Register |= (L1ss.Bits.PmL12 ? B_PCIE_EX_L1SCAP_PPL12S : 0);
  Ctrl1Register |= (L1ss.Bits.PmL11 ? B_PCIE_EX_L1SCAP_PPL11S : 0);
  Ctrl1Register |= (L1ss.Bits.AspmL12 ? B_PCIE_EX_L1SCAP_AL12S : 0);
  Ctrl1Register |= (L1ss.Bits.AspmL11 ? B_PCIE_EX_L1SCAP_AL1SS : 0);
  if ((GetDeviceType (Sbdf) == DevTypePcieDownstream)) {
    Ctrl1Register |= (L1ss.Bits.Cmrt << N_PCIE_EX_L1SCAP_CMRT);
  }
  ///
  /// BWG 1.3 Section 5.5.7.6 LTR Threshold Latency
  /// Set L1.2 LTR threshold using formula (TpoToUs (L1ss.TpoScale, L1ss.TpoValue) + L1ss.Cmrt + 10)
  ///
  Ctrl1Register |= ((TpoToUs (L1ss.Bits.TpoScale, L1ss.Bits.TpoValue) + L1ss.Bits.Cmrt + 10) << N_PCIE_EX_L1SCTL1_L12LTRTLV);
  Ctrl1Register |= (2 << N_PCIE_EX_L1SCTL1_L12LTRTLSV);
  Ctrl1Register |= (0x9D << N_PCIE_EX_L1SCTL1_L12LTRTLV);

  Ctrl2Register |= (L1ss.Bits.TpoScale);
  Ctrl2Register |= (L1ss.Bits.TpoValue << N_PCIE_EX_L1SCTL2_POWT);

  //
  // Set CLKREQ Acceleration Interrupt Enable
  //
  Ctrl1Register |= B_PCIE_EX_L1SCTL1_L1SSEIE;
  PciSegmentWrite32 (Base + PcieCapOffset + R_PCIE_EX_L1SCTL1_OFFSET, 0);
  PciSegmentWrite32 (Base + PcieCapOffset + R_PCIE_EX_L1SCTL2_OFFSET, Ctrl2Register);
  PciSegmentWrite32 (Base + PcieCapOffset + R_PCIE_EX_L1SCTL1_OFFSET, Ctrl1Register);
}

/**
  Modifies L1 latency by provided value

  @param[in] Aspm     Structure that contains ASPM capabilities of a link, including L1 acceptable latency
  @param[in] Value    Value, in microseconds, to be added to L1 acceptable latency. Can be negative.

  @retval             Aspm structure with modified L1 acceptable latency
**/
STATIC
ASPM_CAPS
PatchL1AcceptableLatency (
  ASPM_CAPS Aspm,
  INT8      Value
  )
{
  if (Aspm.Bits.L1AcceptableLatencyUs != ASPM_L1_NO_LIMIT) {
    if (Value > 0) {
      Aspm.Bits.L1AcceptableLatencyUs += Value;
    } else {
      if (Aspm.Bits.L1AcceptableLatencyUs > (UINT32)(-1*Value)) {
        Aspm.Bits.L1AcceptableLatencyUs = Aspm.Bits.L1AcceptableLatencyUs + Value;
      } else {
        Aspm.Bits.L1AcceptableLatencyUs = 0;
      }
    }
  }
  return Aspm;
}

/**
  Reads ASPM capabilities of a device

  @param[in] Sbdf segment:bus:device:function coordinates of a device

  @retval           structure containing device's ASPM capabilities
**/
STATIC
ASPM_CAPS
GetAspmCaps (
  SBDF   Sbdf
  )
{

  UINT32    LinkCapRegister;
  UINT32    DevCapRegister;
  UINT64    Base;
  ASPM_CAPS Aspm = {0};

  Base = SbdfToBase (Sbdf);

  LinkCapRegister = PciSegmentRead32 (Base + Sbdf.PcieCap + R_PCIE_LCAP_OFFSET);
  DevCapRegister = PciSegmentRead32 (Base + Sbdf.PcieCap + R_PCIE_DCAP_OFFSET);

  ///
  /// Check endpoint for pre-1.1 devices based on the Role based Error Reporting Capability bit. Don't report L0s support for old devices
  ///
  if (DevCapRegister & B_PCIE_DCAP_RBER) {
    Aspm.Bits.L0sSupported = !!(LinkCapRegister & B_PCIE_LCAP_APMS_L0S);
  }
  Aspm.Bits.L1Supported = !!(LinkCapRegister & B_PCIE_LCAP_APMS_L1);

  Aspm.Bits.LinkL0sExitLatency = (LinkCapRegister & B_PCIE_LCAP_EL0) >> N_PCIE_LCAP_EL0;
  Aspm.Bits.LinkL1ExitLatencyUs = L1LatencyToUs( (LinkCapRegister & B_PCIE_LCAP_EL1) >> N_PCIE_LCAP_EL1);

  if (GetDeviceType (Sbdf) == DevTypePcieEndpoint) {
    Aspm.Bits.L0sAcceptableLatency = (DevCapRegister & B_PCIE_DCAP_E0AL) >> N_PCIE_DCAP_E0AL;
    Aspm.Bits.L1AcceptableLatencyUs = L1LatencyToUs( (DevCapRegister & B_PCIE_DCAP_E1AL) >> N_PCIE_DCAP_E1AL);
    DEBUG ((DEBUG_INFO, "GetAspmCaps %02x:%02x:%02x L0s%c %d:%d L1%c %d:%d\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func,
                                                                           Aspm.Bits.L0sSupported?'+':'-', Aspm.Bits.LinkL0sExitLatency, Aspm.Bits.L0sAcceptableLatency,
                                                                           Aspm.Bits.L1Supported?'+':'-', Aspm.Bits.LinkL1ExitLatencyUs, Aspm.Bits.L1AcceptableLatencyUs));
  } else {
    Aspm.Bits.L0sAcceptableLatency = ASPM_L0s_NO_LIMIT;
    Aspm.Bits.L1AcceptableLatencyUs = ASPM_L1_NO_LIMIT;
    DEBUG ((DEBUG_INFO, "GetAspmCaps %02x:%02x:%02x L0s%c %d:x L1%c %d:x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func,
                                                                           Aspm.Bits.L0sSupported?'+':'-', Aspm.Bits.LinkL0sExitLatency,
                                                                           Aspm.Bits.L1Supported?'+':'-', Aspm.Bits.LinkL1ExitLatencyUs));
  }
  return Aspm;
}

/**
  Get ASPM L0s and L1 override of given device.

  @param[in] Sbdf                Segment,Bus,Device,Function address of currently visited PCIe device
  @param[in,out] MyAspm          Current device's ASPM capabilities structure
  @param[in] Override            Pch Pcie devices OverrideTable
**/
STATIC
VOID
GetOverrideAspm (
  SBDF           Sbdf,
  ASPM_CAPS      *MyAspm,
  OVERRIDE_TABLE *Override
  )
{
  UINT16      DeviceId;
  UINT16      VendorId;
  UINT8       Revision;
  UINT32      Index;
  UINT64      Base;

  Base = SbdfToBase (Sbdf);

  VendorId = PciSegmentRead16 (Base + PCI_VENDOR_ID_OFFSET);
  DeviceId = PciSegmentRead16 (Base + PCI_DEVICE_ID_OFFSET);
  Revision = PciSegmentRead8  (Base + PCI_REVISION_ID_OFFSET);

  for (Index = 0; Index < Override->Size; Index++) {
    if (((Override->Table[Index].OverrideConfig & PchPcieL1L2Override) == PchPcieL1L2Override) &&
        (Override->Table[Index].VendorId == VendorId) &&
        (Override->Table[Index].DeviceId == DeviceId) &&
        (Override->Table[Index].RevId == Revision || Override->Table[Index].RevId == 0xFF)) {
      DEBUG ((DEBUG_INFO, "GetOverrideAspm %02x:%02x:%02x, original L0sSupported = 0x%x, L1Supported = 0x%x\n",
              Sbdf.Bus, Sbdf.Dev, Sbdf.Func, MyAspm->Bits.L0sSupported, MyAspm->Bits.L1Supported));
      if (MyAspm->Bits.L0sSupported) {
        //
        // If L0s is supported in capability, apply platform override.
        //
        MyAspm->Bits.L0sSupported = Override->Table[Index].EndPointAspm & BIT0;
      }
      if (MyAspm->Bits.L1Supported) {
        //
        // If L1 is supported in capability, apply platform override.
        //
        MyAspm->Bits.L1Supported = (Override->Table[Index].EndPointAspm & BIT1) >> 1;
      }
      DEBUG ((DEBUG_INFO, "GetOverrideAspm %02x:%02x:%02x, override L0sSupported = 0x%x, L1Supported = 0x%x\n",
              Sbdf.Bus, Sbdf.Dev, Sbdf.Func, MyAspm->Bits.L0sSupported, MyAspm->Bits.L1Supported));
    }
  }
}

/**
  Combines ASPM capabilities of two devices on both ends of a link to determine link's ASPM capabilities

  @param[in] AspmA, AspmB  ASPM capabilities of two devices

  @retval    ASPM_CAPS structure containing combined ASPM capabilities
**/
STATIC
ASPM_CAPS
CombineAspm (
  ASPM_CAPS AspmA,
  ASPM_CAPS AspmB,
  BOOLEAN   DownstreamPort
  )
{
  ASPM_CAPS Combined;

  if (DownstreamPort) {
    //
    // When combining ASPM in downstream ports, combination must reflect state of link just below
    // and consider all acceptable latencies of all endpoints anywhere down below that port
    //
    Combined.Bits.L0sSupported          = AspmA.Bits.L0sSupported & AspmB.Bits.L0sSupported;
    Combined.Bits.L1Supported           = AspmA.Bits.L1Supported  & AspmB.Bits.L1Supported;
    Combined.Bits.LinkL0sExitLatency    = MAX (AspmA.Bits.LinkL0sExitLatency,    AspmB.Bits.LinkL0sExitLatency);
    Combined.Bits.LinkL1ExitLatencyUs   = MAX (AspmA.Bits.LinkL1ExitLatencyUs,   AspmB.Bits.LinkL1ExitLatencyUs);
    Combined.Bits.L0sAcceptableLatency  = MIN (AspmA.Bits.L0sAcceptableLatency,  AspmB.Bits.L0sAcceptableLatency);
    Combined.Bits.L1AcceptableLatencyUs = MIN (AspmA.Bits.L1AcceptableLatencyUs, AspmB.Bits.L1AcceptableLatencyUs);
  } else {
    //
    // When combining ASPM in switch upstream ports,
    // Supported and ExitLatency must only reflect capabilities of upstream port itself
    // But acceptable latencies must consider all endpoints anywhere below
    //
    Combined.Bits.L0sSupported          = AspmA.Bits.L0sSupported;
    Combined.Bits.L1Supported           = AspmA.Bits.L1Supported;
    Combined.Bits.LinkL0sExitLatency    = AspmA.Bits.LinkL0sExitLatency;
    Combined.Bits.LinkL1ExitLatencyUs   = AspmA.Bits.LinkL1ExitLatencyUs;
    Combined.Bits.L0sAcceptableLatency  = MIN (AspmA.Bits.L0sAcceptableLatency, AspmB.Bits.L0sAcceptableLatency);
    Combined.Bits.L1AcceptableLatencyUs = MIN (AspmA.Bits.L1AcceptableLatencyUs, AspmB.Bits.L1AcceptableLatencyUs);
  }
  DEBUG ((DEBUG_INFO, "CombineAspm %x:%x -> %x\n", AspmA.Bits.L1AcceptableLatencyUs, AspmB.Bits.L1AcceptableLatencyUs, Combined.Bits.L1AcceptableLatencyUs));
  return Combined;
}

/**
  Checks if L1 can be enabled on given link, according to ASPM parameters of that link

  @param[in] Aspm            set of parameters describing this link and endpoint devices below it

  @retval    TRUE if L1 can be enabled
**/
STATIC
BOOLEAN
IsL1Allowed (
  ASPM_CAPS Aspm
  )
{
  return (Aspm.Bits.L1Supported && (Aspm.Bits.L1AcceptableLatencyUs >= Aspm.Bits.LinkL1ExitLatencyUs));
}

/**
  Checks if L0s can be enabled on given link, according to ASPM parameters of that link

  @param[in] Aspm            set of parameters describing this link and endpoint devices below it

  @retval    TRUE if L0s can be enabled
**/
STATIC
BOOLEAN
IsL0sAllowed (
  ASPM_CAPS Aspm
  )
{
  return (Aspm.Bits.L0sSupported && (Aspm.Bits.L0sAcceptableLatency >= Aspm.Bits.LinkL0sExitLatency));
}

/**
  Enables L0s and L1 for given port, if possible.
  L0s/L1 can be enabled if it's supported on both sides of a link and if link's latency doesn't exceed
  acceptable latency of any endpoint below this link

  @param[in] Base            device's base address
  @param[in] Aspm            set of parameters describing this link and endpoint devices below it
**/
STATIC
VOID
SetAspm (
  SBDF      Sbdf,
  ASPM_CAPS Aspm
  )
{
  UINT16 DataOr;

  DataOr = 0;
  if (IsL0sAllowed (Aspm)) {
    DataOr |= V_PCIE_LCTL_ASPM_L0S;
  }
  if (IsL1Allowed (Aspm)) {
    DataOr |= V_PCIE_LCTL_ASPM_L1;
  }
  DEBUG ((DEBUG_INFO, "SetAspm on %02x:%02x:%02x to %d\n", Sbdf.Bus,Sbdf.Dev,Sbdf.Func, DataOr));
  PciSegmentAndThenOr16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_LCTL_OFFSET, (UINT16)~B_PCIE_LCTL_ASPM, DataOr);
}

/**
  Configures Dynamic Link Throttling on the root port

  @param[in] RpSbdf   segment:bus:device:function coordinates of rootport
**/
STATIC
VOID
DynamicLinkThrottlingEnable (
  SBDF RpSbdf
  )
{
  UINT16  Data16;
  BOOLEAN L0sEnabled;

  Data16 = PciSegmentRead16 (SbdfToBase (RpSbdf) + RpSbdf.PcieCap + R_PCIE_LCTL_OFFSET);
  L0sEnabled = ((Data16 & B_PCIE_LCTL_ASPM ) == V_PCIE_LCTL_ASPM_L0S || (Data16 & B_PCIE_LCTL_ASPM ) == V_PCIE_LCTL_ASPM_L0S_L1);
  if (L0sEnabled) {
    DEBUG ((DEBUG_INFO, "Enabling dynamic link throttling \n"));
    PciSegmentOr32 (SbdfToBase (RpSbdf) + RpSbdf.PcieCap + R_PCIE_CFG_TNPT, B_PCIE_CFG_TNPT_DRXLTE | B_PCIE_CFG_TNPT_DTXLTE);
  }
}

/**
  Remove device entry from a list and clear its bus assignment

  @param[in,out] Table    array of devices
**/
STATIC
VOID
ClearBusFromTable (
  SBDF_TABLE *Table
  )
{
  while (Table->Count > 0) {
    PciSegmentWrite32 (SbdfToBase (Table->Entry[Table->Count - 1]) + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, 0);
    Table->Count--;
  }
}

/**
  Enables L0s and/or L1 for PCIE links in the hierarchy below
  L0s/L1 can be enabled when both sides of a link support it and link latency is smaller than acceptable latency
  ASPM of a given link is independend from any other link (except 1ms L1 adjustment, read below), so it's possible to
  have a hierarchy when RP link has no ASPM but links below do.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] Depth                          How many links there are between this port and root complex
  @param[in] Override                       Pch Pcie devices OverrideTable

  @retval structure that describes acceptable latencies of all endpoints below plus ASPM parameters of last link
**/
STATIC
ASPM_CAPS
RecursiveAspmConfiguration (
  SBDF           Sbdf,
  UINT8          Depth,
  OVERRIDE_TABLE *Override
  )
{
  SBDF         ChildSbdf;
  ASPM_CAPS    MyAspm;
  ASPM_CAPS    ChildAspm;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveAspmConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  //
  // On way down:
  //   pass number of links traversed; increase it per upstream port visited (not endpoint)
  // On way up:
  //   EndPoint: read Acceptable Latencies; subtract Depth From L1AcceptableLat to account for "1us per switch additional delay"
  //   Downstreamport: AND L0s/L1 caps; calculate LinkLatency; enable L0s/L1 if supported and if acceptable latency is bigger than link latency;
  //     if L1 not enabled, add back 1us to Acceptable Latency to cancel earlier Depth subtraction
  //   UpstreamPort: calculate minimum of below Acceptable Latencies; return that, with upper link's Latency and L0s/L1 support
  //
  DevType = GetDeviceType(Sbdf);
  if (DevType == DevTypePcieUpstream) {
    Depth++;
  }
  MyAspm = GetAspmCaps (Sbdf);
  //
  // Get ASPM L0s and L1 override
  //
  if (Override != NULL) {
    GetOverrideAspm (Sbdf, &MyAspm, Override);
  }
  if (DevType == DevTypePcieEndpoint) {
    //
    // Every switch between endpoint and CPU introduces 1us additional latency on L1 exit. This is reflected by
    // subtracting 1us per switch from endpoint's acceptable L1 latency.
    // In case L1 doesn't get enabled in one of switches, that 1us will be added back.
    // This calculation is not precise. It ignores that some switches' added delay may be shadowed by
    // other links' exit latency. But it guarantees that acceptable latency won't be exceeded and is simple
    // enough to perform in a single iteration without backtracking.
    //
    return PatchL1AcceptableLatency (MyAspm, (-1 * Depth));
  }
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildAspm = RecursiveAspmConfiguration (ChildSbdf, Depth, Override);
      MyAspm = CombineAspm (MyAspm, ChildAspm, (DevType == DevTypePcieDownstream));
    }
    if (DevType == DevTypePcieDownstream) {
      SetAspm (Sbdf, MyAspm);
      //
      // ASPM config must be consistent across all functions of a device. That's why there's while loop.
      //
      while (FindNextPcieChild (DevType, &ChildSbdf)) {
        SetAspm (ChildSbdf, MyAspm);
      }
      if (!IsL1Allowed (MyAspm)) {
        MyAspm = PatchL1AcceptableLatency (MyAspm, 1);
      }
    }
  }
  return MyAspm;
}

/**
  Enables L1 substates for PCIE links in the hierarchy below
  L1.1 / L1.2 can be enabled if both sides of a link support it.

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @retval  structure that describes L1ss capabilities of the device
**/
STATIC
L1SS_CAPS
RecursiveL1ssConfiguration (
  SBDF           Sbdf,
  OVERRIDE_TABLE *Override
  )
{
  UINT64  Base;
  SBDF    ChildSbdf;
  L1SS_CAPS CombinedCaps;
  L1SS_CAPS ChildCaps;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveL1ssConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  Base = SbdfToBase (Sbdf);
  //
  // On way down:
  //   do nothing
  // On way up:
  //   In downstream ports, combine L1ss capabilities of that port and device behind it, then enable L1.1 and/or L1.2 if possible
  //   Return L1ss capabilities
  //
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildCaps = RecursiveL1ssConfiguration (ChildSbdf, Override);
      if (DevType == DevTypePcieDownstream && ChildSbdf.Func == 0) {
        CombinedCaps = CombineL1ss (GetL1ssCaps (Base, Override), ChildCaps);
        SetL1ss (Sbdf, CombinedCaps, Override);
        SetL1ss (ChildSbdf, CombinedCaps, Override);
      }
    }
  }
  return GetL1ssCaps (Base, Override);
}

/**
  Checks if there is an IoAPIC device in the PCIe hierarchy.
  If one is found, this function doesn't check for more and returns

  @param[in] BusLimit                       maximum Bus number that can be assigned below this port
  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @retval  TRUE if IoAPIC device was found
**/
STATIC
BOOLEAN
RecursiveIoApicCheck (
  SBDF       Sbdf
  )
{
  SBDF         ChildSbdf;
  UINT8        IoApicPresent;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveIoApicCheck %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  IoApicPresent = FALSE;

  if (IsIoApicDevice (SbdfToBase (Sbdf))) {
    DEBUG ((DEBUG_INFO, "IoApicFound @%x:%x:%x:%x\n", Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return TRUE;
  }
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      IoApicPresent = RecursiveIoApicCheck (ChildSbdf);
      if (IoApicPresent) {
        break;
      }
    }
  }
  DEBUG ((DEBUG_INFO, "IoApic status %d @%x:%x:%x:%x\n", IoApicPresent, Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
  return IoApicPresent;
}

/**
  Calculates Maximum Payload Size supported by PCIe hierarchy.
  Starting from a device, it finds the minimum MPS supported by devices below it.
  There are many valid strategies for setting MPS. This implementation chooses
  one that is safest, but doesn't guarantee maximum performance:
    Find minimum MPS under given rootport, then program that minimum value everywhere below that rootport

  @param[in] BusLimit                       maximum Bus number that can be assigned below this port
  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @retval  MPS supported by PCIe hierarchy, calculated as MIN(MPS of all devices below)
**/
STATIC
UINT8
RecursiveMpsCheck (
  SBDF       Sbdf
  )
{
  SBDF         ChildSbdf;
  UINT8        MyMps;
  UINT8        SubtreeMps;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveMpsCheck %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  MyMps = GetMps (Sbdf);
  if (MyMps == 0) {
    return MyMps;
  }
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      SubtreeMps = RecursiveMpsCheck (ChildSbdf);
      MyMps = MIN(MyMps, SubtreeMps);
    }
  }
  return MyMps;
}

/**
  Sets Maximum Payload Size in PCIe hierarchy.
  Starting from a device, it programs the same MPS value to it and all devices below it.
  There are many valid strategies for setting MPS. This implementation chooses
  one that is safest, but doesn't guarantee maximum performance:
    Find minimum MPS under given rootport, then program that minimum value everywhere below that rootport

  @param[in] BusLimit                       maximum Bus number that can be assigned below this port
  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] Mps                            Maximum Payload Size to be programmed
**/
STATIC
VOID
RecursiveMpsConfiguration (
  SBDF       Sbdf,
  UINT8      Mps
  )
{
  SBDF    ChildSbdf;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveMpsConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      RecursiveMpsConfiguration (ChildSbdf, Mps);
    }
  }
  SetMps (Sbdf, Mps);
}

/**
  Sets Enable Clock Power Management bit for devices that support it.
  A device supports CPM only if all function of this device report CPM support.
  Downstream ports never report CPM capability, so it's only relevant for upstream ports.
  When this function executes on upstream component, it will check CPM & set ECPM of downstream component
  When this function executes on downstream component, all devices below it are guaranteed to
  return CPM=0 so it will do nothing

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device

  @retval TRUE = this device supports CPM, FALSE = it doesn't
**/
STATIC
BOOLEAN
RecursiveCpmConfiguration (
  SBDF       Sbdf
  )
{
  SBDF         ChildSbdf;
  BOOLEAN      ChildCpm;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveCpmConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  ChildCpm          = FALSE;
  ChildSbdf.PcieCap = 0;

  if (HasChildBus (Sbdf, &ChildSbdf)) {
    ChildCpm = TRUE;
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildCpm &= RecursiveCpmConfiguration (ChildSbdf);
    }
    if (ChildCpm) {
      while (FindNextPcieChild (DevType, &ChildSbdf)) {
        EnableCpm (ChildSbdf);
      }
    }
  }
  return IsCpmSupported (Sbdf);
}

/**
  Sets Common Clock Configuration bit for devices that share common clock across link
  Devices on both sides of a PCIE link share common clock if both upstream component
  and function 0 of downstream component report Slot Clock Configuration bit = 1.
  When this function executes on upstream component, it checks SCC of both sides of the link
  If they both support it, sets CCC for both sides (this means all functions of downstream component)
  When this function executes on downstream component, it only returns SCC capability

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] WaitForRetrain                 decides if this function should busy-wait for link retrain

  @retval TRUE = this device supports SCC, FALSE = it doesn't
**/
STATIC
BOOLEAN
RecursiveCccConfiguration (
  SBDF       Sbdf,
  BOOLEAN    WaitForRetrain
  )
{
  UINT64       Base;
  SBDF         ChildSbdf;
  BOOLEAN      MyScc;
  BOOLEAN      ChildScc;
  BOOLEAN      LinkScc;
  PCI_DEV_TYPE DevType;

  ChildScc          = 0;
  ChildSbdf.PcieCap = 0;
  Base     = SbdfToBase (Sbdf);
  MyScc    = GetScc (SbdfToBase (Sbdf), (UINT8)Sbdf.PcieCap);
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildScc |= RecursiveCccConfiguration (ChildSbdf, WaitForRetrain);
    }
    if (DevType == DevTypePcieDownstream) {
      LinkScc = MyScc & ChildScc;
      if (LinkScc) {
        EnableCcc (SbdfToBase (Sbdf), (UINT8)Sbdf.PcieCap);
        while (FindNextPcieChild (DevType, &ChildSbdf)) {
          EnableCcc (SbdfToBase (ChildSbdf), (UINT8)ChildSbdf.PcieCap);
        }
        RetrainLink(Base, (UINT8)Sbdf.PcieCap, WaitForRetrain);
      }
    }
  }
  return MyScc;
}

/**
  Sets Common Clock Configuration bit for devices that share common clock across link
  Devices on both sides of a PCIE link share common clock if both upstream component
  and function 0 of downstream component report Slot Clock Configuration bit = 1.
  When this function executes on upstream component, it checks SCC of both sides of the link
  If they both support it, sets CCC for both sides (this means all functions of downstream component)
  When this function executes on downstream component, it only returns SCC capability

  @param[in] *pInst               Point of pInst
  @retval  IpCsiStsSuccess        The function completes successfully
**/
IP_CSI_STATUS
PcieRpDownStreamCcConfiguration (
  IP_PCIE_INST   *pInst
  )
{
  BOOLEAN         RpScc;
  BOOLEAN         ChildScc;
  SBDF            RpSbdf;
  SBDF            ChildSbdf;
  IP_CSI_STATUS   Status;
  PCI_DEV_TYPE    DevType;
  IP_WR_REG_INFO  *RegInfo;
  BOOLEAN         WaitForRetrain;
  SBDF_TABLE      BridgeCleanupList;

  DEBUG ((DEBUG_INFO, "%a (), RpIndex = %d\n", __FUNCTION__, pInst->RpIndex));

  if ((UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, 0, IpWrRegFlagSize16Bits) == 0xFFFF) {
    return IpCsiStsError;
  }

  RegInfo        = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;
  RpSbdf.Seg     = RegInfo->RegType.Pci.Seg;
  RpSbdf.Bus     = RegInfo->RegType.Pci.Bus;
  RpSbdf.Dev     = RegInfo->RegType.Pci.Dev;
  RpSbdf.Func    = RegInfo->RegType.Pci.Fun;
  RpSbdf.PcieCap = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);

  BridgeCleanupList.Count = 0;
  RecursiveBusAssignment (RpSbdf, pInst->PrivateConfig.BusMin, pInst->PrivateConfig.BusMax, &BridgeCleanupList);

  RpScc    = !! IpPcieGetCapability (pInst, pInst->RpIndex, IpPcieFeatIdCcc, &Status);
  ChildScc = 0;
  WaitForRetrain = TRUE;
  if (HasChildBus (RpSbdf, &ChildSbdf)) {
    DevType  = GetDeviceType (RpSbdf);
    if (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildScc = GetScc (SbdfToBase (ChildSbdf), (UINT8)ChildSbdf.PcieCap);
      if (DevType == DevTypePcieDownstream && (RpScc & ChildScc)) {
        RecursiveCccConfiguration (ChildSbdf, WaitForRetrain);
      }
    }
  }

  if (RpScc & ChildScc) {
    Status = IpPcieSetControl (pInst, pInst->RpIndex, IpPcieFeatIdCcc, (RpScc & ChildScc));
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      EnableCcc (SbdfToBase (ChildSbdf), (UINT8)ChildSbdf.PcieCap);
    }
    RetrainLink (SbdfToBase (RpSbdf), (UINT8)RpSbdf.PcieCap, WaitForRetrain);
  }

  ClearBusFromTable (&BridgeCleanupList);
  return IpCsiStsSuccess;
}

/**
  Configures Latency Tolerance Reporting in given device and in PCIe tree below it.
  This function configures Maximum LTR and enables LTR mechanism. It visits devices using depth-first search
  and skips branches behind devices which do not support LTR.
  Maximum LTR:
    This function will set LTR's upper bound for every visited device. Max LTR value is provided as a parameter
  Enable LTR:
    LTR should be enabled top-to-bottom in every visited device that supports LTR. This function does not
    iterate down behind devices with no LTR support. In effect, LTR will be enabled in given device if that device
    and all devices above it on the way to RootComplex do support LTR.

  This function expects that bridges have bus numbers already configured

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] LtrLimit                       Ltr to be programmed to every endpoint

  @retval MaxLTR programmed in this device
**/
STATIC
VOID
RecursiveLtrConfiguration (
  SBDF       Sbdf,
  LTR_LIMIT  LtrLimit
  )
{
  UINT64  Base;
  SBDF    ChildSbdf;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveLtrConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  Base = SbdfToBase (Sbdf);

  if (!IsLtrCapable (Sbdf)) {
    DEBUG ((DEBUG_INFO, "Not LtrCapable %02x:%02x:%02x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return;
  }
  EnableLtr (Sbdf);
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      RecursiveLtrConfiguration (ChildSbdf, LtrLimit);
    }
  }
  SetLtrLimit (Base, LtrLimit);
}

BOOLEAN
IpPcieRecursiveIsLtrCapable (
  SBDF       Sbdf
  )
{
  SBDF            ChildSbdf;
  PCI_DEV_TYPE    DevType;
  BOOLEAN         LtrCapable;
  BOOLEAN         TreeLtrCapable;

  DEBUG ((DEBUG_INFO, "IpPcieRecursiveIsLtrCapable %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  LtrCapable = IsLtrCapable (Sbdf);
  if (LtrCapable == 0) {
    DEBUG ((DEBUG_INFO, "Not LtrCapable %02x:%02x:%02x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));
    return LtrCapable;
  }
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      TreeLtrCapable = IpPcieRecursiveIsLtrCapable (ChildSbdf);
      LtrCapable &= TreeLtrCapable;
    }
  }
  return LtrCapable;
}

/**
  Checks to see device is PTM Capable

  @param[in] Sbdf    device's segment:bus:device:function coordinates

  @retval TRUE = PTM Capability found, FALSE = Not PTM capable
**/
STATIC
BOOLEAN
IsPtmCapable (
  SBDF       Sbdf
  )
{
  UINT16     CapHeaderOffset;

  CapHeaderOffset = PcieFindExtendedCapId ((UINT8) Sbdf.Seg, (UINT8) Sbdf.Bus, (UINT8) Sbdf.Dev, (UINT8) Sbdf.Func, V_PCIE_EX_PTM_CID);

  return (CapHeaderOffset != 0);
}

/**
  Get PTM Capability register from PCIe Extended Capability Space.

  @param[in] Sbdf                device's segment:bus:device:function coordinates
  @param[in] PtmCapHeaderOffset  PTM Capability Header Offset

  @retval LocalPtm       Returns PTM Capability.
                         If Device is not PTM capable then PTM Capability is zeroed out.
**/
STATIC
PTM_CAPS
GetPtmCapability (
  SBDF      Sbdf,
  UINT16    PtmCapHeaderOffset
  )
{
  PTM_CAPS PtmCaps;

  PtmCaps.Uint32 = 0;

  if (PtmCapHeaderOffset != 0) {
    PtmCaps.Uint32 = PciSegmentRead32 (SbdfToBase (Sbdf) + PtmCapHeaderOffset + R_PCIE_EX_PTMCAP_OFFSET);
  }

  return PtmCaps;
}

/**
  Get PTM Control register from PCIe Extended Capability Space.

  @param[in] Sbdf                device's segment:bus:device:function coordinates
  @param[in] PtmCapHeaderOffset  PTM Capability Header Offset

  @retval LocalPtm       Returns PTM Control.
                         If Device is not PTM capable then PTM Control is zeroed out.
**/
STATIC
PTM_CTRL
GetPtmControl (
  SBDF      Sbdf,
  UINT16    PtmCapHeaderOffset
  )
{
  PTM_CTRL PtmCtrl;

  PtmCtrl.Uint32 = 0;

  if (PtmCapHeaderOffset != 0) {
    PtmCtrl.Uint32 = PciSegmentRead32 (SbdfToBase (Sbdf) + PtmCapHeaderOffset + R_PCIE_EX_PTMCTL_OFFSET);
  }

  return PtmCtrl;
}

/**
  Set PTM Control register in the PCIe Extended Capability Space.

  @param[in] Sbdf                device's segment:bus:device:function coordinates
  @param[in] PtmCapHeaderOffset  PTM Capability Header Offset
  @param[in] PtmCtrl             PTM Control Register
**/
STATIC
VOID
SetPtmControl (
  SBDF      Sbdf,
  UINT16    PtmCapHeaderOffset,
  PTM_CTRL  PtmCtrl
  )
{
  if (PtmCapHeaderOffset != 0) {
    PciSegmentWrite32 (SbdfToBase (Sbdf) + PtmCapHeaderOffset + R_PCIE_EX_PTMCTL_OFFSET, PtmCtrl.Uint32);
  }
}

/**
  Enable PTM on device's control register. Set the Effective Clock Granularity.

  @param[in]     Sbdf                 device's segment:bus:device:function coordinates
  @param[in out] EffectiveGranularity Effective Clock Granularity of the PTM hierarchy
  @param[in out] PtmHierarchy         Indicates if the current device is within a PTM hierarchy
**/
STATIC
VOID
SetPtm (
  IN     SBDF       Sbdf,
  IN OUT UINT8      *EffectiveGranularity,
  IN OUT BOOLEAN    *PtmHierarchy
  )
{
  PTM_CTRL CurrentPtmCtrl;
  PTM_CAPS CurrentPtmCaps;
  PTM_CTRL OrigPtmCtrl;
  UINT16   PtmCapHeaderOffset;

  PtmCapHeaderOffset = PcieFindExtendedCapId ((UINT8) Sbdf.Seg, (UINT8) Sbdf.Bus, (UINT8) Sbdf.Dev, (UINT8) Sbdf.Func, V_PCIE_EX_PTM_CID);
  CurrentPtmCtrl = GetPtmControl (Sbdf, PtmCapHeaderOffset);
  CurrentPtmCaps = GetPtmCapability (Sbdf, PtmCapHeaderOffset);

  OrigPtmCtrl.Uint32 = CurrentPtmCtrl.Uint32;

  if ( (*PtmHierarchy == FALSE) && CurrentPtmCaps.Bits.RootCapable) {
    // Select device as PTM Root if PTM Root is not selected.
    CurrentPtmCtrl.Bits.RootSelect = TRUE;
    *EffectiveGranularity = (UINT8) CurrentPtmCaps.Bits.LocalClockGranularity;
    *PtmHierarchy = TRUE;
  }

  if (*PtmHierarchy == TRUE) {
    // Enable PTM if device is part of a ptm hierarchy
    CurrentPtmCtrl.Bits.Enable = TRUE;

    if (CurrentPtmCaps.Bits.RequesterCapable) {
      // Set Effective Granularity if PTM device is Requester roles.
      CurrentPtmCtrl.Bits.EffectiveGranularity = *EffectiveGranularity;
    }
  }

  if (OrigPtmCtrl.Uint32 != CurrentPtmCtrl.Uint32) {
    SetPtmControl (Sbdf, PtmCapHeaderOffset, CurrentPtmCtrl);
  }

  // Update EffectiveGranularity.
  // Set to zero if 1 or more switches between the root and endpoint report local granularity = 0.
  // Otherwise set to the max local granularity of the hierarchy.
  if ( ((CurrentPtmCaps.Bits.LocalClockGranularity == 0) && CurrentPtmCaps.Bits.RequesterCapable) ||
       (*EffectiveGranularity  == 0) ) {
    *EffectiveGranularity = 0;
  } else {
    *EffectiveGranularity = MAX (*EffectiveGranularity, (UINT8) CurrentPtmCaps.Bits.LocalClockGranularity);
  }
}

/**
  Configures PTM hierarchies by searching for Endpoints and Upstream Switches
  that are PTM capable. Each PTM device role is identified and configured accordingly.
  PTM root capable devices are selected as PTM root if the device is not already in a
  PTM hierarchy.
  PTM capable Root Ports must be configured before calling this function.
  @note: This function has not been tested with switches.

  @param[in] Sbdf                 Address of curretly visited Pcie device
  @param[in] EffectiveGranularity The largest Clock Granularity from Upstream Devices
  @param[in] PtmHierarchy         TRUE = Device in a PTM Hierarchy, FALSE = Not in PTM Hierarchy
**/
STATIC
VOID
RecursivePtmConfiguration (
  SBDF          Sbdf,
  UINT8         EffectiveGranularity,
  BOOLEAN       PtmHierarchy
  )
{
  SBDF         ChildSbdf;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursivePtmConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  DevType = GetDeviceType (Sbdf);

  if (IsPtmCapable (Sbdf)) {
    //
    // Enable PTM for PTM Capable devices (Endpoints, Upstream Switch, and Root Ports).
    // @Note: Switches have not been tested.
    //
    SetPtm (Sbdf, &EffectiveGranularity, &PtmHierarchy);
  } else if (!IsPtmCapable (Sbdf) && (DevType == DevTypePcieUpstream) ) {
    //
    // Non-PTM UpStream Switch Ports breaks the PTM Hierarchy.
    // No other downstream PTM devices should be PTM enabled until a PTM Root capable device is selected.
    //
    PtmHierarchy = FALSE;
    EffectiveGranularity = 0;
  }

  if (HasChildBus (Sbdf, &ChildSbdf)) {
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      RecursivePtmConfiguration (ChildSbdf, EffectiveGranularity, PtmHierarchy);
    }
  }
}

/**
   Set Endpoint Error Flags

  @param[in] SBDF  Sbdf
  @retval  VOID
**/
VOID
SetEpErroreFlags (
  SBDF          Sbdf
)
{
  UINT8                   EpPcieCapPtr;
  UINT8                   Revision;
  UINT16                  VendorId;
  UINT16                  DeviceId;
  UINT64                  EpBase;
  DCTL_PCIE_CFG_STRUCT    DctlCfg;

  VendorId = 0xFFFF;
  DeviceId = 0xFFFF;
  EpBase = SbdfToBase (Sbdf);
  EpPcieCapPtr = 0;

  //
  // A config write is required in order for the device to re-capture the Bus number,
  // according to PCI Express Base Specification, 2.2.6.2
  // Write to a read-only register VendorID to not cause any side effects.
  //
  PciSegmentWrite16(EpBase + PCI_VENDOR_ID_OFFSET, 0x00);

  VendorId = PciSegmentRead16 (EpBase + PCI_VENDOR_ID_OFFSET);
  DeviceId = PciSegmentRead16 (EpBase + PCI_DEVICE_ID_OFFSET);
  Revision = PciSegmentRead8  (EpBase + PCI_REVISION_ID_OFFSET);
  DEBUG ((DEBUG_INFO, "VendorId=%x, DeviceId=%x, Revision=%x\n", VendorId, DeviceId, Revision));

  if (VendorId != 0xFFFF) {
    EpPcieCapPtr = PcieBaseFindCapId (EpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);
  }

  DctlCfg.Bits.ure = 1;
  DctlCfg.Bits.fee = 1;
  DctlCfg.Bits.nfe = 1;
  DctlCfg.Bits.cee = 1;
  PciSegmentWrite16((EpBase + EpPcieCapPtr + R_PCIE_DCTL_OFFSET), DctlCfg.Data);
  DEBUG ((DEBUG_INFO, "DCTL=%x\n", PciSegmentRead16(EpBase + EpPcieCapPtr + R_PCIE_DCTL_OFFSET)));
}

/**
   Recursively Set Endpoint Error Flags

  @param[in] SBDF  Sbdf
  @retval  VOID
**/
STATIC VOID
RecursiveSetEpErrorFlags (
  SBDF          Sbdf
)
{
  SBDF    ChildSbdf;
  PCI_DEV_TYPE DevType;

  DEBUG ((DEBUG_INFO, "RecursiveSetEpErrorFlag %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      RecursiveSetEpErrorFlags (ChildSbdf);
    }
  }
  SetEpErroreFlags (Sbdf);
}

/**
  Returns combination of 10 bit tag capabilities of two devices on both ends to determine 10 bit tag capabilities

  @param[in] TenBitTagA      10 bit tag capabilities of first device
  @param[in] TenBitTagA      10 bit tag capabilities of second device

  @retval TenBitTag  configuration to be programmed
**/
STATIC
TENBITTAG_CAPS
Combine10BitTag (
  TENBITTAG_CAPS TenBitTagA,
  TENBITTAG_CAPS TenBitTagB
  )
{
  TENBITTAG_CAPS Combined;

  Combined.Bits.RequesterSupported   = TenBitTagA.Bits.CompleterSupported && TenBitTagB.Bits.RequesterSupported;
  Combined.Bits.CompleterSupported   = TenBitTagA.Bits.RequesterSupported && TenBitTagB.Bits.CompleterSupported;
  return Combined;
}

/**
  Returns 10 bit tag capabilities of a device

  @param[in] Sbdf segment:bus:device:function coordinates of a device

  @retval TenBitTag  configuration to be programmed
**/
STATIC
TENBITTAG_CAPS
Get10BitTagCaps (
  SBDF           Sbdf
  )
{
  TENBITTAG_CAPS Capabilities = {0};
  UINT32         CapsRegister;

  if (Sbdf.PcieCap == 0) {
    return Capabilities;
  }

  CapsRegister = PciSegmentRead32 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCAP2_OFFSET);

  Capabilities.Bits.RequesterSupported = !!(CapsRegister & B_PCIE_DCAP2_PX10BTRS);
  Capabilities.Bits.CompleterSupported = !!(CapsRegister & B_PCIE_DCAP2_PX10BTCS);
  return Capabilities;
}

/**
  Configures 10 bit tag in a device

  @param[in] Sbdf       segment:bus:device:function coordinates of a device
  @param[in] TenBitTag  configuration to be programmed
**/
STATIC
VOID
Set10BitTag (
  SBDF           Sbdf,
  TENBITTAG_CAPS TenBitTag
  )
{
  UINT16    CtrlRegister;
  UINT16    Ctrl2Register;
  UINT32    CapRegister;

  CtrlRegister = 0;
  Ctrl2Register = 0;
  CapRegister = 0;

  if (Sbdf.PcieCap == 0) {
    return;
  }

  CapRegister =   PciSegmentRead32 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCAP_OFFSET);

  if (CapRegister & B_PCIE_DCAP_ETFS) {
  CtrlRegister |= B_PCIE_DCTL_ETFE;
  PciSegmentWrite16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCTL_OFFSET, CtrlRegister);
  }

  Ctrl2Register |= B_PCIE_DCTL2_PX10BTRE;
  PciSegmentWrite16 (SbdfToBase (Sbdf) + Sbdf.PcieCap + R_PCIE_DCTL2_OFFSET, Ctrl2Register);
}

/**
  Program registers to Support 10-bit Tag
  Only if given Root Port and tree below it supports 10BitTag Completer/Requester

  @param[in] Sbdf        segment:bus:device:function coordinates of a device

  @retval TENBITTAG_CAPS structure that describes L1ss capabilities of the device
**/
STATIC
TENBITTAG_CAPS
Recursive10BitTagConfiguration (
  SBDF           Sbdf
  )
{
  SBDF           ChildSbdf;
  TENBITTAG_CAPS CombinedCaps;
  TENBITTAG_CAPS ChildCaps;
  PCI_DEV_TYPE   DevType;

  DEBUG ((DEBUG_INFO, "Recursive10BitTagConfiguration %x:%x:%x\n", Sbdf.Bus, Sbdf.Dev, Sbdf.Func));

  //
  // On way down:
  //   do nothing
  // On way up:
  //   
  //
  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildCaps = Recursive10BitTagConfiguration (ChildSbdf);
      if (DevType == DevTypePcieDownstream) {
        CombinedCaps = Combine10BitTag (Get10BitTagCaps (Sbdf), ChildCaps);
        if (CombinedCaps.Bits.RequesterSupported) {
          Set10BitTag (ChildSbdf, CombinedCaps);
        }
      }
    }
  }
  return Get10BitTagCaps (Sbdf);
}

/**
  This function enables MultiVc support

  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] ExtVc                          Multi Virtual channel capability offset and count
  @param[in] TcVcMap                        TC VC map
  @param[in] LinkVcCount                    Number of channels supported by the link
**/
VOID
EnableMultiVc (
  IN SBDF              Sbfd,
  IN MULTI_VC_SUPPORT  ExtVc,
  IN TC_VC_MAP         *TcVcMap,
  IN UINT32            LinkVcCount
  )
{
  UINT64       PcieBase;
  UINT32       VcIndex;
  UINT32       VcCtrlReg;

  PcieBase  = PCI_SEGMENT_LIB_ADDRESS (Sbfd.Seg, Sbfd.Bus, Sbfd.Dev, Sbfd.Func, 0);

  DEBUG ((DEBUG_INFO, "Programming Multi Virtual Channel configuration for BDF %x|%x|%x\n", Sbfd.Bus, Sbfd.Dev, Sbfd.Func));
  VcCtrlReg = VC0_CTRL_REG;
  for (VcIndex = 0; VcIndex < VC_MAX_NUM && VcIndex < (LinkVcCount + 1); VcIndex++) {
    VcCtrlReg += VcIndex * VCX_CTRL_REG_OFFSET;
    PciSegmentWrite8 (PcieBase + ExtVc.CapOffset + VcCtrlReg, TcVcMap->TcVcMap[VcIndex]);
    DEBUG ((DEBUG_INFO, "TcVc[%d] = %X\n", VcIndex, PciSegmentRead8 (PcieBase + VcCtrlReg)));
    if (VcIndex != 0) {
      PciSegmentAndThenOr32 (PcieBase + ExtVc.CapOffset + VcCtrlReg, (UINT32) ~(VC_ID), (VcIndex << 24) | VC_EN);
    }
  }
}

/**
  This function checks if both root port and end point supports MultiVC and enables it only if both supports MultiVc

  @param[in] Sbdf     Address of currently visited PCIe device
  @param[in] TcVcMap  TC VC map

  @return    Multi Virtual channel capability offset and count supported by the Root Port
**/
MULTI_VC_SUPPORT
RecursiveMultiVcConfiguration (
  IN SBDF       Sbdf,
  IN TC_VC_MAP  *TcVcMap
  )
{
  UINT64            PcieBase;
  UINT8             LinkVcCount;
  SBDF              ChildSbdf;
  PCI_DEV_TYPE      DevType;
  MULTI_VC_SUPPORT  MyExtVc;
  MULTI_VC_SUPPORT  ChildExtVc;

  PcieBase           = PCI_SEGMENT_LIB_ADDRESS (Sbdf.Seg, Sbdf.Bus, Sbdf.Dev, Sbdf.Func, 0);
  MyExtVc.ExtVcCount = 0;
  //
  // Check for optional Multi Function Virtual Channel Capability ID
  //
  MyExtVc.CapOffset = PcieBaseFindExtendedCapId (PcieBase, MFVC_CAP_ID);
  if (MyExtVc.CapOffset == 0) {
    MyExtVc.CapOffset  = PcieBaseFindExtendedCapId (PcieBase, VC_CAP_ID);
  }

  if (MyExtVc.CapOffset != 0) {
    MyExtVc.ExtVcCount = (PciSegmentRead8 (PcieBase + MyExtVc.CapOffset + VC_CAP_REG) & B_PCIE_CFG_PVCCR1_EVCC);
  } else {
    DEBUG ((DEBUG_INFO, "MVC not supported\n"));
  }

  if (HasChildBus (Sbdf, &ChildSbdf)) {
    DevType = GetDeviceType (Sbdf);
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildExtVc  = RecursiveMultiVcConfiguration (ChildSbdf, TcVcMap);
      LinkVcCount = (MyExtVc.ExtVcCount > ChildExtVc.ExtVcCount) ? ChildExtVc.ExtVcCount : MyExtVc.ExtVcCount;
      if (ChildExtVc.CapOffset != 0) {
        EnableMultiVc (Sbdf, MyExtVc, TcVcMap, LinkVcCount);
        EnableMultiVc (ChildSbdf, ChildExtVc, TcVcMap, LinkVcCount);
      }
    }
  }

  return MyExtVc;
}

/**
  Multi VC configuration.

  @param[in]  Sbdf     SBDF
  @param[in]  BusMin   minimum bus
  @param[in]  BusMax   maximum bus
  @param[in]  TcVcMap  TC/VC map
**/
VOID
PcieRpMultiVcConfiguration (
  IP_PCIE_INST   *pInst,
  TC_VC_MAP      *TcVcMap
  )
{

  SBDF            RpSbdf;
  SBDF_TABLE      BridgeCleanupList;
  IP_WR_REG_INFO  *RegInfo;

  RegInfo        = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;
  RpSbdf.Seg     = RegInfo->RegType.Pci.Seg;
  RpSbdf.Bus     = RegInfo->RegType.Pci.Bus;
  RpSbdf.Dev     = RegInfo->RegType.Pci.Dev;
  RpSbdf.Func    = RegInfo->RegType.Pci.Fun;
  RpSbdf.PcieCap = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);

  DEBUG ((DEBUG_INFO, "MultiVcConfiguration for %x:%x:%x\n", RpSbdf.Bus, RpSbdf.Dev, RpSbdf.Func));

  BridgeCleanupList.Count = 0;
  RecursiveBusAssignment (RpSbdf, pInst->PrivateConfig.BusMin, pInst->PrivateConfig.BusMax, &BridgeCleanupList);

  RecursiveMultiVcConfiguration (RpSbdf, TcVcMap);

  ClearBusFromTable (&BridgeCleanupList);
}

/**
   Configure Pcie configure the downstream bridges and end point

  @param[in] pInst  *pInst

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
PcieRpDownstreamConfiguration (
  IP_PCIE_INST   *pInst
  )
{
  UINT8           Mps;
  BOOLEAN         IoApicPresent;
  PTM_CAPS        PtmCaps;
  PTM_CTRL        PtmCtrl;
  SBDF            RpSbdf;
  SBDF_TABLE      BridgeCleanupList;
  SBDF            ChildSbdf;
  PCI_DEV_TYPE    DevType;
  IP_WR_REG_INFO  *RegInfo;
  IP_CSI_STATUS   Status;
  UINT8           EffectiveGranularity;
  BOOLEAN         PtmHierarchy;

  IoApicPresent         = FALSE;
  EffectiveGranularity = 0;
  PtmHierarchy         = FALSE;

  if ((UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, 0, IpWrRegFlagSize16Bits) == 0xFFFF) {
    return IpCsiStsError;
  }

  RegInfo        = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;
  RpSbdf.Seg     = RegInfo->RegType.Pci.Seg;
  RpSbdf.Bus     = RegInfo->RegType.Pci.Bus;
  RpSbdf.Dev     = RegInfo->RegType.Pci.Dev;
  RpSbdf.Func    = RegInfo->RegType.Pci.Fun;
  RpSbdf.PcieCap = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);

  BridgeCleanupList.Count = 0;
  RecursiveBusAssignment (RpSbdf, pInst->PrivateConfig.BusMin, pInst->PrivateConfig.BusMax, &BridgeCleanupList);

  Mps = (UINT8) IpPcieGetCapability (pInst, pInst->RpIndex, IpPcieFeatIdMps, &Status);

  DevType = GetDeviceType (RpSbdf);
  if (HasChildBus (RpSbdf, &ChildSbdf)) {
    if (FindNextPcieChild (DevType, &ChildSbdf)) {
      Mps = MIN (Mps, RecursiveMpsCheck (ChildSbdf));
      RecursiveMpsConfiguration (ChildSbdf, Mps);
    }
    Status = IpPcieSetControl (pInst, pInst->RpIndex, IpPcieFeatIdMps, Mps);
  }

  ///
  /// Set Error Reporting in the EP Device Control Register
  ///
  if(pInst->PcieRpCommonConfig.EpErrorReporting){
    DevType = GetDeviceType (RpSbdf);
    if (HasChildBus (RpSbdf, &ChildSbdf)) {
      if (FindNextPcieChild (DevType, &ChildSbdf)) {
        RecursiveSetEpErrorFlags(ChildSbdf);
      }
    }
  }

  //
  // Checks if there is an IoAPIC device in the PCIe hierarchy.
  // If one is found, Does not need to check further
  //
  IoApicPresent = !! IpPcieGetCapability (pInst, pInst->RpIndex, IpPcieFeatIdIoApic, &Status);

  if (IoApicPresent == FALSE) {
    if (HasChildBus (RpSbdf, &ChildSbdf)) {
      DevType = GetDeviceType (RpSbdf);
      while (FindNextPcieChild (DevType, &ChildSbdf)) {
        IoApicPresent = RecursiveIoApicCheck (ChildSbdf);
        if (IoApicPresent) {
          break;
        }
      }
    }
  }
  Status = IpPcieSetControl (pInst, pInst->RpIndex, IpPcieFeatIdIoApic, IoApicPresent);
  if (IoApicPresent) {
    PtlPcdPsfEnableRpEoiTarget (pInst->RpIndex);
  }

  PtmCaps.Uint32 = IpPcieGetCapability (pInst, pInst->RpIndex, IpPcieFeatIdPtm, &Status);
  if (PtmCaps.Uint32 != 0) {
    DevType = GetDeviceType (RpSbdf);
    if (HasChildBus (RpSbdf, &ChildSbdf)) {
      if (FindNextPcieChild (DevType, &ChildSbdf) && IsPtmCapable (ChildSbdf)) {
        DEBUG ((DEBUG_INFO, "Child supports PTM proceeding with PTM configuration\n"));
        PtmCtrl.Uint32 = 0;

        // @todo: Need to verify the flow
        if (PtmCaps.Bits.RootCapable) {
          // Select device as PTM Root if PTM Root is not selected.
          PtmCtrl.Bits.RootSelect = TRUE;
          PtmCtrl.Bits.Enable     = TRUE;
          EffectiveGranularity   = (UINT8) PtmCaps.Bits.LocalClockGranularity;
          PtmHierarchy           = TRUE;
          if (PtmCaps.Bits.RequesterCapable) {
            // Set Effective Granularity if PTM device is Requester roles.
            PtmCtrl.Bits.EffectiveGranularity = EffectiveGranularity;
          }
        }
        IpPcieSetControl (pInst, pInst->RpIndex, IpPcieFeatIdPtm, PtmCtrl.Uint32);
        RecursivePtmConfiguration (ChildSbdf, EffectiveGranularity, PtmHierarchy);
      }
    }
  }

  if (HasChildBus (RpSbdf, &ChildSbdf)) {
    DevType = GetDeviceType (RpSbdf);
    if (FindNextPcieChild (DevType, &ChildSbdf)) {
      Recursive10BitTagConfiguration (ChildSbdf);
    }
  }

  ClearBusFromTable (&BridgeCleanupList);
  return IpCsiStsSuccess;
}

/**
  Configure Downstream PM configuration

  @param[in] pInst  *pInst
  @param[in] AspmOverrideTableSize    size of override array
  @param[in] AspmOverrideTable        array of device that need exceptions in configuration

  @retval  IpCsiStsSuccess        The function completes successfully
  @retval  IpCsiStsErrorNullPtr   pInst was NULL
**/
IP_CSI_STATUS
IpPcieRpDownstreamPmConfiguration (
  IP_PCIE_INST                        *pInst,
  UINT32                              AspmOverrideTableSize,
  PCIE_DEVICE_OVERRIDE                *AspmOverrideTable
  )
{
  UINT32         IsLtrCapable;
  LTR_LIMIT      PolicyLtr;
  LTR_OVERRIDE   TreeLtr;
  LTR_OVR        ChildLtrOvr;
  OVERRIDE_TABLE PmOverrideTable;
  UINT64         RpBase;
  SBDF           RpSbdf;
  SBDF           ChildSbdf;
  SBDF_TABLE     BridgeCleanupList;
  PCI_DEV_TYPE   DevType;
  L1SS_CAPS      RpL1ss;
  L1SS_CAPS      ChildL1ss;
  L1SS_CAPS      CombinedCaps;
  IP_WR_REG_INFO *RegInfo;
  IP_CSI_STATUS  Status;
  ASPM_CAPS      RpAspm;
  ASPM_CAPS      TreeAspm;
  ASPM_CAPS      CombinedAspm;
  BOOLEAN        LtrCapable;

  if ((UINT16) IpWrRegRead (pInst->RegCntxt_Cfg_Pri, 0, IpWrRegFlagSize16Bits) == 0xFFFF) {
    return IpCsiStsErrorNullPtr;
  }

  PmOverrideTable.Size  = AspmOverrideTableSize;
  PmOverrideTable.Table = AspmOverrideTable;

  PolicyLtr.MaxNoSnoopLatencyScale = (pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency & 0x1c00) >> 10;
  PolicyLtr.MaxNoSnoopLatencyValue = pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxNoSnoopLatency & 0x3FF;
  PolicyLtr.MaxSnoopLatencyScale   = (pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency & 0x1c00) >> 10;
  PolicyLtr.MaxSnoopLatencyValue   = pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrMaxSnoopLatency & 0x3FF;

  RegInfo        = (IP_WR_REG_INFO*)(UINTN) pInst->RegCntxt_Cfg_Pri;
  RpSbdf.Seg     = RegInfo->RegType.Pci.Seg;
  RpSbdf.Bus     = RegInfo->RegType.Pci.Bus;
  RpSbdf.Dev     = RegInfo->RegType.Pci.Dev;
  RpSbdf.Func    = RegInfo->RegType.Pci.Fun;
  RpSbdf.PcieCap = IpPcieFindCapId (pInst, EFI_PCI_CAPABILITY_ID_PCIEXP);

  RpBase = PCI_SEGMENT_LIB_ADDRESS (RpSbdf.Seg, RpSbdf.Bus, RpSbdf.Dev, RpSbdf.Func, 0);
  //
  // This code could execute either before or after enumeration. If before, then buses would not yet be assigned to bridges,
  // making devices deeper in the hierarchy inaccessible.
  // RecursiveBusAssignment will scan whole PCie tree and assign bus numbers to uninitialized bridges, if there are any
  // List of such bridges will be kept in CleanupList, so that after PM programming is done, bus numbers can brought to original state
  //
  BridgeCleanupList.Count = 0;
  RecursiveBusAssignment (RpSbdf, pInst->PrivateConfig.BusMin, pInst->PrivateConfig.BusMax, &BridgeCleanupList);

  TreeLtr = RecursiveLtrOverrideCheck (RpSbdf, &PmOverrideTable);

  LtrCapable = IpPcieRecursiveIsLtrCapable (RpSbdf);

  if (pInst->PcieRpCommonConfig.PcieRpLtrConfig.LtrOverrideSpecCompliant && !LtrCapable) {
    IpPcieSpecCompliantLtrOverride (RpBase);
  } else {
    IpPcieConfigureRpLtrOverride (RpBase, RpSbdf.Dev, &TreeLtr, &(pInst->PcieRpCommonConfig.PcieRpLtrConfig));
  }

  IsLtrCapable = IpPcieGetCapability (pInst, pInst->RpIndex, IpPcieFeatIdLtr, &Status);
  if (!!IsLtrCapable) {
    if (HasChildBus (RpSbdf, &ChildSbdf)) {
      DevType = GetDeviceType (RpSbdf);
      if (FindNextPcieChild (DevType, &ChildSbdf)) {
        RecursiveLtrConfiguration (ChildSbdf, PolicyLtr);
      }
    }
    Status = IpPcieSetControl (pInst, pInst->RpIndex, IpPcieFeatIdLtr, IsLtrCapable);
  }

  ChildLtrOvr.Bits.ForceOverride                = TreeLtr.ForceOverride;
  ChildLtrOvr.Bits.MaxNoSnoopLatencyRequirement = TreeLtr.MaxNoSnoopLatencyRequirement;
  ChildLtrOvr.Bits.MaxNoSnoopLatencyScale       = TreeLtr.MaxNoSnoopLatencyScale;
  ChildLtrOvr.Bits.MaxNoSnoopLatencyValue       = TreeLtr.MaxNoSnoopLatencyValue;
  ChildLtrOvr.Bits.MaxSnoopLatencyRequirement   = TreeLtr.MaxSnoopLatencyRequirement;
  ChildLtrOvr.Bits.MaxSnoopLatencyScale         = TreeLtr.MaxSnoopLatencyScale;
  ChildLtrOvr.Bits.MaxSnoopLatencyValue         = TreeLtr.MaxSnoopLatencyValue;

  Status  = IpPcieSetControl (pInst, pInst->RpIndex, IpPcieFeatIdLtr, ChildLtrOvr.Uint32);

  //
  // Downstream ports never report CPM capability, so it's only relevant for upstream ports.
  // So Skipped the CPM configuration for rootport
  //
  if (pInst->PcieRpCommonConfig.EnableCpm) {
     RecursiveCpmConfiguration (RpSbdf);
  }

  //
  // L1 substates can be modified only when L1 is disabled, so this function must execute
  // before Aspm configuration which enables L1
  //
  RpL1ss.Uint32 = IpPcieGetCapability (pInst, pInst->RpIndex, IpPcieFeatIdL1ss, &Status);
  if (PmOverrideTable.Size > 0) {
    OverrideL1ssCaps (RpBase, &PmOverrideTable, &RpL1ss);
  }

  //
  // L1 substates can be modified only when L1 is disabled, so this function must execute
  // before Aspm configuration which enables L1
  //
  if (HasChildBus (RpSbdf, &ChildSbdf)) {
    DevType = GetDeviceType (RpSbdf);
    if (FindNextPcieChild (DevType, &ChildSbdf)) {
      ChildL1ss = RecursiveL1ssConfiguration (ChildSbdf, &PmOverrideTable);
    }

    if (DevType == DevTypePcieDownstream && ChildSbdf.Func == 0) {
      CombinedCaps = CombineL1ss (RpL1ss, ChildL1ss);
      SetL1ss (ChildSbdf, CombinedCaps, &PmOverrideTable);
      Status = IpPcieSetControl (pInst, pInst->RpIndex, IpPcieFeatIdL1ss, CombinedCaps.Uint32);
    }
  }

  RpAspm.Uint32 = IpPcieGetCapability (pInst, pInst->RpIndex, IpPcieFeatIdAspm, &Status);
  if (PmOverrideTable.Size > 0) {
    GetOverrideAspm (RpSbdf, &RpAspm, &PmOverrideTable);
  }
  if (!IsL1Allowed (RpAspm)) {
    RpAspm = PatchL1AcceptableLatency (RpAspm, 1);
  }

  if (HasChildBus (RpSbdf, &ChildSbdf)) {
    DevType = GetDeviceType (RpSbdf);
    if (FindNextPcieChild (DevType, &ChildSbdf)) {
      TreeAspm = RecursiveAspmConfiguration (ChildSbdf, 0, &PmOverrideTable);
    }
    CombinedAspm = CombineAspm (RpAspm, TreeAspm, (DevType == DevTypePcieDownstream));

    IpPcieSetControl (pInst, pInst->RpIndex, IpPcieFeatIdAspm, CombinedAspm.Uint32);
    //
    // ASPM config must be consistent across all functions of a device. That's why there's while loop.
    //
    while (FindNextPcieChild (DevType, &ChildSbdf)) {
      SetAspm (ChildSbdf, CombinedAspm);
    }
  }

  DynamicLinkThrottlingEnable (RpSbdf);
  ClearBusFromTable (&BridgeCleanupList);

  return IpCsiStsSuccess;
}

VOID
IpPcieSpecCompliantLtrOverride (
  UINT64           RpBase
)
{
  UINT32                    OvrEn;
  UINT32                    OvrVal;

  DEBUG ((DEBUG_INFO, "IpPcieSpecCompliantLtrOverride Start \n"));

  //
  // LTR settings from LTROVR register only get acknowledged on rising edge of LTROVR2[1:0]
  // If those bits were already set (that can happen on a plug-hotUnplug-hotPlug scenario),
  // they need to be toggled
  //
  if (PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_LTROVR2) != 0) {
    PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR2, 0);
  }

  OvrEn = B_PCH_PCIE_CFG_LTROVR2_LTRSOVREN | B_PCH_PCIE_CFG_LTROVR2_LTRNSOVREN;
  OvrVal = 0x81908190;

  DEBUG ((DEBUG_INFO, "Val %x En %x\n", OvrVal, OvrEn));

  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR, OvrVal);
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR2, OvrEn);

  DEBUG ((DEBUG_INFO, "IpPcieSpecCompliantLtrOverride End \n"));
}

/**
  Configures LTR override in Root Port's proprietary registers.
  @param[in] RpBase                        Root Port base address
  @param[in] DevNum                        Currently visited device number
  @param[in] TreeLtr                       Snoop Latency Override settings
  @param[in] LtrConfig                     Root Port LTR configuration
**/
VOID
IpPcieConfigureRpLtrOverride (
  UINT64              RpBase,
  UINT32              DevNum,
  LTR_OVERRIDE        *TreeLtr,
  IP_PCIE_LTR_CONFIG  *LtrConfig
  )
{
  UINT32                    OvrEn;
  UINT32                    OvrVal;

  OvrEn = 0;
  OvrVal = 0;

  DEBUG ((DEBUG_INFO, "IpPcieConfigureRpLtrOverride Start \n"));

  //
  // LTR settings from LTROVR register only get acknowledged on rising edge of LTROVR2[1:0]
  // If those bits were already set (that can happen on a plug-hotUnplug-hotPlug scenario),
  // they need to be toggled
  //
  if (PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_LTROVR2) != 0) {
    DEBUG ((DEBUG_INFO, "Clear LTROVR2\n"));
    PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR2, 0);
  }
  //
  // (*)LatencyOverrideMode = 0 -> no override
  //                          1 -> override with RP policy values
  //                          2 -> override with endpoint's override values
  //

  if (LtrConfig->ForceLtrOverride || TreeLtr->ForceOverride) {
    OvrEn |= B_PCIE_CFG_LTROVR2_LTROVRPLCY;
  }

  if (LtrConfig->SnoopLatencyOverrideMode) {
    OvrEn |= (UINT32) B_PCH_PCIE_CFG_LTROVR2_LTRSOVREN;
    OvrVal |= (UINT32) LtrConfig->SnoopLatencyOverrideValue;
    OvrVal |= ((UINT32) LtrConfig->SnoopLatencyOverrideMultiplier) << 10;
    OvrVal |= (UINT32) B_PCH_PCIE_CFG_LTROVR_LTRSROVR;
  } else if (TreeLtr->MaxSnoopLatencyRequirement) {
      OvrEn |= (UINT32) B_PCH_PCIE_CFG_LTROVR2_LTRSOVREN;
      OvrVal |= TreeLtr->MaxSnoopLatencyValue;
      OvrVal |= TreeLtr->MaxSnoopLatencyScale << 10;
      OvrVal |= (UINT32) B_PCH_PCIE_CFG_LTROVR_LTRSROVR;
  }
  if (LtrConfig->NonSnoopLatencyOverrideMode) {
    OvrEn |= (UINT32) B_PCH_PCIE_CFG_LTROVR2_LTRNSOVREN;
    OvrVal |= ((UINT32)LtrConfig->NonSnoopLatencyOverrideValue) << 16;
    OvrVal |= ((UINT32) LtrConfig->NonSnoopLatencyOverrideMultiplier) << 26;
    OvrVal |= (UINT32) B_PCH_PCIE_CFG_LTROVR_LTRNSROVR;
  } else if (TreeLtr->MaxNoSnoopLatencyRequirement) {
      OvrEn |= (UINT32) B_PCH_PCIE_CFG_LTROVR2_LTRNSOVREN;
      OvrVal |= TreeLtr->MaxNoSnoopLatencyValue << 16;
      OvrVal |= TreeLtr->MaxNoSnoopLatencyScale << 26;
      OvrVal |= (UINT32) B_PCH_PCIE_CFG_LTROVR_LTRNSROVR;
  }

  DEBUG ((DEBUG_INFO, "Val %x En %x\n", OvrVal, OvrEn));

  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR, OvrVal);
  PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_LTROVR2, OvrEn);

  DEBUG ((DEBUG_INFO, "IpPcieConfigureRpLtrOverride End\n"));
}