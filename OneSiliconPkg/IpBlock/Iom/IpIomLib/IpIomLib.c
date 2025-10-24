/** @file
  The TCSS IOM Init Library

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

#include <IpCsi.h>
#include <IpWrapper.h>

#include <Registers/IomRegs.h>
#include <IpIom.h>
#include "IpIomPrivate.h"
#include <Library/TcssPmcLib.h>

/**
  IOM Mailbox data write

  @param[in] pInst              IOM IP instance structure
  @param[in] Command            Mailbox command
  @param[in] Data               Data to write
  @param[in] Timeout            Timeout for completion in microseconds

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIomMailboxWrite (
  IP_IOM_INST  *pInst,
  UINT32       Command,
  UINT32       Data,
  UINT32       Timeout
  )
{
  IP_CSI_STATUS  Status;
  UINT32         Cmd;

  Status = IpCsiStsSuccess;

  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_BIOS_MAILBOX_DATA, Data, IpWrRegFlagSize32Bits);
  IOM_PRINT_INFO ("IOM mailbox data = 0x%08X\n", Data);

  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_BIOS_MAILBOX_CMD, Command, IpWrRegFlagSize32Bits);
  IOM_PRINT_INFO ("IOM mailbox command = 0x%08X\n", Command);

  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_BIOS_MAILBOX_CMD, (Command | B_IOM_PCR_IOM_BIOS_MAILBOX_CMD_RUN_BUSY), IpWrRegFlagSize32Bits);

  if (Timeout) {
    // Time out is 0.1sec
    IpWrRegPollUs (
      pInst->RegCntxtIomSb,
      R_IOM_PCR_IOM_BIOS_MAILBOX_CMD,
      IpWrRegFlagSize32Bits,
      B_IOM_PCR_IOM_BIOS_MAILBOX_CMD_RUN_BUSY,
      0,
      Timeout,
      pInst->TimeCntxt,
      1,
      &Status
      );

    if (Status == IpCsiStsErrorTimeout) {
      IOM_PRINT_ERROR ("Execute Mailbox command timeout!\n");
    }

    Cmd = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_BIOS_MAILBOX_CMD, IpWrRegFlagSize32Bits);
    if (Cmd != 0) {
      IOM_PRINT_ERROR ("IOM response fail. Error code 0x%X\n", Cmd);
    }
  }

  return Status;
}

/**
  IpIomReadyCheck - This function performs IOM ready state

  This function does the following steps:
  (1) Examines IOM_READY (Register IOM_TYPEC_STATUS_1, bit 30)
  (2) If 0, retry in IOM_READY_TIMEOUT ms (1ms timeout)
  (3) Print success/failure

  @param[in] pInst    IOM IP instance structure

  @retval     IpCsiStsSuccess if IOM is ready
  @retval     IpCsiStsErrorTimeout if IOM is NOT ready
**/
IP_CSI_STATUS
IpIomReadyCheck (
  IP_IOM_INST  *pInst
  )
{
  IP_CSI_STATUS  Status;

  IOM_PRINT_INFO ("%a: Enter\n", __FUNCTION__);

  IpWrRegPollUs (
    pInst->RegCntxtIomSb,
    R_IOM_PCR_IOM_TYPEC_STATUS_1,
    IpWrRegFlagSize32Bits,
    B_IOM_PCR_IOM_TYPEC_STATUS_1_IOM_READY,
    B_IOM_PCR_IOM_TYPEC_STATUS_1_IOM_READY,
    5,
    pInst->TimeCntxt,
    200,
    &Status
    );

  return Status;
}

