/** @file
  Sample implementation of OEM member functions that are used by MRC,
  and SPD read routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include "MrcOemPlatform.h"
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/SmbusLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/SmbusInfoLib.h>
#include <Register/RtcRegs.h>
#include <HostBridgeConfig.h>
#include <Ppi/SiPolicy.h>
#include <Library/PostCodeLib.h>
#include <TraceHubDataHob.h>
#include <Register/TraceHubRegs.h>
#include <Library/TraceHubDebugLib.h>
#include <Library/HobLib.h>
#include "MrcBlueGreenCommunication.h"
#include "BlueMrcAgentCommunication.h"

#pragma pack (push, 1)
typedef union {
  struct {
    UINT32                         : 8;
    UINT32 MAX_NON_TURBO_LIM_RATIO : 8;
    UINT32                         : 16;
    UINT32                         : 32;
  } Bits;
  UINT64 Data;
  UINT32 Data32[2];
  UINT16 Data16[4];
  UINT8  Data8[8];
} PCU_CR_PLATFORM_INFO_STRUCT;

#pragma pack (pop)

#define SA_SYSTEM_BCLK                (100)
#define PCU_CR_PLATFORM_INFO          (0xCE)
#define MRC_POST_CODE_LOW_BYTE_ADDR   (0x48)
#define MRC_POST_CODE_HIGH_BYTE_ADDR  (0x49)
#define MAX_SPD_PAGE_COUNT            (4)
#define MAX_SPD_PAGE_SIZE             (256)
#define MAX_SPD_PAGE_SIZE_SPD5        (128)
#define MAX_SPD_SIZE                  (MAX_SPD_PAGE_SIZE * MAX_SPD_PAGE_COUNT)
#define SPD_DDR5_SDRAM_TYPE_NUMBER    (0x12)
#define SPD_LPDDR5_SDRAM_TYPE_NUMBER  (0x13)
#define ISVT_END_OF_MRC_STATE         (0x10)

/**
  Reset the SPD page back to page 0 on an SPD5 Hub device at the input
  SPD SUMBUS address.

  @param[in] SpdAddress  - SPD SMBUS address

  @retval None
**/
void
ResetPageSpd5 (
  IN     const UINT8     SpdAddress
  )
{
  // Set SPD5 MR11[2:0] = 0 (Page 0)
  SmBusWriteDataByte (SpdAddress | ((UINT32) SPD5_MEMREG_REG (SPD5_MR11) << 8), 0, NULL);
}

/**
  Check if SPD Byte 0 matches the SPD5 HUB MR0 identifier.

  @param[in] SpdAddress  - SPD SMBUS address

  @retval TRUE if the SPD HUB at the input address is SPD5
  @retval FALSE if the SMBUS read failed or the value did not match the SPD5 ID.
**/
BOOLEAN
IsSpd5Hub (
  IN     const UINT8     SpdAddress
  )
{
  RETURN_STATUS EfiStatus;
  UINT8     SpdHubByte;
  BOOLEAN   Spd5;

  Spd5 = FALSE;
  SpdHubByte = 0;
  // Read SPD5 MR0
  SpdHubByte = SmBusReadDataByte (SpdAddress | ((UINT32) SPD5_MEMREG_REG (SPD5_MR0) << 8), &EfiStatus);
  if (!EFI_ERROR (EfiStatus)) {
    Spd5 = (SpdHubByte == SPD5_MR0_SPD5_HUB_DEVICE);
  }
  return Spd5;
}

