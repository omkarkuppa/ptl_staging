/** @file
  Implementation of the memory controller hardware abstraction layer.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

**/

// Include files
#include "MrcHalRegisterAccess.h" // for function prototypes and interface definitions
#include "CMcAddress.h"
#include "MrcCommon.h"
#include "MrcDdrIoApi.h"
#include "MrcChipApi.h"
#include "MrcHalInternal.h"
#include "MrcDdrIoDefines.h"
#include "CMrcStatsTracker.h" // for MrcStatsIncrementData()
#include "CBlueMrcCommonTypes.h"
#include "MrcCkdOffsets.h"
#include "MrcDdr5Ckd.h"

// Local function pointer typedefs
typedef MrcStatus  (*MRC_GET_HASH_FUNC)  (MrcParameters *const MrcData, UINT32 const Socket, UINT32 const Controller, UINT32 const Channel, UINT32 const Rank, UINT32 const Strobe, UINT32 const Lane, UINT32 const FreqIndex, GSM_GT const Group, UINT32 *const HashVal, UINT64_STRUCT *const VolatileMask);

// Local Structures
typedef struct {
  GSM_GT             EndMarker;
  MRC_GET_HASH_FUNC  MrcGetHashFunc;
} MrcGetSetControl;

// Local Static Arrays
static MrcGetSetControl MrcGetSetLoopControl[] = {
  {EndOfPhyMarker, &MrcGetDdrIoHash},
  {EndOfIocMarker, &MrcGetDdrIoCfgHash},
  {EndOfMctMarker, &MrcGetMcTimingHash},
  {EndOfMccMarker, &MrcGetMcConfigHash},
  {EndOfPmaMarker, &MrcGetPmaHash},
  {EndOfCkdMarker, &MrcGetCkdHash}
};

#ifdef MRC_DEBUG_PRINT
///
/// DDR IO Debug Strings
///
const CHAR8* GsmGtDebugStrings[GsmDebugStringMax] = {
  "RecEnDelay",
  "RxDqsPDelay",
  "RxDqsNDelay",
  "RxVref",
  "RxDqVref",
  "RxDqVrefByte",
  "RxVocRise",
  "McOdtDelay",
  "McOdtDuration",
  "SenseAmpDelay",
  "SenseAmpDuration",
  "RoundTripDelay",
  "RxDqsBitDelay",
  "TxDqsDelay",
  "TxDqDelay",
  "TxDqBitDelay",
  "TxRonUp",
  "TxRonDn",
  "CmdGrpPi",
  "CtlGrpPi",
  "RecEnOffset",
  "RxDqsOffset",
  "RxVrefOffset",
  "TxDqsOffset",
  "TxDqOffset",
  "RxDqsBitOffset",
  "WckGrpPi",
  "TxRankMuxDelay",
  "TxRankMuxDelayDq",
  "TxRankMuxDelayDqs",
  "TxR2RDqEq",
  "TxR2RDqsEq",
  "TxR2RDqPi",
  "TxR2RDqsPi",
  "RxR2RDqsOff",
  "RxR2RDqOff",
  "RxR2RRxPi",
  "RxR2RRcvEn",
  "RxRankMuxDelay",
  "TxDqFifoWrEnTcwlDelay",
  "TxDqFifoRdEnTcwlDelay",
  "DefDrvEnLow",
  "GsmIocDllWeakLock",
  "GsmIocTxDllWeakLock",
  "RxVocFall",
  "WrRetrainDeltaPiCode",
  "RdRetrainDeltaPiCode",
  "RxCompDqsDelayP",
  "RxCompDqsDelayN",
  "RxCompDqsOffset",
  "WrDSCodeUpCmd",
  "WrDSCodeUpCtl",
  "WrDSCodeUpClk",
  "WrDSCodeDnCmd",
  "WrDSCodeDnCtl",
  "WrDSCodeDnClk",
  "DqOdtVrefUp",
  "DqOdtVrefDn",
  "DqDrvVrefUp",
  "DqDrvVrefDn",
  "ClkDrvVrefUp",
  "ClkDrvVrefDn",
  "CmdDrvVrefUp",
  "CmdDrvVrefDn",
  "CtlDrvVrefUp",
  "CtlDrvVrefDn",
  "CompRcompOdtUp",
  "CompRcompOdtDn",
                                ///< -----------------------------------------------------------------
  "EndOfPhyMarker",             ///< Marker for end of phy groups.
                                ///< -----------------------------------------------------------------
  "GsmIocCccCaVoltageSelect",
  "GsmIocCccCtlVoltageSelect",
  "GsmIocCccClkVoltageSelect",
  "GsmIocWckClkVoltageSelect",
  "GsmIocCccTxCANmosOnly",
  "GsmIocCccTxCTLNmosOnly",
  "GsmIocCccTxCLKNmosOnly",
  "GsmIocWckTxCLKNmosOnly",
  "GsmIocDisWckPupDcc",
  "GsmIocDataTxDqNmosOnly" ,
  "GsmIocCompTxCANmosOnly" ,
  "GsmIocCompTxCTLNmosOnly",
  "GsmIocCompTxCLKNmosOnly",
  "GsmIocCompTxDqNmosOnly",
  "GsmIocIoReset",
  "GsmIocDccClkGateDisable",
  "GsmTrainWCkEn",
  "GsmTrainWCkPulse",
  "GsmTrainWCkBL",
  "GsmTrainWCkMask",
  "GsmWCkDiffLowInIdle",
  "GsmIocDisDataIdlClkGate",
  "GsmIocScramLp5Mode",
  "GsmIocChNotPop",
  "GsmIocDfiCmdDecoderEn",
  "GsmIocDisIosfSbClkGate",
  "GsmIocScramEn",
  "GsmIocWrRetrainOvrd",
  "GsmIocDdr52NMode",
  "GsmDdrReset",
  "GsmIocDdr5CkdMode",
  "GsmIocDisableTxDqs",
  "GsmIocForceRxAmpOn",
  "GsmIocForceRxOnDqs",
  "GsmIocForceRxOnDqsMux",
  "GsmIocForceRxOnDq",
  "GsmIocForceOdtOn",
  "GsmIocTxPiPwrDnDis",
  "GsmIocInternalClocksOn",
  "GsmIocDqsMaskPulseCnt",
  "GsmIocDqsPulseCnt",
  "GsmIocDqOverrideData",
  "GsmIocDqOverrideEn",
  "GsmIocDqsOvrdToggle",
  "GsmIocRankOverrideEn",
  "GsmIocRankOverrideVal",
  "GsmIocDataDqsOdtParkMode",
  "GsmIocDataInvertNibble",
  "GsmIocEnableLpMode4",
  "GsmIocWrEnViewDrv",
  "GsmDataDccRankEn",
  "GsmDataDccLaneEn",
  "GsmClkDccRankEn",
  "GsmWckDccRankEn",
  "GsmWckDccLaneEn",
  "GsmCompVccDdqTarget",
  "GsmInitCompleteOvrd",
  "GsmInitCompleteOvrdVal",
  "GsmPChannelEn",
  "GsmSkipRestoreCR",
  "GsmSkipRetentionCR",
  "GsmSaxgEnable",
  "GsmSaxgReady",
  "GsmSaxgPwrGood",
  "GsmPostCode",
  "GsmWorkPointAuxClk",
  "GsmWorkPointRatio",
  "GsmWorkPointGear4",
  "GsmWorkPointLCPLLWPSel",
  "GsmWorkPointPhClkRefClk",
  "GsmWorkPointWP2LCPLL",
  "GsmTrimClkOvrd",
  "GsmDccPHClkPhaseEn",
  "GsmDccPHClkDutyCycleEn",
  "GsmCompCtrlPeriodicComp",
  "GsmCompCtrlDVFSComp",
  "GsmCompCtrlCompDist",
  "GsmCompCtrlCompUpdate",
  "GsmIocRetrainSwizzleCtlRetrainEn",
  "GsmIocRetrainSwizzleCtlSerialMrr",
  "GsmIocRetrainInitPiCode",
  "GsmIocRetrainRoCount",
  "GsmIocRetrainCtlInitTrain",
  "GsmIocRetrainCtlDuration",
  "GsmIocRetrainCtlResetStatus",
  "GsmIocAuxClkOffAuxClk",
  "GsmIocQClkCalOff",
  "GsmIocPHClkOffsetFastCal",
  "GsmIocAuxOffsetFastCal",
  "GsmMiscPmMessage",
  "GsmMiscPmMessageRunBusy",
  "GsmMiscPmMessageWpSel",
  "GsmIocCs2NReset",
  "GsmIocCsGearDownEnable",
  "GsmIocCsGearDownForce",
  "GsmIocCsGearDownSrDramTrack",
  "GsmIocITerdcdselovrval",
  "GsmIocITerdcdselovren",
  "GsmDccDllDcdTargetSel",
  "GsmLvrCurrentSensorCntStart",
  "GsmLvrCurrentSensorAmpHyster",
  "GsmLvrCurrentSensorEnChop",
  "GsmLvrCurrentSensorEnSensor",
  "GsmLvrCurrentSensorDuration",
  "GsmLvrCurrentSensorSelLVR",
  "GsmCompLvrCurrentSensorCntStart",
  "GsmCompLvrCurrentSensorAmpHyster",
  "GsmCompLvrCurrentSensorEnChop",
  "GsmCompLvrCurrentSensorEnSensor",
  "GsmCompLvrCurrentSensorDuration",
  "GsmCompLvrCurrentSensorSelLVR",
  "GsmCccVccClkFFCurrentSensorCntStart",
  "GsmCccVccClkFFCurrentSensorDuration",
  "GsmDataVccClkFFCurrentSensorCntStart",
  "GsmDataVccClkFFCurrentSensorDuration",
  "GsmCccVccClkFFCurrentSensorSelCnt",
  "GsmDataVccClkFFCurrentSensorSelCnt",
  "GsmLvrCurrentSensorEnCnt",
  "GsmCompLvrCurrentSensorEnCnt",
  "GsmCccVccClkFFCurrentSensorEnCnt",
  "GsmDataVccClkFFCurrentSensorEnCnt",
  "GsmFFCodePhDrvG",
  "GsmLvrCurrentSensorCount",
  "GsmDataVccClkFFCurrentSensorCount",
  "GsmCccVccClkFFCurrentSensorCount",
  "GsmLvrAutoTrimOffsetClkRx0",
  "GsmLvrAutoTrimOffsetClkRx1",
  "GsmLvrAutoTrimOffsetClkTx",
  "GsmLvrAutoTrimOffsetIOG",
  "GsmLvrAutoTrimOffsetNbiasFF",
  "GsmLvrAutoTrimCccOffsetClkTx",
  "GsmLvrAutoTrimCccOffsetIOG",
  "GsmLvrAutoTrimOffsetClkQ",
  "GsmLvrAutoTrimOffsetDIST",
  "GsmLvrAutoTrimOffsetPLL",
  "GsmLvrWakeIncClkRx",
  "GsmLvrWakeIncClkTx",
  "GsmLvrWakeIncIOG",
  "GsmLvrCccWakeIncClkTx",
  "GsmLvrCccWakeIncIOG",
  "GsmLvrCompWakeIncClkQ",
  "GsmLvrCompWakeIncIOG",
  "GsmLvrCompWakeIncDIST",
  "GsmLvrCompWakeIncPLL",
  "GsmLvrEnDiv",
  "GsmLvrChop",
  "GsmLvrUsePSAL",
  "GsmLvrVoltageSensorCntStart",
  "GsmLvrVoltageSensorDuration",
  "GsmLvrAutoTrimDelay",
  "GsmLvrEnSensor",
  "GsmLvrTarget0",
  "GsmLvrTarget1",
  "GsmLvrVinDivider",
  "GsmLvrSelCount",
  "GsmLvrSelLVR",
  "RcvEnFfMuxSelByte0",
  "GsmLvrVoltageSensorSelLvr",
  "GsmLvrAutoTrimSelLvr",
  "GsmVccIoVccF0",
  "GsmVccIoVccF1",
  "GsmVccIoVccF2",
  "GsmVccIoOCSlope",
  "GsmVccClkVccF0",
  "GsmVccClkVccF1",
  "GsmVccClkVccF2",
  "GsmVccClkOCSlope",
  "GsmVccClkRxCtlB0FbDivider",
  "GsmVccClkRxCtlB0Target",
  "GsmVccClkRxCtlB0BiasAdj",
  "GsmVccClkRxCtlB0PMBiasAdj",
  "GsmVccClkRxCtlB1FbDivider",
  "GsmVccClkRxCtlB1Target",
  "GsmVccClkRxCtlB1BiasAdj",
  "GsmVccClkRxCtlB1PMBiasAdj",
  "GsmVccClkFFBiasFbDivider",
  "GsmVccClkFFBiasNbTarget",
  "GsmVccPllControlFbDivider",
  "GsmVccPllControlTarget",
  "GsmVccDistControlFbDivider",
  "GsmVccDistControlTarget",
  "GsmVccIogControlFbDivider",
  "GsmVccIogControlTarget",
  "GsmVccIogControlBiasAdj",
  "GsmVccIogControlPMBiasAdj",
  "GsmVccClkTxControlFbDivider",
  "GsmVccClkTxControlTarget",
  "GsmVccClkTxControlBiasAdj",
  "GsmVccClkTxControlPMBiasAdj",
  "GsmVccClkqControlFbDivider",
  "GsmVccClkqControlTarget",
  "GsmVccClkqControlBiasAdj",
  "GsmVccClkqControlPMBiasAdj",
  "GsmVccDdqControlFbDivider",
  "GsmVccDdqControlTarget",
  "GsmVccDdqControlBiasAdj",
  "GsmVccDdqControlPMBiasAdj",
  "GsmLvrAutoTrimRunBusy",
  "GsmLvrAutoTrimStatusVsenseOffset",
  "GsmLvrPwrGoodOnVrefChange",
  "GsmLvrRstCount",
  "GsmLvrVoltageSensorCount0",
  "GsmLvrVoltageSensorCount1",
  "GsmDataLvrAutoTrimRunBusy",
  "GsmCccLvrAutoTrimRunBusy",
  "GsmCompLvrAutoTrimRunBusy",
  "GsmIocCompForceLvrOn",
  "GsmIOGLVREnableOvrd",
  "GsmIOGLVRVrefEnOvrd",
  "GsmIOGPwrGoodOvrd",
  "GsmIOGGVLVROvrdModeEn",
  "GsmIOGLVROvrdModeEn",
  "GsmDataLvrAutoTrimOffsetClk",
  "GsmDataLvrAutoTrimOffsetIOG",
  "GsmCccLvrAutoTrimOffsetClk",
  "GsmCccLvrAutoTrimOffsetIOG",
  "GsmCompLvrAutoTrimOffsetIOG",
  "GsmCompLvrAutoTrimOffsetIOGGV",
  "GsmCompLvrAutoTrimOffsetDist",
  "GsmIogLvrVrefSel",
  "GsmIogLvrSelVdd2Ladder",
  "GsmClkLvrVrefSel",
  "GsmClkLvrRxDllB0VrefSel",
  "GsmClkLvrRxDllB1VrefSel",
  "GsmClkLvrSelVdd2Ladder",
  "GsmIogGvLvrVrefSel",
  "GsmIogGvLvrSelVdd2Ladder",
  "GsmDistGLvrVrefSel",
  "GsmDistGLvrSelVdd2Ladder",
  "GsmVsBonusIn",
  "GsmVsSelVdd2Ladder",
  "EndOfIocMarker",
  "GsmMctRCD",
  "GsmMctRCDw",
  "GsmMctRDA2ACT",
  "GsmMctWRA2ACT",
  "GsmMctRP",
  "GsmMctRPab",
  "GsmMctRAS",
  "GsmMctnWRDiff",
  "GsmMctRDPRE",
  "GsmMctPPD",
  "GsmMctWRPRE",
  "GsmMctFAW",
  "GsmMctRRDsg",
  "GsmMctRRDdg",
  "GsmMctVrcgEnable",
  "GsmMctVrcgDisable",
  "GsmMctREFSbRd",
  "GsmMctLpDeratingExt",
  "GsmMctRDRDsg",
  "GsmMctRDRDdg",
  "GsmMctRDRDdr",
  "GsmMctRDRDdd",
  "GsmMctRDWRsg",
  "GsmMctRDWRdg",
  "GsmMctRDWRdr",
  "GsmMctRDWRdd",
  "GsmMctWRRDsg",
  "GsmMctWRRDdg",
  "GsmMctWRRDdr",
  "GsmMctWRRDdd",
  "GsmMctWRWRsg",
  "GsmMctWRWRdg",
  "GsmMctWRWRdr",
  "GsmMctWRWRdd",
  "GsmMctCKE",
  "GsmMctXP",
  "GsmMctPRPDEN",
  "GsmMctRDPDEN",
  "GsmMctWRPDEN",
  "GsmMctCA2CS",
  "GsmMctCSL",
  "GsmMctCSH",
  "GsmtccdCasDelta",
  "GsmMctCL",
  "GsmMctCWL",
  "GsmMctCWLAdd",
  "GsmMctCWLDec",
  "GsmMctXSDLL",
  "GsmMctXSR",
  "GsmMctLpMode4SrxDelay",
  "GsmMctSR",
  "GsmMctXSRDs",
  "GsmMctMRD",
  "GsmMctSrIdleTimer",
  "GsmMctSrefDelayQsync",
  "GsmMctREFI",
  "GsmMctRFC",
  "GsmMcDdr5Rir",
  "GsmMctRefreshHpWm",
  "GsmMctRefreshPanicWm",
  "GsmMctZQCalPeriod",
  "GsmMcttZQLatch",
  "GsmMctZQCAL",
  "GsmMctCPDED",
  "GsmMctCKCKEH",
  "GsmMctRFCpb",
  "GsmMctRFM",
  "GsmMctDRFM",
  "GsmMcttOSCO",
  "GsmMcttPREMRR",
  "GsmMctWrOsclRuntime",
  "GsmMcttMRR",
  "GsmMcttCSLCK",
  "GsmMcttWckStop",
  "GSmMcttWckOff",
  "GsmMcttCkFspX",
  "GsmMctWckEnlFs",
  "GsmMctWckEnlRd",
  "GsmMctWckEnlWr",
  "GsmMctWckPreToggleFs",
  "GsmMctWckPreToggleRd",
  "GsmMctWckPreToggleWr",
  "GsmMctWckDfiStop",
  "GsmMctWckDfiOffset",
  "GsmMctMrhAfterCommandDelay",
  "EndOfMctMarker",
  "GsmMccWCKDiffLowInIdle",
  "GsmMccDramType",
  "GsmMccCmdStretch",
  "GsmMccAddrMirror",
  "GsmMccCmdTriStateDis",
  "GsmMccFreqPoint",
  "GsmMccEnableOdtMatrix",
  "GsmMccGearMode",
  "GsmMccNoGear4ParamDivide",
  "GsmMccOneDpc",
  "GsmMccMultiCycCmd",
  "GsmMccEnhancedInterleave",
  "GsmMccHashEnabled",
  "GsmMccHashLsb",
  "GsmMccHashZone1Start",
  "GsmMccEccMode",
  "GsmMccExtendedBankHash",
  "GsmMccBg0Hash",
  "GsmMccAddrDecodeDdrType",
  "GsmMccCh0Size",
  "GsmMccCh1Size",
  "GsmMccCpgcActive",
  "GsmMccInOrderIngress",
  "GsmMccSubch0SdramWidth",
  "GsmMccSubch0RankCnt",
  "GsmMccSubch0Density",
  "GsmMccSubch1SdramWidth",
  "GsmMccSubch1RankCnt",
  "GsmMccSubch1Density",
  "GsmMccFspOp",
  "GsmMccFspRdWr",
  "GsmMccFspMrsOp",
  "GsmMccEnableRefresh",
  "GsmMccSrState",
  "GsmMccForceSreWithFreqChange",
  "GsmMccEnableDclk",
  "GsmMccPureSrx",
  "GsmMccCpgcInOrder",
  "GsmMccBlockXarb",
  "GsmMccBlockCke",
  "GsmMccRankOccupancy",
  "GsmMccMcSrx",
  "GsmMccEnRefTypeDisplay",
  "GsmMccHashMask",
  "GsmMccLsbMaskBit",
  "GsmMccPbrDis",
  "GsmMccRefreshAbrRelease",
  "GsmMccCkeOverride",
  "GsmMccCkeOn",
  "GsmMccRrdValidTrigger",
  "GsmMccRrdValidOverflow",
  "GsmMccRrdValidValue",
  "GsmMccRrdValidSign",
  "GsmMccCsOverride0",
  "GsmMccCsOverrideVal0",
  "GsmMccCsOverride1",
  "GsmMccCsOverrideVal1",
  "GsmMccMinRefRate",
  "GsmMccMR4Period",
  "GsmMccLpddrCurrentFsp",
  "GsmMccLpddrCurrentFspTrk",
  "GsmMccZqSerialize",
  "GsmMccDeswizzleByte",
  "GsmMccDeswizzleBit",
  "GsmMccMrrResult",
  "GsmMccLp5BankMode",
  "GsmMccLp5WckMode",
  "GsmMccLp5WckFastMode",
  "GsmMccRefreshRankMask",
  "GsmDisAllCplInterleave",
  "GsmDisVc1CplInterleave",
  "GsmMccRhLfsr0",
  "GsmMccRhLfsr1",
  "GsmMccRhTRRDimmEnable",
  "GsmMccRhDrfmEnable",
  "GsmMccRhLfsr0Mask",
  "GsmMccRhLfsr1Mask",
  "GsmMccRhMA1Swizzle",
  "GsmMccRhMA2Swizzle",
  "GsmMccMicronFSwizzle",
  "GsmMccMicronRSwizzle",
  "GsmMccBrc",
  "GsmMccRhEnableDimm0",
  "GsmMccRhEnableDimm1",
  "GsmMccRhHighWM",
  "GsmMccRhLowWM",
  "GsmMccRhNormalRefSub",
  "GsmMccRhRefmSub",
  "GsmMccRhRefmEn",
  "GsmMccAllowRHDebtSR",
  "GsmMccOppSrefEnable",
  "GsmMccForceSRState",
  "GsmMccVC0ReadEntries",
  "GsmMccVC1ReadEntries",
  "GsmMccSpineGateDis",
  "GsmMccMainGlbDrvGateDis",
  "GsmMccSchedGlbDrvGateDis",
  "GsmMccOdtMatrixRd",
  "GsmMccOdtMatrixWr",
  "GsmMccCasStopAdditionalGap",
  "GsmMccRdWckAsyncGap",
  "GsmMccWrWckAsyncGap",
  "GsmMccEnSrxWrRetraining",
  "GsmMccEnWrRetraining",
  "GsmMccEnRdRetraining",
  "GsmMccRetrainPeriod",
  "GsmMccVc0LatencyGaurdTimerx16",
  "GsmMccVc1LatencyGuardTimerx8",
  "GsmMccVc1IsocLatencyGuardTimerx8",
  "GsmMccLatencySensitiveTimerx16",
  "GsmMccSelfRefreshEnable",
  "GsmMccSelfRefreshLength",
  "GsmMccPowerDownEnable",
  "GsmMccIdleEnable",
  "GsmMccDis2cByp",
  "GsmMccSchedDisClkGateLocal",
  "GsmMccDisAsyncOdt",
  "GsmMccDisStarvedPriorityOnNewReq",
  "GsmMccDisIosfSbClkGate",
  "GsmMccCkDisTristate",
  "GsmMccPageIdleTimerDisable",
  "GsmMccDisableSplitAct",
  "GsmMccAllowOppRefBelowWrtThreshold",
  "GsmMccWrtThreshold",
  "GsmMccPdwnIdleCounterSubch0",
  "GsmMccPdwnIdleCounterSubch1",
  "GsmMccPdwnLpmodeEnable",
  "GsmMccDisCkett",
  "GsmMccTTIdleCounter",
  "GsmMccAPD",
  "GsmMccPPD",
  "GsmMccCmiSourceId0",
  "GsmMccCmiSourceId1",
  "GsmMccCmiSourceId2",
  "GsmMccCmiSourceId3",
  "GsmMccDelayPMAckCycles",
  "GsmMccEnSrxTempRead",
  "GsmMccSrxZqcal",
  "GsmMccEcDis",
  "GsmMccEccGranularity",
  "GsmMccAutoPrechargeEn",
  "GsmMccEnCsGearDown",
  "GsmMccCfiE2eParityEnRdata",
  "GsmMccCfiE2eParityEnRdCmplHeader",
  "GsmMccCfiE2eParityEn",
  "GsmScPbrEcsRefabEnable",
  "GsmScPbrEcsRefabPeriod",
  "GsmMccDdr5CkdEnable",
  "GsmMccDisLpddr5RdwrInterleaving",
  "GsmMccEccCorrectionDisable",
  "GsmMccMcMntsSpareRw",
  "GsmMccMcMntsRfFastSleepDisable",
  "GsmMccMcCbRowPressDisAutoPre",
  "GsmMccPageOpenPolicyMaxCount",
  "GsmMccPageOpenPolicyEn",
  "GsmMccRd2rdEchoMask",
  "GsmMccWr2wrEchoMask",
  "GsmMccDdr1rSplitBgOnSubch",
  "EndOfMccMarker",
  "GsmPmaEnableMc",
  "GsmPmaEnableIbecc",
  "GsmPmaEnableCce",
  "GsmPmaEnableTme",
  "GsmPmaDdrType",
  "GsmPmaMaxBandwidth",
  "GsmPmaQclkRatioReq",
  "GsmPmaGearTypeReq",
  "GsmPmaDvfsVddq",
  "GsmPmaVddqChangeOnly",
  "GsmPmaMaxBwGbps",
  "GsmPmaQclkWpIdx",
  "GsmPmaRunBusy",
  "GsmPmaMrcSave",
  "GsmPmaMemConfigDone",
  "GsmPmaMemConfigDoneAck",
  "GsmPmaQclkRatioData",
  "GsmPmaGearTypeData",
  "GsmPmaErrorCode",
  "GsmPmaBiosMailboxReserved",
  "EndOfPmaMarker",
  "GsmCkdPllMode",
  "GsmCkdIctInputClkTerm",
  "GsmCkdQckClkDisable",
  "GsmCkdQckOutputDelayEnable",
  "GsmCkdQckDrive",
  "GsmCkdQckSlewRate",
  "GsmCkdQckOutputDelay",
  "EndOfCkdMarker",
  "GsmComplexRcvEn",
  "GsmComplexRxBias",
  "GsmGtMax"
};
#endif // MCR_DEBUG_PRINT

