/**@file
  IP IGpu Init library.

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

#include <IpWrapper.h>
#include <IpWrapper64Bit.h>
#include <IpIGpu.h>
#include <IpIGpuVersion.h>
#include <IGpuCfg.h>
#include <IGpuDisplayRegs.h>
#include <IGpuMediaRegs.h>
#include <IpIGpuGtReg.h>

#define GT_WAIT_TIMEOUT  3000000          ///< ~3 seconds

/**
InstInit for this version of IP

@param[in]  pInst  A pointer to the IP instance to be used.

@retval     IpCsiStsSuccess on success
@retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuInstInit (
  IP_IGPU_INST  *pInst
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("%s: ERROR: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->Prefix.pName == NULL) {
    // force valid name for debug printing
    pInst->Prefix.pName = "IGpuAgent";
  }

  pInst->Signature = IP_IGPU_SIGNATURE;

  PRINT_LEVEL1 ("%s(%s):\n", __FUNCTION__, pInst->Prefix.pName);

  return IpCsiStsSuccess;
}

/**
  Since InstInit is not allowed to access any registers, IpInit is used to write to any registers needed.
  * IpInit() shall be the first API function called after InstInit.
  * this is a good place to fix bad register defaults or work around bugs

  @param[in]  pInst  A pointer to the IP instance to be used.

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuIpInit (
  IP_IGPU_INST  *pInst
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("%s: ERROR: Invalid pInst\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorBadParam;
  }

  return IpCsiStsSuccess;
}

/**
  This function will check the pInst for IpIGpu is valid or not

  @param[in]  pInst       The instance

  @retval TRUE          PInst is valid
  @retval FALSE         PInst is not valid
**/
BOOLEAN
IpIGpuIsInstValid (
  IP_IGPU_INST  *pInst
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NO_CNTXT ("%s: ERROR: Invalid pInst\n", __FUNCTION__);
    return FALSE;
  }

  if (pInst->Signature != IP_IGPU_SIGNATURE) {
    PRINT_ERROR_NO_CNTXT ("%s: ERROR: pInst signature not matching\n", __FUNCTION__);
    return FALSE;
  }

  return TRUE;
}

/**
  Get version information

  @param[in]  pInst  The instance
  @param[in]  VerId  Version ID

  @retval            0 is returned upon any error
  @retval            return value define by VerId
**/
UINT32
IpIGpuGetVersion (
  IP_IGPU_INST   *pInst,
  IP_CSI_VER_ID  VerId
  )
{
  UINT32  ReturnData;

  ReturnData = 0;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return ReturnData;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  switch (VerId) {
    case IpCsiVerIdCsi:
      ReturnData = VERSION_CSI;
      PRINT_LEVEL1 (
        "%a(IpCsiVerIdCsi) = %x.%x (0x%x)\n",
        __FUNCTION__,
        IpWrRShiftU64 (ReturnData, 16),
        ReturnData & 0xFFFF,
        ReturnData
        );
      break;
    case IpCsiVerIdHwIp:
      ReturnData = 0;
      PRINT_WARNING ("%s(IpCsiVerIdHwIp is not supported) = 0x%x\n", __FUNCTION__, ReturnData);
      break;
    case IpCsiVerIdIpFwIntf:
      ReturnData = VERSION_IPFWINTF_IGPU;
      PRINT_LEVEL1 (
        "%a(IpCsiVerIdIpFwIntf) = Gen=%x Year=%x Qtr=%x Rev=%x (0x%x)\n",
        __FUNCTION__,
        (UINT8)IpWrRShiftU64 (ReturnData, 24),
        (UINT8)IpWrRShiftU64 (ReturnData, 16),
        (UINT8)IpWrRShiftU64 (ReturnData, 8),
        (UINT8)IpWrRShiftU64 (ReturnData, 0),
        ReturnData
        );
      break;
    case IpCsiVerIdIpFwIntfExt:
      ReturnData = VERSION_IPFWINTFEXT_IGPU;
      PRINT_LEVEL1 (
        "%s(IpCsiVerIdIpFwIntfExt) = Major=%x (0x%x)\n",
        __FUNCTION__,
        (UINT8)IpWrRShiftU64 (ReturnData, 0),
        ReturnData
        );
      break;
    case IpCsiVerIdIpFw:
      ReturnData = VERSION_IPFW_IGPU;
      PRINT_LEVEL1 (
        "%s(IpCsiVerIdIpFw) = Year=%x WW=%x Day=%x Oops=%x (0x%x)\n",
        __FUNCTION__,
        (UINT8)IpWrRShiftU64 (ReturnData, 24),
        (UINT8)IpWrRShiftU64 (ReturnData, 16),
        (UINT8)IpWrRShiftU64 (ReturnData, 8),
        (UINT8)IpWrRShiftU64 (ReturnData, 0),
        ReturnData
        );
      break;
    case IpCsiVerIdHwIpCustom:
      ReturnData = 0;
      PRINT_WARNING ("%s(IpCsiVerIdHwIpCustom is not supported) = 0x%x\n", __FUNCTION__, ReturnData);
      break;
    default:
      PRINT_WARNING ("%s(%d) is not supported\n", __FUNCTION__, VerId);
      ReturnData = 0;
      break;
  }

  return ReturnData;
}

