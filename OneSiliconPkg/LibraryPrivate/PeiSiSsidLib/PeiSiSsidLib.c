/** @file
  Program silicon SVID and SSID.

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

@par Specification Reference:
**/

#include <Library/DebugLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/PeiSiSsidLib.h>
#include <Ppi/SiPolicy.h>
#include <PcieRegs.h>
#include <TcssDataHob.h>
#include <Library/HobLib.h>

#define PROGRAM_SSID_WITH_DID 0xFFFE

STATIC SVID_SID_SKIP_TABLE mPcieSsidTable[] = {{0,   1,  0},
                                               {0,   1,  1},
                                               {0,   6,  0},
                                               {0,   6,  1},
                                               {0,   6,  2},
                                               {0,  26,  0},
                                               {0,  26,  1},
                                               {0,  26,  2},
                                               {0,  26,  3},
                                               {0,  27,  0},
                                               {0,  27,  1},
                                               {0,  27,  2},
                                               {0,  27,  3},
                                               {0,  27,  4},
                                               {0,  27,  5},
                                               {0,  27,  6},
                                               {0,  27,  7},
                                               {0,  28,  0},
                                               {0,  28,  1},
                                               {0,  28,  2},
                                               {0,  28,  3},
                                               {0,  28,  4},
                                               {0,  28,  5},
                                               {0,  28,  6},
                                               {0,  28,  7},
                                               {0,  29,  0},
                                               {0,  29,  1},
                                               {0,  29,  2},
                                               {0,  29,  3},
                                               {0,  29,  4},
                                               {0,  29,  5},
                                               {0,  29,  6},
                                               {0,  29,  7},
                                              };

/**
  Get SVID offset for PCI DEV FUNC.

  @param[in] PciCfgBase    Pci base address

  @retval  SVID offset
**/
STATIC
UINT16
GetSvidOffset (
  UINT64 PciCfgBase
  )
{
  UINT8 HeaderType;
  UINT8 CapList;

  HeaderType = PciSegmentRead8 (PciCfgBase + PCI_HEADER_TYPE_OFFSET);

  if ((HeaderType & HEADER_TYPE_CARDBUS_BRIDGE) != 0) {
    return R_PCI_CARDBUS_BRIDGE_SVID;
  } else if ((HeaderType & HEADER_TYPE_PCI_TO_PCI_BRIDGE) != 0) {
    //
    // If header type is PCI-to-PCI bridge, but device has SUBSYSTEM_VENDOR capability, it means it
    // has SSID/SVID regs on offset SVCAP + 0x04.
    // If capability does not exist, return 0 to skip programming that device.
    //
    CapList = PcieBaseFindCapId (PciCfgBase, PCI_CAP_ID_SUBSYSTEM_VENDOR);
    if (CapList != 0) {
      return CapList + R_PCIE_SUBSYSTEM_VENDOR_ID_OFFSET;
    } else {
      return 0;
    }
  } else {
    return PCI_SVID_OFFSET;
  }
}

/**
  Override default SVID and SSID

  @param[in]      PciDevNum                 Pci device number
  @param[in]      PciFuncNum                Pci function number
  @param[in]      SsidTablePtr              Ssid policy table
  @param[in]      NumberOfSsidTableEntry    Side of Ssid policy table
  @param[in,out]  Svid                      Svid value
  @param[in,out]  Ssid                      Ssid value
**/
STATIC
VOID
OverrideSvidSsidValue (
  UINT32                 PciDevNum,
  UINT32                 PciFuncNum,
  SVID_SID_INIT_ENTRY    *SsidTablePtr,
  UINT16                 NumberOfSsidTableEntry,
  UINT16                 *Svid,
  UINT16                 *Ssid
  )
{
  UINT32    Index;
  UINT64 PciCfgBase;

  if (SsidTablePtr != NULL) {
    for (Index = 0; Index < NumberOfSsidTableEntry; Index++) {
      if ((SsidTablePtr[Index].Address.Bits.Device == PciDevNum) &&
          (SsidTablePtr[Index].Address.Bits.Function == PciFuncNum))
      {
        *Svid = SsidTablePtr[Index].SvidSidValue.SubSystemVendorId;
        *Ssid = SsidTablePtr[Index].SvidSidValue.SubSystemId;
        //
        // Program SSID with DID value in case of SubSystemId is 0xFFFE
        //
        if (SsidTablePtr[Index].SvidSidValue.SubSystemId == PROGRAM_SSID_WITH_DID) {
          PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (0, 0, PciDevNum, PciFuncNum, 0);
          *Ssid = PciSegmentRead16 (PciCfgBase + PCI_DEVICE_ID_OFFSET);
        }
      }
    }
  }
}