static const UINT8 DqMirroringTable[MAX_BITS] = {7, 5, 6, 4, 3, 1, 2, 0};

/**
  Function to translate DQ Bitmask within GetSet value using internal PhyDqSwizzling if needed.
  Assumes DQ bitmask on bits[7:0].

  @param[in]  Strobe - Dqs data group within the rank (0-based).
  @param[in]  Value  - Get/Set value to operate on.

  @retval Translated GetSet value
**/
INT64
TranslateDqBitmaskThroughPhyDqSwizzling (
  IN UINT32 Strobe,
  IN INT64  Value
  )
{
  INT64 DeswizzledValue;
  UINT8 TranslatedDqBitMask;
  UINT8 Index;

  // PHY Data swizzling applies only for DATA1/3/5/7 instances.
  // Check if internal swizzling applies to given partition.
  if ((Strobe % 2) == 1) {
    DeswizzledValue = (Value & ~MRC_UINT8_MAX); // clear DQ bits[7:0]
    TranslatedDqBitMask = 0;
    for (Index = 0; Index < MAX_BITS; Index++) {
      if ((((UINT8)Value >> Index) & 0x1) == 0x1) {
        // If the bit is set, move it to the new (translated) bit position
        TranslatedDqBitMask |= (1 << DqMirroringTable[Index]);
      }
    }
    DeswizzledValue |= TranslatedDqBitMask; // fill bits[7:0] with translated value
  } else {
    DeswizzledValue = Value;
  }

  return DeswizzledValue;
}

/**
  Function to translate Bit/Lane number using internal PhyDqSwizzling if needed.

  @param[in]  Strobe - Dqs data group within the rank (0-based).
  @param[in]  Bit    - Bit value within the data group (0-based).

  @retval Translated value
**/
UINT32
TranslateDqBitThroughPhyDqSwizzling (
  IN UINT32 Strobe,
  IN UINT32 Bit
  )
{
  UINT32 TranslatedBit;

  // PHY Data swizzling applies only for DATA1/3/5/7 instances.
  // Check if internal swizzling applies to given partition.
  if (((Strobe % 2) == 1) && (Bit < MAX_BITS)) {
    TranslatedBit = DqMirroringTable[Bit];
  } else {
    TranslatedBit = Bit;
  }

  return TranslatedBit;
}

/**
  Function to translate specific GetSet value (bitmask or bit number) using internal PhyDqSwizzling if needed.

  @param[in]      Group  - DDRIO group to access.
  @param[in]      Strobe - Dqs data group within the rank (0-based).
  @param[in,out]  Value  - Pointer to value for Get/Set to operate on.
**/
VOID
TranslateGetSetValueThroughPhyDqSwizzling (
  IN     GSM_GT Group,
  IN     UINT32 Strobe,
  IN OUT INT64  *Value
  )
{
  switch (Group) {
    case GsmIocDqOverrideData:
    case GsmIocDqOverrideEn:
      *Value = TranslateDqBitmaskThroughPhyDqSwizzling (Strobe, *Value);
      break;

    default:
      break;
  }
}