/**
  Get capability

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Cap ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval                    defined by FeatureId
**/
UINT32
IpIGpuGetCapability (
  IP_IGPU_INST     *pInst,
  UINT32           IpTarget,
  IP_IGPU_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  )
{
  UINT32  FeatureVal;

  FeatureVal = 0;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    if (pCsiSts != NULL) {
      *pCsiSts = IpCsiStsErrorBadParam;
    }

    return FeatureVal;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  switch (FeatureId) {
    default:
      PRINT_WARNING ("WARNING:%s() unknown/unexpected FeatureId=%d\n", __FUNCTION__, FeatureId);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsErrorUnknownId;
      }

      break;
  }

  if (pCsiSts != NULL) {
    *pCsiSts = IpCsiStsSuccess;
  }

  PRINT_LEVEL1 ("[%s] FeatureId: %x FeatureVal: %x\n", __FUNCTION__, FeatureId, FeatureVal);
  return FeatureVal;
}

/**
  Get status

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Status ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval         defined by FeatureId
**/
UINT32
IpIGpuGetStatus (
  IP_IGPU_INST     *pInst,
  UINT32           IpTarget,
  IP_IGPU_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  )
{
  if (pCsiSts != NULL) {
    *pCsiSts = IpCsiStsSuccess;
  }

  return IpIGpuFeatValDis;
}

/**
  Get control

  @param[in]      pInst      The instance
  @param[in]      IpTarget   IP defined value for target within the IP.
  @param[in]      FeatureId  Control ID
  @param[in, out] pCsiSts    Optional and may be NULL.  If non-NULL return API status

  @retval         defined by FeatureId
**/
UINT32
IpIGpuGetControl (
  IP_IGPU_INST     *pInst,
  UINT32           IpTarget,
  IP_IGPU_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  )
{
  if (pCsiSts != NULL) {
    *pCsiSts = IpCsiStsSuccess;
  }

  return IpIGpuFeatValDis;
}

/**
  Set control

  @param[in]  pInst       The instance
  @param[in]  IpTarget    IP defined value for target within the IP.
  @param[in]  FeatureId   Feature ID
  @param[in]  FeatureVal  Value based on FeatureId

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuSetControl (
  IP_IGPU_INST     *pInst,
  UINT32           IpTarget,
  IP_IGPU_FEAT_ID  FeatureId,
  UINT32           FeatureVal
  )
{
  IP_CSI_STATUS  Status;

  Status = IpCsiStsErrorNullPtr;
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return Status;
  }

  PRINT_LEVEL1 ("%s Start \n", __FUNCTION__);

  switch (FeatureId) {
    default:
      PRINT_WARNING ("WARNING:%s() unknown/unexpected FeatureId=%d\n", __FUNCTION__, FeatureId);
      Status = IpCsiStsErrorUnknownId;
      break;
  }

  return Status;
}

/**
  This function will check if Bus Initiator and Memory access on 0:2:0 is enabled or not

  @param[in]  pInst       The instance

  @retval TRUE          IGD Bus Initiator Access and Memory Space access is Enabled
  @retval FALSE         IGD Bus Initiator Access and Memory Space access is Disable
**/
BOOLEAN
IpIGpuCmdRegEnabled (
  IP_IGPU_INST  *pInst
  )
{
  PCICMD_IGPU_STRUCT  PciCmd;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return FALSE;
  }

  PciCmd.Data = (UINT16)IpWrRegRead (pInst->PcieCfgAccess, PCICMD_IGPU_REG, IpWrRegFlagSize16Bits);

  if ((PciCmd.Bits.mae == TRUE) && (PciCmd.Bits.bme == TRUE)) {
    return TRUE;
  }

  return FALSE;
}

/**
  This function will check if Intel Graphics is Enabled or Supported

  @param[in]  pInst       The instance

  @retval TRUE          Graphics Enabled or Supported
  @retval FALSE         Graphics not Enabled or not Supported
**/
BOOLEAN
IpIGpuSupported (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return FALSE;
  }

  if ((UINT16)IpWrRegRead (pInst->PcieCfgAccess, VID2_IGPU_REG, IpWrRegFlagSize16Bits) == 0xFFFF) {
    return FALSE;
  }

  return TRUE;
}

/**
  This function will program the GttMmAdr

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuProgramGttMmAdr (
  IP_IGPU_INST  *pInst
  )
{
  UINT64  GttMmAdr;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->IGpuPreMemConfig.GttMmAdr == 0) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: GttMmAdr is 0\n", __FUNCTION__);
    return IpCsiStsError;
  }

  GttMmAdr = IGPU_GTTMMADR_ALING (pInst->IGpuPreMemConfig.GttMmAdr);

  IpWrRegWrite (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, GttMmAdr, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

  return IpCsiStsSuccess;
}

/**
  Get GttMmAdr

  @param[in]      pInst      The instance

  @retval         GttMmAdr
**/
UINT64
IpIGpuGetGttMmAdr (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: GttMmAdr is 0\n", __FUNCTION__);
    return 0;
  }

  return IGPU_GTTMMADR_ADDRESS (IpWrRegRead (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow)));
}

/**
  This function will program the LMemBar

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuProgramLMemBar (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  if (pInst->IGpuPreMemConfig.LMemBar == 0) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: LMemBar is 0\n", __FUNCTION__);
    return IpCsiStsError;
  }

  IpWrRegWrite (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, pInst->IGpuPreMemConfig.LMemBar, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

  return IpCsiStsSuccess;
}

/**
  Get LMemBar
  @param[in]      pInst      The instance
  @retval         GttMmAdr
**/
UINT64
IpIGpuGetLMemBar (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    PRINT_ERROR_NO_CNTXT ("ERROR: %s: GttMmAdr is 0\n", __FUNCTION__);
    return 0;
  }

  return IGPU_GTTMMADR_ADDRESS (IpWrRegRead (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow)));
}

