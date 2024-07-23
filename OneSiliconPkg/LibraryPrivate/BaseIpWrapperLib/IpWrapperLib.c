/** @file
  This file contains standard library for IP wrapper

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

// EDK and SoC includes
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/P2SbSidebandAccessLib.h>

#include <IpCsi.h>
#include <IpWrapperCntxtInfoClient.h>
#include <Library/RegisterAddressTranslationLib.h>

//
// Reg Cntxt static template for IPWR print.
//
CONST IP_WR_PRINT_INFO  mPrintInfoError = {
  DEBUG_ERROR
};
CONST IP_WR_PRINT_INFO  mPrintInfoWarning = {
  DEBUG_WARN
};
CONST IP_WR_PRINT_INFO  mPrintInfoLevel1 = {
  DEBUG_INFO
};
CONST IP_WR_PRINT_INFO  mPrintInfoLevelDefault = {
  DEBUG_ERROR | DEBUG_WARN | DEBUG_INFO
};

//
// Reg Cntxt static template for IPWR memory opration
//
CONST IP_WR_MEM_INFO  mMemInfoBsData = {
  EfiBootServicesData
};
CONST IP_WR_MEM_INFO  mMemInfoRtData = {
  EfiRuntimeServicesData
};
CONST IP_WR_MEM_INFO  mMemInfoRsvdData = {
  EfiReservedMemoryType
};

#define MAX_STR_BUF_SIZE  256

/**
  Read PCI CFG register

  @param[in] Seg         PCI segment number
  @param[in] Bus         PCI bus number
  @param[in] Dev         PCI device number
  @param[in] Fun         PCI function number
  @param[in] Offset      Register offset
  @param[in] RegFlags    Register size

  @retval Data64         Register data
**/
STATIC
UINT64
RegPciCfgRead (
  UINT8            Seg,
  UINT8            Bus,
  UINT8            Dev,
  UINT8            Fun,
  UINT32           Offset,
  IP_WR_REG_FLAGS  RegFlags
  )
{
  UINT64  PciCfgAddress;
  UINT64  Data64 = (UINT64) ~0;

  PciCfgAddress = PCI_SEGMENT_LIB_ADDRESS (Seg, Bus, Dev, Fun, Offset);

  switch (RegFlags & IpWrRegFlagSizeMask) {
    case (IpWrRegFlagSize8Bits):
      Data64 = PciSegmentRead8 (PciCfgAddress);
      break;
    case (IpWrRegFlagSize16Bits):
      Data64 = PciSegmentRead16 (PciCfgAddress);
      break;
    case (IpWrRegFlagSize32Bits):
      Data64 = PciSegmentRead32 (PciCfgAddress);
      break;
    case (IpWrRegFlagSize64Bits):
      switch (RegFlags & IpWrRegFlagOrderMask) {
        case IpWrRegFlagOrderHighToLow:
          Data64  = LShiftU64 (PciSegmentRead32 (PciCfgAddress + 4), 32);
          Data64 |= (UINT64)PciSegmentRead32 (PciCfgAddress);
          break;
        case IpWrRegFlagOrderUndefined:
          DEBUG ((DEBUG_ERROR, "RegPciCfgRead: 64bit reg must specify 'IpWrRegFlagOrder*' in RegFlags\n"));
        // no "break", fall through to IpWrRegFlagOrderHighToLow after warning above
        case IpWrRegFlagOrderLowToHigh:
        default:
          Data64  = (UINT64)PciSegmentRead32 (PciCfgAddress);
          Data64 |= LShiftU64 (PciSegmentRead32 (PciCfgAddress + 4), 32);
          break;
      }

      break;
    default:
      ASSERT (FALSE);
      break;
  }

  return Data64;
}

