/** @file
  Unit test case of the Memory Health Insights driver.

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

@par Specification
**/

#include "MhiTestCase.h"

#include <stdlib.h>
#include <time.h>
#include <MhiTable.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#include "MemInfoHob.h"

/**
  Initialize MHI updating function.
  Format MRC Memory Info Data structure for ACPI publishing.

  @param[in]      MemInfoData       Memory info data hob structure pointer
  @param[in, out] MhiDataTable      Configured data to MHI table and PHAT append

  @retval     EFI_SUCCESS           Data set successfully
  @retval     EFI_OUT_OF_RESOURCES  Could not allocate memory for MHI table
  @retval     Others                Failed to set data to new table
**/
extern
EFI_STATUS
UpdateMemInfoToMhiPhat (
  IN      MEMORY_INFO_DATA_HOB        *MemInfoData,
  IN OUT  MHI_DATA_TABLE              *MhiDataTable
  );

/**
  Randomly generates number of input length.

  @param[in]  len     Number of bytes to generate

  @retval     num     Randomly generated number
**/
UINTN
GetRandNum (
  IN UINTN  len
)
{
  UINTN     num = 0;

  for (int i = 0; i < len; i++) {
    num = (num << 8) | ((UINT8) (rand() % 255));
  }
  return num;
}