/**
Top level function used to interact with SOC.
The flow is as follows:
  Get the GSM_GT bit shift instruction, CR offset.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Level       - DDRIO level to access.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
GetSet (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Dimm,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      UINT32        const   FreqIndex,
  IN      GSM_LT        const   Level,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
)
{
  MrcStatus         Status;
  INT64             CurrentVal;
  INT64             UpdateVal;
  INT64             Min;
  INT64             Max;
  UINT64            RegVal;
  UINT64            NewRegVal;
  UINT64            ReadHwRegVal;
  UINT64            CacheRegVal;
  UINT32            Offset;
  UINT32            Delay;
  BOOLEAN           RegSize;
  BOOLEAN           ReadOnly;
  BOOLEAN           MergeBackNeeded;
  MRC_REGISTER_HASH_STRUCT  HashVal;
  UINT64_STRUCT     VolatileMask;
#ifdef MRC_DEBUG_PRINT
  MrcDebug      *Debug;
  INT64_STRUCT  PrintVal;
  CHAR8         Str[120];
  CHAR8         *p;

  Debug = &MrcData->Outputs.Debug;
#endif // MRC_DEBUG_PRINT
#ifdef MRC_DISABLE_CACHING
  Mode &= ~GSM_CACHE_ONLY;
  Mode |= GSM_READ_CSR;
#endif // MRC_DISABLE_CACHING

  ReadOnly = (Mode & GSM_READ_ONLY) == GSM_READ_ONLY;
  VolatileMask.Data = 0;
  MergeBackNeeded = FALSE;

  Status = MrcGetHash (MrcData, Socket, Controller, Channel, Rank, Strobe, Bit, FreqIndex, Group, &HashVal.Data, &VolatileMask);
  if (Status != mrcSuccess) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "MrcGetHash failed, group: %d\n", Group);
    return Status;
  }

  Offset = HashVal.Bits.Offset;
  RegSize = (HashVal.Bits.RegSize != 0);
#ifdef MRC_DEBUG_PRINT
  if (Offset > MRC_REGISTER_HASH_Offset_MAX) {
    p = Str;
    p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "No Offset found for Group %s(%d)", GsmGtDebugStrings[Group], Group) - 1;
    if (Socket != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Socket %u,", Socket) - 1;
    }
    if (Controller != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Controller %u,", Controller) - 1;
    }
    if (Channel != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Channel %u,", Channel) - 1;
    }
    if (Rank != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Rank %u,", Rank) - 1;
    }
    if (Strobe != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Strobe %u,", Strobe) - 1;
    }
    if (Bit != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Bit %u,", Bit) - 1;
    }
    if (FreqIndex != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " FreqIndex %u", FreqIndex) - 1;
    }
    MRC_DEBUG_ASSERT (Offset < MRC_REGISTER_HASH_Offset_MAX, Debug, "%s\n", Str);
  }
#endif // MRC_DEBUG_PRINT

  // If we try to read a volatile bitfield from cache, then assert
  if (HashVal.Bits.IsVolatile && ((Mode & GSM_READ_ONLY) == GSM_READ_ONLY)) {
    MRC_DEBUG_ASSERT (((Mode & GSM_READ_CSR) == 1), Debug, "Reading a Volatile Bitfield from Cache with Group: %s\n", GsmGtDebugStrings[Group]);
  }

  // Init data.
  RegVal = MrcCrCacheRead (MrcData, Offset, RegSize, Mode);

  // Get the group value
  CurrentVal = MrcHalGsmGetBitField (MrcData, HashVal, RegVal);

  if (ReadOnly) {
    *Value = CurrentVal;
    // UpdatedVal is used for printing below due to W/A around SOT parameter
    UpdateVal = CurrentVal;
  } else {
    CurrentVal = ((Mode & GSM_WRITE_OFFSET) == GSM_WRITE_OFFSET) ? CurrentVal + *Value : *Value;

    // Check value limits.
    Status = MrcGetSetLimits (MrcData, Group, Rank, &Min, &Max, &Delay);
    if (Status != mrcSuccess) {
      return Status;
    }

    if ((CurrentVal < Min) || (CurrentVal > Max)) {
#ifdef MRC_DEBUG_PRINT
      p = Str;
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "%s", gWarnString) - 1;
      if (Controller != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "Mc%u", Controller) - 1;
      }
      if (Channel != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".C%u", Channel) - 1;
      }
      if (Rank != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".R%u", Rank) - 1;
      }
      if (Strobe != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".S%u", Strobe) - 1;
      }
      if (Bit != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".B%u,", Bit) - 1;
      }
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " %s value %lld is out of range [%lld:%lld]. Clamping\n",
        GsmGtDebugStrings[Group], CurrentVal, Min, Max) - 1;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s\n", Str);
#endif // MRC_DEBUG_PRINT
      Status = mrcParamSaturation;
    }
#ifdef MRC_DEBUG_PRINT
    if (Delay != 0) {
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Delay for %uus\n", Delay);
    }
#endif
    CurrentVal = RANGE (CurrentVal, Min, Max);

    UpdateVal = CurrentVal;

    // Check if writing a volatile CR
    if (HashVal.Bits.IsVolatile) {
      MRC_DEBUG_ASSERT (VolatileMask.Data != 0, Debug, "\nVolatile Mask is not defined, Group: %s\n", GsmGtDebugStrings[Group]);
      MRC_DEBUG_ASSERT ((Mode & (GSM_CACHE_ONLY | GSM_UPDATE_CACHE)) != (GSM_CACHE_ONLY | GSM_UPDATE_CACHE), Debug,
        "\nAttempting to update only the cache for a field that is Volatile (%s).  HW register needs to be updated at same time\n",
        GsmGtDebugStrings[Group]);
      MergeBackNeeded = TRUE;
    } else if (!(HashVal.Bits.IsVolatile) && (VolatileMask.Data != 0)) {
      // Writing a non-volatile bit in a register with volatiles
      MergeBackNeeded = TRUE;
    }

    if (MergeBackNeeded) {
      // MRC_DEBUG_MSG(Debug, MSG_LEVEL_TIME, "HashVal: 0x%08x, Group: %s, ", HashVal.Data, GsmGtDebugStrings[Group]);
      // MRC_DEBUG_MSG(Debug, MSG_LEVEL_TIME, "Cached Reg Value: 0x%08x, ", RegVal);
      // MRC_DEBUG_MSG(Debug, MSG_LEVEL_TIME, "Cached Reg Value to Write: 0x%08x, ", NewRegVal);
      // ReadCR
      ReadHwRegVal = MrcCrCacheRead (MrcData, Offset, RegSize, ReadUncached);
      // MRC_DEBUG_MSG(Debug, MSG_LEVEL_TIME, "Volatile Mask: 0x%08x, ", VolatileMask.Data);
      // MRC_DEBUG_MSG(Debug, MSG_LEVEL_TIME, "HW CR Read Reg Value: 0x%08x, ", ReadHwRegVal);

      // AND HW CR with VolatileMask
      ReadHwRegVal = (ReadHwRegVal) & (VolatileMask.Data);

      //(~VolatileMask) & CacheRegVal
      CacheRegVal = (~VolatileMask.Data) & (RegVal);

      // WriteVal = ReadCR | CacheRegVal
      RegVal = ReadHwRegVal | CacheRegVal;
      // MRC_DEBUG_MSG(Debug, MSG_LEVEL_TIME, "HW CR Reg Value with Updated Volatile Fields: 0x%08x, ", RegVal);

      // Update CR with new value
      NewRegVal = MrcHalGsmSetBitField (MrcData, HashVal, UpdateVal, RegVal);

      // MRC_DEBUG_MSG(Debug, MSG_LEVEL_TIME, "New Reg Value to Write: 0x%08x\n", NewRegVal);
    } else {
      // Update CR with new value
      NewRegVal = MrcHalGsmSetBitField (MrcData, HashVal, UpdateVal, RegVal);
    }

    if (((Mode & GSM_FORCE_WRITE) == GSM_FORCE_WRITE) || (RegVal != NewRegVal)) {
      MrcCrCacheWrite (MrcData, Offset, RegSize, Mode, Delay, NewRegVal);
    }
  }

#ifdef MRC_DEBUG_PRINT
  if (Mode & GSM_PRINT_VAL) {
    p = Str;
    PrintVal.Data = UpdateVal;
    *p++ = (ReadOnly) ? 'R' : 'W';
    *p++ = ' ';
    if (Controller != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "Mc%u", Controller) - 1;
    }
    if (Channel != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".C%u", Channel) - 1;
    }
    if (Rank != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".R%u", Rank) - 1;
    }
    if (Strobe != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".S%u", Strobe) - 1;
    }
    if (Bit != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".B%u", Bit) - 1;
    }
    p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " %s: ", GsmGtDebugStrings[Group]) - 1;
    if (HashVal.Bits.BfWidth > 32) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "0x%llx", CurrentVal) - 1;
    } else {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "%d", PrintVal.Data32.Low) - 1;
      if ((HashVal.Bits.BfSign == 0) && ((UINT32) (PrintVal.Data32.Low) > 9)) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " (0x%x)", PrintVal.Data32.Low) - 1;
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s\n", Str);
  }
#endif // MRC_DEBUG_PRINT

  return Status;
}

/**
  Top level function that is System Level Specificity.
  The flow is as follows:
    Check Multicast/Unicast, translate from system to IP register architecture, side effect and update other PHY logic.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - Dqs data group within the rank (0-based).
  @param[in]      Lane        - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Level       - DDRIO level to access.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcGetSet (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Dimm,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Lane,
  IN      UINT32        const   FreqIndex,
  IN      GSM_LT        const   Level,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
)
{
  const MRC_FUNCTION *MrcCall;
  MrcStatus         Status;
  MrcStatus         CurrentStatus;
  MrcOutput         *Outputs;
  UINT32            SocketLoop;
  UINT32            ControllerLoop;
  UINT32            ChannelLoop;
  UINT32            RankLoop;
  UINT32            StrobeLoop;
  UINT32            LaneLoop;
  BOOLEAN           ReadOnly;
  BOOLEAN           IsLpddr;
  BOOLEAN           IsSkipGroup;
  BOOLEAN           MulticastAccess;
  BOOLEAN           ValidMc;
  BOOLEAN           ValidCh;
  BOOLEAN           ValidRank;
  BOOLEAN           ValidStrobe;
  BOOLEAN           SkipEmptyFubs;
  UINT32            TransController;
  UINT32            TransChannel;
  UINT32            TransRank;
  UINT32            TransStrobe;
  UINT32            TransLane;
  MRC_RANGE         SocketRange;
  MRC_RANGE         ControllerRange;
  MRC_RANGE         ChannelRange;
  MRC_RANGE         RankRange;
  MRC_RANGE         StrobeRange;
  MRC_RANGE         LaneRange;

  // Check that the Group is unsupported
  if (MrcCheckGroupUnSupported (MrcData, Group, Strobe)) {
    return mrcSuccess;
  }

#ifdef MRC_DEBUG_PRINT
  MrcDebug      *Debug;

  Debug = &MrcData->Outputs.Debug;

  MrcStatsIncrementData (MrcData, MRC_BLUE_GET_SET_CALLS);
  // Check that the level is supported
  switch (Level) {
    case DdrLevel:
      break;

    default:
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "GetSet, Level %d is not supported!\n", Level);
      return mrcWrongInputParameter;
      break;
  }

  // Check that the Group is supported
  Status = MrcCheckGroupSupported (MrcData, Group);
  if (Status != mrcSuccess) {
    return Status;
  }
#endif // MRC_DEBUG_PRINT

  Outputs = &MrcData->Outputs;
  MrcCall = MrcData->Inputs.Call.Func;
  IsLpddr = Outputs->IsLpddr;
  MrcCall->MrcSetMem ((UINT8 *) &SocketRange,     sizeof (SocketRange),     0);
  MrcCall->MrcSetMem ((UINT8 *) &ControllerRange, sizeof (ControllerRange), 0);
  MrcCall->MrcSetMem ((UINT8 *) &ChannelRange,    sizeof (ChannelRange),    0);
  MrcCall->MrcSetMem ((UINT8 *) &RankRange,       sizeof (RankRange),       0);
  MrcCall->MrcSetMem ((UINT8 *) &StrobeRange,     sizeof (StrobeRange),     0);
  MrcCall->MrcSetMem ((UINT8 *) &LaneRange,       sizeof (LaneRange),       0);

  ReadOnly = (Mode & GSM_READ_ONLY) == GSM_READ_ONLY;
  SkipEmptyFubs = (Mode & GSM_WRITE_ALL) == 0;
  Status = mrcSuccess;

  // Detect and convert all Multicast accesses into unicast.
  MulticastAccess = MrcDetectMulticast (
                      MrcData,
                      Group,
                      Socket,
                      Controller,
                      Channel,
                      Rank,
                      Strobe,
                      Lane,
                      &SocketRange,
                      &ControllerRange,
                      &ChannelRange,
                      &RankRange,
                      &StrobeRange,
                      &LaneRange
                    );

  ValidMc     = (Controller != MRC_IGNORE_ARG);
  ValidCh     = (Channel != MRC_IGNORE_ARG);
  ValidRank   = (Rank != MRC_IGNORE_ARG);
  ValidStrobe = (Strobe != MRC_IGNORE_ARG);

  IsSkipGroup = MrcGsmGtSkip (Group);

  // Break Multicast requests into multiple unicasts if it is not GSM_READ_ONLY.
  if (MulticastAccess) {
    if (ReadOnly) {
      // Ensure no one is trying to read with multicast parameters.
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Attempted to read from a Multicast. Group: %s(%d) Controller: %u, Channel: %u, Rank: %u, Byte: %u, Lane: %u\n",
        gWarnString, GsmGtDebugStrings[Group], Group, Controller, Channel, Rank, Strobe, Lane);
      return mrcWrongInputParameter;
    }
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "SocketStart: %d\tSocketEnd: %d\n", SocketRange.Start, SocketRange.End);
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "ControllerStart: %d\tControllerEnd: %d\n", ControllerRange.Start, ControllerRange.End);
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "ChannelStart: %d\tChannelEnd: %d\n", ChannelRange.Start, ChannelRange.End);
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "RankStart: %d\tRankEnd: %d\n", RankRange.Start, RankRange.End);
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "StrobeStart: %d\tStrobeEnd: %d\n", StrobeRange.Start, StrobeRange.End);
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "LaneStart: %d\tLaneEnd: %d\n", LaneRange.Start, LaneRange.End);
    for (SocketLoop = SocketRange.Start; SocketLoop <= SocketRange.End; SocketLoop++) {
      for (ControllerLoop = ControllerRange.Start; ControllerLoop <= ControllerRange.End; ControllerLoop++) {
        if (ValidMc) {
          if (!MrcControllerExist (MrcData, ControllerLoop) && SkipEmptyFubs) {
            MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s skipped as %s %d does not exist\n", GsmGtDebugStrings[Group], gControllerStr, ControllerLoop);
            continue;
          }
        }
        for (ChannelLoop = ChannelRange.Start; ChannelLoop <= ChannelRange.End; ChannelLoop++) {
          if (ValidMc && ValidCh) {
            if (!MrcChannelExist (MrcData, ControllerLoop, ChannelLoop) && SkipEmptyFubs) {
              MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s skipped as %s %d does not exist\n", GsmGtDebugStrings[Group], gChannelStr, ChannelLoop);
              continue;
            } else if (IsSkipGroup && (IS_MC_SUB_CH (IsLpddr, ChannelLoop))) {
              // Skip calling MrcGetSet for CH 1/3 if we are LPDDR since there isn't any register associated to it.
              continue;
            }
          }
          for (RankLoop = RankRange.Start; RankLoop <= RankRange.End; RankLoop++) {
            if (ValidMc && ValidCh && ValidRank) {
              if (!MrcRankExist (MrcData, ControllerLoop, ChannelLoop, RankLoop) && SkipEmptyFubs) {
                MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s skipped as %s %d does not exist\n", GsmGtDebugStrings[Group], gRankStr, RankLoop);
                continue;
              }
            }
            for (StrobeLoop = StrobeRange.Start; StrobeLoop <= StrobeRange.End; StrobeLoop++) {
              if (ValidMc && ValidCh && ValidRank && ValidStrobe) {
                if (!MrcByteExist (MrcData, ControllerLoop, ChannelLoop, StrobeLoop) && SkipEmptyFubs) {
                  MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s skipped as %s %d does not exist\n", GsmGtDebugStrings[Group], gByteStr, StrobeLoop);
                  continue;
                }
              }
              for (LaneLoop = LaneRange.Start; LaneLoop <= LaneRange.End; LaneLoop++) {
                CurrentStatus = MrcGetSet (
                                  MrcData,
                                  SocketLoop,
                                  ControllerLoop,
                                  ChannelLoop,
                                  Dimm,
                                  RankLoop,
                                  StrobeLoop,
                                  LaneLoop,
                                  FreqIndex,
                                  Level,
                                  Group,
                                  Mode,
                                  Value
                                  );
                if (Status == mrcSuccess) {
                  Status = CurrentStatus;
                }
              } // BitLoop
            } // StrobeLoop
          } // RankLoop
        } // ChannelLoop
      } // ControllerLoop
    } // SocketLoop
    // We stop here when breaking up multicasts.
    return Status;
  }

  if (MrcCheckComplexOrSideEffect (MrcData, Group, MRC_IS_COMPLEX)) {
    // Complex parameters will stop here.
    Status = MrcGetSetComplexParam (MrcData, Socket, Controller, Channel, 0, Rank, Strobe, FreqIndex, Group, Mode, Value);
    if (Status == mrcComplexParamDone) {
      return mrcSuccess;
    }
  }

  TransController = Controller;
  TransChannel = Channel;
  TransRank = Rank;
  TransStrobe = Strobe;
  TransLane = Lane;

#if defined(LOCAL_STUB_STATE_FLAG)
  // Record the logical location information of the GetSet field. Fields like CurrentWrV
  // and CmdVrefSetting need to be preserved between writes to the LsState register, but
  // StageIncrement must be cleared, as the state of LocalStub is modified whenever it is
  // set.
  MRC_LOCAL_STUB_STATE_STRUCT LsState;
  LsState.Data = MrcReadCR64(MrcData, MRC_LOCAL_STUB_STATE_REG);
  LsState.Bits.Socket = Socket;
  LsState.Bits.Controller = Controller;
  LsState.Bits.Channel = Channel;
  LsState.Bits.Dimm = Dimm;
  LsState.Bits.Rank = Rank;
  LsState.Bits.Strobe = Strobe;
  LsState.Bits.Lane = Lane;
  LsState.Bits.StateInit = 0;
  LsState.Bits.StageIncrement = 0;
  MrcWriteCR64(MrcData, MRC_LOCAL_STUB_STATE_REG, LsState.Data);
#endif // STUB_FLAG

  Status = MrcTranslateSystemToIp (MrcData, &TransController, &TransChannel, &TransRank, &TransStrobe, &TransLane, Group);

  if (Status == mrcSuccess) {
    if (!ReadOnly) {
      // Translate write value for specific Groups
      TranslateGetSetValueThroughPhyDqSwizzling (Group, Strobe, Value);
    }
    Status = GetSet (MrcData, Socket, TransController, TransChannel, Dimm, TransRank, TransStrobe, TransLane, FreqIndex, Level, Group, Mode, Value);

    if (!ReadOnly) {
      // Update other Phy Channels
      MrcGetSetUpdatePhyChannels (MrcData, Socket, Controller, Channel, Dimm, Rank, Strobe, Lane, FreqIndex, Level, Group, Mode, Value);

      // Update MRC Host
      if ((Mode & GSM_UPDATE_HOST) == GSM_UPDATE_HOST) {
        MrcGetSetUpdateHost (MrcData, Controller, Channel, Rank, Strobe, Lane, Group, Value);
      }

      // Check if the field written to has some side effect and resolve it.
      if (MrcCheckComplexOrSideEffect (MrcData, Group, MRC_IS_SIDE_EFFECT)) {
        MrcGetSetSideEffect (MrcData, Socket, Controller, Channel, 0, Dimm, Rank, Strobe, Lane, FreqIndex, Level, Group, Mode, Value);
      }
    }
  }

  return Status;
}

/**
 Helper function to get max partition number.

 @param[in] Partition - Partition type.
 @param[in] UlxUlt    - Whether CPU SKU is ULX / ULT (PHY).

**/
UINT32
GetPartitionMax (
  IN PARTITION_TYPE   Partition,
  IN BOOLEAN          UlxUlt
  )
{
  UINT32 PartitionMax;

  switch (Partition) {
    case PartitionPg:
      PartitionMax = MRC_PG_NUM;
      break;

    case PartitionDataShared:
      PartitionMax = MRC_DATA_SHARED_NUM;
      break;

    case PartitionCccShared:
      PartitionMax = MRC_CCC_SHARED_NUM;
      break;

    case PartitionComp:
      PartitionMax = MRC_COMP_NUM;
      break;

    case PartitionPll:
    default:
      PartitionMax = MAX_BITS;
      break;
  }
  return PartitionMax;
}

