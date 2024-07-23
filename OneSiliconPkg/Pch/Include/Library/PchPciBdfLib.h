/** @file
  Header file for PchPciBdfLib.

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
#ifndef _PCH_PCI_BDF_LIB_H_
#define _PCH_PCI_BDF_LIB_H_

/**
  Get CNVI WiFi controller address that can be passed to the PCI Segment Library functions.

  @retval CNVI WiFi controller address in PCI Segment Library representation
**/
UINT64
CnviWifiPciCfgBase (
  VOID
  );

/**
  Get CNVI Bluetooth controller address that can be passed to the PCI Segment Library functions.

  @retval CNVI Bluetooth controller address in PCI Segment Library representation
**/
UINT64
CnviBtPciCfgBase (
  VOID
  );

/**
  Get CNVI WiFi controller PCIe Device Number

  @retval CNVI WiFi controller PCIe Device Number
**/
UINT8
CnviWifiDevNumber (
  VOID
  );

/**
  Get CNVI WiFi controller PCIe Function Number

  @retval CNVI WiFi controller PCIe Function Number
**/
UINT8
CnviWifiFuncNumber (
  VOID
  );

/**
  Get CNVI Bluetooth controller PCIe Device Number

  @retval CNVI Bluetooth controller PCIe Device Number
**/
UINT8
CnviBtDevNumber (
  VOID
  );

/**
  Get CNVI Bluetooth controller PCIe Function Number

  @retval CNVI Bluetooth controller PCIe Function Number
**/
UINT8
CnviBtFuncNumber (
  VOID
  );

/**
  Get eSPI controller address that can be passed to the PCI Segment Library functions.

  @retval eSPI controller address in PCI Segment Library representation
**/
UINT64
EspiPciCfgBase (
  VOID
  );

/**
  Get eSPI controller PCIe Device Number

  @retval eSPI controller PCIe Device Number
**/
UINT8
EspiDevNumber (
  VOID
  );

/**
  Get eSPI controller PCIe Function Number

  @retval eSPI controller PCIe Function Number
**/
UINT8
EspiFuncNumber (
  VOID
  );

/**
  Get GbE controller address that can be passed to the PCI Segment Library functions.

  @retval GbE controller address in PCI Segment Library representation
**/
UINT64
GbePciCfgBase (
  VOID
  );

/**
  Returns Gigabit Ethernet PCI Device Number

  @retval  GbE device number
**/
UINT8
GbeDevNumber (
  VOID
  );

/**
  Returns Gigabit Ethernet PCI Function Number

  @retval  GbE function number
**/
UINT8
GbeFuncNumber (
  VOID
  );

/**
  Get HDA controller address that can be passed to the PCI Segment Library functions.

  @retval HDA controller address in PCI Segment Library representation
**/
UINT64
HdaPciCfgBase (
  VOID
  );

/**
  Get HDA PCI device number

  @retval PCI dev number
**/
UINT8
HdaDevNumber (
  VOID
  );

/**
  Get HDA PCI function number

  @retval PCI fun number
**/
UINT8
HdaFuncNumber (
  VOID
  );

/**
  Get IEH controller address that can be passed to the PCI Segment Library functions.

  @retval IEH controller address in PCI Segment Library representation
**/
UINT64
IehPciCfgBase (
  VOID
  );

/**
  Get IEH PCI device number

  @retval PCI dev number
**/
UINT8
IehDevNumber (
  VOID
  );

/**
  Get IEH PCI function number

  @retval PCI fun number
**/
UINT8
IehFuncNumber (
  VOID
  );

/**
  Get P2SB controller address that can be passed to the PCI Segment Library functions.

  @retval P2SB controller address in PCI Segment Library representation
**/
UINT64
P2sbPciCfgBase (
  VOID
  );

/**
  Get P2SB 16b controller address that can be passed to the PCI Segment Library functions.

  @retval P2SB controller address in PCI Segment Library representation
**/
UINT64
P2sb16bPciCfgBase (
  VOID
  );

/**
  Get P2SB PCI device number

  @retval PCI dev number
**/
UINT8
P2sbDevNumber (
  VOID
  );

/**
  Get P2SB PCI function number

  @retval PCI fun number
**/
UINT8
P2sbFuncNumber (
  VOID
  );

/**
  Get ISH controller address that can be passed to the PCI Segment Library functions.

  @retval ISH controller address in PCI Segment Library representation
**/
UINT64
IshPciCfgBase (
  VOID
  );

/**
  Get ISH Device Number

  @retval  ISH Device Number
**/
UINT8
IshDevNumber (
  VOID
  );

