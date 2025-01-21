/** @file
  Si Config Block

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _SI_CONFIG_H_
#define _SI_CONFIG_H_

#define SI_CONFIG_REVISION  1

extern EFI_GUID gSiConfigGuid;


#pragma pack (push,1)

/**
  The Silicon Policy allows the platform code to publish a set of configuration
  information that the RC drivers will use to configure the silicon hardware.

  <b>Revision 1</b>:  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;  ///< Offset 0 - 27 Config Block Header
  //
  // Platform specific common policies that used by several silicon components.
  //
  /**
    This is used to skip the SSID programming in silicon code.
    When set to TRUE, silicon code will not do any SSID programming and platform code
    needs to handle that by itself properly.
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT8  SkipSsidProgramming;   ///< offset 28
  UINT8  RsvdBytes0[3];         ///< offset 29 - 31
  /**
    When SkipSsidProgramming is FALSE, silicon code will use this as default value
    to program the SVID for all internal devices.
    <b>0: use silicon default SVID 0x8086 </b>, Non-zero: use customized SVID.
  **/
  UINT16 CustomizedSvid;        ///< offset 32 - 33
  /**
    When SkipSsidProgramming is FALSE, silicon code will use this as default value
    to program the Sid for all internal devices.
    <b>0: use silicon default SSID 0x7270 </b>, Non-zero: use customized SSID.
  **/
  UINT16 CustomizedSsid;        ///< offset 34 - 35
  /**
    SsidTablePtr contains the SVID_SID_INIT_ENTRY table.
    This is valid when SkipSsidProgramming is FALSE;
    It doesn't need to contain entries for all Intel internal devices.
    It can only contains the SVID_SID_INIT_ENTRY entries for those Dev# Func# which needs
    to be overridden.
    In the enties, only Dev, Function, SubSystemVendorId, and SubSystemId are required.
    <b>Default is NULL.</b>

    E.g. Platform only needs to override BDF 0:31:5 to AAAA:BBBB and BDF 0:31:3 to CCCC:DDDD,
    it can be done in platform like this:
    STATIC SVID_SID_INIT_ENTRY mSsidTablePtr[SI_MAX_DEVICE_COUNT] = {0};

    VOID SiPolicyUpdate () {
      UINT32 EntryCount = 0;
      SiPolicy->SkipSsidProgramming = FALSE;
      SiPolicy->SsidTablePtr = mSsidTablePtr;

      mSsidTablePtr[EntryCount].Address.Bits.Device   = SpiDeviceNumber ();
      mSsidTablePtr[EntryCount].Address.Bits.Function = SpiFunctionNumber ();
      mSsidTablePtr[EntryCount].SvidSidValue.SubSystemVendorId = 0xAAAA;
      mSsidTablePtr[EntryCount].SvidSidValue.SubSystemId       = 0xBBBB;
      EntryCount ++;
      mSsidTablePtr[EntryCount].Address.Bits.Device   = HdaDevNumber ();
      mSsidTablePtr[EntryCount].Address.Bits.Function = HdaFuncNumber ();
      mSsidTablePtr[EntryCount].SvidSidValue.SubSystemVendorId = 0xCCCC;
      mSsidTablePtr[EntryCount].SvidSidValue.SubSystemId       = 0xDDDD;
      EntryCount ++;
      ASSERT (EntryCount < SI_MAX_DEVICE_COUNT);
      SiPolicy->NumberOfSsidTableEntry = EntryCount;
    }
  **/
  UINT32 *SsidTablePtr;             ///< offset 36 - 39
  /**
    Number of valid enties in SsidTablePtr.
    This is valid when SkipSsidProgramming is FALSE;
    <b>Default is 0.</b>
  **/
  UINT16 NumberOfSsidTableEntry;    ///< offset 40 - 41
  UINT8  RsvdBytes1[2];             ///< offset 42 - 43
  /**
    This is used to skip setting BIOS_DONE MSR during firmware update boot mode.
    When set to TRUE and boot mode is BOOT_ON_FLASH_UPDATE,
    skip setting BIOS_DONE MSR at EndofPei.
    <b>0: FALSE</b>, 1: TRUE
  **/
  UINT8  SkipBiosDoneWhenFwUpdate;  ///< Offset 44
  UINT8  RsvdBytes2[3];             ///< Offset 45 - 47
} SI_CONFIG;

#pragma pack (pop)

#define DEFAULT_SSVID        0x8086
#define DEFAULT_SSDID        0x7270
#define SI_MAX_DEVICE_COUNT  70

///
/// Subsystem Vendor ID / Subsystem ID
///
typedef struct {
  UINT16         SubSystemVendorId;
  UINT16         SubSystemId;
} SVID_SID_VALUE;

//
// Below is to match PCI_SEGMENT_LIB_ADDRESS () which can directly send to PciSegmentRead/Write functions.
//
typedef struct {
  union {
    struct {
      UINT32  Register:12;
      UINT32  Function:3;
      UINT32  Device:5;
      UINT32  Bus:8;
      UINT32  Reserved1:4;
      UINT32  Segment:16;
      UINT32  Reserved2:16;
    } Bits;
    UINT64    SegBusDevFuncRegister;
  } Address;
  SVID_SID_VALUE SvidSidValue;
  UINT32 Reserved;
} SVID_SID_INIT_ENTRY;


typedef struct {
  UINT32  SkipBus;
  UINT32  SkipDevice;
  UINT32  SkipFunction;
} SVID_SID_SKIP_TABLE;

#endif // _SI_CONFIG_H_
