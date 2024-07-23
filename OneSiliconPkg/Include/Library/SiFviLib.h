/** @file
  Header file for Reference code Firmware Version Info Interface Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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
#ifndef _SI_FVI_LIB_H_
#define _SI_FVI_LIB_H_

#include <SiFvi.h>

/**
  Initialize all Smbios FVI OEM Type Data Hob
**/
VOID
BuildFviInfoHob (
  VOID
  );

/**
  Update All Smbios FVI OEM Type Data

  @param[in]  SmbiosOemType    - SMBIOS OEM Type
  @param[in]  Crid             - GENERAL HOB Crid value

  @retval EFI_UNSUPPORTED      - Could not locate SMBIOS protocol
  @retval EFI_SUCCESS          - Successfully update FVI data
**/
EFI_STATUS
UpdateFviInfo (
  IN UINT8                    SmbiosOemType,
  IN UINT32                   Crid
  );

/**
  Create the Reference code version info as per Firmware Version Info (FVI) Interface Spec v0.8
  and add the SMBIOS table using the SMBIOS protocol

  Invoke this routine to add the table entry when all the Fvi data is finalized.

  - @pre
    - EFI_SMBIOS_PROTOCOL in Native mode

  @param[in] Header    The expanded header includes the standard SMBIOS table header, plus the Count
                       of the number of elements in the Data and Strings arrays.
  @param[in] *Data     Pointer to an array of Data blocks.
  @param[in] *Strings  Pointer to an array of Strings. There are FVI_NUMBER_OF_STRINGS * Count strings total.

  @retval EFI_SUCCESS          - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES - if not able to get resources.
  @retval EFI_UNSUPPORTED      - if required DataHub or SMBIOS protocol is not available.
**/
EFI_STATUS
AddFviEntry (
  IN FVI_HEADER  Header,
  IN FVI_DATA    *Data,
  IN FVI_STRINGS *Strings
  );

#endif
