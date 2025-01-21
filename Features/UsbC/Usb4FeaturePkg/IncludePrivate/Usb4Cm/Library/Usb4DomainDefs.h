/** @file
  Header file for USB4 domain definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _USB4_DOMAIN_DEFS_H_
#define _USB4_DOMAIN_DEFS_H_

#include <Uefi.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4RouterDefs.h>

//
// DP stream information in USB4 domain
//
typedef struct _USB4_DP_STREAM {
  // Router containing DP source
  PUSB4_ROUTER    DpSrcRouter;
  // DP source adapter number
  UINT8           DpSrcAdpNum;
  // Router containing DP sink
  PUSB4_ROUTER    DpSinkRouter;
  // DP sink adapter number
  UINT8           DpSinkAdpNum;
  // DP stream configuration is ready
  BOOLEAN         StreamReady;
} USB4_DP_STREAM, *PUSB4_DP_STREAM;

struct _USB4_HR_INSTANCE;

#define USB4_DOMAIN_SIGNATURE      SIGNATURE_32 ('u', '4', 'd', 'm')

//
// USB4 domain instance for maintaining domain topology
//
typedef struct _USB4_DOMAIN {
  UINT32            Signature;
  // Router pool in domain
  USB4_ROUTER       Router[USB4_ROUTER_SUPPORT_MAX];
  // Start index of next free router search
  UINT32            NextRtSearch;
  // In-use router count in router pool
  UINT32            RtInUse;
  // Root of router tree in domain, it represents router node of host router
  USB4_ROUTER       *RootRt;
  // Router count in domain
  UINT32            RouterCount;
  // DP stream count in domain
  UINT32            DpStreamCount;
  // DP stream information in domain
  USB4_DP_STREAM    DpStream[USB4_DP_STREAM_MAX];
  // USB4_CS_IO function table used for USB4 config space access in domain
  USB4_CS_IO        *Usb4CsIo;
  // USB4 host router instance binding to domain
  struct _USB4_HR_INSTANCE    *Usb4Hr;
} USB4_DOMAIN, *PUSB4_DOMAIN;

#endif
