/** @file
  Prototype of the PreSiliconEnvDetectLib library.

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
#ifndef _PRESILICON_ENV_DETECT_LIB_H_
#define _PRESILICON_ENV_DETECT_LIB_H_

/**
  Check if currently run in Simics environment or not.

  @retval TRUE                  Run in Simics environment
  @retval FALSE                 Not run in Simics environment
**/
BOOLEAN
IsSimicsEnvironment (
  VOID
  );

/**
  Check if currently run in HFPGA environment or not.

  @retval TRUE                  Run in HFPGA environment
  @retval FALSE                 Not run in HFPGA environment
**/
BOOLEAN
IsHfpgaEnvironment (
  VOID
  );

/**
  Check if currently run in HSLE environment or not.

  @retval TRUE                  Run in HSLE environment
  @retval FALSE                 Not run in HSLE environment
**/
BOOLEAN
IsHSLEEnvironment (
  VOID
  );

/**
  Check if currently run in HSLE UPF environment or not.

  @retval TRUE                  Run in HSLE UPF environment
  @retval FALSE                 Not run in HSLE UPF environment
**/
BOOLEAN
IsHSLEEnvironment_UPF (
  VOID
  );

/**
  Check if currently run in FastEmu environment or not.

  @retval TRUE                  Run in FastEMU environment
  @retval FALSE                 Not run in FastEMU environment
**/
BOOLEAN
IsFastEMUEnvironment (
  VOID
  );

/**
  Check if currently run in PSS environment or not.

  @retval TRUE                  Run in PSS environment
  @retval FALSE                 Not run in PSS environment
**/
BOOLEAN
IsPreSiEnvironment (
  VOID
  );
#endif // _PRESILICON_ENV_DETECT_LIB_H_