/**
  Read the SPD data over the SMBus, at the specified SPD address, starting at
  the specified starting offset and read the given amount of data.

  @param[in] SpdAddress  - SPD SMBUS address
  @param[in, out] Buffer - Buffer to store the data.
  @param[in] Start       - Starting SPD offset
  @param[in] Size        - The number of bytes of data to read and also the size of the buffer.
  @param[in, out] Page   - The final page that is being pointed to.
  @param[in] Spd5        - Specifies that SPD5 Hub spec must be used.

  @retval RETURN_SUCCESS if the read is successful, otherwise error status.
**/
static
RETURN_STATUS
DoSpdRead (
  IN     const UINT8  SpdAddress,
  IN OUT UINT8        *const Buffer,
  IN     const UINT16 Start,
  IN           UINT16 Size,
  IN OUT UINT8        *const Page,
  IN     BOOLEAN      Spd5
  )
{
  RETURN_STATUS EfiStatus;
  BOOLEAN       PageUpdate;
  UINT16        Count;
  UINT16        Index;
  UINT32        MaxPageSize;
  UINT32        Offset;

  EfiStatus = RETURN_DEVICE_ERROR;
  MaxPageSize = (Spd5 ? MAX_SPD_PAGE_SIZE_SPD5 : MAX_SPD_PAGE_SIZE);
  if ((Buffer != NULL) && (Start < MAX_SPD_SIZE) && ((Start + Size) <= MAX_SPD_SIZE)) {
    Count = 0;
    PageUpdate = FALSE;
    while (Size--) {
      Index = Start + Count;
      if ((Index / MaxPageSize) != *Page) {
        *Page = (UINT8) (Index / MaxPageSize);
        PageUpdate = TRUE;
      }
      Index %= MaxPageSize;
      if (PageUpdate == TRUE) {
        PageUpdate = FALSE;
        if (Spd5) {
          Offset = SPD5_MEMREG_REG (SPD5_MR11);
          SmBusWriteDataByte (SpdAddress | (Offset << 8), *Page, &EfiStatus);
        } else {
          SmBusWriteDataByte ((*Page == 0) ? SPD_PAGE_ADDRESS_0 : SPD_PAGE_ADDRESS_1, 0, &EfiStatus);
        }
      }
      if (Spd5) {
        Index = SPD5_MEMREG_NVM (Index);
      }
      Buffer[Count] = SmBusReadDataByte (SpdAddress | ((UINT32) Index << 8), &EfiStatus);
      if (RETURN_SUCCESS != EfiStatus) {
        Buffer[Count] = 0;
        break;
      }
      Count++;
    }
    EfiStatus = RETURN_SUCCESS;
  }
  return (EfiStatus);
}

/**
  See if there is valid XMP SPD data.

  @param[in] Debug    - Mrc debug structure.
  @param[in, out] Spd - Mrc SPD structure.
  @param[in] XmpStart - The current offset in the SPD.

  @retval TRUE if valid, FALSE in not.
**/
static
BOOLEAN
VerifyXmp (
  IN OUT MrcSpd *const Spd,
  IN const UINT16 XmpStart
  )
{
  SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0  *Header3;
  BOOLEAN                                 Xmp;

  Xmp = FALSE;

  switch (((UINT8 *) Spd) [2]) {
    case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
      Header3 = &Spd->Ddr5.EndUser.Xmp.Header;
      if (XmpStart == ((UINTN) (Header3) - (UINTN) Spd)) {
        Xmp = TRUE;
        if (((Header3->XmpRevision.Data) == 0x30) &&
           (Header3->XmpOrg.Bits.ProfileEnable1 != 0)) {
          // Need read XMP_PROFILE1
          return (TRUE);
        } else {
          Header3->XmpId            = 0;
          Header3->XmpOrg.Data      = 0;
          Header3->XmpConf.Data     = 0;
          Header3->XmpRevision.Data = 0;
        }
      } else if (XmpStart > ((UINTN) (Header3) - (UINTN) Spd)) {
        Xmp = TRUE;
        /*
         * Read sequence:
         * 1) XMP1. If There is no XMP1, stop reading.
         * 2) When XMP1 is ok, read USER_PROFILE4/5.
         * 3) Read XMP_PROFILE2/3 based on Enable flag.
         */
        if (XmpStart == SPD_XMP3_PROFILE1_START_BYTE) {
          // Need read USER_PROFILE4
          return (TRUE);
        } else if (XmpStart == SPD_XMP3_USER_PROFILE4_START_BYTE) {
          // Need read USER_PROFILE5
          return (TRUE);
        } else if ((XmpStart == SPD_XMP3_USER_PROFILE5_START_BYTE) && (Header3->XmpOrg.Bits.ProfileEnable2 != 0)) {
          // Need read XMP_PROFILE2
          return (TRUE);
        } else if ((XmpStart == SPD_XMP3_PROFILE2_START_BYTE) && (Header3->XmpOrg.Bits.ProfileEnable3 != 0)) {
          // Need read XMP_PROFILE3
          return (TRUE);
        }
      }
      break;
    case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
      return (TRUE);
    default:
      return (FALSE);
  }
  if (!Xmp) {
    return (TRUE);
  }
  return (FALSE);
}