/**
  This function will enable Bus Initiator and Memory access on 0:2:0

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuEnableCmdReg (
  IP_IGPU_INST  *pInst
  )
{
  PCICMD_IGPU_STRUCT  PciCmd;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  PciCmd.Data     = (UINT16)IpWrRegRead (pInst->PcieCfgAccess, PCICMD_IGPU_REG, IpWrRegFlagSize16Bits);
  PciCmd.Bits.bme = TRUE;
  PciCmd.Bits.mae = TRUE;
  IpWrRegWrite (pInst->PcieCfgAccess, PCICMD_IGPU_REG, PciCmd.Data, IpWrRegFlagSize16Bits);

  return IpCsiStsSuccess;
}

/**
  This function will return the IGPU Vendor ID

  @param[in]  pInst       The instance

  @retval VendorId        IGPU 0 2 0 Vendor ID
**/
UINT16
IpIGpuGetVendorId (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0xFFFF;
  }

  return ((UINT16)IpWrRegRead (pInst->PcieCfgAccess, VID2_IGPU_REG, IpWrRegFlagSize16Bits));
}

/**
  This function will return the IGPU Device ID

  @param[in]  pInst       The instance

  @retval DeviceId        IGPU 0 2 0 Device ID
**/
UINT16
IpIGpuGetDeviceId (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0xFFFF;
  }

  return ((UINT16)IpWrRegRead (pInst->PcieCfgAccess, DID2_IGPU_REG, IpWrRegFlagSize16Bits));
}

/**
  This function will return the IGPU Subsystem Vendor ID

  @param[in]  pInst          The instance

  @retval SubsystemVendorId  IGPU 0 2 0 Subsystem Vendor ID
**/
UINT16
IpIGpuGetSubsystemVendorId (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0xFFFF;
  }

  return ((UINT16)IpWrRegRead (pInst->PcieCfgAccess, SVID2_IGPU_REG, IpWrRegFlagSize16Bits));
}

/**
  This function will return the IGPU Subsystem ID

  @param[in]  pInst          The instance

  @retval SubsystemId        IGPU 0 2 0 Subsystem ID
**/
UINT16
IpIGpuGetSubsystemId (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0xFFFF;
  }

  return ((UINT16)IpWrRegRead (pInst->PcieCfgAccess, SID2_IGPU_REG, IpWrRegFlagSize16Bits));
}

/**
  This function will return the IGPU Revision ID

  @param[in]  pInst         The instance

  @retval RevisionId        IGPU 0 2 0 Revision ID
**/
UINT8
IpIGpuGetRevisionId (
  IP_IGPU_INST  *pInst
  )
{
  RID2_CC_IGPU_STRUCT  RevisionId;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0xFF;
  }

  RevisionId.Data = (UINT32)IpWrRegRead (pInst->PcieCfgAccess, RID2_CC_IGPU_REG, IpWrRegFlagSize32Bits);
  return (UINT8)RevisionId.Bits.rid;
}

