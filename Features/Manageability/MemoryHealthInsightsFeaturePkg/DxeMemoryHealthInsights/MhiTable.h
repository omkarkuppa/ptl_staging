/** @file
  Memory Health Insights Data Table Appending to ACPI Platform Health Assessment Table Definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _MHI_TABLE_
#define _MHI_TABLE_

#include <PiDxe.h>
#include <Uefi.h>
#include <MemInfoHob.h>

#include <Ptl/MrcGlobalDefinitions.h>

//
// Memory Health Insights Data Table
//
typedef struct {
  UINT8     MemoryType;                               ///<  Memory Type Memory Type of the module (E.g DDR4, DDR5)
  UINT16    MfgId[MAX_NODE][MAX_CH][MAX_DIMM];        ///<  Manufacturer ID Part number of the memory module
  UINT16    Speed[MAX_NODE][MAX_CH][MAX_DIMM];        ///<  Speed Max data rate supported by the memory module on board, in MHz
  UINT8     RankInDimm[MAX_NODE][MAX_CH][MAX_DIMM];   ///<  Number of rank Memory organization: number of ranks of the memory module
  UINT8     Banks[MAX_NODE][MAX_CH][MAX_DIMM];        ///<  Number of banks Memory organization: number of banks of the memory module
  UINT8     BankGroups[MAX_NODE][MAX_CH][MAX_DIMM];   ///<  Bank group Memory organization
  UINT8     DimmCount[MAX_NODE][MAX_CH];              ///<  Number of dims Number of memory modules on board
  UINT8     FormFactor[MAX_NODE][MAX_CH][MAX_DIMM];   ///<  SPD ModuleType
  UINT32    DataRate[HOB_MAX_SAGV_POINTS];            ///<  Actual current data rate
  UINT32    Bandwidth[HOB_MAX_SAGV_POINTS];           ///<  Calculated Peak Bandwidth Theoretical max bandwidth
  UINT32    SaGvPointMask;                            ///<  Maximum data rate of Integrated Memory Controller maximum fused data rate supported by the IMC Also, includes calculated SAGV points
  UINT16    MaxFrequency;                             ///<  Maximum operating frequency of IMC
  UINT8     PprAvailable;                             ///<  PPR enabled from variable
  struct {
    UINT8 Reserved          : 6;
    UINT8 AmtRanLastBoot    : 1;
    UINT8 PprRanLastBoot    : 1;
  } AmtPprRanInLastBoot;                              ///<  Whether AMT and PPR ran in the prior boot [Bit 0: PPR, Bit 1: AMT]
  UINT16    TotalRowFailuresDiscovered;               ///<  PPR: Counts of detected bad rows
  UINT16    TotalPprRowRepairsSuccessful;             ///<  PPR: Counts of repair successes
  UINT16    TotalUncorrectableDevices;                ///<  PPR: Counts of repair failure
  PPR_RESULT_COLUMNS_HOB PprErrorInfo;                ///<  PPR: Error location
  UINT8     AvailableResources[MAX_NODE][MAX_CH][MAX_RANK_IN_CHANNEL][MAX_SDRAM_IN_DIMM]; ///< PPR available resources per device
} MHI_DATA_TABLE;

//
// ACPI structure for Memory Health Table addition to PHAT
//
typedef struct {
  UINT16                        PlatformRecordType;
  UINT16                        RecordLength;
  UINT8                         Revision;
  MHI_DATA_TABLE                MemHealthInsightData;
} PHAT_MEMORY_HEALTH_INSIGHTS_RECORD_STRUCTURE;

//
// This value depicts the format and contents of the MHI record
//
#define MHI_PHAT_RECORD_TYPE      0x2000
#define MHI_PHAT_RECORD_VERSION   1

#endif  // _MHI_TABLE_
