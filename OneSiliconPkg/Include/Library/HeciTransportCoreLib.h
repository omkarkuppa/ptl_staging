/** @file
 Definitions for HECI Transport driver

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

@par Specification
**/
#ifndef _HECI_TRANSPORT_CORE_LIB_H_
#define _HECI_TRANSPORT_CORE_LIB_H_

#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/HeciAccessProtocol.h>

#pragma pack (1)
/**
  HECI packet header.

  HECI packet header is one double word long. It identifies communication
  entities (subsystems) on engine and host side and provides the exact number
  of bytes in the packet body following this header. Several packets may
  establish one HECI message. In this case the last and only the last packet
  must have Complete bit set to 1.
**/
typedef union {
  UINT32   Data;
  struct {
    UINT32 EngineAddress   : 8;  ///< Address on engine (ME/IE) side
    UINT32 HostAddress     : 8;  ///< Address on host side, zero for BIOS
    UINT32 Length          : 9;  ///< Number of bytes following the header
    UINT32 Reserved        : 6;
    UINT32 MessageComplete : 1;  ///< Whether this is last fragment of a message
  } Fields;
} HECI_PACKET_HEADER;
#pragma pack ()

typedef struct {
  HECI_TRANSPORT      HeciTransport;
  // Related HeciAccess pointer, Heci Transport is based on
  HECI_ACCESS         *HeciAccess;           /// HeciAccess pointer to link Heci Transport and Access in PostMem
  UINT32              HeciAccessIndex;       /// HeciAccess index to link Heci Transport and Access in PreMem
  VOID                *HeciTransportRoot;    /// HeciTransport list root
  VOID                *HeciTransportNext;    /// Next HeciTransport protocol (used in Smm/Dxe))

  BOOLEAN             IsInitialized;         ///< Flag if interface is successfully initialized
  // By PchIndex & PciFunction we are able to match Transport and access during relink
  UINT32              SecurityEngine;        ///< Engine device where HECI is located
  UINTN               PciSegment;            ///< PCI segment where this HECI instance is located
  UINTN               PciBus;                ///< PCI bus where this HECI instance is located
  UINTN               PciDevice;             ///< PCI device where this HECI instance is located
  UINTN               PciFunction;           ///< PCI function number of this HECI instance
  //
  // Default HECI timeouts
  //
  UINT32              SendTimeout;           ///< HECI default send timeout
  UINT32              ReceiveTimeout;        ///< HECI default receive timeout
  UINT32              ResetTimeout;          ///< HECI default reset timeout
} HECI_TRANSPORT_PRIVATE;

/**
  Initialize HECI_TRANSPORT protocol functions

  @param[in]     HeciTransport         HECI transport pointer to be initialized
**/
VOID
SetHeciTransportFunctions (
  IN OUT HECI_TRANSPORT_PRIVATE *Heci
  );

/**
  Initialize HECI_TRANSPORT protocol instance

  @param[in]     HeciTransport HECI transport pointer to be initialized
  @param[in]     HeciAccess    Associated HECI access instance

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_SUCCESS           Heci transport initialized
**/
EFI_STATUS
HeciInitializeNewTransportInstance (
  IN OUT HECI_TRANSPORT_PRIVATE *Heci,
  IN     HECI_ACCESS            *HeciAccess
  );

/**
  Function returns HeciAccess for given HeciTransport

  @param[in] HeciTransport      HeciTransport for getting HeciAccess

  @retval On success valid pointer to linked HeciAccess
  @retval NULL if HeciAccess couldn't be found
**/
HECI_ACCESS *
GetHeciAccess (
  HECI_TRANSPORT_PRIVATE    *HeciTransport
  );

/**
  Get Next HECI Transport

  @param[in] This     Pointer to HECI Transport protocol instance from which the next instance should be returned.

  @retval Pointer to the next HeciTransport protocol instance
**/
HECI_TRANSPORT *
EFIAPI
GetNextHeciTransport (
  IN HECI_TRANSPORT   *This
  );

/**
  Get HECI_TRANSPORT instance for given PchIndex and HeciFunction in HeciAccess.
  (PEI only)

  @param[in]     HeciAccess           Heci Access that parameters HeciAccess Has to match
  @param[in,out] HeciCurrentInstance  Pointer to found instance id

  @retval HECI_TRANSPORT pointer
  @retval NULL pointer if no HECI_TRANSPORT_PRIVATE is found
**/
HECI_TRANSPORT *
GetHeciTransportForHeciAccess (
  IN     HECI_ACCESS      *HeciAccess,
  IN OUT UINTN            *HeciCurrentInstance
  );

#endif // _HECI_TRANSPORT_CORE_LIB_H_

