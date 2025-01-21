/** @file
  Memory Initialization library API.

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

@par Specification Reference:
**/
#ifndef _MEMORY_INIT_LIB_H_
#define _MEMORY_INIT_LIB_H_

/**
  <b>Entry point of MemoryInit library</b>
  This function will invoke main memory detection code after the required dependencies satisified.
  - <b>Introduction</b> \n
    The Memory Reference Code (MRC) for Broadwell configures the Broadwell System Agent Memory controller and performs DRAM initialization tasks (MRC is included in System Agent Reference Code package and is not released separately).
    Please refer to the Framework Client BIOS Broadwell Memory Reference Code Porting Guide for detailed porting information.
    Please refer the MRC_ReleaseNote for MRC release and version information.
    MemoryInit PEIM is required by all systems supporting Intel Broadwell.

  - @pre
    - _SI_SA_POLICY_PPI: A PPI published by a platform PEIM executed earlier; documented in this chapter as well.
    - @link SA_PEI_INIT_PPI_GUID @endlink (@link gSaPeiInitPpiGuid @endlink): A PPI published by SA Init PEIM executed earlier.
    - PEI_SMBUS_PPI: A PPI published by PCH SMBUS PEIM executed earlier.
    - PEI_STALL_PPI: A PPI published by SB PEI executed earlier (Conditional if TXT support is enabled).
    - WDT_PPI (_WDT_PROTOCOL): A PPI published by PCH PEIM executed earlier.
    - _SI_CPU_POLICY_PPI: A PPI published by CPU PEIM executed earlier.
    - PEI_MASTER_BOOT_MODE_PEIM_PPI: A PPI published by foundation to get bootmode executed earlier. This PPI is the last dependency of the notify PPI registration to perform MemoryInit.

  - @result
    System Agent Memory Controller is configured and DRAM is initialized and ready for EFI to use. EFI memory HOBs are installed, and on non S3 resume path, S3 Restore Data are saved to Hob. \n

  - <b>Integration Checklist</b> \n
    Note: Not all of the memory range options are properly handled by the current implementation, previous known implementation, and subsequent resource configuration modules. Please be very careful and verify expected implementation behavior when using this interface. For example PCI memory mask is not currently handled by wrapper and PCI memory use is determined by PciHostBridge driver.
    - Ensure that your platform is publishing the SI_SA_POLICY_PPI on all boot paths
    - Ensure that your platform is publishing the _PEI_CAPSULE_PPI on the capsule update boot path
    - Ensure that your platform is publishing the PEI_READ_ONLY_VARIABLE_PPI on the S3 resume path
    - Verify SMRAM initialization and configuration
**/
EFI_STATUS
EFIAPI
InstallMrcCallback (
  VOID
  );

#endif