/**
  Get ISH Function Number

  @retval  ISH Function Number
**/
UINT8
IshFuncNumber (
  VOID
  );

/**
  Get PCH Trace Hub PCI config space base address

  @retval PCI config space base address
**/
UINT64
PchTraceHubPciCfgBase (
  VOID
  );

/**
  Get PCH Trace Hub PCI device number

  @retval PCI dev number
**/
UINT8
PchTraceHubDevNumber (
  VOID
  );

/**
  Get PCH Trace Hub PCI function number

  @retval PCI fun number
**/
UINT8
PchTraceHubFuncNumber (
  VOID
  );

/**
  Get PMC controller address that can be passed to the PCI Segment Library functions.

  @retval PMC controller address in PCI Segment Library representation
**/
UINT64
PmcPciCfgBase (
  VOID
  );

/**
  Get PMC PCI device number

  @retval PCI dev number
**/
UINT8
PmcDevNumber (
  VOID
  );

/**
  Get PMC PCI function number

  @retval PCI fun number
**/
UINT8
PmcFuncNumber (
  VOID
  );

/**
  Get PMC SSRAM controller address that can be passed to the PCI Segment Library functions.

  @retval PMC SSRAM controller address in PCI Segment Library representation
**/
UINT64
PmcSsramPciCfgBase (
  VOID
  );

/**
  Get TSN controller address that can be passed to the PCI Segment Library functions.

  @param[in]  TsnIndex     Index of the Tsn controller

  @retval TSN controller address in PCI Segment Library representation
**/
UINT64
TsnPciCfgBase (
  IN  UINT8  TsnIndex
  );

/**
  Get Tsn PCI device number

  @param[in]  TsnIndex     Index of the Tsn controller

  @retval PCI dev number
**/
UINT8
TsnDevNumber (
  IN  UINT8  TsnIndex
  );

/**
  Get Tsn PCI function number

  @param[in]  TsnIndex     Index of the Tsn controller

  @retval PCI fun number
**/
UINT8
TsnFuncNumber (
  IN  UINT8  TsnIndex
  );

/**
  Returns SPI PCI Config Space base address

  @retval  UINT64  SPI Config Space base address
**/
UINT64
SpiPciCfgBase (
  VOID
  );

/**
  Returns SPI Device number

  @retval UINT8   PCH SPI Device number
**/
UINT8
SpiDevNumber (
  VOID
  );

/**
  Returns SPI Function number

  @retval UINT8   PCH SPI Function number
**/
UINT8
SpiFuncNumber (
  VOID
  );

/**
  Get XHCI controller address that can be passed to the PCI Segment Library functions.

  @retval XHCI controller address in PCI Segment Library representation
**/
UINT64
PchXhciPciCfgBase (
  VOID
  );

/**
  Get XHCI controller PCIe Device Number

  @retval XHCI controller PCIe Device Number
**/
UINT8
PchXhciDevNumber (
  VOID
  );

/**
  Get XHCI controller PCIe Function Number

  @retval XHCI controller PCIe Function Number
**/
UINT8
PchXhciFuncNumber (
  VOID
  );

/**
  Get XDCI controller address that can be passed to the PCI Segment Library functions.

  @retval XDCI controller address in PCI Segment Library representation
**/
UINT64
PchXdciPciCfgBase (
  VOID
  );

/**
  Get XDCI controller PCIe Device Number

  @retval XDCI controller PCIe Device Number
**/
UINT8
PchXdciDevNumber (
  VOID
  );

/**
  Get XDCI controller PCIe Function Number

  @retval XDCI controller PCIe Function Number
**/
UINT8
PchXdciFuncNumber (
  VOID
  );

/**
  Get SMBUS controller address that can be passed to the PCI Segment Library functions.

  @retval SMBUS controller address in PCI Segment Library representation
**/
UINT64
SmbusPciCfgBase (
  VOID
  );

/**
  Return DMA Smbus Device Number

  @retval DMA Smbus Device Number
**/
UINT8
SmbusDmaDevNumber (
  VOID
  );

/**
  Return DMA Smbus Function Number

  @retval  DMA Smbus Function Number
**/
UINT8
SmbusDmaFuncNumber (
  VOID
  );

/**
  Get DMA SMBUS controller address that can be passed to the PCI Segment Library functions.

  @retval DMA SMBUS controller address in PCI Segment Library representation
**/
UINT64
SmbusDmaPciCfgBase (
  VOID
  );