/**
  Skip specific PCI devices program SSID before the device ready

  @param[in] SsidSkipTablePtr             SSID policy skip table
  @param[in] NumberOfSsidSkipTableEntry   Side of SSID policy skip table
  @param[in] PciDevNum                    PCI device number
  @param[in] PciFuncNum                   PCI function number

  @retval TRUE            skip program the SSID
  @retval FALSE           doesn't skip program the SSID
**/
STATIC
BOOLEAN
SkipSsidFunc (
  IN SVID_SID_SKIP_TABLE           *SsidSkipTablePtr,
  IN UINT16                        NumberOfSsidSkipTableEntry,
  IN UINT32                        PciDevNum,
  IN UINT32                        PciFuncNum
  )
{
  UINT16     Index;

  if (SsidSkipTablePtr != NULL) {
    for (Index = 0; Index < NumberOfSsidSkipTableEntry; Index++) {
      if ((PciDevNum == SsidSkipTablePtr[Index].SkipDevice) &&
          (PciFuncNum == SsidSkipTablePtr[Index].SkipFunction)) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
  Program devices Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

  @param[in] SiPolicy             The SI Policy PPI instance
  @param[in] SiSsidOverrideFunc   Silicon SSID override function
  @param[in] Segment              Segment number required for PciCfgBase evaluation
  @param[in] Bus                  Bus number required for PciCfgBase evaluation
**/
VOID
SiProgramSsid (
  IN  SI_POLICY_PPI             *SiPolicy,
  IN  SI_SSID_OVERRIDE_FUNC     SiSsidOverrideFunc,
  IN  UINT8                     Segment,
  IN  UINT8                     Bus
  )
{
  UINT16                        NumberOfSsidTableEntry;
  SVID_SID_INIT_ENTRY           *SsidTablePtr;
  EFI_STATUS                    Status;
  SI_CONFIG                     *SiConfig;
  UINT16                        CommonSvid;
  UINT16                        CommonSsid;
  UINT16                        DataSvid;
  UINT16                        DataSsid;
  UINT32                        PciDevNum;
  UINT32                        PciFuncNum;
  UINT64                        PciCfgBase;
  UINT16                        SvidOffset;
  BOOLEAN                       IsMultiFunc;
  UINT16                        NumberOfTcssSsidTable;
  SVID_SID_SKIP_TABLE           *SsidSkipTablePtr;
  TCSS_DATA_SSID_HOB            *TcssSsidHob;
  UINT16                        NumberOfPcieSsidTable;
  SVID_SID_SKIP_TABLE           *PcieSsidSkipTablePtr;
  UINT16                        NumberOfThcSsidTable;
  SVID_SID_SKIP_TABLE           *ThcSsidSkipTablePtr;
  THC_DATA_SSID_CONFIG_HOB      *ThcSsidHob;

  CommonSvid = DEFAULT_SSVID;
  CommonSsid = DEFAULT_SSDID;
  SsidTablePtr = NULL;
  NumberOfSsidTableEntry = 0;
  SsidSkipTablePtr = NULL;
  NumberOfTcssSsidTable = 0;
  ThcSsidSkipTablePtr = NULL;
  NumberOfThcSsidTable = 0;

  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
    if (!EFI_ERROR (Status) && (SiConfig != NULL)) {
      if (SiConfig->SkipSsidProgramming) {
        //
        // Skip all SSID programming
        //
        return;
      }
      if (SiConfig->CustomizedSvid != 0) {
        CommonSvid = SiConfig->CustomizedSvid;
      }
      if (SiConfig->CustomizedSsid != 0) {
        CommonSsid = SiConfig->CustomizedSsid;
      }
      //
      // Use SiPolicy Table and table counts
      //
      SsidTablePtr = (SVID_SID_INIT_ENTRY*) SiConfig->SsidTablePtr;
      NumberOfSsidTableEntry = SiConfig->NumberOfSsidTableEntry;
      ASSERT (NumberOfSsidTableEntry < SI_MAX_DEVICE_COUNT);
    }
  }

  ///
  /// Get Tcss SSID data Hob
  ///
  TcssSsidHob = NULL;
  TcssSsidHob = (TCSS_DATA_SSID_HOB *) GetFirstGuidHob (&gTcssSsidHobGuid);
  if (TcssSsidHob != NULL) {
    SsidSkipTablePtr = (SVID_SID_SKIP_TABLE*) TcssSsidHob->TcssSsidTable;
    NumberOfTcssSsidTable = TcssSsidHob->NumberOfTcssSsidTable;
    ASSERT (NumberOfTcssSsidTable < SI_MAX_DEVICE_COUNT);
  }

  PcieSsidSkipTablePtr = mPcieSsidTable;
  NumberOfPcieSsidTable = ARRAY_SIZE (mPcieSsidTable);

  ///
  /// Get THC SSID data Hob
  ///
  ThcSsidHob = NULL;
  ThcSsidHob = (THC_DATA_SSID_CONFIG_HOB *) GetFirstGuidHob (&gThcSsidHobGuid);
  if (ThcSsidHob != NULL) {
    ThcSsidSkipTablePtr = (SVID_SID_SKIP_TABLE*) ThcSsidHob->ThcSsidTable;
    NumberOfThcSsidTable = ThcSsidHob->NumberOfThcSsidTable;
    ASSERT (NumberOfThcSsidTable < SI_MAX_DEVICE_COUNT);
  }

  //
  // Silicon code will scan all devices in given segment and bus and program SSID.
  //
  for (PciDevNum = 0; PciDevNum <= PCI_MAX_DEVICE; PciDevNum++) {
    IsMultiFunc = FALSE;
    for (PciFuncNum = 0; PciFuncNum <= PCI_MAX_FUNC; PciFuncNum++) {
      //
      // Skip high function if not multple function controller
      //
      if ((PciFuncNum > 0) && !IsMultiFunc) {
        break;
      }

      //
      // Get SVID offset
      //
      PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, PciDevNum, PciFuncNum, 0);
      SvidOffset = GetSvidOffset (PciCfgBase);

      //
      // Skip if the device is not present or does not have SVID register
      //
      if (PciSegmentRead32 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF || SvidOffset == 0) {
        continue;
      }

      //
      // Check if it's multiple function controller
      //
      if (PciFuncNum == 0) {
        IsMultiFunc = (PciSegmentRead8 (PciCfgBase + PCI_HEADER_TYPE_OFFSET) & HEADER_TYPE_MULTI_FUNCTION) != 0;
      }

      if (SkipSsidFunc (SsidSkipTablePtr, NumberOfTcssSsidTable, PciDevNum, PciFuncNum) == TRUE) {
        continue;
      }

      if (SkipSsidFunc (PcieSsidSkipTablePtr, NumberOfPcieSsidTable, PciDevNum, PciFuncNum) == TRUE) {
        continue;
      }

      if (SkipSsidFunc (ThcSsidSkipTablePtr, NumberOfThcSsidTable, PciDevNum, PciFuncNum) == TRUE) {
        continue;
      }

      //
      // Get SVID SSID value
      //
      DataSvid = CommonSvid;
      DataSsid = CommonSsid;

      if ((SiSsidOverrideFunc == NULL) ||
          !SiSsidOverrideFunc (PciDevNum, PciFuncNum, &DataSvid, &DataSsid))
      {
        OverrideSvidSsidValue (PciDevNum, PciFuncNum, SsidTablePtr, NumberOfSsidTableEntry, &DataSvid, &DataSsid);
      }

      //
      // Register programming
      //
      PciSegmentWrite32 (PciCfgBase + SvidOffset, (DataSsid << 16) | DataSvid);
    }
  }
}

/**
  Program TCSS Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

  @param[in] SiPolicy             The SI Policy PPI instance
  @param[in] TcssSsidTablePtr     Tcss SSID policy table
  @param[in] NumberOfTcssTable    The number of SSID Items
**/
VOID
SiTcssProgramSsid (
  IN  SI_POLICY_PPI             *SiPolicy,
  IN  SVID_SID_SKIP_TABLE       *TcssSsidTablePtr,
  IN  UINT16                    NumberOfTcssTable
  )
{
  UINT16                        NumberOfSsidTableEntry;
  SVID_SID_INIT_ENTRY           *SsidTablePtr;
  EFI_STATUS                    Status;
  SI_CONFIG                     *SiConfig;
  UINT16                        CommonSvid;
  UINT16                        CommonSsid;
  UINT16                        DataSvid;
  UINT16                        DataSsid;
  UINT32                        PciDevNum;
  UINT32                        PciFuncNum;
  UINT64                        PciCfgBase;
  UINT16                        SvidOffset;
  UINT16                        Index;

  CommonSvid = DEFAULT_SSVID;
  CommonSsid = DEFAULT_SSDID;
  SsidTablePtr = NULL;
  NumberOfSsidTableEntry = 0;

  if ((TcssSsidTablePtr == NULL) || (NumberOfTcssTable == 0)) {
    return;
  }

  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
    if (!EFI_ERROR (Status) && (SiConfig != NULL)) {
      if (SiConfig->SkipSsidProgramming) {
        //
        // Skip all SSID programming
        //
        return;
      }
      if (SiConfig->CustomizedSvid != 0) {
        CommonSvid = SiConfig->CustomizedSvid;
      }
      if (SiConfig->CustomizedSsid != 0) {
        CommonSsid = SiConfig->CustomizedSsid;
      }
      //
      // Use SiPolicy Table and table counts
      //
      SsidTablePtr = (SVID_SID_INIT_ENTRY*) SiConfig->SsidTablePtr;
      NumberOfSsidTableEntry = SiConfig->NumberOfSsidTableEntry;
      ASSERT (NumberOfSsidTableEntry < SI_MAX_DEVICE_COUNT);
    }
  }

  for (Index = 0; Index < NumberOfTcssTable; Index++) {
    PciDevNum = TcssSsidTablePtr[Index].SkipDevice;
    PciFuncNum = TcssSsidTablePtr[Index].SkipFunction;

    //
    // Skip if the device is not present
    //
    PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (0, 0, PciDevNum, PciFuncNum, 0);
    if (PciSegmentRead32 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
      continue;
    }

    //
    // Get SVID offset
    //
    SvidOffset = GetSvidOffset (PciCfgBase);

    //
    // Get SVID SSID value
    //
    DataSvid = CommonSvid;
    DataSsid = CommonSsid;

    OverrideSvidSsidValue (PciDevNum, PciFuncNum, SsidTablePtr, NumberOfSsidTableEntry, &DataSvid, &DataSsid);

    //
    // Register programming
    //
    PciSegmentWrite32 (PciCfgBase + SvidOffset, (DataSsid << 16) | DataSvid);
  }
}

