/** @file
  PCIe Config Space access lib- produces PCIe Conf Space access protocol

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcieConfigAccessLib.h>
#include <IndustryStandard/Pci30.h>

/**
  Reads a 8-bit PCI configuration register

  @param  This        Pointer to the REGISTER_ACCESS
  @param  Offset      Offset of the register within the PCI config

  @return The 8-bit PCI configuration register specified by Address
**/
UINT8
PcieConfigRead8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (
                            PcieDevice->Seg,
                            PcieDevice->Bus,
                            PcieDevice->Dev,
                            PcieDevice->Fun,
                            Offset));
}

/**
  Writes a 8-bit PCI configuration register

  @param  This        Pointer to the REGISTER_ACCESS
  @param  Offset      Offset of the register within the PCI config
  @param  Value       The value to write

  @return The parameter of Value
**/
UINT8
PcieConfigWrite8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            Value
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentWrite8 (PCI_SEGMENT_LIB_ADDRESS (
                             PcieDevice->Seg,
                             PcieDevice->Bus,
                             PcieDevice->Dev,
                             PcieDevice->Fun,
                             Offset),
                             Value);
}

/**
  Performs an 8-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 8-bit register value written to register
**/
UINT8
PcieConfigOr8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            OrData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentOr8 (PCI_SEGMENT_LIB_ADDRESS (
                          PcieDevice->Seg,
                          PcieDevice->Bus,
                          PcieDevice->Dev,
                          PcieDevice->Fun,
                          Offset),
                          OrData);
}

/**
  Performs an 8-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 8-bit register value written to register
**/
UINT8
PcieConfigAnd8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            AndData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentAnd8 (PCI_SEGMENT_LIB_ADDRESS (
                           PcieDevice->Seg,
                           PcieDevice->Bus,
                           PcieDevice->Dev,
                           PcieDevice->Fun,
                           Offset),
                           AndData);
}

/**
  Performs an 8-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 8-bit register value written to register
**/
UINT8
PcieConfigAndThenOr8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            AndData,
  IN UINT8            OrData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentAndThenOr8 (PCI_SEGMENT_LIB_ADDRESS (
                                 PcieDevice->Seg,
                                 PcieDevice->Bus,
                                 PcieDevice->Dev,
                                 PcieDevice->Fun,
                                 Offset),
                                 AndData,
                                 OrData);
}

/**
  Reads a 16-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 16-bit register value specified by Offset
**/
UINT16
PcieConfigRead16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (
                             PcieDevice->Seg,
                             PcieDevice->Bus,
                             PcieDevice->Dev,
                             PcieDevice->Fun,
                             Offset));
}

/**
  Writes a 16-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] Value   Value to write to register

  @return The 16-bit register value written to register
**/
UINT16
PcieConfigWrite16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           Value
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentWrite16 (PCI_SEGMENT_LIB_ADDRESS (
                              PcieDevice->Seg,
                              PcieDevice->Bus,
                              PcieDevice->Dev,
                              PcieDevice->Fun,
                              Offset),
                              Value);
}

/**
  Performs a 16-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
UINT16
PcieConfigOr16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           OrData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentOr16 (PCI_SEGMENT_LIB_ADDRESS (
                           PcieDevice->Seg,
                           PcieDevice->Bus,
                           PcieDevice->Dev,
                           PcieDevice->Fun,
                           Offset),
                           OrData);
}

/**
  Performs a 16-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 16-bit register value written to register
**/
UINT16
PcieConfigAnd16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           AndData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentAnd16 (PCI_SEGMENT_LIB_ADDRESS (
                            PcieDevice->Seg,
                            PcieDevice->Bus,
                            PcieDevice->Dev,
                            PcieDevice->Fun,
                            Offset),
                            AndData);
}

/**
  Performs a 16-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
UINT16
PcieConfigAndThenOr16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           AndData,
  IN UINT16           OrData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentAndThenOr16 (PCI_SEGMENT_LIB_ADDRESS (
                                  PcieDevice->Seg,
                                  PcieDevice->Bus,
                                  PcieDevice->Dev,
                                  PcieDevice->Fun,
                                  Offset),
                                  AndData,
                                  OrData);
}

/**
  Reads a 32-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 32-bit register value specified by Offset
**/
UINT32
PcieConfigRead32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (
                             PcieDevice->Seg,
                             PcieDevice->Bus,
                             PcieDevice->Dev,
                             PcieDevice->Fun,
                             Offset));
}

