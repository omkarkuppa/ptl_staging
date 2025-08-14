/** @file
  This file contains definitions required for creation of
  end-to-end check-the-checker test result hob.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _FUSA_INFO_HOB_H_
#define _FUSA_INFO_HOB_H_

#pragma pack (push, 1)

extern EFI_GUID gSiFusaInfoGuid;

/**
  FuSa Info HOB version
  Use this to compare to the HOB retrieved from the FSP for the
  exact match
**/
#define FUSA_INFO_VERSION 300

/** Constants used for FUSA_TEST_RESULT->CheckResults[] and
 *  FUSA_TEST_RESULT->TestResult */
/**@defgroup ResultConstant Check Result Constants*/
/**@{*/
#define FUSA_TEST_DEVICE_NOTAVAILABLE  0xFF    /**<device is not available*/
#define FUSA_TEST_NOTRUN               0x0U    /**<check is not run*/
#define FUSA_TEST_FAIL                 0xD2U   /**<check fail*/
#define FUSA_TEST_PASS                 0x2DU   /**<check pass*/
#define FUSA_TEST_NOT_SUPPORTED        0x55    /** Test Not Supported*/
/**@}*/

/** Fusa test result structure
 **/
typedef struct
{
  UINT32 TestNumber;            /**< test number assigned to this test */
  UINT32 TotalChecks;           /**< total number of checks in this test */
  UINT8  TestResult;            /**< if all tests passed then this is FUSA_TEST_PASS.
                                  if at least one check fails, then this is TEST_FAIL
                                  if the device (eg. MC channel DIMM) is not available
                                  then this is FUSA_TEST_DEVICE_NOTAVAILABLE.
                                  if the test has not been run, then this is FUSA_TEST_NOTRUN*/
  UINT8  ReservedByte[3];       /**< reserved, as padding for 4 byte-alignment */
  UINT8  CheckResults[32];      /**< test result for each check.*/
  UINT32 Crc32;                 /**< crc32 of the structure */
} FUSA_TEST_RESULT;