/**
  Get L3Bank register status
  @param[in]      pInst      The instance

  @retval         Status of L3BANK lock bit
**/
BOOLEAN
IpIGpuIsL3BankLocked (
  IP_IGPU_INST  *pInst
  )
{
  L3BANKLOCK_LBCF_GFX_GT_STRUCT  L3lockGt;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0;
  }

  L3lockGt.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, L3BANKLOCK_LBCF_GFX_GT_REG, IpWrRegFlagSize32Bits);
  if (L3lockGt.Bits.lbcflockmsgreg_lock == 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  Get L3Bank Media register status
  @param[in]      pInst      The instance

  @retval         Status of L3BANK MEDIA lock bit
**/
BOOLEAN
IpIGpuIsL3BankMediaLocked (
  IP_IGPU_INST  *pInst
  )
{
  L3BANKLOCK_LBCF_MEDIA_MEDIA_STRUCT  L3lockMedia;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0;
  }

  L3lockMedia.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, L3BANKLOCK_LBCF_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
  if (L3lockMedia.Bits.lbcflockmsgreg_lock == 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  This function will return GttMmAdr Bar Length

  @param[in]  pInst       The instance

  @retval     GttMmAdr Size on success
  @retval     0 on failure
**/
UINT64
IpIGpuGetGttMmAdrSize (
  IP_IGPU_INST  *pInst
  )
{
  GTTMMADR0_IGPU_STRUCT  GttMmAdr;
  UINT64                 SaveBar64;
  UINT64                 Data64;
  UINT64                 GttMmAdrSize64;
  UINT32                 Data32;
  UINT64                 GttMmAdrSize32;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0;
  }

  GttMmAdr.Data = (UINT32)IpWrRegRead (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, IpWrRegFlagSize32Bits);

  if ((GttMmAdr.Data & (BIT1 + BIT2)) == PCI_BASE_ADDRESS_MEM_TYPE_64) {
    //
    // Store the Current GttMmAdr
    //
    SaveBar64 = IpWrRegRead (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));
    //
    // Write all F's to GttMmAdr
    //
    IpWrRegWrite (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, MAX_UINT64, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

    //
    // Read the GttMmAdr Size
    //
    Data64 = IpWrRegRead (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

    GttMmAdrSize64 = ~(Data64 & 0xFFFFFFFFFFFFFFF0ULL) + 1;

    //
    // Restore the LMem Bar Base Address
    //
    IpWrRegWrite (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, SaveBar64, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

    return GttMmAdrSize64;
  } else {
    //
    // Write all F's to GttMmAdr
    //
    IpWrRegWrite (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, MAX_UINT32, IpWrRegFlagSize32Bits);

    //
    // Read the GttMmAdr Size
    //
    Data32 = (UINT32)IpWrRegRead (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, IpWrRegFlagSize32Bits);

    GttMmAdrSize32 = ~(Data32 & 0xFFFFFFF0) + 1;

    //
    // Restore the LMem Bar Base Address
    //
    IpWrRegWrite (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, GttMmAdr.Data, IpWrRegFlagSize32Bits);

    return (UINT64)GttMmAdrSize32;
  }
}

/**
  This function will return LMem Bar Length

  @param[in]  pInst       The instance

  @retval     LMemBar Size on success
  @retval     0 on failure
**/
UINT64
IpIGpuGetLMemBarSize (
  IP_IGPU_INST  *pInst
  )
{
  LMEMBAR0_IGPU_STRUCT  LMemBar;
  UINT64                SaveBar64;
  UINT64                Data64;
  UINT64                LMemBarSize64;
  UINT32                Data32;
  UINT64                LMemBarSize32;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return 0;
  }

  LMemBar.Data = (UINT32)IpWrRegRead (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, IpWrRegFlagSize32Bits);

  if ((LMemBar.Data & (BIT1 + BIT2)) == PCI_BASE_ADDRESS_MEM_TYPE_64) {
    //
    // Store the Current LMemBar
    //
    SaveBar64 = IpWrRegRead (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));
    //
    // Write all F's to LMemBar
    //
    IpWrRegWrite (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, MAX_UINT64, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

    //
    // Read the LMemBar Size
    //
    Data64 = IpWrRegRead (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

    LMemBarSize64 = ~(Data64 & 0xFFFFFFFFFFFFFFF0ULL) + 1;

    //
    // Restore the LMem Bar Base Address
    //
    IpWrRegWrite (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, SaveBar64, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

    return LMemBarSize64;
  } else {
    //
    // Write all F's to LMemBar
    //
    IpWrRegWrite (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, MAX_UINT32, IpWrRegFlagSize32Bits);

    //
    // Read the LMemBar Size
    //
    Data32 = (UINT32)IpWrRegRead (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, IpWrRegFlagSize32Bits);

    LMemBarSize32 = ~(Data32 & 0xFFFFFFF0) + 1;

    //
    // Restore the LMem Bar Base Address
    //
    IpWrRegWrite (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, LMemBar.Data, IpWrRegFlagSize32Bits);

    return (UINT64)LMemBarSize32;
  }
}

/**
  Function to Store the GMD Status in Private Config

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure

**/
IP_CSI_STATUS
IpIGpuSaveGMDStatus (
  IP_IGPU_INST  *pInst
  )
{
  GU_CNTL_PROTECTED_IGPU_STRUCT  GuCntl;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  GuCntl.Data                               = (UINT32)IpWrRegRead (pInst->MmioAccess, GU_CNTL_PROTECTED_IGPU_REG, IpWrRegFlagSize32Bits);
  pInst->IGpuPrivateConfig.IsDisplayPresent = (BOOLEAN)GuCntl.Bits.depresent;
  pInst->IGpuPrivateConfig.IsMediaPresent   = (BOOLEAN)GuCntl.Bits.impresent;
  pInst->IGpuPrivateConfig.IsGtPresent      = (BOOLEAN)GuCntl.Bits.gtpresent;

  return IpCsiStsSuccess;
}

/**
  Function to clear GMS, GGMS and disable IVD and Vemen

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure

**/
IP_CSI_STATUS
IpIGpuAdditionalStepToDisable (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  GGC_IGPU_STRUCT  GgcStruct;

  GgcStruct.Data       = (UINT32)IpWrRegRead (pInst->MmioAccess, GGC_IGPU_REG, IpWrRegFlagSize32Bits);
  GgcStruct.Bits.gms   = 0;
  GgcStruct.Bits.ivd   = TRUE;
  GgcStruct.Bits.vamen = TRUE;
  IpWrRegWrite (pInst->MmioAccess, GGC_IGPU_REG, GgcStruct.Data, IpWrRegFlagSize32Bits);

  return IpCsiStsSuccess;
}

/**
  Function to clear temporary Graphics BAR at end of PEI

  @param[in]  pInst       The instance

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure

**/
IP_CSI_STATUS
IpIGpuClearTempBar (
  IP_IGPU_INST  *pInst
  )
{
  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  IpWrRegWrite (pInst->PcieCfgAccess, PCICMD_IGPU_REG, 0, IpWrRegFlagSize16Bits);
  IpWrRegWrite (pInst->PcieCfgAccess, GTTMMADR0_IGPU_REG, 0, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));
  IpWrRegWrite (pInst->PcieCfgAccess, LMEMBAR0_IGPU_REG, 0, (IpWrRegFlagSize64Bits | IpWrRegFlagOrderHighToLow));

  return IpCsiStsSuccess;
}

/**
  "Poll Status" for GT/Media Readiness

  @param[in]  pInst       The instance
  @param[in] Offset          - MMIO Offset
  @param[in] Mask            - Mask
  @param[in] Result          - Value to wait for

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuPollReady (
  IP_IGPU_INST  *pInst,
  UINT32        Offset,
  UINT32        Mask,
  UINT32        Result
  )
{
  UINT32  GtStatus;
  UINT32  StallCount;

  StallCount = 0;

  ///
  /// Register read
  ///
  GtStatus = (UINT32)IpWrRegRead (pInst->MmioAccess, Offset, IpWrRegFlagSize32Bits);

  while (((GtStatus & Mask) != Result) && (StallCount < GT_WAIT_TIMEOUT)) {
    ///
    /// 10 microSec wait
    ///
    IpWrDelayUs (pInst->TimeCntxt, 10);
    StallCount += 10;

    GtStatus = (UINT32)IpWrRegRead (pInst->MmioAccess, Offset, IpWrRegFlagSize32Bits);
  }

  if (StallCount < GT_WAIT_TIMEOUT) {
    return IpCsiStsSuccess;
  } else {
    PRINT_ERROR ("StallCount = 0x%x Timeout = 0x%x\n", StallCount, GT_WAIT_TIMEOUT);
    return IpCsiStsErrorTimeout;
  }
}

/**  This function is to set IGPU Memory map.

  @param[in]  pInst       A pointer to the IP instance to be used.

  @retval                 IpCsiStsSuccess on success
  @retval                 !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuSetMemMap (
  IP_IGPU_INST  *pInst
  )
{
  DE_BDF_DISP_STRUCT             DisplayBdf;
  CSMEBDF_IGPU_STRUCT            CsmeBdf;
  GU_CNTL_PROTECTED_IGPU_STRUCT  GuCntl;
  GMD_ID_STRUCT                  MediaGmdId;

  PRINT_LEVEL1 ("%s Start\n", __FUNCTION__);

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  if (IpIGpuSupported (pInst) == FALSE) {
    PRINT_LEVEL1 ("Returning from %s since IGPU is not present\n", __FUNCTION__);
    return IpCsiStsSuccess;
  }

  if (IpIGpuGetGttMmAdr (pInst) == 0) {
    IpIGpuProgramGttMmAdr (pInst);
  }

  if (!IpIGpuCmdRegEnabled (pInst)) {
    ///
    /// Enable Bus Initiator and Memory access on 0:2:0
    ///
    IpIGpuEnableCmdReg (pInst);
  }

  // GGC
  PRINT_LEVEL1 ("Programming GGC 0x%llx \n", pInst->IGpuPrivateConfig.GgcBase);
  IpWrRegWrite (pInst->MmioAccess, GGC_IGPU_REG, pInst->IGpuPrivateConfig.GgcBase, IpWrRegFlagSize16Bits);
  IpWrRegWrite (pInst->MmioAccess, DE_GGC_DISP_REG, pInst->IGpuPrivateConfig.GgcBase, IpWrRegFlagSize16Bits);

  PRINT_LEVEL1 ("Programming BDSM 0x%llx \n", pInst->IGpuPrivateConfig.BdsmBase);
  IpWrRegWrite (pInst->MmioAccess, DSMBASE0_IGPU_REG, pInst->IGpuPrivateConfig.BdsmBase, IpWrRegFlagSize64Bits);
  IpWrRegWrite (pInst->MmioAccess, DEDSMBASE_0_DISP_REG, pInst->IGpuPrivateConfig.BdsmBase, IpWrRegFlagSize64Bits);

  // BGSM
  PRINT_LEVEL1 ("Programming BGSM 0x%llx \n", pInst->IGpuPrivateConfig.BgsmBase);
  IpWrRegWrite (pInst->MmioAccess, GSMBASE0_IGPU_REG, pInst->IGpuPrivateConfig.BgsmBase, IpWrRegFlagSize64Bits);
  IpWrRegWrite (pInst->MmioAccess, DEGSMBASE_0_DISP_REG, pInst->IGpuPrivateConfig.BgsmBase, IpWrRegFlagSize64Bits);

  // DE BDF
  DisplayBdf.Data          = (UINT32)IpWrRegRead (pInst->MmioAccess, DE_BDF_DISP_REG, IpWrRegFlagSize32Bits);
  DisplayBdf.Bits.bus      = 0x0;
  DisplayBdf.Bits.device   = 0x2;
  DisplayBdf.Bits.function = 0x0;
  IpWrRegWrite (pInst->MmioAccess, DE_BDF_DISP_REG, DisplayBdf.Data, IpWrRegFlagSize32Bits);

  if (pInst->XeVersion <= IpIGpuXe3) {
    // CSME BDF
    CsmeBdf.Data        = (UINT32)IpWrRegRead (pInst->MmioAccess, CSMEBDF_IGPU_REG, IpWrRegFlagSize32Bits);
    CsmeBdf.Bits.busnum = pInst->IGpuPrivateConfig.CsmeBusNum;
    CsmeBdf.Bits.devnum = pInst->IGpuPrivateConfig.CsmeDevNum;
    CsmeBdf.Bits.funnum = pInst->IGpuPrivateConfig.CsmeFuncNum;

    PRINT_LEVEL1 ("Programming CSME BDF 0x%x \n", CsmeBdf.Data);

    IpWrRegWrite (pInst->MmioAccess, CSMEBDF_IGPU_REG, CsmeBdf.Data, IpWrRegFlagSize32Bits);
    IpWrRegWrite (pInst->MmioAccess, VDMBDFBARKVM_DISP_REG, CsmeBdf.Data, IpWrRegFlagSize32Bits);
  }

  if (pInst->XeVersion == IpIGpuXe3) {
    MediaGmdId.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, MIRROR_GMD_ID_RPM_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
    if (MediaGmdId.Bits.GMDRevId < GMD_MEDIA_REV_30_0_0_B0) {
      if ((MediaGmdId.Data & CLEAR_GMD_REVID_MASK) == GMD_MEDIA_30_0) {
        GuCntl.Data                     = (UINT32)IpWrRegRead (pInst->MmioAccess, GU_CNTL_PROTECTED_IGPU_REG, IpWrRegFlagSize32Bits);
        GuCntl.Bits.stolen_mem_wipe_dis = TRUE;
        IpWrRegWrite (pInst->MmioAccess, GU_CNTL_PROTECTED_IGPU_REG, GuCntl.Data, IpWrRegFlagSize32Bits);
      }
    }
  }

  PRINT_LEVEL1 ("%s End\n", __FUNCTION__);

  return IpCsiStsSuccess;
}

/**
  This function is to read the register.
  @param[in] pInst         - A pointer to the IP instance to be used.
  @param[in] DataRequested - Instance of Mode of the Register
  @retval UINT64           - Register Details based on Mode
**/
UINT64
IpIGpuGetRegistersData (
  IP_IGPU_INST      *pInst,
  IP_IGPU_REGISTER  RegisterName,
  UINT16            DataRequested
  )
{
  RC6CTXBASE_RPM_GCD_0_GT_STRUCT                GtRc6Ctx;
  RC6CTXBASE_RPM_MEDIA_0_MEDIA_STRUCT           MediaRc6Ctx;
  DSMBASE0_IGPU_STRUCT                          DsmBaseCfg;
  GSMBASE0_IGPU_STRUCT                          GsmBaseCfg;
  GGC_IGPU_STRUCT                               GgcIGpuSgCfg;
  PAVPC0_REG_IGPU_STRUCT                        MediaPavpcCfg;
  DEGSMBASE_0_DISP_STRUCT                       DispGsmCfg;
  DEDSMBASE_0_DISP_STRUCT                       DispDsmCfg;
  DE_GGC_DISP_STRUCT                            DispGgcCfg;
  DEPAVPC_REG_0_DISP_STRUCT                     DisplPavpcCfg;
  FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_STRUCT  MediaFlatCcsLowBase;
  FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_STRUCT  MediaFlatCcsHighBase;
  FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_STRUCT     GtFlatCcsLowBase;
  FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_STRUCT     GtFlatCcsHighBase;
  FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_STRUCT        GtFlatCcsGamCfg;
  FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_STRUCT  MediaFlatCcsGamCfg;

  UINT64  FlatCcsBase;

  if (IpIGpuSupported (pInst) == FALSE) {
    return MAX_UINT64;
  }

  switch (RegisterName) {
    case GtRc6CtxLowMemReg:
      if (DataRequested == GtRc6CtxLowMemLock) {
        GtRc6Ctx.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_GCD_0_GT_REG, IpWrRegFlagSize32Bits);
        return (BOOLEAN)GtRc6Ctx.Bits.rc6memlock;
      } else {
        GtRc6Ctx.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_GCD_0_GT_REG, IpWrRegFlagSize32Bits);
        return GtRc6Ctx.Bits.rc6membase << RC6CTXBASE_RPM_GCD_0_GT_RC6MEMBASE_LSB;
      }

    case DsmBaseReg:
      DsmBaseCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, DSMBASE0_IGPU_REG, IpWrRegFlagSize32Bits);
      return (DsmBaseCfg.Bits.bdsm_0 << DSMBASE0_IGPU_BDSM_0_LSB);

    case GsmBaseReg:
      GsmBaseCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, GSMBASE0_IGPU_REG, IpWrRegFlagSize32Bits);
      return (GsmBaseCfg.Bits.bgsm_lsb << GSMBASE0_IGPU_BGSM_LSB_LSB);

    case GgcIGpuSgReg:
      GgcIGpuSgCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, GGC_IGPU_REG, IpWrRegFlagSize32Bits);
      return GgcIGpuSgCfg.Data;

    case MediaPavpcReg:
      if (DataRequested == MediaPavpcBaseMB) {
        MediaPavpcCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, PAVPC0_REG_IGPU_REG, IpWrRegFlagSize32Bits);
        return (UINT32)(MediaPavpcCfg.Bits.wopcmbase_0);
      } else if (DataRequested == MediaPavpcLock) {
        MediaPavpcCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, PAVPC0_REG_IGPU_REG, IpWrRegFlagSize32Bits);
        return (BOOLEAN)MediaPavpcCfg.Bits.lock;
      } else if (DataRequested == MediaPavpcSize) {
        MediaPavpcCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, PAVPC0_REG_IGPU_REG, IpWrRegFlagSize32Bits);
        return MediaPavpcCfg.Bits.wopcm_size;
      } else if (DataRequested ==  MediaPavpcData) {
        MediaPavpcCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, PAVPC0_REG_IGPU_REG, IpWrRegFlagSize32Bits);
        return (UINT32)MediaPavpcCfg.Data;
      } else {
        MediaPavpcCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, PAVPC0_REG_IGPU_REG, IpWrRegFlagSize32Bits);
        return (UINT32)(MediaPavpcCfg.Bits.wopcmbase_0 << PAVPC0_REG_IGPU_WOPCMBASE_0_LSB);
      }

    case DispPavpcReg:
      if (DataRequested == DispPavpcBase) {
        DisplPavpcCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, DEPAVPC_REG_0_DISP_REG, IpWrRegFlagSize32Bits);
        return (UINT32)(DisplPavpcCfg.Bits.wopcmbase<< DEPAVPC_REG_0_DISP_WOPCMBASE_LSB);
      } else {
        DisplPavpcCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, DEPAVPC_REG_0_DISP_REG, IpWrRegFlagSize32Bits);
        return (BOOLEAN)(DisplPavpcCfg.Bits.lock);
      }

    case DispGgcReg:
      DispGgcCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, DE_GGC_DISP_REG, IpWrRegFlagSize32Bits);
      return DispGgcCfg.Data;

    case MediaFlatCcsReg:
      MediaFlatCcsLowBase.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_LOWER_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits);
      //
      // Clear the lower bits
      //
      MediaFlatCcsLowBase.Bits.ccsenable = FALSE;
      MediaFlatCcsLowBase.Bits.rsvd_0    = 0;

      MediaFlatCcsHighBase.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_UPPER_I_SAM_MEDIA_REG, IpWrRegFlagSize32Bits);
      //
      // Clear the upper bits
      //
      MediaFlatCcsHighBase.Bits.flatccsize = 0;
      MediaFlatCcsHighBase.Bits.rsvd_0     = 0;
      MediaFlatCcsHighBase.Bits.rsvd_1     = 0;

      FlatCcsBase  = (UINT64)MediaFlatCcsLowBase.Data;
      FlatCcsBase += IpWrLShiftU64 ((UINT64)MediaFlatCcsHighBase.Data, 32);
      return FlatCcsBase;

    case GtFlatCcsReg:
      GtFlatCcsLowBase.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_LOWER_I_GCD_GT_REG, IpWrRegFlagSize32Bits);
      //
      // Clear the lower bits
      //
      GtFlatCcsLowBase.Bits.ccsenable = FALSE;
      GtFlatCcsLowBase.Bits.rsvd_0    = 0;

      GtFlatCcsHighBase.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, FLATCCSBASEANDRANGE_UPPER_I_GCD_GT_REG, IpWrRegFlagSize32Bits);
      //
      // Clear the upper bits
      //
      GtFlatCcsHighBase.Bits.flatccsize = 0;
      GtFlatCcsHighBase.Bits.rsvd_0     = 0;
      GtFlatCcsHighBase.Bits.rsvd_1     = 0;

      FlatCcsBase  = (UINT64)GtFlatCcsLowBase.Data;
      FlatCcsBase += IpWrLShiftU64 ((UINT64)GtFlatCcsHighBase.Data, 32);
      return FlatCcsBase;

    case GtFlatCcsGamReg:
      GtFlatCcsGamCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, FLAT_CCS_BASE_ADDR_GAMREQ_3D_GT_REG, IpWrRegFlagSize32Bits);
      return GtFlatCcsGamCfg.Bits.flat_ccs_en;

    case MediaFlatCcsGamReg:
      MediaFlatCcsGamCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, FLAT_CCS_BASE_ADDR_GAMREQ_MEDIA_MEDIA_REG, IpWrRegFlagSize32Bits);
      return MediaFlatCcsGamCfg.Bits.flat_ccs_en;

    case MediaRc6CtxMemReg:
      if (DataRequested == MediaRc6CtxMemLock) {
        MediaRc6Ctx.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_MEDIA_0_MEDIA_REG, IpWrRegFlagSize32Bits);
        return (BOOLEAN)MediaRc6Ctx.Bits.rc6memlock;
      } else {
        MediaRc6Ctx.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, RC6CTXBASE_RPM_MEDIA_0_MEDIA_REG, IpWrRegFlagSize32Bits);
        return MediaRc6Ctx.Bits.rc6membase << RC6CTXBASE_RPM_MEDIA_0_MEDIA_RC6MEMBASE_LSB;
      }

    case DispDsmBaseReg:
      DispDsmCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, DEDSMBASE_0_DISP_REG, IpWrRegFlagSize32Bits);
      return (DispDsmCfg.Bits.bdsm << DEDSMBASE_0_DISP_BDSM_LSB);

    case DispGsmBaseReg:
      DispGsmCfg.Data = (UINT32)IpWrRegRead (pInst->MmioAccess, DEGSMBASE_0_DISP_REG, IpWrRegFlagSize32Bits);
      return (DispGsmCfg.Bits.bgsm_lsb << DEGSMBASE_0_DISP_BGSM_LSB_LSB);

    default:
      break;
  }

  PRINT_LEVEL1 ("Invalid Register Requested\n");
  IpWrError (pInst->ErrorAssert, IpWrErrorMajorError, __LINE__);

  return MAX_UINT64;
}