/**
  Limit TCSS C-State option.
  This programming shall be executed one-time during boot exit (Cold/warm/Sx).

  This function does the following steps:
  (1) BIOS will send desired C-State value (legal values are 0,1,2,4,5,6,7,10) via
      IOM_BIOS_MAILBOX_DATA register and set the RUN_BUSY bit.
  (2) IOMFW will consume this BIOS mailbox request,
      and update the IOM_BIOS_MAILBOX_DATA register with resolved deepest TCx
      state that can be supported.
  (3) IOMFW will clear the RUN_BUSY bit after step 2.

  @param[in] pInst    IOM IP instance structure
  @param[out] CLimit  TC C-State limit number written.

  @retval     IpCsiStsSuccess - TC C-State set correctly
  @retval     !IpCsiStsSuccess - Error case

**/
IP_CSI_STATUS
IpIomTcssCstatelimit (
  IP_IOM_INST  *pInst,
  UINT8        *CLimit
  )
{
  IP_CSI_STATUS  Status;
  UINT32         MailBoxData;
  UINT32         LegalCstateValue[] = { 0, 1, 2, 4, 5, 6, 7, 10 };
  BOOLEAN        LegalCstateFlag;
  UINT8          i;

  IOM_PRINT_INFO ("%a: Enter\n", __FUNCTION__);

  //
  // Check if users provide a valid C-State value
  //
  LegalCstateFlag = FALSE;
  for (i = 0; i < (sizeof (LegalCstateValue) / sizeof (UINT32)); i++) {
    if (pInst->IomConfig.TcStateLimit == LegalCstateValue[i]) {
      LegalCstateFlag = TRUE;
      break;
    }
  }

  if (LegalCstateFlag == FALSE) {
    // Set to default TC10 if users provide an invalid C-State value
    pInst->IomConfig.TcStateLimit = 10;
    IOM_PRINT_WARNING ("WARNING: Invalid C-State value, set TcStateLimit to TC10\n");
  }

  //
  // Update C-State limit to IOM-BIOS MAILBOX data register
  //
  Status = IpIomMailboxWrite (pInst, V_IOM_BIOS_TCSTATE_LIMIT_CMD, pInst->IomConfig.TcStateLimit, 100000);
  if (Status != IpCsiStsSuccess) {
    return Status;
  }

  MailBoxData = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_BIOS_MAILBOX_DATA, IpWrRegFlagSize32Bits);
  //
  // If IOM replys any value greater than 8, that means TC10
  //
  if (MailBoxData >= 8) {
    IOM_PRINT_WARNING ("Override MailBoxData from %d to 10\n", MailBoxData);
    MailBoxData = 10;
  }

  IOM_PRINT_INFO ("[TCSS] System Supported TC State = 0x%08x\n", MailBoxData);

  *CLimit = (UINT8)MailBoxData;

  return IpCsiStsSuccess;
}

/**
  This function performs basic initialization IOM for AUX Layout in PEI phase after Policy produced at Post-Mem phase.

  For those GPIO pins used for DP Aux orientation control and enabled, BIOS is responsible to -
  1. Configure Pad Mode (PMode) to function# associated with IOM_GPP*_*
     The Pad Mode here could be various per PCH design.
  2. BIOS Provide the following information for the DP Aux orientation bias control for provide the GPIO VwIndex,
     Bit Position, and VW Index information to IOM FW.
     I. GPIO endpoint IOSF-SB port ID (Pch Community port ID)
     II. VW index and data bit position

  @param[in]  pInst       - IOM IP instance structure
  @param[in]  GpioPadNum  - GPIO Pad number
  @param[out] PchPortId   - GPIO Pad mapping GPCOM PortID number
  @param[out] Bits        - GPIO Pad mapping Bit position
  @param[out] VwIndex     - GPIO Pad mapping Virtual Wire Index number

**/
void
IpGpioIomAuxOriSetting (
  IP_IOM_INST  *pInst,
  UINT32       GpioPadNum,
  UINT16       *PchPortId,
  UINT8        *Bits,
  UINT8        *VwIndex
  )
{
  if (pInst->Callback.GetAuxOriGpioSettings != NULL) {
    pInst->Callback.GetAuxOriGpioSettings (GpioPadNum, PchPortId, Bits, VwIndex);
  }
}

/**
This function returns IOM_AUX_ISO_CTRL_0 address offset

  @param[in] Index         - offset

  @retval UINT16           - Regaddr

**/
UINT32
IpGetAuxIsoCtrlRegOffset (
  UINT32  Index
  )
{
  return (R_IOM_PCR_AUX_ISO_CTRL_0 + (Index*4));
}

/**
  Instance structure initialization for this version of IP
  For current version of IOM IP there is no requirements for this function.
  All data in Instance Structure is initialized by the caller.
  The function just returns success status.

  @param[in] pInst  IOM instance structure pointer

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIomInstInit (
  IP_IOM_INST  *pInst
  )
{
  IOM_API_ENTRY ();

  return IpCsiStsSuccess;
}

/**
  The function shall be the first API function called after IpIomInstInit.
  This is a good place to fix bad register defaults or work around bugs.
  For current version of IOM IP there is no requirements for this function.
  The function just returns success status.

  @param[in]  pInst IOM instance structure pointer

  @retval         IpCsiStsSuccess on success
  @retval         !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpIomIpInit (
  IP_IOM_INST  *pInst
  )
{
  IOM_API_ENTRY ();

  return IpCsiStsSuccess;
}

/**
  This interface will return a version value that corresponds to the VerId provided and
  is formatted per the CSI specification.
  - CSI Defined Interface

  @param[in]      pInst      A pointer to the IP instance to be used.
  @param[in]      VerId      An enum value defined in the CSI specification of type IP_CSI_VER_ID.

  @retval         0          The VerId value is not recognized and a version could not be determined.
  @retval         Non-Zero   A version value corresponding to the VerId provided, and formatted per the CSI.
**/
UINT32
IpIomGetVersion (
  IP_IOM_INST    *pInst,
  IP_CSI_VER_ID  VerId
  )
{
  IP_CSI_VER_ID  RetValue;

  IOM_API_ENTRY ();

  switch (VerId) {
    case (IpCsiVerIdHwIp):
      RetValue = VERSION_HW_IOM;
      break;
    case (IpCsiVerIdHwIpCustom):
      RetValue = VERSION_HW_CUSTOM_IOM;
      break;
    case (IpCsiVerIdIpFwIntf):
      RetValue = VERSION_IPFWINTF_IOM;
      break;
    case (IpCsiVerIdIpFwIntfExt):
      RetValue = VERSION_IPFWINTFEXT_IOM;
      break;
    case (IpCsiVerIdIpFw):
      RetValue = VERSION_FW_IOM;
      break;
    case (IpCsiVerIdCsi):
      RetValue = VERSION_CSI;
      break;
    default:
      RetValue = VERSION_UNKNOWN_IOM;
      break;
  }

  return RetValue;
}

