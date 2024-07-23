/** @file
  PEI DTBT Init library.

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

#include <Library/PeiDTbtInitLib.h>
#include <Ppi/PeiDTbtPolicy.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <TcssDataHob.h>
#include <TbtMailBoxCmdDefinition.h>

#define LTSSM_FSM_RESTORE 0xC74

/**
  The Task Table for DTBT
**/
GLOBAL_REMOVE_IF_UNREFERENCED TBT_CALL_TABLE_ENTRY  DTbtCallTable[] = {
  ///
  /// The functions are executed in the following order, as the policy flag dictates.
  /// TbtTask, TbtBootModeFlag, debug_string
  ///
  { RetrieveDTbtRpBdf,         (TBT_S4 | TBT_NORMAL),    "RetrieveDTbtRpBdf"              },
  { DTbtSetMaxPayloadSize,     TBT_ALL,                  "Send_SetMaxPayloadSize_Mailbox" },
  { DTbtSetTPch25Timing,       TBT_ALL,                  "DTbtSetTPch25Timing"            },
  { DTbtClearVgaRegisters,     TBT_ALL,                  "DTbtClearVgaRegisters"          },
  { PassDTbtPolicyToHob,       (TBT_S4 | TBT_NORMAL),    "Pass_DTbt_Policy_To_Hob"        },
  { NULL,                      TBT_NULL,                 "END_OF_TASK"                    }
};

/**
  To retrieve RP bus/dev/func number.
  @param[in]  VOID             PeiTbtConfig
  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
RetrieveDTbtRpBdf (
  IN  VOID  *PeiTbtConfig
)
{
  EFI_STATUS                       Status;
  UINT8                            Index;
  PEI_DTBT_POLICY                  *PeiDTbtConfig;

  DEBUG ((DEBUG_INFO, "RetrieveDTbtRpBdf call Inside\n"));

  PeiDTbtConfig = PeiTbtConfig;
  Status        = EFI_SUCCESS;

  for (Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
    if (PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn == 1) {
      Status = GetDTbtRpDevFun (PeiDTbtConfig->DTbtControllerConfig[Index].RpType,
                                PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpNumber,
                                &PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpBus,
                                &PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpDev,
                                &PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpFunc);
    }
  }

  DEBUG ((DEBUG_INFO, "RetrieveDTbtRpBdf call Return\n"));

  return Status;
}


/**
  Execute Mail box command "Set Max Payload Size".

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS            The function completes successfully
  @retval     EFI_UNSUPPORTED        dTBT is not supported.
  @retval     EFI_INVALID_PARAMETER  PeiTbtConfig is NULL
**/
EFI_STATUS
EFIAPI
DTbtSetMaxPayloadSize (
  IN  VOID  *PeiTbtConfig
)
{
  EFI_STATUS       Status;
  UINT32           OrgBusNumberConfiguration;
  UINT8            RpBus;
  UINT8            RpDev;
  UINT8            RpFunc;
  UINT8            Index;
  UINT8            TbtMpsData;
  UINT8            DtbtTempBus;
  PEI_DTBT_POLICY  *PeiDTbtConfig;

  DEBUG ((DEBUG_INFO, "DTbtSetMaxPayloadSize call Inside\n"));

  Status        = EFI_SUCCESS;
  PeiDTbtConfig = PeiTbtConfig;
  DtbtTempBus   = FixedPcdGet8 (PcdDTbtTempBusNumber);

  if (PeiTbtConfig == NULL){
    DEBUG ((DEBUG_ERROR, "DTbtSetMaxPayloadSize PeiTbtConfig is NULL\n"));
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  for (Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
    if (PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn == 1) {
      RpBus  = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpBus;
      RpDev  = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpDev;
      RpFunc = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpFunc;

      OrgBusNumberConfiguration = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET));
      //
      // Set Sec/Sub buses to a temporary value
      //
      PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (0, RpBus, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), (UINT32)((DtbtTempBus << 16) | (DtbtTempBus << 8)));

      //
      // Maximum payload size can be switch thru sending command through PCIE2TBT register with different data sending.
      // Meta data is used to select between 128B to 256B :
      //     0 - mapped to 128B, default
      //     1 - mapped to 256B
      //
      TbtMpsData = (PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpMps == 1) ? PCIE2TBT_MAX_PAYLOAD_SIZE_DATA_256 : PCIE2TBT_MAX_PAYLOAD_SIZE_DATA_128;

      //
      // Execute Mail box command "Set Max Payload Size"
      //
      if (SetMaxPayloadSize (TbtMpsData, DtbtTempBus, 0, 0, TBT_5S_TIMEOUT)) {
        DEBUG ((DEBUG_INFO, "DTbtSetMaxPayloadSize - Set DtbtController[%d] MPS attribute to %X\n", Index, TbtMpsData));
      } else {
        DEBUG ((DEBUG_ERROR, "DTbtSetMaxPayloadSize - Set Max Payload Size message sent failed \n"));
      }

      //
      // Reset Sec/Sub buses to original value
      //
      PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (0, 0, RpDev, RpFunc, PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET), OrgBusNumberConfiguration);
    }
  }

  DEBUG ((DEBUG_INFO, "DTbtSetMaxPayloadSize call Return\n"));
  return Status;
}

