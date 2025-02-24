/** @file
  The GUID definition for IGPU Info Lib

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

#ifndef __IGPU_INFO_LIB_H__
#define __IGPU_INFO_LIB_H__

#include <Uefi/UefiBaseType.h>
#include <Pi/PiPeiCis.h>

/**
  This function will check if IGPU is Enabled or Supported

  @retval TRUE          IGPU Enabled or Supported
  @retval FALSE         IGPU not Enabled or not Supported
**/
BOOLEAN
EFIAPI
IGpuIsSupported (
  VOID
  );

/**
  Checks if Display is present on 0/2/0 or not.

  @retval FALSE  Display is not present
  @retval TRUE   Display is present
**/
BOOLEAN
EFIAPI
IGpuIsDisplayPresent (
  VOID
  );

/**
  This function will check if IGPU Media Present or not

  @retval TRUE          IGPU Media Present
  @retval FALSE         IGPU Media not Present
**/
BOOLEAN
EFIAPI
IGpuIsMediaPresent (
  VOID
  );

/**
  This function will check if IGPU Gt Present or not

  @retval TRUE          IGPU Gt Present
  @retval FALSE         IGPU Gt not Present
**/
BOOLEAN
EFIAPI
IGpuIsGtPresent (
  VOID
  );

/**
  This function will get the IGPU Data Stolen Memory Size.

  @retval DsmSize          IGPU Data Stolen Memory Size
**/
UINT32
EFIAPI
IGpuGetDsmSizeInBytes (
  VOID
  );

/**
  This function will return LMem Bar Length
  @retval     LMemBar Size
**/
UINT64
EFIAPI
IGpuGetLMemBarSize (
  VOID
  );

/**
  Get IGpu GSM Size in Mb
  @retval UINT32 - return the IGpu Ggms Size in Mb
**/
UINT32
EFIAPI
IGpuGetGsmSizeInBytes (
  VOID
  );

/**
  This function will check if DSM base is consistent in different memory scopes.
  @retval TRUE          DSM Base is consistent
  @retval FALSE         DSM Base is not consistent
**/
BOOLEAN
EFIAPI
IGpuIsDsmBaseAndImrMemoryConsistent (
  VOID
  );

/**
  This function will check if GSM base is consistent in different memory scopes.
  @retval TRUE          GSM Base is consistent
  @retval FALSE         GSM Base is not consistent
**/
BOOLEAN
EFIAPI
IGpuIsGsmBaseAndImrMemoryConsistent (
  VOID
  );

/**
This function will check if GCD RC6 is in Range or not on IGD.
@param[in] GttMmAdr         IGD MMIO Base address value
@retval TRUE          Gcd RC6 is in Range
@retval FALSE         Gcd RC6 is not in Range OR GT is not present OR IGPU is not supported
**/
BOOLEAN
EFIAPI
IGpuIsGcdRc6RangeValid (
  VOID
  );

/**
  This function will check if GCD RC6 Context Base is locked or not on IGD
  @param[in] GttMmAdr         IGD MMIO Base address value
  @retval TRUE          GCD RC6 is Locked
  @retval FALSE         If IGPU is not supported OR GCD RC6 is not locked OR GT not present
**/
BOOLEAN
EFIAPI
IGpuIsGcdRc6Locked (
  VOID
  );

/**
  This function will check if Media RC6 Context Base is locked or not on IGD
  @retval TRUE          Media RC6 is Locked
  @retval FALSE         If IGPU is not supported OR Media RC6 is not locked OR Media not present
**/
BOOLEAN
EFIAPI
IGpuIsMediaRc6Locked (
  VOID
  );

/**
  Get GttMmAddr Base Address

  @retval UINT64 - return the GttMmAddr Base Address
**/
UINT64
EFIAPI
IGpuGetGttMmAdrBase (
  VOID
  );

/**
  Get Dsm Base Address.
  @retval 32 Bit Base Address of Dsm Register.
**/
UINT32
EFIAPI
IGpuGetDsmBase (
  VOID
  );

/**
  Get Gsm Base Address.
  @retval 32 Bit Base Address of Gsm Register.
**/
UINT32
EFIAPI
IGpuGetGsmBase (
  VOID
  );

/**
  Get IGpuSgData After Setting Lock Bit Of IGpuSg Register .
  @retval 32 Bit Data After Setting Lock Bit Of IGpuSg Register.
**/
UINT32
EFIAPI
IGpuGetGgcSgData (
  VOID
  );