/**
  Variable Max Limits for parameters.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - The group to be checked.
  @param[in]  Index     - The index of the group to be checked.
  @param[out] RankMax   - Maximum value allow for Rank parameter.
  @param[out] StrobeMax - Maximum value allow for Strobe parameter.
  @param[out] LaneMax   - Maximum value allow for Lane parameter.
**/
VOID
MrcGetSetParamMaxAdjust (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group,
  IN  UINT32                Index,
  OUT UINT32                *RankMax,
  OUT UINT32                *StrobeMax,
  OUT UINT32                *LaneMax
  )
{
  MrcInput *Inputs;
  BOOLEAN  UlxUlt;

  Inputs    = &MrcData->Inputs;
  UlxUlt    = Inputs->IsDdrIoUlxUlt;

  MRC_DEBUG_ASSERT (LaneMax != 0, &MrcData->Outputs.Debug, "%s %s", gNullPtrErrStr, __FUNCTION__);

  switch (Group) {
    case GsmCompVccDdqTarget:
      *LaneMax = MRC_COMP_NUM;
      break;

    case GsmDccPHClkPhaseEn:
    case GsmDccPHClkDutyCycleEn:
      *StrobeMax = MaxPartition;
      *LaneMax   = GetPartitionMax (Index, UlxUlt);
      break;

    default:
      //Everything else is considered as part of Data set
      *LaneMax = MAX_BITS;
      break;
  }
}

/**
  Top level function used to interact with SOC.
  The flow is as follows:
    Get the GSM_GT bit shift instruction, CR offset.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
CkdGetSet (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      UINT32        const   FreqIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  )
{
  MrcStatus         Status;
  INT64             CurrentVal;
  INT64             UpdateVal;
  INT64             Min;
  INT64             Max;
  UINT64            RegVal;
  UINT64            NewRegVal;
  UINT32            Offset;
  UINT32            Delay;
  BOOLEAN           ReadOnly;
  MRC_REGISTER_HASH_STRUCT  HashVal;
  UINT64_STRUCT     VolatileMask;
  INT64             ValueField;

#ifdef MRC_DEBUG_PRINT
  MrcDebug      *Debug;
  INT64_STRUCT  PrintVal;
  CHAR8         Str[120];
  CHAR8         *p;

  Debug = &MrcData->Outputs.Debug;

#endif // MRC_DEBUG_PRINT

  ReadOnly = (Mode & GSM_READ_ONLY) == GSM_READ_ONLY;
  VolatileMask.Data = 0;

  Status = MrcGetHash (MrcData, Socket, Controller, Channel, Rank, Strobe, Bit, FreqIndex, Group, &HashVal.Data, &VolatileMask);

  if (Status != mrcSuccess) {
    return Status;
  }

  Offset = HashVal.Bits.Offset;
#ifdef MRC_DEBUG_PRINT
  if (Offset >= MAX_CKD_REG) {
    p = Str;
    p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "No Offset found for Group %s(%d)", GsmGtDebugStrings[Group], Group) - 1;
    if (Socket != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Socket %u,", Socket) - 1;
    }
    if (Controller != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Controller %u,", Controller) - 1;
    }
    if (Channel != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Channel %u,", Channel) - 1;
    }
    if (Rank != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Rank %u,", Rank) - 1;
    }
    if (Strobe != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Strobe %u,", Strobe) - 1;
    }
    if (Bit != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " Bit %u,", Bit) - 1;
    }
    if (FreqIndex != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " FreqIndex %u", FreqIndex) - 1;
    }
    *p = '\0';
    MRC_DEBUG_ASSERT (Offset < MAX_CKD_REG, Debug, "%s\n", Str);
  }
#endif // MRC_DEBUG_PRINT

  // Init data.
  RegVal = MrcCkdBufferRead (MrcData, (UINT8) Strobe, Offset);

  // Get the group value
  CurrentVal = MrcHalGsmGetBitField (MrcData, HashVal, RegVal);

  if (ReadOnly) {
    *Value = CurrentVal;
    UpdateVal = CurrentVal;
  } else {
    ValueField = *Value;
    CurrentVal = ((Mode & GSM_WRITE_OFFSET) == GSM_WRITE_OFFSET) ? CurrentVal + ValueField : ValueField;

    // Check value limits.
    MrcGetSetLimits (MrcData, Group, Rank, &Min, &Max, &Delay);

    if ((CurrentVal < Min) || (CurrentVal > Max)) {
#ifdef MRC_DEBUG_PRINT
      p = Str;
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "%s", gWarnString) - 1;
      if (Controller != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "Mc%u", Controller) - 1;
      }
      if (Channel != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".C%u", Channel) - 1;
      }
      if (Rank != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".R%u", Rank) - 1;
      }
      if (Strobe != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".S%u", Strobe) - 1;
      }
      if (Bit != MRC_IGNORE_ARG) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".B%u,", Bit) - 1;
      }
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " %s value %lld is out of range [%lld:%lld]. Clamping\n",
        GsmGtDebugStrings[Group], CurrentVal, Min, Max) - 1;
      *p = '\0';
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s\n", Str);
#endif // MRC_DEBUG_PRINT
      Status = mrcParamSaturation;
    }
    CurrentVal = RANGE (CurrentVal, Min, Max);

    UpdateVal = CurrentVal;
    // Update CR with new value
    NewRegVal = MrcHalGsmSetBitField (MrcData, HashVal, UpdateVal, RegVal);

    if (((Mode & GSM_FORCE_WRITE) == GSM_FORCE_WRITE) || (RegVal != NewRegVal)) {
      MrcCkdBufferWrite (MrcData, Mode, (UINT8) Strobe, Offset, (UINT8) NewRegVal);
    }
  }

#ifdef MRC_DEBUG_PRINT
  if (Mode & GSM_PRINT_VAL) {
    p = Str;
    PrintVal.Data = UpdateVal;
    *p++ = (ReadOnly) ? 'R' : 'W';
    *p++ = ' ';
    if (Controller != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "Mc%u", Controller) - 1;
    }
    if (Channel != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".C%u", Channel) - 1;
    }
    if (Rank != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".R%u", Rank) - 1;
    }
    if (Strobe != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".S%u", Strobe) - 1;
    }
    if (Bit != MRC_IGNORE_ARG) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), ".B%u", Bit) - 1;
    }
    p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " %s: ", GsmGtDebugStrings[Group]) - 1;
    if (HashVal.Bits.BfWidth > 32) {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "0x%llx", CurrentVal) - 1;
    } else {
      p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), "%d", PrintVal.Data32.Low) - 1;
      if ((HashVal.Bits.BfSign == 0) && (PrintVal.Data32.Low > 9)) {
        p += MrcSPrintf (MrcData, p, sizeof (Str)-(p - Str), " (0x%x)", PrintVal.Data32.Low) - 1;
      }
    }
    *p = '\0';
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "%s\n", Str);
  }
#endif // MRC_DEBUG_PRINT

  return Status;
}

/**
  Top level function used to read/write CKD Control Word

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus
**/
MrcStatus
MrcCkdGetSet (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  )
{
  const MRC_FUNCTION *MrcCall;
  MrcStatus         Status;
  MrcStatus         CurrentStatus;
  UINT32            ControllerLoop;
  UINT32            ChannelLoop;
  UINT32            RankLoop;
  BOOLEAN           ReadOnly;
  BOOLEAN           MulticastAccess;
  BOOLEAN           ValidMc;
  BOOLEAN           ValidCh;
  BOOLEAN           ValidRank;
  UINT32            TransController;
  UINT32            TransChannel;
  UINT32            TransRank;
  UINT32            TransStrobe;
  UINT32            TransLane;
  MRC_RANGE         ControllerRange;
  MRC_RANGE         ChannelRange;
  MRC_RANGE         RankRange;

#ifdef MRC_DEBUG_PRINT
  MrcDebug      *Debug;

  Debug = &MrcData->Outputs.Debug;

  // Check that the Group is supported
  Status = MrcCheckGroupSupported (MrcData, Group);
  if (Status != mrcSuccess) {
    return Status;
  }
#endif // MRC_DEBUG_PRINT

  MrcCall = MrcData->Inputs.Call.Func;
  MrcCall->MrcSetMem ((UINT8 *) &ControllerRange, sizeof (ControllerRange), 0);
  MrcCall->MrcSetMem ((UINT8 *) &ChannelRange,    sizeof (ChannelRange),    0);
  MrcCall->MrcSetMem ((UINT8 *) &RankRange,       sizeof (RankRange),       0);

  ReadOnly = (Mode & GSM_READ_ONLY) == GSM_READ_ONLY;
  Status = mrcSuccess;

  // Detect and convert all Multicast accesses into unicast.
  MulticastAccess = MrcDetectMulticast (
    MrcData,
    Group,
    MRC_IGNORE_ARG,
    Controller,
    Channel,
    Rank,
    MRC_IGNORE_ARG,
    MRC_IGNORE_ARG,
    NULL,
    &ControllerRange,
    &ChannelRange,
    &RankRange,
    NULL,
    NULL
  );

  ValidMc     = (Controller != MRC_IGNORE_ARG);
  ValidCh     = (Channel != MRC_IGNORE_ARG);
  ValidRank   = (Rank != MRC_IGNORE_ARG);

  // Break Multicast requests into multiple unicasts if it is not GSM_READ_ONLY.
  if (MulticastAccess) {
    if (ReadOnly) {
      // Ensure no one is trying to read with multicast parameters.
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Attempted to read from a Multicast. Group: %s(%d) Controller: %u, Channel: %u, Rank: %u\n",
        GsmGtDebugStrings[Group], Group, Controller, Channel, Rank);
      return mrcWrongInputParameter;
    }
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "ControllerStart: %d\tControllerEnd: %d\n", ControllerRange.Start, ControllerRange.End);
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "ChannelStart: %d\tChannelEnd: %d\n", ChannelRange.Start, ChannelRange.End);
    MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "RankStart: %d\tRankEnd: %d\n", RankRange.Start, RankRange.End);
    for (ControllerLoop = ControllerRange.Start; ControllerLoop <= ControllerRange.End; ControllerLoop++) {
      if (ValidMc) {
        if (!MrcControllerExist (MrcData, ControllerLoop)) {
          MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s skipped as %s %d does not exist\n", GsmGtDebugStrings[Group], gControllerStr, ControllerLoop);
          continue;
        }
      }
      for (ChannelLoop = ChannelRange.Start; ChannelLoop <= ChannelRange.End; ChannelLoop++) {
        if (ValidMc && ValidCh) {
          if (!MrcChannelExist (MrcData, ControllerLoop, ChannelLoop)) {
            MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s skipped as %s %d does not exist\n", GsmGtDebugStrings[Group], gChannelStr, ChannelLoop);
            continue;
          }
        }
        for (RankLoop = RankRange.Start; RankLoop <= RankRange.End; RankLoop++) {
          if (ValidMc && ValidCh && ValidRank) {
            if (!MrcRankExist (MrcData, ControllerLoop, ChannelLoop, RankLoop)) {
              MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s skipped as %s %d does not exist\n", GsmGtDebugStrings[Group], gRankStr, RankLoop);
              continue;
            }
          }
          CurrentStatus = MrcCkdGetSet (
                MrcData,
                ControllerLoop,
                ChannelLoop,
                RankLoop,
                Group,
                Mode,
                Value
              );
          if (Status == mrcSuccess) {
            Status = CurrentStatus;
          }
        } // RankLoop
      } // ChannelLoop
    } // ControllerLoop
    // We stop here when breaking up multicasts.
    return Status;
  }

  // TransRank used to select QCK pin and TransStrobe to select CKD DIMM
  TransController = Controller;
  TransChannel    = Channel;
  TransRank       = Rank;
  Status = MrcTranslateSystemToIp (MrcData, &TransController, &TransChannel, &TransRank, &TransStrobe, &TransLane, Group);

  if (Status != mrcSuccess) {
    return Status;
  }

  Status = CkdGetSet (MrcData, MRC_IGNORE_ARG, TransController, TransChannel, TransRank, TransStrobe, TransLane, MRC_IGNORE_ARG, Group, Mode, Value);

  return Status;
}

