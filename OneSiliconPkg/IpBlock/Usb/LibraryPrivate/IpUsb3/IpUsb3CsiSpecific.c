/** @file
  USB3 IP initialization library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <IpCsi.h>
#include <IpWrapperBaseTypes.h>

#include <IpUsb3.h>
#include <IpUsb3VscId.h>
#include <IpUsb3Private.h>
#include "IpUsb3Version.h"
#include <Register/UsbHostRegs.h>
#include <IpUsb3Defines.h>

/**
  Get version information

  @param[in]      pInst the instance
  @param[in]      VerId version ID

  @retval         0 is returned upon any error or unavailable version ID
  @retval         return value define by VerId
**/
UINT32
IpUsb3GetVersion (
  IP_USB3_INST   *pInst,
  IP_CSI_VER_ID  VerId
  )
{
  UINT32  Ret = 0;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return 0;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  switch (VerId) {
    case IpCsiVerIdCsi:
      Ret = VERSION_CSI;
      break;
    case IpCsiVerIdIpFw:
      Ret = VERSION_IPSD_UBS3;
      break;
    case IpCsiVerIdIpFwIntf:
      Ret = VERSION_IPSDINTF_USB3;
      break;
    default:
      break;
  }

  return Ret;
}

/**
  Get Control

  @param[in]      pInst       USB3 IP instance
  @param[in]      IpTarget    IP defined value for target within the IP (for USB3 only 0 is accepted)
  @param[in]      FeatureId   Feature ID
  @param[in,out]  pCsiSts     Optional may be NULL. If non-NULL return IP_CSI_STATUS
    IpCsiStsSuccess           completed without error
    IpCsiStsErrorUnknownId    unknown Feature ID was provided
    IpCsiStsErrorNullPtr      null pointer to IP Instance provided
    IpCsiStsErrorBadParam     IpTarget value provided is different than 0

  @retval         UINT32      defined by IP_USB3_FEATURE_ID
**/
UINT32
IpUsb3GetControl (
  IP_USB3_INST     *pInst,
  UINT32           IpTarget,
  IP_USB3_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  )
{
  UINT32   Data;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    if (pCsiSts != NULL) {
      *pCsiSts = IpCsiStsErrorNullPtr;
    }
    return 0;
  }

  if (IpTarget != 0) {
    PRINT_ERROR ("IpTarget other than 0 is not accepted\n");
    if (pCsiSts != NULL) {
      *pCsiSts = IpCsiStsErrorBadParam;
    }
    return 0;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  switch (FeatureId) {
    case IpUsb3FeatIdAudioOffload:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_GET_SET_EXTD_CAP_IDENTIFIER);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if ((Data & B_XHCI_MEM_GET_SET_EXTD_CAP_IDENTIFIER_UAOL) == B_XHCI_MEM_GET_SET_EXTD_CAP_IDENTIFIER_UAOL) {
        return IpUsb3FeatValAudioOffloadEn;
      } else {
        return IpUsb3FeatValAudioOffloadDis;
      }

    case IpUsb3FeatIdPortResetMessaging:
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      return IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_U2PRM_U2PRDE);

    case IpUsb3FeatIdResetPrepOverride:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_PM_CTRL_REG1);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }

      if ((Data & B_XHCI_MEM_PM_CTRL_REG1_OVERRIDE_DISABLE) == B_XHCI_MEM_PM_CTRL_REG1_OVERRIDE_DISABLE) {
        return IpUsb3FeatValResetPrepOverrideEn;
      } else {
        return IpUsb3FeatValResetPrepOverrideDis;
      }

    case IpUsb3FeatIdU2MappingToP3State:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_AUX_CTRL_REG2);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if ((Data & B_XHCI_MEM_AUX_CTRL_REG2_EN_U2_P3) == B_XHCI_MEM_AUX_CTRL_REG2_EN_U2_P3) {
        return IpUsb3FeatValU2MappingToP3StateEn;
      } else {
        return IpUsb3FeatValU2MappingToP3StateDis;
      }

    case IpUsb3FeatIdCreditRedeem:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_TRM_REG2);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if ((Data & B_XHCI_MEM_HOST_CTRL_TRM_REG2_CLR_CPCR) == B_XHCI_MEM_HOST_CTRL_TRM_REG2_CLR_CPCR) {
        return IpUsb3FeatValCreditRedeemEn;
      } else {
        return IpUsb3FeatValCreditRedeemDis;
      }

    case IpUsb3FeatIdBackToBackBTOHandlingEnable:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_D0I2_CTRL_REG);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if ((Data & B_XHCI_MEM_D0I2_CTRL_REG_B2B_BTO_EN) == B_XHCI_MEM_D0I2_CTRL_REG_B2B_BTO_EN) {
        return IpUsb3FeatValBackToBackBTOHandlingEnableEn;
      } else {
        return IpUsb3FeatValBackToBackBTOHandlingEnableDis;
      }

    case IpUsb3FeatIdHhhFlushFix:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if ((Data & B_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1_CFG_DIS_HHH_FLUSH_FIX) == B_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1_CFG_DIS_HHH_FLUSH_FIX) {
        return IpUsb3FeatValHhhFlushFixEn;
      } else {
        return IpUsb3FeatValHhhFlushFixDis;
      }

    case IpUsb3FeatIdB2bRetryFix:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if ((Data & B_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE_CHK_BIT_EN_B2B_RETRY_FIX) == B_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE_CHK_BIT_EN_B2B_RETRY_FIX) {
        return IpUsb3FeatValB2bRetryFixEn;
      } else {
        return IpUsb3FeatValB2bRetryFixDis;
      }

    case IpUsb3FeatIdUsb3ProgrammingLock:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      return !!(Data & B_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE_USB3_PROG_DONE);

    case IpUsb3FeatIdAccessControlLock:
      Data = IP_WR_READ_32 (pInst->RegCntxtPci, R_XHCI_CFG_XHCC1);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      return !!(Data & B_XHCI_CFG_XHCC1_ACCTRL);

    case IpUsb3FeatIdReadAfterWriteDelay:
      Data = IP_WR_READ_32 (pInst->RegCntxtPci, R_XHCI_CFG_XHCC2);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if (Data & B_XHCI_CFG_XHCC2_RAWDD) {
        return IpUsb3FeatValReadAfterWriteDelayDis;
      }
      return IpUsb3FeatValReadAfterWriteDelayEn;

    case IpUsb3FeatIdPowerControl:
      Data = IP_WR_READ_16 (pInst->RegCntxtPci, R_XHCI_CFG_PCE_REG);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if (Data & B_XHCI_CFG_PCE_REG_D3_HOT_EN) {
        return IpUsb3FeatValPowerControlEn;
      }
      return IpUsb3FeatValPowerControlDis;

    case IpUsb3FeatIdHSAsyncActiveExposure:
      Data = IP_WR_READ_32 (pInst->RegCntxtPci, R_XHCI_CFG_HSCFG1);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }

      if (Data & B_XHCI_CFG_HSCFG1_HSAAPE) {
        return IpUsb3FeatValHSAsyncActiveExposureEn;
      }
      return IpUsb3FeatValHSAsyncActiveExposureDis;
      break;

    case IpUsb3FeatIdSoftErrorTriggerRecalibration:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_RECAL_REG);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }

      if (Data & BIT31) {
        return IpUsb3FeatValSoftErrorTriggerRecalibrationEn;
      }
      return IpUsb3FeatValSoftErrorTriggerRecalibrationDis;
      break;

    case IpUsb3FeatIdDbcTraceInEp:
      Data = IP_WR_READ_32 (pInst->RegCntxtDciSbMmio, R_XHCI_PCR_PMREQDBC);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }

      if (Data & B_XHCI_PCR_PMREQDBC_DBCTRACEEPWAKE) {
        return IpUsb3FeatValDbcTraceInEpEn;
      }
      return IpUsb3FeatValDbcTraceInEpDis;
      break;

    case IpUsb3FeatIdDwbEnable:
      Data = IP_WR_READ_32 (pInst->RegCntxtMem, R_XHCI_MEM_TRB_PRF_CTRL_REG4);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsSuccess;
      }
      if ((Data & B_XHCI_MEM_TRB_PRF_CTRL_REG4_LTRNDEISOINEN_HS) &&
          (Data & B_XHCI_MEM_TRB_PRF_CTRL_REG4_LTRNDEISOINEN_ESS) &&
          (Data & B_XHCI_MEM_TRB_PRF_CTRL_REG4_PRFTCHHOLDEN) &&
          (((Data & B_XHCI_MEM_TRB_PRF_CTRL_REG4_DWBFLUSHT) >> N_XHCI_MEM_TRB_PRF_CTRL_REG4_DWBFLUSHT) == V_XHCI_MEM_TRB_PRF_CTRL_REG4_DWBFLUSHT)) {
        return IpUsb3FeatValDwbEnable;
      } else {
        return IpUsb3FeatValDwbDisable;
      }

    case IpUsb3FeatIdUnknown:
    default:
      PRINT_WARNING ("Invalid FeatureId provided to %s\n", __FUNCTION__);
      if (pCsiSts != NULL) {
        *pCsiSts = IpCsiStsErrorUnknownId;
      }
      return 0;
  }
}