/**
  Writes a 32-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] Value   Value to write to register

  @return The 32-bit register value written to register
**/
UINT32
PcieConfigWrite32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           Value
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentWrite32 (PCI_SEGMENT_LIB_ADDRESS (
                              PcieDevice->Seg,
                              PcieDevice->Bus,
                              PcieDevice->Dev,
                              PcieDevice->Fun,
                              Offset),
                              Value);
}

/**
  Performs a 32-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
UINT32
PcieConfigOr32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           OrData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentOr32 (PCI_SEGMENT_LIB_ADDRESS (
                           PcieDevice->Seg,
                           PcieDevice->Bus,
                           PcieDevice->Dev,
                           PcieDevice->Fun,
                           Offset),
                           OrData);
}

/**
  Performs a 32-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 32-bit register value written to register
**/
UINT32
PcieConfigAnd32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentAnd32 (PCI_SEGMENT_LIB_ADDRESS (
                            PcieDevice->Seg,
                            PcieDevice->Bus,
                            PcieDevice->Dev,
                            PcieDevice->Fun,
                            Offset),
                            AndData);
}

/**
  Performs a 32-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
UINT32
PcieConfigAndThenOr32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData,
  IN UINT32           OrData
  )
{
  PCI_CONFIG_SPACE_ACCESS   *PcieDevice;

  PcieDevice = (PCI_CONFIG_SPACE_ACCESS*)This;

  return PciSegmentAndThenOr32 (PCI_SEGMENT_LIB_ADDRESS (
                                  PcieDevice->Seg,
                                  PcieDevice->Bus,
                                  PcieDevice->Dev,
                                  PcieDevice->Fun,
                                  Offset),
                                  AndData,
                                  OrData);
}

/**
  Initializes PCI_CONFIG_SPACE_ACCESS protocol
  based on Segment/Bus/Device/Function.

  @param[in]  Seg                     PCIe Segment
  @param[in]  Bus                     PCIe Bus
  @param[in]  Dev                     PCIe Device
  @param[in]  Fun                     PCIe Function
  @param[out] PcieConfigSpaceAccess   Structure to be initialized

  @retval TRUE   Initialization successful
  @retval FALSE  Failed to initialize
**/
BOOLEAN
PciConfigAccessInit (
  IN UINT8                      Seg,
  IN UINT8                      Bus,
  IN UINT8                      Dev,
  IN UINT8                      Fun,
  OUT PCI_CONFIG_SPACE_ACCESS   *PcieConfigSpaceAccess
  )
{
  PcieConfigSpaceAccess->RegAccess.Read8        = PcieConfigRead8;
  PcieConfigSpaceAccess->RegAccess.Write8       = PcieConfigWrite8;
  PcieConfigSpaceAccess->RegAccess.Or8          = PcieConfigOr8;
  PcieConfigSpaceAccess->RegAccess.And8         = PcieConfigAnd8;
  PcieConfigSpaceAccess->RegAccess.AndThenOr8   = PcieConfigAndThenOr8;
  PcieConfigSpaceAccess->RegAccess.Read16       = PcieConfigRead16;
  PcieConfigSpaceAccess->RegAccess.Write16      = PcieConfigWrite16;
  PcieConfigSpaceAccess->RegAccess.Or16         = PcieConfigOr16;
  PcieConfigSpaceAccess->RegAccess.And16        = PcieConfigAnd16;
  PcieConfigSpaceAccess->RegAccess.AndThenOr16  = PcieConfigAndThenOr16;
  PcieConfigSpaceAccess->RegAccess.Read32       = PcieConfigRead32;
  PcieConfigSpaceAccess->RegAccess.Write32      = PcieConfigWrite32;
  PcieConfigSpaceAccess->RegAccess.Or32         = PcieConfigOr32;
  PcieConfigSpaceAccess->RegAccess.And32        = PcieConfigAnd32;
  PcieConfigSpaceAccess->RegAccess.AndThenOr32  = PcieConfigAndThenOr32;

  PcieConfigSpaceAccess->Seg = Seg;
  PcieConfigSpaceAccess->Bus = Bus;
  PcieConfigSpaceAccess->Dev = Dev;
  PcieConfigSpaceAccess->Fun = Fun;

  return TRUE;
}