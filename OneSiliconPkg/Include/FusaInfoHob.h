/** @file
  This file contains definitions required for creation of TGL
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
#define FUSA_INFO_VERSION 0x00000103

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

  @note While the core4-7 (cbo4-7) that are strictly related to
        the TGL-H are listed, there are not within the
        implementation scope and validation scope yet.
**/
typedef enum
{
  FusaTestNumMc0Cmi = 0,          ///<Check MC0 CMI path, valid if there is DIMM using MC0
  FusaTestNumMc1Cmi,              ///<Check MC1 CMI path, valid if there is DIMM using MC1
  FusaTestNumMc0CmiCh0Data,       ///<Check MC0 CH0 CMI path, valid if there is DIMM using MC0 CH0
  FusaTestNumMc0CmiCh1Data,       ///<Check MC0 CH1 CMI path, valid if there is DIMM using MC0 CH1
  FusaTestNumMc0CmiCh2Data,       ///<Check MC0 CH2 CMI path, valid if there is DIMM using MC0 CH2
  FusaTestNumMc0CmiCh3Data,       ///<Check MC0 CH3 CMI path, valid if there is DIMM using MC0 CH3
  FusaTestNumMc1CmiCh0Data,       ///<Check MC1 CH0 CMI path, valid if there is DIMM using MC1 CH0
  FusaTestNumMc1CmiCh1Data,       ///<Check MC1 CH1 CMI path, valid if there is DIMM using MC1 CH1
  FusaTestNumMc1CmiCh2Data,       ///<Check MC1 CH2 CMI path, valid if there is DIMM using MC1 CH2
  FusaTestNumMc1CmiCh3Data,       ///<Check MC1 CH3 CMI path, valid if there is DIMM using MC1 CH3
  FusaTestNumMc0CmiCh3Address,    ///<Check MC0 CMF CMI path, valid if there is DIMM using MC0
  FusaTestNumMc1CmiCh4Address,    ///<Check MC1 CMF CMI path, valid if there is DIMM using MC1
  FusaTestNumIbecc0Cmi,           ///<Check Ibecc0 CMI path, valid if there is IBECC range covering MC0 DIMMs
  FusaTestNumIbecc1Cmi,           ///<Check Ibecc1 CMI path, valid if there is IBECC range covering MC1 DIMMs
  FusaTestNumIbecc0EccCorrError,  ///<Check Ibecc0 ECC correctable error, valid if there is IBECC range covering MC0 DIMMs
  FusaTestNumIbecc1EccCorrError,  ///<Check Ibecc1 ECC correctable error, valid if there is IBECC range covering MC1 DIMMs
  FusaTestNumIbecc0EccUncorrError,///<Check Ibecc0 ECC uncorrectable error, valid if there is IBECC range covering MC0 DIMMs
  FusaTestNumIbecc1EccUncorrError,///<Check Ibecc1 ECC uncorrectable error, valid if there is IBECC range covering MC1 DIMMs

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

  /* Max 6 core CPU */
  FusaTestNumCpu0Idi,             ///<Check core0 IDI path, valid if there is core0 in the SKU
  FusaTestNumCpu1Idi,             ///<Check core1 IDI path, valid if there is core1 in the SKU
  FusaTestNumCpu2Idi,             ///<Check core2 IDI path, valid if there is core2 in the SKU
  FusaTestNumCpu3Idi,             ///<Check core3 IDI path, valid if there is core3 in the SKU
  FusaTestNumCpu4Idi,             ///<Check core4 IDI path, valid if there is core4 in the SKU
  FusaTestNumCpu5Idi,             ///<Check core5 IDI path, valid if there is core5 in the SKU

  FusaTestNumWRC,
  FusaTestNumNcuIdp,

  FusaTestNumOpiLinkIosfData,       ///<Check OPI Link path
  FusaTestNumDip,                   ///<Check DIP path
  FusaTestNumIop,                   ///<Check IOP path

  FusaTestNumGrtArray0Bist,             ///<Check core0 Pbist, valid if there is core0 in the SKU
  FusaTestNumGrtArray1Bist,             ///<Check core1 Pbist, valid if there is core1 in the SKU
  FusaTestNumGrtArray2Bist,             ///<Check core2 Pbist, valid if there is core2 in the SKU
  FusaTestNumGrtArray3Bist,             ///<Check core3 Pbist, valid if there is core3 in the SKU
  FusaTestNumGrtArray4Bist,             ///<Check core4 Pbist, valid if there is core4 in the SKU
  FusaTestNumGrtArray5Bist,             ///<Check core5 Pbist, valid if there is core5 in the SKU
  FusaTestNumGrtArray6Bist,             ///<Check core6 Pbist, valid if there is core6 in the SKU
  FusaTestNumGrtArray7Bist,             ///<Check core7 Pbist, valid if there is core7 in the SKU

  FusaTestNumGrtScan0Bist,             ///<Check core0 Pbist, valid if there is core0 in the SKU
  FusaTestNumGrtScan1Bist,             ///<Check core1 Pbist, valid if there is core1 in the SKU
  FusaTestNumGrtScan2Bist,             ///<Check core2 Pbist, valid if there is core2 in the SKU
  FusaTestNumGrtScan3Bist,             ///<Check core3 Pbist, valid if there is core3 in the SKU
  FusaTestNumGrtScan4Bist,             ///<Check core4 Pbist, valid if there is core4 in the SKU
  FusaTestNumGrtScan5Bist,             ///<Check core5 Pbist, valid if there is core5 in the SKU
  FusaTestNumGrtScan6Bist,             ///<Check core6 Pbist, valid if there is core6 in the SKU
  FusaTestNumGrtScan7Bist,             ///<Check core7 Pbist, valid if there is core7 in the SKU

   /* Max 8 core CPU */
  FusaTestNumCRCon0PBISTROM,
  FusaTestNumCRCon1PBISTROM,
  FusaTestNumCRCon2PBISTROM,
  FusaTestNumCRCon3PBISTROM,
  FusaTestNumCRCon4PBISTROM,
  FusaTestNumCRCon5PBISTROM,
  FusaTestNumCRCon6PBISTROM,
  FusaTestNumCRCon7PBISTROM,

   /* Max 8 core CPU */
  FusaTestNumCboSlice0EgressLlcDoubleParityCtC,    ///<Check CBO0 ingress path, valid if there is core0 in the SKU
  FusaTestNumCboSlice1EgressLlcDoubleParityCtC,    ///<Check CBO1 ingress path, valid if there is core1 in the SKU
  FusaTestNumCboSlice2EgressLlcDoubleParityCtC,    ///<Check CBO2 ingress path, valid if there is core2 in the SKU
  FusaTestNumCboSlice3EgressLlcDoubleParityCtC,    ///<Check CBO3 ingress path, valid if there is core3 in the SKU
  FusaTestNumCboSlice4EgressLlcDoubleParityCtC,    ///<Check CBO4 ingress path, valid if there is core4 in the SKU
  FusaTestNumCboSlice5EgressLlcDoubleParityCtC,    ///<Check CBO5 ingress path, valid if there is core5 in the SKU
  FusaTestNumCboSlice6EgressLlcDoubleParityCtC,    ///<Check CBO6 ingress path, valid if there is core6 in the SKU
  FusaTestNumCboSlice7EgressLlcDoubleParityCtC,    ///<Check CBO7 ingress path, valid if there is core7 in the SKU

  FusaTestNumCboSlice0Ingress,            ///<Check CBO0 ingress path, valid if there is core0 in the SKU
  FusaTestNumCboSlice1Ingress,            ///<Check CBO1 ingress path, valid if there is core1 in the SKU
  FusaTestNumCboSlice2Ingress,            ///<Check CBO2 ingress path, valid if there is core2 in the SKU
  FusaTestNumCboSlice3Ingress,            ///<Check CBO3 ingress path, valid if there is core3 in the SKU
  FusaTestNumCboSlice4Ingress,            ///<Check CBO4 ingress path, valid if there is core4 in the SKU
  FusaTestNumCboSlice5Ingress,            ///<Check CBO5 ingress path, valid if there is core5 in the SKU
  FusaTestNumCboSlice6Ingress,            ///<Check CBO6 ingress path, valid if there is core6 in the SKU
  FusaTestNumCboSlice7Ingress,            ///<Check CBO7 ingress path, valid if there is core7 in the SKU

  FusaTestNumTotal                        ///<Totak CTC groups count
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
