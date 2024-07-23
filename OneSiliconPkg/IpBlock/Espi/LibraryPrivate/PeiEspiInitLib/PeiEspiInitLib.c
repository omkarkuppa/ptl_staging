/**@file
  This is the code that initializes eSPI.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include <ConfigBlock.h>
#include <EspiConfig.h>
#include <EspiHandle.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/EspiPrivateLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/SpiRegs.h>
#include <Register/EspiRegs.h>
#include <Library/PeiEspiInitLib.h>

/**
  Configure SPI/eSPI power management

  @param[in] EspiConfig - pointer to eSPI config
**/
STATIC
VOID
EspiConfigurePm (
  IN  ESPI_HANDLE       *EspiHandle
  )
{
  UINT32 AndData;
  UINT32 OrData;
  PCH_ESPI_CONFIG   *EspiConfig;

  EspiConfig = EspiHandle->EspiConfig;

  //
  // Configuring SPI and eSPI dynamic clock gating
  OrData = 0x2000F73F;
  if (EspiHandle->EspiPrivateConfig.DynamicClockGatingEnableForEspiIoLogic == TRUE) {
    OrData |= BIT30;
  }
  EspiHandle->EspiPcrAccess->AndThenOr32 (EspiHandle->EspiPcrAccess, R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL, ~0u, OrData);

  if (EspiConfig->EspiPmHAE) {
    //
    // Clear SPI PCR 0xC008 bit 2,1,0
    // Set SPI PCR 0xC008 bit 5
    //
    AndData = (UINT32)~(BIT2 | BIT1 | BIT0);
    OrData  = BIT5;
  } else {
    //
    // Clear SPI PCR 0xC008 bit 5,2,1,0
    //
    AndData = (UINT32)~(BIT5 | BIT2 | BIT1 | BIT0);
    OrData  = 0;
  }
  EspiHandle->EspiPcrAccess->AndThenOr32 (EspiHandle->EspiPcrAccess, R_SPI_PCR_SPI_PWRCNTRL_EN, AndData, OrData);
}

