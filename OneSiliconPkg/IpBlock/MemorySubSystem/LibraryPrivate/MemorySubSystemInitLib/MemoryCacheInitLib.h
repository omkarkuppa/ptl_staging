/** @file
  Memory Cache Init Library header.

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


#define SAGV_CONFIG_HANDLER_ID                           0x22
#define NCLK_MAILBOX_SUBCOMMAND_GET_CONFIGURATION_ID     0x10
#define NCLK_MAILBOX_SUBCOMMAND_SET_CONFIGURATION_ID     0x11

#define D2DCLK_MAILBOX_SUBCOMMAND_GET_CONFIGURATION_ID   0x20
#define D2DCLK_MAILBOX_SUBCOMMAND_SET_CONFIGURATION_ID   0x21

#define MSCLK_MAILBOX_SUBCOMMAND_GET_CONFIGURATION_ID    0x30
#define MSCLK_MAILBOX_SUBCOMMAND_SET_CONFIGURATION_ID    0x31

typedef union _FABRIC_GV_CONFIGURATION_DATA {
  UINT32 Data;
  struct {
    UINT32 IsFabricGvEn    : 32;   ///<Fabric GV. 0: is enabled, 0xFFF: is disabled>
  } Fields;
} FABRIC_GV_CONFIGURATION_DATA;

/**
  Memory cache initalization.

  @param[in]  SiPreMemPolicyPpi  The SI PreMem Policy PPI instance

  @retval     EFI_SUCCES         Initialized successfully
**/
EFI_STATUS
PeiMemoryCacheInit (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );

EFI_STATUS
PeiFabricGvInit(
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  );