/**
  Return Smbus Device Number

  @retval Smbus Device Number
**/
UINT8
SmbusDevNumber (
  VOID
  );

/**
  Return Smbus Function Number

  @retval  Smbus Function Number
**/
UINT8
SmbusFuncNumber (
  VOID
  );

/**
  Get SCS SD Card controller address that can be passed to the PCI Segment Library functions.

  @retval SCS SD Card controller address in PCI Segment Library representation
**/
UINT64
ScsSdCardPciCfgBase (
  VOID
  );

/**
  Get SD Card controller PCIe Device Number

  @retval SD Card controller PCIe Device Number
**/
UINT8
ScsSdCardDevNumber (
  VOID
  );

/**
  Get SD Card controller PCIe Function Number

  @retval SD Card controller PCIe Function Number
**/
UINT8
ScsSdCardFuncNumber (
  VOID
  );

/**
  Get SCS EMMC controller address that can be passed to the PCI Segment Library functions.

  @retval SCS EMMC controller address in PCI Segment Library representation
**/
UINT64
ScsEmmcPciCfgBase (
  VOID
  );

/**
  Get EMMC controller PCIe Device Number

  @retval EMMC controller PCIe Device Number
**/
UINT8
ScsEmmcDevNumber (
  VOID
  );

/**
  Get EMMC controller PCIe Function Number

  @retval EMMC controller PCIe Function Number
**/
UINT8
ScsEmmcFuncNumber (
  VOID
  );

/**
  Get UFS controller PCIe Device Number

  @param[in]  UfsIndex     Index of the UFS controller

  @retval UFS controller PCIe Device Number
**/
UINT8
ScsUfsDevNumber (
  IN  UINT8  UfsIndex
  );

/**
  Get UFS controller PCIe Function Number

  @param[in]  UfsIndex     Index of the UFS controller

  @retval UFS controller PCIe Function Number
**/
UINT8
ScsUfsFuncNumber (
  IN  UINT8  UfsIndex
  );

/**
  Get SCS EMMC controller address that can be passed to the PCI Segment Library functions.

  @param[in]  UfsIndex     Index of the UFS controller

  @retval SCS EMMC controller address in PCI Segment Library representation
**/
UINT64
ScsUfsPciCfgBase (
  IN  UINT8  UfsIndex
  );

/**
  Gets SATA controller PCIe config space base address

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe config space base address
**/
UINT64
SataPciCfgBase (
  IN UINT32 SataCtrlIndex
  );

/**
  Gets SATA controller PCIe Device Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Device Number
**/
UINT8
SataDevNumber (
  IN UINT32 SataCtrlIndex
  );

/**
  Gets SATA controller PCIe Function Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval SATA controller PCIe Function Number
**/
UINT8
SataFuncNumber (
  IN UINT32 SataCtrlIndex
  );

/**
  Returns PCH LPC device PCI base address.

  @retval                   PCH LPC PCI base address.
**/
UINT64
LpcPciCfgBase (
  VOID
  );

/**
  Get LPC controller PCIe Device Number

  @retval LPC controller PCIe Device Number
**/
UINT8
LpcDevNumber (
  VOID
  );

/**
  Get Thermal Device PCIe Device Number

  @retval Thermal Device PCIe Device Number
**/
UINT8
ThermalDevNumber (
  VOID
  );

/**
  Get Thermal Device PCIe Function Number

  @retval Thermal Device PCIe Function Number
**/
UINT8
ThermalFuncNumber (
  VOID
  );

/**
  Returns Thermal Device PCI base address.

  @retval                   Thermal Device PCI base address.
**/
UINT64
ThermalPciCfgBase (
  VOID
  );

/**
  Get LPC controller PCIe Function Number

  @retval LPC controller PCIe Function Number
**/
UINT8
LpcFuncNumber (
  VOID
  );

/**
  Get LPSS I2C controller PCIe Device Number

  @param[in]  I2cNumber       LPSS I2C controller index

  @retval LPSS I2C controller PCIe Device Number
**/
UINT8
LpssI2cDevNumber (
  IN UINT8       I2cNumber
  );

/**
  Get LPSS I2C controller PCIe Function Number

  @param[in]  I2cNumber       LPSS I2C controller index

  @retval LPSS I2C controller PCIe Function Number
**/
UINT8
LpssI2cFuncNumber (
  IN UINT8       I2cNumber
  );

/**
  Get LPSS I2C controller address that can be passed to the PCI Segment Library functions.

  @param[in]  I2cNumber       LPSS I2C controller index

  @retval LPSS I2C controller address in PCI Segment Library representation
**/
UINT64
LpssI2cPciCfgBase (
  IN UINT8        I2cNumber
  );

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
  );

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
  );

