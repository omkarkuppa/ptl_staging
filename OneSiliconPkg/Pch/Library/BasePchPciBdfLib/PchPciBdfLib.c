/** @file
  PCH PCIe Bus Device Function Library.
  All functions from this library are available in PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PchPcieRpLib.h>
#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Library/SataSocLib.h>

/**
  Check if a Device is present for PCH FRU
  If the data is defined for PCH RFU return it
  If the data is not defined (Device is NOT present) assert.

  @param[in]  DataToCheck       Device or Function number to check

  @retval Device or Function number value if defined for PCH FRU
          0xFF if not present in PCH FRU
**/
UINT8
CheckAndReturn (
  UINT8 DataToCheck
  )
{
  if (DataToCheck == NOT_PRESENT) {
    ASSERT (FALSE);
  }
  return DataToCheck;
}

/**
  Get CNVI WiFi controller PCIe Device Number

  @retval CNVI WiFi controller PCIe Device Number
**/
UINT8
CnviWifiDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_CNVI_WIFI);
}

/**
  Get CNVI WiFi controller PCIe Function Number

  @retval CNVI WiFi controller PCIe Function Number
**/
UINT8
CnviWifiFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_CNVI_WIFI);
}

/**
  Get CNVI Bluetooth controller PCIe Device Number

  @retval CNVI Bluetooth controller PCIe Device Number
**/
UINT8
CnviBtDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_CNVI_BT);
}

/**
  Get CNVI Bluetooth controller PCIe Function Number

  @retval CNVI Bluetooth controller PCIe Function Number
**/
UINT8
CnviBtFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_CNVI_BT);
}

/**
  Get CNVI WiFi controller address that can be passed to the PCI Segment Library functions.

  @retval CNVI WiFi controller address in PCI Segment Library representation
**/
UINT64
CnviWifiPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            CnviWifiDevNumber (),
            CnviWifiFuncNumber (),
            0
            );
}

/**
  Get CNVI Bluetooth controller address that can be passed to the PCI Segment Library functions.

  @retval CNVI Bluetooth controller address in PCI Segment Library representation
**/
UINT64
CnviBtPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            CnviBtDevNumber (),
            CnviBtFuncNumber (),
            0
            );
}

/**
  Get eSPI controller address that can be passed to the PCI Segment Library functions.

  @retval eSPI controller address in PCI Segment Library representation
**/
UINT64
EspiPciCfgBase (
  VOID
  )
{
  ASSERT (PCI_DEVICE_NUMBER_PCH_ESPI != NOT_PRESENT);

  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_ESPI,
           PCI_FUNCTION_NUMBER_PCH_ESPI,
           0
           );
}

/**
  Returns Gigabit Ethernet PCI Device Number

  @retval  GbE device number
**/
UINT8
GbeDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_GBE);
}

/**
  Returns Gigabit Ethernet PCI Function Number

  @retval  GbE function number
**/
UINT8
GbeFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_GBE);
}

/**
  Get GbE controller address that can be passed to the PCI Segment Library functions.

  @retval GbE controller address in PCI Segment Library representation
**/
UINT64
GbePciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            GbeDevNumber (),
            GbeFuncNumber (),
            0
            );
}

/**
  Get HDA PCI device number

  @retval PCI dev number
**/
UINT8
HdaDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_HDA);
}

/**
  Get HDA PCI function number

  @retval PCI fun number
**/
UINT8
HdaFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_HDA);
}

/**
  Get HDA controller address that can be passed to the PCI Segment Library functions.

  @retval HDA controller address in PCI Segment Library representation
**/
UINT64
HdaPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            HdaDevNumber (),
            HdaFuncNumber (),
            0
            );
}

/**
  Get IEH PCI device number

  @retval PCI dev number
**/
UINT8
IehDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_IEH);
}

/**
  Get IEH PCI function number

  @retval PCI fun number
**/
UINT8
IehFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_IEH);
}