/**
  Set Control. Based on provided FeatureId it controls that feature and programming
  that comes with this.

  @param[in]  pInst       USB3 IP instance
  @param[in]  IpTarget    IP defined value for target within the IP (for USB3 only 0 is accepted)
  @param[in]  FeatureId   Feature ID
  @param[in]  FeatureVal  Value based on FeatureId

  @retval     IpCsiStsSuccess          completed without error
  @retval     IpCsiStsErrorUnknownId   unknown Feature ID was provided
  @retval     IpCsiStsErrorNullPtr     null pointer to IP Instance provided
  @retval     IpCsiStsErrorBadParam    IpTarget value provided is different than 0 or unsupported FeatureVal
**/
IP_CSI_STATUS
IpUsb3SetControl (
  IP_USB3_INST     *pInst,
  UINT32           IpTarget,
  IP_USB3_FEAT_ID  FeatureId,
  UINT32           FeatureVal
  )
{
  UINT32   Data32And;
  UINT32   Data32Or;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  if (IpTarget != 0) {
    PRINT_ERROR ("IpTarget other than 0 is not accepted\n");
    return IpCsiStsErrorBadParam;
  }

  PRINT_LEVEL1 ("Entered %s - FeatureId %x, FeatureVal %x\n", __FUNCTION__, FeatureId, FeatureVal);

  switch (FeatureId) {
    case IpUsb3FeatIdAudioOffload:
      if (FeatureVal == IpUsb3FeatValAudioOffloadEn) {
        IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_GET_SET_EXTD_CAP_IDENTIFIER, B_XHCI_MEM_GET_SET_EXTD_CAP_IDENTIFIER_UAOL);

        IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                            R_XHCI_MEM_PWR_SCHED_CTRL0,
                            ~(B_XHCI_MEM_PWR_SCHED_CTRL0_BPSAW),
                            (pInst->FabricPllAdvanceWake << N_XHCI_MEM_PWR_SCHED_CTRL0_BPSAW) & B_XHCI_MEM_PWR_SCHED_CTRL0_BPSAW);
      } else if (FeatureVal == IpUsb3FeatValAudioOffloadDis) {
        IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_GET_SET_EXTD_CAP_IDENTIFIER, ~(B_XHCI_MEM_GET_SET_EXTD_CAP_IDENTIFIER_UAOL));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdPortResetMessaging:
      IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_U2PRM_U2PRDE, FeatureVal);
      break;

    case IpUsb3FeatIdResetPrepOverride:
      if (FeatureVal == IpUsb3FeatValResetPrepOverrideEn) {
        IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_PM_CTRL_REG1, B_XHCI_MEM_PM_CTRL_REG1_OVERRIDE_DISABLE);
      } else if (FeatureVal == IpUsb3FeatValResetPrepOverrideDis) {
        IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_PM_CTRL_REG1, ~(B_XHCI_MEM_PM_CTRL_REG1_OVERRIDE_DISABLE));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdUsb3ProgrammingLock:
      IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE, ~(B_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE_USB3_PROG_DONE));
      break;

    case IpUsb3FeatIdPowerControl:
      if (FeatureVal == IpUsb3FeatValPowerControlEn) {
        IP_WR_OR_16 (pInst->RegCntxtPci, R_XHCI_CFG_PCE_REG, B_XHCI_CFG_PCE_REG_D3_HOT_EN);
      } else if (FeatureVal == IpUsb3FeatValPowerControlDis) {
        IP_WR_AND_16 (pInst->RegCntxtPci, R_XHCI_CFG_PCE_REG, ~(B_XHCI_CFG_PCE_REG_D3_HOT_EN));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdAccessControlLock:
      IP_WR_AND_THEN_OR_32 (pInst->RegCntxtPci, R_XHCI_CFG_XHCC1, ~(B_XHCI_CFG_XHCC1_URD), B_XHCI_CFG_XHCC1_ACCTRL);
      break;

    case IpUsb3FeatIdU2MappingToP3State:
      if (FeatureVal == IpUsb3FeatValU2MappingToP3StateEn) {
        IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_AUX_CTRL_REG2, B_XHCI_MEM_AUX_CTRL_REG2_EN_U2_P3);
      } else if (FeatureVal == IpUsb3FeatValU2MappingToP3StateDis) {
        IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_AUX_CTRL_REG2, ~(B_XHCI_MEM_AUX_CTRL_REG2_EN_U2_P3));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdCreditRedeem:
      if (FeatureVal == IpUsb3FeatValCreditRedeemEn) {
        IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_TRM_REG2, B_XHCI_MEM_HOST_CTRL_TRM_REG2_CLR_CPCR);
      } else if (FeatureVal == IpUsb3FeatValCreditRedeemDis) {
        IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_TRM_REG2, ~(B_XHCI_MEM_HOST_CTRL_TRM_REG2_CLR_CPCR));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdBackToBackBTOHandlingEnable:
      if (FeatureVal == IpUsb3FeatValBackToBackBTOHandlingEnableEn) {
        IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_D0I2_CTRL_REG, B_XHCI_MEM_D0I2_CTRL_REG_B2B_BTO_EN);
      } else if (FeatureVal == IpUsb3FeatValBackToBackBTOHandlingEnableDis) {
        IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_D0I2_CTRL_REG, ~(B_XHCI_MEM_D0I2_CTRL_REG_B2B_BTO_EN));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdHhhFlushFix:
      if (FeatureVal == IpUsb3FeatValHhhFlushFixEn) {
        IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1, B_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1_CFG_DIS_HHH_FLUSH_FIX);
        IP_WR_AND_32 (pInst->RegCntxtDciSbMmio, R_XHCI_PCR_DBC_ECO_POLICY_REG1, ~(B_XHCI_PCR_DBC_ECO_POLICY_REG1_EN_DBCHHH_USB3LL));
      } else if (FeatureVal == IpUsb3FeatValHhhFlushFixDis) {
        IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1, ~(B_XHCI_MEM_HOST_CTRL_SSP_CONFIG_REG1_CFG_DIS_HHH_FLUSH_FIX));
        IP_WR_OR_32 (pInst->RegCntxtDciSbMmio, R_XHCI_PCR_DBC_ECO_POLICY_REG1, B_XHCI_PCR_DBC_ECO_POLICY_REG1_EN_DBCHHH_USB3LL);
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdB2bRetryFix:
      if (FeatureVal == IpUsb3FeatValB2bRetryFixEn) {
        IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE, B_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE_CHK_BIT_EN_B2B_RETRY_FIX);
      } else if (FeatureVal == IpUsb3FeatValB2bRetryFixDis) {
        IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE, ~(B_XHCI_MEM_HOST_CTRL_USB3_PORT_CONFIG2_TYPE_CHK_BIT_EN_B2B_RETRY_FIX));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdReadAfterWriteDelay:
      if (FeatureVal == IpUsb3FeatValReadAfterWriteDelayEn) {
        IP_WR_AND_32 (pInst->RegCntxtPci, R_XHCI_CFG_XHCC2, ~(B_XHCI_CFG_XHCC2_RAWDD));
      } else if (FeatureVal == IpUsb3FeatValReadAfterWriteDelayDis) {
        IP_WR_OR_32 (pInst->RegCntxtPci, R_XHCI_CFG_XHCC2, B_XHCI_CFG_XHCC2_RAWDD);
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdHSAsyncActiveExposure:
      if (FeatureVal == IpUsb3FeatValHSAsyncActiveExposureEn) {
        IP_WR_OR_32 (pInst->RegCntxtPci, R_XHCI_CFG_HSCFG1, B_XHCI_CFG_HSCFG1_HSAAPE);
      } else if (FeatureVal == IpUsb3FeatValHSAsyncActiveExposureDis) {
        IP_WR_AND_32 (pInst->RegCntxtPci, R_XHCI_CFG_HSCFG1, ~(B_XHCI_CFG_HSCFG1_HSAAPE));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdSoftErrorTriggerRecalibration:
      if (FeatureVal == IpUsb3FeatValSoftErrorTriggerRecalibrationEn) {
        IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_RECAL_REG, BIT31);
      } else if (FeatureVal == IpUsb3FeatValSoftErrorTriggerRecalibrationDis) {
        IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_RECAL_REG, ~(BIT31));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdDbcTraceInEp:
      if (FeatureVal == IpUsb3FeatValDbcTraceInEpEn) {
        IP_WR_OR_32 (pInst->RegCntxtDciSbMmio, R_XHCI_PCR_PMREQDBC, B_XHCI_PCR_PMREQDBC_DBCTRACEEPWAKE);
      } else if (FeatureVal == IpUsb3FeatValDbcTraceInEpDis) {
        IP_WR_AND_32 (pInst->RegCntxtDciSbMmio, R_XHCI_PCR_PMREQDBC, ~(B_XHCI_PCR_PMREQDBC_DBCTRACEEPWAKE));
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }
      break;

    case IpUsb3FeatIdDwbEnable:
      if (FeatureVal == IpUsb3FeatValDwbEnable) {
        Data32Or = (UINT32) (B_XHCI_MEM_TRB_PRF_CTRL_REG4_LTRNDEISOINEN_HS |
                            B_XHCI_MEM_TRB_PRF_CTRL_REG4_PRFTCHHOLDEN |
                            (V_XHCI_MEM_TRB_PRF_CTRL_REG4_DWBFLUSHT << N_XHCI_MEM_TRB_PRF_CTRL_REG4_DWBFLUSHT));
      } else if (FeatureVal == IpUsb3FeatValDwbDisable) {
        Data32Or = B_XHCI_MEM_TRB_PRF_CTRL_REG4_EVENTRUN125_EN;
      } else {
        PRINT_WARNING ("Invalid parameter provided to %s\n", __FUNCTION__);
        return IpCsiStsErrorBadParam;
      }

      Data32And = (UINT32)~(B_XHCI_MEM_TRB_PRF_CTRL_REG4_LTRNDEISOINEN_HS |
                            B_XHCI_MEM_TRB_PRF_CTRL_REG4_LTRNDEISOINEN_ESS |
                            B_XHCI_MEM_TRB_PRF_CTRL_REG4_EVENTRUN125_EN |
                            B_XHCI_MEM_TRB_PRF_CTRL_REG4_PRFTCHHOLDEN |
                            B_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK |
                            B_XHCI_MEM_TRB_PRF_CTRL_REG4_DWBFLUSHT);

      Data32Or |= (UINT32)(V_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK << N_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK);

      IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_TRB_PRF_CTRL_REG4, Data32And, Data32Or);
      break;

    case IpUsb3FeatIdUnknown:
    default:
      PRINT_WARNING ("Invalid FeatureId provided to %s\n", __FUNCTION__);
      return IpCsiStsErrorUnknownId;
  }

  return IpCsiStsSuccess;
}