/**
  Read the SPD data over the SMBus, at the given SmBus SPD address and copy the data to the data structure.
  The SPD data locations read is controlled by the current boot mode.

  @param[in] BootMode           - The current MRC boot mode.
  @param[in] Address            - SPD SmBus address offset.
  @param[in] Buffer             - Buffer that contains the data read from the SPD.
  @param[in] SpdDdr5Table       - Indicates which SPD bytes to read.
  @param[in] SpdDdr5TableSize   - Size of SpdDdr5Table in bytes.
  @param[in] SpdLpddrTable      - Indicates which SPD bytes to read.
  @param[in] SpdLpddrTableSize  - Size of SpdLpddrTable in bytes.

  @retval TRUE if the read is successful, otherwise FALSE on error.
**/
BOOLEAN
EFIAPI
GetSpdData (
  IN SPD_BOOT_MODE BootMode,
  IN UINT8         Address,
  IN OUT   UINT8   *Buffer,
  IN UINT8         *SpdDdr5Table,
  IN UINT32        SpdDdr5TableSize,
  IN UINT8         *SpdLpddrTable,
  IN UINT32        SpdLpddrTableSize
  )
{
  const SPD_OFFSET_TABLE *Tbl;
  const SPD_OFFSET_TABLE *TableSelect;
  RETURN_STATUS          Status;
  UINT32                 Byte;
  UINT32                 Stop;
  UINT8                  Page;
  BOOLEAN                Spd5;

  // The "SPD Write Disable" bit must not be set
  ASSERT(!SmbusIsHostLocked());
  Spd5   = IsSpd5Hub (Address);
  Page   = (UINT8) (~0);
  Status = DoSpdRead (Address, &Buffer[SPD_SDRAM_TYPE_OFFSET], SPD_SDRAM_TYPE_OFFSET, 1, &Page, Spd5);
  if (RETURN_SUCCESS == Status) {
    switch (Buffer[SPD_SDRAM_TYPE_OFFSET]) {
      case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
        TableSelect = (SPD_OFFSET_TABLE *) SpdDdr5Table;
        Stop = (SpdDdr5TableSize / sizeof (SPD_OFFSET_TABLE));
        break;

      case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
      case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
        TableSelect = (SPD_OFFSET_TABLE *) SpdLpddrTable;
        Stop = (SpdLpddrTableSize / sizeof (SPD_OFFSET_TABLE));
        break;

      default:
        TableSelect = NULL;
        Stop = 0;
        break;
    }
    for (Byte = 0; (RETURN_SUCCESS == Status) && (Byte < Stop); Byte++) {
      Tbl = &TableSelect[Byte];
      if ((1 << BootMode) & Tbl->BootMode) {
        Status = DoSpdRead (Address, &Buffer[Tbl->Start], Tbl->Start, Tbl->End - Tbl->Start + 1, &Page, Spd5);
        if (Status == RETURN_SUCCESS) {
          if (SpdCold == BootMode) {
            if (FALSE == VerifyXmp ((MrcSpd *) Buffer, Tbl->Start)) {
              break;
            }
          }
        } else {
          break;
        }
      }
    }
  }
  if (Spd5) {
    // Reset the page for the next loop iteration
    ResetPageSpd5 (Address);
  }

  return ((RETURN_SUCCESS == Status) ? TRUE : FALSE);
}