/**
  Get IEH controller address that can be passed to the PCI Segment Library functions.

  @retval IEH controller address in PCI Segment Library representation
**/
UINT64
IehPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            IehDevNumber (),
            IehFuncNumber (),
            0
            );
}

/**
  Get P2SB PCI device number

  @retval PCI dev number
**/
UINT8
P2sbDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_P2SB);
}

/**
  Get P2SB PCI function number

  @retval PCI fun number
**/
UINT8
P2sbFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_P2SB);
}

/**
  Get P2SB controller address that can be passed to the PCI Segment Library functions.

  @retval P2SB controller address in PCI Segment Library representation
**/
UINT64
P2sbPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           P2sbDevNumber (),
           P2sbFuncNumber (),
           0
           );
}

/**
  Get P2SB 16b PCI device number

  @retval PCI dev number
**/
UINT8
P2sb16bDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_SECOND_P2SB);
}

/**
  Get P2SB 16b PCI function number

  @retval PCI fun number
**/
UINT8
P2sb16bFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_SECOND_P2SB);
}

/**
  Get P2SB 16b controller address that can be passed to the PCI Segment Library functions.

  @retval P2SB controller address in PCI Segment Library representation
**/
UINT64
P2sb16bPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           P2sb16bDevNumber (),
           P2sb16bFuncNumber (),
           0
           );
}

/**
  Get ISH Device Number

  @retval  ISH Device Number
**/
UINT8
IshDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_ISH);
}

/**
  Get ISH Function Number

  @retval  ISH Function Number
**/
UINT8
IshFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_ISH);
}

/**
  Get ISH controller address that can be passed to the PCI Segment Library functions.

  @retval ISH controller address in PCI Segment Library representation
**/
UINT64
IshPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            IshDevNumber (),
            IshFuncNumber (),
            0
            );
}

/**
  Get PCH Trace Hub PCI device number

  @retval PCI dev number
**/
UINT8
PchTraceHubDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_TRACE_HUB);
}

/**
  Get PCH Trace Hub PCI function number

  @retval PCI fun number
**/
UINT8
PchTraceHubFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_TRACE_HUB);
}

/**
  Get PCH Trace Hub PCI config space base address

  @retval PCI config space base address
**/
UINT64
PchTraceHubPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PchTraceHubDevNumber (),
          PchTraceHubFuncNumber (),
          0
          );
}

/**
  Get PMC PCI device number

  @retval PCI dev number
**/
UINT8
PmcDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_PMC);
}

/**
  Get PMC PCI function number

  @retval PCI fun number
**/
UINT8
PmcFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_PMC);
}

/**
  Get PMC controller address that can be passed to the PCI Segment Library functions.

  @retval PMC controller address in PCI Segment Library representation
**/
UINT64
PmcPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PmcDevNumber (),
            PmcFuncNumber (),
            0
            );
}

/**
  Get PMC SSRAM controller address that can be passed to the PCI Segment Library functions.

  @retval PMC SSRAM controller address in PCI Segment Library representation
**/
UINT64
PmcSsramPciCfgBase (
  VOID
  )
{
  ASSERT (PCI_DEVICE_NUMBER_PCH_PMC_SSRAM != NOT_PRESENT);

  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PCI_DEVICE_NUMBER_PCH_PMC_SSRAM,
            PCI_FUNCTION_NUMBER_PCH_PMC_SSRAM,
            0
            );
}

/**
  Get TSN controller PCIe Device Number

  @param[in]  TsnIndex     Index of the Tsn controller

  @retval Tsn controller PCIe Device Number
**/
UINT8
TsnDevNumber (
  IN  UINT8  TsnIndex
  )
{
  switch (TsnIndex) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_TSN1);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_TSN2);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_TSN3);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_TSN4);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get TSN controller PCIe Function Number

  @param[in]  TsnIndex     Index of the Tsn controller

  @retval Tsn controller PCIe Function Number