/**
  Print IGPU PCI Config space in Debug log.
  @retval None
**/
VOID
EFIAPI
IGpuPrintPciConfig (
  VOID
  );

/**
  This function will check if PAVP is in Range or not on IGD.

  @retval TRUE          RC6 is in Range
  @retval FALSE         RC6 is not in Range
**/
BOOLEAN
EFIAPI
IGpuIsWoPcmRangeValid (
  VOID
  );

/**
  This function will check if PAVP is within Gfx Stolen range
  @retval BOOLEAN - TRUE  WOPCM is within Gfx Stolen range
                    FALSE WOPCM is not within Gfx Stolen range
**/
BOOLEAN
IsWopcmWithinStolenGfx (
  VOID
  );

/**
  This function will check if Media CCS base is consistent in different memory scopes.

  @retval TRUE          Media CCS Base is consistent
  @retval FALSE         Media CCS Base is not consistent or IGPU is not supported or Media is not present
**/
BOOLEAN
EFIAPI
IGpuIsMediaCcsBaseAndImrMemoryConsistent (
  VOID
  );

/**
  This function will check if SA Media RC6 is in Range or not on IGD.

  @retval TRUE          Media RC6 is in Range
  @retval FALSE         Media RC6 is not in Range OR IGPU is not supported OR Media is not present
**/
BOOLEAN
EFIAPI
IGpuIsMediaRc6RangeValid (
  VOID
  );

/**
  IGpuGetDisplayDsmBase : Get Display Dsm Base Address.

  @retval UINT32 - return Display Dsm Base
**/
UINT32
EFIAPI
IGpuGetDisplayDsmBase (
  VOID
  );

/**
  IGpuGetDisplayGsmBase : Get Display Gsm Base Address.

  @retval UINT32 - return Display Gsm Base
**/
UINT32
EFIAPI
IGpuGetDisplayGsmBase (
  VOID
  );

/**
  IGpuGetDisplayGgcData : Get Display Ggc Data

  @retval UINT32 - return Display Ggc Data
**/
UINT32
EFIAPI
IGpuGetDisplayGgcData (
  VOID
  );

/**
  This function will check if WOPCM base is consistent in different memory scopes.

  @retval TRUE          WOPCM Base is consistent
  @retval FALSE         WOPCM Base is not consistent
**/
BOOLEAN
EFIAPI
IGpuIsWoPcmBaseRegistersConsistent (
  VOID
  );

/**
  This function will check if PAVP is locked in different memory scopes.

  @retval TRUE          PAVP is locked
  @retval FALSE         PAVP is not locked
**/
BOOLEAN
EFIAPI
IGpuIsPavpBitLocked (
  VOID
  );

/**
  This function will check if Gt CCS base is consistent in different memory scopes.

  @retval TRUE          Gt CCS Base is consistent
  @retval FALSE         Gt CCS Base is not consistent or GT is not present or IGPU is not supported
**/
BOOLEAN
EFIAPI
IGpuIsGtCcsBaseAndImrMemoryConsistent (
  VOID
  );

/**
  This function will check if Gt CCS is enabled

  @retval TRUE          CCS Enable
  @retval FALSE         CCS Disable
**/
BOOLEAN
EFIAPI
IGpuIsGtCcsEnabled (
  VOID
  );

/**
  This function will check if Media CCS is enabled

  @retval TRUE          CCS Enable
  @retval FALSE         CCS Disable
**/
BOOLEAN
EFIAPI
IGpuIsMediaCcsEnabled (
  VOID
  );

/**
  Get IGPU Flat CCS (Compression Control Surface) Size in Mb

  @retval UINT32 - return the Flat CCS Size in Mb
**/
UINT32
EFIAPI
IGpuGetFlatCcsSizeInMb (
  VOID
  );

/**
  This function will return the IGPU Vendor ID

  @retval VendorId        IGPU 0 2 0 Vendor ID
**/
UINT16
EFIAPI
IGpuGetVendorId (
  VOID
  );

/**
  This function will return the IGPU SVID

  @retval SubsystemVendorId        IGPU 0 2 0 SVID
**/
UINT16
EFIAPI
IGpuGetSubsystemVendorId (
  VOID
  );

/**
  This function will return the IGPU Device ID

  @retval DeviceId        IGPU 0 2 0 Device ID
**/
UINT16
EFIAPI
IGpuGetDeviceId (
  VOID
  );