/**
  Check whether the group is supported by current project

  @param[in]  MrcData - Global MRC Data.
  @param[in]  Group   - The group to be accessed.
  @param[in]  Strobe  - The Strobe to be accessed.

  @retval True if unsupported, otherwise False
**/
BOOLEAN
MrcCheckGroupUnSupported (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group,
  IN  UINT32        const   Strobe
  )
{
  switch (Group) {
      case GsmMccNoGear4ParamDivide:
      case GsmVccClkRxCtlB0FbDivider:
      case GsmVccClkRxCtlB0Target:
      case GsmVccClkRxCtlB0BiasAdj:
      case GsmVccClkRxCtlB0PMBiasAdj:
      case GsmVccClkRxCtlB1FbDivider:
      case GsmVccClkRxCtlB1Target:
      case GsmVccClkRxCtlB1BiasAdj:
      case GsmVccClkRxCtlB1PMBiasAdj:
      case GsmVccClkFFBiasFbDivider:
      case GsmVccClkFFBiasNbTarget:
      case GsmVccPllControlFbDivider:
      case GsmVccPllControlTarget:
      case GsmVccDistControlFbDivider:
      case GsmVccDistControlTarget:
      case GsmVccIogControlFbDivider:
      case GsmVccIogControlTarget:
      case GsmVccIogControlBiasAdj:
      case GsmVccIogControlPMBiasAdj:
      case GsmVccClkTxControlFbDivider:
      case GsmVccClkTxControlTarget:
      case GsmVccClkTxControlBiasAdj:
      case GsmVccClkTxControlPMBiasAdj:
      case GsmVccClkqControlFbDivider:
      case GsmVccClkqControlTarget:
      case GsmVccClkqControlBiasAdj:
      case GsmVccClkqControlPMBiasAdj:
      case GsmVccDdqControlFbDivider:
      case GsmVccDdqControlTarget:
      case GsmVccDdqControlBiasAdj:
      case GsmVccDdqControlPMBiasAdj:
      case GsmMccDdr1rSplitBgOnSubch:
        return TRUE;

      default:
        return FALSE;
  }
}

#ifdef MRC_DEBUG_PRINT
/**
  This debug only function checks to ensure the group is supported by GetSet.

  @param[in]  MrcData - Global MRC Data.
  @param[in]  Group   - The group to be accessed.

  @retval mrcSuccess if supported, otherwise mrcWrongInputParameter
**/
MrcStatus
MrcCheckGroupSupported (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group
  )
{
  BOOLEAN   IsSupported;

  IsSupported = TRUE;

  switch (Group) {
    case RecEnDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
    case RxVref:
    case RxDqVref:
    case RxVocRise:
    case RxDqsBitDelay:
    case RxDqsBitOffset:
    case RxRankMuxDelay:
    case TxDqsDelay:
    case TxDqDelay:
    case DefDrvEnLow:
    case TxRankMuxDelay:
    case TxR2RDqsEq:
    case TxR2RDqEq:
    case TxR2RDqsPi:
    case TxR2RDqPi:
    case RxR2RDqsOff:
    case RxR2RDqOff:
    case RxR2RRxPi:
    case RxR2RRcvEn:
    case TxDqBitDelay:
    case RecEnOffset:
    case RxDqsOffset:
    case RxVrefOffset:
    case TxDqsOffset:
    case TxDqOffset:
    case WrRetrainDeltaPiCode:
    case RdRetrainDeltaPiCode:
    case RxCompDqsDelayP:
    case RxCompDqsDelayN:
    case RxCompDqsOffset:
    case RoundTripDelay:
    case TxDqFifoWrEnTcwlDelay:
    case TxDqFifoRdEnTcwlDelay:
    case SenseAmpDelay:
    case SenseAmpDuration:
    case McOdtDelay:
    case McOdtDuration:
    case CmdGrpPi:
    case CtlGrpPi:
    case WckGrpPi:
    case TxRonUp:
    case TxRonDn:
    case GsmIocEnableLpMode4:
    case GsmIocDqsMaskPulseCnt:
    case GsmIocDqsPulseCnt:
    case GsmIocDqOverrideData:
    case GsmIocDqOverrideEn:
    case GsmIocRankOverrideEn:
    case GsmIocRankOverrideVal:
    case GsmIocDisableTxDqs:
    case GsmIocForceRxAmpOn:
    case GsmIocForceOdtOn:
    case GsmIocTxPiPwrDnDis:
    case GsmIocInternalClocksOn:
    case GsmIocDataDqsOdtParkMode:
    case GsmIocIoReset:
    case GsmIocDccClkGateDisable:
    case GsmIocScramLp5Mode:
    case GsmIocChNotPop:
    case GsmIocDfiCmdDecoderEn:
    case GsmIocDisIosfSbClkGate:
    case GsmIocDisDataIdlClkGate:
    case GsmIocDataInvertNibble:
    case GsmDataDccRankEn:
    case GsmIocAuxClkOffAuxClk:
    case GsmIocQClkCalOff:
    case GsmIocPHClkOffsetFastCal:
    case GsmIocAuxOffsetFastCal:
    case GsmDccPHClkPhaseEn:
    case GsmDccPHClkDutyCycleEn:
    case GsmCompCtrlPeriodicComp:
    case GsmTrainWCkPulse:
    case GsmTrainWCkBL:
    case GsmTrainWCkMask:
    case GsmWCkDiffLowInIdle:
    case GsmCompCtrlDVFSComp:
    case GsmCompCtrlCompDist:
    case GsmCompCtrlCompUpdate:
    case GsmIocRetrainSwizzleCtlRetrainEn:
    case GsmIocRetrainSwizzleCtlSerialMrr:
    case GsmIocRetrainInitPiCode:
    case GsmIocRetrainRoCount:
    case GsmIocRetrainCtlInitTrain:
    case GsmIocRetrainCtlDuration:
    case GsmIocRetrainCtlResetStatus:
    case GsmClkDccRankEn:
    case GsmWckDccLaneEn:
    case GsmWckDccRankEn:
    case GsmCompVccDdqTarget:
    case GsmInitCompleteOvrd:
    case GsmInitCompleteOvrdVal:
    case GsmPChannelEn:
    case GsmSkipRestoreCR:
    case GsmSkipRetentionCR:
    case GsmSaxgEnable:
    case GsmSaxgReady:
    case GsmSaxgPwrGood:
    case GsmWorkPointAuxClk:
    case GsmWorkPointRatio:
    case GsmWorkPointGear4:
    case GsmMctRCD:
    case GsmMctRCDw:
    case GsmMctRP:
    case GsmMctRDA2ACT:
    case GsmMctWRA2ACT:
    case GsmMctRPab:
    case GsmMctRAS:
    case GsmMctnWRDiff:
    case GsmMctRDPRE:
    case GsmMctPPD:
    case GsmMctWRPRE:
    case GsmMctFAW:
    case GsmMctRRDsg:
    case GsmMctRRDdg:
    case GsmMctVrcgEnable:
    case GsmMctVrcgDisable:
    case GsmMctLpDeratingExt:
    case GsmMctRDRDsg:
    case GsmMctRDRDdg:
    case GsmMctRDRDdr:
    case GsmMctRDRDdd:
    case GsmMctRDWRsg:
    case GsmMctRDWRdg:
    case GsmMctRDWRdr:
    case GsmMctRDWRdd:
    case GsmMctWRRDsg:
    case GsmMctWRRDdg:
    case GsmMctWRRDdr:
    case GsmMctWRRDdd:
    case GsmMctWRWRsg:
    case GsmMctWRWRdg:
    case GsmMctWRWRdr:
    case GsmMctWRWRdd:
    case GsmMctCKE:
    case GsmMctXP:
    case GsmMctPRPDEN:
    case GsmMctRDPDEN:
    case GsmMctWRPDEN:
    case GsmMctCA2CS:
    case GsmMctCSL:
    case GsmMctCSH:
    case GsmMctXSDLL:
    case GsmMctXSR:
    case GsmMctLpMode4SrxDelay:
    case GsmMctMRD:
    case GsmMctZQCAL:
    case GsmMctZQCalPeriod:
    case GsmMcttZQLatch:
    case GsmMctSrIdleTimer:
    case GsmMctSrefDelayQsync:
    case GsmMctREFI:
    case GsmMctRFC:
    case GsmMctRefreshHpWm:
    case GsmMcDdr5Rir:
    case GsmMctRefreshPanicWm:
    case GsmMctMrhAfterCommandDelay:
    case GsmMccEnhancedInterleave:
    case GsmMccHashEnabled:
    case GsmMccHashLsb:
    case GsmMccHashZone1Start:
    case GsmMccEccMode:
    case GsmMccExtendedBankHash:
    case GsmMccBg0Hash:
    case GsmMccAddrDecodeDdrType:
    case GsmMccCpgcActive:
    case GsmMccInOrderIngress:
    case GsmMccSubch0SdramWidth:
    case GsmMccSubch0RankCnt:
    case GsmMccSubch0Density:
    case GsmMccSubch1SdramWidth:
    case GsmMccSubch1RankCnt:
    case GsmMccSubch1Density:
    case GsmMccCh0Size:
    case GsmMccCh1Size:
    case GsmMccEnableRefresh:
    case GsmMccSrState:
    case GsmMccForceSreWithFreqChange:
    case GsmMccEnableDclk:
    case GsmMccPureSrx:
    case GsmMccCpgcInOrder:
    case GsmMccBlockXarb:
    case GsmMccDramType:
    case GsmMccCmdStretch:
    case GsmMccAddrMirror:
    case GsmMccCmdTriStateDis:
    case GsmMccFreqPoint: // This aliases with ddr_probless_low_frequency.
    case GsmMccEnableOdtMatrix:
    case GsmMccDisIosfSbClkGate:
    case GsmMccCkDisTristate:
    case GsmMccAllowOppRefBelowWrtThreshold:
    case GsmMccWrtThreshold:
    case GsmMccPdwnIdleCounterSubch0:
    case GsmMccPdwnIdleCounterSubch1:
    case GsmMccPdwnLpmodeEnable:
    case GsmMccDisCkett:
    case GsmMccTTIdleCounter:
    case GsmMccPPD:
    case GsmMccGearMode:
    case GsmMccOneDpc:
    case GsmMccMultiCycCmd:
    case GsmtccdCasDelta:
    case GsmMctCL:
    case GsmMctCWL:
    case GsmMctCWLAdd:
    case GsmMctCWLDec:
    case GsmMctCKCKEH:
    case GsmMccRankOccupancy:
    case GsmMccMcSrx:
    case GsmMccEnRefTypeDisplay:
    case GsmMccHashMask:
    case GsmMccLsbMaskBit:
    case GsmMctCPDED:
    case GsmMctRFCpb:
    case GsmMctRFM:
    case GsmMctDRFM:
    case GsmMcttOSCO:
    case GsmMcttPREMRR:
    case GsmMctWrOsclRuntime:
    case GsmMcttMRR:
    case GsmMcttCSLCK:
    case GsmMcttWckStop:
    case GSmMcttWckOff:
    case GsmMcttCkFspX:
    case GsmMctWckEnlFs:
    case GsmMctWckEnlRd:
    case GsmMctWckEnlWr:
    case GsmMctWckPreToggleFs:
    case GsmMctWckPreToggleRd:
    case GsmMctWckPreToggleWr:
    case GsmMctWckDfiStop:
    case GsmMctWckDfiOffset:
    case GsmMccPbrDis:
    case GsmMccRefreshAbrRelease:
    case GsmMccCkeOverride:
    case GsmMccCkeOn:
    case GsmMccMinRefRate:
    case GsmMccMR4Period:
    case GsmMccLpddrCurrentFsp:
    case GsmMccLpddrCurrentFspTrk:
    case GsmMccZqSerialize:
    case GsmMccDeswizzleByte:
    case GsmMccDeswizzleBit:
    case GsmMccMrrResult:
    case GsmMccLp5BankMode:
    case GsmMccLp5WckMode:
    case GsmMccLp5WckFastMode:
    case GsmDisAllCplInterleave:
    case GsmDisVc1CplInterleave:
    case GsmMccRhLfsr0:
    case GsmMccRhLfsr1:
    case GsmMccRhTRRDimmEnable:
    case GsmMccRhDrfmEnable:
    case GsmMccRhLfsr0Mask:
    case GsmMccRhLfsr1Mask:
    case GsmMccRhMA1Swizzle:
    case GsmMccRhMA2Swizzle:
    case GsmMccMicronFSwizzle:
    case GsmMccMicronRSwizzle:
    case GsmMccBrc:
    case GsmMccRhEnableDimm0:
    case GsmMccRhEnableDimm1:
    case GsmMccRhHighWM:
    case GsmMccRhLowWM:
    case GsmMccRhNormalRefSub:
    case GsmMccRhRefmSub:
    case GsmMccRhRefmEn:
    case GsmMccAllowRHDebtSR:
    case GsmMccOppSrefEnable:
    case GsmMccForceSRState:
    case GsmMccVC0ReadEntries:
    case GsmMccVC1ReadEntries:
    case GsmMccSpineGateDis:
    case GsmMccMainGlbDrvGateDis:
    case GsmMccSchedGlbDrvGateDis:
    case GsmMccPageIdleTimerDisable:
    case GsmMccRetrainPeriod:
    case GsmMccOdtMatrixRd:
    case GsmMccOdtMatrixWr:
    case GsmMccCasStopAdditionalGap:
    case GsmMccRdWckAsyncGap:
    case GsmMccWrWckAsyncGap:
    case GsmMccEnSrxWrRetraining:
    case GsmMccEnWrRetraining:
    case GsmMccEnRdRetraining:
    case GsmMccVc0LatencyGaurdTimerx16:
    case GsmMccVc1LatencyGuardTimerx8:
    case GsmMccVc1IsocLatencyGuardTimerx8:
    case GsmMccLatencySensitiveTimerx16:
    case GsmMccSelfRefreshEnable:
    case GsmMccSelfRefreshLength:
    case GsmMccPowerDownEnable:
    case GsmMccIdleEnable:
    case GsmMccDis2cByp:
    case GsmMccSchedDisClkGateLocal:
    case GsmMccDisAsyncOdt:
    case GsmMccDisStarvedPriorityOnNewReq:
    case GsmMccAPD:
    case GsmMccDisableSplitAct:
    case GsmMccCmiSourceId0:
    case GsmMccCmiSourceId1:
    case GsmMccCmiSourceId2:
    case GsmMccCmiSourceId3:
    case GsmMccDelayPMAckCycles:
    case GsmMccRrdValidTrigger:
    case GsmMccRrdValidOverflow:
    case GsmMccRrdValidValue:
    case GsmMccRrdValidSign:
    case GsmMccEcDis:
    case GsmMccEccGranularity:
    case GsmMccAutoPrechargeEn:
    case GsmMccEnCsGearDown:
    case GsmScPbrEcsRefabEnable:
    case GsmScPbrEcsRefabPeriod:
    case GsmMccDdr5CkdEnable:
    case GsmMccDisLpddr5RdwrInterleaving:
    case GsmMccEccCorrectionDisable:
    case GsmMccMcMntsSpareRw:
    case GsmMccMcMntsRfFastSleepDisable:
    case GsmMccMcCbRowPressDisAutoPre:
    case GsmMccPageOpenPolicyMaxCount:
    case GsmMccPageOpenPolicyEn:
    case GsmPmaEnableMc:
    case GsmPmaEnableIbecc:
    case GsmPmaEnableCce:
    case GsmPmaEnableTme:
    case GsmPmaDdrType:
    case GsmPmaMaxBandwidth:
    case GsmPmaQclkRatioReq:
    case GsmPmaGearTypeReq:
    case GsmPmaDvfsVddq:
    case GsmPmaVddqChangeOnly:
    case GsmPmaMaxBwGbps:
    case GsmPmaQclkWpIdx:
    case GsmPmaRunBusy:
    case GsmPmaMrcSave:
    case GsmPmaMemConfigDone:
    case GsmPmaMemConfigDoneAck:
    case GsmPmaQclkRatioData:
    case GsmPmaGearTypeData:
    case GsmPmaErrorCode:
    case GsmPmaBiosMailboxReserved:
    case GsmMccWCKDiffLowInIdle:
    case GsmMctREFSbRd:
    case GsmDdrReset:
    case GsmMccCsOverride0:
    case GsmMccCsOverrideVal0:
    case GsmMccCsOverride1:
    case GsmMccCsOverrideVal1:
    case GsmMccBlockCke:
    case GsmMctSR:
    case GsmMctXSRDs:
    case GsmIocScramEn:
    case GsmIocWrRetrainOvrd:
    case GsmIocDdr52NMode:
    case GsmIocCs2NReset:
    case GsmMccRefreshRankMask:
    case GsmMiscPmMessage:
    case GsmMiscPmMessageRunBusy:
    case GsmMiscPmMessageWpSel:
    case GsmMccEnSrxTempRead:
    case GsmMccSrxZqcal:
    case GsmCkdPllMode:
    case GsmCkdIctInputClkTerm:
    case GsmCkdQckClkDisable:
    case GsmCkdQckOutputDelayEnable:
    case GsmCkdQckDrive:
    case GsmCkdQckSlewRate:
    case GsmCkdQckOutputDelay:
    case GsmIocCsGearDownEnable:
    case GsmIocCsGearDownForce:
    case GsmIocCsGearDownSrDramTrack:
    case GsmMccCfiE2eParityEnRdata:
    case GsmMccCfiE2eParityEnRdCmplHeader:
    case GsmMccCfiE2eParityEn:
      break;


    default:
      IsSupported = FALSE;
      break;
  }

  if (!IsSupported) {
    MRC_DEBUG_MSG (
      &MrcData->Outputs.Debug,
      MSG_LEVEL_ERROR,
      "%s GetSet Group %s(%d) is not supported!\n",
      gErrString,
      GsmGtDebugStrings[Group],
      Group
      );
    return mrcWrongInputParameter;
  }
  return mrcSuccess;
}
#endif