**/
UINT8
TsnFuncNumber (
  IN  UINT8  TsnIndex
  )
{
  switch (TsnIndex) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_TSN1);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_TSN2);
    case 2:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_TSN3);
    case 3:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_TSN4);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get TSN controller address that can be passed to the PCI Segment Library functions.

  @param[in]  TsnIndex     Index of the Tsn controller

  @retval TSN controller address in PCI Segment Library representation
**/
UINT64
TsnPciCfgBase (
  IN  UINT8  TsnIndex
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            TsnDevNumber (TsnIndex),
            TsnFuncNumber (TsnIndex),
            0
            );
}

/**
  Returns PCH SPI Device number

  @retval UINT8   PCH SPI Device number
**/
UINT8
SpiDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SPI);
}

/**
  Returns PCH SPI Function number

  @retval UINT8   PCH SPI Function number
**/
UINT8
SpiFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SPI);
}

/**
  Returns PCH SPI PCI Config Space base address

  @retval  UINT64  PCH SPI Config Space base address
**/
UINT64
SpiPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SpiDevNumber (),
           SpiFuncNumber (),
           0
           );
}

/**
  Get XHCI controller PCIe Device Number

  @retval XHCI controller PCIe Device Number
**/
UINT8
PchXhciDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_XHCI);
}

/**
  Get XHCI controller PCIe Function Number

  @retval XHCI controller PCIe Function Number
**/
UINT8
PchXhciFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_XHCI);
}

/**
  Get XHCI controller address that can be passed to the PCI Segment Library functions.

  @retval XHCI controller address in PCI Segment Library representation
**/
UINT64
PchXhciPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PchXhciDevNumber (),
          PchXhciFuncNumber (),
          0
          );
}

/**
  Get XDCI controller PCIe Device Number

  @retval XDCI controller PCIe Device Number
**/
UINT8
PchXdciDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_XDCI);
}

/**
  Get XDCI controller PCIe Function Number

  @retval XDCI controller PCIe Function Number
**/
UINT8
PchXdciFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_XDCI);
}

/**
  Get XDCI controller address that can be passed to the PCI Segment Library functions.

  @retval XDCI controller address in PCI Segment Library representation
**/
UINT64
PchXdciPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PchXdciDevNumber (),
          PchXdciFuncNumber (),
          0
          );
}

/**
  Return Smbus Device Number

  @retval Smbus Device Number
**/
UINT8
SmbusDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SMBUS);
}

/**
  Return Smbus Function Number

  @retval  Smbus Function Number
**/
UINT8
SmbusFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SMBUS);
}

/**
  Get SMBUS controller address that can be passed to the PCI Segment Library functions.

  @retval SMBUS controller address in PCI Segment Library representation
**/
UINT64
SmbusPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            SmbusDevNumber (),
            SmbusFuncNumber (),
            0
            );
}

/**
  Return DMA Smbus Device Number

  @retval DMA Smbus Device Number
**/
UINT8
SmbusDmaDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_DMA_SMBUS);
}

/**
  Return DMA Smbus Function Number

  @retval  DMA Smbus Function Number
**/
UINT8
SmbusDmaFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_DMA_SMBUS);
}

/**
  Get DMA SMBUS controller address that can be passed to the PCI Segment Library functions.

  @retval DMA SMBUS controller address in PCI Segment Library representation
**/
UINT64
SmbusDmaPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            SmbusDmaDevNumber (),
            SmbusDmaFuncNumber (),
            0
            );
}

/**
  Get SD Card controller PCIe Device Number

  @retval SD Card controller PCIe Device Number
**/
UINT8
ScsSdCardDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SCS_SDCARD);
}

/**
  Get SD Card controller PCIe Function Number

  @retval SD Card controller PCIe Function Number
**/
UINT8
ScsSdCardFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SCS_SDCARD);
}

