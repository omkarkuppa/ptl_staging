/** @file
  PEI Heci Transport PPI - ME-BIOS communication over HECI in PEI.

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
#ifndef _HECI_TRANSPORT_PPI_H_
#define _HECI_TRANSPORT_PPI_H_

#include <Protocol/HeciTransportProtocol.h>

/**
  ME HECI PPI GUID
  This PPI provides an interface to communicate with Intel ME in PEI phase.
**/
extern EFI_GUID gHeciTransportPpiGuid;

#endif // _HECI_TRANSPORT_PPI_H_