/**
  This function is used to determine if the GSM_GT passed in is a complex parameter.

  @param[in]  MrcData - Pointer to global data structure.
  @param[in]  Group   - GetSet group to check.
  @param[in]  IsComplex - The requested check is to see if the parameter is complex.  Otherwise is there a side effect.

  @retval TRUE if complex or side effect, otherwise FALSE.
**/
BOOLEAN
MrcCheckComplexOrSideEffect (
  IN  MrcParameters *const  MrcData,
  IN  GSM_GT        const   Group,
  IN  BOOLEAN       const   IsComplex
  )
{
  BOOLEAN RetVal;

  RetVal = FALSE;

  if (IsComplex) {
  } else {
    // Side Effect
    switch (Group) {
      case RecEnDelay:
      case RecEnOffset:
      case TxDqsDelay:
      case TxDqDelay:
      case TxDqsOffset:
      case TxDqOffset:
        RetVal = TRUE;
        break;

      // No Side Effect.
      default:
        break;
    }
  }

  /*MRC_HAL_DEBUG_MSG (
    &MrcData->Outputs.Debug,
    MSG_LEVEL_HAL,
    "GetSet Group %s(%d) is %s a %s param.\n",
    GsmGtDebugStrings[Group],
    Group,
    (RetVal) ? "" : "not",
    (IsComplex) ? "complex" :  "side effect"
    );*/

  return RetVal;
}

/*
  This function is used to update other Phy register in the channel based on DDR type.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Level       - DDRIO level to access.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval Nothing.
**/
void
MrcGetSetUpdatePhyChannels (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Dimm,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      UINT32        const   FreqIndex,
  IN      GSM_LT        const   Level,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN      INT64         *const  Value
  )
{
  UINT8             Ddr5SideEffect[1] = { 1 };
  UINT8             SideEffectLoop;
  UINT32            SideEffectController;
  UINT32            SideEffectChannel;
  UINT32            MaxChannel;
  MrcOutput        *Outputs;

  Outputs = &MrcData->Outputs;
  MaxChannel = Outputs->MaxChannels;

  switch (Group) {
      // MCMISCS_WRITECFGCHx_REG
    case TxDqFifoWrEnTcwlDelay:
    case TxDqFifoRdEnTcwlDelay:
      switch (Outputs->DdrType) {
        case MRC_DDR_TYPE_DDR5:
          for (SideEffectLoop = 0; SideEffectLoop < sizeof(Ddr5SideEffect); SideEffectLoop++) {
            SideEffectController = 0;
            SideEffectChannel = Controller * MAX_CHANNEL;
            SideEffectChannel += Channel * (MAX_CHANNEL / MaxChannel) + Ddr5SideEffect[SideEffectLoop];
            GetSet (MrcData, Socket, SideEffectController, SideEffectChannel, Dimm, Rank, Strobe, Bit, FreqIndex, Level, Group, Mode, Value);
          }
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/*
  This function perform any side effect action required from the Group that was just set.
  It will inherit the same Mode parameter so it passed it on to the actions executed.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      SubChannel  - DDR SubChannel number within a Channel (0-Based).
  @param[in]      Dimm        - DIMM Number within the DDR Channel (0-based). Ignored as Rank is rank number in the channel.
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Level       - DDRIO level to access.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval Nothing.
**/
void
MrcGetSetSideEffect (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   SubChannel,
  IN      UINT32        const   Dimm,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      UINT32        const   FreqIndex,
  IN      GSM_LT        const   Level,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN      INT64         *const  Value
  )
{
  //@todo BOOLEAN   Flush;
  GSM_GT    WriteGroup;
  GSM_GT    OffsetGroup;
  GSM_GT    PiGroup;
  INT64     GetSetVal;
  INT64     GetSetVal2;
  INT64     GetSetValOffset;
  INT64     GetSetValOffset2;
  UINT32    LocalRank;
  UINT32    LocalModeWrite;
  UINT32    LocalModeRead;
  UINT32    SearchVal;
  UINT32    SearchVal2;
  UINT32    DriftPI;
  BOOLEAN   Gear4;
  BOOLEAN   IsLpddr5;

  Gear4  = MrcData->Outputs.GearMode ? 1 : 0;
  IsLpddr5 = MrcData->Outputs.IsLpddr5;

  // Maximum drift due to periodic retraining, assume 70ps for both LP5 and DDR5
  // Convert to PI ticks
  DriftPI = UDIVIDEROUND (70 * 64, MrcData->Outputs.UIps);

  // Read from cache or register based on the write Mode to the group
  LocalModeRead = GSM_READ_ONLY;
  if (Mode & GSM_READ_CSR) {
    LocalModeRead |= GSM_READ_CSR;
  }
  // Write based on original mode. Clear offset write flag
  LocalModeWrite = Mode;
  LocalModeWrite &= ~GSM_WRITE_OFFSET;

  if ((Mode & GSM_UPDATE_CACHE) == 0) {
    // If caller did not update cache, then read from the CR
    LocalModeRead  |= GSM_READ_CSR;
    // If original mode is not using the cache, the side effect write should not use it as well
    LocalModeWrite |= GSM_READ_CSR;
  }
  if ((Group == RecEnDelay) || (Group == RecEnOffset) || (Group == TxDqsDelay) || (Group == TxDqsOffset) || (Group == TxDqDelay) || (Group == TxDqOffset)) {
    // Find the smallest RecEnDelay/TxDqsDelay/TxDqDelay in the Byte/Channel and update the new [Rx,Tx]RankMuxDelay
    // Calibration steps may update pi codes regardless of population, do not update [Rx, Tx]RankMuxDelay if channel is not enabled
    if ((Group == RecEnDelay) || (Group == RecEnOffset)) {
      WriteGroup = RxRankMuxDelay;
      OffsetGroup = RecEnOffset;
      PiGroup = RecEnDelay;
      SearchVal = MRC_UINT32_MAX;
      for (LocalRank = 0; LocalRank < MAX_RANK_IN_CHANNEL; LocalRank++) {
        if (MrcRankExist (MrcData, Controller, Channel, LocalRank)) {
          MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, LocalRank, Strobe, Bit, FreqIndex, Level, OffsetGroup, LocalModeRead, &GetSetValOffset);
          MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, LocalRank, Strobe, Bit, FreqIndex, Level, PiGroup, LocalModeRead, &GetSetVal);
          GetSetVal = GetSetVal + GetSetValOffset;
          SearchVal = MIN (SearchVal, (UINT32) GetSetVal);
        }
      }
      if (SearchVal != MRC_UINT32_MAX) {
        // Rounded down to QCLK
        SearchVal = SearchVal >> (7 + Gear4);
        GetSetVal = SearchVal;
        MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, 0, Strobe, Bit, FreqIndex, Level, WriteGroup, LocalModeWrite, &GetSetVal);
      }

    } else { // TxDqDelay, TxDqOffset, TxDqsDelay, TxDqsOffset
      // Avoid overwrite R2RDelay after Rank to Rank Training Done
      if (!MrcData->Outputs.IsR2RDone) {
        SearchVal = MRC_UINT32_MAX;
        SearchVal2 = IsLpddr5 ? 0 : MRC_UINT32_MAX;   // Do not look at TxDqs in LP5
        for (LocalRank = 0; LocalRank < MAX_RANK_IN_CHANNEL; LocalRank++) {
          if (MrcRankExist (MrcData, Controller, Channel, LocalRank)) {
            MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, LocalRank, Strobe, Bit, FreqIndex, Level, TxDqOffset, LocalModeRead, &GetSetValOffset);
            MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, LocalRank, Strobe, Bit, FreqIndex, Level, TxDqDelay, LocalModeRead, &GetSetVal);
            GetSetVal = GetSetVal + GetSetValOffset;
            SearchVal = MIN (SearchVal, (UINT32) GetSetVal); // TxDq
            if (!IsLpddr5) {
              MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, LocalRank, Strobe, Bit, FreqIndex, Level, TxDqsOffset, LocalModeRead, &GetSetValOffset2);
              MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, LocalRank, Strobe, Bit, FreqIndex, Level, TxDqsDelay, LocalModeRead, &GetSetVal2);
              GetSetVal2 = GetSetVal2 + GetSetValOffset2;
              SearchVal2 = MIN (SearchVal2, (UINT32) GetSetVal2); // TxDqs
            }
          }
        }
        if (!(SearchVal == MRC_UINT32_MAX || SearchVal2 == MRC_UINT32_MAX)) {
          // Account for periodic TxDqDqs Retraining which may reduce Tx DQ PI
          if (SearchVal > DriftPI) {
            SearchVal -= DriftPI;
          }
          // Rounded down to QCLK
          SearchVal = SearchVal >> (7 + Gear4); // Min TxDq
          SearchVal2 = SearchVal2 >> (7 + Gear4); // Min TxDqs
          GetSetVal = IsLpddr5 ? SearchVal : MIN (SearchVal, SearchVal2); // RankMux Delay is the min of the TxDQ/DQS across ranks, rounded down to QClk (DQS is not used in LP5)
          MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, 0, Strobe, Bit, FreqIndex, Level, TxRankMuxDelay, LocalModeWrite, &GetSetVal);
          if (!IsLpddr5) {
            GetSetVal2 = SearchVal - GetSetVal; // DQS is earlier, DQ is updated with the difference
            MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, 0, Strobe, Bit, FreqIndex, Level, TxR2RDqPi, LocalModeWrite, &GetSetVal2);
            MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, 0, Strobe, Bit, FreqIndex, Level, TxR2RDqEq, LocalModeWrite, &GetSetVal2);
            GetSetVal2 = SearchVal2 - GetSetVal; // DQ is earlier, DQS is updated with the difference
            MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, 0, Strobe, Bit, FreqIndex, Level, TxR2RDqsPi, LocalModeWrite, &GetSetVal2);
            MrcGetSet (MrcData, Socket, Controller, Channel, Dimm, 0, Strobe, Bit, FreqIndex, Level, TxR2RDqsEq, LocalModeWrite, &GetSetVal2);
          }
        }
      }
    }
  }
}