/**
  set tPCH25 Timing to 10 ms for DTBT.

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtSetTPch25Timing (
  IN  VOID  *PeiTbtConfig
)
{
  DEBUG ((DEBUG_INFO, "DTbtSetTPch25Timing call Inside\n"));

  //
  //During boot, reboot and wake  tPCH25 Timing should be set to 10 ms
  //
  PmcSetTPch25Timing(PmcTPch25_10ms);

  DEBUG((DEBUG_INFO, "DTbtSetTPch25Timing call Return\n"));
  return EFI_SUCCESS;
}

/**
  Clear VGA Registers for DTBT.

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
DTbtClearVgaRegisters (
  IN  VOID  *PeiTbtConfig
)
{
  EFI_STATUS      Status;
  UINT8           Index;
  PEI_DTBT_POLICY *PeiDTbtConfig;

  DEBUG ((DEBUG_INFO, "DTbtClearVgaRegisters call Inside\n"));

  Status        = EFI_SUCCESS;
  PeiDTbtConfig = PeiTbtConfig;

  for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
    if (PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn == 1) {
      //
      // VGA Enable and VGA 16-bit decode registers of Bridge control register of Root port where
      // Host router resides should be cleaned
      //
      if (IS_DTBT_RP_NUM_VALID (PeiDTbtConfig->DTbtControllerConfig, Index)) {
        TbtClearVgaRegisters (0x00,
          (UINTN) PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpBus,
          (UINTN) PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpDev,
          (UINTN) PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpFunc
          );
      }
    }
  }

  DEBUG ((DEBUG_INFO, "DTbtClearVgaRegisters call Return\n"));
  return Status;
}

/**
  This function pass PEI DTbt Policy to Hob.

  @param[in]  VOID             PeiTbtConfig

  @retval     EFI_SUCCESS      The function completes successfully
  @retval     EFI_UNSUPPORTED  dTBT is not supported.
**/
EFI_STATUS
EFIAPI
PassDTbtPolicyToHob (
  IN  VOID  *PeiTbtConfig
)
{
  EFI_STATUS            Status;
  UINT8                 Index;
  DTBT_INFO_HOB         *DTbtInfoHob;
  PEI_DTBT_POLICY       *PeiDTbtConfig;

  DEBUG((DEBUG_INFO, "PassDTbtPolicyToHob - Start\n"));

  PeiDTbtConfig = PeiTbtConfig;
  DTbtInfoHob = GetFirstGuidHob (&gDTbtInfoHobGuid);
  if (DTbtInfoHob == NULL) {
    //
    // Create HOB for DTBT Data
    //
    Status = PeiServicesCreateHob (
               EFI_HOB_TYPE_GUID_EXTENSION,
               sizeof (DTBT_INFO_HOB),
               (VOID **) &DTbtInfoHob
               );
    DEBUG ((DEBUG_INFO, "DTbtInfoHob Created \n"));
    ASSERT_EFI_ERROR (Status);
  }

  if (DTbtInfoHob != NULL) {
    //
    // Initialize the DTBT INFO HOB data.
    //
    DTbtInfoHob->EfiHobGuidType.Name = gDTbtInfoHobGuid;

    //
    // Update DTBT Policy to Hob
    //
    for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++) {
      DTbtInfoHob->DTbtControllerConfig[Index].DTbtControllerEn = PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn;
      DTbtInfoHob->DTbtControllerConfig[Index].RpType           = PeiDTbtConfig->DTbtControllerConfig[Index].RpType;
      DTbtInfoHob->DTbtControllerConfig[Index].PcieRpNumber     = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpNumber;
      DTbtInfoHob->DTbtControllerConfig[Index].RpAcpiNumber     = PeiDTbtConfig->DTbtControllerConfig[Index].RpAcpiNumber;
      DTbtInfoHob->DTbtControllerConfig[Index].PcieRpBus        = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpBus;
      DTbtInfoHob->DTbtControllerConfig[Index].PcieRpDev        = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpDev;
      DTbtInfoHob->DTbtControllerConfig[Index].PcieRpFunc       = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpFunc;
    }
    DTbtInfoHob->DTbtGenericConfig.Usb4CmMode = PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode;
    DTbtInfoHob->DTbtGenericConfig.Usb4ClassOption = PeiDTbtConfig->DTbtGenericConfig.Usb4ClassOption;
    DTbtInfoHob->DTbtGenericConfig.TbtRtd3PepEnable = PeiDTbtConfig->DTbtGenericConfig.TbtRtd3PepEnable;
  } else {
    return EFI_NOT_FOUND;
  }

  DEBUG((DEBUG_INFO, "PassDTbtPolicyToHob - End\n"));

  return EFI_SUCCESS;
}