/**
  Generates and prints MRC data info HOB for test

  @param[in, out]  MemInfoData   Memory info data hob structure pointer

  @retval     EFI_SUCCESS           Failed to set data and print
  @retval     EFI_INVALID_PARAMETER Inputs are invalid
**/
EFI_STATUS
GenMemInfoHob (
  IN OUT MEMORY_INFO_DATA_HOB   *MemInfoData
)
{
  UINT8     i, j, k, l;

  if (MemInfoData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // Use RNG so the test will be unique
  srand((unsigned) time(NULL));
  MemInfoData->MemoryType = (UINT8) (rand () % 3);
  for (i = 0; i < MAX_NODE; i++) {
    for (j = 0; j < MAX_CH; j++) {
      for (k = 0; k < MAX_DIMM; k++) {
        MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].MfgId         = (UINT16) GetRandNum (4);
        MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].Speed         = (UINT16) GetRandNum (4);
        MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].RankInDimm    = (UINT8)  GetRandNum (2);
        MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].Banks         = (UINT8)  GetRandNum (2);
        MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].BankGroups    = (UINT8)  GetRandNum (2);
        MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].SpdModuleType = (UINT8)  GetRandNum (2);
      }
      MemInfoData->Controller[i].ChannelInfo[j].DimmCount           = (UINT8)  GetRandNum (2);
    }
  }
  for (i = 0; i < HOB_MAX_SAGV_POINTS; i++) {
    MemInfoData->SagvConfigInfo.SaGvTiming[i].DataRate            = (UINT32) GetRandNum (8);
    MemInfoData->SagvConfigInfo.SaGvTiming[i].MaxMemoryBandwidth  = (UINT16) GetRandNum (4);
  }
  MemInfoData->SagvConfigInfo.SaGvPointMask = (UINT32) GetRandNum (8);
  MemInfoData->MaximumMemoryClockSpeed      = (UINT16) GetRandNum (4);

  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));
  DEBUG ((DEBUG_INFO, "|-----------  Memory Info Test Generate Data  ----------|\n"));
  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));
  DEBUG ((DEBUG_INFO, "|  Memory Type                             0x%-10X |\n", MemInfoData->MemoryType));
  for (i = 0; i < MAX_NODE; i++) {
    DEBUG ((DEBUG_INFO, "|  Node          %d                                      |\n", i));
    for (j = 0; j < MAX_CH; j++) {
      DEBUG ((DEBUG_INFO, "|  |  Channel    %d                                      |\n", j));
      for (k = 0; k < MAX_DIMM; k++) {
        DEBUG ((DEBUG_INFO, "|  |  |  DIMM    %d                                      |\n", k));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Manufacture ID                 0x%-10X |\n", MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].MfgId));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Memory Speed                   0x%-10X |\n", MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].Speed));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Rank In DIMM                   0x%-10X |\n", MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].RankInDimm));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Banks In DIMM                  0x%-10X |\n", MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].Banks));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Bank Groups In DIMM            0x%-10X |\n", MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].BankGroups));
        DEBUG ((DEBUG_INFO, "|  |  |  |  Form Factor                    0x%-10X |\n", MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].SpdModuleType));
      }
      DEBUG ((DEBUG_INFO, "|  |  |  DIMM Count                        0x%-10X |\n", MemInfoData->Controller[i].ChannelInfo[j].DimmCount));
      DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));
    }
  }
  for (i = 0; i < HOB_MAX_SAGV_POINTS; i++) {
    DEBUG ((DEBUG_INFO, "|  Sa Gv Point - %d                                |\n", i+1));
    DEBUG ((DEBUG_INFO, "|  |  |  Data Rate                         0x%-10X |\n", MemInfoData->SagvConfigInfo.SaGvTiming[i].DataRate));
    DEBUG ((DEBUG_INFO, "|  |  |  Max Memory Bandwidth              0x%-10X |\n", MemInfoData->SagvConfigInfo.SaGvTiming[i].MaxMemoryBandwidth));
  }
  DEBUG ((DEBUG_INFO, "|  SaGvPointMask                           0x%-10X |\n", MemInfoData->SagvConfigInfo.SaGvPointMask));
  DEBUG ((DEBUG_INFO, "|  Maximum Frequency                       0x%-10X |\n", MemInfoData->MaximumMemoryClockSpeed));
  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));

  // PPR
  MemInfoData->PprRanInLastBoot                     = 0x3; // Set to [Bit2] AMT enabled and [Bit1] PPR Enabled
  MemInfoData->PprDetectedErrors                    = (UINT16) GetRandNum (4);
  MemInfoData->PprRepairsSuccessful                 = (UINT16) GetRandNum (4);
  MemInfoData->PprRepairFails                       = (UINT16) GetRandNum (4);
  MemInfoData->PprErrorInfo.PprRowRepairsSuccessful = (UINT8)  GetRandNum (3);
  MemInfoData->PprErrorInfo.Controller              = (UINT8)  GetRandNum (3);
  MemInfoData->PprErrorInfo.Channel                 = (UINT8)  GetRandNum (2);
  MemInfoData->PprErrorInfo.Rank                    = (UINT8)  GetRandNum (2);
  MemInfoData->PprErrorInfo.BankGroup               = (UINT8)  GetRandNum (2);
  MemInfoData->PprErrorInfo.Bank                    = (UINT8)  GetRandNum (2);
  MemInfoData->PprErrorInfo.Row                     = (UINT32) GetRandNum (8);
  for (i = 0; i < MAX_NODE; i++) {
    for (j = 0; j < MAX_CH; j++) {
      for (k = 0; k < MAX_RANK_IN_CHANNEL; k++) {
        for (l = 0; l < MAX_SDRAM_IN_DIMM; l++) {
          MemInfoData->PprAvailableResources[i][j][k][l]  = (UINT8) GetRandNum (2);
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "|  AmtPprRanInLastBoot                     0x%-10X |\n", MemInfoData->PprRanInLastBoot));
  DEBUG ((DEBUG_INFO, "|  Number of Detected Errors               0x%-10X |\n", MemInfoData->PprDetectedErrors));
  DEBUG ((DEBUG_INFO, "|  Number of Successful Repairs            0x%-10X |\n", MemInfoData->PprRepairsSuccessful));
  DEBUG ((DEBUG_INFO, "|  Number of Uncorrectable Devices         0x%-10X |\n", MemInfoData->PprRepairFails));
  DEBUG ((DEBUG_INFO, "|  Error Repair Successful                 0x%-10X |\n", MemInfoData->PprErrorInfo.PprRowRepairsSuccessful));
  DEBUG ((DEBUG_INFO, "|  Error Controller                        0x%-10X |\n", MemInfoData->PprErrorInfo.Controller));
  DEBUG ((DEBUG_INFO, "|  Error Channel                           0x%-10X |\n", MemInfoData->PprErrorInfo.Channel));
  DEBUG ((DEBUG_INFO, "|  Error Rank                              0x%-10X |\n", MemInfoData->PprErrorInfo.Rank));
  DEBUG ((DEBUG_INFO, "|  Error Bank Group                        0x%-10X |\n", MemInfoData->PprErrorInfo.BankGroup));
  DEBUG ((DEBUG_INFO, "|  Error Bank                              0x%-10X |\n", MemInfoData->PprErrorInfo.Bank));
  DEBUG ((DEBUG_INFO, "|  Error Row                               0x%-10X |\n", MemInfoData->PprErrorInfo.Row));
  for (i = 0; i < MAX_NODE; i++) {
    DEBUG ((DEBUG_INFO, "|  Node          %d                                      |\n", i));
    for (j = 0; j < MAX_CH; j++) {
      DEBUG ((DEBUG_INFO, "|  |  Channel    %d                                      |\n", j));
      for (k = 0; k < MAX_RANK_IN_CHANNEL; k++) {
        DEBUG ((DEBUG_INFO, "|  |  |  Rank    %d                                      |\n", k));
        for (l = 0; l < MAX_SDRAM_IN_DIMM; l++) {
          DEBUG ((DEBUG_INFO, "|  |  |  |  Device    %d                                    |\n", l));
          DEBUG ((DEBUG_INFO, "|  |  |  |  |  Available Resources         0x%-10X |\n", MemInfoData->PprAvailableResources[i][j][k][l]));
        }
      }
    }
  }
  DEBUG ((DEBUG_INFO, "|-------------------------------------------------------|\n"));

  return EFI_SUCCESS;
}

/**
  Verifies the data in MHI table based off the expected MRC Memory Info Data HOB

  @param[in, out] MemInfoData     Memory info data hob structure pointer
  @param[in, out] MhiDataTable    Configured data to MHI table and PHAT append

  @retval     Success             All data table values match
  @retval     Failure             Values in tables are invalid
**/
UNIT_TEST_STATUS
VerifyMhiTable (
  IN MEMORY_INFO_DATA_HOB    *MemInfoData,
  IN MHI_DATA_TABLE          *MhiDataTable
  )
{
  UINT8                   i, j, k, l;

  if (MemInfoData->MemoryType != MhiDataTable->MemoryType) {
    DEBUG ((DEBUG_ERROR, "[FAIL] Failed to load matching data from MRC to MHI\n"));
    return UNIT_TEST_ERROR_TEST_FAILED;
  }
  for (i = 0; i < MAX_NODE; i++) {
    for (j = 0; j < MAX_CH; j++) {
      for (k = 0; k < MAX_DIMM; k++) {
        if ((MhiDataTable->MfgId[i][j][k]       != MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].MfgId) ||
            (MhiDataTable->Speed[i][j][k]       != MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].Speed) ||
            (MhiDataTable->RankInDimm[i][j][k]  != MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].RankInDimm) ||
            (MhiDataTable->Banks[i][j][k]       != MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].Banks) ||
            (MhiDataTable->BankGroups[i][j][k]  != MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].BankGroups) ||
            (MhiDataTable->FormFactor[i][j][k]  != MemInfoData->Controller[i].ChannelInfo[j].DimmInfo[k].SpdModuleType)) {
          DEBUG ((DEBUG_ERROR, "[FAIL] Failed to load matching data from MRC to MHI\n"));
          return UNIT_TEST_ERROR_TEST_FAILED;
        }
      }
      if (MhiDataTable->DimmCount[i][j] != MemInfoData->Controller[i].ChannelInfo[j].DimmCount) {
        DEBUG ((DEBUG_ERROR, "[FAIL] Failed to load matching data from MRC to MHI\n"));
        return UNIT_TEST_ERROR_TEST_FAILED;
      }
    }
  }

  for (i = 0; i < HOB_MAX_SAGV_POINTS; i++) {
    if ((MhiDataTable->DataRate[i]     != MemInfoData->SagvConfigInfo.SaGvTiming[i].DataRate) ||
        (MhiDataTable->Bandwidth[i]     != MemInfoData->SagvConfigInfo.SaGvTiming[i].MaxMemoryBandwidth)) {
      DEBUG ((DEBUG_ERROR, "[FAIL] Failed to load matching data from MRC to MHI\n"));
      return UNIT_TEST_ERROR_TEST_FAILED;
    }
  }

  if ((MhiDataTable->SaGvPointMask != MemInfoData->SagvConfigInfo.SaGvPointMask) ||
      (MhiDataTable->MaxFrequency  != MemInfoData->MaximumMemoryClockSpeed)) {
    DEBUG ((DEBUG_ERROR, "[FAIL] Failed to load matching data from MRC to MHI\n"));
    return UNIT_TEST_ERROR_TEST_FAILED;
  }

  // PPR value check
  if ((MhiDataTable->AmtPprRanInLastBoot.AmtRanLastBoot     != ((MemInfoData->PprRanInLastBoot & BIT1) >> 1)) ||
      (MhiDataTable->AmtPprRanInLastBoot.PprRanLastBoot     != (MemInfoData->PprRanInLastBoot & BIT0)) ||
      (MhiDataTable->TotalRowFailuresDiscovered             != MemInfoData->PprDetectedErrors) ||
      (MhiDataTable->TotalPprRowRepairsSuccessful           != MemInfoData->PprRepairsSuccessful) ||
      (MhiDataTable->TotalUncorrectableDevices              != MemInfoData->PprRepairFails) ||
      (MhiDataTable->PprErrorInfo.PprRowRepairsSuccessful   != MemInfoData->PprErrorInfo.PprRowRepairsSuccessful) ||
      (MhiDataTable->PprErrorInfo.Controller                != MemInfoData->PprErrorInfo.Controller) ||
      (MhiDataTable->PprErrorInfo.Channel                   != MemInfoData->PprErrorInfo.Channel) ||
      (MhiDataTable->PprErrorInfo.Rank                      != MemInfoData->PprErrorInfo.Rank) ||
      (MhiDataTable->PprErrorInfo.BankGroup                 != MemInfoData->PprErrorInfo.BankGroup) ||
      (MhiDataTable->PprErrorInfo.Bank                      != MemInfoData->PprErrorInfo.Bank) ||
      (MhiDataTable->PprErrorInfo.Row                       != MemInfoData->PprErrorInfo.Row)) {
    DEBUG ((DEBUG_ERROR, "[FAIL] Failed to get AMT PPR MRC data into MHI\n"));
    return UNIT_TEST_ERROR_TEST_FAILED;
  }
  for (i = 0; i < MAX_NODE; i++) {
    for (j = 0; j < MAX_CH; j++) {
      for (k = 0; k < MAX_RANK_IN_CHANNEL; k++) {
        for (l = 0; l < MAX_SDRAM_IN_DIMM; l++) {
          if (MhiDataTable->AvailableResources[i][j][k][l] != MemInfoData->PprAvailableResources[i][j][k][l]) {
            DEBUG ((DEBUG_ERROR, "[FAIL] Failed to get AMT PPR MRC data into MHI\n"));
            return UNIT_TEST_ERROR_TEST_FAILED;
          }
        }
      }
    }
  }
  return UNIT_TEST_PASSED;
}