/**
  Program devices Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID) for specific SBDF device.

  @param[in] SiPolicy             The SI Policy PPI instance
  @param[in] SiSsidOverrideFunc   Silicon SSID override function
  @param[in] Segment              Segment number required for PciCfgBase
  @param[in] Bus                  Bus number required for PciCfgBase
  @param[in] DevNum               Device number required for PciCfgBase
  @param[in] FuncNum              Function number required for PciCfgBase
**/
VOID
SiProgramSsidForSbdf (
  IN  SI_POLICY_PPI             *SiPolicy,
  IN  SI_SSID_OVERRIDE_FUNC     SiSsidOverrideFunc,
  IN  UINT8                     Segment,
  IN  UINT8                     Bus,
  IN  UINT32                    DevNum,
  IN  UINT32                    FuncNum
  )
{
  UINT16                        NumberOfSsidTableEntry;
  SVID_SID_INIT_ENTRY           *SsidTablePtr;
  EFI_STATUS                    Status;
  SI_CONFIG                     *SiConfig;
  UINT16                        CommonSvid;
  UINT16                        CommonSsid;
  UINT16                        DataSvid;
  UINT16                        DataSsid;
  UINT64                        PciCfgBase;
  UINT16                        SvidOffset;

  PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, DevNum, FuncNum, 0);

  //
  // Skip if the device is not present
  //
  if (PciSegmentRead32 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    return;
  }

  SvidOffset = GetSvidOffset (PciCfgBase);

  //
  // Skip if the device does not have SVID register
  //
  if (SvidOffset == 0) {
    return;
  }

  CommonSvid = DEFAULT_SSVID;
  CommonSsid = DEFAULT_SSDID;
  SsidTablePtr = NULL;
  NumberOfSsidTableEntry = 0;

  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
    if (!EFI_ERROR (Status) && (SiConfig != NULL)) {
      if (SiConfig->SkipSsidProgramming) {
        //
        // Skip all SSID programming
        //
        return;
      }
      if (SiConfig->CustomizedSvid != 0) {
        CommonSvid = SiConfig->CustomizedSvid;
      }
      if (SiConfig->CustomizedSsid != 0) {
        CommonSsid = SiConfig->CustomizedSsid;
      }
      //
      // Use SiPolicy Table and table counts
      //
      SsidTablePtr = (SVID_SID_INIT_ENTRY *) SiConfig->SsidTablePtr;
      NumberOfSsidTableEntry = SiConfig->NumberOfSsidTableEntry;
      ASSERT (NumberOfSsidTableEntry < SI_MAX_DEVICE_COUNT);
    }
  }

  //
  // Get SVID SSID value
  //
  DataSvid = CommonSvid;
  DataSsid = CommonSsid;

  if ((SiSsidOverrideFunc == NULL) ||
      !SiSsidOverrideFunc (DevNum, FuncNum, &DataSvid, &DataSsid)) {
    OverrideSvidSsidValue (DevNum, FuncNum, SsidTablePtr, NumberOfSsidTableEntry, &DataSvid, &DataSsid);
  }

  //
  // Register programming
  //
  PciSegmentWrite32 (PciCfgBase + SvidOffset, (DataSsid << 16) | DataSvid);
}