/**
  Get LPSS I3C controller address that can be passed to the PCI Segment Library functions.

  @param[in]  I3cNumber       LPSS I3C interface number (zero based)

  @retval LPSS I3C controller address in PCI Segment Library representation
**/
UINT64
LpssI3cPciCfgBase (
  IN UINT8        I3cNumber
  );

/**
  Get LPSS SPI controller PCIe Device Number

  @param[in]  I2cNumber       LPSS SPI controller index

  @retval LPSS SPI controller PCIe Device Number
**/
UINT8
LpssSpiDevNumber (
  IN UINT8       SpiNumber
  );

/**
  Get LPSS SPI controller PCIe Function Number

  @param[in]  SpiNumber       LPSS SPI controller index

  @retval LPSS SPI controller PCIe Function Number
**/
UINT8
LpssSpiFuncNumber (
  IN UINT8       SpiNumber
  );

/**
  Get LPSS SPI controller address that can be passed to the PCI Segment Library functions.

  @param[in]  SpiNumber       LPSS SPI controller index

  @retval LPSS SPI controller address in PCI Segment Library representation
**/
UINT64
LpssSpiPciCfgBase (
  IN UINT8        SpiNumber
  );

/**
  Get LPSS UART controller PCIe Device Number

  @param[in]  UartNumber       LPSS UART controller index

  @retval LPSS UART controller PCIe Device Number
**/
UINT8
LpssUartDevNumber (
  IN UINT8       UartNumber
  );

/**
  Get LPSS UART controller PCIe Function Number

  @param[in]  UartNumber       LPSS UART controller index

  @retval LPSS UART controller PCIe Function Number
**/
UINT8
LpssUartFuncNumber (
  IN UINT8       UartNumber
  );

/**
  Get LPSS UART controller address that can be passed to the PCI Segment Library functions.

  @param[in]  UartNumber       LPSS UART controller index

  @retval LPSS UART controller address in PCI Segment Library representation
**/
UINT64
LpssUartPciCfgBase (
  IN UINT8        UartNumber
  );

/**
  Get PCH PCIe controller PCIe Device Number

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Device Number
**/
UINT8
PchPcieRpDevNumber (
  IN  UINTN   RpIndex
  );

/**
  Get PCH PCIe controller PCIe Function Number

  @param[in]  RpIndex       Root port physical number. (0-based)

  @retval PCH PCIe controller PCIe Function Number
**/
UINT8
PchPcieRpFuncNumber (
  IN  UINTN   RpIndex
  );

/**
  Get PCH PCIe controller address that can be passed to the PCI Segment Library functions.

  @param[in]  RpIndex       PCH PCIe Root Port physical number. (0-based)

  @retval PCH PCIe controller address in PCI Segment Library representation
**/
UINT64
PchPcieRpPciCfgBase (
  IN  UINT32   RpIndex
  );

/**
  Get Touch Host Controller PCIe Device Number

  @param[in]  ThcNumber       THC controller index

  @retval THC controller PCIe Device Number
**/
UINT8
ThcDevNumber (
  IN UINT8       ThcNumber
  );

/**
  Get Touch Host Controller PCIe Function Number

  @param[in]  ThcNumber       THC controller index

  @retval THC controller PCIe Function Number
**/
UINT8
ThcFuncNumber (
  IN UINT8       ThcNumber
  );

/**
  Get Touch Host Controller address that can be passed to the PCI Segment Library functions.

  @param[in]  ThcNumber       THC controller index

  @retval THC controller address in PCI Segment Library representation
**/
UINT64
ThcPciCfgBase (
  IN UINT8        ThcNumber
  );

/**
  Get CAN PCIe Device Number

  @param[in]  CanIndex       CAN controller index

  @retval CAN controller PCIe Device Number
**/
UINT8
CanDevNumber (
  IN UINT8       CanIndex
  );

/**
  Get CAN PCIe Function Number

  @param[in]  CanIndex       CAN controller index

  @retval CAN controller PCIe Function Number
**/
UINT8
CanFuncNumber (
  IN UINT8       CanIndex
  );

/**
  Get CAN address that can be passed to the PCI Segment Library functions.

  @param[in]  CanIndex       CAN controller index

  @retval CAN controller address in PCI Segment Library representation
**/
UINT64
CanPciCfgBase (
  IN UINT8        CanIndex
  );

#endif //_PCH_PCI_BDF_LIB_H_
