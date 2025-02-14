/** @file
  This file updates voltage regulator overrides and  programs Fivr Rfi settings.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <Library/DebugLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuCommonLib.h>
#include <CpuPowerMgmt.h>
#include <Library/IoLib.h>
#include <Ppi/SiPolicy.h>
#include <Register/VoltageRegulatorRegs.h>
#include <Library/PeiVrLib.h>
#include <Guid/CpuPmData.h>
#include <Register/B2pMailbox.h>
#include <Register/Ptl/VoltageRegulator/VoltageRegulatorDomains.h>

UINT32  mVrList[] = {
  SIGNATURE_32 ('C', 'O', 'R', 'E'), SIGNATURE_32 ('_', '_', 'G', 'T'),
  SIGNATURE_32 ('_', '_', 'S', 'A'), SIGNATURE_32 ('_', 'L', 'P', 'E')
};

/**
  Get the VR name list

  @param[out]  VrCount   Output the list count

  @retval      The VR name list
**/
UINT32 *
EFIAPI
GetVrListFru (
  OUT UINTN  *VrCount
  )
{
  *VrCount = ARRAY_SIZE (mVrList);
  return mVrList;
}

///
/// VR Override table structure
///
typedef struct {
  UINT16          Ia;
  UINT16          IaFvm;
  UINT16          IaFvmItrip;
  UINT16          Gt;
  UINT16          GtFvm;
  UINT16          GtFvmItrip;
  UINT16          Sa;
  UINT16          SaFvm;
  UINT16          SaFvmItrip;
  UINT16          Atom;
} CPU_VR_ICCMAX;

typedef struct {
  UINT16          IaLimit;
  UINT32          IaTimeLimit;
  UINT16          IaLimitWithIrms;
  UINT32          IaTimeLimitWithIrms;
  UINT16          GtLimit;
  UINT32          GtTimeLimit;
  UINT16          SaLimit;
  UINT32          SaTimeLimit;
  UINT16          AtomLimit;
  UINT32          AtomTimeLimit;
  UINT16          AtomLimitWithIrms;
  UINT32          AtomTimeLimitWithIrms;
} CPU_VR_TDC;

typedef struct {
  UINT16    Ia;
  UINT16    Gt;
  UINT16    Sa;
  UINT16    Atom;
} CPU_VR_VOLTAGELIMIT;

typedef struct {
  CPU_VR_ICCMAX       IccMax;
  CPU_VR_TDC          Tdc;
  CPU_VR_VOLTAGELIMIT VoltageLimit;
} CPU_VR_PROFILE;

typedef struct {
  UINT16          IaAc;
  UINT16          IaDc;
  UINT16          GtAc;
  UINT16          GtDc;
  UINT16          SaAc;
  UINT16          SaDc;
  UINT16          AtomAc;
  UINT16          AtomDc;
} CPU_VR_LOAD_LINE_PROFILE;