/**
  Write PCI CFG register

  @param[in] Seg         PCI segment number
  @param[in] Bus         PCI bus number
  @param[in] Dev         PCI device number
  @param[in] Fun         PCI function number
  @param[in] Offset      Register offset
  @param[in] Data64      Register data
  @param[in] RegFlags    Register size

  @retval Data64         Register data
**/
STATIC
void
RegPciCfgWrite (
  UINT8            Seg,
  UINT8            Bus,
  UINT8            Dev,
  UINT8            Fun,
  UINT32           Offset,
  UINT64           Data64,
  IP_WR_REG_FLAGS  RegFlags
  )
{
  UINT64  PciCfgAddress;

  PciCfgAddress = PCI_SEGMENT_LIB_ADDRESS (Seg, Bus, Dev, Fun, Offset);

  switch (RegFlags & IpWrRegFlagSizeMask) {
    case (IpWrRegFlagSize8Bits):
      PciSegmentWrite8 (PciCfgAddress, (UINT8)Data64);
      break;
    case (IpWrRegFlagSize16Bits):
      PciSegmentWrite16 (PciCfgAddress, (UINT16)Data64);
      break;
    case (IpWrRegFlagSize32Bits):
      PciSegmentWrite32 (PciCfgAddress, (UINT32)Data64);
      break;
    case (IpWrRegFlagSize64Bits):
      switch (RegFlags & IpWrRegFlagOrderMask) {
        case IpWrRegFlagOrderHighToLow:
          PciSegmentWrite32 (PciCfgAddress + 4, (UINT32)RShiftU64 (Data64, 32));
          PciSegmentWrite32 (PciCfgAddress, (UINT32)Data64);
          break;
        case IpWrRegFlagOrderUndefined:
          DEBUG ((DEBUG_ERROR, "RegPciCfgWrite: 64bit reg must specify 'IpWrRegFlagOrder*' in RegFlags\n"));
        // no "break", fall through to IpWrRegFlagOrderHighToLow after warning above
        case IpWrRegFlagOrderLowToHigh:
        default:
          PciSegmentWrite32 (PciCfgAddress, (UINT32)Data64);
          PciSegmentWrite32 (PciCfgAddress + 4, (UINT32)RShiftU64 (Data64, 32));
          break;
      }

      break;
    default:
      ASSERT (FALSE);
      break;
  }
}