//
// This is from Edk2\MdeModulePkg\Include\Guid\StatusCodeDataTypeDebug.h
// Might need to be adjusted for a particular BIOS core
//
#ifndef EFI_STATUS_CODE_DATA_MAX_SIZE
#define EFI_STATUS_CODE_DATA_MAX_SIZE 0x200
#endif

/**
  Output a string to the debug stream/device.
  If there is a '%' sign in the string, convert it to '%%', so that DEBUG() macro will print it properly.

  @param[in] String     - The string to output.

  @retval Nothing.
**/
VOID
EFIAPI
SaDebugPrint (
  VOID   *String
  )
{
  CHAR8   Str[EFI_STATUS_CODE_DATA_MAX_SIZE];
  CHAR8   *InputStr;
  CHAR8   *OutputStr;
  UINT32  i;

  InputStr = (CHAR8 *) String;
  OutputStr = Str;
  i = 0;
  while ((*InputStr != 0) && (i < (EFI_STATUS_CODE_DATA_MAX_SIZE - 2))) {
    *OutputStr++ = *InputStr;
    i++;
    if (*InputStr++ == '%') {
      *OutputStr++ = '%';
      i++;
    }
  }
  *OutputStr = 0;  // Terminating NULL
  // Print debug string with DEBUG_ERROR (highest priority) since MRC debug message is already screened for ErrorLevel in MrcPrintfVaList
  DEBUG ((DEBUG_ERROR, Str));
}

/**
  Calculate the PCI device address for the given Bus/Device/Function/Offset.

  @param[in] Bus      - PCI bus
  @param[in] Device   - PCI device
  @param[in] Function - PCI function
  @param[in] Offset   - Offset

  @retval The PCI device address.
**/
UINT32
EFIAPI
GetPciDeviceAddress (
  IN const UINT8 Bus,
  IN const UINT8 Device,
  IN const UINT8 Function,
  IN const UINT8 Offset
  )
{
  return (
    ((UINT32) ((Bus)      & 0xFF) << 16) |
    ((UINT32) ((Device)   & 0x1F) << 11) |
    ((UINT32) ((Function) & 0x07) << 8)  |
    ((UINT32) ((Offset)   & 0xFF) << 0)  |
    (1UL << 31));
}

/**
  Calculate the PCIE device address for the given Bus/Device/Function/Offset.

  @param[in] Bus      - PCI bus
  @param[in] Device   - PCI device
  @param[in] Function - PCI function
  @param[in] Offset   - Offset

   The PCIE device address.

  @retval The PCIe device address
**/
UINT32
EFIAPI
GetPcieDeviceAddress (
  IN const UINT8 Bus,
  IN const UINT8 Device,
  IN const UINT8 Function,
  IN const UINT8 Offset
  )
{
  return (
    ((UINT32) Bus << 20) +
    ((UINT32) Device << 15) +
    ((UINT32) Function << 12) +
    ((UINT32) Offset << 0));
}

/**
  Read specific RTC/CMOS RAM

  @param[in] Location        Point to RTC/CMOS RAM offset for read

  @retval The data of specific location in RTC/CMOS RAM.
**/
UINT8
EFIAPI
PeiRtcRead (
  IN const UINT8 Location
  )
{
  UINT8  RtcIndexPort;
  UINT8  RtcDataPort;

  //
  // CMOS access registers (using alternative access not to handle NMI bit)
  //
  if (Location < RTC_BANK_SIZE) {
    //
    // First bank
    //
    RtcIndexPort  = R_RTC_IO_INDEX_ALT;
    RtcDataPort   = R_RTC_IO_TARGET_ALT;
  } else {
    //
    // Second bank
    //
    RtcIndexPort  = R_RTC_IO_EXT_INDEX_ALT;
    RtcDataPort   = R_RTC_IO_EXT_TARGET_ALT;
  }

  IoWrite8 (RtcIndexPort, Location & RTC_INDEX_MASK);
  return IoRead8 (RtcDataPort);
}

