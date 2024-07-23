/** @file
  The PEI Library Implements ME Init.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2008 Intel Corporation.

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
#ifndef _ME_INIT_LIB_H_
#define _ME_INIT_LIB_H_

#include <Ppi/SiPolicy.h>
#include <MeHandle.h>

/**
  Check some ME policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] SiPreMemPolicyPpi    The SI Policy protocol instance

**/
VOID
MeValidatePolicy (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );

/**
  This function performs basic initialization for ME in PEI phase after Policy produced.
**/
VOID
MeOnPolicyInstalled (
  VOID
  );

/**
  Internal function performing PM register initialization for Me
**/
VOID
MePmInit (
  VOID
  );

/**
  Initialize early ME HOBs

  @retval EFI_SUCCESS     HECI1 initialized successfully
  @retval Other           Error occurred
**/
EFI_STATUS
InstallMeHobs (
  VOID
  );

/**
  This function performs basic initialization for ME in PEI phase after memory is initialized.

  @param[in] SiPolicy     The Silicon Policy PPI instance

**/
VOID
MePostMemInit (
  IN  SI_POLICY_PPI  *SiPolicy
  );

/**
  Install MbpData Hob.

  @retval     EFI_SUCCESS        The function completed successfully.
  @retval     EFI_UNSUPPORTED    MBP Hob creation skipped
  @retval     Others             Failed to create MBP Hob
**/
EFI_STATUS
InstallMbpHob (
  VOID
  );

//
// Function Prototype(s) for PchMeUma.c
//

/**
  This procedure will determine whether or not the CPU was replaced
  during system power loss or via dynamic fusing.
  Calling this procedure could result in a warm reset (if ME FW is requesting one).

  @param[in] FfsHeader            Not used.
  @param[out] ForceFullTraining   When set = 0x1, MRC will be forced to perform a full
                                  memory training cycle.

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
CpuReplacementCheck (
  IN VOID                         *FfsHeader,
  OUT UINT8                       *ForceFullTraining
  );

/**
  This procedure will configure the ME Host General Status register,
  indicating that DRAM Initialization is complete and ME FW may
  begin using the allocated ME UMA space.

  @param[in] FfsHeader            Pointer to the FFS file header
  @param[in] InitStat             H_GS[27:24] Status
  @param[in] ImrBaseLow           Base address for IMR region (Low DWORD)
  @param[in] ImrBaseHigh          Base address for IMR region (High DWORD)

  @retval EFI_SUCCESS             The function completed successfully.
**/
EFI_STATUS
MeConfigDidReg (
  IN VOID                         *FfsHeader,
  IN UINT8                        InitStat,
  IN UINT32                       ImrBaseLow,
  IN UINT32                       ImrBaseHigh
  );

/**
  This procedure will send the information to CSME about the IMRs which need to be
  enabled and retrieve the information about total size of memory and alignment
  required for IMRs.

  @param[out] ImrsSize            Total IMR size in MB.
  @param[out] Alignment           Required address alignment in MB.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES    ME UMA Size outside of a 0MB-128MB
  @retval Others                  An error has occured.
**/
EFI_STATUS
MeImrConfig (
  OUT UINT32     *ImrsSize,
  OUT UINT32     *Alignment
  );

/**
  Checks EndOfManufacturing (EOM)

  @retval TRUE   Out of Manufacturing Mode
  @retval FALSE  In Manufacturing Mode
**/
BOOLEAN
IsEom (
  VOID
  );

/**
  Installs the ME (Management Engine) Handle PPI/Protocol.

  @retval VOID  This function does not return a value and has no return statement.
**/
VOID
InstallMeHandle (
  VOID
  );

/**
  Retrieves the ME (Management Engine) Handle PPI/Protocol.

  @param[out] MeHandle            The pointer to a pointer to the ME_HANDLE structure that
                                  will be updated with the ME_HANDLE PPI/Protocol instance.

  @retval EFI_SUCCESS             The ME_HANDLE PPI/Protocol was located and returned successfully.
  @retval EFI_NOT_FOUND           The ME_HANDLE PPI/Protocol could not be located.
  @retval EFI_INVALID_PARAMETER   One or more parameters are invalid.
  @retval Other                   An error returned by the LocateProtocol function.
**/
EFI_STATUS
GetMeHandle (
  OUT ME_HANDLE   **MeHandle
  );

/**
  Check if MCTP (Management Component Transport) Protocol is supported by CSME and BIOS.

  @param[in] SiPolicy   Silicon Policy

  @retval    TRUE                     MCTP is supported by CSME or BIOS
  @retval    FALSE                    MCTP is not supported by CSME or BIOS
**/
BOOLEAN
IsMctpConfigurationSupported (
  IN  SI_POLICY_PPI  *SiPolicy
  );
#endif