/**
  Get SCS SD Card controller address that can be passed to the PCI Segment Library functions.

  @retval SCS SD Card controller address in PCI Segment Library representation
**/
UINT64
ScsSdCardPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          ScsSdCardDevNumber (),
          ScsSdCardFuncNumber (),
          0
          );
}

/**
  Get EMMC controller PCIe Device Number

  @retval EMMC controller PCIe Device Number
**/
UINT8
ScsEmmcDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SCS_EMMC);
}

/**
  Get EMMC controller PCIe Function Number

  @retval EMMC controller PCIe Function Number
**/
UINT8
ScsEmmcFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SCS_EMMC);
}

/**
  Get SCS EMMC controller address that can be passed to the PCI Segment Library functions.

  @retval SCS EMMC controller address in PCI Segment Library representation
**/
UINT64
ScsEmmcPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          ScsEmmcDevNumber (),
          ScsEmmcFuncNumber (),
          0
          );
}

/**
  Get UFS controller PCIe Device Number

  @param[in]  UfsIndex     Index of the UFS controller

  @retval UFS controller PCIe Device Number
**/
UINT8
ScsUfsDevNumber (
  IN  UINT8  UfsIndex
  )
{
  switch (UfsIndex) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SCS_UFS0);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SCS_UFS1);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get UFS controller PCIe Function Number

  @param[in]  UfsIndex     Index of the UFS controller

  @retval UFS controller PCIe Function Number
**/
UINT8
ScsUfsFuncNumber (
  IN  UINT8  UfsIndex
  )
{
  switch (UfsIndex) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SCS_UFS0);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SCS_UFS1);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get SCS EMMC controller address that can be passed to the PCI Segment Library functions.

  @param[in]  UfsIndex     Index of the UFS controller

  @retval SCS EMMC controller address in PCI Segment Library representation
**/
UINT64
ScsUfsPciCfgBase (
  IN  UINT8  UfsIndex
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            ScsUfsDevNumber (UfsIndex),
            ScsUfsFuncNumber (UfsIndex),
            0
            );
}

/**
  Get SATA controller PCIe Device Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Device Number
**/
UINT8
SataDevNumber (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < (UINT32) MaxSataControllerNum ());

  if (SataCtrlIndex == 0) {
    return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SATA_1);
  } else if (SataCtrlIndex == 1) {
    return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SATA_2);
  } else if (SataCtrlIndex == 2) {
    return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SATA_3);
  } else {
    ASSERT (FALSE);
    return 0xFF;
  }
}

/**
  Get SATA controller PCIe Function Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Function Number
**/
UINT8
SataFuncNumber (
  IN UINT32  SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < (UINT32) MaxSataControllerNum ());

  if (SataCtrlIndex == 0) {
    return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SATA_1);
  } else if (SataCtrlIndex == 1) {
    return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SATA_2);
  } else if (SataCtrlIndex == 2) {
    return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SATA_3);
  } else {
    ASSERT (FALSE);
    return 0xFF;
  }
}

/**
  Get SATA controller address that can be passed to the PCI Segment Library functions.

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller address in PCI Segment Library representation
**/
UINT64
SataPciCfgBase (
  IN UINT32  SataCtrlIndex
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           SataDevNumber (SataCtrlIndex),
           SataFuncNumber (SataCtrlIndex),
           0
           );
}

/**
  Get LPC controller PCIe Device Number

  @retval LPC controller PCIe Device Number
**/
UINT8
LpcDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_LPC);
}

/**
  Get LPC controller PCIe Function Number

  @retval LPC controller PCIe Function Number
**/
UINT8
LpcFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_LPC);
}

/**
  Returns PCH LPC device PCI base address.

  @retval                   PCH LPC PCI base address.
**/
UINT64
LpcPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           LpcDevNumber (),
           LpcFuncNumber (),
           0
           );
}

/**
  Get Thermal Device PCIe Device Number

  @retval Thermal Device PCIe Device Number
**/
UINT8
ThermalDevNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_THERMAL);
}

