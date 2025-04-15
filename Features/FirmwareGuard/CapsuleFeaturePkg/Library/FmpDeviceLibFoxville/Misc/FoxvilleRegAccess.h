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

#ifndef __FOXVILLE_REG_ACCESS_H__
#define __FOXVILLE_REG_ACCESS_H__

//
// EDK-II Foundation.
//
#include <Uefi.h>
#include <Protocol/PciIo.h>

#define FOXVILLE_EERD_DONE_TIMEOUT                              5
#define FOXVILLE_EERD_DONE_MAX_ATTEMPT                          100000
#define FOXVILLE_EEWR_DONE_TIMEOUT                              5
#define FOXVILLE_EEWR_DONE_MAX_ATTEMPT                          100000
#define FOXVILLE_EEC_FLASH_UPDATE_DONE_TIMEOUT                  5
#define FOXVILLE_EEC_FLASH_UPDATE_DONE_MAX_ATTEMPT              20000
#define FOXVILLE_FLASH_SW_CTRL_DONE_TIMEOUT                     5
#define FOXVILLE_FLASH_SW_CTRL_DONE_MAX_ATTEMPT                 1000
#define FOXVILLE_FLASH_SW_CTRL_DONE_AND_FLASH_BUSY_TIMEOUT      1
#define FOXVILLE_FLASH_SW_CTRL_DONE_AND_FLASH_BUSY_MAX_ATTEMPT  10000000
#define FOXVILLE_FLFWUPDATE_AUTH_DONE_TIMEOUT                   200000
#define FOXVILLE_FLFWUPDATE_AUTH_DONE_MAX_ATTEMPT               25

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

#endif
