/** @file
  This function handle the register/unregister of PCH PCIe specific SMI events.

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
#include "PchSmmHelpers.h"
#include <Register/PmcRegs.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PcieSipRegs.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/PchRegs.h>

extern UINT32  mNumOfRootPorts;

//
// PcieRpHotPlug srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SMM_SOURCE_DESC PchPcieSmiRpHotPlugTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_MPC}
      },
      S_PCH_PCIE_CFG_MPC,
      N_PCH_PCIE_CFG_MPC_HPME
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_SMSCS}
      },
      S_PCH_PCIE_CFG_SMSCS,
      N_PCH_PCIE_CFG_SMSCS_HPPDM
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PCI_EXP_SMI_STS
  }
};

//
// PcieRpLinkActive srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SMM_SOURCE_DESC PchPcieSmiRpLinkActiveTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_MPC}
      },
      S_PCH_PCIE_CFG_MPC,
      N_PCH_PCIE_CFG_MPC_HPME
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_SMSCS}
      },
      S_PCH_PCIE_CFG_SMSCS,
      N_PCH_PCIE_CFG_SMSCS_HPLAS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PCI_EXP_SMI_STS
  }
};

//
// PcieRpLinkEq srcdesc
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_SMM_SOURCE_DESC PchPcieSmiRpLinkEqTemplate = {
  PCH_SMM_NO_FLAGS,
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCIE_CFG_EQCFG1}
      },
      S_PCIE_CFG_EQCFG1,
      N_PCIE_CFG_EQCFG1_LERSMIE
    },
    NULL_BIT_DESC_INITIALIZER
  },
  {
    {
      {
        PCIE_ADDR_TYPE,
        {R_PCH_PCIE_CFG_SMSCS}
      },
      S_PCH_PCIE_CFG_SMSCS,
      N_PCH_PCIE_CFG_SMSCS_LERSMIS
    }
  },
  {
    {
      ACPI_ADDR_TYPE,
      {R_ACPI_IO_SMI_STS}
    },
    S_ACPI_IO_SMI_STS,
    N_ACPI_IO_SMI_STS_PCI_EXP_SMI_STS
  }
};

/**
  Get Root Port physical Number by CPU or PCH Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)
**/
VOID
GetPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  )
{
  UINT64 RpBase;

  RpBase = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, RpDev, RpFun, 0);
  *RpNumber = (PciSegmentRead32 (RpBase + R_PCIE_CFG_LCAP) >> N_PCIE_CFG_LCAP_PN) -1;
}

/**
  Get CPU or PCH Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.
**/
VOID
GetPcieRpDevFun (
  IN  UINTN   RpIndex,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  )
{
  *RpDev = PchPcieRpDevNumber (RpIndex);
  *RpFun = PchPcieRpFuncNumber (RpIndex);
}

/**
  For each PCIE RP clear PME SCI status and disable SCI, then PCIEXP_WAKE_STS from PMC.
  This prevents platform from waking more than one time due to a single PCIE wake event.
  Normally it's up to OS to clear SCI statuses. But in a scenario where platform wakes
  and goes to S5 instead of booting to OS, the SCI status would remain set and would trigger another wake.
**/
VOID
ClearPcieSci (
  VOID
  )
{
  UINT32 RpIndex;
  UINT64 RpBase;

  for (RpIndex = 0; RpIndex < mNumOfRootPorts; RpIndex++) {
    RpBase = PchPcieRpPciCfgBase (RpIndex);
    if (PciSegmentRead16 (RpBase + PCI_VENDOR_ID_OFFSET) != 0xFFFF) {
      PciSegmentAnd8 ((RpBase + R_PCH_PCIE_CFG_MPC + 3), (UINT8)~((UINT8)(B_PCH_PCIE_CFG_MPC_PMCE >> 24)));
      PciSegmentWrite32 (RpBase + R_PCH_PCIE_CFG_SMSCS, B_PCH_PCIE_CFG_SMSCS_PMCS);
    }
  }
  IoWrite16 (mAcpiBaseAddr + R_ACPI_IO_PM1_STS, B_ACPI_IO_PM1_STS_PCIEXP_WAKE_STS);
}
