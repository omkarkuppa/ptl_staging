/** @file
  Header file for PeiIshInitLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _PEI_ISH_INIT_LIB_H_
#define _PEI_ISH_INIT_LIB_H_

#include <Ppi/SiPolicy.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <IshConfig.h>

typedef struct {
  REGISTER_ACCESS  *SidebandMmio;
  REGISTER_ACCESS  *PcieConfig;
} ISH_ACCESS;

typedef struct {
  UINT8   InterruptPin;
  UINT8   Irq;
} ISH_PRIVATE;

typedef struct _ISH_CALLBACKS ISH_CALLBACKS;

typedef struct {
  ISH_PRIVATE      Private;
  ISH_ACCESS       *Access;
  ISH_CALLBACKS    *Callbacks;
  ISH_CONFIG       *IshConfig;
} ISH_HANDLE;

/**
  ISH Configure SOC interrupts

  @param[in] IshHandle   ISH_HANDLE
**/
typedef
VOID
(*ISH_CONFIGURE_INTERRUPTS) (
  IN  ISH_HANDLE      *IshHandle
  );

/**
  Configures ISH-related GPIO pins to ISH native mode

  @param[in] IshHandle   ISH_HANDLE

  @retval EFI_SUCCESS
**/
typedef
VOID
(*ISH_SET_GPIO) (
  IN  ISH_HANDLE      *IshHandle
  );

/**
  Disable ISH BAR1 in PSF
**/
typedef
VOID
(*ISH_DISABLE_DEVICE_BAR) (
  VOID
  );

/**
  Disabling ISH

  @param[in]  IshHandle  ISH_HANDLE
**/
typedef
VOID
(*ISH_SOC_DISABLE) (
  ISH_HANDLE   *IshHandle
  );

/**
  Function checks if ISH is PowerGated and if is, enable
  Power Management for ISH and issue reset

  @param[in] IshHandle  ISH_HANDLE

  @retval    TRUE       ISH Configuration should be omitted
  @retval    FALSE      ISH needs to be configured
**/
typedef
BOOLEAN
(*ISH_IS_POWER_GATED) (
  IN  ISH_HANDLE   *IshHandle
  );

struct _ISH_CALLBACKS {
  ISH_CONFIGURE_INTERRUPTS          ConfigureInterrupts;
  ISH_DISABLE_DEVICE_BAR            DisableDeviceBar;
  ISH_SET_GPIO                      SetGpio;
  ISH_SOC_DISABLE                   Disable;
};

/**
  Configures ISH interrupts

  @param[in] IshDevice  ISH Device
  @param[in] InterruptPin
  @param[in] Irq
**/
VOID
IshConfigureInterrupts (
  IN  ISH_HANDLE      *IshDevice,
  IN  UINT8           InterruptPin,
  IN  UINT8           Irq
  );

/**
  Configures Integrated Sensor Hub (ISH) Controller

  @param[in] IshHandle       ISH_HANDLE
**/
VOID
IshConfigure (
  IN  ISH_HANDLE      *IshHandle
  );

/**
  Sets Integrated Sensor Hub (ISH) PSF Function Disable

  @param[in]  IshHandle  ISH_HANDLE
**/
VOID
IshDisable (
  ISH_HANDLE  *IshHandle
  );
#endif