/**
  Get Thermal Device PCIe Function Number

  @retval Thermal Device PCIe Function Number
**/
UINT8
ThermalFuncNumber (
  VOID
  )
{
  return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_THERMAL);
}

/**
  Returns Thermal Device PCI base address.

  @retval                   Thermal Device PCI base address.
**/
UINT64
ThermalPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           ThermalDevNumber (),
           ThermalFuncNumber (),
           0
           );
}

/**
  Get Serial IO I2C controller PCIe Device Number

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller PCIe Device Number
**/
UINT8
LpssI2cDevNumber (
  IN UINT8       I2cNumber
  )
{
  if (GetMaxI2cInterfacesNum () <= I2cNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (I2cNumber) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C0);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C1);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C2);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C3);
    case 4:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C4);
    case 5:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C5);
    case 6:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C6);
    case 7:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_I2C7);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO I2C controller PCIe Function Number

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller PCIe Function Number
**/
UINT8
LpssI2cFuncNumber (
  IN UINT8       I2cNumber
  )
{
  if (GetMaxI2cInterfacesNum () <= I2cNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (I2cNumber) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C0);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C1);
    case 2:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C2);
    case 3:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C3);
    case 4:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C4);
    case 5:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C5);
    case 6:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C6);
    case 7:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_I2C7);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Returns PCIe Device Number for given LPSS I3C interface
  Note:
  There are two I3C interfaces per Slice/Controller.
  Each Slice/Controller is represented by separate PCIe Function
  Example:
  I3C 0: Slice/Ctrl 0, Interface 0 -> BDF 0:17:0
  I3C 1: Slice/Ctrl 0, Interface 1 -> BDF 0:17:0
  I3C 2: Slice/Ctrl 1, Interface 0 -> BDF 0:17:1
  I3C 3: Slice/Ctrl 1, Interface 1 -> BDF 0:17:1

  @param[in]  I3CNumber   LPSS I3C interface number (zero based)

  @retval PCIe Device Number for given LPSS I3C interface
**/
UINT8
LpssI3cDevNumber (
  IN UINT8       I3cNumber
  )
{
  if (GetMaxI3cInterfacesNum () <= I3cNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (I3cNumber) {
    case 0:
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_LPSS_I3C_CTRL_1);
    case 2:
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_LPSS_I3C_CTRL_2);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Returns PCIe Function Number for given LPSS I3C interface
  Note:
  There are two I3C interfaces per Slice/Controller
  Each Slice/Controller is represented by separate PCIe Function
  Example:
  I3C 0: Slice/Ctrl 0, Interface 0 -> BDF 0:17:0
  I3C 1: Slice/Ctrl 0, Interface 1 -> BDF 0:17:0
  I3C 2: Slice/Ctrl 1, Interface 0 -> BDF 0:17:1
  I3C 3: Slice/Ctrl 1, Interface 1 -> BDF 0:17:1

  @param[in]  I3CNumber   LPSS I3C interface number (zero based)

  @retval PCIe Function Number for given LPSS I3C interface
**/
UINT8
LpssI3cFuncNumber (
  IN UINT8       I3cNumber
  )
{
  if (GetMaxI3cInterfacesNum () <= I3cNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (I3cNumber) {
    case 0:
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_LPSS_I3C_CTRL_1);
    case 2:
    case 3:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_LPSS_I3C_CTRL_2);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}


/**
  Get Serial IO I2C controller address that can be passed to the PCI Segment Library functions.

  @param[in]  I2cNumber       Serial IO I2C controller index

  @retval Serial IO I2C controller address in PCI Segment Library representation
**/
UINT64
LpssI2cPciCfgBase (
  IN UINT8        I2cNumber
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           LpssI2cDevNumber (I2cNumber),
           LpssI2cFuncNumber (I2cNumber),
           0
           );
}

