/** @file
  Initializes Integrated Sensor Hub Controller (ISH 3.0).

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Library/PchLimits.h>
#include <Library/PeiIshInitLib.h>
#include <Register/IshRegs.h>

/**
  Configures ISH interrupts

  @param[in] IshDevice  ISH Device
  @param[in] InterruptPin
  @param[in] Irq
**/
VOID
IshConfigureInterrupts (
  IN  ISH_HANDLE      *IshDevice,
  IN  UINT8           InterruptPin,
  IN  UINT8           Irq
  )
{
  UINT32    Data32Or;
  UINT32    Data32And;

  Data32Or =  (UINT32) ((InterruptPin << N_ISH_PCR_PCICFGCTR1_IPIN1) | (Irq << N_ISH_PCR_PCICFGCTR1_PCI_IRQ1));
  Data32And =~(UINT32) (B_ISH_PCR_PCICFGCTR1_PCI_IRQ1 | B_ISH_PCR_PCICFGCTR1_ACPI_IRQ1 | B_ISH_PCR_PCICFGCTR1_IPIN1);

  IshDevice->Access->SidebandMmio->AndThenOr32 (
    IshDevice->Access->SidebandMmio,
    R_ISH_PCR_PCICFGCTR1,
    Data32And,
    Data32Or
    );
}

/**
  Configure in PCI mode - disable BAR1 (Shadowed PCI)
  Set PCICFGCTR.BAR1_DISABLE (PCR[ISHBR] + 200h[7] = 1b)
  Read back is needed to enforce the sideband and primary ordering.

  @param[in] IshDevice  ISH Device

  @retval EFI_SUCCESS
**/
STATIC
VOID
IshBar1Disable (
  IN  ISH_HANDLE  *IshHandle
  )
{
  IshHandle->Access->SidebandMmio->AndThenOr32 (
    IshHandle->Access->SidebandMmio,
    R_ISH_PCR_PCICFGCTR1,
    (UINT32) ~0,
    B_ISH_PCR_PCICFGCTR1_BAR1_DISABLE1
    );
}

/**
  Configure ISH Power Management

  @param[in] IshHandle  ISH Device

  @retval  EFI_SUCCESS
**/
STATIC
VOID
IshPmConfigure (
  IN  ISH_HANDLE  *IshHandle
  )
{
  UINT32 Data32Or;
  Data32Or = (
    B_ISH_PCR_PMCTL_AXI_TRUNK_GATE_EN |
    B_ISH_PCR_PMCTL_IOSF_SB_TRUNK_GATE_EN |
    B_ISH_PCR_PMCTL_IOSF_PRIM_TRUNK_GATE_EN |
    B_ISH_PCR_PMCTL_AXI_LOCAL_GATE_EN |
    B_ISH_PCR_PMCTL_IOSF_PRIM_LOCAL_GATE_EN |
    B_ISH_PCR_PMCTL_IOSF_SB_LOCAL_GATE_EN
    );
  //
  // Power management programming
  // Set PMCTL = 3Fh (PCR[ISHBR] + 1D0h[5:0] = 111111b)
  //
  IshHandle->Access->SidebandMmio->AndThenOr32 (
    IshHandle->Access->SidebandMmio,
    R_ISH_PCR_PMCTL,
    (UINT32) ~(0),
    Data32Or
    );
}

/**
  Set D0Idle Max Power On Latency register (RWO)

  @param[in] IshHandle       ISH_HANDLE
**/
STATIC
VOID
IshSetMpo (
  ISH_HANDLE *IshHandle
  )
{
  IshHandle->Access->PcieConfig->And16 (
    IshHandle->Access->PcieConfig,
    R_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG,
    (UINT16)~(B_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_POW_LAT_VALUE | B_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_POW_LAT_SCALE)
  );
}

/**
  Set Hardware Autonomous Enable to allow ISH to independently enter/exit IPAPG states
  Set PCE.HAE (PCI[ISH] + A2h[5] = 1b)

  @param[in] IshHandle       ISH_HANDLE
**/
STATIC
VOID
IshSetHae (
  ISH_HANDLE *IshHandle
  )
{
  IshHandle->Access->PcieConfig->Or32 (
    IshHandle->Access->PcieConfig,
    R_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG,
    B_ISH_CFG_D0I3_MAX_POW_LAT_PG_CONFIG_HAE
  );
}

/**
  Configures Integrated Sensor Hub (ISH) Controller

  @param[in] IshHandle       ISH_HANDLE
**/
VOID
IshConfigure (
  IN  ISH_HANDLE      *IshHandle
  )
{
  DEBUG ((DEBUG_INFO, "IshConfigure() Start\n"));
  DEBUG ((DEBUG_INFO, "ISH: BAR1 - Shadowed PCI Config Space Disabled\n"));

  IshBar1Disable (IshHandle);

  //
  // Disable ISH BAR1 in PSF
  //
  IshHandle->Callbacks->DisableDeviceBar ();
  IshPmConfigure (IshHandle);
  IshSetMpo (IshHandle);
  IshSetHae (IshHandle);
  IshHandle->Callbacks->SetGpio (IshHandle);
  IshHandle->Callbacks->ConfigureInterrupts (IshHandle);

  DEBUG ((DEBUG_INFO, "IshConfigure() End\n"));
}

/**
  Put ISH into D3 power state

  @param[in] IshHandle  ISH_HANDLE
**/
STATIC
VOID
IshPutIntoD3 (
  IN  ISH_HANDLE  *IshHandle
  )
{
  IshHandle->Access->PcieConfig->Or32 (
    IshHandle->Access->PcieConfig,
    R_ISH_CFG_PMECTRLSTATUS,
    B_ISH_CFG_PMECTRLSTATUS_POWERSTATE
  );
}

/**
  Sets Integrated Sensor Hub (ISH) PSF Function Disable

  @param[in]  IshHandle  ISH_HANDLE
**/
VOID
IshDisable (
  ISH_HANDLE  *IshHandle
  )
{
  DEBUG ((DEBUG_INFO, "IshDisable() Start\n"));

  IshPutIntoD3 (IshHandle);

  IshHandle->Callbacks->Disable (IshHandle);

  DEBUG ((DEBUG_INFO, "IshDisable() End \n"));
}