/**
  Print Graphics PCI space in Debug log.
  @param[in] pInst   - A pointer to the IP instance to be used.
  @retval None
**/
VOID
IpIGpuPrintConfigSpace (
  IP_IGPU_INST  *pInst
  )
{
  UINT8  i;
  UINT8  j;

  if (IpIGpuSupported (pInst) == FALSE) {
    PRINT_LEVEL1 ("Returning from %s since IGPU is not present\n", __FUNCTION__);
    return;
  }

  PRINT_LEVEL1 ("\nPrinting PCI space for device 2\n  ");
  for (i = 0; i <= 0xF; i++) {
    PRINT_LEVEL1 ("  %2X", i);
  }

  for (i = 0; i <= 0xF; i++) {
    PRINT_LEVEL1 ("\n%2X", (i * 0x10));
    for (j = 0; j <= 0xF; j++) {
      PRINT_LEVEL1 ("  %2X", (UINT8)IpWrRegRead (pInst->PcieCfgAccess, ((i * 0x10) + j), IpWrRegFlagSize8Bits));
    }
  }

  PRINT_LEVEL1 ("\n");
}

/**
  This function will clear command register of 0:2:0

  @param[in] pInst   - A pointer to the IP instance to be used.

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIGpuClearCmdReg (
  IP_IGPU_INST  *pInst
  )
{
  PCICMD_IGPU_STRUCT  PciCmd;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return IpCsiStsErrorNullPtr;
  }

  PciCmd.Data     = (UINT16)IpWrRegRead (pInst->PcieCfgAccess, PCICMD_IGPU_REG, IpWrRegFlagSize16Bits);
  PciCmd.Bits.bme = FALSE;
  PciCmd.Bits.mae = FALSE;
  IpWrRegWrite (pInst->PcieCfgAccess, PCICMD_IGPU_REG, PciCmd.Data, IpWrRegFlagSize16Bits);

  return IpCsiStsSuccess;
}

/**
  Print IGPU Register Info.

  @param[in] pInst      - A pointer to the IP instance to be used.
  @param[in] String     - IGPU Register String name if provided.
  @param[in] Register   - IGPU Register offset.
  @param[in] Data       - IGPU Register Value which is written.
  @param[in] Value      - IGPU Register Data after write.
**/
VOID
IpIGpuPrintRegData (
  IP_IGPU_INST  *pInst,
  CHAR8         *String,
  UINT32        Register,
  UINT64        Value,
  UINT64        Data
  )
{
  if (String != NULL) {
    PRINT_LEVEL1 ("IGPU %s Offset = 0x%x Value = 0x%016lX Data = 0x%016lX\n", String, Register, Value, Data);
  } else {
    PRINT_LEVEL1 ("IGPU Offset = 0x%x Value = 0x%016lX Data = 0x%016lX\n", Register, Value, Data);
  }
}