/**
  Get LPSS I3C controller address that can be passed to the PCI Segment Library functions.

  @param[in]  I3cNumber       LPSS I3C interface number (zero based)

  @retval LPSS I3C controller address in PCI Segment Library representation
**/
UINT64
LpssI3cPciCfgBase (
  IN UINT8        I3cNumber
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           LpssI3cDevNumber (I3cNumber),
           LpssI3cFuncNumber (I3cNumber),
           0
           );
}

/**
  Get Serial IO SPI controller PCIe Device Number

  @param[in]  I2cNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller PCIe Device Number
**/
UINT8
LpssSpiDevNumber (
  IN UINT8       SpiNumber
  )
{
  if (GetMaxSpiInterfacesNum () <= SpiNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  if (IsPchH ()) {
    switch (SpiNumber) {
      case 0:
        return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0);
      case 1:
        return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1);
      case 2:
        return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI2);
      case 3:
        return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI3);
      case 4:
        return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI4);
      case 5:
        return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_H_SERIAL_IO_SPI5);
      case 6:
        return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_H_SERIAL_IO_SPI6);
      default:
        ASSERT (FALSE);
        return 0xFF;
    }
  }
  switch (SpiNumber) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI0);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI1);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI2);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI3);
    case 4:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI4);
    case 5:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI5);
    case 6:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_SPI6);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO SPI controller PCIe Function Number

  @param[in]  SpiNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller PCIe Function Number
**/
UINT8
LpssSpiFuncNumber (
  IN UINT8       SpiNumber
  )
{
  if (GetMaxSpiInterfacesNum () <= SpiNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  if (IsPchH ()) {
    switch (SpiNumber) {
      case 0:
        return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0);
      case 1:
        return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1);
      case 2:
        return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI2);
      case 3:
        return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI3);
      case 4:
        return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI4);
      case 5:
        return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_H_SERIAL_IO_SPI5);
      case 6:
        return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_H_SERIAL_IO_SPI6);
      default:
        ASSERT (FALSE);
        return 0xFF;
    }
  }
  switch (SpiNumber) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI0);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI1);
    case 2:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI2);
    case 3:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI3);
    case 4:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI4);
    case 5:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI5);
    case 6:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_SPI6);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO SPI controller address that can be passed to the PCI Segment Library functions.

  @param[in]  SpiNumber       Serial IO SPI controller index

  @retval Serial IO SPI controller address in PCI Segment Library representation
**/
UINT64
LpssSpiPciCfgBase (
  IN UINT8        SpiNumber
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           LpssSpiDevNumber (SpiNumber),
           LpssSpiFuncNumber (SpiNumber),
           0
           );
}

/**
  Get Serial IO UART controller PCIe Device Number

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller PCIe Device Number
**/
UINT8
LpssUartDevNumber (
  IN UINT8       UartNumber
  )
{
  if (GetMaxUartInterfacesNum () <= UartNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (UartNumber) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART0);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART1);
    case 2:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART2);
    case 3:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART3);
    case 4:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART4);
    case 5:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART5);
    case 6:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_SERIAL_IO_UART6);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO UART controller PCIe Function Number

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller PCIe Function Number
**/
UINT8
LpssUartFuncNumber (
  IN UINT8       UartNumber
  )
{
  if (GetMaxUartInterfacesNum () <= UartNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (UartNumber) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART0);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART1);
    case 2:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART2);
    case 3:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART3);
    case 4:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART4);
    case 5:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART5);
    case 6:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_SERIAL_IO_UART6);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Serial IO UART controller address that can be passed to the PCI Segment Library functions.

  @param[in]  UartNumber       Serial IO UART controller index

  @retval Serial IO UART controller address in PCI Segment Library representation
**/
UINT64
LpssUartPciCfgBase (
  IN UINT8        UartNumber
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           LpssUartDevNumber (UartNumber),
           LpssUartFuncNumber (UartNumber),
           0
           );
}