/**
  This function initializes Aux and HSL Orientation registers in IOM.

  @param[in] pInst    IOM IP instance structure

  @retval     IpCsiStsSuccess - AuxHslOri set correctly
  @retval     !IpCsiStsSuccess - Error case
**/
IP_CSI_STATUS
IpIomAuxHslOriInit (
  IP_IOM_INST  *pInst
  )
{
  UINT32                         AuxOriReg;
  UINT32                         HslOriReg;
  UINT32                         Index;
  UINT8                          PadIndex;
  IP_IOM_AUX_ORI_BIAS_STRUCTURE  RegSetting;
  UINT32                         PadRegAddr[2];
  UINT32                         RegAddr;
  UINT16                         PchPid;
  UINT8                          Bits;
  UINT8                          VwIndex;

  IOM_PRINT_INFO ("%a: Enter\n", __FUNCTION__);
  IOM_API_ENTRY ();

  Bits    = 0;
  VwIndex = 0;
  PchPid  = 0;

  for (Index = 0; Index < pInst->IomConfig.AuxBiasCount; Index++) {
    //
    // Get SBU1 and SBU2 pins
    //
    PadRegAddr[0]       = pInst->IomConfig.IomAuxPortPad[Index].ReceptacleSbu1BiasCtrl;
    PadRegAddr[1]       = pInst->IomConfig.IomAuxPortPad[Index].ReceptacleSbu2BiasCtrl;
    RegSetting.RegValue = 0;

    //
    // GPIO setting is NULL go to check next port config
    //
    if ((PadRegAddr[0] == 0) || (PadRegAddr[1] == 0)) {
      continue;
    }

    ///
    /// SBU1 and SBU2 loop start
    ///
    for (PadIndex = 0; PadIndex < 2; PadIndex++) {
      IpGpioIomAuxOriSetting (pInst, PadRegAddr[PadIndex], &PchPid, &Bits, &VwIndex);
      if ((PchPid == 0) && (VwIndex == 0)) {
        //
        // The Data is invalidate, skip the setting.
        //
        break;
      }

      RegSetting.Bits.Bit        = Bits;
      RegSetting.Bits.VwIndex    = VwIndex;
      RegSetting.Bits.GpioPortId = PchPid;

      //
      // Get correspond SBUx BIAS CTRL offset
      //
      if (PadIndex == 0) {
        RegAddr = R_IOM_PCR_RECEPTACLE_SBU1_BIAS_CTL_0 + (Index * 4);  // Up Pin
      } else {
        RegAddr = R_IOM_PCR_RECEPTACLE_SBU2_BIAS_CTL_0 + (Index * 4);  // Down Pin
      }

      //
      // Set register
      //
      IpWrRegWrite (pInst->RegCntxtIomSb, RegAddr, RegSetting.RegValue, IpWrRegFlagSize32Bits);
      IOM_PRINT_INFO ("IpGpioIomAuxOriSetting: Regaddr %08x, RegValue %08x\n", RegAddr, RegSetting.RegValue);
    }

    if (pInst->IomConfig.IomAuxPortPad[Index].AuxIsoCtrl != 0) {
      PchPid  = 0;
      VwIndex = 0;
      IpGpioIomAuxOriSetting (pInst, pInst->IomConfig.IomAuxPortPad[Index].AuxIsoCtrl, &PchPid, &Bits, &VwIndex);
      if ((PchPid == 0) && (VwIndex == 0)) {
        //
        // The Data is invalidate, skip the setting.
        //
        continue;
      }

      RegSetting.Bits.Bit        = Bits;
      RegSetting.Bits.VwIndex    = VwIndex;
      RegSetting.Bits.GpioPortId = PchPid;
      RegAddr                    = IpGetAuxIsoCtrlRegOffset (Index);
      IpWrRegWrite (pInst->RegCntxtIomSb, RegAddr, RegSetting.RegValue, IpWrRegFlagSize32Bits);

      IOM_PRINT_INFO ("R_IOM_AUX_ISO_CTRL Setting: Regaddr %08x, RegValue %08x\n", RegAddr, RegSetting.RegValue);
    }
  }

  ///
  /// Set IOM Aux Orientation per configuration.
  ///
  AuxOriReg = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, IpWrRegFlagSize32Bits);
  IOM_PRINT_INFO ("Original AuxOriReg: 0x%08X\n", AuxOriReg);
  IOM_PRINT_INFO ("IomConfig->IomOverrides.AuxOri: 0x%08X\n", (UINT32)pInst->IomConfig.AuxOriOverride);
  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, (UINT32)pInst->IomConfig.AuxOriOverride | AuxOriReg, IpWrRegFlagSize32Bits);
  IOM_PRINT_INFO ("Update AuxOri: 0x%08X\n", IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, IpWrRegFlagSize32Bits));

  ///
  /// Set IOM HSL Orientation per configuration.
  ///
  HslOriReg = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4, IpWrRegFlagSize32Bits);
  IOM_PRINT_INFO ("Original HslOriReg: 0x%08X\n", HslOriReg);
  IOM_PRINT_INFO ("IomConfig->IomOverrides.HslOri: 0x%08X\n", (UINT32)pInst->IomConfig.HslOriOverride);
  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4, (UINT32)pInst->IomConfig.HslOriOverride, IpWrRegFlagSize32Bits);
  IOM_PRINT_INFO ("Update HslOri: 0x%08X\n", IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4, IpWrRegFlagSize32Bits));
  IOM_PRINT_INFO ("[TCSS] IpIomAuxHslOriInit () - End\n");

  return IpCsiStsSuccess;
}