/**
  Unit-Test Function for UpdateMemInfoToMhiPhat.

  @param[in]  Context                            An optional parameter for precondition.

  @retval  UNIT_TEST_PASSED                      Unit test case is passed.
  @retval  UNIT_TEST_ERROR_PREREQUISITE_NOT_MET  Test case should be skipped.
**/
UNIT_TEST_STATUS
EFIAPI
VerifyUpdateMhiTable (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS              Status;
  UNIT_TEST_STATUS        TestStatus;
  MEMORY_INFO_DATA_HOB    *MemInfoData;
  MHI_DATA_TABLE          *MhiDataTable;
  MHI_DATA_TABLE          *MhiDataTableTemp;

  DEBUG ((DEBUG_INFO, "[UnitTestGetAndStoreMemData] Testing MHI get MRC data HOB and Store it in local table for later PHAT publishing...\n"));

  MemInfoData       = NULL;
  MhiDataTable      = NULL;
  MhiDataTableTemp  = NULL;

  //
  // Generate Control MRC Memory Info Data HOB data structure to send
  //
  MemInfoData = (MEMORY_INFO_DATA_HOB *) AllocateZeroPool (sizeof (MEMORY_INFO_DATA_HOB));
  if (MemInfoData == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to allocate memory for Memory Info HOB.\n", __FUNCTION__));
    TestStatus = UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
    goto Exit;
  }

  // Print the generated test table for manual reviewal
  Status = GenMemInfoHob (MemInfoData);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to generate Memory Info HOB.\n", __FUNCTION__));
    TestStatus = UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
    goto Exit;
  }

  // Set memory data to MHI PHAT format
  MhiDataTable = (MHI_DATA_TABLE *) AllocateZeroPool (sizeof (MHI_DATA_TABLE));
  if (MhiDataTable == NULL) {
    DEBUG ((DEBUG_ERROR, "[%a] Failed to allocate memory for MHI data table.\n", __FUNCTION__));
    TestStatus = UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
    goto Exit;
  }

  //
  // Run test for the function: UpdateMemInfoToMhiPhat
  //
  Status = UpdateMemInfoToMhiPhat (MemInfoData, MhiDataTable);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[%a] VerifyUpdateMemInfoToMhiPhat failed with status: %r\n", __FUNCTION__, Status));
    TestStatus = UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
    goto Exit;
  }

  // Compare the passed MRC info to the Stored MemInfo for the table
  TestStatus = VerifyMhiTable (MemInfoData, MhiDataTable);

  Exit:
  // Free memory space
  if (MemInfoData != NULL) {
    FreePool (MemInfoData);
  }

  // Exit with no issues
  DEBUG ((DEBUG_ERROR, "[%r] Unit test %a complete!\n", TestStatus, __FUNCTION__));
  return TestStatus;
}