/**
Allocate GSM2 Base Address and Size

@param[in]  pInst  A pointer to the IP instance to be used.
@param[in]  UINT64 Base Address
@param[in]  UINT64 Size
**/
VOID
IpIGpuGsm2Allocation (
  IP_IGPU_INST  *pInst,
  UINT64        Gsm2BaseAddress,
  UINT8         Gsm2Size
  )
{
  B2D_SCRATCH0_STRUCT  Gsm2Struct;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    PRINT_WARNING ("Invalid parameters for %s\n", __FUNCTION__);
    return;
  }

  Gsm2Struct.Data = 0;

  if (IpIGpuSupported (pInst) == FALSE) {
    PRINT_LEVEL1 ("Returning from %s since IGPU is not present\n", __FUNCTION__);
    return;
  }

  if (Gsm2BaseAddress != 0) {
    Gsm2Struct.Bits.Gsm2BaseAddress = (UINT32)IpWrDivU64x32 (Gsm2BaseAddress, BASE_32MB);
  }

  Gsm2Struct.Bits.Gsm2Size = Gsm2Size;
  PRINT_LEVEL1 ("GSM2 Data = 0x%lx\n", Gsm2Struct.Data);

  IpWrRegWrite (pInst->MmioAccess, B2D_SCRATCH0_REG, Gsm2Struct.Data, IpWrRegFlagSize32Bits);
}