/**
  Get PCH PCIe controller PCIe Device Number

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Device Number
**/
UINT8
PchPcieRpDevNumber (
  IN  UINTN   RpIndex
  )
{
  UINTN   Device;
  UINTN   Function;

  GetPchPcieRpDevFun (RpIndex, &Device, &Function);

  return (UINT8)Device;
}

/**
  Get PCH PCIe controller PCIe Function Number
  Note:
  For Client PCH generations Function Number can be various
  depending on "Root Port Function Swapping". For such cases
  Function Number  MUST be obtain from proper register.
  For Server PCHs we have no "Root Port Function Swapping"
  and we can return fixed Function Number.
  To address this difference in this, PCH generation independent,
  library we should call specific function in PchPcieRpLib.

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Function Number
**/
UINT8
PchPcieRpFuncNumber (
  IN  UINTN   RpIndex
  )
{
  UINTN   Device;
  UINTN   Function;

  GetPchPcieRpDevFun (RpIndex, &Device, &Function);

  return (UINT8)Function;
}

/**
  Get PCH PCIe controller address that can be passed to the PCI Segment Library functions.

  @param[in]  RpIndex       PCH PCIe Root Port physical number. (0-based)

  @retval PCH PCIe controller address in PCI Segment Library representation
**/
UINT64
PchPcieRpPciCfgBase (
  IN  UINT32   RpIndex
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
          DEFAULT_PCI_SEGMENT_NUMBER_PCH,
          DEFAULT_PCI_BUS_NUMBER_PCH,
          PchPcieRpDevNumber (RpIndex),
          PchPcieRpFuncNumber (RpIndex),
          0
          );
}

/**
  Get Touch Host Controller PCIe Device Number

  @param[in]  ThcNumber       THC controller index

  @retval THC controller PCIe Device Number
**/
UINT8
ThcDevNumber (
  IN UINT8       ThcNumber
  )
{
  if (GetPchMaxThcCount () <= ThcNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (ThcNumber) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_THC0);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_THC1);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Touch Host Controller PCIe Function Number

  @param[in]  ThcNumber       THC controller index

  @retval THC controller PCIe Function Number
**/
UINT8
ThcFuncNumber (
  IN UINT8       ThcNumber
  )
{
  if (GetPchMaxThcCount () <= ThcNumber) {
    ASSERT (FALSE);
    return 0xFF;
  }
  switch (ThcNumber) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_THC0);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_THC1);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get Touch Host Controller address that can be passed to the PCI Segment Library functions.

  @param[in]  ThcNumber       THC controller index

  @retval THC controller address in PCI Segment Library representation
**/
UINT64
ThcPciCfgBase (
  IN UINT8        ThcNumber
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           ThcDevNumber (ThcNumber),
           ThcFuncNumber (ThcNumber),
           0
           );
}

/**
  Get CAN PCIe Device Number

  @param[in]  CanIndex       CAN controller index

  @retval CAN controller PCIe Device Number
**/
UINT8
CanDevNumber (
  IN UINT8       CanIndex
  )
{
  switch (CanIndex) {
    case 0:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_CAN1);
    case 1:
      return CheckAndReturn (PCI_DEVICE_NUMBER_PCH_CAN2);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get CAN PCIe Function Number

  @param[in]  CanIndex       CAN controller index

  @retval CAN controller PCIe Function Number
**/
UINT8
CanFuncNumber (
  IN UINT8       CanIndex
  )
{
  switch (CanIndex) {
    case 0:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_CAN1);
    case 1:
      return CheckAndReturn (PCI_FUNCTION_NUMBER_PCH_CAN2);
    default:
      ASSERT (FALSE);
      return 0xFF;
  }
}

/**
  Get CAN address that can be passed to the PCI Segment Library functions.

  @param[in]  CanIndex       CAN controller index

  @retval CAN controller address in PCI Segment Library representation
**/
UINT64
CanPciCfgBase (
  IN UINT8        CanIndex
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           CanDevNumber (CanIndex),
           CanFuncNumber (CanIndex),
           0
           );
}
