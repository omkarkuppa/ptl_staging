/** @file
  Internal file to access Foxville register.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
//
// Self-Module Foundation.
//
#include "Defines/FoxvilleRegs.h"
#include "Misc/FoxvillePciIoAccess.h"
#include "Misc/FoxvilleRegAccess.h"

//
// 0x00020 (RW): Media Dependent Interface (MDI) Control Register.
//

/**
  Read the MDIC register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the MDIC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the MDIC register.

**/
EFI_STATUS
EFIAPI
ReadMdic (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_MDIC_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_MDIC,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the MDIC register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the MDIC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the MDIC register.

**/
EFI_STATUS
EFIAPI
WriteMdic (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_MDIC,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

//
// 0x00E04 (RW): MDC/MDIO Configuration Register.
//

/**
  Read the MDICNFG register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the MDICNFG register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the MDICNFG register.

**/
EFI_STATUS
EFIAPI
ReadMdicnfg (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_MDICNFG_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_MDICNFG,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the MDICNFG register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the MDICNFG register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the MDICNFG register.

**/
EFI_STATUS
EFIAPI
WriteMdicnfg (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_MDICNFG,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

//
// 0x05B50 (RW): Software Semaphore Register.
//

/**
  Read the SWSM register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the SWSM register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the SWSM register.

**/
EFI_STATUS
EFIAPI
ReadSwsm (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_SWSM_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_SWSM,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the SWSM register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the SWSM register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the SWSM register.

**/
EFI_STATUS
EFIAPI
WriteSwsm (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_SWSM,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Get the bit status of SMBI bit in SWSM register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsSetPtr          Pointer to return the status of bit.

  @retval  EFI_SUCCESS            Succeed to get the SMBI bit status.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the SMBI bit status.

**/
EFI_STATUS
EFIAPI
GetSwsmSmbi (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsSetPtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_SWSM_REGISTER  Response;
  BOOLEAN                 IsBitSet;

  Response.Uint32 = 0;
  IsBitSet        = FALSE;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadSwsm (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.Smbi != V_SWSM_SMBI_SET) {
    IsBitSet = FALSE;
  } else {
    IsBitSet = TRUE;
  }

  *IsSetPtr = IsBitSet;

  return EFI_SUCCESS;
}

/**
  Clear the SWSM register SMBI bit.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to clear SMBI bit in SWSM register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to clear SMBI bit in SWSM register.

**/
EFI_STATUS
EFIAPI
ClearSwsmSmbi (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_SWSM_REGISTER  Request;
  FOXVILLE_SWSM_REGISTER  Response;

  Request.Uint32  = 0;
  Response.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadSwsm (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Request.Uint32       = Response.Uint32;
  Request.Bits.Smbi    = V_SWSM_SMBI_CLEAR;
  Request.Bits.SweSmbi = V_SWSM_SWE_SMBI_CLEAR;

  Status = WriteSwsm (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Get the bit status of SWESMBI bit in SWSM register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsSetPtr          Pointer to return the status of bit.

  @retval  EFI_SUCCESS            Succeed to get the SWESMBI bit status.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the SWESMBI bit status.

**/
EFI_STATUS
EFIAPI
GetSwsmSweSmbi (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsSetPtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_SWSM_REGISTER  Response;
  BOOLEAN                 IsBitSet;

  Response.Uint32 = 0;
  IsBitSet        = FALSE;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadSwsm (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.SweSmbi != V_SWSM_SWE_SMBI_SET) {
    IsBitSet = FALSE;
  } else {
    IsBitSet = TRUE;
  }

  *IsSetPtr = IsBitSet;

  return EFI_SUCCESS;
}

/**
  Set the SWSM register SWESMBI bit.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to set SWESMBI bit in SWSM register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set SWESMBI bit in SWSM register.

**/
EFI_STATUS
EFIAPI
SetSwsmSweSmbi (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_SWSM_REGISTER  Request;
  FOXVILLE_SWSM_REGISTER  Response;

  Request.Uint32  = 0;
  Response.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadSwsm (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Request.Uint32       = Response.Uint32;
  Request.Bits.SweSmbi = V_SWSM_SWE_SMBI_SET;

  Status = WriteSwsm (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

//
// 0x05B54 (Host-RO; FW-RW): Firmware Semaphore Register.
//

/**
  Read the FWSM register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the FWSM register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the FWSM register.

**/
EFI_STATUS
EFIAPI
ReadFwsm (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_FWSM_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FWSM,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the FWSM register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_UNSUPPORTED  Not supported to write the register.

**/
EFI_STATUS
EFIAPI
WriteFwsm (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  ASSERT (FALSE);

  return EFI_UNSUPPORTED;
}

//
// 0x05B5C (RWM): Software-Firmware Synchronization.
//

/**
  Read the SW_FW_SYNC register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the SW_FW_SYNC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the SW_FW_SYNC register.

**/
EFI_STATUS
EFIAPI
ReadSwFwSync (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_SW_FW_SYNC_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_SW_FW_SYNC,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the SW_FW_SYNC register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the SW_FW_SYNC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the SW_FW_SYNC register.

**/
EFI_STATUS
EFIAPI
WriteSwFwSync (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_SW_FW_SYNC,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Check the NVM access resource via SW_FW_SYNC register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsAvailablePtr    Pointer to indicate the resource of NVM access.

  @retval  EFI_SUCCESS            NVM access resource is available.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to check the NVM access resource.

**/
EFI_STATUS
EFIAPI
CheckSwFwSyncNvmResource (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsAvailablePtr
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_SW_FW_SYNC_REGISTER  Response;
  BOOLEAN                       IsResourceFree;

  Response.Uint32 = 0;
  IsResourceFree  = FALSE;

  if ((PciIoProtocolPtr == NULL) || (IsAvailablePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadSwFwSync (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  //
  // Indicate the NVM access resource availability.
  //
  if (Response.Bits.FwFlashSm == V_SW_FW_SYNC_FW_FLASH_SM_SET) {
    IsResourceFree = FALSE;
  } else if (Response.Bits.SwFlashSm == V_SW_FW_SYNC_SW_FLASH_SM_SET) {
    IsResourceFree = FALSE;
  } else {
    IsResourceFree = TRUE;
  }

  *IsAvailablePtr = IsResourceFree;

  return EFI_SUCCESS;
}

/**
  Set the SW_FW_SYNC register SW_FLASH_SM bit.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to set SW_FLASH_SM bit in SW_FW_SYNC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set SW_FLASH_SM bit in SW_FW_SYNC register.

**/
EFI_STATUS
EFIAPI
SetSwFwSyncSwFlashSm (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_SW_FW_SYNC_REGISTER  Request;
  FOXVILLE_SW_FW_SYNC_REGISTER  Response;

  Request.Uint32  = 0;
  Response.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadSwFwSync (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Request.Uint32         = Response.Uint32;
  Request.Bits.SwFlashSm = V_SW_FW_SYNC_SW_FLASH_SM_SET;

  Status = WriteSwFwSync (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Clear the SW_FW_SYNC register SW_FLASH_SM bit.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to clear SW_FLASH_SM bit in SW_FW_SYNC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to clear SW_FLASH_SM bit in SW_FW_SYNC register.

**/
EFI_STATUS
EFIAPI
ClearSwFwSyncSwFlashSm (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_SW_FW_SYNC_REGISTER  Request;
  FOXVILLE_SW_FW_SYNC_REGISTER  Response;

  Request.Uint32  = 0;
  Response.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadSwFwSync (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Request.Uint32         = Response.Uint32;
  Request.Bits.SwFlashSm = V_SW_FW_SYNC_SW_FLASH_SM_CLEAR;

  Status = WriteSwFwSync (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

//
// 0x12010 (RW): EEPROM-Mode Control Register.
//

/**
  Read the EEC register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the EEC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the EEC register.

**/
EFI_STATUS
EFIAPI
ReadEec (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS             Status;
  FOXVILLE_EEC_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_EEC,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the EEC register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the EEC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the EEC register.

**/
EFI_STATUS
EFIAPI
WriteEec (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_EEC,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Set the EEC register flash update bit.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to set flash update bit in EEC register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set flash update bit in EEC register.

**/
EFI_STATUS
EFIAPI
SetEecFlashUpdate (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS             Status;
  FOXVILLE_EEC_REGISTER  Request;
  FOXVILLE_EEC_REGISTER  Response;

  Request.Uint32  = 0;
  Response.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadEec (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Request.Uint32           = Response.Uint32;
  Request.Bits.FlashUpdate = V_EEC_FLASH_UPDATE_SET;

  Status = WriteEec (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Check the EEC register flash update done bit.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsDonePtr         Pointer to indicate the flash update done status.

  @retval  EFI_SUCCESS            Succeed to get the flash update done status.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the flash update done status.

**/
EFI_STATUS
EFIAPI
CheckEecFlashUpdateDone (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsDonePtr
  )
{
  EFI_STATUS             Status;
  FOXVILLE_EEC_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (IsDonePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsDonePtr = FALSE;

  Status = ReadEec (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.FlashUpdateDone == V_EEC_FLASH_UPDATE_DONE_SET) {
    *IsDonePtr = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  Polling to check the EEC register flash update operation is done.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Device had processed the firmware update operation.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Device had not processed the firmware update operation.

**/
EFI_STATUS
EFIAPI
PollEecFlashUpdateDone (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsDone;
  UINTN       Count;

  IsDone = FALSE;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_EEC_FLASH_UPDATE_DONE_MAX_ATTEMPT; Count++) {
    Status = CheckEecFlashUpdateDone (PciIoProtocolPtr, &IsDone);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if (IsDone) {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_EEC_FLASH_UPDATE_DONE_TIMEOUT);
  }

  return EFI_TIMEOUT;
}

/**
  Check the validity of sector 1 via ECC register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to check the validity of sector 1.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to check the validity of sector 1.

**/
EFI_STATUS
EFIAPI
CheckEecSector1Valid (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsValid
  )
{
  EFI_STATUS             Status;
  FOXVILLE_EEC_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (IsValid == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsValid = FALSE;

  Status = ReadEec (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.Sector1Valid == V_EEC_SECTOR_1_VALID_SET) {
    *IsValid = TRUE;
  }

  return EFI_SUCCESS;
}

//
// 0x12014 (RW): EEPROM-Mode Read Register.
//

/**
  Read the EERD register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  DataPtr           Pointer to return the value in Data field.

  @retval  EFI_SUCCESS            Succeed to read the EERD register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the EERD register.

**/
EFI_STATUS
EFIAPI
ReadEerd (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT16               *DataPtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_EERD_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (DataPtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_EERD,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *DataPtr = (UINT16)(Response.Bits.Data);

  return EFI_SUCCESS;
}

/**
  Write the EERD register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Value to fill the Address field in request.

  @retval  EFI_SUCCESS            Succeed to write the EERD register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the EERD register.

**/
EFI_STATUS
EFIAPI
WriteEerd (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT16               Address
  )
{
  EFI_STATUS              Status;
  FOXVILLE_EERD_REGISTER  Request;

  Request.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Form the request field.
  //
  Request.Bits.Valid   = V_EERD_COMMAND_VALID;
  Request.Bits.Address = Address;

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_EERD,
             Request.Uint32
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Polling to check the EERD register read operation is done.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Device had processed the read operation.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Device had not processed the read operation.

**/
EFI_STATUS
EFIAPI
PollEerdReadDone (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_EERD_REGISTER  Response;
  UINTN                   Count;

  Response.Uint32 = 0;
  Count           = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_EERD_DONE_MAX_ATTEMPT; Count++) {
    Status = ReadMacRegUint32 (
               PciIoProtocolPtr,
               R_FOXVILLE_EERD,
               &(Response.Uint32)
               );
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if (Response.Bits.ReadDone == V_EERD_READ_DONE) {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_EERD_DONE_TIMEOUT);
  }

  return EFI_TIMEOUT;
}

//
// 0x12018 (RW): EEPROM-Mode Write Register.
//

/**
  Read the EEWR register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the EEWR register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the EEWR register.

**/
EFI_STATUS
EFIAPI
ReadEewr (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_EEWR_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_EEWR,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the EEWR register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Address           Value to fill the Address field in request.
  @param[in]  Data              Data used to write into assigned address.

  @retval  EFI_SUCCESS            Succeed to write the EEWR register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the EEWR register.

**/
EFI_STATUS
EFIAPI
WriteEewr (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT16               Address,
  IN UINT16               Data
  )
{
  EFI_STATUS              Status;
  FOXVILLE_EEWR_REGISTER  Request;

  Request.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Form the request field.
  //
  Request.Bits.Valid   = V_EEWR_COMMAND_VALID;
  Request.Bits.Address = Address;
  Request.Bits.Data    = Data;

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_EEWR,
             Request.Uint32
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Polling to check the EEWR register write operation is done.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Device had processed the write operation.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Device had not processed the write operation.

**/
EFI_STATUS
EFIAPI
PollEewrWriteDone (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS              Status;
  FOXVILLE_EEWR_REGISTER  Response;
  UINTN                   Count;

  Response.Uint32 = 0;
  Count           = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_EEWR_DONE_MAX_ATTEMPT; Count++) {
    Status = ReadEewr (PciIoProtocolPtr, &(Response.Uint32));
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if (Response.Bits.WriteDone == V_EEWR_WRITE_DONE) {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_EEWR_DONE_TIMEOUT);
  }

  return EFI_TIMEOUT;
}

//
// 0x1201C (RW): Flash Access Register.
//

/**
  Read the FLA register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the FLA register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the FLA register.

**/
EFI_STATUS
EFIAPI
ReadFla (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS             Status;
  FOXVILLE_FLA_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLA,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the FLA register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the FLA register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the FLA register.

**/
EFI_STATUS
EFIAPI
WriteFla (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLA,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

//
// 0x12048 (RW): Software Flash Control Register.
//

/**
  Read the FLSWCTL register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the FLSWCTL register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the FLSWCTL register.

**/
EFI_STATUS
EFIAPI
ReadFlashSwCtrl (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLSWCTL_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLSWCTL,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the FLSWCTL register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the FLSWCTL register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the FLSWCTL register.

**/
EFI_STATUS
EFIAPI
WriteFlashSwCtrl (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLSWCTL,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Polling to check the FLSWCTL register flash operation is done.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Device had processed the flash operation.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Device had not processed the flash operation.

**/
EFI_STATUS
EFIAPI
PollFlashSwCtrlDone (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLSWCTL_REGISTER  Response;
  UINTN                      Count;

  Response.Uint32 = 0;
  Count           = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_FLASH_SW_CTRL_DONE_MAX_ATTEMPT; Count++) {
    Status = ReadFlashSwCtrl (PciIoProtocolPtr, &(Response.Uint32));
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if (Response.Bits.Done == V_FLSWCTL_DONE_SET) {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_FLASH_SW_CTRL_DONE_TIMEOUT);
  }

  return EFI_TIMEOUT;
}

/**
  Polling to check the FLSWCTL register flash operation is done and flash is not busy.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Device had processed the flash operation and flash is not busy.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Device had not processed the flash operation and flash is not busy.

**/
EFI_STATUS
EFIAPI
PollFlashSwCtrlDoneAndFlashBusy (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLSWCTL_REGISTER  Response;
  UINTN                      Count;

  Response.Uint32 = 0;
  Count           = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_FLASH_SW_CTRL_DONE_AND_FLASH_BUSY_MAX_ATTEMPT; Count++) {
    Status = ReadFlashSwCtrl (PciIoProtocolPtr, &(Response.Uint32));
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if ((Response.Bits.FlashBusy == V_FLSWCTL_FLASH_BUSY_CLEAR) && \
        (Response.Bits.Done      == V_FLSWCTL_DONE_SET))
    {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_FLASH_SW_CTRL_DONE_AND_FLASH_BUSY_TIMEOUT);
  }

  return EFI_TIMEOUT;
}

//
// 0x1204C (RW): Software Flash Burst Data Register.
//

/**
  Read the FLSWDATA register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the FLSWDATA register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the FLSWDATA register.

**/
EFI_STATUS
EFIAPI
ReadFlashSwData (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS                  Status;
  FOXVILLE_FLSWDATA_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLSWDATA,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the FLSWDATA register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the FLSWDATA register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the FLSWDATA register.

**/
EFI_STATUS
EFIAPI
WriteFlashSwData (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLSWDATA,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

//
// 0x12050 (RW): Software Flash Burst Access Counter Register.
//

/**
  Read the FLSWCNT register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the FLSWCNT register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the FLSWCNT register.

**/
EFI_STATUS
EFIAPI
ReadFlashSwCnt (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLSWCNT_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLSWCNT,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the FLSWCNT register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the FLSWCNT register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the FLSWCNT register.

**/
EFI_STATUS
EFIAPI
WriteFlashSwCnt (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLSWCNT,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

//
// 0x12054 (RO in Secured Mode): Flash Op-code Register.
//

/**
  Read the FLASHOP register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the FLASHOP register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the FLASHOP register.

**/
EFI_STATUS
EFIAPI
ReadFlashOp (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS                 Status;
  FOXVILLE_FLASHOP_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLASHOP,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the FLASHOP register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the FLASHOP register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the FLASHOP register.

**/
EFI_STATUS
EFIAPI
WriteFlashOp (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLASHOP,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

//
// 0x12108 (RW): Flash Firmware Code Update Register.
//

/**
  Read the FLFWUPDATE register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the FLFWUPDATE register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the FLFWUPDATE register.

**/
EFI_STATUS
EFIAPI
ReadFlashFwUpdate (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_FLFWUPDATE_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLFWUPDATE,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the FLFWUPDATE register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_SUCCESS            Succeed to write the FLFWUPDATE register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to write the FLFWUPDATE register.

**/
EFI_STATUS
EFIAPI
WriteFlashFwUpdate (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  EFI_STATUS  Status;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = WriteMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLFWUPDATE,
             Value
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Set the FLFWUPDATE register UPDATE bit.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Succeed to set UPDATE bit in FLFWUPDATE register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to set UPDATE bit in FLFWUPDATE register.

**/
EFI_STATUS
EFIAPI
SetFlashFwUpdateUpdate (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_FLFWUPDATE_REGISTER  Request;
  FOXVILLE_FLFWUPDATE_REGISTER  Response;

  Request.Uint32  = 0;
  Response.Uint32 = 0;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadFlashFwUpdate (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  Request.Uint32        = Response.Uint32;
  Request.Bits.Update   = V_FLFWUPDATE_UPDATE_SET;
  Request.Bits.AuthDone = V_FLFWUPDATE_AUTH_DONE_CLEAR;

  Status = WriteFlashFwUpdate (PciIoProtocolPtr, Request.Uint32);
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Check the FLFWUPDATE register authentication done bit.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  IsDonePtr         Pointer to indicate the authentication done status.

  @retval  EFI_SUCCESS            Succeed to get the authentication done status.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to get the authentication done status.

**/
EFI_STATUS
EFIAPI
CheckFlashFwUpdateAuthDone (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT BOOLEAN              *IsDonePtr
  )
{
  EFI_STATUS                    Status;
  FOXVILLE_FLFWUPDATE_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (IsDonePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *IsDonePtr = FALSE;

  Status = ReadFlashFwUpdate (PciIoProtocolPtr, &(Response.Uint32));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  if (Response.Bits.AuthDone == V_FLFWUPDATE_AUTH_DONE_SET) {
    *IsDonePtr = TRUE;
  }

  return EFI_SUCCESS;
}

/**
  Polling to check the FLFWUPDATE register authentication done bit.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.

  @retval  EFI_SUCCESS            Device had processed the authentication operation.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  EFI_TIMEOUT            Operation is time out.
  @retval  Others                 Device had not processed the authentication operation.

**/
EFI_STATUS
EFIAPI
PollFlashFwUpdateAuthDone (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr
  )
{
  EFI_STATUS  Status;
  BOOLEAN     IsDone;
  UINTN       Count;

  IsDone = FALSE;

  if (PciIoProtocolPtr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (Count = 0; Count < FOXVILLE_FLFWUPDATE_AUTH_DONE_MAX_ATTEMPT; Count++) {
    Status = CheckFlashFwUpdateAuthDone (PciIoProtocolPtr, &IsDone);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    if (IsDone) {
      return EFI_SUCCESS;
    }

    gBS->Stall (FOXVILLE_FLFWUPDATE_AUTH_DONE_TIMEOUT);
  }

  return EFI_TIMEOUT;
}

//
// 0x12114 (Host-RO; FW-RW): Flash Security Register.
//

/**
  Read the FLSECU register.

  @param[in]   PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[out]  ValuePtr          Pointer to return the value of register.

  @retval  EFI_SUCCESS            Succeed to read the FLSECU register.
  @retval  EFI_INVALID_PARAMETER  Any input parameter is invalid.
  @retval  EFI_DEVICE_ERROR       Operation could not be complete.
  @retval  Others                 Failed to read the FLSECU register.

**/
EFI_STATUS
EFIAPI
ReadFlashSecurity (
  IN     EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
     OUT UINT32               *ValuePtr
  )
{
  EFI_STATUS                Status;
  FOXVILLE_FLSECU_REGISTER  Response;

  Response.Uint32 = 0;

  if ((PciIoProtocolPtr == NULL) || (ValuePtr == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = ReadMacRegUint32 (
             PciIoProtocolPtr,
             R_FOXVILLE_FLSECU,
             &(Response.Uint32)
             );
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }

  *ValuePtr = Response.Uint32;

  return EFI_SUCCESS;
}

/**
  Write the FLSECU register.

  @param[in]  PciIoProtocolPtr  Pointer to the PCI IO protocol instance.
  @param[in]  Value             Value to write the register.

  @retval  EFI_UNSUPPORTED  Not supported to write the register.

**/
EFI_STATUS
EFIAPI
WriteFlashSecurity (
  IN EFI_PCI_IO_PROTOCOL  *PciIoProtocolPtr,
  IN UINT32               Value
  )
{
  ASSERT (FALSE);

  return EFI_UNSUPPORTED;
}