//
// VR Profile: PTL
// Caution: The order of profile enum ID should match the order in profiles array.
//
enum {
  ProfileVrPtlU15W40,      //  PTL U      15W 4+0
  ProfileVrPtlU15W20,      //  PTL U      15W 2+0
  ProfileVrPtlH12Xe25W48,  //  PTL H 12Xe 25W 4+8
  ProfileVrPtlH12Xe25W44,  //  PTL H 12Xe 25W 4+4
  ProfileVrPtlH12Xe25W28,  //  PTL H 12Xe 25W 2+8
  ProfileVrPtlH12Xe25W24,  //  PTL H 12Xe 25W 2+4
  ProfileVrPtlH4Xe25W48,   //  PTL H 4Xe  25W 4+8
  ProfileVrPtlH4Xe25W44,   //  PTL H 4Xe  25W 4+4
  ProfileVrPtlH4Xe25W28,   //  PTL H 4Xe  25W 2+8
  ProfileVrPtlH4Xe25W24,   //  PTL H 4Xe  25W 2+4
  ProfileVrPtlMax
};
GLOBAL_REMOVE_IF_UNREFERENCED CPU_VR_PROFILE mPtlVr[] = {
//
//  ICC MAX [1/4A]                                                                     TDC [1/8A, Sec]
// {{Ia,     IaFvm, IaFvmItrip, Gt,  GtFvm, GtFvmItrip, Sa,  SaFvm, SaFvmItrip, Atom} {IaLimit, IaTimeLimit, IaIrmsLimit, IaIrmsTimeLimit, GtLimit, GtTimeLimit, SaLimit, SaTimeLimit, AtomLimit, AtomTimeLimit, AtomIrmsLimit, AtomIrmsTimeLimit} {IaVoltageLimit, GtVoltageLimit, SaVoltageLimit, AtomVoltageLimit}}
//
  {{  66*4,   80*4,   62*4,    38*4,  51*4,   38*4,    52*4, 52*4,     38*4,   25*4}, { 37*8,      1,             0,          28,          22*8,         1,       21*8,       1,         12*8,         1,              0,                0       },  {1520,               1520,           1520,          1520}}, // ProfileVrPtlU15W40
  {{  66*4,   80*4,   62*4,    38*4,  51*4,   38*4,    52*4, 52*4,     38*4,   25*4}, { 37*8,      1,             0,          28,          22*8,         1,       21*8,       1,         12*8,         1,              0,                0       },  {1520,               1520,           1520,          1520}}, // ProfileVrPtlU15W20
  {{ 120*4,  111*4,   86*4,    84*4,  98*4,   84*4,    52*4, 52*4,     34*4,   25*4}, { 48*8,      1,             0,          28,          30*8,         1,       21*8,       1,         10*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}, // ProfileVrPtlH12Xe25W48
  {{ 120*4,  111*4,   86*4,    84*4,  98*4,   84*4,    52*4, 52*4,     34*4,   25*4}, { 48*8,      1,             0,          28,          30*8,         1,       21*8,       1,         10*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}, // ProfileVrPtlH12Xe25W44
  {{ 120*4,  111*4,   86*4,    84*4,  98*4,   84*4,    52*4, 52*4,     34*4,   25*4}, { 48*8,      1,             0,          28,          30*8,         1,       21*8,       1,         10*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}, // ProfileVrPtlH12Xe25W28
  {{ 120*4,  111*4,   86*4,    84*4,  98*4,   84*4,    52*4, 52*4,     34*4,   25*4}, { 48*8,      1,             0,          28,          30*8,         1,       21*8,       1,         10*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}, // ProfileVrPtlH12Xe25W24
  {{ 120*4,  111*4,   86*4,    38*4,  51*4,   38*4,    52*4, 52*4,     34*4,   25*4}, { 48*8,      1,             0,          28,          22*8,         1,       21*8,       1,         10*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}, // ProfileVrPtlH4Xe25W48
  {{ 120*4,  111*4,   86*4,    38*4,  51*4,   38*4,    52*4, 52*4,     34*4,   25*4}, { 48*8,      1,             0,          28,          22*8,         1,       21*8,       1,         10*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}, // ProfileVrPtlH4Xe25W44
  {{ 120*4,  111*4,   86*4,    38*4,  51*4,   38*4,    52*4, 52*4,     34*4,   25*4}, { 48*8,      1,             0,          28,          22*8,         1,       21*8,       1,         10*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}, // ProfileVrPtlH4Xe25W28
  {{ 120*4,  111*4,   86*4,    38*4,  51*4,   38*4,    52*4, 52*4,     34*4,   25*4}, { 48*8,      1,             0,          28,          22*8,         1,       21*8,       1,         10*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}, // ProfileVrPtlH4Xe25W24
};
//
// Catch the mismatch of number of profile enum IDs and number of profiles.
// But the order mismatch cannot be caught.
//
STATIC_ASSERT (ARRAY_SIZE (mPtlVr) == ProfileVrPtlMax, "VR Profile count mismatch!");

enum {
  ProfileLoadLinePtlU15W40,      //  PTL U      15W 4+0
  ProfileLoadLinePtlU15W20,      //  PTL U      15W 2+0
  ProfileLoadLinePtlH12Xe25W48,  //  PTL H 12Xe 25W 4+8
  ProfileLoadLinePtlH12Xe25W44,  //  PTL H 12Xe 25W 4+4
  ProfileLoadLinePtlH12Xe25W28,  //  PTL H 12Xe 25W 2+8
  ProfileLoadLinePtlH12Xe25W24,  //  PTL H 12Xe 25W 2+4
  ProfileLoadLinePtlH4Xe25W48,   //  PTL H 4Xe  25W 4+8
  ProfileLoadLinePtlH4Xe25W44,   //  PTL H 4Xe  25W 4+4
  ProfileLoadLinePtlH4Xe25W28,   //  PTL H 4Xe  25W 2+8
  ProfileLoadLinePtlH4Xe25W24,   //  PTL H 4Xe  25W 2+4
  ProfileLoadLinePtlMax
};
GLOBAL_REMOVE_IF_UNREFERENCED CPU_VR_LOAD_LINE_PROFILE mPtlLoadLine[] = {
//
//  IaAc   IaDc   GtAc   GtDc   SaAc   SaDc  AtomAc  AtomDc  [1/100mOhm]
//
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlU15W40
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlU15W20
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlH12Xe25W48
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlH12Xe25W44
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlH12Xe25W28
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlH12Xe25W24
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlH4Xe25W48
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlH4Xe25W44
  { 330,   330,    350,    0,   550,   550,   580,    580 }, // ProfileLoadLinePtlH4Xe25W28
  { 330,   330,    350,    0,   550,   550,   580,    580 }  // ProfileLoadLinePtlH4Xe25W24
};
//
// Catch the mismatch of number of profile enum IDs and number of profiles.
// But the order mismatch cannot be caught.
//
STATIC_ASSERT (ARRAY_SIZE (mPtlLoadLine) == ProfileLoadLinePtlMax, "Load Line Profile count mismatch!");

//
// VR Profile: WCL
// Caution: The order of profile enum ID should match the order in profiles array.
//
enum {
  ProfileVrWclU15W10,      //  WCL U      15W 1+0
  ProfileVrWclU15W20,      //  WCL U      15W 2+0
  ProfileVrWclMax
};
GLOBAL_REMOVE_IF_UNREFERENCED CPU_VR_PROFILE mWclVr[] = {
//
//  ICC MAX [1/4A]                                                                          TDC [1/8A, Sec]
// {{Ia,     IaFvm, IaFvmItrip, Gt,  GtFvm, GtFvmItrip, Sa,  SaFvm, SaFvmItrip, Atom} {IaLimit, IaTimeLimit, IaIrmsLimit, IaIrmsTimeLimit, GtLimit, GtTimeLimit, SaLimit, SaTimeLimit, AtomLimit, AtomTimeLimit, AtomIrmsLimit, AtomIrmsTimeLimit} {IaVoltageLimit, GtVoltageLimit, SaVoltageLimit, AtomVoltageLimit}}
//
  {{     0,      0,      0,       0,     0,      0,       0,    0,        0,      0}, {    0,      0,             0,          0,             0,         1,          0,       0,            0,         1,              0,                0       },  {1600,               1520,           1520,          1520}},  // ProfileVrWclU15W10
  {{  40*4,      0,      0,    40*4,     0,      0,    35*4,    0,        0,   25*4}, { 20*8,      1,             0,          0,          18*8,         1,       14*8,       0,         11*8,         1,              0,                0       },  {1600,               1520,           1520,          1520}}   // ProfileVrWclU15W20

};
//
// Catch the mismatch of number of profile enum IDs and number of profiles.
// But the order mismatch cannot be caught.
//
STATIC_ASSERT (ARRAY_SIZE (mWclVr) == ProfileVrWclMax, "VR Profile count mismatch!");

enum {
  ProfileLoadLineWclU15W10,      //  WCL U      15W 1+0
  ProfileLoadLineWclU15W20,      //  WCL U      15W 2+0
  ProfileLoadLineWclMax
};
GLOBAL_REMOVE_IF_UNREFERENCED CPU_VR_LOAD_LINE_PROFILE mWclLoadLine[] = {
//
//  IaAc   IaDc   GtAc   GtDc   SaAc   SaDc  AtomAc  AtomDc  [1/100mOhm]
//
  {   0,     0,      0,    0,     0,     0,     0,      0 }, // ProfileLoadLineWclU15W10
  { 520,   520,    660,  660,   750,   750,     0,      0 }  // ProfileLoadLineWclU15W20
};
//
// Catch the mismatch of number of profile enum IDs and number of profiles.
// But the order mismatch cannot be caught.
//
STATIC_ASSERT (ARRAY_SIZE (mWclLoadLine) == ProfileLoadLineWclMax, "Load Line Profile count mismatch!");

typedef struct {
  CPU_IDENTIFIER           CpuIdentifier;

  CPU_VR_PROFILE           *Vr;
  CPU_VR_LOAD_LINE_PROFILE *LoadLine;
} CPU_VR_OVERRIDE_TABLE;

GLOBAL_REMOVE_IF_UNREFERENCED CPU_VR_OVERRIDE_TABLE mCpuVrOverrideTable[] = {
//
//  Identifier                              VR                              LoadLine
//
//  PTL U, H 12Xe/4Xe
//
  { EnumPtlU15Watt40CpuId,     &mPtlVr[ProfileVrPtlU15W40],     &mPtlLoadLine[ProfileLoadLinePtlU15W40]     },
  { EnumPtlU15Watt20CpuId,     &mPtlVr[ProfileVrPtlU15W20],     &mPtlLoadLine[ProfileLoadLinePtlU15W20]     },
  { EnumPtlH12Xe25Watt48CpuId, &mPtlVr[ProfileVrPtlH12Xe25W48], &mPtlLoadLine[ProfileLoadLinePtlH12Xe25W48] },
  { EnumPtlH12Xe25Watt44CpuId, &mPtlVr[ProfileVrPtlH12Xe25W44], &mPtlLoadLine[ProfileLoadLinePtlH12Xe25W44] },
  { EnumPtlH12Xe25Watt28CpuId, &mPtlVr[ProfileVrPtlH12Xe25W28], &mPtlLoadLine[ProfileLoadLinePtlH12Xe25W28] },
  { EnumPtlH12Xe25Watt24CpuId, &mPtlVr[ProfileVrPtlH12Xe25W24], &mPtlLoadLine[ProfileLoadLinePtlH12Xe25W24] },
  { EnumPtlH4Xe25Watt48CpuId,  &mPtlVr[ProfileVrPtlH4Xe25W48],  &mPtlLoadLine[ProfileLoadLinePtlH4Xe25W48]  },
  { EnumPtlH4Xe25Watt44CpuId,  &mPtlVr[ProfileVrPtlH4Xe25W44],  &mPtlLoadLine[ProfileLoadLinePtlH4Xe25W44]  },
  { EnumPtlH4Xe25Watt28CpuId,  &mPtlVr[ProfileVrPtlH4Xe25W28],  &mPtlLoadLine[ProfileLoadLinePtlH4Xe25W28]  },
  { EnumPtlH4Xe25Watt24CpuId,  &mPtlVr[ProfileVrPtlH4Xe25W24],  &mPtlLoadLine[ProfileLoadLinePtlH4Xe25W24]  },
  //
  //  WCL U
  //
  { EnumWclU15Watt10CpuId,     &mWclVr[ProfileVrWclU15W10],     &mWclLoadLine[ProfileLoadLineWclU15W10] },
  { EnumWclU15Watt20CpuId,     &mWclVr[ProfileVrWclU15W20],     &mWclLoadLine[ProfileLoadLineWclU15W20] }
};

/**
  Programs the VR mailbox without using Override Values for the selected VR Address.

  @param[in]  CpuPowerMgmtVrConfig   Vr config block
  @param[in]  TempVrAddress          Selected VR Address.
  @param[in]  VrIndex                Selected VR Index.
**/
VOID
SetVrNonOverrideValuesFru (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig,
  IN UINT8                       TempVrAddress,
  IN UINTN                       VrIndex
  )
{
}

/**
  Load CPU power management Vr Config block default for specific generation.

  @param[in, out] CpuPowerMgmtVrConfig   Pointer to CPU_POWER_MGMT_VR_CONFIG instance
**/
VOID
EFIAPI
PeiCpuLoadPowerMgmtVrConfigDefaultFru (
  IN OUT CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig
  )
{
  CpuPowerMgmtVrConfig->VsysFullScale                   = 24000;  // unit is 1mV
  CpuPowerMgmtVrConfig->VsysCriticalThreshold           = 6000;   // unit is 1mV
  CpuPowerMgmtVrConfig->PsysFullScale                   = 200000; // unit is 1mV
  CpuPowerMgmtVrConfig->PsysCriticalThreshold           = 120000; // unit is 1mV
  CpuPowerMgmtVrConfig->VsysAssertionDeglitchMantissa   = 0x1;
  CpuPowerMgmtVrConfig->VsysDeassertionDeglitchMantissa = 0xD;
  CpuPowerMgmtVrConfig->VsysDeassertionDeglitchExponent = 0x2;
}


/**
  Get Vr topologies which contain generation specific Vr address and bit which represents Svid Enabled/Disabled. Transferred as parameter from Fru to IP block.

  @param[IN,OUT] - VrDomainTopology - It takes VR_DOMAIN_TOPOLOGY as parameter and returns same which contains VR address and Svid Enable/Disabled.
**/
VOID
EFIAPI
GetCpuVrTopologyFru (
  IN OUT VR_DOMAIN_TOPOLOGY *VrDomainTopology
  )
{
  EFI_STATUS                  Status;
  UINT32                      MailboxStatus;
  VR_TOPOLOGY_DATA            VrTopology;
  UINTN                       VrIndex;
  PCODE_MAILBOX_INTERFACE     MailboxCommand;
  UINTN                       MaxNumVrs;

  ///
  /// CPU VR MSR mailbox
  ///
  VrTopology.VrTopology = 0;

  ///
  /// Get CPU VR topology
  ///
  DEBUG ((DEBUG_INFO, "(MAILBOX) Mailbox Read Command = READ_VR_STRAP_CONFIG_CMD\n"));
  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.Command = MAILBOX_VR_CMD_SVID_VR_HANDLER;
  MailboxCommand.Fields.Param1 = MAILBOX_VR_SUBCMD_SVID_GET_STRAP_CONFIGURATION;
  //
  // Use MailboxReadWithInput because the input data 0 indicating to read CPU strap set 2 low
  //
  Status = MailboxReadWithInput (MailboxCommand.InterfaceData, (UINT32*)&VrTopology.VrTopology, &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "VR: Error Reading VR topology. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
  }

  ///
  /// Print VR Topology data
  ///
  DEBUG ((DEBUG_INFO, "VR: VrTopology data = 0x%x\n", VrTopology.VrTopology));
  DEBUG ((DEBUG_INFO, "    VR Type 0 = SVID, VR Type 1 = non-SVID\n"));
  DEBUG ((DEBUG_INFO, "    IA VR Address    = 0x%x\n", VrTopology.Fields.VrIaAddress));
  DEBUG ((DEBUG_INFO, "    IA VR Type       = 0x%x\n", VrTopology.Fields.VrIaSvidType));
  DEBUG ((DEBUG_INFO, "    GT VR Address    = 0x%x\n", VrTopology.Fields.VrGtAddress));
  DEBUG ((DEBUG_INFO, "    GT VR Type       = 0x%x\n", VrTopology.Fields.VrGtSvidType));
  DEBUG ((DEBUG_INFO, "    SA VR Address    = 0x%x\n", VrTopology.Fields.VrSaAddress));
  DEBUG ((DEBUG_INFO, "    SA VR Type       = 0x%x\n", VrTopology.Fields.VrSaSvidType));
  DEBUG ((DEBUG_INFO, "    ATOM VR Address  = 0x%x\n", VrTopology.Fields.VrAtomAddress));
  DEBUG ((DEBUG_INFO, "    ATOM VR Type     = 0x%x\n", VrTopology.Fields.VrAtomSvidType));

  MaxNumVrs = GetMaxNumVrsFru ();

  //
  // Avoid array overflow while looping through VR arrays.
  //
  ASSERT (MaxNumVrs <= ARRAY_SIZE (VrDomainTopology->VrAddress));

  for (VrIndex = 0; VrIndex < MaxNumVrs; VrIndex++) {
    switch (VrIndex) {
      case CPU_VR_DOMAIN_IA:
        VrDomainTopology->VrAddress[VrIndex] = (UINT8) VrTopology.Fields.VrIaAddress;
        VrDomainTopology->SvidEnabled[VrIndex] = (UINT8) (((~VrTopology.Fields.VrIaSvidType) & BIT0));
        break;

      case CPU_VR_DOMAIN_GT:
        VrDomainTopology->VrAddress[VrIndex] = (UINT8) VrTopology.Fields.VrGtAddress;
        VrDomainTopology->SvidEnabled[VrIndex] = (UINT8) (((~VrTopology.Fields.VrGtSvidType) & BIT0));
        break;

      case CPU_VR_DOMAIN_SA:
        VrDomainTopology->VrAddress[VrIndex] = (UINT8) VrTopology.Fields.VrSaAddress;
        VrDomainTopology->SvidEnabled[VrIndex] = (UINT8) (((~VrTopology.Fields.VrSaSvidType) & BIT0));
        break;

      case CPU_VR_DOMAIN_ATOM:
        VrDomainTopology->VrAddress[VrIndex] = (UINT8) VrTopology.Fields.VrAtomAddress;
        VrDomainTopology->SvidEnabled[VrIndex] = (UINT8) (((~VrTopology.Fields.VrAtomSvidType) & BIT0));
        break;

      default:
        VrDomainTopology->VrAddress[VrIndex] = 0;
        VrDomainTopology->SvidEnabled[VrIndex] = 0;
        break;
    }
  }
}

/**
  Returns the maximum number of voltage regulator domains.

  @retval Maximum Number of VR Domains
**/
UINT16
EFIAPI
GetMaxNumVrsFru (
  VOID
  )
{
  return 4;
}

/**
  Enables/Disables DLVR's PHASE_SSC feature.
  @param[in] CpuPowerMgmtVrConfig   Pointer to CPU_POWER_MGMT_VR_CONFIG instance
**/
VOID
EFIAPI
SetDlvrPhaseSsc (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig
  )
{
  EFI_STATUS                                                  Status;
  PCODE_MAILBOX_INTERFACE                                     MailboxCommand;
  UINT32                                                      MailboxStatus;
  UINT32                                                      MailboxData;

  Status                        = EFI_SUCCESS;
  MailboxStatus                 = PCODE_MAILBOX_CC_SUCCESS;
  MailboxCommand.InterfaceData  = 0;

  MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_DLVR_SOC;
  MailboxCommand.Fields.Param1 = MAILBOX_PCODE_DLVR_PHASE_SSC_SET_SUBCOMMAND;

  if (CpuPowerMgmtVrConfig->DlvrPhaseSsc) {
    MailboxData   = 0x1;
    Status = MailboxWrite (MailboxCommand.InterfaceData, MailboxData, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "Mailbox Command Write failed. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    }
  }
}

/**
  Enables CEP/SIRP feature via mailbox. Mobile does not support CEP/SIRP feature.

  @param[in]  CpuPowerMgmtVrConfig   Vr config block
  @param[in]  TempVrAddress          Selected VR Address.
  @param[in]  VrIndex                Selected VR Index.
**/
VOID
SetVrMiscConfig (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig,
  IN UINT8                       TempVrAddress,
  IN UINTN                       VrIndex
  )
{
}

/**
  Get VrData from Fru layer

  @param[in] CpuIdentifier - Non-Zero value to override the Cpu Identifier used to look for CpuPM data
  @param[in] VrCount       - Count of List VrData
  @param[out] VrData       - VrData of the given CpuIdentifier

  @return RETURN_SUCCESS  if the Hob is created successfully
  @return others          if this function is not supported or implemented
**/
RETURN_STATUS
EFIAPI
GetVrOverridesFru (
  IN     UINT64                   CpuIdentifier,
  IN     UINTN                    VrCount,
  OUT    VOLTAGE_REGULATOR_PARAM  *VrData
  )
{
  UINTN                  Index;
  CPU_VR_OVERRIDE_TABLE  *VrOverride;

  ///
  /// Get CpuIdentifier to identify which set of VR values we need to override
  ///
  if (CpuIdentifier == 0) {
    CpuIdentifier = GetCpuIdentifier ();
  }

  DEBUG ((DEBUG_INFO, "VR: Cpu Identifier = %X\n", CpuIdentifier));
  if (CpuIdentifier == EnumUnknownCpuId) {
    DEBUG ((DEBUG_ERROR, "VR: Unknown Cpu Identifier, bypassing VR overrides.\n"));
    return RETURN_NOT_FOUND;
  }

  ///
  /// Update the VR config block with the VR override table data..
  ///
  for (Index = 0; Index < ARRAY_SIZE (mCpuVrOverrideTable); Index++) {
    ///
    /// If Cpu Identifier matches, then update with overrides values
    ///
    VrOverride = &mCpuVrOverrideTable[Index];
    if ((UINT64)VrOverride->CpuIdentifier == CpuIdentifier) {
      ASSERT(VrCount > CPU_VR_DOMAIN_IA);
      VrData[CPU_VR_DOMAIN_IA].IccMax           = VrOverride->Vr->IccMax.Ia;
      VrData[CPU_VR_DOMAIN_IA].IccMaxFvm        = VrOverride->Vr->IccMax.IaFvm;
      VrData[CPU_VR_DOMAIN_IA].IccItripFvm      = VrOverride->Vr->IccMax.IaFvmItrip;
      VrData[CPU_VR_DOMAIN_IA].TdcLimitIrms     = VrOverride->Vr->Tdc.IaLimitWithIrms;
      VrData[CPU_VR_DOMAIN_IA].TdcTimeLimitIrms = VrOverride->Vr->Tdc.IaTimeLimitWithIrms;
      VrData[CPU_VR_DOMAIN_IA].TdcTimeLimit     = VrOverride->Vr->Tdc.IaTimeLimit;
      VrData[CPU_VR_DOMAIN_IA].TdcLimit         = VrOverride->Vr->Tdc.IaLimit;
      VrData[CPU_VR_DOMAIN_IA].AcLoadLine       = VrOverride->LoadLine->IaAc;
      VrData[CPU_VR_DOMAIN_IA].DcLoadLine       = VrOverride->LoadLine->IaDc;
      VrData[CPU_VR_DOMAIN_IA].VoltageLimit     = VrOverride->Vr->VoltageLimit.Ia;

      ASSERT(VrCount > CPU_VR_DOMAIN_GT);
      VrData[CPU_VR_DOMAIN_GT].IccMax       = VrOverride->Vr->IccMax.Gt;
      VrData[CPU_VR_DOMAIN_GT].IccMaxFvm    = VrOverride->Vr->IccMax.GtFvm;
      VrData[CPU_VR_DOMAIN_GT].IccItripFvm  = VrOverride->Vr->IccMax.GtFvmItrip;
      VrData[CPU_VR_DOMAIN_GT].TdcLimit     = VrOverride->Vr->Tdc.GtLimit;
      VrData[CPU_VR_DOMAIN_GT].TdcTimeLimit = VrOverride->Vr->Tdc.GtTimeLimit;
      VrData[CPU_VR_DOMAIN_GT].AcLoadLine   = VrOverride->LoadLine->GtAc;
      VrData[CPU_VR_DOMAIN_GT].DcLoadLine   = VrOverride->LoadLine->GtDc;
      VrData[CPU_VR_DOMAIN_GT].VoltageLimit = VrOverride->Vr->VoltageLimit.Gt;

      ASSERT(VrCount > CPU_VR_DOMAIN_SA);
      VrData[CPU_VR_DOMAIN_SA].IccMax       = VrOverride->Vr->IccMax.Sa;
      VrData[CPU_VR_DOMAIN_SA].IccMaxFvm    = VrOverride->Vr->IccMax.SaFvm;
      VrData[CPU_VR_DOMAIN_SA].IccItripFvm  = VrOverride->Vr->IccMax.SaFvmItrip;
      VrData[CPU_VR_DOMAIN_SA].TdcLimit     = VrOverride->Vr->Tdc.SaLimit;
      VrData[CPU_VR_DOMAIN_SA].TdcTimeLimit = VrOverride->Vr->Tdc.SaTimeLimit;
      VrData[CPU_VR_DOMAIN_SA].AcLoadLine   = VrOverride->LoadLine->SaAc;
      VrData[CPU_VR_DOMAIN_SA].DcLoadLine   = VrOverride->LoadLine->SaDc;
      VrData[CPU_VR_DOMAIN_SA].VoltageLimit = VrOverride->Vr->VoltageLimit.Sa;

      ASSERT(VrCount > CPU_VR_DOMAIN_ATOM);
      VrData[CPU_VR_DOMAIN_ATOM].IccMax           = VrOverride->Vr->IccMax.Atom;
      VrData[CPU_VR_DOMAIN_ATOM].TdcLimitIrms     = VrOverride->Vr->Tdc.AtomLimitWithIrms;
      VrData[CPU_VR_DOMAIN_ATOM].TdcTimeLimitIrms = VrOverride->Vr->Tdc.AtomTimeLimitWithIrms;
      VrData[CPU_VR_DOMAIN_ATOM].TdcTimeLimit     = VrOverride->Vr->Tdc.AtomTimeLimit;
      VrData[CPU_VR_DOMAIN_ATOM].TdcLimit         = VrOverride->Vr->Tdc.AtomLimit;
      VrData[CPU_VR_DOMAIN_ATOM].AcLoadLine       = VrOverride->LoadLine->AtomAc;
      VrData[CPU_VR_DOMAIN_ATOM].DcLoadLine       = VrOverride->LoadLine->AtomDc;
      VrData[CPU_VR_DOMAIN_ATOM].VoltageLimit     = VrOverride->Vr->VoltageLimit.Atom;
      return RETURN_SUCCESS;
    }
  }

  ///
  /// No matching CpuIdentifier found in the table
  ///
  DEBUG ((DEBUG_INFO, "No matching CpuIdentifier found in the VrOverride Table.\n"));
  return RETURN_NOT_FOUND;
}

/**
  This function return whether CEP (Current Excursion Protection) configuration supported or not.
  @param[in]  VrIndex   Index of VR.
  @retval TRUE  - CEP (Current Excursion Protection) configuration supported.
  @retval FALSE - CEP (Current Excursion Protection) configuration unsupported.
**/
BOOLEAN
EFIAPI
IsVrCepConfigSupport (
  IN  UINTN  VrIndex
  )
{
  if ((VrIndex == CPU_VR_DOMAIN_IA) || (VrIndex == CPU_VR_DOMAIN_ATOM) || (VrIndex == CPU_VR_DOMAIN_SA)) {
    return TRUE;
  }
  return FALSE;
}