/**
  Returns the current time, as determined by reading the Real Time Clock (RTC) on the platform.
  Since RTC time is stored in BCD, convert each value to binary.

  @param[out] Seconds       - The current second (0-59).
  @param[out] Minutes       - The current minute (0-59).
  @param[out] Hours         - The current hour (0-23).
  @param[out] DayOfMonth    - The current day of the month (1-31).
  @param[out] Month         - The current month (1-12).
  @param[out] Year          - The current year (2000-2099).

  @retval Nothing.
**/
VOID
EFIAPI
GetRtcTime (
  OUT UINT8  *const Seconds,
  OUT UINT8  *const Minutes,
  OUT UINT8  *const Hours,
  OUT UINT8  *const DayOfMonth,
  OUT UINT8  *const Month,
  OUT UINT16 *const Year
  )
{
  UINT32 Timeout;

  //
  // Wait until RTC "update in progress" bit goes low.
  //
  Timeout = 0x0FFFFF;
  do {
    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGA);
    if ((IoRead8 (RTC_TARGET_REGISTER) & RTC_UPDATE_IN_PROGRESS) != RTC_UPDATE_IN_PROGRESS) {
      break;
    }
  } while (--Timeout > 0);

  if (0 == Timeout) {
    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGB);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_HOLD | RTC_MODE_24HOUR);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGA);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_CLOCK_DIVIDER | RTC_RATE_SELECT);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGC);
    IoRead8 (RTC_TARGET_REGISTER);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGD);
    IoRead8 (RTC_TARGET_REGISTER);

    IoWrite8 (RTC_INDEX_REGISTER, CMOS_REGB);
    IoWrite8 (RTC_TARGET_REGISTER, RTC_MODE_24HOUR);
  }
  //
  // Read seconds
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_SECONDS);
  *Seconds = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read minutes
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_MINUTES);
  *Minutes = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read hours
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_HOURS);
  *Hours = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read day of month
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_DAY_OF_MONTH);
  *DayOfMonth = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read month
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_MONTH);
  *Month = IoRead8 (RTC_TARGET_REGISTER);

  //
  // Read year and add current century.
  //
  IoWrite8 (RTC_INDEX_REGISTER, RTC_YEAR);
  *Year = IoRead8 (RTC_TARGET_REGISTER);

  *Seconds    = BCD2BINARY (*Seconds);
  *Minutes    = BCD2BINARY (*Minutes);
  *Hours      = BCD2BINARY (*Hours);
  *DayOfMonth = BCD2BINARY (*DayOfMonth);
  *Month      = BCD2BINARY (*Month);
  *Year       = BCD2BINARY (*Year) + CENTURY_OFFSET;
}

/**
  Gets CPU current time.

  @retval The current CPU time in milliseconds.
**/
UINT64
EFIAPI
GetCpuTime (
  VOID
  )
{
  PCU_CR_PLATFORM_INFO_STRUCT Msr;
  UINT32                      TimeBase;

  Msr.Data = AsmReadMsr64 (PCU_CR_PLATFORM_INFO);
  TimeBase = (1000 * SA_SYSTEM_BCLK) * Msr.Bits.MAX_NON_TURBO_LIM_RATIO; //In Millisec


  return ((TimeBase == 0) ? 0 : DivU64x32 (AsmReadTsc (), TimeBase));
}

/**
  Gets CPU current time in [ns]

  @param[in] GlobalData - Pointer to global MRC data struct.

  @retval The current CPU time in nanoseconds.
**/
UINT64
GetCpuTimeNanoSec (
  IN VOID     *GlobalData
  )
{
  MrcParameters *MrcData;
  UINT64        TimeBase;

  MrcData = (MrcParameters *) GlobalData;
  TimeBase = ((MrcData->Inputs.TscTimeBase == 0) ? 0 : DivU64x64Remainder (MultU64x32 (AsmReadTsc (), 1000), MrcData->Inputs.TscTimeBase, NULL)); // In [ns]

  return TimeBase;
}


