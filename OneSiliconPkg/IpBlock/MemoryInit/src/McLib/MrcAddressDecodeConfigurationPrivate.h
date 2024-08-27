/** @file
  This header exposes private members of MrcAddressDecodeConfiguration such that
  they can be used in Unit Testing

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

@par Specification Reference:
**/

#ifndef _MrcAddressDecodeConfiguration_h_
#define _MrcAddressDecodeConfiguration_h_

/**
  This function configures the zone configuration registers MAD-CR and MAD-ZR-CR.

  @param[in, out] MrcData     - Include all MRC global data.
  @param[in]      Controller  - Controller to configure.

  @retval Nothing.
**/
void
ZoneConfiguration (
  IN OUT MrcParameters *const MrcData,
  IN     UINT32               Controller
  );

/**
  This function configures the MAD_DIMM_CH0/1 register.

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
**/
void
ChannelAddressDecodeConfiguration (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  );

/**
  Set DIMM Config register for channel in MC

  @param[in] MrcData    - Include all MRC global data.
  @param[in] Controller - Controller to configure.
  @param[in] Channel    - Channel to configure.

  @retval Nothing.
*/
void
SetChannelDimmConfig (
  IN MrcParameters *const MrcData,
  IN const UINT32         Controller,
  IN const UINT32         Channel
  );

/**
  Map SDRAM Density Index to MC Subch Density register value.

  @param[in] Index   - SDRAM Density Index value.
  @param[in] IsLpddr - is LPDDR.

  @retval Result - MC Density register value
*/
UINT8
MapSdramDensityIndex (
  UINT8   Index,
  BOOLEAN IsLpddr
  );

/**
  Initialize MC inter hashing.

  @param[in] MrcData          - Include all MRC global data.

  @retval Nothing.
*/
void
SetMcSliceHashing (
  IN MrcParameters *const MrcData
  );

/**
  Get memory size [GB] for a particular controller.

  @param[in] MrcData          - Include all MRC global data.
  @param[in] Controller       - Controller to configure.

  @retval Result in GB.
*/
UINT32
GetMcCapacity (
  IN MrcParameters *const MrcData,
  IN INT32 Controller
  );

#endif // _TestDataEnginePrivate_h_