/**
  Read memory register

  @param[in] MmioBase    MMIO base address
  @param[in] Offset      Register offset
  @param[in] RegFlags    Register size

  @retval Data64         Register data
**/
STATIC
UINT64
RegMemRead (
  UINT64           MmioBase,
  UINT32           Offset,
  IP_WR_REG_FLAGS  RegFlags
  )
{
  UINTN   MmioAddress;
  UINT64  Data64 = (UINT64) ~0;

  MmioAddress = (UINTN)MmioBase + Offset;

  switch (RegFlags & IpWrRegFlagSizeMask) {
    case (IpWrRegFlagSize8Bits):
      Data64 = MmioRead8 (MmioAddress);
      break;
    case (IpWrRegFlagSize16Bits):
      Data64 = MmioRead16 (MmioAddress);
      break;
    case (IpWrRegFlagSize32Bits):
      Data64 = MmioRead32 (MmioAddress);
      break;
    case (IpWrRegFlagSize64Bits):
      Data64 = MmioRead64 (MmioAddress);
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  return Data64;
}

/**
  Write memory register

  @param[in] MmioBase    MMIO base address
  @param[in] Offset      Register offset
  @param[in] Data64      Register data
  @param[in] RegFlags    Register size

  @retval Data64         Register data
**/
STATIC
void
RegMemWrite (
  UINT64           MmioBase,
  UINT32           Offset,
  UINT64           Data64,
  IP_WR_REG_FLAGS  RegFlags
  )
{
  UINTN  MmioAddress;

  MmioAddress = (UINTN)MmioBase + Offset;

  switch (RegFlags & IpWrRegFlagSizeMask) {
    case (IpWrRegFlagSize8Bits):
      MmioWrite8 (MmioAddress, (UINT8)Data64);
      break;
    case (IpWrRegFlagSize16Bits):
      MmioWrite16 (MmioAddress, (UINT16)Data64);
      break;
    case (IpWrRegFlagSize32Bits):
      MmioWrite32 (MmioAddress, (UINT32)Data64);
      break;
    case (IpWrRegFlagSize64Bits):
      MmioWrite64 (MmioAddress, Data64);
      break;
    default:
      ASSERT (FALSE);
      break;
  }
}

/**
  Read sideband register

  @param[in] SbId        Sideband ID
  @param[in] Offset      Register offset
  @param[in] RegFlags    Register size

  @retval Data64         Register data
**/
STATIC
UINT64
RegSbRead (
  UINT32           SbId,
  UINT32           Offset,
  IP_WR_REG_FLAGS  RegFlags
  )
{
  // @todo
  return 0;
}

/**
  Write sideband register

  @param[in] SbId        Sideband ID
  @param[in] Offset      Register offset
  @param[in] Data64      Register data
  @param[in] RegFlags    Register size

  @retval Data64         Register data
**/
STATIC
void
RegSbWrite (
  UINT32           SbId,
  UINT32           Offset,
  UINT64           Data64,
  IP_WR_REG_FLAGS  RegFlags
  )
{
  // @todo
}

/**
  IP Wrapper (IPWR) definition of Print

  @param[in]      pCntxt  IP Wrapper (IPWR) print context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      format  Formated string compatible with C99
  @param[in]      ...     variable lenght params NOTE: this will use vargs best practices!

**/
void
IP_CSI_ATTR_VA_ARG
IpWrPrint (
  IP_WR_PRINT_CNTXT  pCntxt,
  const char         *Format,
  ...
  )
{
  DEBUG_CODE_BEGIN ();
  VA_LIST           Marker;
  IP_WR_PRINT_INFO  *IpWrPrintLevel;
  UINT32            PrintLevel;
  CHAR8             *TempFormat;
  CHAR8             NewFormat[MAX_STR_BUF_SIZE];
  UINT32            Index;
  BOOLEAN           Done;

  if (pCntxt == (IP_WR_PRINT_CNTXT)0) {
    PrintLevel = DEBUG_ERROR | DEBUG_WARN | DEBUG_INFO;
  } else {
    IpWrPrintLevel = (IP_WR_PRINT_INFO *)(UINTN)pCntxt;
    PrintLevel     = IpWrPrintLevel->PrintLevel;
  }

  TempFormat = (CHAR8 *)Format;
  Index      = 0;
  SetMem (NewFormat, sizeof (NewFormat), 0);
  while ((*TempFormat != 0) && (Index < MAX_STR_BUF_SIZE - 1)) {
    NewFormat[Index] = *TempFormat;
    if (*TempFormat == '%') {
      //
      // Parse [flags][width][.precision]
      //
      for (Done = FALSE; !Done; ) {
        Index++;
        TempFormat++;
        switch (*TempFormat) {
          case '.':
          case '-':
          case '+':
          case ' ':
          case ',':
          case '*':
          case '0':
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
          case '8':
          case '9':
            NewFormat[Index] = *TempFormat;
            break;

          default:
            Done = TRUE;
            break;
        }
      }

      //
      // Parse argument type
      //
      switch (*TempFormat) {
        case 'l':
        case 'L':
          //
          // %lc: Unicode, convert to EDKII '%c'
          // %ls: String of unicode, convert to EDKII '%s'
          // %lx: 32bit  value, convert to EDK2 '%x'. Valid for type X, x, and d.
          // %llx: 64bit value, convert to EDKII '%lx'. valid for types X, x, and d.
          // In conclusion it reduce one 'l' or 'L' char.
          //
          TempFormat++;
          NewFormat[Index] = *TempFormat;
          break;

        case 's':
          //
          // %s: String of characters, convert to EDKII '%a'
          //
          NewFormat[Index] = 'a';
          break;

        default:
          NewFormat[Index] = *TempFormat;
          break;
      }
    }

    Index++;
    TempFormat++;
  }

  NewFormat[Index] = '\0';

  VA_START (Marker, Format);
  DebugVPrint (PrintLevel, NewFormat, Marker);
  VA_END (Marker);
  DEBUG_CODE_END ();
}

/**
  Get RAT offset size from RegFlag

  @param[in] RegFlags    Register flag

  @retval    The size index of the register offset.
**/
STATIC
REGISTER_WIDTH_BYTES
GetRatOffsetSize (
  IP_WR_REG_FLAGS  RegFlags
  )
{
  STATIC IP_WR_REG_FLAGS       SizeMap[]        = { IpWrRegFlagSize8Bits, IpWrRegFlagSize16Bits, IpWrRegFlagSize32Bits, IpWrRegFlagSize64Bits };
  STATIC REGISTER_WIDTH_BYTES  RatRegWidthMap[] = { RegisterWidthByte, RegisterWidthWord, RegisterWidthDWord, RegisterWidthQWord };
  UINT8                        Index;

  for (Index = 0; Index < (sizeof (SizeMap) / sizeof (IP_WR_REG_FLAGS)); Index++) {
    if (SizeMap[Index] == (RegFlags & IpWrRegFlagSizeMask)) {
      return RatRegWidthMap[Index];
    }
  }

  DEBUG ((DEBUG_ERROR, "GetRatOffsetSize: Cannot find matched register size. RegFlags: %08x\n", RegFlags));
  //
  // Set default size to 32 bits in case
  //
  return RegisterWidthDWord;
}

/**
  Read sideband register

  @param[in] SbRegAcc    Pointer to P2SB Register Access structure
  @param[in] Offset      Register offset
  @param[in] RegFlags    Register size

  @retval Data64         Register data
**/
STATIC
UINT64
RegP2SbRead (
  IP_WR_P2SB_SIDEBAND_REGISTER_ACCESS  *SbRegAcc,
  UINT32                               Offset,
  IP_WR_REG_FLAGS                      RegFlags
  )
{
  UINT64 Data64 = (UINT64)~0;

  switch (RegFlags & IpWrRegFlagSizeMask) {
    case (IpWrRegFlagSize8Bits):
      Data64 = P2SbSidebandRead8 ((REGISTER_ACCESS *)SbRegAcc, Offset);
      break;
    case (IpWrRegFlagSize16Bits):
      Data64 = P2SbSidebandRead16 ((REGISTER_ACCESS *)SbRegAcc, Offset);
      break;
    case (IpWrRegFlagSize32Bits):
      Data64 = P2SbSidebandRead32 ((REGISTER_ACCESS *)SbRegAcc, Offset);
      break;
    case (IpWrRegFlagSize64Bits):
      DEBUG ((DEBUG_ERROR, "P2SB: Incorrect access \n"));
      ASSERT (FALSE);
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  return Data64;
}
/**
  Register read IP Wrapper (IPWR)

  @param[in]      pCntxt    IP Wrapper (IPWR) register group context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      offset    Byte offset of reg
  @param[in]      RegFlags  OR'ed values of IP_WR_REG_FLAGS. at least "IpWrRegFlagSize*" is required for size

  @retval         The data read from the Offset of the provided Context.
**/
UINT64
IpWrRegRead (
  IP_WR_REG_CNTXT  pCntxt,
  UINT32           Offset,
  UINT32           RegFlags
  )
{
  IP_WR_REG_INFO    *IpWrRegInfo;
  UINT64            Data64 = (UINT64) ~0;
  IP_WR_REG_ACCESS  AccessType;

  DEBUG ((DEBUG_VERBOSE, "IpWrRegRead %x %x\n", Offset, RegFlags));

  if (pCntxt == (IP_WR_REG_CNTXT)0) {
    DEBUG ((DEBUG_ERROR, "pCntxt is NULL\n"));
    ASSERT (FALSE);
    return (UINT64) ~0;
  }

  IpWrRegInfo = (IP_WR_REG_INFO *)(UINTN)pCntxt;

  if (IpWrRegInfo->OverrideAccessType != IpWrRegAccessDefault) {
    AccessType = IpWrRegInfo->OverrideAccessType;
  } else {
    AccessType = IpWrRegInfo->AccessType;
  }

  switch (AccessType) {
    case (IpWrRegAccessCfg):
      if (IpWrRegInfo->AccessSubType == IpWrRegSubTypeRat) {
        Data64 = ReadIpRegister (
                   IpWrRegInfo->RegType.Rat.Socket,
                   IpWrRegInfo->RegType.Rat.Id,
                   IpWrRegInfo->RegType.Rat.Instance,
                   RegisterSpaceCfg,
                   Offset,
                   GetRatOffsetSize (RegFlags)
                   );
      } else {
        Data64 = RegPciCfgRead (
                  IpWrRegInfo->RegType.Pci.Seg,
                  IpWrRegInfo->RegType.Pci.Bus,
                  IpWrRegInfo->RegType.Pci.Dev,
                  IpWrRegInfo->RegType.Pci.Fun,
                  Offset,
                  RegFlags
                  );
      }
      break;
    case (IpWrRegAccessMem):
      if (IpWrRegInfo->AccessSubType == IpWrRegSubTypeRat) {
        Data64 = ReadIpRegister (
                   IpWrRegInfo->RegType.Rat.Socket,
                   IpWrRegInfo->RegType.Rat.Id,
                   IpWrRegInfo->RegType.Rat.Instance,
                   RegisterSpaceMemBar,
                   Offset,
                   GetRatOffsetSize (RegFlags)
                   );
      } else {
        Data64 = RegMemRead (
                  IpWrRegInfo->RegType.MmioBase,
                  Offset,
                  RegFlags
                  );
      }
      break;
    case (IpWrRegAccessSb):
      if (IpWrRegInfo->AccessSubType == IpWrRegAccessP2Sb) {
        Data64 = RegP2SbRead (
                  &(IpWrRegInfo->RegType.P2Sb),
                  Offset,
                  RegFlags
                  );
      } else {
        Data64 = RegSbRead (
                  IpWrRegInfo->RegType.SbId,
                  Offset,
                  RegFlags
                  );
      }
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  return Data64;
}

/**
  Write sideband register

  @param[in] SbRegAcc    Pointer to P2SB Register Access structure
  @param[in] Offset      Register offset
  @param[in] Data64      Register data
  @param[in] RegFlags    Register size

  @retval Data64         Register data
**/
STATIC
void
RegP2SbWrite (
  IP_WR_P2SB_SIDEBAND_REGISTER_ACCESS  *SbRegAcc,
  UINT32                               Offset,
  UINT64                               Data64,
  IP_WR_REG_FLAGS                      RegFlags
  )
{
  switch (RegFlags & IpWrRegFlagSizeMask) {
    case (IpWrRegFlagSize8Bits):
      P2SbSidebandWrite8 ((REGISTER_ACCESS *)SbRegAcc, Offset, (UINT8) Data64);
      break;
    case (IpWrRegFlagSize16Bits):
      P2SbSidebandWrite16 ((REGISTER_ACCESS *)SbRegAcc, Offset, (UINT16) Data64);
      break;
    case (IpWrRegFlagSize32Bits):
       P2SbSidebandWrite32 ((REGISTER_ACCESS *)SbRegAcc, Offset, (UINT32) Data64);
      break;
    case (IpWrRegFlagSize64Bits):
      DEBUG ((DEBUG_ERROR, "P2SB: Incorrect access \n"));
      ASSERT (FALSE);
      break;
    default:
      ASSERT (FALSE);
      break;
  }
}

/**
  Register write IP Wrapper (IPWR)

  @param[in]      pCntxt    IP Wrapper (IPWR) register group context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      offset    Byte offset of reg
  @param[in]      data      Data to be written
  @param[in]      RegFlags  OR'ed values of IP_WR_REG_FLAGS. at least "IpWrRegFlagSize*" is required for size

**/
UINT64
IpWrRegWrite (
  IP_WR_REG_CNTXT  pCntxt,
  UINT32           Offset,
  UINT64           Data,
  UINT32           RegFlags
  )
{
  IP_WR_REG_INFO    *IpWrRegInfo;
  IP_WR_REG_ACCESS  AccessType;

  DEBUG ((DEBUG_VERBOSE, "IpWrRegWrite %x %x %x\n", Offset, Data, RegFlags));

  if (pCntxt == (IP_WR_REG_CNTXT)0) {
    DEBUG ((DEBUG_ERROR, "pCntxt is NULL\n"));
    ASSERT (FALSE);
    return 0;
  }

  IpWrRegInfo = (IP_WR_REG_INFO *)(UINTN)pCntxt;

  if (IpWrRegInfo->OverrideAccessType != IpWrRegAccessDefault) {
    AccessType = IpWrRegInfo->OverrideAccessType;
  } else {
    AccessType = IpWrRegInfo->AccessType;
  }

  switch (AccessType) {
    case (IpWrRegAccessCfg):
      if (IpWrRegInfo->AccessSubType == IpWrRegSubTypeRat) {
        WriteIpRegister (
          IpWrRegInfo->RegType.Rat.Socket,
          IpWrRegInfo->RegType.Rat.Id,
          IpWrRegInfo->RegType.Rat.Instance,
          RegisterSpaceCfg,
          Offset,
          Data,
          GetRatOffsetSize (RegFlags)
          );
      } else {
        RegPciCfgWrite (
          IpWrRegInfo->RegType.Pci.Seg,
          IpWrRegInfo->RegType.Pci.Bus,
          IpWrRegInfo->RegType.Pci.Dev,
          IpWrRegInfo->RegType.Pci.Fun,
          Offset,
          Data,
          RegFlags
          );
      }
      break;
    case (IpWrRegAccessMem):
      if (IpWrRegInfo->AccessSubType == IpWrRegSubTypeRat) {
        WriteIpRegister (
          IpWrRegInfo->RegType.Rat.Socket,
          IpWrRegInfo->RegType.Rat.Id,
          IpWrRegInfo->RegType.Rat.Instance,
          RegisterSpaceMemBar,
          Offset,
          Data,
          GetRatOffsetSize (RegFlags)
          );
      } else {
        RegMemWrite (
          IpWrRegInfo->RegType.MmioBase,
          Offset,
          Data,
          RegFlags
          );
      }
      break;
    case (IpWrRegAccessSb):
      if (IpWrRegInfo->AccessSubType == IpWrRegAccessP2Sb) {
        RegP2SbWrite (
          &(IpWrRegInfo->RegType.P2Sb),
          Offset,
          Data,
          RegFlags
          );
      } else {
        RegSbWrite (
          IpWrRegInfo->RegType.SbId,
          Offset,
          Data,
          RegFlags
          );
      }
      break;
    default:
      ASSERT (FALSE);
      break;
  }

  return Data;
}

/**
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - The Mask and Match criteria have been met
    - The number of retries is exceeded
  Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match)
  @param[in]      Match          Exit polling when ((regRead & Mask) == Match)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for delay
  @param[in]      DelayMicroSec  Minimum microsecond delay per retry. Can be 0.
  @param[in,out]  pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPollUs (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayMicroSec,
  IP_CSI_STATUS     *pApiSts
  )
{
  IP_CSI_STATUS  TempSts;
  UINT32         Try = 0;
  UINT64         Data;

  if (pRegCntxt == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return;
  }

  if (pApiSts == NULL) {
    pApiSts = &TempSts;
  }

  *pApiSts = IpCsiStsSuccess;

  DEBUG ((DEBUG_VERBOSE, "IpWrRegPollUs: mask=0x%x match=0x%x:\n", (UINT32)Mask, (UINT32)Match));

  while (1) {
    Data = IpWrRegRead (pRegCntxt, Offset, RegFlags);
    DEBUG ((DEBUG_VERBOSE, " -Data=0x%x Try=%d/%d:\n", (UINT32)Data, Try, MaxRetry));
    if ((Data & Mask) == Match) {
      break;
    }

    if (MaxRetry != 0) {
      Try++;
      if (Try == MaxRetry) {
        *pApiSts = IpCsiStsErrorTimeout;
        break;
      }
    }

    if (DelayMicroSec > 0) {
      IpWrDelayUs (pTimeCntxt, DelayMicroSec);
    }
  }
}

/**
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - The Mask and Match criteria have been met
    - The number of retries is exceeded
  Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match)
  @param[in]      Match          Exit polling when ((regRead & Mask) == Match)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for Delay
  @param[in]      DelayNanoSec   minimum nanosecond delay per retry. Can be 0.
  @param[in,out]  pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPollNs (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayNanoSec,
  IP_CSI_STATUS     *pApiSts
  )
{
  IP_CSI_STATUS  TempSts;
  UINT32         Try = 0;
  UINT64         Data;

  if (pRegCntxt == (IP_WR_REG_CNTXT)0) {
    ASSERT (FALSE);
    return;
  }

  if (pApiSts == NULL) {
    pApiSts = &TempSts;
  }

  *pApiSts = IpCsiStsSuccess;

  DEBUG ((DEBUG_VERBOSE, "IpWrRegPollNs: mask=0x%x match=0x%x:\n", (UINT32)Mask, (UINT32)Match));

  while (1) {
    Data = IpWrRegRead (pRegCntxt, Offset, RegFlags);
    DEBUG ((DEBUG_VERBOSE, " -Data=0x%x Try=%d/%d:\n", (UINT32)Data, Try, MaxRetry));
    if ((Data & Mask) == Match) {
      break;
    }

    if (MaxRetry != 0) {
      Try++;
      if (Try == MaxRetry) {
        *pApiSts = IpCsiStsErrorTimeout;
        break;
      }
    }

    if (DelayNanoSec > 0) {
      IpWrDelayNs (pTimeCntxt, DelayNanoSec);
    }
  }
}

/**
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - ((regRead & Mask) == Match1)
    - ((regRead & Mask) == Match2)
    - The number of retries is exceeded
  Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match1) || ((regRead & Mask) == Match2)
  @param[in]      Match1         Exit polling when ((regRead & Mask) == Match1)
  @param[in]      Match2         Exit polling when ((regRead & Mask) == Match2)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for delay
  @param[in]      DelayMicroSec  Minimum microsecond delay per retry. Can be 0.
  @param[in,out]  pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPoll2ValUs (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match1,
  UINT64            Match2,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayMicroSec,
  IP_CSI_STATUS     *pApiSts
  )
{
  UINT64         Data;
  UINT32         Try = 0;
  IP_CSI_STATUS  TempSts;

  if (pApiSts == NULL) {
    pApiSts = &TempSts;
  }

  *pApiSts = IpCsiStsSuccess;

  DEBUG ((DEBUG_VERBOSE, "IpWrRegPoll2ValUs: mask=0x%x match1=0x%x match2=0x%x:\n", (UINT32)Mask, (UINT32)Match1, (UINT32)Match2));

  while (1) {
    Data = IpWrRegRead (pRegCntxt, Offset, RegFlags);
    DEBUG ((DEBUG_VERBOSE, "  Data=0x%x Try=%d/%d:\n", (UINT32)Data, Try, MaxRetry));
    if (((Data & Mask) == Match1) || ((Data & Mask) == Match2)) {
      break;
    }

    if (MaxRetry != 0) {
      Try++;
      if (Try == MaxRetry) {
        *pApiSts = IpCsiStsErrorTimeout;
        break;
      }
    }

    if (DelayMicroSec > 0) {
      IpWrDelayUs (pTimeCntxt, DelayMicroSec);
    }
  }
}

/**
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - ((regRead & Mask) == Match1)
    - ((regRead & Mask) == Match2)
    - The number of retries is exceeded
 Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match)
  @param[in]      Match          Exit polling when ((regRead & Mask) == Match)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for Delay
  @param[in]      DelayNanoSec   minimum nanosecond delay per retry. Can be 0.
  @param[in,out]  pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPoll2ValNs (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match1,
  UINT64            Match2,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayNanoSec,
  IP_CSI_STATUS     *pApiSts
  )
{
  UINT64         Data;
  UINT32         Try = 0;
  IP_CSI_STATUS  TempSts;

  if (pApiSts == NULL) {
    pApiSts = &TempSts;
  }

  *pApiSts = IpCsiStsSuccess;

  DEBUG ((DEBUG_VERBOSE, "IpWrRegPoll2ValNs: mask=0x%x match1=0x%x match2=0x%x:\n", (UINT32)Mask, (UINT32)Match1, (UINT32)Match2));

  while (1) {
    Data = IpWrRegRead (pRegCntxt, Offset, RegFlags);
    DEBUG ((DEBUG_VERBOSE, "  Data=0x%x Try=%d/%d:\n", (UINT32)Data, Try, MaxRetry));
    if (((Data & Mask) == Match1) || ((Data & Mask) == Match2)) {
      break;
    }

    if (MaxRetry != 0) {
      Try++;
      if (Try == MaxRetry) {
        *pApiSts = IpCsiStsErrorTimeout;
        break;
      }
    }

    if (DelayNanoSec > 0) {
      IpWrDelayNs (pTimeCntxt, DelayNanoSec);
    }
  }
}

/**
  Register access change IP Wrapper (IPWR)

  @param[in]      pCntxt      IP Wrapper (IPWR) register group context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      AccessType  Requested access type

  @retval         == IpCsiStsSuccess   Success
  @retval         != IpCsiStsSuccess   Failed
**/
IP_CSI_STATUS
IpWrSetRegAccess (
  IP_WR_REG_CNTXT   pCntxt,
  IP_WR_REG_ACCESS  AccessType
  )
{
  IP_WR_REG_INFO  *IpWrRegInfo;

  DEBUG ((DEBUG_INFO, "IpWrSetRegAccess %x\n", AccessType));

  if (pCntxt == (IP_WR_REG_CNTXT)0) {
    return IpCsiStsError;
  }

  IpWrRegInfo = (IP_WR_REG_INFO *)(UINTN)pCntxt;

  //
  // Need to verify if the RegGroup can support the request access type
  // This is IP dependent.
  // Here it assuemes it can always modify the access type.
  //
  IpWrRegInfo->OverrideAccessType = AccessType;

  return IpCsiStsSuccess;
}

/**
  Return after a microsecond delay that is at least MinMicroSec in duration.
  May "sleep" if supported
  Time Accuracy is limited by the environment.

  @param[in]      pCntxt       Time context (opaque to IPFW)
  @param[in]      MinMicrosec  The minimum number of micro seconds to delay for

**/
void
IpWrDelayUs (
  IP_WR_TIME_CNTXT  pCntxt,
  UINT32            MinMicroSec
  )
{
  MicroSecondDelay (MinMicroSec);
}

/**
  Return after a nanosecond delay that is at least MinNanoSec in duration.
  May "sleep" if supported
  Time Accuracy is limited by the environment.

  @param[in]  pTimeCntxt     Time context (opaque to IPFW)
  @param[in]  MinNanoSec     The minimum number of nano seconds to delay for.
**/
void
IpWrDelayNs (
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            MinNanoSec
  )
{
  NanoSecondDelay (MinNanoSec);
}

/**
  Mark/Note/Capture current time and return its opaque value.
  Return value used with IpWrDelayFromTime*(), etc.

  @param[in]      pTimeCntxt     Time context (opaque to IPFW)

  @return     current opaque time value (IP_WR_TIME_VALUE)
              units of this value are opaque to IPFW
**/
IP_WR_TIME_VALUE
IpWrMarkTime (
  IP_WR_TIME_CNTXT  pTimeCntxt
  )
{
  return (IP_WR_TIME_VALUE)GetPerformanceCounter ();
}

/**
  Waits for at least NumUs microseconds after Time and then returns. May "sleep" if supported
  Time Accuracy is limited by the environment.
  @param[in]      pTimeCntxt     Time context (opaque to IPFW)
  @param[in]      Time           time to start the wait from
  @param[in]      NumUs          minimum # of microseconds to wait after Time
**/
void
IpWrDelayFromTimeUs (
  IP_WR_TIME_CNTXT  pTimeCntxt,
  IP_WR_TIME_VALUE  Time,
  UINT32            NumUs
  )
{
  UINT64  CurrentTime;
  UINT64  StartTime;
  UINT64  PassedNs;
  UINT64  RemainingNs;

  DEBUG ((DEBUG_VERBOSE, "IpWrDelayFromTimeUs %x:%x %x\n", (UINT32)RShiftU64 ((UINT64)Time, 32), (UINT32)Time, NumUs));
  CurrentTime = GetPerformanceCounter ();
  CurrentTime = (UINT64)(IP_WR_TIME_VALUE) CurrentTime;
  StartTime   = (UINT64) Time;
  // Handle timer overflow
  if (StartTime > CurrentTime) {
    CurrentTime += (IP_WR_TIME_VALUE)~0;
  }
  if (StartTime >= CurrentTime) {
    return;
  }
  PassedNs = GetTimeInNanoSecond (CurrentTime - StartTime);
  if (PassedNs >= MultU64x32 (NumUs, 1000)) {
    return;
  }
  RemainingNs = MultU64x32 (NumUs, 1000) - PassedNs;
  IpWrDelayNs (pTimeCntxt, (UINT32)RemainingNs);
}

/**
  Waits for at least NumNs nano seconds after Time and then returns. May "sleep" if supported
  Time Accuracy is limited by the environment.
  @param[in]      pTimeCntxt     Time context (opaque to IPFW)
  @param[in]      Time           time to start the wait from
  @param[in]      NumNs          minimum # of nano seconds to wait after Time
**/
void
IpWrDelayFromTimeNs (
  IP_WR_TIME_CNTXT  pTimeCntxt,
  IP_WR_TIME_VALUE  Time,
  UINT32            NumNs
  )
{
  UINT64  CurrentTime;
  UINT64  StartTime;
  UINT64  PassedNs;
  UINT64  RemainingNs;

  DEBUG ((DEBUG_VERBOSE, "IpWrDelayFromTimeNs %x:%x %x\n", (UINT32)RShiftU64 ((UINT64)Time, 32), (UINT32)Time, NumNs));
  CurrentTime = GetPerformanceCounter ();
  CurrentTime = (UINT64)(IP_WR_TIME_VALUE) CurrentTime;
  StartTime   = (UINT64) Time;
  // Handle timer overflow
  if (StartTime > CurrentTime) {
    CurrentTime += (IP_WR_TIME_VALUE)~0;
  }
  if (StartTime >= CurrentTime) {
    return;
  }
  PassedNs = GetTimeInNanoSecond (CurrentTime - StartTime);
  if (PassedNs >= (UINT64)NumNs) {
    return;
  }
  RemainingNs = (UINT64)NumNs - PassedNs;
  IpWrDelayNs (pTimeCntxt, (UINT32)RemainingNs);
}

/**
  IP Wrapper (IPWR) definition of Error

  @param[in]      pCntxt   IP Wrapper (IPWR) error context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      Major    Major error type uses IP_WR_ERROR_MAJOR or IP_IPNAME_ERROR_MAJOR
  @param[in]      Minor    Minor defined by Major. May be IP_WR_ERROR_MINOR, IP_IPNAME_ERROR_MINOR, or UINT32

**/
void
IpWrError (
  IP_WR_ERROR_CNTXT  pCntxt,
  UINT32             Major,
  UINT32             Minor
  )
{
  DEBUG ((DEBUG_INFO, "IpWrError %x %x\n", Major, Minor));
  if (Major == IpWrErrorMajorAssert) {
    ASSERT (FALSE);
    CpuDeadLoop ();
  }
}

/**
  IP Wrapper (IPWR) definition of Memset

  @param[in]      pCntxt    IP Wrapper (IPWR) memory context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      pMem      The address of the memory to be set
  @param[in]      Value     The byte value to use
  @param[in]      NumBytes  Number of bytes to set in pMem

  @retval         pMem     The address of the memory that was set
**/
void *
IpWrMemset (
  IP_WR_MEM_CNTXT  pCntxt,
  void             *pMem,
  UINT8            Value,
  UINT32           NumBytes
  )
{
  SetMem (pMem, NumBytes, Value);
  return pMem;
}

/**
  IP Wrapper (IPWR) definition of Memcpy

  @param[in]      pCntxt    IP Wrapper (IPWR) memory context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      pDest     Destination address in memory to copy to.
  @param[in]      pSrc      Source address in memory to copy from.
  @param[in]      NumBytes  Number of bytes to copy.

  @retval         pDest     The address in memory that NumBytes of pSrc was copied to
**/
void *
IpWrMemcpy (
  IP_WR_MEM_CNTXT  pCntxt,
  void             *pDest,
  const void       *pSrc,
  UINT32           NumBytes
  )
{
  CopyMem (pDest, pSrc, NumBytes);
  return pDest;
}

/**
  IP Wrapper (IPWR) definition of Malloc

  @param[in]      pCntxt   IP Wrapper (IPWR) memory context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      pInst    The instance
  @param[in]      Size     Number of bytes to allocate

  @retval         NULL      An error occured
  @retval         Non-Zero  Success
**/
void *
IpWrMalloc (
  IP_WR_MEM_CNTXT  pCntxt,
  UINT32           Size
  )
{
  IP_WR_MEM_INFO  *IpWrMemInfo;

  if (pCntxt == (IP_WR_MEM_CNTXT)0) {
    return NULL;
  }

  IpWrMemInfo = (IP_WR_MEM_INFO *)(UINTN)pCntxt;

  switch (IpWrMemInfo->MemoryType) {
    case (EfiReservedMemoryType):
      return AllocateReservedZeroPool (Size);
    case (EfiBootServicesData):
      return AllocateZeroPool (Size);
    case (EfiRuntimeServicesData):
      return AllocateRuntimeZeroPool (Size);
    default:
      ASSERT (FALSE);
      return NULL;
  }
}

/**
  IP Wrapper (IPWR) definition of Free

  @param[in]      pCntxt IP Wrapper (IPWR) memory context. Opaque to IP FW and defined by IP Wrapper (IPWR)
  @param[in]      Ptr    Address in memory to free

**/
void
IpWrFree (
  IP_WR_MEM_CNTXT  pCntxt,
  void             *Ptr
  )
{
  FreePool (Ptr);
}