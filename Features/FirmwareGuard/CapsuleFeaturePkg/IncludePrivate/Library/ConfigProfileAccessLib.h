/** @file
  Parse config file and extract image accordingly

  The format of the confile profile ini file is below:

  [Head]
  NumOfUpdate    = <Num>     # Decimal
  ImageFileGuid  = XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX  # The image to be programmed to FW deivce. e.g., gCapsuleBiosImageFileGuid
  HelperFileGuid = XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX  # The image contains addition gCapsuleBiosBgupFileGuid (Optional)

  Update0        = <Name1>   # String
  Update1        = <Name2>   # String
  Update<Num-1>  = <NameX>   # String

  [Name?]
  BaseAddress    = <BaseAddress>    # Relative address to BIOS region on flash (HEX)
  Length         = <Length>         # Image Length (HEX)
  ImageOffset    = <ImageOffset>    # Image offset within file of ImageFileGuid (HEX)
  HelperOffset   = <ImageOffset>    # Image offset within file of HelperFileGuid (HEX) (Optional)
  HelperLength   = <HelperLength>   # Helper image Length (HEX) (Optional)

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

@par Specification Reference:

**/

#ifndef __CONFIG_PROFILE_ACCESS_LIB_H__
#define __CONFIG_PROFILE_ACCESS_LIB_H__

//
// Update data in config ini file
//
typedef struct {
  UINTN                           NumOfUpdates;
  EFI_GUID                        FileGuid;
  EFI_GUID                        HelperFileGuid;
} CONFIG_HEADER;

typedef struct {
  UINTN                           Index;
  EFI_PHYSICAL_ADDRESS            BaseAddress;
  UINTN                           Length;
  UINTN                           ImageOffset;
  UINTN                           HelperImageOffset;
  UINTN                           HelperImageLength;
} UPDATE_CONFIG_DATA;

/**
  Extract update image by config file.

  The input FvImage contains multiple FFS files including config data file
  and update image file. This function extracts and parse config file first,
  then extracts images of FileGuid and HelperFileGuid described in config header.

  @param[in]   FvImage           The input firmware volume image consisted of multiple FFSs.
  @param[in]   FvImageSize       The size of FvImage in bytes.
  @param[in]   ConfigFileGuid    The filename of Config file within FvImage.
  @param[out]  ConfigHeader      Pointer to parsed Config header.
  @param[out]  ConfigDataArray   Pointer to parsed Config data.
  @param[out]  UpdateImage       Pointer to image described by ConfigHeader->FileGuid.
  @param[out]  UpdateImageSize   The size of UpdateImage in bytes.
  @param[out]  HelperImage       Pointer to image described by ConfigHeader->HelperFileGuid. May be NULL.
  @param[out]  HelperImageSize   The size of HelperImage in bytes. May be NULL.

  @retval EFI_SUCCESS      UpdateImage is successfully extracted.
  @retval EFI_NOT_FOUND    UpdateImage is not extracted.

**/
EFI_STATUS
ExtractUpdateImageByConfigFile (
  IN      VOID                          *FvImage,
  IN      UINTN                         FvImageSize,
  IN      EFI_GUID                      *ConfigFileGuid,
  IN OUT  CONFIG_HEADER                 *ConfigHeader,
  IN OUT  UPDATE_CONFIG_DATA            **ConfigDataArray,
  IN OUT  VOID                          **UpdateImage,
  IN OUT  UINTN                         *UpdateImageSize,
  IN OUT  VOID                          **HelperImage,   OPTIONAL
  IN OUT  UINTN                         *HelperImageSize OPTIONAL
  );

#endif