/**
  Get CPU time in [us].  Time value is only valid/consistent for the IA core.
  Calling this function on a microcontroller may return a value based on a different
  starting point.
  The value returned after a reboot may be inconsistent with values returned before the
  reboot (essentially, a reboot may choose a different time for the value 0).
  Code returns duplicate values after about 70 minutes.

  @param[in] MrcData pointer to global MRC data

  @returns elapsed time in us since an arbitrary but consistent starting time, modulo 2^32.
*/
UINT32
GetCpuTimeMicroSec(
  IN MrcParameters *MrcData
  )
{
  const UINT64 NsTime = GetCpuTimeNanoSec (MrcData);

  return (UINT32) NsTime / 1000;
}

/**
  Wait for at least the given number of nanoseconds.

  @param[in] GlobalData - Pointer to global MRC data struct.
  @param[in] DelayNs    - time to wait in [ns], up to 2^32 [ns] = 4.29 seconds

  @retval Nothing
**/
VOID
EFIAPI
DelayNs (
  IN VOID         *GlobalData,
  IN UINT32       DelayNs
  )
{
  MrcParameters   *MrcData;
  UINT64          Start;
  volatile UINT64 Finish;
  UINT64          Now;
  BOOLEAN         Done;

  MrcData = (MrcParameters *) GlobalData;
  if (MrcData->Inputs.ExtInputs.Ptr->SimicsFlag) {
    return;
  }

  Start  = GetCpuTimeNanoSec (GlobalData);
  Finish = Start + DelayNs;
  Done   = FALSE;

  do {
    Now = GetCpuTimeNanoSec (GlobalData);
    if (Finish > Start) {
      if (Now >= Finish) {
        Done = TRUE;
      }
    } else {
      if ((Now < Start) && (Now >= Finish)) {
        Done = TRUE;
      }
    }
  } while (Done == FALSE);
}

/**
  Sets the specified number of memory words, a word at a time, at the
  specified destination.

  @param[in, out] Dest     - Destination pointer.
  @param[in]      NumWords - The number of dwords to set.
  @param[in]      Value    - The value to set.

  @retval Pointer to the buffer.
**/
VOID *
EFIAPI
SetMemWord (
  IN OUT VOID     *Dest,
  IN UINTN        NumWords,
  IN const UINT16 Value
  )
{
  UINT16 *Buffer;

  Buffer = (UINT16 *) Dest;
  while (0 != NumWords--) {
    *Buffer++ = Value;
  }

  return (Dest);
}

/**
  Sets the specified number of memory dwords, a dword at a time, at the
  specified destination.

  @param[in, out] Dest      - Destination pointer.
  @param[in]      NumDwords - The number of dwords to set.
  @param[in]      Value     - The value to set.

  @retval Pointer to the buffer.
**/
VOID *
EFIAPI
SetMemDword (
  IN OUT VOID     *Dest,
  IN UINTN        NumDwords,
  IN const UINT32 Value
  )
{
  UINT32 *Buffer;

  Buffer = (UINT32 *) Dest;
  while (0 != NumDwords--) {
    *Buffer++ = Value;
  }

  return (Dest);
}

/**
  This function is used by the OEM to do a dedicated task during the MRC.

  @param[in] GlobalData        - include all the MRC data
  @param[in] Command           - A command that indicates the task to perform.
  @param[in] Pointer           - general ptr for general use.

  @retval The status of the task.
**/
UINT32
EFIAPI
CheckPoint (
  IN VOID        *GlobalData,
  IN UINT32      Command,
  IN VOID        *Pointer
  )
{
  MrcParameters                *MrcData;
  MrcInput                     *Inputs;
  MrcStatus                    Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;
  MEMORY_CONFIG_NO_CRC         *MemConfigNoCrc;
  EFI_STATUS                   Status1;

  //
  // Locate SiPreMemPolicyPpi to do a GetConfigBlock() to access platform data
  //
  Status1 = PeiServicesLocatePpi (
              &gSiPreMemPolicyPpiGuid,
              0,
              NULL,
              (VOID **) &SiPreMemPolicyPpi
              );
  ASSERT_EFI_ERROR (Status1);

  Status1 = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR (Status1);
  MrcData             = (MrcParameters *) GlobalData;
  Inputs              = &MrcData->Inputs;
  SiPreMemPolicyPpi = (SI_PREMEM_POLICY_PPI *)(UINTN) Inputs->SiPreMemPolicyPpi;
  Status              = mrcSuccess;

  switch (Command) {
    default:
      break;
  }

  return (Status);
}