/**
  This function handles accessing and updating complex parameter values.
  This function will adjust the signal for that scope.  It will Not adjust other relative signals.
    Any shared parameter will need to be accounted for outside this function.
  The way each complex parameter is composed is project specific.
  The units of each parameter is specified in the parameter declaration in GSM_GT.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Socket      - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      SubChannel  - DDR SubChannel number within a Channel (0-Based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      FreqIndex   - Index supporting multiple operating frequencies.
  @param[in]      Group       - DDRIO group to access.
  @param[in]      Mode        - Bit-field flags controlling Get/Set.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

  @retval MrcStatus: mrcComplexParamDone - no more programming is needed, otherwise continue with GetSet flow.
**/
MrcStatus
MrcGetSetComplexParam (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Socket,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   SubChannel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   FreqIndex,
  IN      GSM_GT        const   Group,
  IN      UINT32                Mode,
  IN OUT  INT64         *const  Value
  )
{
  MrcStatus  Status;
  UINT32     LocalModeWrite;

  // Write based on original mode. Clear offset write flag
  LocalModeWrite = Mode;
  LocalModeWrite &= ~GSM_WRITE_OFFSET;
  if ((Mode & GSM_UPDATE_CACHE) == 0) {
    // If original mode is not using the cache, the side effect write should not use it as well
   //LocalModeWrite |= GSM_READ_CSR;
  }

  Status  = mrcSuccess;
  return Status;
}

/**
  Check if the input group is part of the CLK control fub.

  @param[in]      Group       - DDRIO group to access.

  @retval TRUE  - The input group is part of the CLK control fub.
  @retval FALSE - The input group is not part of the CLK control fub.
**/
BOOLEAN
IsClkGrp (
  IN      GSM_GT        const   Group
  )
{
  BOOLEAN IsInputClkGrp;

  if ((Group == GsmClkDccRankEn)) {
    IsInputClkGrp = TRUE;
  } else {
    IsInputClkGrp = FALSE;
  }
  return IsInputClkGrp;
}

/**
  Check if the input group is a per-bit control fub.

  @param[in] Group - DDRIO group to access.

  @retval TRUE  - The input group is a per-bit control fub.
  @retval FALSE - The input group is not a per-bit control fub.
**/
BOOLEAN
IsPerBitGrp (
  IN GSM_GT const Group
  )
{
  BOOLEAN IsInputPerBit;

  if ((Group == TxDqBitDelay) ||
      (Group == RxDqsBitDelay) ||
      (Group == RxDqsBitOffset) ||
      (Group == RxDqVref) ||
      (Group == RxVocRise)) {
    IsInputPerBit = TRUE;
  } else {
    IsInputPerBit = FALSE;
  }
  return IsInputPerBit;
}

/**
  Converting controller, channel, rank, strobe number from System to IP register architecture.

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - Dqs data group within the rank or the command group (0-based).
  @param[in]      Lane        - Lane index within the GSM_GT group (0-based).
  @param[in]      Group       - DDRIO group to access.

  @retval MrcStatus
**/
MrcStatus
MrcTranslateSystemToIp (
  IN      MrcParameters *const  MrcData,
  IN OUT  UINT32        *const  Controller,
  IN OUT  UINT32        *const  Channel,
  IN OUT  UINT32        *const  Rank,
  IN OUT  UINT32        *const  Strobe,
  IN OUT  UINT32        *const  Lane,
  IN      GSM_GT        const   Group
  )
{
  MrcOutput   *Outputs;
  MrcStatus   Status;
  UINT32      TransController;
  UINT32      TransChannel;
  UINT32      TransRank;
  UINT32      TransStrobe;
  UINT32      TransLane;
  BOOLEAN     IsDdr5;
  BOOLEAN     IsCccNil;

  Outputs = &MrcData->Outputs;
  IsDdr5 = Outputs->IsDdr5;
  Status  = mrcSuccess;
  IsCccNil = (MrcData->Inputs.ExtInputs.Ptr->CccPinsInterleaved) ? 0 : 1;

  // Initialize the translated variables with the input. This gives us a code optimization
  // where we don't have to assign in every Group that doesn't use that scope.
  TransController = *Controller;
  TransChannel    = *Channel;
  TransRank       = *Rank;
  TransStrobe     = *Strobe;
  TransLane       = *Lane;

  switch (Group) {
    // MCMISCS_WRITECFGCHx_REG
    case TxDqFifoWrEnTcwlDelay:
    case TxDqFifoRdEnTcwlDelay:
    // CCC_CR_xxx_REG
      TransController = MRC_IGNORE_ARG;
      TransChannel = (*Controller * MAX_CHANNEL);
      TransChannel += (*Channel * (MAX_CHANNEL / Outputs->MaxChannels));
      break;

    // CCCSHARED
    case DefDrvEnLow:
      TransController = MRC_IGNORE_ARG;
      TransChannel = (*Controller * MAX_CHANNEL_SHARE_REGS);
      TransChannel += (*Channel * MAX_CHANNEL_SHARE_REGS / Outputs->MaxChannels);
      break;

    // CCC_CR_DCCFSMCONTROL
    case GsmClkDccRankEn:
    // CCC_CR_DDRCRCCCPIxxx_REG
    case CmdGrpPi:
      TransController = MRC_IGNORE_ARG;
      TransChannel = (*Controller * MAX_CHANNEL);
      TransChannel += (*Channel * (MAX_CHANNEL / Outputs->MaxChannels));
      if (IsDdr5) {
        // MC0 CH0 CLK0 - CH0CCC
        // MC0 CH0 CLK1 - CH1CCC
        // MC0 CH1 CLK0 - CH2CCC
        // MC0 CH1 CLK1 - CH3CCC
        // MC1 CH0 CLK0 - CH4CCC
        // MC1 CH0 CLK1 - CH5CCC
        // MC1 CH1 CLK0 - CH6CCC
        // MC1 CH1 CLK1 - CH7CCC
        if (IsClkGrp (Group)) {
          TransChannel += *Rank;
        } else {
          TransChannel += *Strobe;
        }
      }
      break;

    case CtlGrpPi:
      TransController = MRC_IGNORE_ARG;
      TransChannel = *Controller * MAX_CHANNEL;
      if (IsDdr5) {
        //            IL       NIL
        // MC0 CH0  - CH0CCC - CH1CCC
        // MC0 CH1  - CH2CCC - CH3CCC
        // MC1 CH0  - CH5CCC - CH5CCC
        // MC1 CH1  - CH7CCC - CH7CCC
        TransChannel += (*Channel * 2);
        TransChannel += (IsCccNil || *Controller > 0) ? 1 : 0;
      } else { // LPDDR5
        TransChannel += (*Channel);
      }
      break;

    // DDRDATAxCHy_REG
    case GsmIocDqsMaskPulseCnt:
    case GsmIocDqsPulseCnt:
    case GsmIocDqOverrideData:
    case GsmIocDqOverrideEn:
    case GsmIocDataInvertNibble:
    case GsmIocForceRxAmpOn:
    case GsmIocForceOdtOn:
    case GsmIocTxPiPwrDnDis:
    case GsmIocInternalClocksOn:
    case GsmIocDataDqsOdtParkMode:
    case GsmIocRankOverrideEn:
    case GsmIocRankOverrideVal:
    case GsmIocDisableTxDqs:
    case RxDqsBitDelay:
    case RxDqsBitOffset:
    case TxDqBitDelay:
    case RxVocRise:
    case RxVref:
    case RxDqVref:
    case RecEnDelay:
    case RxDqsPDelay:
    case RxDqsNDelay:
    case SenseAmpDelay:
    case SenseAmpDuration:
    case McOdtDelay:
    case McOdtDuration:
    case RxRankMuxDelay:
    case TxRankMuxDelay:
    case TxR2RDqsEq:
    case TxR2RDqEq:
    case TxR2RDqsPi:
    case TxR2RDqPi:
    case RxR2RDqsOff:
    case RxR2RDqOff:
    case RxR2RRxPi:
    case RxR2RRcvEn:
    case TxDqsDelay:
    case TxDqDelay:
    case RecEnOffset:
    case RxDqsOffset:
    case RxVrefOffset:
    case TxDqsOffset:
    case TxDqOffset:
    case WrRetrainDeltaPiCode:
    case RdRetrainDeltaPiCode:
    case RxCompDqsDelayP:
    case RxCompDqsDelayN:
    case RxCompDqsOffset:
    case GsmDataDccRankEn:
    case GsmIocRetrainSwizzleCtlRetrainEn:
    case GsmIocRetrainSwizzleCtlSerialMrr:
    case GsmIocRetrainInitPiCode:
    case GsmIocRetrainRoCount:
    case GsmIocRetrainCtlInitTrain:
    case GsmIocRetrainCtlDuration:
    case GsmIocRetrainCtlResetStatus:
      // Pass in Controller/Channel/Byte values to determine the logical offset to access
      // TransChannel and TransStrobe are translated values pointing to the correct offset to access
      //
      //            LPDDR
      //   Controller   Channel   Byte0     Byte1
      // |            |    0     DATA0CH0  DATA0CH1
      // |     0      |    1     DATA1CH0  DATA1CH1
      // |            |    2     DATA2CH0  DATA2CH1
      // |            |    3     DATA3CH0  DATA3CH1
      // |____________|______________________________
      // |            |
      // |            |    4     DATA4CH0  DATA4CH1
      // |     1      |    5     DATA5CH0  DATA5CH1
      // |            |    6     DATA6CH0  DATA6CH1
      // |            |    7     DATA7CH0  DATA7CH1
      //
      //             DDR5
      //   Controller   Channel   Byte0     Byte1     Byte2     Byte3     Byte4
      // |     0      |    0     DATA0CH0  DATA0CH1  DATA1CH0  DATA1CH1  DATA8CH0
      // |            |    1     DATA2CH0  DATA2CH1  DATA3CH0  DATA3CH1  DATA8CH1
      // |____________|____________________________________________________________
      // |            |
      // |     1      |    2     DATA4CH0  DATA4CH1  DATA5CH0  DATA5CH1  DATA9CH0
      // |            |    3     DATA6CH0  DATA6CH1  DATA7CH0  DATA7CH1  DATA9CH1
      //
      // Strobe is [Byte0, Byte1, .., ByteN] based on DddrType
      // DATA[TransStrobe]CH[TransChannel]
      //ByteCount will vary based on DdrType
      TransController = MRC_IGNORE_ARG;
      TransChannel = *Strobe % 2;
      if (IsDdr5) {
        TransStrobe = ((*Controller * Outputs->MaxChannels) + *Channel) * Outputs->MaxChannels;
        TransStrobe += *Strobe / 2;
      } else { // LPDDR5
        TransStrobe = (*Controller * MAX_CHANNEL) + *Channel;
      }
      if (IsPerBitGrp (Group)) {
        TransLane = TranslateDqBitThroughPhyDqSwizzling (*Strobe, *Lane);
      }
      break;

    case GsmMccRrdValidTrigger:
    case GsmMccRrdValidOverflow:
    case GsmMccRrdValidValue:
    case GsmMccRrdValidSign:
    case GsmMccDeswizzleByte:
    case GsmMccDeswizzleBit:
    case GsmMccMrrResult:
      // These belong to MC group, but we need to keep system-level channel for subch indexing (like RoundTripDelay)
      break;

      // DATA_SHARED
      // DQ HW Instance    |   LP5   | DDR NIL | DDR5 IL | RAL Name
      // ------------------|---------|---------|---------|----------
      //       DATA0       | CH0/CH1 |   CH0   | CH0/CH1 | ddrdata_shared0
      //       DATA1       | CH0/CH1 |   CH0   | CH0/CH1 | ddrdata_shared1
      //       DATA2       | CH2/CH3 |   CH1   | CH0/CH1 | ddrdata_shared2
      //       DATA3       | CH2/CH3 |   CH1   | CH0/CH1 | ddrdata_shared3
      //       DATA4       | CH4/CH5 |   CH2   | CH2/CH3 | ddrdata_shared4
      //       DATA5       | CH4/CH5 |   CH2   | CH2/CH3 | ddrdata_shared5
      //       DATA6       | CH6/CH7 |   CH3   | CH2/CH3 | ddrdata_shared6
      //       DATA7       | CH6/CH7 |   CH4   | CH2/CH3 | ddrdata_shared7
      //
      // System - LP5          PHY (IP)
      //------------------------------
      // Controller: 0..1
      // Channel     0..3        0..7
      // Strobe:     0..1        0..1

    case WckGrpPi:
      TransController = MRC_IGNORE_ARG;
      TransChannel    = *Controller * MAX_CHANNEL;
      TransChannel   += *Channel;
      break;

    // DDRDATA_SHAREDx_CR_WCKDCCFSMCONTROL_REG
    case GsmWckDccRankEn:
    case GsmWckDccLaneEn:
      TransController = MRC_IGNORE_ARG;
      TransChannel = (*Controller * MAX_CHANNEL);
      TransChannel += (*Channel * (MAX_CHANNEL / Outputs->MaxChannels));
      break;

    case GsmCkdPllMode:
    case GsmCkdIctInputClkTerm:
      // Belongs to CKD group, only do translation for DIMM#
      TransController = MRC_IGNORE_ARG;
      TransChannel = MRC_IGNORE_ARG;
      TransLane = MRC_IGNORE_ARG;
      TransRank = MRC_IGNORE_ARG;
      MrcCalcCkdDimmPin (MrcData, *Controller, *Channel, *Rank, &TransStrobe, NULL);
      break;

    default:
      if ((Group > EndOfIocMarker) && (Group < EndOfMccMarker)) {
        // This is to cover all MCT and MCC enums
        if (Outputs->IsLpddr5) {
          if (*Channel != MRC_IGNORE_ARG) {
            // Always force writes to this register to the MC channel number for LP.
            TransChannel = *Channel / 2;
            if (*Channel % 2) {
              Status = mrcInstanceDoesNotExist;
              MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR,
                              "GetSet called with Ch of 1/3 when LPDDR, Group %s(%d), Ch%d\n",
                              GsmGtDebugStrings[Group], Group, *Channel);
            }
          }
        }
      } else if ((Group > EndOfPmaMarker) && (Group < EndOfCkdMarker)) {
        // Pass in Controller/Channel,Rank values to determine the physical DIMM/offset/field to access
        // TransChannel/TransRank and TransStrobe are translated values pointing to the correct DIMM/offset/field to access
        TransController = MRC_IGNORE_ARG;
        TransChannel = MRC_IGNORE_ARG;
        TransLane = MRC_IGNORE_ARG;
        MrcCalcCkdDimmPin (MrcData, *Controller, *Channel, *Rank, &TransStrobe, &TransRank);
      }
      break;

  }

  *Controller = TransController;
  *Channel = TransChannel;
  *Rank = TransRank;
  *Strobe = TransStrobe;
  *Lane = TransLane;

  return Status;
}

