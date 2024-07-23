/** @file
  This file contains the tests for the SecureIntegratedGraphics bit

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#include "HstiIhvDxe.h"
#include <Library/IGpuInfoLib.h>


/**
  Check for IGD stolen memory Consistency

  @retval BOOLEAN - TRUE  IGD stolen memory ranges are allocated correctly:
                          Contiguos, DSM below TOLUD, GSM below DSM
                    FALSE IGD stolen memory ranges are not allocated correctly
**/
BOOLEAN
IsIgdStolenMemoryConsistent (
  VOID
  )
{
  BOOLEAN                           Result;
  UINT32                            Bdsm;
  UINT32                            Bgsm;
  UINT32                            Tolud;
  UINT32                            GsmSize;
  UINT32                            DsmSize;

  Result  = TRUE;
  DsmSize = IGpuGetDsmSizeInBytes ();
  GsmSize = IGpuGetGsmSizeInBytes ();

  Bdsm  = (UINT32) GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmBase);
  Bgsm  = (UINT32) GetHostBridgeRegisterData (HostBridgeBgsm, HostBridgeBgsmBase);
  Tolud = (UINT32) GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);

  DEBUG ((DEBUG_INFO, "        INFO: DSM:   0x%08x  Size: 0x%08x\n", Bdsm, DsmSize));
  DEBUG ((DEBUG_INFO, "        INFO: GSM:   0x%08x  Size: 0x%08x\n", Bgsm, GsmSize));
  DEBUG ((DEBUG_INFO, "        INFO: TOLUD: 0x%08x\n", Tolud));

  //
  // If IGD stolen memory ranges: DSM and GSM are allocated correctly:
  // Contiguos, DSM below TOLUD and GSM below DSM
  //

  if (Bgsm != Bdsm - GsmSize) {
    DEBUG ((DEBUG_ERROR, "         Mistmatch found BGSM: 0x%x != BDSM: %x - GGMSize: %x\n",Bgsm,Bdsm,GsmSize));

    Result = FALSE;
  }
  if (Bdsm != Tolud - DsmSize) {
    DEBUG ((DEBUG_ERROR, "        Mistmatch found  BDSM: 0x%x != TOLUD: %x - DSMSize: %x\n",Bdsm,Tolud,DsmSize));

    Result = FALSE;
  }

  return Result;
}

/**
  Check for DSM Registers Consistency

  @retval BOOLEAN - TRUE  DSM registers have the same address value programmed
                    FALSE DSM registers do not have same address or are equals to 0
**/
BOOLEAN
IsDsmRegistersConsistent (
  VOID
  )
{
  UINT32                       Bdsm;
  UINT32                       MediaBdsmGttMmAdr;
  UINT32                       DisplayBdsmGttMmAdr;

  Bdsm = (UINT32) GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmBase);
  MediaBdsmGttMmAdr   = IGpuGetDsmBase ();

  DisplayBdsmGttMmAdr = IGpuGetDisplayDsmBase ();

  if ((Bdsm != MediaBdsmGttMmAdr) || (Bdsm != DisplayBdsmGttMmAdr)) {
    DEBUG ((DEBUG_ERROR, "         Dsm register mistmatch found!\n"));
    DEBUG ((DEBUG_ERROR, "         BdsmAdr: 0x%x\n", Bdsm));
    DEBUG ((DEBUG_ERROR, "         MediaBdsmGttMmAdr: 0x%x\n", MediaBdsmGttMmAdr));
    DEBUG ((DEBUG_ERROR, "         DisplayBdsmGttMmAdr: 0x%x\n", DisplayBdsmGttMmAdr));
    return FALSE;
  }
  return TRUE;
}

/**
  Check for GSM Registers Consistency

  @retval BOOLEAN - TRUE  GSM registers have the same address value programmed
                    FALSE GSM registers do not have same address or are equals to 0
**/
BOOLEAN
IsGsmRegistersConsistent (
  VOID
  )
{
  UINT32                      Bgsm;
  UINT32                      MediaBgsmGttMmAdr;
  UINT32                      DisplayBgsmGttMmAdr;

  Bgsm = (UINT32) GetHostBridgeRegisterData (HostBridgeBgsm, HostBridgeBgsmBase);
  MediaBgsmGttMmAdr   = IGpuGetGsmBase ();
  DisplayBgsmGttMmAdr = IGpuGetDisplayGsmBase ();

  if ((Bgsm != MediaBgsmGttMmAdr) || (Bgsm != DisplayBgsmGttMmAdr)) {
    DEBUG ((DEBUG_ERROR, "         Gsm register mistmatch found!\n"));
    DEBUG ((DEBUG_ERROR, "         BgsmAdr: 0x%x\n", Bgsm));
    DEBUG ((DEBUG_ERROR, "         MediaBgsmGttMmAdr: 0x%x\n", MediaBgsmGttMmAdr));
    DEBUG ((DEBUG_ERROR, "         DisplayBgsmGttMmAdr: 0x%x\n", DisplayBgsmGttMmAdr));
    return FALSE;
  }
  return TRUE;
}