/**
  This API used to Send PostCode to NPK or display to the I/O port 80h.
  @param[in] DisplayDebugNumber - the number to display on port 80.
  @param[in] SimicsFlag         - determines whether or not to enter the simics specific flow for tracing
  @retval Nothing.
**/

VOID
MrcNpkPostCode(
  IN   UINT16        DisplayDebugNumber,
  IN   UINT32        SimicsFlag
  )
{
  TRACEHUB_DATA_HOB            *TraceHubDataHob;
  if (SimicsFlag) {
    TraceHubDataHob = (TRACEHUB_DATA_HOB *) GetFirstGuidHob (&gTraceHubDataHobGuid);
    //expected TraceHub hob is already created
    if (TraceHubDataHob) {
      //Enabled Simics Trace Flag to avoid traces disable in mini sys-T
      TraceHubDataHob->TraceVerbosity = B_TRACE_HUB_MEM_SCRPD2_BIOS_TRACE_EN | (V_TRACE_HUB_MEM_SCRPD2_TRACE_ALL << N_TRACE_HUB_MEM_SCRPD2_TRACE_VERBO);
    }
  }

  //
  //send postcode details TraceHub over Sys-T Lib
  //This also take care of post card display usig IO write to port 0x80
  //
  PostCode (DisplayDebugNumber);
}

/**
  Typically used to display to the I/O port 80h.

  @param[in] GlobalData         - Mrc Global Data
  @param[in] DisplayDebugNumber - the number to display on port 80.

  @retval Nothing.
**/
VOID
EFIAPI
DebugHook (
  IN VOID       *GlobalData,
  UINT16        DisplayDebugNumber
  )
{
  MrcParameters                *MrcData;
  MrcOutput                    *Outputs;
  MrcDebug                     *Debug;

  MrcData = (MrcParameters *) GlobalData;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  Debug->PostCode[MRC_POST_CODE] = DisplayDebugNumber;
  DEBUG ((DEBUG_INFO, "Post Code: %04Xh\n", DisplayDebugNumber));

  MrcNpkPostCode(DisplayDebugNumber, MrcData->Inputs.ExtInputs.Ptr->SimicsFlag);
}

/**
  Hook to take any action after returning from MrcStartMemoryConfiguration()
  and prior to taking any action regarding MrcStatus.  Pre-populated with issuing
  Intel Silicon View Technology (ISVT) checkpoint 0x10.

  @param[in] GlobalData         - Mrc Global Data
  @param[in] MrcStatus          - Mrc status variable
**/
VOID
EFIAPI
ReturnFromSmc (
  IN VOID         *GlobalData,
  IN UINT32       MrcStatus
  )
{
  UINT32          CheckPoint;
  UINT32          PortReading;

  DEBUG ((DEBUG_INFO, "Returned From MrcStartMemoryConfiguration(). MrcStatus = %08Xh\n", MrcStatus));

  //
  // Intel Silicon View Technology (ISVT) IO Reading port with EAX = 0x10 for End of MRC
  //
  CheckPoint = ISVT_END_OF_MRC_STATE;
  PortReading = 0x99;
  IsvtCheckPoint (CheckPoint, PortReading);
}

/**
  Assert or deassert DRAM_RESET# pin; this is used in JEDEC Reset.

  @param[in] PciEBaseAddress  - PCI express base address.
  @param[in] ResetValue       - desired value of DRAM_RESET#. 1 - reset deasserted, 0 - reset asserted.
**/
VOID
SaDramReset (
  IN UINT32 PciEBaseAddress,
  IN UINT32 ResetValue
  )
{
  PmcSetDramResetCtlState (ResetValue);
}
