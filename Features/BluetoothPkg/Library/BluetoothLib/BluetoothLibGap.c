/** @file
  The helper functions are used Generic Attributes Profile.

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

@par Specification
**/

#include <Library/BluetoothLib.h>

#pragma pack(1)
typedef struct {
  UINT8 Length;
  UINT8 Type;
} BLUETOOTH_ADVERTISING_DATA_HEAD;
#pragma pack()
/**
  Finds a specific type of Generic Access Profile data from the advertisement
  data.

  @param[in]      AdvertisementData         Advertisement Data Buffer.
  @param[in]      AdvertisementDataSize     Length of buffer provided.
  @param[in]      Type                      Type of data field that is needed.
  @param[out]     OutLen                    Length of valid data in the output.

  @retval         Other       Null in case it fails and pointer to location of data in case of success.
**/
UINT8*
EFIAPI
BluetoothFindTypeFromAdvData (
  IN UINT8                                 *AdvertisementData,
  IN UINTN                                 AdvertisementDataSize,
  IN UINT8                                 Type,
  OUT UINT8                                *OutLen
  )
{
  BLUETOOTH_ADVERTISING_DATA_HEAD          *Head;
  UINTN                                    Offset;

  if (AdvertisementDataSize < sizeof (BLUETOOTH_ADVERTISING_DATA_HEAD)) {
    return NULL;
  }

  Offset = 0;
  Head   = (BLUETOOTH_ADVERTISING_DATA_HEAD *)AdvertisementData;
  while (Offset <= AdvertisementDataSize - sizeof (BLUETOOTH_ADVERTISING_DATA_HEAD)) {
    Head   = (BLUETOOTH_ADVERTISING_DATA_HEAD *)&AdvertisementData[Offset];
    if (Head->Length == 0) {
      *OutLen = 0;
      return NULL;
    }

    if ((UINTN)Head->Length + 1 > MAX_UINTN - Offset) {
      *OutLen = 0;
      return NULL;
    }
    Offset += Head->Length + 1;
    if (Head->Type == Type) {
      break;
    }
  }

  if ((Offset > AdvertisementDataSize) || (Head->Type != Type)) {
    *OutLen = 0;
    return NULL;
  }

  *OutLen = Head->Length - 1;
  return (UINT8 *)(Head + 1);
}