/**
  Fusa Test Number assigned to each Fusa test.
  This will be used for the unique id for each test.
  FUSA_TEST_RESULT->TestNumber will have this value.
**/
typedef enum
{
  FusaTestNumMc0Cmi = 0,           ///<Check MC0 CMI path, valid if there is DIMM using MC0
  FusaTestNumMc1Cmi,               ///<Check MC1 CMI path, valid if there is DIMM using MC1
  FusaTestNumMc0CmiCh0Data,        ///<Check MC0 CH0 CMI path, valid if there is DIMM using MC0 CH0
  FusaTestNumMc0CmiCh1Data,        ///<Check MC0 CH1 CMI path, valid if there is DIMM using MC0 CH1
  FusaTestNumMc0CmiCh2Data,        ///<Check MC0 CH2 CMI path, valid if there is DIMM using MC0 CH2
  FusaTestNumMc0CmiCh3Data,        ///<Check MC0 CH3 CMI path, valid if there is DIMM using MC0 CH3
  FusaTestNumMc1CmiCh0Data,        ///<Check MC1 CH0 CMI path, valid if there is DIMM using MC1 CH0
  FusaTestNumMc1CmiCh1Data,        ///<Check MC1 CH1 CMI path, valid if there is DIMM using MC1 CH1
  FusaTestNumMc1CmiCh2Data,        ///<Check MC1 CH2 CMI path, valid if there is DIMM using MC1 CH2
  FusaTestNumMc1CmiCh3Data,        ///<Check MC1 CH3 CMI path, valid if there is DIMM using MC1 CH3
  //FusaTestNumMc0CmiCh3Address,    ///<Check MC0 CMF CMI path, valid if there is DIMM using MC0
  //FusaTestNumMc1CmiCh4Address,    ///<Check MC1 CMF CMI path, valid if there is DIMM using MC1
  FusaTestNumIbecc0Cmi,            ///<Check Ibecc0 CMI path, valid if there is IBECC range covering MC0 DIMMs
  FusaTestNumIbecc1Cmi,            ///<Check Ibecc1 CMI path, valid if there is IBECC range covering MC1 DIMMs
  FusaTestNumIbecc0EccCorrError,   ///<Check Ibecc0 ECC correctable error, valid if there is IBECC range covering MC0 DIMMs
  FusaTestNumIbecc1EccCorrError,   ///<Check Ibecc1 ECC correctable error, valid if there is IBECC range covering MC1 DIMMs
  FusaTestNumIbecc0EccUncorrError, ///<Check Ibecc0 ECC uncorrectable error, valid if there is IBECC range covering MC0 DIMMs
  FusaTestNumIbecc1EccUncorrError, ///<Check Ibecc1 ECC uncorrectable error, valid if there is IBECC range covering MC1 DIMMs

  FusaTestNumMc0Mbist,            ///<Check MC0 MBIST
  FusaTestNumMc1Mbist,            ///<Check MC1 MBIST
  FusaTestNumMc0Ch0Mbist,         ///<Check MC0 CH0 MBIST
  FusaTestNumMc0Ch1Mbist,         ///<Check MC0 CH1 MBIST
  FusaTestNumMc0Ch2Mbist,         ///<Check MC0 CH2 MBIST
  FusaTestNumMc0Ch3Mbist,         ///<Check MC0 CH3 MBIST
  FusaTestNumMc1Ch0Mbist,         ///<Check MC1 CH0 MBIST
  FusaTestNumMc1Ch1Mbist,         ///<Check MC1 CH1 MBIST
  FusaTestNumMc1Ch2Mbist,         ///<Check MC1 CH2 MBIST
  FusaTestNumMc1Ch3Mbist,         ///<Check MC1 CH3 MBIST

  FusaTestNumIbecc0Mbist,
  FusaTestNumIbecc1Mbist,

  FusaTestNumArray0Bist,             ///<Check core0 Pbist, valid if there is core0 in the SKU
  FusaTestNumArray1Bist,             ///<Check core1 Pbist, valid if there is core1 in the SKU
  FusaTestNumArray2Bist,             ///<Check core2 Pbist, valid if there is core2 in the SKU
  FusaTestNumArray3Bist,             ///<Check core3 Pbist, valid if there is core3 in the SKU
  FusaTestNumArray4Bist,             ///<Check core4 Pbist, valid if there is core4 in the SKU
  FusaTestNumArray5Bist,             ///<Check core5 Pbist, valid if there is core5 in the SKU
  FusaTestNumArray6Bist,             ///<Check core6 Pbist, valid if there is core6 in the SKU
  FusaTestNumArray7Bist,             ///<Check core7 Pbist, valid if there is core7 in the SKU
  FusaTestNumArray8Bist,             ///<Check core8 Pbist, valid if there is core8 in the SKU
  FusaTestNumArray9Bist,             ///<Check core9 Pbist, valid if there is core9 in the SKU
  FusaTestNumArray10Bist,            ///<Check core10 Pbist, valid if there is core10 in the SKU
  FusaTestNumArray11Bist,            ///<Check core11 Pbist, valid if there is core11 in the SKU

  FusaTestNumScan0Bist,             ///<Check core0 Pbist, valid if there is core0 in the SKU
  FusaTestNumScan1Bist,             ///<Check core1 Pbist, valid if there is core1 in the SKU
  FusaTestNumScan2Bist,             ///<Check core2 Pbist, valid if there is core2 in the SKU
  FusaTestNumScan3Bist,             ///<Check core3 Pbist, valid if there is core3 in the SKU
  FusaTestNumScan4Bist,             ///<Check core4 Pbist, valid if there is core4 in the SKU
  FusaTestNumScan5Bist,             ///<Check core5 Pbist, valid if there is core5 in the SKU
  FusaTestNumScan6Bist,             ///<Check core6 Pbist, valid if there is core6 in the SKU
  FusaTestNumScan7Bist,             ///<Check core7 Pbist, valid if there is core7 in the SKU
  FusaTestNumScan8Bist,             ///<Check core8 Pbist, valid if there is core8 in the SKU
  FusaTestNumScan9Bist,             ///<Check core9 Pbist, valid if there is core9 in the SKU
  FusaTestNumScan10Bist,            ///<Check core10 Pbist, valid if there is core10 in the SKU
  FusaTestNumScan11Bist,            ///<Check core11 Pbist, valid if there is core11 in the SKU

  FusaTestNumCRCon0PBISTROM,
  FusaTestNumCRCon1PBISTROM,
  FusaTestNumCRCon2PBISTROM,
  FusaTestNumCRCon3PBISTROM,
  FusaTestNumCRCon4PBISTROM,
  FusaTestNumCRCon5PBISTROM,
  FusaTestNumCRCon6PBISTROM,
  FusaTestNumCRCon7PBISTROM,
  FusaTestNumCRCon8PBISTROM,
  FusaTestNumCRCon9PBISTROM,
  FusaTestNumCRCon10PBISTROM,
  FusaTestNumCRCon11PBISTROM,

  FusaTestNumCboSlice0EgressLlcDoubleParityCtC,
  FusaTestNumCboSlice1EgressLlcDoubleParityCtC,
  FusaTestNumCboSlice2EgressLlcDoubleParityCtC,
  FusaTestNumCboSlice3EgressLlcDoubleParityCtC,
  FusaTestNumCboSlice4EgressLlcDoubleParityCtC,
  FusaTestNumCboSlice5EgressLlcDoubleParityCtC,

  FusaTestNumHbo0E2eCfiParityCtc,
  FusaTestNumHbo1E2eCfiParityCtc,
  FusaTestNumSNCUE2eCfiParityCtc,
  FusaTestNumSVTUE2eCfiParityCtc,
  FusaTestNumPunitE2eCfiParityCtc,
  FusaTestNumIPU_PB_PIDE2eCfiParityCtc,
  FusaTestNumVPU_PB_PIDE2eCfiParityCtc,
  FusaTestNumMEDIA_PBE2eCfiParityCtc,
  FusaTestNumGT_PBE2eCfiParityCtc,
  FusaTestNumIAX_PBE2eCfiParityCtc,
  FusaTestNumDNI2CFIE2eCfiParityCtc,
  FusaTestNumCCE0E2eCfiParityCtc,
  FusaTestNumCCE1E2eCfiParityCtc,
  FusaTestNumIDIBE2EIDIParityCtC,
  FusaTestNumCCF_SANTA1E2EParityCtc,
  FusaTestNumCCF_SANTA0E2EParityCtc,
  FusaTestNumIOCE2eCfiParityCtc,
  FusaTestNumIOCCEE2eCfiParityCtc,
  FusaTestNumIVTUE2eCfiParityCtc,
  FusaTestNumIDIBE2ECfiParityCtC,
  FusaTestNumCCFE2ECNCUParityCtC0,
  FusaTestNumIDIBE2ECNCUParityCtC0,

  FusaTestNumCCFE2EIDIParityCtC0,
  FusaTestNumCCFE2EIDIParityCtC1,
  FusaTestNumCCFE2EIDIParityCtC2,
  FusaTestNumCCFE2EIDIParityCtC3,
  FusaTestNumCCFE2EIDIParityCtC4,
  FusaTestNumCCFE2EIDIParityCtC5,
#if 0
  FusaTestNumCoreCacheArrayStartupBistCore0,
  FusaTestNumCoreCacheArrayStartupBistCore1,
  FusaTestNumCoreCacheArrayStartupBistCore2,
  FusaTestNumCoreCacheArrayStartupBistCore3,
#endif
  FusaTestNumCoreRomCrcStartupBistCore0,
  FusaTestNumCoreRomCrcStartupBistCore1,
  FusaTestNumCoreRomCrcStartupBistCore2,
  FusaTestNumCoreRomCrcStartupBistCore3,

  FusaTestNumCoreIdiParityCtcCore0,
  FusaTestNumCoreIdiParityCtcCore1,
  FusaTestNumCoreIdiParityCtcCore2,
  FusaTestNumCoreIdiParityCtcCore3,

  FusaTestNumGtL3EccCtc,

  FusaTestNumTotal                        ///<Total CTC groups count
} FUSA_TEST_NUMBER;

/**
  Fusa test result HOB structure
**/
typedef struct {
  UINT32 Version;
  FUSA_TEST_RESULT FspDxCtcTestResult[FusaTestNumTotal];
} FUSA_INFO_HOB;

#pragma pack (pop)

#endif // _FUSA_INFO_HOB_H_