/**
  Check for GGC Registers Consistency

  @retval BOOLEAN - TRUE  GGC registers have the same address value programmed
                    FALSE GGC registers do not have same address or are equals to 0
**/
BOOLEAN
IsGgcRegistersConsistent (
  VOID
  )
{
  UINT32          GgcAdr;
  UINT32          MediaGgcGttMmAdr;
  UINT32          DisplayGgcGttMmAdr;

  GgcAdr  =  (UINT16) GetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcMask);
  MediaGgcGttMmAdr    = IGpuGetGgcSgData ();
  DisplayGgcGttMmAdr  = IGpuGetDisplayGgcData ();

  if ((GgcAdr != MediaGgcGttMmAdr) || (GgcAdr != DisplayGgcGttMmAdr)) {
    DEBUG ((DEBUG_ERROR, "         Ggc register mistmatch found!\n"));
    DEBUG ((DEBUG_ERROR, "         GgcAdr: 0x%x\n", GgcAdr));
    DEBUG ((DEBUG_ERROR, "         MediaGgcGttMmAdr: 0x%x\n", MediaGgcGttMmAdr));
    DEBUG ((DEBUG_ERROR, "         DisplayGgcGttMmAdr: 0x%x\n", DisplayGgcGttMmAdr));
    return FALSE;
  }
  return TRUE;
}

