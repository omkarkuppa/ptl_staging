/** @file
  This library will determine processor configuration information to
  create SMBIOS CPU tables appropriately.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

/**
  This function returns the maximum number of cores supported in this physical processor package
  by leverging CPUID_CACHE_PARAMS with offset 26.

  @retval Maximum number of supported cores in the package.
**/
UINT8
GetMaxSupportedCoreCount (
  VOID
  )
{
  return 0;
}

/**
  This function returns the supported number of threads per core,
  and supported total threads in the physical processor package
  by leverging CPUID_EXTENDED_TOPOLOGY with index 1 & 2.

  @param[out] *ThreadsPerCore    - variable that will store Maximum enabled threads per core.
  @param[out] *Threads           - variable that will store supported total threads.
**/
VOID
GetSupportedThreadCount (
  OUT UINT8 *ThreadsPerCore,  OPTIONAL
  OUT UINT8 *Threads          OPTIONAL
  )
{
  return;
}

/**
  This function produces SMBIOS Cache Info HOBs, and an SMBIOS Processor Info HOB.
**/
VOID
EFIAPI
InitializeSmbiosCpuHobs (
  VOID
  )
{
  return;
}