/**
  USB3 IP Instance Initialization

  @param[in]  pInst  USB3 IP instance

  @retval     IpCsiStsSuccess        completed without error
  @retval     IpCsiStsErrorNullPtr   null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3InstInit (
  IP_USB3_INST  *pInst
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  return IpCsiStsSuccess;
}


/**
  USB3 IP initialization procedure entry point

  @param[in]  pInst   USB3 IP instance

  @retval     IpCsiStsSuccess        completed without error
  @retval     IpCsiStsError          couldn't enable MMIO space register access
  @retval     IpCsiStsErrorNullPtr   null pointer to IP Instance provide
**/
IP_CSI_STATUS
IpUsb3IpInit (
  IP_USB3_INST  *pInst
  )
{
  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  if (IpUsb3VersionSpecificConfigurationEnabled (pInst, IpUsb3VscIdSocCreditExchangeTime)) {
    IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                        R_XHCI_MEM_PM_CTRL_REG1,
                        ~(B_XHCI_MEM_PM_CTRL_REG1_IP_INACCESSIBLE_HYSTERESIS_TIMER),
                        V_XHCI_MEM_PM_CTRL_REG1_IP_INACCESSIBLE_HYSTERESIS_TIMER_150US);
  }

  if (IpUsb3VersionSpecificConfigurationEnabled (pInst, IpUsb3VscIdUsb2RfPowerGating)) {
    IP_WR_AND_32 (pInst->RegCntxtMem, R_XHCI_MEM_PMCTRL2, ~(B_XHCI_MEM_PMCTRL2_PORT_RF_PG_USB2_DIS_DISC_EN));
  }

  if (IpUsb3VersionSpecificConfigurationEnabled (pInst, IpUsb3VscIdCameraFlicker)) {
    IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                         R_XHCI_MEM_TRB_PRF_CTRL_REG4,
                         ~(B_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK | B_XHCI_MEM_TRB_PRF_CTRL_REG4_LTRNDEISOINEN_ESS),
                         (V_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK << N_XHCI_MEM_TRB_PRF_CTRL_REG4_TDWTMRK));

    IP_WR_OR_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_SSP_LINK_REG2, (B_XHCI_MEM_HOST_CTRL_SSP_LINK_REG2_CFG_DIS_U2_SS | B_XHCI_MEM_HOST_CTRL_SSP_LINK_REG2_CFG_DIS_U1_SS));
  }

  if (IpUsb3VersionSpecificConfigurationEnabled (pInst, IpUsb3VscIdC20PhyElasticBuffer)) {
    IP_WR_AND_32 (pInst->RegCntxtSbMmio, R_XHCI_PCR_DAP_PHY_CONTROL_REG_1, ~(B_XHCI_PCR_DAP_PHY_CONTROL_REG_1_EB_MODE_GEN2));
  }

  return IpCsiStsSuccess;
}