/**
  Check DSM Bit Locked

  @retval BOOLEAN - TRUE  DSM bit is locked
                    FALSE DSM bit is not locked
**/
BOOLEAN
IsDsmBitLocked (
  VOID
  )
{

  if ((BOOLEAN) GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmLock) == 0){
    DEBUG ((DEBUG_ERROR, "         BdsmLock bit is unlocked\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Check GSM Bit Locked

  @retval BOOLEAN - TRUE  GSM bit is locked
                    FALSE GSM bit is not locked
**/
BOOLEAN
IsGsmBitLocked (
  VOID
  )
{

  if ((BOOLEAN) GetHostBridgeRegisterData (HostBridgeBgsm, HostBridgeBgsmLock) == 0){
    DEBUG ((DEBUG_ERROR, "         BgsmLock bit is unlocked\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Check GGC Bit Locked

  @retval BOOLEAN - TRUE  GGC bit is locked
                    FALSE GGC bit is not locked
**/
BOOLEAN
IsGgcBitLocked (
  VOID
  )
{
  UINT32    GgcLock;

  GgcLock =  (BOOLEAN) GetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgGgcLock);

  if (GgcLock == 0){
    DEBUG ((DEBUG_ERROR, "         GgcLock bit is unlocked\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Check TOLUD Bit Locked

  @retval BOOLEAN - TRUE  TOLUD bit is locked
                    FALSE TOLUD bit is not locked
**/
BOOLEAN
IsToludBitLocked (
  VOID
  )
{

  if ((BOOLEAN) GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludLock) == 0){
    DEBUG ((DEBUG_ERROR, "         ToludLock bit is unlocked\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Check TOUUD Bit Locked

  @retval BOOLEAN - TRUE  TOUUD bit is locked
                    FALSE TOUUD bit is not locked
**/
BOOLEAN
IsTouudBitLocked (
  VOID
  )
{

  if ((BOOLEAN) GetHostBridgeRegisterData (HostBridgeTouud, HostBridgeTouudLock) == 0){
    DEBUG ((DEBUG_ERROR, "         TouudLock bit is unlocked\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Check TSEG Bit Locked

  @retval BOOLEAN - TRUE  TSEG bit is locked
                    FALSE TSEG bit is not locked
**/
BOOLEAN
IsTsegBitLocked (
  VOID
  )
{

  if ((BOOLEAN) GetHostBridgeRegisterData (HostBridgeTsegMb, HostBridgeTsegMbLock) == 0){
    DEBUG ((DEBUG_ERROR, "         TsegLock bit is unlocked\n"));
    return FALSE;
  }

  return TRUE;
}

/**
  Check DSM range valid

  @retval BOOLEAN - TRUE  DSM range valid
                    FALSE DSM range valid
**/
BOOLEAN
IsDsmRangeValid (
  VOID
  )
{
  UINT32                       DsmSize;
  UINT32                       Bdsm;
  UINT32                       Tolud;

  DsmSize        = IGpuGetDsmSizeInBytes ();

  Bdsm  = (UINT32) GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmBase);
  Tolud = (UINT32) GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);


  if ((Bdsm + DsmSize) != Tolud) {
    DEBUG ((DEBUG_ERROR, "         Invalid Range found BDSM: 0x%x + GGMSize: 0x%x != TOLUD: 0x%x\n", Bdsm, DsmSize, Tolud));

    return FALSE;
  }

  return TRUE;
}

/**
  Check GSM range valid

  @retval BOOLEAN - TRUE  GSM range valid
                    FALSE GSM range valid
**/
BOOLEAN
IsGsmRangeValid (
  VOID
  )
{
  UINT32                     GsmSize;
  UINT32                     Bdsm;
  UINT32                     Bgsm;

  Bdsm  = (UINT32) GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmBase);
  Bgsm  = (UINT32) GetHostBridgeRegisterData (HostBridgeBgsm, HostBridgeBgsmBase);

  GsmSize     = IGpuGetGsmSizeInBytes();

  if ((Bgsm + GsmSize) != Bdsm){
    DEBUG ((DEBUG_ERROR, "         Invalid Range found BGSM: 0x%x + GGMSize: 0x%x != BDSM: 0x%x\n", Bgsm, GsmSize,Bdsm));

    return FALSE;
  }

  return TRUE;
}

/**
  Run tests for SecureIntegratedGraphicsConfiguration bit
**/
VOID
CheckSecureIntegratedGraphicsConfiguration (
  VOID
  )
{
  EFI_STATUS      Status;
  BOOLEAN         Result;
  UINT16          IgdVendorId;
  UINT16          IgdDid;
  BOOLEAN         IsCommandRegChanged;

  IsCommandRegChanged = FALSE;

  //
  // Read Integrated Graphics Device
  //
  IgdVendorId     = IGpuGetVendorId () & B_PCI_VENDOR_ID_MASK;
  IgdDid          = IGpuGetDeviceId ();

  if (((mFeatureImplemented[1] & HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION) == 0) ||
      (IgdVendorId == B_PCI_VENDOR_ID_MASK) || (IgdDid == B_IGD_DID_MASK)) {
    return;
  }

  if (!IGpuCmdRegEnabled()) {

    IsCommandRegChanged = TRUE;
    ///
    /// Enable Bus Initiator and Memory access on 0:2:0 if not enabled
    ///
    IGpuEnableCmdReg ();
  }

  Result = TRUE;

  //
  // Force Wake Up GT and Media.
  //
  IGpuForceWakeupGt ();
  IGpuForceWakeupMedia ();

  DEBUG ((DEBUG_INFO, "      IGD Stolen Memory Consistency Test\n"));
  if (IsIgdStolenMemoryConsistent () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Mistmatch found among BGSM, BDSM- GSMSize or/and TOLUD - DSMSize\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_2,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_2
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      WOPCM Range Check Test\n"));
  if (IsWopcmWithinStolenGfx () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Mistmatch found among BGSM, BDSM- GSMSize or/and TOLUD - DSMSize\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_3,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_3
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      WOPCM BASE consistency Test\n"));
  if (IGpuIsWoPcmBaseRegistersConsistent () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: One or more of the WOPMC Base Register(s) is/are not equal among them or is/are equal zero \n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_4,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_4
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      DSM consistency Test\n"));
  if (IsDsmRegistersConsistent () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: DSM Registers are not equal or are equal zero \n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_5,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_5
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      GSM consistency Test\n"));
  if (IsGsmRegistersConsistent () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: GSM Registers are not equal or are equal zero \n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_6,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_6
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      GGC consistency Test\n"));
  if (IsGgcRegistersConsistent () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: GGC Registers are not equal or are equal zero \n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_7,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_7
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      PAVP locked Test\n"));
  if (IGpuIsPavpBitLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: PAVP bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_B,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_B
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      DSM locked Test\n"));
  if (IsDsmBitLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: DSM bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_C,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_C
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      GSM locked Test\n"));
  if (IsGsmBitLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: GSM bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_D,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_D
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      GGC locked Test\n"));
  if (IsGgcBitLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: GGC bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_E,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_E
      );
    Result = FALSE;
  }

  //
  // Uncore Security Configuration
  //

  DEBUG ((DEBUG_INFO, "      TOLUD locked Test\n"));
  if (IsToludBitLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TOLUD bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_F,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_F
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      TOUUD locked Test\n"));
  if (IsTouudBitLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TOUUD bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_10,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_10
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      TSEG locked Test\n"));
  if (IsTsegBitLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: TSEG bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_11,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_11
      );
    Result = FALSE;
  }

  //
  // Integrated Graphics Security Configuration
  //

  DEBUG ((DEBUG_INFO, "      GCD RC6 locked Test\n"));
  if (IGpuIsGcdRc6Locked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: GCD RC6 bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_12,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_12
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Media RC6 locked Test\n"));
  if (IGpuIsMediaRc6Locked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Media RC6 bit is unlocked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_1C,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_1C
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      DSM range valid Test\n"));
  if (IsDsmRangeValid () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: DSM range is invalid\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_13,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_13
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      GSM range valid Test\n"));
  if (IsGsmRangeValid () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: GSM range is invalid\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_14,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_14
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      WOPCM range valid Test\n"));
  if (IGpuIsWoPcmRangeValid () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: WOPCM range is invalid\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_15,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_15
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      GCD RC6 range valid Test\n"));
  if (IGpuIsGcdRc6RangeValid () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: GCD RC6 range is invalid\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_16,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_16
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      Media RC6 range valid Test\n"));
  if (IGpuIsMediaRc6RangeValid () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: Media RC6 range is invalid\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_17,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_17
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      DSM in memory controller and DSM IMR is consistent Test\n"));
  if (IGpuIsDsmBaseAndImrMemoryConsistent () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: DSM in memory controller and GSM IMR is not consistent\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_18,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_18
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      GSM in memory controller and GSM IMR is consistent Test\n"));
  if (IGpuIsGsmBaseAndImrMemoryConsistent () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: GSM in memory controller and GSM IMR is not consistent\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_19,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_19
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      CCS in Graphics and CCS IMR is consistent Test\n"));
  if ((IGpuIsGtCcsBaseAndImrMemoryConsistent () == FALSE) ||
      (IGpuIsMediaCcsBaseAndImrMemoryConsistent () == FALSE)) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: CCS is Graphics and CCS IMR is not consistent\n"));
    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_20,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_20
      );
    Result = FALSE;
  }

  //
  // CCS Enabled Tests
  //
  DEBUG ((DEBUG_INFO, "      CCS in Graphics Enabled Range bit Test\n"));
  if (IGpuIsGtCcsEnabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: CCS in Graphics Enable Range bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_21,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_21
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      CCS in Media Enabled Range bit Test\n"));
  if (IGpuIsMediaCcsEnabled () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: CCS in Media Enable Range bit is not set\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_22,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_22
      );
    Result = FALSE;
  }
  //
  // Enable Idle mode for GT and Media.
  //
  IGpuIdleGt ();
  IGpuIdleMedia ();

  DEBUG ((DEBUG_INFO, "      L3LOCKMSGREG locked Tests\n"));

  DEBUG ((DEBUG_INFO, "      L3LOCKMSGREG Media locked Test\n"));
  if (IGpuIsL3BankMediaLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: L3LOCKMSGREG Media Lock bit is not Locked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_23,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_23
      );
    Result = FALSE;
  }

  DEBUG ((DEBUG_INFO, "      L3LOCKMSGREG GT locked Test\n"));
  if (IGpuIsL3BankLocked () == FALSE) {
    DEBUG ((DEBUG_INFO, "        Unexpected Status: L3LOCKMSGREG GT Lock bit is not Locked\n"));

    BuildAndAppendHstiStatusString (
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_CODE_24,
      HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION_UNEXP_STATUS_STRING_24
      );
    Result = FALSE;
  }



  //
  // ALL PASS
  //
  if (Result) {
    Status = HstiLibSetFeaturesVerified (
               PLATFORM_SECURITY_ROLE_PLATFORM_REFERENCE,
               NULL,
               1,
               HSTI_BYTE1_SECURE_INTEGRATED_GRAPHICS_CONFIGURATION
               );
    CheckStatusForHstiLibSet (Status);
  }

  if (IsCommandRegChanged) {
    IGpuClearCmdReg ();
  }

  return;
}