/**
  Program Pcie Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

  @param[in] SiConfig             The SI config information
**/
VOID
SiPcieProgramSsid (
  IN  SI_CONFIG                 *SiConfig
  )
{
  UINT16                        NumberOfSsidTableEntry;
  SVID_SID_INIT_ENTRY           *SsidTablePtr;
  UINT16                        CommonSvid;
  UINT16                        CommonSsid;
  UINT16                        DataSvid;
  UINT16                        DataSsid;
  UINT32                        PciDevNum;
  UINT32                        PciFuncNum;
  UINT64                        PciCfgBase;
  UINT16                        SvidOffset;
  UINT16                        Index;
  SVID_SID_SKIP_TABLE           *PcieSsidTablePtr;
  UINT8                         NumberOfPcieTable;

  CommonSvid = DEFAULT_SSVID;
  CommonSsid = DEFAULT_SSDID;
  SsidTablePtr = NULL;
  NumberOfSsidTableEntry = 0;

  PcieSsidTablePtr = mPcieSsidTable;
  NumberOfPcieTable = ARRAY_SIZE (mPcieSsidTable);

  if ((PcieSsidTablePtr == NULL) || (NumberOfPcieTable == 0)) {
    return;
  }

  if (SiConfig != NULL) {
    if (SiConfig->SkipSsidProgramming) {
      //
      // Skip all SSID programming
      //
      return;
    }
    if (SiConfig->CustomizedSvid != 0) {
      CommonSvid = SiConfig->CustomizedSvid;
    }
    if (SiConfig->CustomizedSsid != 0) {
      CommonSsid = SiConfig->CustomizedSsid;
    }
    //
    // Use SiPolicy Table and table counts
    //
    SsidTablePtr = (SVID_SID_INIT_ENTRY*) SiConfig->SsidTablePtr;
    NumberOfSsidTableEntry = SiConfig->NumberOfSsidTableEntry;
    ASSERT (NumberOfSsidTableEntry < SI_MAX_DEVICE_COUNT);
  }

  for (Index = 0; Index < NumberOfPcieTable; Index++) {
    PciDevNum = PcieSsidTablePtr[Index].SkipDevice;
    PciFuncNum = PcieSsidTablePtr[Index].SkipFunction;

    //
    // Skip if the device is not present
    //
    PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (0, 0, PciDevNum, PciFuncNum, 0);
    if (PciSegmentRead32 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
      continue;
    }

    //
    // Get SVID offset
    //
    SvidOffset = GetSvidOffset (PciCfgBase);

    //
    // Get SVID SSID value
    //
    DataSvid = CommonSvid;
    DataSsid = CommonSsid;

    OverrideSvidSsidValue (PciDevNum, PciFuncNum, SsidTablePtr, NumberOfSsidTableEntry, &DataSvid, &DataSsid);

    //
    // Register programming
    //
      PciSegmentWrite32 (PciCfgBase + SvidOffset, (DataSsid << 16) | DataSvid);
  }
}