/**
  This interface will apply the tuning values provided in the tuning list to the IP.

  @param[in]  pInst         USB3 IP Instance
  @param[in]  pTuningList   List of Parameter Ids and Parameter Values to be applied.
  @param[in]  EntryCount    The number of tuning list entries in the pTuningList.

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
  @retval     IpCsiStsErrorUnknownId  bad IP_CSI_TUNING_PARAM_ENTRY
**/
IP_CSI_STATUS
IpUsb3ApplyTuningList (
  IP_USB3_INST                *pInst,
  IP_CSI_TUNING_PARAM_ENTRY   *pTuningList,
  UINT32                      EntryCount
  )
{
  UINT32   Index;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return IpCsiStsErrorNullPtr;
  }

  if (pTuningList == NULL) {
    PRINT_ERROR ("You must provide tuning list to %s\n", __FUNCTION__);
    return IpCsiStsErrorNullPtr;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  for (Index = 0; Index < EntryCount; Index++) {
    switch (pTuningList[Index].ParamId) {
      case IpCsiTuningParamIdReleaseInfo:
        PRINT_LEVEL1 ("%s: TuningList release info data %0X\n", __FUNCTION__, pTuningList[Index].ParamData);
        break;

      case IpCsiTuningParamIdAscii:
        PRINT_LEVEL1 (
          "%s: TuningList Ascii data: %c%c%c%c\n",
          __FUNCTION__,
          pTuningList[Index].ParamData & 0xFF,
          ((pTuningList[Index].ParamData >> 8) & 0xFF),
          ((pTuningList[Index].ParamData >> 16) & 0xFF),
          ((pTuningList[Index].ParamData >> 24) & 0xFF)
          );
        break;

      case IpUsb3TuningParamIdCp13Deemphasis:
        IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_CP13_DEEMPH_REG, pTuningList[Index].ParamData);
        break;

      case IpUsb3TuningParamIdCp14Deemphasis:
        IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_CP14_DEEMPH_REG, pTuningList[Index].ParamData);
        break;

      case IpUsb3TuningParamIdCp15Deemphasis:
        IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_CP15_DEEMPH_REG, pTuningList[Index].ParamData);
        break;

      case IpUsb3TuningParamIdCp16Deemphasis:
        IP_WR_WRITE_32 (pInst->RegCntxtMem, R_XHCI_MEM_HOST_CTRL_USB3_CP16_DEEMPH_REG, pTuningList[Index].ParamData);
        break;

      case IpUsb3TuningParamIdLinkPollingMinTime:
        if (pTuningList[Index].ParamData > 0x7) {
          PRINT_ERROR ("%s - Invalid tuning value provided to IpUsb3TuningParamIdLinkPollingMinTime\n", __FUNCTION__);
          return IpCsiStsErrorBadParam;
        }

        IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                            R_XHCI_MEM_HOST_CTRL_PORT_LINK_REG,
                            ~(B_XHCI_MEM_HOST_CTRL_PORT_LINK_REG_LP_MIN_TM),
                            ((pTuningList[Index].ParamData << N_XHCI_MEM_HOST_CTRL_PORT_LINK_REG_LP_MIN_TM) & B_XHCI_MEM_HOST_CTRL_PORT_LINK_REG_LP_MIN_TM));
        break;

      case IpUsb3TuningParamIdGapTimeAfterLsTx:
        if (pTuningList[Index].ParamData > 0xFF) {
          PRINT_ERROR ("%s - Invalid tuning value provided to IpUsb3TuningParamIdGapTimeAfterLsTx\n", __FUNCTION__);
          return IpCsiStsErrorBadParam;
        }

        IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                            R_XHCI_MEM_USB2_PROTOCOL_GAP_TIMER_REG_HI,
                            ~(B_XHCI_MEM_USB2_PROTOCOL_GAP_TIMER_REG_HI_LSTX_GAP_TIME),
                            ((pTuningList[Index].ParamData << N_XHCI_MEM_USB2_PROTOCOL_GAP_TIMER_REG_HI_LSTX_GAP_TIME) & B_XHCI_MEM_USB2_PROTOCOL_GAP_TIMER_REG_HI_LSTX_GAP_TIME));
        break;

      case IpUsb3TuningParamIdU2ExitLatency:
        if (pTuningList[Index].ParamData > 0xFFFF) {
          PRINT_ERROR ("%s - Invalid tuning value provided to IpUsb3TuningParamIdU2ExitLatency\n", __FUNCTION__);
          return IpCsiStsErrorBadParam;
        }

        IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                            R_XHCI_MEM_HCSPARAMS3,
                            ~(B_XHCI_MEM_HCSPARAMS3_U2DEL),
                            ((pTuningList[Index].ParamData << N_XHCI_MEM_HCSPARAMS3_U2DEL) & B_XHCI_MEM_HCSPARAMS3_U2DEL));
        break;

      case IpUsb3TuningParamIdHbufWaterMark:
        if (pTuningList[Index].ParamData > 0x7FFF) {
          PRINT_ERROR ("%s - Invalid tuning value provided to IpUsb3TuningParamIdHbufWaterMark\n", __FUNCTION__);
          return IpCsiStsErrorBadParam;
        }

        IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                            R_XHCI_MEM_HOST_CTRL_PORT_CTRL,
                            ~(B_XHCI_MEM_HOST_CTRL_PORT_CTRL_HBUF_WATER_MARK),
                            ((pTuningList[Index].ParamData << N_XHCI_MEM_HOST_CTRL_PORT_CTRL_HBUF_WATER_MARK) & B_XHCI_MEM_HOST_CTRL_PORT_CTRL_HBUF_WATER_MARK));
        break;

      case IpUsb3TuningParamIdSsBackbonePxpTrunkClkGtEn:
        if (pTuningList[Index].ParamData > 0xF) {
          PRINT_ERROR ("%s - Invalid tuning value provided to IpUsb3TuningParamIdSsBackbonePxpTrunkClkGtEn\n", __FUNCTION__);
          return IpCsiStsErrorBadParam;
        }

        IP_WR_AND_THEN_OR_32 (pInst->RegCntxtPci,
                            R_XHCI_CFG_XHCLKGTEN,
                            ~(B_XHCI_CFG_XHCLKGTEN_SSTCGE),
                            ((pTuningList[Index].ParamData << N_XHCI_CFG_XHCLKGTEN_SSTCGE) & B_XHCI_CFG_XHCLKGTEN_SSTCGE));
        break;

      case IpUsb3TuningParamIdIdleHysteresisScale:
        if (pTuningList[Index].ParamData > 0x2) {
          PRINT_ERROR ("%s - Invalid tuning value provided to IpUsb3TuningParamIdIdleHysteresisScale\n", __FUNCTION__);
          return IpCsiStsErrorBadParam;
        }

        IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                            R_XHCI_MEM_PWR_SCHED_CTRL2,
                            ~(B_XHCI_MEM_PWR_SCHED_CTRL2_IDLE_SCALE),
                            ((pTuningList[Index].ParamData << N_XHCI_MEM_PWR_SCHED_CTRL2_IDLE_SCALE) & B_XHCI_MEM_PWR_SCHED_CTRL2_IDLE_SCALE));
        break;

      case IpUsb3TuningParamIdMinIdleSpan:
        if (pTuningList[Index].ParamData > 0xFF) {
          PRINT_ERROR ("%s - Invalid tuning value provided to IpUsb3TuningParamIdMinIdleSpan\n", __FUNCTION__);
          return IpCsiStsErrorBadParam;
        }

        IP_WR_AND_THEN_OR_32 (pInst->RegCntxtMem,
                            R_XHCI_MEM_PWR_SCHED_CTRL0,
                            ~(B_XHCI_MEM_PWR_SCHED_CTRL0_EIH),
                            ((pTuningList[Index].ParamData << N_XHCI_MEM_PWR_SCHED_CTRL0_EIH) & B_XHCI_MEM_PWR_SCHED_CTRL0_EIH));
        break;

      case IpUsb3TuningParamIdUnknown:
      default:
        PRINT_ERROR ("Invalid ParamId found in pTuningList\n");
        return IpCsiStsErrorUnknownId;
    }
  }

  return IpCsiStsSuccess;
}