/**
  This function programs fixed connection for TCSS ports.

  @param[in]    pInst       IOM instance structure pointer

  @retval   IP_CSI_STATUS
  - IpCsiStsSuccess:       programming completed without error
  - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomFixedConnectionPortInit (
  IP_IOM_INST  *pInst
  )
{
  UINT32  TcssDpType;
  UINT8   PortIndex;
  UINT8   DpType;

  IOM_API_ENTRY ();

  if (pInst->TcssVersion == TcssOld) {
    TcssDpType = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_CONFIGURATION_1, IpWrRegFlagSize32Bits);

    for (PortIndex = 0; PortIndex < FIX_CONNECTION_PORTS_MAX; PortIndex++) {
      DpType = pInst->IomConfig.FixConPort[PortIndex] & B_IOM_PCR_IOM_TYPEC_CONFIGURATION_1_FIXED_CONNECTION_PORT1;
      if (DpType != 0) {
        if ((DpType >= IOM_DP) && (DpType <= IOM_EDP)) {
          TcssDpType &= ~((UINT32)B_IOM_PCR_IOM_TYPEC_CONFIGURATION_1_FIXED_CONNECTION_PORT1 << (PortIndex * N_IOM_PCR_IOM_TYPEC_CONFIGURATION_1_FIXED_CONNECTION_PORT2));
          TcssDpType |= (DpType << (PortIndex * 4));
        } else {
          IOM_PRINT_WARNING ("TCSS USBC Port %d has unknown DP type 0x%x\n", PortIndex, DpType);
        }
      }
    }

    IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_CONFIGURATION_1, TcssDpType, IpWrRegFlagSize32Bits);
    IOM_PRINT_INFO ("Setting IOM_TYPEC_CONFIGURATION_1 to 0x%08x\n", TcssDpType);
  }

  return IpCsiStsSuccess;
}

/**
  Initializes TCSS Device Enables in IOM
  Note:
      Device Enables argument passed to this function
      MUST be in-sync with SOC's IO Cache service.

  @param[in]    pInst       IOM instance structure pointer

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomDevEnInit (
  IP_IOM_INST  *pInst
  )
{
  IOM_API_ENTRY ();

  //
  // IOM controller isn't out of reset at this stage
  // so set timeout to 0 (means don't wait for response)
  //
  return IpIomMailboxWrite (pInst, V_IOM_BIOS_UPDATE_DEVEN_CMD, pInst->IomConfig.DevEn.TcssDevEn, 0);
}

/**
  Enables Compatibility Revision ID in IOM

  @param[in]    pInst       IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomCrIdProgram (
  IP_IOM_INST  *pInst
  )
{
  UINT32  IomTypecConfigValue;

  IOM_API_ENTRY ();

  IomTypecConfigValue  = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IpWrRegFlagSize32Bits);
  IomTypecConfigValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_CRID_EN;
  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IomTypecConfigValue, IpWrRegFlagSize32Bits);

  return IpCsiStsSuccess;
}

/**
  Initializas IOM at Post-Mem phase
  - Program VccSt
  - Program D3ColdEnable
  - Program C State Limit
  - Program Aux and HSL Orientation
  Note:
        Device Enables argument passed to this function
        MUST be in-sync with SOC's IO Cache service.

  @param[in]    pInst       IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:          programming completed without error
    - IpCsiStsError:            IOM programming error
    - IpCsiStsErrorNotAllowed:  xHCI disabled- abort the init
**/
IP_CSI_STATUS
IpIomInitPostMemory (
  IP_IOM_INST  *pInst
  )
{
  IP_CSI_STATUS  IomReadyStatus;
  UINT32         IomTypecConfigValue;
  UINT8          CLimit;
  UINT32         PmcTcssInitStatus;

  IOM_API_ENTRY ();

  ///
  /// Step 1 - Perform IOM ready check
  ///
  IomReadyStatus = IpIomReadyCheck (pInst);
  if (IomReadyStatus != IpCsiStsSuccess) {
    //
    // IOM is corrupted
    //
    IOM_PRINT_ERROR ("\tIomReadyCheck Failed!\n");
    if (pInst->Callback.SetTcssDevenRegisterMchbar != NULL) {
      pInst->Callback.SetTcssDevenRegisterMchbar (0);
    }

    IpIomConfigurationLock (pInst);
    return IpCsiStsErrorNotAllowed;
  }

  if (!pInst->IomConfig.DevEn.TcssDevEnBit.TcssXhciEn) {
    if (pInst->Callback.PmcTcssBiosInitDone != NULL) {
      PmcTcssInitStatus = (UINT32)pInst->Callback.PmcTcssBiosInitDone (IomReadyStatus, 0);
      IOM_PRINT_INFO ("[TCSS] PmcTcssInitStatus = 0x%08X\n", PmcTcssInitStatus);
    }

    IOM_PRINT_ERROR ("[TCSS] CPU XHCI is disabled, skip whole TCSS initialization!\n");
    //
    // Request TC cold for TCSS disabling case
    //
    //
    // Enable D3 Cold based on policy.
    //
    if (pInst->IomConfig.D3ColdEnable == 1) {
      //
      // SET BIT 9 of IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1
      //
      IOM_PRINT_ERROR ("[TCSS] IOM D3Cold == 1, So enabling IOM D3Cold for IomTypecConfigValue\n");
      IomTypecConfigValue  = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IpWrRegFlagSize32Bits);
      IomTypecConfigValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_D3_COLD_REQ | B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_D3_COLD_EN;
      IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IomTypecConfigValue, IpWrRegFlagSize32Bits);
    }

    //
    // IOM Software configure registers lock when TCSS XHCI is disable.
    //
    IpIomConfigurationLock (pInst);
    return IpCsiStsErrorNotAllowed;
  }

  ///
  /// Step 2 - If IOM is ready, then programs VCCST, REPLAY and CONNECTION_OVERRIDE setting
  ///
  if (IomReadyStatus == IpCsiStsSuccess) {
    //
    // Read IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1
    //
    IomTypecConfigValue = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IpWrRegFlagSize32Bits);

    //
    // CLEAR BITS of IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1
    //
    IOM_PRINT_INFO ("[TCSS] Clearing all IomTypecConfigValue as default initialization\n");
    IomTypecConfigValue &= ~(B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_EC_REPLAY_CONNECTION_S4
                             | B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_EC_REPLAY_CONNECTION_S3
                             | B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_D3_COLD_EN);

    if (pInst->IomConfig.VccSt == 0) {
      //
      // SET BIT 13 of IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1
      //
      IomTypecConfigValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_EC_REPLAY_CONNECTION_S4;
    }

    //
    // Enable D3 Cold based on policy.
    //
    if (pInst->IomConfig.D3ColdEnable == 1) {
      //
      // SET BIT 9 of IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1
      //
      IOM_PRINT_INFO ("[TCSS] IOM D3Cold == 1, So enabling IOM D3Cold for IomTypecConfigValue\n");
      IomTypecConfigValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_D3_COLD_EN;
    }

    //
    // Write to IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1
    //
    IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IomTypecConfigValue, IpWrRegFlagSize32Bits);

    //
    // Set Tcss Cstate limit
    //
    IpIomTcssCstatelimit (pInst, &CLimit);
  }

  //
  // Step 3 - Update IOM_TYPEC_SW_CONFIGURATION_3 Reg, IGD policy.
  //
  //
  // Enable Display support based on policy.
  //
  if (pInst->IomConfig.TcNotifyIgd == 1) {
    //
    // SET BIT 12 of IOM_TYPEC_SW_CONFIGURATION_3
    //
    IomTypecConfigValue  = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, IpWrRegFlagSize32Bits);
    IomTypecConfigValue |= V_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3_WAKE_IGFX_FROM_D3_EN;
    IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, IomTypecConfigValue, IpWrRegFlagSize32Bits);
    IOM_PRINT_INFO ("[TCSS] IOM TcNotifyIgd == 1, So enabling IOM TcNotifyIgd for IomTypecConfigValue\n");
    IOM_PRINT_INFO ("Update IOM_TYPEC_SW_CONFIGURATION_3: 0x%08X\n", IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, IpWrRegFlagSize32Bits));
  }

  return IpCsiStsSuccess;
}