/**
  Program THC Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

    Get SVID SSID value based on below priority -
      1) Program THC Setup page Customized SSDID/SSVID if its other than default value,
         it means user wants to progam non default values and will be true for NULL driver case as well.
      2) If THC setup page values are default then check for opther inputs as well such as SiConfig Customized values
         or SsidTable entries for valid SSDID/SSVID entries also and program either of them.

         THC Setup page customized value >> Siconfig customized values >> SSID Table entry >> default value

  @param[in] SiPolicy            The SI Policy PPI instance
  @param[in] ThcSsidTablePtr     Thc SSID policy table
  @param[in] NumberOfThcTable    The number of SSID Items
**/
VOID
SiThcProgramSsid (
  IN  SI_POLICY_PPI             *SiPolicy,
  IN  SVID_SID_SKIP_TABLE       *ThcSsidTablePtr,
  IN  UINT8                     Index,
  IN  UINT16                    CustomizedSvid,
  IN  UINT16                    CustomizedSsid
  )
{
  UINT16                        NumberOfSsidTableEntry;
  SVID_SID_INIT_ENTRY           *SsidTablePtr;
  EFI_STATUS                    Status;
  SI_CONFIG                     *SiConfig;
  UINT32                        PciDevNum;
  UINT32                        PciFuncNum;
  UINT64                        PciCfgBase;
  UINT16                        SvidOffset;
  UINT16                        DataSvid;
  UINT16                        DataSsid;

  SsidTablePtr            = NULL;
  NumberOfSsidTableEntry  = 0;
  DataSsid                = DEFAULT_SSDID;
  DataSvid                = DEFAULT_SSVID;

  if (ThcSsidTablePtr == NULL) {
    return;
  }

  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
    if (!EFI_ERROR (Status) && (SiConfig != NULL)) {
      if (SiConfig->SkipSsidProgramming) {
        //
        // Skip all SSID programming
        //
        return;
      }
      if (SiConfig->CustomizedSvid != 0) {
        DataSvid = SiConfig->CustomizedSvid;
      }
      if (SiConfig->CustomizedSsid != 0) {
        DataSsid = SiConfig->CustomizedSsid;
      }
      //
      // Use SiPolicy Table and table counts
      //
      SsidTablePtr = (SVID_SID_INIT_ENTRY*) SiConfig->SsidTablePtr;
      NumberOfSsidTableEntry = SiConfig->NumberOfSsidTableEntry;
      ASSERT (NumberOfSsidTableEntry < SI_MAX_DEVICE_COUNT);
    }
  }


  PciDevNum = ThcSsidTablePtr[Index].SkipDevice;
  PciFuncNum = ThcSsidTablePtr[Index].SkipFunction;

  //
  // Skip if the device is not present
  //
  PciCfgBase = PCI_SEGMENT_LIB_ADDRESS (0, 0, PciDevNum, PciFuncNum, 0);
  if (PciSegmentRead32 (PciCfgBase + PCI_VENDOR_ID_OFFSET) == 0xFFFFFFFF) {
    return;
  }

  //
  // Get SVID offset
  //
  SvidOffset = GetSvidOffset (PciCfgBase);

  //
  // Program/Get SVID SSID value based on below priority -
  //  1) Program THC Setup page Customized SSDID/SSVID if its other than default value,
  //     it means user wants to progam non default values and will be true for NULL driver case as well.
  //  2) If THC setup page values are default then Check for SiConfig Customized values
  //     or SsidTable entries for valid SSDID/SSVID entries also and program either
  //
  if ((CustomizedSsid != DEFAULT_SSDID) || (CustomizedSvid != DEFAULT_SSVID)) {
    DataSvid = CustomizedSvid;
    DataSsid = CustomizedSsid;
  }

  OverrideSvidSsidValue (PciDevNum, PciFuncNum, SsidTablePtr, NumberOfSsidTableEntry, &DataSvid, &DataSsid);

  //
  // Register programming
  //
  PciSegmentWrite32 (PciCfgBase + SvidOffset, (DataSsid << 16) | DataSvid);

}

