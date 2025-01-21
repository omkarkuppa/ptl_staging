/** @file
  Declares the interface for TbtNvmDrvRetimerThruHr class.
  This class is in charge of providing the way to access the retimer
  through TBT integrated HR.

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

#ifndef TBT_NVM_DRV_RETIMER_THRU_HR_
#define TBT_NVM_DRV_RETIMER_THRU_HR_

#include <Library/TbtNvmDrvUtils.h>
#include <Library/TbtNvmDrvRetimerThruHrHelpers.h>

/**
  Constructs the Retimer thru HR module.
  Initializes all the internal data structures and initialize the required HW.

  @param[in] FirmwareType          The type of update firmware.
  @param[in] TbtPcieRootPort       The parent pcie root port of discrete TBT device.
  @param[in] TbtPort               The port connected to the target retimer.
  @param[in] CascadedRetimerIndex  The index of the retimer in the path from HR to the target retimer.
  @param[in] PcieRpConfig          PCIe root port config.
  @param[in] ForcePwrFunc          HR Force power function, applied if supplied.

  @retval A pointer to the device interface.

**/
TBT_RETIMER *
TbtNvmDrvRetimerThruHrCtor (
  IN UINT8          FirmwareType,
  IN PCIE_BDF       *TbtDmaPcieBdf,
  IN TBT_PORT       TbtPort,
  IN UINT32         CascadedRetimerIndex,
  IN PCIE_RP_CONFIG *PcieRpConfig,
  IN FORCE_PWR_HR   ForcePwrFunc OPTIONAL
  );

#endif /* TBT_NVM_DRV_RETIMER_THRU_HR_ */