/**
  This function returns CKD Control Word register limits.
  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - PMA group to access.
  @param[in]  Rank      - Rank number within a channel (0-based).
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.
  @retval MrcStatus
**/
MrcStatus
MrcGetCkdGroupLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  IN  UINT32  const   Rank,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  )
{
  MrcStatus Status;
  MrcDebug  *Debug;
  INT64     Max;
  INT64     Min;
  UINT32    Wait;

  Debug   = &MrcData->Outputs.Debug;
  Status  = mrcSuccess;
  Wait    = 0;
  Min     = MRC_INT64_MIN;
  Max     = MRC_INT64_MAX;

  switch (Group) {
    case GsmCkdQckClkDisable:
    case GsmCkdQckOutputDelayEnable:
      Min = 0;
      Max = 1;
      break;

    case GsmCkdPllMode:
    case GsmCkdIctInputClkTerm:
      Min = CKD_CONFIGURATION_PllMode_MIN;
      Max = CKD_CONFIGURATION_PllMode_MAX;
      break;

    case GsmCkdQckDrive:
      Min = CKD_QCK_SIGNALS_DRIVER_Qck0ADrive_MIN;
      Max = CKD_QCK_SIGNALS_DRIVER_Qck0ADrive_MAX;
      break;

    case GsmCkdQckSlewRate:
      Min = CKD_QCK_SLEW_RATE_Qck01ASlewRate_MIN;
      Max = CKD_QCK_SLEW_RATE_Qck01ASlewRate_MAX;
      break;

    case GsmCkdQckOutputDelay:
      Min = CKD_QCK0_OUTPUT_DELAY_RANGE_QckOutputDelay_MIN;
      Max = CKD_QCK0_OUTPUT_DELAY_RANGE_QckOutputDelay_MAX;
      break;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Group %s(%d) has no limits defined\n", GsmGtDebugStrings[Group], Group);
      Status = mrcWrongInputParameter;
      break;
  }

  // Null guard the pointers
  if (MinVal != NULL) {
    *MinVal = Min;
  }
  if (MaxVal != NULL) {
    *MaxVal = Max;
  }
  if (WaitTime != NULL) {
    *WaitTime = Wait;
  }

  return Status;
}

/**
  Check if the given GSM_GT group key is skipable for reasons of writing to
  channels 1/3.

  @param[in]  Group  - DDRIO group to access.

  @retval IsSkipGroup - TRUE, if within a GSM_GT skipable range
                      - FALSE, otherwise
**/
BOOLEAN
MrcGsmGtSkip (
  IN  GSM_GT  const  Group
)
{
  BOOLEAN  IsSkipGroup;

  // Check if we are a MC Group. If so, then we want to skip writes to channel 1/3
  if ((Group > EndOfIocMarker) && (Group < EndOfMccMarker)) {
    IsSkipGroup = TRUE;
  } else {
    IsSkipGroup = FALSE;
  }

  switch (Group) {
    case GsmMccRrdValidTrigger:
    case GsmMccRrdValidOverflow:
    case GsmMccRrdValidValue:
    case GsmMccRrdValidSign:
      // We are using system-level channel for these items, because the register fields are per subch in LP5 (like RoundTripDelay)
      // Hence do not skip channels 1 and 3
      IsSkipGroup = FALSE;
      break;
    default:
      break;
  }

  return IsSkipGroup;
}

/**
  Routing function to retrieve the correct Hash and Volatile Mask for a GSM_GT group instance.

  @param[in]      MrcData       - Pointer to global data structure.
  @param[in]      Socket        - Processor socket in the system (0-based).  Not used in Core MRC.
  @param[in]      Controller    - Memory Controller Number within the processor (0-Based).
  @param[in]      Channel       - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank          - Rank number within a channel (0-based).
  @param[in]      Strobe        - Dqs data group within the rank (0-based).
  @param[in]      Lane          - Lane index within the GSM_GT group (0-based).
  @param[in]      FreqIndex     - Index supporting multiple operating frequencies.
  @param[in]      Group         - DDRIO group to access.
  @param[out]     HashVal       - Pointer to return the MMIO access instruction.
  @param[out]     VolatileMask  - Pointer to return the Register volatile mask.

  @retval MrcStatus - mrcSuccess, the hash was sucessfully retreived
                    - mrcInstanceDoesNotExist, otherwise
**/
MrcStatus
MrcGetHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   Rank,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
)
{
  MrcStatus  Status;
  UINT8      Index;

  Status = mrcInstanceDoesNotExist;

  for (Index = 0; Index < ARRAY_COUNT (MrcGetSetLoopControl); Index++) {
    if (Group < MrcGetSetLoopControl[Index].EndMarker) {
      Status = MrcGetSetLoopControl[Index].MrcGetHashFunc (MrcData, Socket, Controller, Channel, Rank, Strobe, Lane, FreqIndex, Group, HashVal, VolatileMask);
      break;
    }
  }

  return Status;
}

/**
  This function returns complex group limits.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - Complex group to access.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetComplexGroupLimits (
  IN  MrcParameters *const  MrcData,
  IN  UINT32        const   Group,
  OUT INT64         *const  MinVal,
  OUT INT64         *const  MaxVal,
  OUT UINT32        *const  WaitTime
  )
{
  MrcStatus Status;
#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  MrcDebug  *Debug;
#endif
  INT64     Min;
  INT64     Max;
  UINT32    Wait;

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Debug   = &MrcData->Outputs.Debug;
#endif
  Wait    = 0;
  Min     = MRC_INT64_MIN;
  Max     = MRC_INT64_MAX;

  switch (Group) {
    case GsmComplexRcvEn:
      // Resolve C4065: switch statement contians 'default' but no 'case' labels.
      // We currently don't have any complex GSM_GT's enabled/supported; but want to maintain the infrastructure.
    default:
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Group %s(%d) has no limits defined\n", GsmGtDebugStrings[Group], Group);
      Status = mrcWrongInputParameter;
      break;
  }

  // Null guard the pointers
  if (MinVal != NULL) {
    *MinVal = Min;
  }
  if (MaxVal != NULL) {
    *MaxVal = Max;
  }
  if (WaitTime != NULL) {
    *WaitTime = Wait;
  }

  return Status;
}

/**
  This function returns the hash used to execute the Get/Set function.
  The hash consists of: Register Offset, BitField start bit, BitField length.
  @param[in]  MrcData       - Pointer to global data.
  @param[in]  Socket        - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]  Controller    - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]  Channel       - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]  CkdPin        - CKD output number within a CKD device (0-based).
  @param[in]  Strobe        - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]  Lane          - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]  FreqIndex     - Unused here, but needed so we can use a pointer to this function or a different function
  @param[in]  Group         - DDRIO group to access.
  @param[out] HashVal       - Pointer to return the MMIO access instruction.
  @param[out] VolatileMask  - Unused here, but needed so we can use a pointer to this function or a different function
  @retval MrcStatus
**/
MrcStatus
MrcGetCkdHash (
  IN   MrcParameters *const  MrcData,
  IN   UINT32        const   Socket,
  IN   UINT32        const   Controller,
  IN   UINT32        const   Channel,
  IN   UINT32        const   CkdPin,
  IN   UINT32        const   Strobe,
  IN   UINT32        const   Lane,
  IN   UINT32        const   FreqIndex,
  IN   GSM_GT        const   Group,
  OUT  UINT32        *const  HashVal,
  OUT  UINT64_STRUCT *const  VolatileMask
  )
{
  MrcStatus Status;
#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  MrcDebug  *Debug;
#endif
  MRC_REGISTER_HASH_STRUCT *HashPtr;
  UINT32 CrOffset;

  static const UINT32 QckClkDisableHsh[MAX_CKD_PIN] = {
    CKD_CONFIGURATION_DisQck0AClk_HSH, CKD_CONFIGURATION_DisQck1AClk_HSH,
    CKD_CONFIGURATION_DisQck0BClk_HSH, CKD_CONFIGURATION_DisQck1BClk_HSH
  };
  static const UINT32 QckOutputDelayEnableHsh[MAX_CKD_PIN] = {
    CKD_OUTPUT_DELAY_Qck0AOutputDelay_HSH, CKD_OUTPUT_DELAY_Qck1AOutputDelay_HSH,
    CKD_OUTPUT_DELAY_Qck0BOutputDelay_HSH, CKD_OUTPUT_DELAY_Qck1BOutputDelay_HSH
  };
  static const UINT32 QckDriveHsh[MAX_CKD_PIN] = {
    CKD_QCK_SIGNALS_DRIVER_Qck0ADrive_HSH, CKD_QCK_SIGNALS_DRIVER_Qck1ADrive_HSH,
    CKD_QCK_SIGNALS_DRIVER_Qck0BDrive_HSH, CKD_QCK_SIGNALS_DRIVER_Qck1BDrive_HSH
  };
  static const UINT32 QckSlewRateHsh[CKD_QCK_SHARE] = {
    CKD_QCK_SLEW_RATE_Qck01ASlewRate_HSH, CKD_QCK_SLEW_RATE_Qck01BSlewRate_HSH
  };

#if (defined (MRC_HAL_DEBUG) && (MRC_HAL_DEBUG == SUPPORT))
  Debug   = &MrcData->Outputs.Debug;
#endif
  Status  = mrcSuccess;
  HashPtr = (MRC_REGISTER_HASH_STRUCT *) HashVal;
  HashPtr->Data = MRC_UINT32_MAX;
  CrOffset = MRC_UINT32_MAX;

  switch (Group) {
    case GsmCkdPllMode:
      HashPtr->Data = CKD_CONFIGURATION_PllMode_HSH;
      break;

    case GsmCkdIctInputClkTerm:
      HashPtr->Data = CKD_CONFIGURATION_IctInputClkTermination_HSH;
      break;

    case GsmCkdQckClkDisable:
      HashPtr->Data = QckClkDisableHsh[CkdPin];
      break;

    case GsmCkdQckOutputDelayEnable:
      HashPtr->Data = QckOutputDelayEnableHsh[CkdPin];
      break;

    case GsmCkdQckDrive:
      HashPtr->Data = QckDriveHsh[CkdPin];
      break;

    case GsmCkdQckSlewRate:
      HashPtr->Data = QckSlewRateHsh[CkdPin / CKD_QCK_SHARE];
      break;

    case GsmCkdQckOutputDelay:
      HashPtr->Data = CKD_QCK0_OUTPUT_DELAY_RANGE_QckOutputDelay_HSH;
      break;

    default:
      MRC_HAL_DEBUG_MSG (Debug, MSG_LEVEL_HAL, "Hash undefined. Group %s(%d) CkdPin %u\n", GsmGtDebugStrings[Group], Group, CkdPin);
      HashPtr->Data = MRC_UINT32_MAX;
      Status = mrcWrongInputParameter;
      break;
  }

  if (Status == mrcSuccess) {
    if (HashPtr->Data != MRC_UINT32_MAX) {
      CrOffset = MrcGetCkdRegOffset (Group, CkdPin);
    }
  }

  HashPtr->Bits.Offset = CrOffset;

  return Status;
}

///
/// Public Functions
///
/**
  This function is the interface for the core of the MRC to get the limits of a
  specific GSM_GT parameter.

  @param[in]  MrcData   - Pointer to global data.
  @param[in]  Group     - DDRIO group to access.
  @param[in]  Rank      - Rank number within a channel (0-based).
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetSetLimits (
  IN  MrcParameters *const MrcData,
  IN  GSM_GT  const   Group,
  IN  UINT32  const   Rank,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  )
{
  MrcStatus Status;

  Status = mrcSuccess;

  if (Group < EndOfPhyMarker) {
    MrcGetDdrIoGroupLimits (MrcData, Group, Rank, MinVal, MaxVal, WaitTime);
  } else if (Group < EndOfIocMarker) {
    MrcGetDdrIoCfgGroupLimits (MrcData, Group, Rank, MinVal, MaxVal, WaitTime);
  } else if (Group < EndOfMctMarker) {
    MrcGetMcTimingGroupLimits (MrcData, Group, MinVal, MaxVal, WaitTime);
  } else if (Group < EndOfMccMarker) {
    MrcGetMcConfigGroupLimits (MrcData, Group, MinVal, MaxVal, WaitTime);
  } else if (Group < EndOfPmaMarker) {
    MrcGetPmaGroupLimits (MrcData, Group, MinVal, MaxVal, WaitTime);
  } else if (Group < EndOfCkdMarker) {
    MrcGetCkdGroupLimits (MrcData, Group, Rank, MinVal, MaxVal, WaitTime);
  } else {
    MrcGetComplexGroupLimits (MrcData, Group, MinVal, MaxVal, WaitTime);
  }
  return Status;
}

/**
  This function is the interface for the core of the MRC to get the limits of RxDqVref

  @param[in]  MrcData   - Pointer to global data.
  @param[out] MinVal    - Return pointer for Minimum Value supported.
  @param[out] MaxVal    - Return pointer for Maximum Value supported.
  @param[out] WaitTime  - Return pointer for settle time required in microseconds.

  @retval MrcStatus - mrcSuccess if the parameter is found, otherwise mrcFail.
**/
MrcStatus
MrcGetSetRxVrefLimits (
  IN  MrcParameters *const MrcData,
  OUT INT64   *const  MinVal,
  OUT INT64   *const  MaxVal,
  OUT UINT32  *const  WaitTime
  )
{
  MrcGetSetLimits (MrcData, RxDqVref, 0, MinVal, MaxVal, NULL);
  return mrcSuccess;
}

/**
  Update MRC Host Data Structure when GSM_UPDATE_HOST is set
  Currently only used for RxDqsBitDelay and TxDqBitDelay

  @param[in]      MrcData     - Pointer to global data structure.
  @param[in]      Controller  - Memory Controller Number within the processor (0-based).
  @param[in]      Channel     - DDR Channel Number within the processor socket (0-based).
  @param[in]      Rank        - Rank number within a channel (0-based).
  @param[in]      Strobe      - If Group is a CMD/CTL/CLK Index type, this is the index for that signal.  Otherwise, Dqs data group within the rank (0-based).
  @param[in]      Bit         - Bit index within the data group (0-based).
  @param[in]      Group       - DDRIO group to access.
  @param[in,out]  Value       - Pointer to value for Get/Set to operate on.  Can be offset or absolute value based on mode.

**/
void
MrcGetSetUpdateHost (
  IN      MrcParameters *const  MrcData,
  IN      UINT32        const   Controller,
  IN      UINT32        const   Channel,
  IN      UINT32        const   Rank,
  IN      UINT32        const   Strobe,
  IN      UINT32        const   Bit,
  IN      GSM_GT        const   Group,
  IN OUT  INT64         *const  Value
  )
{
  MrcOutput          *Outputs;
  MrcControllerOut   *ControllerOut;
  MrcChannelOut      *ChannelOut;

  Outputs = &MrcData->Outputs;
  ControllerOut = &Outputs->Controller[Controller];
  ChannelOut = &ControllerOut->Channel[Channel];

  switch (Group) {
    case RxDqsBitDelay:
      ChannelOut->RxDqPb[Rank][Strobe][Bit].Center = (UINT8) *Value;
      break;

    case TxDqBitDelay:
      ChannelOut->TxDqPb[Rank][Strobe][Bit].Center = (UINT8) *Value;

    default:
      break;
  }

}

/**
  This function calculates values by dividing 2 if needed.

  @param[in]  MrcData       - Pointer to global data.
  @param[in]  Value         - Value
  @return     Result.
**/
INT64
MrcGetDividedValueTck (
  IN    MrcParameters *const  MrcData,
  IN    INT64                 Value
  )
{
  return Value;
}

/**
  This function programs WCK DCC registers to work in a cross-platform manner

  @param[in] MrcData     - Include all MRC global data.
  @param[in] GetSetGroup - WCK DCC GetSet Group
  @param[in] GetSetVal   - WCK DCC Value
**/
VOID
MrcSetWckDccCrossProj (
  IN MrcParameters* const MrcData,
  IN GSM_GT               GetSetGroup,
  IN INT64                *GetSetVal
  )
{
  MrcGetSetMcCh (MrcData, MAX_CONTROLLER, MAX_CHANNEL, GetSetGroup, WriteCached, GetSetVal);
}