/**
  Get Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID)
  for requested Device and function number

  @param[in]   SiPolicy     The SI Policy PPI instance
  @param[in]   PciDevNum    PCI device number
  @param[in]   PciFuncNum   PCI function number
  @param[out]  Svid         Svid value
  @param[out]  Ssid         Ssid value
**/
VOID
SiGetSsidSvid (
  IN  SI_POLICY_PPI      *SiPolicy,
  IN  UINT32             PciDevNum,
  IN  UINT32             PciFuncNum,
  OUT UINT16             *Svid,
  OUT UINT16             *Ssid
  )
{
  UINT16                        NumberOfSsidTableEntry;
  SVID_SID_INIT_ENTRY           *SsidTablePtr;
  EFI_STATUS                    Status;
  SI_CONFIG                     *SiConfig;
  UINT16                        CommonSvid;
  UINT16                        CommonSsid;
  UINT16                        DataSvid;
  UINT16                        DataSsid;

  CommonSvid = DEFAULT_SSVID;
  CommonSsid = DEFAULT_SSDID;
  SsidTablePtr = NULL;
  NumberOfSsidTableEntry = 0;

  if (SiPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPolicy, &gSiConfigGuid, (VOID *) &SiConfig);
    if (!EFI_ERROR (Status) && (SiConfig != NULL)) {
      if (SiConfig->SkipSsidProgramming) {
        //
        // Skip all SSID programming
        //
        *Svid = 0;
        *Ssid = 0;
        return;
      }
      if (SiConfig->CustomizedSvid != 0) {
        CommonSvid = SiConfig->CustomizedSvid;
      }
      if (SiConfig->CustomizedSsid != 0) {
        CommonSsid = SiConfig->CustomizedSsid;
      }
      //
      // Use SiPolicy Table and table counts
      //
      SsidTablePtr = (SVID_SID_INIT_ENTRY*) SiConfig->SsidTablePtr;
      NumberOfSsidTableEntry = SiConfig->NumberOfSsidTableEntry;
      ASSERT (NumberOfSsidTableEntry < SI_MAX_DEVICE_COUNT);
    }
  }
  //
  // Get SVID SSID value
  //
  DataSvid = CommonSvid;
  DataSsid = CommonSsid;

  OverrideSvidSsidValue (PciDevNum, PciFuncNum, SsidTablePtr, NumberOfSsidTableEntry, &DataSvid, &DataSsid);

  *Svid = DataSvid;
  *Ssid = DataSsid;
}