/**
  IOM configuration lock procedure

  @param[in]    pInst       IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         IOM programming error
**/
IP_CSI_STATUS
IpIomConfigurationLock (
  IP_IOM_INST  *pInst
  )
{
  UINT32  RegValue;

  IOM_API_ENTRY ();

  //
  // Lock R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, R_IOM_PCR_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3,
  // R_IOM_PCR_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4 IOM registers.
  //
  RegValue  = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IpWrRegFlagSize32Bits);
  RegValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_LOCK;
  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, RegValue, IpWrRegFlagSize32Bits);

  RegValue  = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, IpWrRegFlagSize32Bits);
  RegValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3_LOCK;
  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, RegValue, IpWrRegFlagSize32Bits);

  RegValue  = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4, IpWrRegFlagSize32Bits);
  RegValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4_LOCK;
  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4, RegValue, IpWrRegFlagSize32Bits);

  return IpCsiStsSuccess;
}

/**
  Returns IOM Isolated Memory Region MG status

  @param[in]    pInst       IOM instance structure
  @param[out]   Status      Pointer to status variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:                    Executed successfully
    - IpCsiStsNotReady:                   MG FW not downloaded to IMR
    - IpCsiStsErrorInsufficientResources: Untrusted MG FW detected
    - IpCsiStsErrorNullPtr:               Status Pointer not set
**/
IP_CSI_STATUS
IpIomGetImrMgStatus (
  IP_IOM_INST  *pInst,
  UINT32       *Status
  )
{
  IOM_MG_IMR_STATUS  MgImrStatus;

  IOM_API_ENTRY ();

  if (Status == NULL) {
    IOM_PRINT_ERROR ("Error: IOM IMR MG Status Pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  //
  // Read MG IMR status
  //
  MgImrStatus.RegValue = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_CSME_IMR_MG_STATUS, IpWrRegFlagSize32Bits);
  *Status              = MgImrStatus.RegValue;
  IOM_PRINT_INFO ("R_IOM_PCR_IOM_CSME_IMR_MG_STATUS: 0x%08X\n", MgImrStatus.RegValue);
  IOM_PRINT_INFO ("MG FW version: 0x%04X\n", (UINT16)MgImrStatus.Bits.MgFwVersion);

  if (MgImrStatus.Bits.Done) {
    if (!MgImrStatus.Bits.Valid) {
      IOM_PRINT_ERROR ("Untrusted MG FW detected!\n");
      return IpCsiStsErrorInsufficientResources;
    }
  } else {
    IOM_PRINT_ERROR ("MG FW has not yet downloaded to IMR!\n");
    if (pInst->IomConfig.Environment != HfpgaEnv) {
      return IpCsiStsNotReady;
    }
  }

  return IpCsiStsSuccess;
}

/**
  Returns IOM Isolated Memory Region TBT status

  @param[in]    pInst       IOM instance structure
  @param[out]   Status      Pointer to status variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:                    Executed successfully
    - IpCsiStsNotReady:                   TBT FW not downloaded to IMR
    - IpCsiStsErrorInsufficientResources: Untrusted TBT FW detected
    - IpCsiStsErrorNullPtr:               Status Pointer not set
**/
IP_CSI_STATUS
IpIomGetImrTbtStatus (
  IP_IOM_INST  *pInst,
  UINT32       *Status
  )
{
  IOM_MG_IMR_STATUS  MgImrStatus;

  IOM_API_ENTRY ();

  if (Status == NULL) {
    IOM_PRINT_ERROR ("Error: IOM IMR Status pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  //
  // Read MG IMR status
  //
  MgImrStatus.RegValue = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_CSME_IMR_TBT_STATUS, IpWrRegFlagSize32Bits);
  *Status              = MgImrStatus.RegValue;
  IOM_PRINT_INFO ("R_IOM_PCR_IOM_CSME_IMR_TBT_STATUS: 0x%08X\n", MgImrStatus.RegValue);
  IOM_PRINT_INFO ("TBT FW version: 0x%04X\n", (UINT16)MgImrStatus.Bits.MgFwVersion);

  if (MgImrStatus.Bits.Done) {
    if (!MgImrStatus.Bits.Valid) {
      IOM_PRINT_ERROR ("Untrusted TBT FW detected!\n");
      return IpCsiStsErrorInsufficientResources;
    }
  } else {
    IOM_PRINT_ERROR ("TBT FW has not yet downloaded to IMR!\n");
    return IpCsiStsNotReady;
  }

  return IpCsiStsSuccess;
}

/**
  Returns IOM Deep St TypeC State

  @param[in]    pInst       IOM instance structure
  @param[out]   State       Pointer to state variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  Status Pointer not set
**/
IP_CSI_STATUS
IpIomGetDeepStTcState (
  IP_IOM_INST  *pInst,
  UINT32       *State
  )
{
  IP_CSI_STATUS  Status;
  UINT8          CLimit;

  IOM_API_ENTRY ();

  if (State == NULL) {
    IOM_PRINT_ERROR ("Error: IOM Deep State Pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  Status = IpIomTcssCstatelimit (pInst, &CLimit);
  if (Status != IpCsiStsSuccess) {
    IOM_PRINT_ERROR ("Cannot read IOM Deep St TypeC State!\n");
    return Status;
  }

  *State = (UINT32)CLimit;

  return IpCsiStsSuccess;
}

/**
  This function gets TCSS SoC product information.

  @param[in]    pInst             IOM instance structure
  @param[out]   SkuId             TCSS SoC Sku ID pointer
  @param[out]   BaseSteppingId    TCSS SoC Base stepping ID pointer
  @param[out]   DashSteppingId    TCSS SoC Dash stepping ID pointer

  @retval       IP_CSI_STATUS
                - IpCsiStsSuccess:       Executed successfully
                - IpCsiStsErrorNullPtr:  SkuId Pointer not set
                - IpCsiStsErrorNullPtr:  BaseSteppingId Pointer not set
                - IpCsiStsErrorNullPtr:  DashSteppingId Pointer not set

**/
IP_CSI_STATUS
IpIomGetTcssProductInfo (
  IP_IOM_INST  *pInst,
  UINT8        *SkuId,
  UINT8        *BaseSteppingId,
  UINT8        *DashSteppingId
  )
{
  UINT32  ProductInfo;

  IOM_API_ENTRY ();

  if (SkuId == NULL) {
    IOM_PRINT_ERROR ("Error: IOM SkuId Pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  if (BaseSteppingId == NULL) {
    IOM_PRINT_ERROR ("Error: IOM BaseSteppingId Pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  if (DashSteppingId == NULL) {
    IOM_PRINT_ERROR ("Error: IOM DashSteppingId Pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  ProductInfo     = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_PRODUCT_INFO, IpWrRegFlagSize32Bits);
  *SkuId          = (ProductInfo >> N_IOM_PCR_PRODUCT_INFO_SKU_ID)           & 0x0000000F;
  *BaseSteppingId = (ProductInfo >> N_IOM_PCR_PRODUCT_INFO_BASE_STEPPING_ID) & 0x0000000F;
  *DashSteppingId = (ProductInfo >> N_IOM_PCR_PRODUCT_INFO_DASH_STEPPING_ID) & 0x0000000F;

  return IpCsiStsSuccess;
}

/**
  Checks if IOM controller is ready to use

  @param[in]    pInst       IOM instance structure
  @param[out]   Ready       Pointer to ready variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  Ready Pointer not set
**/
IP_CSI_STATUS
IpIomIsReady (
  IP_IOM_INST  *pInst,
  UINT32       *Ready
  )
{
  IP_CSI_STATUS  Status;

  IOM_API_ENTRY ();

  if (Ready == NULL) {
    IOM_PRINT_ERROR ("Error: IOM Ready Pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  Status = IpIomReadyCheck (pInst);
  if (Status == IpCsiStsSuccess) {
    *Ready = 1;
  } else {
    *Ready = 0;
  }

  return Status;
}

/**
  Returns IOM firmware version

  @param[in]    pInst       IOM instance structure
  @param[out]   Version     Pointer to version variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsErrorTimeout   if IOM is NOT ready
    - IpCsiStsErrorNullPtr:  Version Pointer not set
**/
IP_CSI_STATUS
IpIomGetFwVersion (
  IP_IOM_INST  *pInst,
  UINT32       *Version
  )
{
  IP_CSI_STATUS  Status;

  IOM_API_ENTRY ();

  if (Version == NULL) {
    IOM_PRINT_ERROR ("Error: IOM FW Version Pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  Status = IpIomReadyCheck (pInst);
  if (Status == IpCsiStsSuccess) {
    *Version = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_FW_INFO, IpWrRegFlagSize32Bits);
  } else {
    *Version = 0xFFFFFFFF;
  }

  return Status;
}

/**
  Returns IOM Engineering firmware version

  @param[in]    pInst       IOM instance structure
  @param[out]   Version     Pointer to version variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:      Executed successfully
    - IpCsiStsErrorTimeout  if IOM is NOT ready
    - IpCsiStsErrorNullPtr  Version Pointer not set
**/
IP_CSI_STATUS
IpIomGetFwEngrVersion (
  IP_IOM_INST  *pInst,
  UINT32       *Version
  )
{
  IP_CSI_STATUS  Status;

  IOM_API_ENTRY ();

  if (Version == NULL) {
    IOM_PRINT_ERROR ("Error: IOM Eng FW Version Pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  Status = IpIomReadyCheck (pInst);
  if (Status == IpCsiStsSuccess) {
    *Version = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_FW_INFO_ENGR, IpWrRegFlagSize32Bits);
  } else {
    *Version = 0xFFFFFFFF;
  }

  return Status;
}

/**
  Returns IOM configuration lock status

  @param[in]    pInst       IOM instance structure
  @param[out]   ConfigLock  Pointer to config structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM reading
    - IpCsiStsErrorNullPtr:  ConfigLock Pointer not set
**/
IP_CSI_STATUS
IpIomGetSwConfigs (
  IP_IOM_INST         *pInst,
  IP_IOM_CONFIG_LOCK  *ConfigLock
  )
{
  IOM_API_ENTRY ();

  if (ConfigLock == NULL) {
    IOM_PRINT_ERROR ("Error: IOM CONFIG LOCK pointer not set!\n");
    return IpCsiStsErrorNullPtr;
  }

  ConfigLock->Lock1 = !!((UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IpWrRegFlagSize32Bits) & B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_LOCK);
  ConfigLock->Lock3 = !!((UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3, IpWrRegFlagSize32Bits) & B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_3_LOCK);
  ConfigLock->Lock4 = !!((UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4, IpWrRegFlagSize32Bits) & B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_4_LOCK);
  return IpCsiStsSuccess;
}

/**
  Programs TCSS Devices Initialization Done

  @param[in]    pInst       IOM instance structure
  @param[out]   Done        TCSS Devices Init Done

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
    - IpCsiStsError:         Error during IOM programming
**/
IP_CSI_STATUS
IpIomSetDevInitDone (
  IP_IOM_INST    *pInst,
  DEV_INIT_DONE  Done
  )
{
  UINT32  RegValue;

  IOM_API_ENTRY ();

  RegValue = (UINT32)IpWrRegRead (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, IpWrRegFlagSize32Bits);
  if (Done.Dev.PcieRp) {
    RegValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_PCIE_INIT_DONE;
  }

  if (Done.Dev.Xhci) {
    RegValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_USB3_INIT_DONE;
  }

  if (Done.Dev.Usb4) {
    RegValue |= B_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1_USB4_HR_INIT_DONE;
  }

  IpWrRegWrite (pInst->RegCntxtIomSb, R_IOM_PCR_IOM_TYPEC_SW_CONFIGURATION_1, RegValue, IpWrRegFlagSize32Bits);
  return IpCsiStsSuccess;
}

/**
  Sets SVID, SSID and UUID in USB4HR

  @param[in]    pInst    IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
**/
IP_CSI_STATUS
IpIomSetUsb4Id (
  IP_IOM_INST  *pInst
  )
{
  return IpCsiStsSuccess;
}

/**
  Could enable RTD3 in USB4HR
  Lock IOM_USB4HR_MISC_CONFIG, IOM_USB4HR_SSID_SVID
  and IOM_USB4HR_UUID registers

  @param[in]    pInst        IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
**/
IP_CSI_STATUS
IpIomSetUsb4Rtd3 (
  IP_IOM_INST  *pInst
  )
{
  return IpCsiStsSuccess;
}

/**
  Enable and disable HTI in TCSS_DEBUG_CONFIG register
  Set TCSS_PORT_CONFIG registers according to TypeCPortCap.
  Set IOM_USB4HR_MISC_CONFIG.ENABLE80G_BIOS_PREFERENCEaccording to IOM_FUSE_REG1

  @param[in]    pInst        IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
**/
IP_CSI_STATUS
IpIomTcssSettings (
  IP_IOM_INST  *pInst
  )
{
  return IpCsiStsSuccess;
}

/**
  Enable eDP on TypeC port 0

  @param[in]    pInst        IOM instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfully
**/
IP_CSI_STATUS
IpIomEnableEdp (
  IP_IOM_INST   *pInst
  )
{
  return IpCsiStsSuccess;
}