/**@file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#ifndef __OverClockSetup__H__
#define __OverClockSetup__H__
#ifdef __cplusplus
extern "C"
{
#endif

// SPD Defines
#define SPDGENERAL      0
#define SPDREVISION     1
#define DRAMDEVICETYPE  2

#define CASLATENCIESLSB_DEFAULT               14
#define CASLATENCIESMSB_DEFAULT               15
#define MINIMUMRASTOCASDELAYTIME_DEFAULT      18
#define MINIMUMROWPRECHARGEDELAYTIME_DEFAULT  20
#define UPPERNIBBLESFORTRASANDTRC_DEFAULT     21
#define TRASMIN_DEFAULT                       22

#define CASLATENCIESLSB_XMP1                  188
#define CASLATENCIESMSB_XMP1                  189
#define MINIMUMRASTOCASDELAYTIME_XMP1         192
#define MINIMUMROWPRECHARGEDELAYTIME_XMP1     191
#define UPPERNIBBLESFORTRASANDTRC_XMP1        194
#define TRASMIN_XMP1                          195

#define CASLATENCIESLSB_XMP2                  223
#define CASLATENCIESMSB_XMP2                  224
#define MINIMUMRASTOCASDELAYTIME_XMP2         227
#define MINIMUMROWPRECHARGEDELAYTIME_XMP2     226
#define UPPERNIBBLESFORTRASANDTRC_XMP2        229
#define TRASMIN_XMP2                          230

#define SPD_COUNT                             7

// SPD Profile
#define DEFAULT_SPD_PROFILE 0
#define CUSTOM_PROFILE      1
#define XMP_PROFILE_1       2
#define XMP_PROFILE_2       3
#define XMP_PROFILE_3       4
#define USER_PROFILE_4      5
#define USER_PROFILE_5      6
#define MEMORY_PROFILE_MAX  7

// Frequency Ratio Limits
#define MEMORY_RATIO_MIN    3
#define MEMORY_RATIO_MAX    255

#include <SetupPrivate.h>
#include <Library/CpuMailboxLib.h>
#include <CMrcInterface.h>
#include <Protocol/MemInfo.h>
#include <Library/OcMailboxLib.h>

typedef struct {
  UINT32        tCK;
  MrcFrequency  Frequency;
  UINT8         FreqFlag;  // Bitmask of flags from MrcFreqFlag enum
} TRangeTable;

/**
  This callback function displays specific checked value in the OC Setup Menu when OC Enabled.

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action          Specifies the type of action taken by the browser.
  @param QuestionId      A unique value which is sent to the original exporting driver
                         so that it can identify the type of data to expect.
  @param Type            The type of value for the question.
  @param Value           A pointer to the data being sent to the original exporting driver.
  @param ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
OcFormCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  );

/**
  This function displays the Memory Timing values in the OC Setup Menu when Memory Profile is changed.

  @param This            Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param Action          Specifies the type of action taken by the browser.
  @param QuestionId      A unique value which is sent to the original exporting driver
                         so that it can identify the type of data to expect.
  @param Type            The type of value for the question.
  @param Value           A pointer to the data being sent to the original exporting driver.
  @param ActionRequest   On return, points to the action requested by the callback function.

  @retval EFI_SUCCESS - The function completed successfully
  @retval EFI_UNSUPPORTED - The Setup Menu was not updated
  @retval EFI_NOT_FOUND - There is an error in Locating protocols or retrieving browser data
**/
EFI_STATUS
EFIAPI
OcFormMemoryTimingCallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  );

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif
#endif