/**
  Ip Version Specific Configuration(VSC) Enabled

  @param[in]  pInst         USB3 IP Instance
  @param[in]  Id            Configuration ID (see IP_USB3_VSC_ID)

  @retval     TRUE          IP Version requires that programming
  @retval     FALSE         IP Version doesn't require additional programming
**/
BOOLEAN
IpUsb3VersionSpecificConfigurationEnabled (
  IP_USB3_INST    *pInst,
  IP_USB3_VSC_ID  Id
  )
{
  BOOLEAN   RetVal;

  if (pInst == NULL) {
    PRINT_ERROR_NULL_INST ("Instance pointer cannot be NULL\n");
    return FALSE;
  }

  PRINT_LEVEL1 ("Entered %s\n", __FUNCTION__);

  switch (Id) {
    case IpUsb3VscIdUsb2RfPowerGating:
      RetVal = (pInst->IpVersion < IpUsb3IpVersion19p2);
      break;
    case IpUsb3VscIdSocCreditExchangeTime:
      RetVal = (pInst->IpVersion <= IpUsb3IpVersion19p3) && (pInst->Integration != IpUsb3IntegrationUsb4ss);
      break;

    case IpUsb3VscIdCameraFlicker:
      RetVal = (pInst->IpVersion == IpUsb3IpVersion19p3) && (pInst->Integration != IpUsb3IntegrationUsb4ss);
      break;

    case IpUsb3VscIdC20PhyElasticBuffer:
      RetVal = (pInst->IpVersion == IpUsb3IpVersion19p3) && (pInst->Integration == IpUsb3IntegrationUsb4ss);
      break;

    case IpUsb3VscIdUnknown:
    default:
      RetVal = FALSE;
      break;
  }

  if (RetVal == FALSE) {
    RetVal = IpWrVersionSpecificConfigurationEnabled (pInst->IpWrVscCntxt, (UINT32)Id);
  }

  return RetVal;
}
