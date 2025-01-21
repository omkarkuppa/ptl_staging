/** @file
  This file contains PEI PCH DMI methods

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#ifndef _PEI_PCH_DMI_LIB_H_
#define _PEI_PCH_DMI_LIB_H_

#include <Ppi/SiPolicy.h>

//
// Data structure definitions
//
typedef enum {
  PchDmiVcTypeVc0,
  PchDmiVcTypeVc1,
  PchDmiVcTypeVcm,
  PchDmiVcTypeMax
} PCH_DMI_VC_TYPE;

typedef enum {
  PchDmiTcTypeTc0,
  PchDmiTcTypeTc1,
  PchDmiTcTypeTc2,
  PchDmiTcTypeTc3,
  PchDmiTcTypeTc4,
  PchDmiTcTypeTc5,
  PchDmiTcTypeTc6,
  PchDmiTcTypeTc7,
  PchDmiTcTypeMax
} PCH_TC_VC_MAP;

/**
  The data structure to describe DMI TC configuration setting
**/
typedef struct {
  PCH_DMI_VC_TYPE       Vc;       ///< The Virtual Channel to which the TC is mapped
  UINT8                 TcId;     ///< Tc ID Encoding
} PCH_DMI_TC_CONFIG;

/**
  The data structure to describe DMI VC configuration setting
**/
typedef struct {
  BOOLEAN               Enable;   ///< 0: Disable; 1: Enable
  UINT8                 VcId;     ///< Vc ID Encoding for the Virtual Channel
} PCH_DMI_VC_CONFIG;

/**
  PCH_DMI_TC_VC_MAP Structure Definition
  Note: The default DMI TC/VC mapping will be used if it's not initialized
**/
typedef struct {
  PCH_DMI_TC_CONFIG DmiTc[PchDmiTcTypeMax];  ///< Configures PCH DMI Traffic class mapping.
  PCH_DMI_VC_CONFIG DmiVc[PchDmiVcTypeMax];  ///< Configures PCH DMI Virtual Channel setting.
} PCH_DMI_TC_VC_MAP;

/**
  Get PCH default TC VC Mapping settings. This funciton returns the default PCH setting
  System Agent can update the settings according to polices.

  @param[in, out] PchDmiTcVcMap         Buffer for PCH_DMI_TC_VC_MAP instance.

**/
VOID
PchDmiTcVcMapInit (
  IN OUT  PCH_DMI_TC_VC_MAP             *PchDmiTcVcMap
  );

/**
  This function provides Max Payload Size Supported by PCH in DMI.

  @return A value encoded in PCIe Capabilities is returned, i.e. 1 - 128B, 2 - 256B, 3 - 512B, etc.
**/
UINT8
PchDmiGetMaxPayloadSizeSupported (
  VOID
  );

/**
  This function reads Supported Link Speeds Vector from Link Capabilities 2 register.

  @return A bitmap of supported link speeds (gens) is returned.
**/
UINT8
PchDmiGetSupportedLinkSpeedsVector (
  VOID
  );

/**
  This function performs early PCH-H DMI SIP15 init.
  It must be called before any upstream transactions are enabled
**/
VOID
PchDmi15EarlyInit (
  VOID
  );

/**
  Makes memory cycles that fall within the given range inclusive to be
  forwarded using source decode to the specified Destination ID

  @param[in]    RangeIndex      Index of which General Purpose Memory
                                Range shall be configured
  @param[in]    RangeBase       Base of Source Decode Range, bits[15:0]
                                of range are assumed to be 0000h
  @param[in]    RangeLimit      Limit of Source Decode range, bits[15:0]
                                of range are assumed to be FFFFh
  @param[in]    DestinationId   The destination ID to be used to forward
                                the cycle decode

  @retval   EFI_SUCCESS               Range was programmed and enabled
  @retval   EFI_INVALID_PARAMETER     One of arguments is incorrect
**/
EFI_STATUS
PchDmiSetGeneralPurposeMemoryRange (
  UINT8     RangeIndex,
  UINT32    RangeBase,
  UINT32    RangeLimit,
  UINT32    DestinationId
  );

/**
  Set BIOS interface Lock-Down
**/
VOID
PchDmiSetBiosLockDown (
  VOID
  );
#endif