/**
  This function will check if Bus Initiator and Memory access on 0:2:0 is enabled or not

  @retval TRUE          IGD Bus Initiator Access and Memory Space access is Enabled
  @retval FALSE         IGD Bus Initiator Access and Memory Space access is Disable
**/
BOOLEAN
EFIAPI
IGpuCmdRegEnabled (
  );

/**
  Get LMemBar

**/
UINT64
EFIAPI
IGpuGetLMemBar (
  VOID
  );

/**
  This function will return the IGPU Subsystem ID

  @retval SubsystemId        IGPU 0 2 0 Subsystem ID
**/
UINT16
EFIAPI
IGpuGetSubsystemId (
  VOID
  );

/**
  This function will enable Bus Initiator and Memory access on 0:2:0

**/
VOID
EFIAPI
IGpuEnableCmdReg (
  VOID
  );

/**
  This function will clear command register of 0:2:0

**/
VOID
EFIAPI
IGpuClearCmdReg (
  VOID
  );

/**
  IGpuGetDevNumber: Get IGpu Dev Number

  @retval PCI dev number for IGpu
**/
UINT8
EFIAPI
IGpuGetDevNumber (
  VOID
  );

/**
  IGpuGetFuncNumber: Get IGpu Fun Number

  @retval PCI fun number for IGpu
**/
UINT8
EFIAPI
IGpuGetFuncNumber (
  VOID
  );

/**
  IGpuGetGttMmAdrOffset: Get GTTMMADR Offset value

  @retval PCI Offset for GTTMMADR
**/
UINT8
EFIAPI
IGpuGetGttMmAdrOffset (
  VOID
  );

/**
  IGpuGetLMemBarOffset: Get LMEMBAR Offset value

  @retval PCI Offset for LMEMBAR
**/
UINT8
EFIAPI
IGpuGetLMemBarOffset (
  VOID
  );

/**
  Get L3Bank register status

  @retval         Status of L3BANK lock bit
**/
BOOLEAN
IGpuIsL3BankLocked (
  VOID
  );

/**
  Get L3Bank Media register status
  @param[in]      pInst      The instance

  @retval         Status of L3BANK MEDIA lock bit
**/

BOOLEAN
IGpuIsL3BankMediaLocked (
  VOID
  );

/**
  Enable Force Wake Up for GT.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
VOID
EFIAPI
IGpuForceWakeupGt (
  VOID
  );

/**
  Enable Idle mode for GT.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
VOID
EFIAPI
IGpuIdleGt (
  VOID
  );

/**
  Enable Force Wake Up for media.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
VOID
EFIAPI
IGpuForceWakeupMedia (
  VOID
  );

/**
  Enable Idle mode for Media.

  @param[in] pInst              Pointer to IP inst

  @retval                       IpCsiStsSuccess on success
  @retval                       !IpCsiStsSuccess on failure
**/
VOID
EFIAPI
IGpuIdleMedia (
  VOID
  );

/**
  Updates the progress bar on the VGA display.

  This function is responsible for updating the progress bar displayed on the VGA screen.
  It ensures that the visual representation of the progress is accurately reflected based
  on the current progress state.

  @param[in] Percentage  The percentage of the progress bar to fill (0-100).
**/
VOID
UpdateProgressBar (
  IN UINT8  Percentage
  );

/**
  Clear the VGA display based on the current video mode (Mode 3 or Mode 12).

  This function checks the current VGA mode, and depending on whether the mode is
  Mode 3 (80x25 text mode) or Mode 12 (640x480 graphics mode with 16 colors),
  it clears the display by setting all memory locations to zero, effectively
  resetting the display to a blank state (black background).
**/
VOID
ClearVgaDisplay (
  VOID
  );

/**
  Write a block of graphics data to the VGA memory in Mode 12h.

  This function writes a block of graphics data to the VGA memory in Mode 12h (640x480, 16 colors).
  It processes each of the 4 VGA planes and writes the corresponding data to VGA memory.

  @param[in] X          The X coordinate of the top-left corner of the block.
  @param[in] Y          The Y coordinate of the top-left corner of the block.
  @param[in] Width      The width of the block.
  @param[in] Height     The height of the block.
  @param[in] VgaBuffer  Pointer to the buffer containing the graphics data.
**/
VOID
EFIAPI
WriteVgaMode12Graphics (
  IN UINT32      X,
  IN UINT32      Y,
  IN UINT32      Width,
  IN UINT32      Height,
  IN const VOID  *VgaBuffer
  );

#endif // __IGPU_INFO_LIB_H__