/**
  Enable Parity error handling for eSPI.
**/
VOID
EspiEnableErrorReporting(
  IN  ESPI_HANDLE       *EspiHandle
  )
{
  PCH_ESPI_CONFIG   *EspiConfig;
  UINT32            OrData;

  EspiConfig = EspiHandle->EspiConfig;

  OrData = (V_ESPI_PCR_PCERR_SLVX_PCFEE_SERR << N_ESPI_PCR_XERR_SLVX_XFEE) |
    B_ESPI_PCR_PCERR_SLV0_PCURRE |
    (V_ESPI_PCR_PCERR_SLVX_PCRMTARE_SERR << N_ESPI_PCR_PCERR_SLV0_PCRMTARE);

  if (!EspiConfig->HideNonFatalErrors) {
    OrData |= (V_ESPI_PCR_PCERR_SLVX_PCNFEE_SERR << N_ESPI_PCR_XERR_SLVX_XNFEE);
  }
  //
  //  Set PCERR_SLV0 register for enable error reporting
  //
  EspiHandle->EspiPcrAccess->AndThenOr32 (
    EspiHandle->EspiPcrAccess,
    R_ESPI_PCR_PCERR_SLV0,
    0,
    OrData
  );

  //
  //  Set PCERR_SLV1 register for enable error reporting
  //
  EspiHandle->EspiPcrAccess->AndThenOr32 (
    EspiHandle->EspiPcrAccess,
    R_ESPI_PCR_PCERR_SLV1,
    0,
    OrData
  );

  //
  //  Set FCERR_SLV0 register for enable error reporting
  //
  EspiHandle->EspiPcrAccess->AndThenOr32 (
    EspiHandle->EspiPcrAccess,
    R_ESPI_PCR_FCERR_SLV0,
    (UINT32) ~0,
    ((V_ESPI_PCR_FCERR_SLV0_FCFEE_SERR << N_ESPI_PCR_XERR_SLVX_XFEE) |
    (V_ESPI_PCR_FCERR_SLV0_FCNFEE_SERR << N_ESPI_PCR_XERR_SLVX_XNFEE))
  );

  //
  //  Set VWERR_SLV0 register for enable error reporting
  //
  EspiHandle->EspiPcrAccess->AndThenOr32 (
    EspiHandle->EspiPcrAccess,
    R_ESPI_PCR_VWERR_SLV0,
    (UINT32) ~0,
    ((V_ESPI_PCR_VWERR_SLVX_VWFEE_SERR << N_ESPI_PCR_XERR_SLVX_XFEE) |
    (V_ESPI_PCR_VWERR_SLVX_VWNFEE_SERR << N_ESPI_PCR_XERR_SLVX_XNFEE))
  );

  //
  //  Set VWERR_SLV1 register for enable error reporting
  //
  EspiHandle->EspiPcrAccess->AndThenOr32 (
    EspiHandle->EspiPcrAccess,
    R_ESPI_PCR_VWERR_SLV1,
    (UINT32) ~0,
    ((V_ESPI_PCR_VWERR_SLVX_VWFEE_SERR << N_ESPI_PCR_XERR_SLVX_XFEE) |
    (V_ESPI_PCR_VWERR_SLVX_VWNFEE_SERR << N_ESPI_PCR_XERR_SLVX_XNFEE))
  );

  //
  //  Set LNKERR_SLV0 register for enable error reporting
  //
  EspiHandle->EspiPcrAccess->AndThenOr32 (
    EspiHandle->EspiPcrAccess,
    R_ESPI_PCR_LNKERR_SLV0,
    (UINT32) ~(V_ESPI_PCR_LNKERR_SLV0_LFET1E_SMI << N_ESPI_PCR_LNKERR_SLV0_LFET1E),
    (V_ESPI_PCR_LNKERR_SLV0_LFER1E_SERR << N_ESPI_PCR_LNKERR_SLV0_LFET1E)
  );

  //
  //  Set LNKERR_SLV1 register for enable error reporting
  //
  EspiHandle->EspiPcrAccess->AndThenOr32 (
    EspiHandle->EspiPcrAccess,
    R_ESPI_PCR_LNKERR_SLV1,
    (UINT32) ~(V_ESPI_PCR_LNKERR_SLV1_LFET1E_SMI << N_ESPI_PCR_LNKERR_SLV1_LFET1E),
    (V_ESPI_PCR_LNKERR_SLV1_LFER1E_SERR << N_ESPI_PCR_LNKERR_SLV1_LFET1E)
  );
}

/**
  Modifies CNTLR_PWRGATE_TYMOUT and CLKGATE_HOLDOFF_TYMOUT registers with
  new timeout value

  @param[in] EspiHandle      Pointer to initialized ESPI handle
  @param[in] TimeoutValue    New timeout value that will be programmed
 */
VOID
EspiSetTimeoutValues (
  ESPI_HANDLE       *EspiHandle,
  UINT8             TimeoutValue
  )
{
  UINT32    AndData32;
  UINT32    OrData32;

  AndData32 = (UINT32)~(
    B_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL_CLKGATE_HOLDOFF_TYMOUT |
    B_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL_CNTLR_PWRGATE_TYMOUT);

  OrData32 = (UINT32) (
    ((TimeoutValue & 0xF) << N_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL_CLKGATE_HOLDOFF_TYMOUT) |
    ((TimeoutValue & 0xF) << N_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL_CNTLR_PWRGATE_TYMOUT)
    );

  // Set CDC Power Gate Disable before modifying TYMOUT register
  EspiHandle->EspiPcrAccess->Or32 (
                               EspiHandle->EspiPcrAccess,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL,
                               B_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL_CDCPWRGTDIS
                               );

  // Modify Timeout values
  EspiHandle->EspiPcrAccess->AndThenOr32 (
                               EspiHandle->EspiPcrAccess,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL,
                               AndData32,
                               OrData32
                               );

  // Clear CDC Power Gate Disable after modifying TYMOUT register
  EspiHandle->EspiPcrAccess->And32 (
                               EspiHandle->EspiPcrAccess,
                               R_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL,
                               (UINT32)~(B_SPI_PCR_SPI_CLK_PWR_GATE_CNTRL_CDCPWRGTDIS)
                               );
}