/**
  This function will enable IO Bar on 0:2:0

  @param[in] pInst   - A pointer to the IP instance to be used.
  @retval None
**/
VOID
IpIGpuEnableIoCmdReg (
  IP_IGPU_INST  *pInst
  )
{
  PCICMD_IGPU_STRUCT  PciCmd;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return;
  }

  PciCmd.Data      = (UINT16)IpWrRegRead (pInst->PcieCfgAccess, PCICMD_IGPU_REG, IpWrRegFlagSize16Bits);
  PciCmd.Bits.ioae = TRUE;
  IpWrRegWrite (pInst->PcieCfgAccess, PCICMD_IGPU_REG, PciCmd.Data, IpWrRegFlagSize16Bits);
}

/**
  This function will disable IO Bar on 0:2:0

  @param[in] pInst   - A pointer to the IP instance to be used.
  @retval None
**/
VOID
IpIGpuDisableIoCmdReg (
  IP_IGPU_INST  *pInst
  )
{
  PCICMD_IGPU_STRUCT  PciCmd;

  if (IpIGpuIsInstValid (pInst) == FALSE) {
    return;
  }

  PciCmd.Data      = (UINT16)IpWrRegRead (pInst->PcieCfgAccess, PCICMD_IGPU_REG, IpWrRegFlagSize16Bits);
  PciCmd.Bits.ioae = FALSE;
  IpWrRegWrite (pInst->PcieCfgAccess, PCICMD_IGPU_REG, PciCmd.Data, IpWrRegFlagSize16Bits);
}
