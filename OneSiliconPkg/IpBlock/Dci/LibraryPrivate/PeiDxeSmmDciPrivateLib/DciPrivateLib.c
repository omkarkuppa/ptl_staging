/** @file
  DCI private library.
  All functions from this library are available in PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Register/DciRegs.h>
#include <RegisterAccess.h>

/**
  Return HDCI Debug Enabled status.

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   HDCI/HEEN Debug is enabled
  @retval FALSE  HDCI/HEEN Debug is disabled
**/
BOOLEAN
IsHdciDebugEnabled (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  UINT32 Data;

  Data = DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_ECTRL);

  if (Data & B_DCI_PCR_ECTRL_HEEN) {
    DEBUG ((DEBUG_INFO, "HDCI Debug Enabled\n"));
    return TRUE;
  }

  DEBUG ((DEBUG_INFO, "HDCI Debug Disabled\n"));
  return FALSE;
}

/**
  Return DCI Debug Enabled status.

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   DCI Debug is enabled
  @retval FALSE  DCI Debug is disabled
**/
BOOLEAN
IsDciDebugEnabled (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  UINT32 Data;

  Data = DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_ECTRL);

  if (Data & B_DCI_PCR_ECTRL_DBGENABLE) {
    DEBUG ((DEBUG_INFO, "DCI Debug Enabled\n"));
    return TRUE;
  }

  DEBUG ((DEBUG_INFO, "DCI Debug Disabled\n"));
  return FALSE;
}

/**
  Return USB2 DbC enable status.

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   USB2 DbC is enabled
  @retval FALSE  USB2 DbC is disabled
**/
BOOLEAN
IsUsb2DbcDebugEnabled (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  UINT32 Data;

  Data  = DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_ECTRL);
  Data &= (B_DCI_PCR_ECTRL_DBGENABLE | B_DCI_PCR_ECTRL_USB3DBCEN | B_DCI_PCR_ECTRL_USB2DBCEN);

  // If ECTRL[8, 6, 5] = 1b, 1b, 1b, then USB2 DbC is enabled.
  if (Data == (UINT32)(B_DCI_PCR_ECTRL_DBGENABLE | B_DCI_PCR_ECTRL_USB3DBCEN | B_DCI_PCR_ECTRL_USB2DBCEN)) {
    DEBUG ((DEBUG_INFO, "USB2 DbC is enabled\n"));
    return TRUE;
  }

  DEBUG ((DEBUG_INFO, "USB2 DbC is disabled\n"));
  return FALSE;
}

/**
  Return PCH USB DbC connection status

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   PCH USB DbC is connected
  @retval FALSE  PCH USB DbC is not connected
**/
BOOLEAN
IsPchUsbDbcConnected (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  if (DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_EARBCTRL) & B_DCI_PCR_EARBCTRL_ARB_GNT_DBC_SOUTH) {
    DEBUG ((DEBUG_INFO, "PCH USB DbC is connected.\n"));
    return TRUE;
  }
  return FALSE;
}

/**
  Return DCI host connection status

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE   DCI host is connected
  @retval FALSE  DCI host is not connected
**/
BOOLEAN
IsDciHostConnected (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  if (DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_ECTRL) & B_DCI_PCR_ECTRL_EGRANT) {
    DEBUG ((DEBUG_INFO, "DCI Host is connected\n"));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "DCI host is not connected\n"));
    return FALSE;
  }
}

/**
  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  Disable DCI clock request override.
**/
VOID
DciDisClkReq (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  DciPcrAccess->AndThenOr8 (DciPcrAccess, R_DCI_PCR_ESRCCLK, (UINT8)~(B_DCI_PCR_ESRCCLK_SRCCLK_RO_FAST_CLK_OVR_S0I3P4 | B_DCI_PCR_ESRCCLK_SRCCLK_RO_FAST_CLK_OVR), (B_DCI_PCR_ESRCCLK_SRCCLK_RO_FAST_CLK_OVR_S0I3P4 | B_DCI_PCR_ESRCCLK_SRCCLK_RO_FAST_CLK_OVR));
}

/**
  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  Enable DCI clock request override.
**/
VOID
DciEnClkReq (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  DciPcrAccess->AndThenOr8 (DciPcrAccess, R_DCI_PCR_ESRCCLK, (UINT8)~(B_DCI_PCR_ESRCCLK_SRCCLK_RO_FAST_CLK_OVR_S0I3P4 | B_DCI_PCR_ESRCCLK_SRCCLK_RO_FAST_CLK_OVR), 0);
}

/**
  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  DCI keep early trace
**/
VOID
DciKeepEarlyTrace (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  DciPcrAccess->AndThenOr32 (DciPcrAccess, R_DCI_PCR_ECTRL, ~0u, B_DCI_PCR_ECTRL_KET);
}

/**
  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  DCI disable early trace
**/
VOID
DciDisableEarlyTrace (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  DciPcrAccess->AndThenOr32 (DciPcrAccess, R_DCI_PCR_ECTRL, (UINT32)~(B_DCI_PCR_ECTRL_KET), 0);
}

/**
  Return status of keep early trace

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE    Keep early trace
  @retval FALSE   Stop early trace
**/
BOOLEAN
IsKeepEarlyTrace (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  if (DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_ECTRL) & B_DCI_PCR_ECTRL_KET) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Returns status if any DbC options are enabled

  @param[in]  *DciPcrAccess       Instance of DCI sideband Register Access Protocol

  @retval TRUE    Either USB2DbC or USB3DbC is enabled
  @retval FALSE   Both USB2DbC and USB3DbC are disabled
**/
BOOLEAN
IsAnyDbcEnabled (
  REGISTER_ACCESS            *DciPcrAccess
  )
{
  UINT32 Data32;

  Data32  = DciPcrAccess->Read32 (DciPcrAccess, R_DCI_PCR_ECTRL);
  Data32 &= (B_DCI_PCR_ECTRL_USB3DBCEN | B_DCI_PCR_ECTRL_USB2DBCEN);

  return (Data32 != 0);
}