/**
  Configures PCH eSPI

  @param[in] EspiHandle      Pointer to initialized ESPI handle
**/
VOID
EspiInit (
  IN  ESPI_HANDLE       *EspiHandle
  )
{
  UINT32            Data32;
  PCH_ESPI_CONFIG   *EspiConfig;

  DEBUG ((DEBUG_VERBOSE, "[%a] Entry.\n", __FUNCTION__));

  EspiConfig        = EspiHandle->EspiConfig;

  if ((PciSegmentRead32 (EspiHandle->PciCfgBase + R_ESPI_CFG_ESPI_BC) & B_ESPI_CFG_ESPI_BC_ESPI) == 0) {
    DEBUG ((DEBUG_INFO, "[%a] eSPI is disabled through strap\n", __FUNCTION__));
    return;
  }

  if (EspiConfig->HostC10ReportEnable) {
    //
    // If desired enable HOST_C10 reporting in Virtual Wire Channel Error for device 0 (VWERR_SLV0)
    // No need to enable in both device 0 and device 1 because device 1 uses bits from device 0 register
    //
    EspiHandle->EspiPcrAccess->AndThenOr32 (
      EspiHandle->EspiPcrAccess,
      R_ESPI_PCR_VWERR_SLV0,
      (UINT32) ~(B_ESPI_PCR_XERR_SLVX_XNFES | B_ESPI_PCR_XERR_SLVX_XFES),
      B_ESPI_PCR_VWERR_SLV0_VWHC10OE
      );
  }

  if (EspiConfig->BmeHostDeviceEnabled && EspiHandle->EspiPrivateConfig.BmeControl) {
    //
    // Set the BME bit on eSPI-MC
    //
    PciSegmentOr16 (EspiHandle->PciCfgBase + PCI_COMMAND_OFFSET, (UINT32) EFI_PCI_COMMAND_BUS_MASTER);

    //
    // Set the BME bit on eSPI device 0 using the Tunneled Access
    //
    PchEspiDeviceGetConfig (0, R_ESPI_DEVICE_CHA_0_CAP_AND_CONF, EspiHandle->EspiPcrAccess, &Data32);
    Data32 |= B_ESPI_DEVICE_BME;
    PchEspiDeviceSetConfig (0, R_ESPI_DEVICE_CHA_0_CAP_AND_CONF, EspiHandle->EspiPcrAccess, Data32);

    Data32 = EspiHandle->EspiPcrAccess->Read32 (EspiHandle->EspiPcrAccess, R_SPI_PCR_ESPI_SOFTSTRAP_REG0);
    if (Data32 & B_SPI_PCR_ESPI_SOFTSTRAP_REG0_CS1_EN) {
      //
      // Set the BME bit on eSPI device 1 using the Tunneled Access
      //
      PchEspiDeviceGetConfig (1, R_ESPI_DEVICE_CHA_0_CAP_AND_CONF, EspiHandle->EspiPcrAccess, &Data32);
      Data32 |= B_ESPI_DEVICE_BME;
      PchEspiDeviceSetConfig (1, R_ESPI_DEVICE_CHA_0_CAP_AND_CONF, EspiHandle->EspiPcrAccess, Data32);
    }
  }

  if (EspiHandle->EspiPrivateConfig.PmSupport) {
    EspiConfigurePm (EspiHandle);
  }

  if(EspiHandle->EspiPrivateConfig.EspiErrorReporting) {
    EspiEnableErrorReporting (EspiHandle);
  }

  //
  // Set SBLCL bit after the initial eSPI link configuration to lock
  // access to Configuration Registers from offsets 0x0 - 0x7FF
  //
  if (EspiConfig->LockLinkConfiguration) {
    EspiHandle->EspiPcrAccess->AndThenOr32 (
      EspiHandle->EspiPcrAccess,
      (UINT16) R_ESPI_PCR_SLV_CFG_REG_CTL,
      ~0u,
      B_ESPI_PCR_SLV_CFG_REG_CTL_SBLCL
      );
  }

  //
  // Set the ESPI_NMI_EN_CS1 bit to enable eSPI NMI VW events to be processed by the SOC.
  //
  if (EspiConfig->NmiEnableCs1) {
    EspiHandle->EspiPcrAccess->Or32 (
      EspiHandle->EspiPcrAccess,
      R_ESPI_PCR_GENERAL_CONFIG,
      B_ESPI_PCR_NMI_EN_CS1
    );
  }

  DEBUG ((DEBUG_VERBOSE, "[%a] Exit\n", __FUNCTION__));
}
