/** @file
  This file implements a Setup callback funtion to update the
  Expected CPU frequency with respect to BusRatio and BusSpeed.

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

#include <OverClockSetup.h>
#include <Protocol/SmbusHc.h>
#include <Library/HiiLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#if FixedPcdGetBool(PcdOverclockEnable) == 1
#include <Library/MemoryAllocationLib.h>
#include <Library/OcPlatformLib.h>
#include <Library/DccProgramLib.h>
#include <Library/HobLib.h>
#include <OcDataHob.h>
#include <Library/OcMailboxLib.h>
#endif
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/GenerationMsr.h>
#include <Protocol/MpService.h>
#include <Register/B2pMailbox.h>

#define MAX_EFFICIENCY_RATIO_MASK           0xff
#define MAX_EFFICIENCY_RATIO_OFFSET         40
#define MAX_NON_TURBO_RATIO_OFFSET          8
#define MAX_NON_TURBO_RATIO_MASK            0xff
//
// Max valid bits in Strap Set 3 data.
//
#define OC_CPU_CORE_DISABLE_MASK_BITS 40

GLOBAL_REMOVE_IF_UNREFERENCED CHAR16 Timing5cString[] = L"  % 5d   % 5d   % 5d   % 5d   % 5d   % 5d   % 5d";
GLOBAL_REMOVE_IF_UNREFERENCED CHAR16 Timing7cString[] = L"% 7d % 7d % 7d % 7d % 7d % 7d % 7d";

UINT16    mIaIccMax;
UINT16    mGtIccMax;

/**
                                                                   // Ratio | Ratio
GLOBAL_REMOVE_IF_UNREFERENCED const TRangeTable FreqTable[] = {    // 133   | 100
  { 0xFFFFFFFF,            fInvalid, MRC_FREQ_INVALID           }, //----------------
  { MRC_DDR_800_TCK_MIN,  f800,     MRC_FREQ_133 | MRC_FREQ_100 }, //   6   |   8
  { MRC_DDR_1000_TCK_MIN, f1000,                   MRC_FREQ_100 }, //       |  10
  { MRC_DDR_1067_TCK_MIN, f1067,    MRC_FREQ_133                }, //   8   |
  { MRC_DDR_1100_TCK_MIN, f1100,                   MRC_FREQ_100 }, //       |  11
  { MRC_DDR_1200_TCK_MIN, f1200,    MRC_FREQ_133 | MRC_FREQ_100 }, //   9   |  12
  { MRC_DDR_1300_TCK_MIN, f1300,                   MRC_FREQ_100 }, //       |  13
  { MRC_DDR_1333_TCK_MIN, f1333,    MRC_FREQ_133                }, //  10   |
  { MRC_DDR_1400_TCK_MIN, f1400,                   MRC_FREQ_100 }, //       |  14
  { MRC_DDR_1467_TCK_MIN, f1467,    MRC_FREQ_133                }, //  11   |
  { MRC_DDR_1500_TCK_MIN, f1500,                   MRC_FREQ_100 }, //       |  15
  { MRC_DDR_1600_TCK_MIN, f1600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  12   |  16
  { MRC_DDR_1700_TCK_MIN, f1700,                   MRC_FREQ_100 }, //       |  17
  { MRC_DDR_1733_TCK_MIN, f1733,    MRC_FREQ_133                }, //  13   |
  { MRC_DDR_1800_TCK_MIN, f1800,                   MRC_FREQ_100 }, //       |  18
  { MRC_DDR_1867_TCK_MIN, f1867,    MRC_FREQ_133                }, //  14   |
  { MRC_DDR_1900_TCK_MIN, f1900,                   MRC_FREQ_100 }, //       |  19
  { MRC_DDR_2000_TCK_MIN, f2000,    MRC_FREQ_133 | MRC_FREQ_100 }, //  15   |  20
  { MRC_DDR_2100_TCK_MIN, f2100,                   MRC_FREQ_100 }, //       |  21
  { MRC_DDR_2133_TCK_MIN, f2133,    MRC_FREQ_133                }, //  16   |
  { MRC_DDR_2200_TCK_MIN, f2200,                   MRC_FREQ_100 }, //       |  22
  { MRC_DDR_2267_TCK_MIN, f2267,    MRC_FREQ_133                }, //  17   |
  { MRC_DDR_2300_TCK_MIN, f2300,                   MRC_FREQ_100 }, //       |  23
  { MRC_DDR_2400_TCK_MIN, f2400,    MRC_FREQ_133 | MRC_FREQ_100 }, //  18   |  24
  { MRC_DDR_2500_TCK_MIN, f2500,                   MRC_FREQ_100 }, //       |  25
  { MRC_DDR_2533_TCK_MIN, f2533,    MRC_FREQ_133                }, //  19   |
  { MRC_DDR_2600_TCK_MIN, f2600,                   MRC_FREQ_100 }, //       |  26
  { MRC_DDR_2667_TCK_MIN, f2667,    MRC_FREQ_133                }, //  20   |
  { MRC_DDR_2700_TCK_MIN, f2700,                   MRC_FREQ_100 }, //       |  27
  { MRC_DDR_2800_TCK_MIN, f2800,    MRC_FREQ_133 | MRC_FREQ_100 }, //  21   |  28
  { MRC_DDR_2900_TCK_MIN, f2900,                   MRC_FREQ_100 }, //       |  29
  { MRC_DDR_2933_TCK_MIN, f2933,    MRC_FREQ_133                }, //  22   |
  { MRC_DDR_3000_TCK_MIN, f3000,                   MRC_FREQ_100 }, //       |  30
  { MRC_DDR_3067_TCK_MIN, f3067,    MRC_FREQ_133                }, //  23   |
  { MRC_DDR_3100_TCK_MIN, f3100,                   MRC_FREQ_100 }, //       |  31
  { MRC_DDR_3200_TCK_MIN, f3200,    MRC_FREQ_133 | MRC_FREQ_100 }, //  24   |  32
  { MRC_DDR_3300_TCK_MIN, f3300,                   MRC_FREQ_100 }, //       |  33
  { MRC_DDR_3333_TCK_MIN, f3333,    MRC_FREQ_133                }, //  25   |
  { MRC_DDR_3400_TCK_MIN, f3400,                   MRC_FREQ_100 }, //       |  34
  { MRC_DDR_3467_TCK_MIN, f3467,    MRC_FREQ_133                }, //  26   |
  { MRC_DDR_3500_TCK_MIN, f3500,                   MRC_FREQ_100 }, //       |  35
  { MRC_DDR_3600_TCK_MIN, f3600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  27   |  36
  { MRC_DDR_3700_TCK_MIN, f3700,                   MRC_FREQ_100 }, //       |  37
  { MRC_DDR_3733_TCK_MIN, f3733,    MRC_FREQ_133                }, //  28   |
  { MRC_DDR_3800_TCK_MIN, f3800,                   MRC_FREQ_100 }, //       |  38
  { MRC_DDR_3867_TCK_MIN, f3867,    MRC_FREQ_133                }, //  29   |
  { MRC_DDR_3900_TCK_MIN, f3900,                   MRC_FREQ_100 }, //       |  39
  { MRC_DDR_4000_TCK_MIN, f4000,    MRC_FREQ_133 | MRC_FREQ_100 }, //  30   |  40
  { MRC_DDR_4100_TCK_MIN, f4100,                   MRC_FREQ_100 }, //       |  41
  { MRC_DDR_4133_TCK_MIN, f4133,    MRC_FREQ_133                }, //  31   |
  { MRC_DDR_4200_TCK_MIN, f4200,                   MRC_FREQ_100 }, //       |  42
  { MRC_DDR_4267_TCK_MIN, f4267,    MRC_FREQ_133                }, //  32   |
  { MRC_DDR_4300_TCK_MIN, f4300,                   MRC_FREQ_100 }, //       |  43
  { MRC_DDR_4400_TCK_MIN, f4400,    MRC_FREQ_133 | MRC_FREQ_100 }, //  33   |  44
  { MRC_DDR_4500_TCK_MIN, f4500,                   MRC_FREQ_100 }, //       |  45
  { MRC_DDR_4533_TCK_MIN, f4533,    MRC_FREQ_133                }, //  34   |
  { MRC_DDR_4600_TCK_MIN, f4600,                   MRC_FREQ_100 }, //       |  46
  { MRC_DDR_4667_TCK_MIN, f4667,    MRC_FREQ_133                }, //  35   |
  { MRC_DDR_4700_TCK_MIN, f4700,                   MRC_FREQ_100 }, //       |  47
  { MRC_DDR_4800_TCK_MIN, f4800,    MRC_FREQ_133 | MRC_FREQ_100 }, //  36   |  48
  { MRC_DDR_4900_TCK_MIN, f4900,                   MRC_FREQ_100 }, //       |  49
  { MRC_DDR_4933_TCK_MIN, f4933,    MRC_FREQ_133                }, //  37   |
  { MRC_DDR_5000_TCK_MIN, f5000,                   MRC_FREQ_100 }, //       |  50
  { MRC_DDR_5067_TCK_MIN, f5067,    MRC_FREQ_133                }, //  38   |
  { MRC_DDR_5100_TCK_MIN, f5100,                   MRC_FREQ_100 }, //       |  51
  { MRC_DDR_5200_TCK_MIN, f5200,    MRC_FREQ_133 | MRC_FREQ_100 }, //  39   |  52
  { MRC_DDR_5300_TCK_MIN, f5300,                   MRC_FREQ_100 }, //       |  53
  { MRC_DDR_5333_TCK_MIN, f5333,    MRC_FREQ_133                }, //  40   |
  { MRC_DDR_5400_TCK_MIN, f5400,                   MRC_FREQ_100 }, //       |  54
  { MRC_DDR_5467_TCK_MIN, f5467,    MRC_FREQ_133                }, //  41   |
  { MRC_DDR_5500_TCK_MIN, f5500,                   MRC_FREQ_100 }, //       |  55
  { MRC_DDR_5600_TCK_MIN, f5600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  42   |  56
  { MRC_DDR_5700_TCK_MIN, f5700,                   MRC_FREQ_100 }, //       |  57
  { MRC_DDR_5733_TCK_MIN, f5733,    MRC_FREQ_133                }, //  43   |
  { MRC_DDR_5800_TCK_MIN, f5800,                   MRC_FREQ_100 }, //       |  58
  { MRC_DDR_5867_TCK_MIN, f5867,    MRC_FREQ_133                }, //  44   |
  { MRC_DDR_5900_TCK_MIN, f5900,                   MRC_FREQ_100 }, //       |  59
  { MRC_DDR_6000_TCK_MIN, f6000,    MRC_FREQ_133 | MRC_FREQ_100 }, //  45   |  60
  { MRC_DDR_6100_TCK_MIN, f6100,                   MRC_FREQ_100 }, //       |  61
  { MRC_DDR_6133_TCK_MIN, f6133,    MRC_FREQ_133                }, //  46   |
  { MRC_DDR_6200_TCK_MIN, f6200,                   MRC_FREQ_100 }, //       |  62
  { MRC_DDR_6267_TCK_MIN, f6267,    MRC_FREQ_133                }, //  47   |
  { MRC_DDR_6300_TCK_MIN, f6300,                   MRC_FREQ_100 }, //       |  63
  { MRC_DDR_6400_TCK_MIN, f6400,    MRC_FREQ_133 | MRC_FREQ_100 }, //  48   |  64
  { MRC_DDR_6500_TCK_MIN, f6500,                   MRC_FREQ_100 }, //       |  65
  { MRC_DDR_6533_TCK_MIN, f6533,    MRC_FREQ_133                }, //  49   |
  { MRC_DDR_6600_TCK_MIN, f6600,                   MRC_FREQ_100 }, //       |  66
  { MRC_DDR_6667_TCK_MIN, f6667,    MRC_FREQ_133                }, //  50   |
  { MRC_DDR_6700_TCK_MIN, f6700,                   MRC_FREQ_100 }, //       |  67
  { MRC_DDR_6800_TCK_MIN, f6800,    MRC_FREQ_133 | MRC_FREQ_100 }, //  51   |  68
  { MRC_DDR_6900_TCK_MIN, f6900,                   MRC_FREQ_100 }, //       |  69
  { MRC_DDR_6933_TCK_MIN, f6933,    MRC_FREQ_133                }, //  52   |
  { MRC_DDR_7000_TCK_MIN, f7000,                   MRC_FREQ_100 }, //       |  70
  { MRC_DDR_7067_TCK_MIN, f7067,    MRC_FREQ_133                }, //  53   |
  { MRC_DDR_7100_TCK_MIN, f7100,                   MRC_FREQ_100 }, //       |  71
  { MRC_DDR_7200_TCK_MIN, f7200,    MRC_FREQ_133 | MRC_FREQ_100 }, //  54   |  72
  { MRC_DDR_7300_TCK_MIN, f7300,                   MRC_FREQ_100 }, //       |  73
  { MRC_DDR_7333_TCK_MIN, f7333,    MRC_FREQ_133                }, //  55   |
  { MRC_DDR_7400_TCK_MIN, f7400,                   MRC_FREQ_100 }, //       |  74
  { MRC_DDR_7467_TCK_MIN, f7467,    MRC_FREQ_133                }, //  56   |
  { MRC_DDR_7500_TCK_MIN, f7500,                   MRC_FREQ_100 }, //       |  75
  { MRC_DDR_7600_TCK_MIN, f7600,    MRC_FREQ_133 | MRC_FREQ_100 }, //  57   |  76
  { MRC_DDR_7700_TCK_MIN, f7700,                   MRC_FREQ_100 }, //       |  77
  { MRC_DDR_7733_TCK_MIN, f7733,    MRC_FREQ_133                }, //  58   |
  { MRC_DDR_7800_TCK_MIN, f7800,                   MRC_FREQ_100 }, //       |  78
  { MRC_DDR_7867_TCK_MIN, f7867,    MRC_FREQ_133                }, //  59   |
  { MRC_DDR_7900_TCK_MIN, f7900,                   MRC_FREQ_100 }, //       |  79
  { MRC_DDR_8000_TCK_MIN, f8000,    MRC_FREQ_133 | MRC_FREQ_100 }, //  60   |  80
  { MRC_DDR_8100_TCK_MIN, f8100,                   MRC_FREQ_100 }, //       |  81
  { MRC_DDR_8133_TCK_MIN, f8133,    MRC_FREQ_133                }, //  61   |
  { MRC_DDR_8200_TCK_MIN, f8200,                   MRC_FREQ_100 }, //       |  82
  { MRC_DDR_8267_TCK_MIN, f8267,    MRC_FREQ_133                }, //  62   |
  { 0,                    fNoInit,  MRC_FREQ_INVALID            }
};
**/

#if FixedPcdGetBool(PcdOverclockEnable) == 1
STATIC CONST EFI_STRING_ID mVfPointOcRatioValue[] = {
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE0),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE1),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE2),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE3),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE4),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE5),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE6),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE7),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE8),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE9),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE10),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE11),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE12),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE13),
  STRING_TOKEN (STR_VF_POINT_OC_RATIO_VALUE14)
};

STATIC CONST EFI_STRING_ID mGtVfPointOcRatioValue[] = {
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE0),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE1),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE2),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE3),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE4),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE5),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE6),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE7),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE8),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE9),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE10),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE11),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE12),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE13),
  STRING_TOKEN (STR_GT_VF_POINT_OC_RATIO_VALUE14)
};

STATIC CONST EFI_STRING_ID mRingVfPointOcRatioValue[] = {
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE0),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE1),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE2),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE3),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE4),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE5),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE6),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE7),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE8),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE9),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE10),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE11),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE12),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE13),
  STRING_TOKEN (STR_RING_VF_POINT_OC_RATIO_VALUE14)
};

STATIC CONST EFI_STRING_ID mMemSSVfPointOcRatioValue[] = {
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE0),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE1),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE2),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE3),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE4),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE5),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE6),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE7),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE8),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE9),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE10),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE11),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE12),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE13),
  STRING_TOKEN (STR_MEMSS_VF_POINT_OC_RATIO_VALUE14)
};

STATIC CONST EFI_STRING_ID mNguVfPointOcRatioValue[] = {
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE0),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE1),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE2),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE3),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE4),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE5),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE6),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE7),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE8),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE9),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE10),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE11),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE12),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE13),
  STRING_TOKEN (STR_NGU_VF_POINT_OC_RATIO_VALUE14)
};

/**
  This funciton converts the tCK value in femtoseconds to MHz.

  @param[in]      tCKvalue       - The tCK value to be converted to MHz.

  @retval tCkvalue in MHz
**/
UINT32
ConvertFs2Mhz (
  UINT32        tCKvalue
  )
{
  UINT32    tCKMhz;

  tCKMhz = (tCKvalue == 0) ? 0 : (2000000000 + (tCKvalue >> 1)) / tCKvalue;

  return tCKMhz;
}
#endif

/**
  Initializes Per Core Disable strings based on the bit mask of active cores.

  @param[in]  HiiHandle      Handle to HII database
**/
VOID
InitPerCoreDisableStrings (
  EFI_HII_HANDLE        HiiHandle
  )
{
  EFI_STATUS              Status;
  OC_SETUP                OcSetup;
  UINTN                   VariableSize;
  UINT32                  VariableAttr;
  UINT8                   Index;
  PCODE_MAILBOX_FULL      MailboxCommand;
  UINT32                  MailboxStatus;
  UINT64                  PerCoreDisableBitMask;
  UINT32                  ActiveCoreIndex;
  EFI_STRING_ID           StrRef1;
  EFI_STRING_ID           StrRef2;
  UINT32                  DisableCoreMaskLowOri;
  UINT32                  DisableCoreMaskHighOri;

  VariableSize = sizeof (OC_SETUP);
  Status = gRT->GetVariable (
                  L"OcSetup",
                  &gOcSetupVariableGuid,
                  &VariableAttr,
                  &VariableSize,
                  &OcSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return;
  }

  if (OcSetup.PerCoreDisableOriMask == 0) {
    DEBUG ((DEBUG_INFO, "(OC) Reading PerCoreDisableOriMask for the first time \n"));

    //
    // Get the current values of CPU soft straps DW3 Low and High
    //
    ZeroMem (&MailboxCommand, sizeof (MailboxCommand));
    MailboxCommand.Interface.Fields.Command = MAILBOX_PCODE_CMD_READ_SOFT_STRAPS;
    MailboxCommand.Interface.Fields.Param1  = CPU_SOFTSTRAP_SET3_LOW;
    MailboxCommand.Interface.Fields.Param2  = V_STRAP_RESOLVED_VALUE;
    Status = MailboxRead (MailboxCommand.Interface.InterfaceData, &MailboxCommand.Data, &MailboxStatus);
    if (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "(OC) Error Reading CPU Softstraps. EFI_STATUS = %r, \n", Status));
      ASSERT_EFI_ERROR (Status);
      return ;
    }
    DisableCoreMaskLowOri = MailboxCommand.Data;
    DEBUG ((DEBUG_INFO, "(OC) Deafult value of CPU soft strap low DW is = 0x%x\n", DisableCoreMaskLowOri));

    MailboxCommand.Interface.Fields.Param1  = CPU_SOFTSTRAP_SET3_HIGH;
    MailboxCommand.Data       = 0;
    Status = MailboxRead (MailboxCommand.Interface.InterfaceData, &MailboxCommand.Data, &MailboxStatus);
    if (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "(OC) Error Reading CPU Softstraps. EFI_STATUS = %r, \n", Status));
      ASSERT_EFI_ERROR (Status);
      return ;
    }
    DisableCoreMaskHighOri = MailboxCommand.Data;
    DEBUG ((DEBUG_INFO, "(OC) Deafult value of CPU soft strap high DW is = 0x%x\n", DisableCoreMaskHighOri));
    OcSetup.PerCoreDisableOriMask = DisableCoreMaskHighOri;
    OcSetup.PerCoreDisableOriMask <<= 32;
    OcSetup.PerCoreDisableOriMask |= (UINT64 )DisableCoreMaskLowOri;
    DEBUG ((DEBUG_INFO, "(OC) Deafult value of Core Disable mask is 0x%lx\n", OcSetup.PerCoreDisableOriMask));
  }
  Status = gRT->SetVariable (
                  L"OcSetup",
                  &gOcSetupVariableGuid,
                  VariableAttr,
                  VariableSize,
                  &OcSetup
                  );
  ASSERT_EFI_ERROR (Status);
  PerCoreDisableBitMask = OcSetup.PerCoreDisableOriMask;
  ActiveCoreIndex = 0;
  // Update the core strings as per the mask bit info
  for ( Index = 0; Index < OC_CPU_CORE_DISABLE_MASK_BITS; ++Index ) {
    if (((PerCoreDisableBitMask >> Index) & 0x1) != 1) {
      switch(Index) {
        case 0:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT0);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT0_HELP);
          break;
        case 1:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT1);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT1_HELP);
          break;
        case 2:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT2);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT2_HELP);
          break;
        case 3:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT3);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT3_HELP);
          break;
        case 4:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT4);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT4_HELP);
          break;
        case 5:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT5);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT5_HELP);
          break;
        case 6:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT6);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT6_HELP);
          break;
        case 7:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT7);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT7_HELP);
          break;
        case 8:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT8);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT8_HELP);
          break;
        case 9:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT9);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT9_HELP);
          break;
        case 10:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT10);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT10_HELP);
          break;
        case 11:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT11);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT11_HELP);
          break;
        case 12:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT12);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT12_HELP);
          break;
        case 13:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT13);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT13_HELP);
          break;
        case 14:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT14);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT14_HELP);
          break;
        case 15:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT15);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT15_HELP);
          break;
        case 16:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT16);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT16_HELP);
          break;
        case 17:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT17);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT17_HELP);
          break;
        case 18:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT18);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT18_HELP);
          break;
        case 19:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT19);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT19_HELP);
          break;
        case 20:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT20);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT20_HELP);
          break;
        case 21:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT21);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT21_HELP);
          break;
        case 22:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT22);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT22_HELP);
          break;
        case 23:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT23);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT23_HELP);
          break;
        case 24:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT24);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT24_HELP);
          break;
        case 25:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT25);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT25_HELP);
          break;
        case 26:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT26);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT26_HELP);
          break;
        case 27:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT27);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT27_HELP);
          break;
        case 28:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT28);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT28_HELP);
          break;
        case 29:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT29);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT29_HELP);
          break;
        case 30:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT30);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT30_HELP);
          break;
        case 31:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT31);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT31_HELP);
          break;
        case 32:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT32);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT32_HELP);
          break;
        case 33:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT33);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT33_HELP);
          break;
        case 34:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT34);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT34_HELP);
          break;
        case 35:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT35);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT35_HELP);
          break;
        case 36:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT36);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT36_HELP);
          break;
        case 37:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT37);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT37_HELP);
          break;
        case 38:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT38);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT38_HELP);
          break;
        default:
          StrRef1 = STRING_TOKEN (STR_CORE_BIT39);
          StrRef2 = STRING_TOKEN (STR_CORE_BIT39_HELP);
          break;
      }
      InitString (HiiHandle, StrRef1,L"Core %d", ActiveCoreIndex);
      InitString (HiiHandle, StrRef2,L"Check to Disable Core %d", ActiveCoreIndex);
      ++ActiveCoreIndex;
    }
  }
}

/**
  Initialize Max OC Ratio Limt Strings.

  @param[in]  HiiHandle      Handle to HII database
  @param[in]  Class          Indicates the setup class
**/
VOID
InitMaxOcRatioLimitStrings (
  EFI_HII_HANDLE        HiiHandle,
  UINT16                Class
  )
{
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  UINT8                 DomainId;
  OC_DATA_HOB           *OcDataHob;
  VOID                  *Hob;

  DEBUG ((DEBUG_INFO, "<InitMaxOcRatioLimitStrings>\n"));
  Hob = GetFirstGuidHob (&gOcDataHobGuid);
  if (Hob == NULL) {
    DEBUG((DEBUG_INFO, "(OC) OcDataHob Not Found\n"));
    ASSERT (Hob != NULL);
  }
  OcDataHob = GET_GUID_HOB_DATA (Hob);

  for (DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE; DomainId <= MAILBOX_OC_DOMAIN_ID_CPU_SA; DomainId++) {
    switch (DomainId) {
    case MAILBOX_OC_DOMAIN_ID_IA_CORE:
      DEBUG ((DEBUG_INFO, "(OC) Max OC Ratio Limt: DomainId=0x%02X, MaxOcRatioLimit=0x%04X\n", DomainId, OcDataHob->MaxOcRatioLimit[DomainId]));
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_CORE_MAX_OC_RATIO_LIMIT_VALUE),
        L"%d",
        OcDataHob->MaxOcRatioLimit[DomainId]
        );
    break;

    case MAILBOX_OC_DOMAIN_ID_GT:
      DEBUG ((DEBUG_INFO, "(OC) Max OC Ratio Limt: DomainId=0x%02X, MaxOcRatioLimit=0x%04X\n", DomainId, OcDataHob->MaxOcRatioLimit[DomainId]));
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_GT_MAX_OC_RATIO_LIMIT_VALUE),
        L"%d",
        OcDataHob->MaxOcRatioLimit[DomainId]
        );
    break;

    case MAILBOX_OC_DOMAIN_ID_RING:
      DEBUG ((DEBUG_INFO, "(OC) Max OC Ratio Limt: DomainId=0x%02X, MaxOcRatioLimit=0x%04X\n", DomainId, OcDataHob->MaxOcRatioLimit[DomainId]));
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_RING_MAX_OC_RATIO_LIMIT_VALUE),
        L"%d",
        OcDataHob->MaxOcRatioLimit[DomainId]
        );
    break;

    case MAILBOX_OC_DOMAIN_ID_MEMORY_SUBSYSTEM:
      DEBUG ((DEBUG_INFO, "(OC) Max OC Ratio Limt: DomainId=0x%02X, MaxOcRatioLimit=0x%04X\n", DomainId, OcDataHob->MaxOcRatioLimit[DomainId]));
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_MEMSS_MAX_OC_RATIO_LIMIT_VALUE),
        L"%d",
        OcDataHob->MaxOcRatioLimit[DomainId]
        );
    break;

    case MAILBOX_OC_DOMAIN_ID_NGU:
      DEBUG ((DEBUG_INFO, "(OC) Max OC Ratio Limt: DomainId=0x%02X, MaxOcRatioLimit=0x%04X\n", DomainId, OcDataHob->MaxOcRatioLimit[DomainId]));
      InitString (
        HiiHandle,
        STRING_TOKEN (STR_NGU_MAX_OC_RATIO_LIMIT_VALUE),
        L"%d",
        OcDataHob->MaxOcRatioLimit[DomainId]
        );
    break;

    default:
    break;
    }
  }
#endif
}

/**
  Get DCC current frequencies.

  @param[in]  Integer           The integer parts of divider register.
  @param[in]  Fractional        The fractional parts of divider register.

  @retval     Frequency         Discrete Clock frequency.
**/
UINT16
GetDccCurrentFreq (
  IN UINT16   Integer,
  IN UINT32   Fractional
  )
{
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  UINT64      DividerValue;
  UINT64      DefualtFreq;

  DividerValue = (Integer % BIT9);// The RC260x chip only provide [8:0] for integer register
  DividerValue = LShiftU64 (DividerValue, 28);
  DividerValue += (Fractional % BIT28); // The RC260x chip only provide [27:0] for fraction register

  DefualtFreq = RENESAS_260X_CHIP_FREQUENCY;
  DefualtFreq = LShiftU64 (DefualtFreq, 28);

  return (DividerValue == 0)? 0 : (UINT16) (DefualtFreq / DividerValue);
#else
  return 0;
#endif
}

/**
  Initializes CPU Overclocking related variables.

**/
VOID
InitCpuOcSetupVariables (
  VOID
  )
{
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  EFI_STATUS                      Status;
  OC_SETUP                        OcSetup;
  UINTN                           VariableSize;
  UINT32                          VariableAttr;
  MSR_ARCH_CAPABILITIES_REGISTER  Ia32ArchCapabilities;
  OC_DATA_HOB                     *OcDataHob;
  VOID                            *Hob;
  UINTN                           DomainId;
  UINT8                           MaxDomainId;

  MaxDomainId = MAILBOX_OC_DOMAIN_ID_MAX;

  VariableSize = sizeof (OC_SETUP);
  Status = gRT->GetVariable (
                  L"OcSetup",
                  &gOcSetupVariableGuid,
                  &VariableAttr,
                  &VariableSize,
                  &OcSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ERROR : Unable to get Variables!\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }


  Ia32ArchCapabilities.Uint64 = AsmReadMsr64 (MSR_ARCH_CAPABILITIES);
  DEBUG ((DEBUG_INFO, "(OC) IA32_ARCH_CAPABILITIES is 0x%lx\n", Ia32ArchCapabilities.Uint64));
  OcSetup.OverclockingMsrStatus = (UINT8) ((Ia32ArchCapabilities.Uint64 & BIT23) >> 23);
  DEBUG ((DEBUG_INFO, "(OC) OverclockingMsrStatus is %d\n", OcSetup.OverclockingMsrStatus));

  //
  // Update OcSetup info according to OC Data Hob.
  //
  Hob  = GetFirstGuidHob (&gOcDataHobGuid);
  if (Hob == NULL) {
    DEBUG((DEBUG_INFO, "(OC) OcDataHob Not Found\n"));
    ASSERT (Hob != NULL);
  }
  OcDataHob = GET_GUID_HOB_DATA (Hob);
  //
  // Update Setup info according to OC Data Hob.
  //
  for (DomainId = 0; DomainId < MaxDomainId; DomainId++) {
    if (DomainId == MAILBOX_OC_DOMAIN_ID_IA_CORE || DomainId == MAILBOX_OC_DOMAIN_ID_GT || DomainId == MAILBOX_OC_DOMAIN_ID_RING || \
      DomainId == MAILBOX_OC_DOMAIN_ID_MEMORY_SUBSYSTEM || DomainId == MAILBOX_OC_DOMAIN_ID_NGU) {
      //
      //  Update VfPointCount for Domain IA_CORE, GT, RING, MEMSS, NGU
      //
      OcSetup.VfPointCount[DomainId]   = OcDataHob->VfPointCount[DomainId];
    }
  }

  OcSetup.GranularRatioSupported = IsGranularRatioSupport ();
  OcSetup.IsPerCoreVotage = IsPerCoreVotage ();

  Status = gRT->SetVariable (
                  L"OcSetup",
                  &gOcSetupVariableGuid,
                  VariableAttr,
                  VariableSize,
                  &OcSetup
                  );
  ASSERT_EFI_ERROR (Status);
#endif
}

/**
  Installs OverClock Callback function.

  @param[in]  HiiHandle      Handle to HII database
  @param[in]  Class          Indicates the setup class
**/
VOID
InitOverClockStrings (
  EFI_HII_HANDLE            HiiHandle,
  UINT16                    Class
  )
{
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  EFI_STATUS                Status;
  SA_SETUP                  SaSetup;
  SETUP_DATA                SetupData;
  CPU_SETUP                 CpuSetup;
  MEM_INFO_PROTOCOL         *MemInfoHobProtocol;
  UINTN                     VariableSize;
  MEMORY_INFO_DATA          *MemInfoData;
  MEMORY_TIMING             *MemoryTiming;
  UINT32                    VariableAttr;
  UINT64                    BclkFrequency;
  CPU_SKU                   CpuSku;
  UINT32                    tCKvalues[MEMORY_PROFILE_MAX];
  UINT8                     Index;
  UINT32                    MchBar;
  UINT32                    CpuBclkFreq;
  UINT32                    SocBclkFreq;
  OC_DATA_HOB               *OcDataHob;
  VOID                      *Hob;
  EFI_HOB_GUID_TYPE         *DccDataBufferHob;
  DISCRETE_CLOCL_CHIP_HOB   *DccChipHob;

  if (Class != ADVANCED_FORM_SET_CLASS) {
    return;
  }

  DEBUG ((DEBUG_INFO, "<InitOverClockStrings>\n"));

  // Call InitCpuOcSetupVariables to update OC related CPU setup variables
  InitCpuOcSetupVariables();

  //
  // Get the pcode derived BCLK frequency in kHz. Message is only valid on DT/Halo skus.
  //
  CpuSku = GetCpuSku ();
  if ((CpuSku == EnumCpuHalo) || (CpuSku == EnumCpuTrad)) {

    MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
    BclkFrequency = MmioRead64 (MchBar + PCU_CR_BCLK_FREQ_MCHBAR);
    // SOC Bclk Frequency in Khz resolution
    SocBclkFreq = (UINT32)(BclkFrequency & 0xFFFFFFFF);
    CpuBclkFreq = (UINT32)(BclkFrequency >> 32);
    DEBUG ((DEBUG_INFO, "(OC) CPU BCLK frequency = %X\n", CpuBclkFreq));
    DEBUG ((DEBUG_INFO, "(OC) SOC BCLK frequency = %X\n", SocBclkFreq));

    //
    // Initialize BCLK Frequency String in MHz
    //
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_CPU_BCLK_FREQUENCY_VALUE),
      L"%d.%02d MHz",
      CpuBclkFreq / 1000,
      CpuBclkFreq % 1000
      );

    //
    // Initialize BCLK Frequency String in MHz
    //
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_SOC_BCLK_FREQUENCY_VALUE),
      L"%d.%02d MHz",
      SocBclkFreq / 1000,
      SocBclkFreq % 1000
      );
  }

  //
  // Update DCC Strings
  //
  VariableSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  &VariableAttr,
                  &VariableSize,
                  &CpuSetup
                  );
  ASSERT_EFI_ERROR (Status);

  if (CpuSetup.DccClkCtrl == 1) {
    //
    // Update Cpu BCLK & PEG/DMI Freq.
    //
    DccDataBufferHob = GetFirstGuidHob (&gDccDividerDataBufferHobGuid);
    DccChipHob = (DISCRETE_CLOCL_CHIP_HOB *) GET_GUID_HOB_DATA (DccDataBufferHob);
    CpuSetup.CurrentCpuBclkFreq = GetDccCurrentFreq (DccChipHob->Fod0IntegerDividerValue.Uint16, DccChipHob->Fod0FractionalDividerValue.Uint32);
    CpuSetup.CurrentPegDmiClkFreq = GetDccCurrentFreq (DccChipHob->Fod1IntegerDividerValue.Uint16, DccChipHob->Fod1FractionalDividerValue.Uint32);

    InitString (
      HiiHandle,
      STRING_TOKEN (STR_DCC_CPU_BCLK_FREQ_VALUE),
      L"%d MHz",
      CpuSetup.CurrentCpuBclkFreq
      );

    InitString (
      HiiHandle,
      STRING_TOKEN (STR_DCC_PEGDMI_CLK_FREQ_VALUE),
      L"%d MHz",
      CpuSetup.CurrentPegDmiClkFreq
      );
  }

  //
  // Read the initial bit mask from PCODE.
  // This input is required only once and it will be used to display all the active cores supported on that part in BIOS menu.
  //
  InitPerCoreDisableStrings(HiiHandle);

  /// Get OC Data Hob
  ///
  Hob = GetFirstGuidHob (&gOcDataHobGuid);
  if (Hob == NULL) {
    DEBUG((DEBUG_INFO, "(OC) OcDataHob Not Found\n"));
    ASSERT (Hob != NULL);
  }
  OcDataHob = GET_GUID_HOB_DATA (Hob);

  for (Index = 0; Index < MAX_OC_VF_POINTS; Index++) {
    InitString (
      HiiHandle,
      mVfPointOcRatioValue[Index],
      L"%d",
      OcDataHob->VfPointRatio[MAILBOX_OC_DOMAIN_ID_IA_CORE][Index]
      );

    InitString (
      HiiHandle,
      mGtVfPointOcRatioValue[Index],
      L"%d",
      OcDataHob->VfPointRatio[MAILBOX_OC_DOMAIN_ID_GT][Index]
      );

    InitString (
      HiiHandle,
      mRingVfPointOcRatioValue[Index],
      L"%d",
      OcDataHob->VfPointRatio[MAILBOX_OC_DOMAIN_ID_RING][Index]
      );

    InitString (
      HiiHandle,
      mMemSSVfPointOcRatioValue[Index],
      L"%d",
      OcDataHob->VfPointRatio[MAILBOX_OC_DOMAIN_ID_MEMORY_SUBSYSTEM][Index]
      );

    InitString (
      HiiHandle,
      mNguVfPointOcRatioValue[Index],
      L"%d",
      OcDataHob->VfPointRatio[MAILBOX_OC_DOMAIN_ID_NGU][Index]
      );
  }

  ///
  /// Setting ICC VR string values
  ///
  mIaIccMax = OcDataHob->IaIccMax;
  mGtIccMax = OcDataHob->GtIccMax;

  DEBUG ((DEBUG_INFO, "(OC) IaIccMax and GtIccMax is %d  %d\n", OcDataHob->IaIccMax, OcDataHob->GtIccMax));
  InitString (
    HiiHandle,
    STRING_TOKEN(STR_IA_VR_ICCMAX_INFO_VALUE),
    L"%d",
    OcDataHob->IaIccMax
    );

  InitString (
    HiiHandle,
    STRING_TOKEN(STR_GT_VR_ICCMAX_INFO_VALUE),
    L"%d",
    OcDataHob->GtIccMax
    );

  if (IsGranularRatioSupport ()) {
    ///
    /// Initialize GranularRatio Units info strings
    ///
    InitString (
      HiiHandle,
      STRING_TOKEN (STR_GRANULAR_RATIO_UNITS_VALUE),
      L"%d",
      OcDataHob->GranularityUnits
      );
  }

  //
  // Get the Memory Info HOB Protocol if it exists.
  //
  Status = gBS->LocateProtocol (&gMemInfoProtocolGuid, NULL, (VOID **) &MemInfoHobProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }

  MemInfoData  = &MemInfoHobProtocol->MemInfoData;
  MemoryTiming = MemInfoData->Timing;

  for (Index = 0; Index < MEMORY_PROFILE_MAX; Index++) {
    tCKvalues[Index] = ConvertFs2Mhz (MemoryTiming[Index].tCK);
  }

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TCK_VALUE), Timing7cString,
    tCKvalues[DEFAULT_SPD_PROFILE],
    tCKvalues[CUSTOM_PROFILE],
    tCKvalues[XMP_PROFILE_1],
    tCKvalues[XMP_PROFILE_2],
    tCKvalues[XMP_PROFILE_3],
    tCKvalues[USER_PROFILE_4],
    tCKvalues[USER_PROFILE_5]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TCL_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tCL,
    MemoryTiming[CUSTOM_PROFILE].tCL,
    MemoryTiming[XMP_PROFILE_1].tCL,
    MemoryTiming[XMP_PROFILE_2].tCL,
    MemoryTiming[XMP_PROFILE_3].tCL,
    MemoryTiming[USER_PROFILE_4].tCL,
    MemoryTiming[USER_PROFILE_5].tCL
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRCD_TRP_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRCDtRP,
    MemoryTiming[CUSTOM_PROFILE].tRCDtRP,
    MemoryTiming[XMP_PROFILE_1].tRCDtRP,
    MemoryTiming[XMP_PROFILE_2].tRCDtRP,
    MemoryTiming[XMP_PROFILE_3].tRCDtRP,
    MemoryTiming[USER_PROFILE_4].tRCDtRP,
    MemoryTiming[USER_PROFILE_5].tRCDtRP
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRAS_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRAS,
    MemoryTiming[CUSTOM_PROFILE].tRAS,
    MemoryTiming[XMP_PROFILE_1].tRAS,
    MemoryTiming[XMP_PROFILE_2].tRAS,
    MemoryTiming[XMP_PROFILE_3].tRAS,
    MemoryTiming[USER_PROFILE_4].tRAS,
    MemoryTiming[USER_PROFILE_5].tRAS
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TCWL_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tCWL,
    MemoryTiming[CUSTOM_PROFILE].tCWL,
    MemoryTiming[XMP_PROFILE_1].tCWL,
    MemoryTiming[XMP_PROFILE_2].tCWL,
    MemoryTiming[XMP_PROFILE_3].tCWL,
    MemoryTiming[USER_PROFILE_4].tCWL,
    MemoryTiming[USER_PROFILE_5].tCWL
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TFAW_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tFAW,
    MemoryTiming[CUSTOM_PROFILE].tFAW,
    MemoryTiming[XMP_PROFILE_1].tFAW,
    MemoryTiming[XMP_PROFILE_2].tFAW,
    MemoryTiming[XMP_PROFILE_3].tFAW,
    MemoryTiming[USER_PROFILE_4].tFAW,
    MemoryTiming[USER_PROFILE_5].tFAW
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TREFI_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tREFI,
    MemoryTiming[CUSTOM_PROFILE].tREFI,
    MemoryTiming[XMP_PROFILE_1].tREFI,
    MemoryTiming[XMP_PROFILE_2].tREFI,
    MemoryTiming[XMP_PROFILE_3].tREFI,
    MemoryTiming[USER_PROFILE_4].tREFI,
    MemoryTiming[USER_PROFILE_5].tREFI
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRFC_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRFC,
    MemoryTiming[CUSTOM_PROFILE].tRFC,
    MemoryTiming[XMP_PROFILE_1].tRFC,
    MemoryTiming[XMP_PROFILE_2].tRFC,
    MemoryTiming[XMP_PROFILE_3].tRFC,
    MemoryTiming[USER_PROFILE_4].tRFC,
    MemoryTiming[USER_PROFILE_5].tRFC
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRRD_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRRD,
    MemoryTiming[CUSTOM_PROFILE].tRRD,
    MemoryTiming[XMP_PROFILE_1].tRRD,
    MemoryTiming[XMP_PROFILE_2].tRRD ,
    MemoryTiming[XMP_PROFILE_3].tRRD,
    MemoryTiming[USER_PROFILE_4].tRRD,
    MemoryTiming[USER_PROFILE_5].tRRD
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRTP_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRTP,
    MemoryTiming[CUSTOM_PROFILE].tRTP,
    MemoryTiming[XMP_PROFILE_1].tRTP,
    MemoryTiming[XMP_PROFILE_2].tRTP,
    MemoryTiming[XMP_PROFILE_3].tRTP,
    MemoryTiming[USER_PROFILE_4].tRTP,
    MemoryTiming[USER_PROFILE_5].tRTP
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TWR_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tWR,
    MemoryTiming[CUSTOM_PROFILE].tWR,
    MemoryTiming[XMP_PROFILE_1].tWR,
    MemoryTiming[XMP_PROFILE_2].tWR,
    MemoryTiming[XMP_PROFILE_3].tWR,
    MemoryTiming[USER_PROFILE_4].tWR,
    MemoryTiming[USER_PROFILE_5].tWR
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TWTR_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tWTR,
    MemoryTiming[CUSTOM_PROFILE].tWTR,
    MemoryTiming[XMP_PROFILE_1].tWTR,
    MemoryTiming[XMP_PROFILE_2].tWTR,
    MemoryTiming[XMP_PROFILE_3].tWTR,
    MemoryTiming[USER_PROFILE_4].tWTR,
    MemoryTiming[USER_PROFILE_5].tWTR
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_NMODE_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].NMode,
    MemoryTiming[CUSTOM_PROFILE].NMode,
    MemoryTiming[XMP_PROFILE_1].NMode,
    MemoryTiming[XMP_PROFILE_2].NMode,
    MemoryTiming[XMP_PROFILE_3].NMode,
    MemoryTiming[USER_PROFILE_4].NMode,
    MemoryTiming[USER_PROFILE_5].NMode
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_VOLTAGE_VDD_VALUE),
    Timing5cString,
    MemInfoData->VddVoltage[DEFAULT_SPD_PROFILE],
    MemInfoData->VddVoltage[CUSTOM_PROFILE],
    MemInfoData->VddVoltage[XMP_PROFILE_1],
    MemInfoData->VddVoltage[XMP_PROFILE_2],
    MemInfoData->VddVoltage[XMP_PROFILE_3],
    MemInfoData->VddVoltage[USER_PROFILE_4],
    MemInfoData->VddVoltage[USER_PROFILE_5]
    );

/*  Mark for compiling successfully when PcdOverclockEnable set True.
    Because memory OC related strings were reomved from SaSetup. Keep if next platform uses.

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRFCPB_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRFCpb,
    MemoryTiming[CUSTOM_PROFILE].tRFCpb,
    MemoryTiming[XMP_PROFILE_1].tRFCpb,
    MemoryTiming[XMP_PROFILE_2].tRFCpb,
    MemoryTiming[XMP_PROFILE_3].tRFCpb,
    MemoryTiming[USER_PROFILE_4].tRFCpb,
    MemoryTiming[USER_PROFILE_5].tRFCpb
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRFC2_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRFC2,
    MemoryTiming[CUSTOM_PROFILE].tRFC2,
    MemoryTiming[XMP_PROFILE_1].tRFC2,
    MemoryTiming[XMP_PROFILE_2].tRFC2,
    MemoryTiming[XMP_PROFILE_3].tRFC2,
    MemoryTiming[USER_PROFILE_4].tRFC2,
    MemoryTiming[USER_PROFILE_5].tRFC2
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRFC4_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRFC4,
    MemoryTiming[CUSTOM_PROFILE].tRFC4,
    MemoryTiming[XMP_PROFILE_1].tRFC4,
    MemoryTiming[XMP_PROFILE_2].tRFC4,
    MemoryTiming[XMP_PROFILE_3].tRFC4,
    MemoryTiming[USER_PROFILE_4].tRFC4,
    MemoryTiming[USER_PROFILE_5].tRFC4
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRRD_L_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRRD_L,
    MemoryTiming[CUSTOM_PROFILE].tRRD_L,
    MemoryTiming[XMP_PROFILE_1].tRRD_L,
    MemoryTiming[XMP_PROFILE_2].tRRD_L,
    MemoryTiming[XMP_PROFILE_3].tRRD_L,
    MemoryTiming[USER_PROFILE_4].tRRD_L,
    MemoryTiming[USER_PROFILE_5].tRRD_L
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TRRD_S_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tRRD_S,
    MemoryTiming[CUSTOM_PROFILE].tRRD_S,
    MemoryTiming[XMP_PROFILE_1].tRRD_S,
    MemoryTiming[XMP_PROFILE_2].tRRD_S,
    MemoryTiming[XMP_PROFILE_3].tRRD_S,
    MemoryTiming[USER_PROFILE_4].tRRD_S,
    MemoryTiming[USER_PROFILE_5].tRRD_S
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TWTR_L_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tWTR_L,
    MemoryTiming[CUSTOM_PROFILE].tWTR_L,
    MemoryTiming[XMP_PROFILE_1].tWTR_L,
    MemoryTiming[XMP_PROFILE_2].tWTR_L,
    MemoryTiming[XMP_PROFILE_3].tWTR_L,
    MemoryTiming[USER_PROFILE_4].tWTR_L,
    MemoryTiming[USER_PROFILE_5].tWTR_L
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TWTR_S_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tWTR_S,
    MemoryTiming[CUSTOM_PROFILE].tWTR_S,
    MemoryTiming[XMP_PROFILE_1].tWTR_S,
    MemoryTiming[XMP_PROFILE_2].tWTR_S,
    MemoryTiming[XMP_PROFILE_3].tWTR_S,
    MemoryTiming[USER_PROFILE_4].tWTR_S,
    MemoryTiming[USER_PROFILE_5].tWTR_S
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TCCD_L_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tCCD_L,
    MemoryTiming[CUSTOM_PROFILE].tCCD_L,
    MemoryTiming[XMP_PROFILE_1].tCCD_L,
    MemoryTiming[XMP_PROFILE_2].tCCD_L,
    MemoryTiming[XMP_PROFILE_3].tCCD_L,
    MemoryTiming[USER_PROFILE_4].tCCD_L,
    MemoryTiming[USER_PROFILE_5].tCCD_L
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_TIMING_TCCD_L_WR_VALUE),
    Timing5cString,
    MemoryTiming[DEFAULT_SPD_PROFILE].tCCD_L_WR,
    MemoryTiming[CUSTOM_PROFILE].tCCD_L_WR,
    MemoryTiming[XMP_PROFILE_1].tCCD_L_WR,
    MemoryTiming[XMP_PROFILE_2].tCCD_L_WR,
    MemoryTiming[XMP_PROFILE_3].tCCD_L_WR,
    MemoryTiming[USER_PROFILE_4].tCCD_L_WR,
    MemoryTiming[USER_PROFILE_5].tCCD_L_WR
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_VOLTAGE_VDDQ_VALUE),
    Timing5cString,
    MemInfoData->VddqVoltage[DEFAULT_SPD_PROFILE],
    MemInfoData->VddqVoltage[CUSTOM_PROFILE],
    MemInfoData->VddqVoltage[XMP_PROFILE_1],
    MemInfoData->VddqVoltage[XMP_PROFILE_2],
    MemInfoData->VddqVoltage[XMP_PROFILE_3],
    MemInfoData->VddqVoltage[USER_PROFILE_4],
    MemInfoData->VddqVoltage[USER_PROFILE_5]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_VOLTAGE_VPP_VALUE),
    Timing5cString,
    MemInfoData->VppVoltage[DEFAULT_SPD_PROFILE],
    MemInfoData->VppVoltage[CUSTOM_PROFILE],
    MemInfoData->VppVoltage[XMP_PROFILE_1],
    MemInfoData->VppVoltage[XMP_PROFILE_2],
    MemInfoData->VppVoltage[XMP_PROFILE_3],
    MemInfoData->VppVoltage[USER_PROFILE_4],
    MemInfoData->VppVoltage[USER_PROFILE_5]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_RCOMP_RDODT_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->RcompTarget[DEFAULT_SPD_PROFILE][0],
    MemInfoData->RcompTarget[CUSTOM_PROFILE][0],
    MemInfoData->RcompTarget[XMP_PROFILE_1][0],
    MemInfoData->RcompTarget[XMP_PROFILE_2][0],
    MemInfoData->RcompTarget[XMP_PROFILE_3][0],
    MemInfoData->RcompTarget[USER_PROFILE_4][0],
    MemInfoData->RcompTarget[USER_PROFILE_5][0]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_RCOMP_WRDS_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->RcompTarget[DEFAULT_SPD_PROFILE][1],
    MemInfoData->RcompTarget[CUSTOM_PROFILE][1],
    MemInfoData->RcompTarget[XMP_PROFILE_1][1],
    MemInfoData->RcompTarget[XMP_PROFILE_2][1],
    MemInfoData->RcompTarget[XMP_PROFILE_3][1],
    MemInfoData->RcompTarget[USER_PROFILE_4][1],
    MemInfoData->RcompTarget[USER_PROFILE_5][1]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_RCOMP_WRDSCMD_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->RcompTarget[DEFAULT_SPD_PROFILE][2],
    MemInfoData->RcompTarget[CUSTOM_PROFILE][2],
    MemInfoData->RcompTarget[XMP_PROFILE_1][2],
    MemInfoData->RcompTarget[XMP_PROFILE_2][2],
    MemInfoData->RcompTarget[XMP_PROFILE_3][2],
    MemInfoData->RcompTarget[USER_PROFILE_4][2],
    MemInfoData->RcompTarget[USER_PROFILE_5][2]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_RCOMP_WRDSCTL_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->RcompTarget[DEFAULT_SPD_PROFILE][3],
    MemInfoData->RcompTarget[CUSTOM_PROFILE][3],
    MemInfoData->RcompTarget[XMP_PROFILE_1][3],
    MemInfoData->RcompTarget[XMP_PROFILE_2][3],
    MemInfoData->RcompTarget[XMP_PROFILE_3][3],
    MemInfoData->RcompTarget[USER_PROFILE_4][3],
    MemInfoData->RcompTarget[USER_PROFILE_5][3]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_RCOMP_WRDSCLK_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->RcompTarget[DEFAULT_SPD_PROFILE][4],
    MemInfoData->RcompTarget[CUSTOM_PROFILE][4],
    MemInfoData->RcompTarget[XMP_PROFILE_1][4],
    MemInfoData->RcompTarget[XMP_PROFILE_2][4],
    MemInfoData->RcompTarget[XMP_PROFILE_3][4],
    MemInfoData->RcompTarget[USER_PROFILE_4][4],
    MemInfoData->RcompTarget[USER_PROFILE_5][4]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTWR_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][0],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][0],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][0],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][0],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][0],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][0],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][0]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTNOMRD_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][1],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][1],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][1],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][1],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][1],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][1],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][1]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTNOMWR_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][2],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][2],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][2],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][2],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][2],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][2],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][2]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTPARK_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][3],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][3],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][3],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][3],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][3],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][3],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][3]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTPARKDQS_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][4],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][4],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][4],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][4],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][4],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][4],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][4]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTCA_GROUPA_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][5],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][5],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][5],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][5],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][5],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][5],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][5]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTCS_GROUPA_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][6],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][6],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][6],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][6],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][6],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][6],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][6]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTCK_GROUPA_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][7],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][7],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][7],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][7],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][7],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][7],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][7]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTCA_GROUPB_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][8],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][8],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][8],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][8],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][8],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][8],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][8]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTCS_GROUPB_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][9],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][9],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][9],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][9],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][9],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][9],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][9]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM0_RTTCK_GROUPB_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][0][10],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][0][10],
    MemInfoData->DimmOdt[XMP_PROFILE_1][0][10],
    MemInfoData->DimmOdt[XMP_PROFILE_2][0][10],
    MemInfoData->DimmOdt[XMP_PROFILE_3][0][10],
    MemInfoData->DimmOdt[USER_PROFILE_4][0][10],
    MemInfoData->DimmOdt[USER_PROFILE_5][0][10]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTWR_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][0],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][0],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][0],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][0],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][0],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][0],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][0]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTNOMRD_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][1],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][1],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][1],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][1],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][1],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][1],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][1]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTNOMWR_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][2],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][2],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][2],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][2],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][2],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][2],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][2]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTPARK_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][3],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][3],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][3],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][3],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][3],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][3],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][3]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTPARKDQS_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][4],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][4],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][4],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][4],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][4],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][4],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][4]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTCA_GROUPA_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][5],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][5],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][5],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][5],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][5],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][5],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][5]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTCS_GROUPA_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][6],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][6],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][6],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][6],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][6],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][6],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][6]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTCK_GROUPA_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][7],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][7],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][7],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][7],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][7],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][7],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][7]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTCA_GROUPB_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][8],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][8],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][8],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][8],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][8],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][8],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][8]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTCS_GROUPB_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][9],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][9],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][9],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][9],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][9],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][9],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][9]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_DIMM1_RTTCK_GROUPB_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmOdt[DEFAULT_SPD_PROFILE][1][10],
    MemInfoData->DimmOdt[CUSTOM_PROFILE][1][10],
    MemInfoData->DimmOdt[XMP_PROFILE_1][1][10],
    MemInfoData->DimmOdt[XMP_PROFILE_2][1][10],
    MemInfoData->DimmOdt[XMP_PROFILE_3][1][10],
    MemInfoData->DimmOdt[USER_PROFILE_4][1][10],
    MemInfoData->DimmOdt[USER_PROFILE_5][1][10]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_C0D0_DIMM_TAP1_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmDFE[DEFAULT_SPD_PROFILE][0][0][0],
    MemInfoData->DimmDFE[CUSTOM_PROFILE][0][0][0],
    MemInfoData->DimmDFE[XMP_PROFILE_1][0][0][0],
    MemInfoData->DimmDFE[XMP_PROFILE_2][0][0][0],
    MemInfoData->DimmDFE[XMP_PROFILE_3][0][0][0],
    MemInfoData->DimmDFE[USER_PROFILE_4][0][0][0],
    MemInfoData->DimmDFE[USER_PROFILE_5][0][0][0]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_C0D0_DIMM_TAP2_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmDFE[DEFAULT_SPD_PROFILE][0][0][1],
    MemInfoData->DimmDFE[CUSTOM_PROFILE][0][0][1],
    MemInfoData->DimmDFE[XMP_PROFILE_1][0][0][1],
    MemInfoData->DimmDFE[XMP_PROFILE_2][0][0][1],
    MemInfoData->DimmDFE[XMP_PROFILE_3][0][0][1],
    MemInfoData->DimmDFE[USER_PROFILE_4][0][0][1],
    MemInfoData->DimmDFE[USER_PROFILE_5][0][0][1]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_C0D1_DIMM_TAP1_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmDFE[DEFAULT_SPD_PROFILE][0][1][0],
    MemInfoData->DimmDFE[CUSTOM_PROFILE][0][1][0],
    MemInfoData->DimmDFE[XMP_PROFILE_1][0][1][0],
    MemInfoData->DimmDFE[XMP_PROFILE_2][0][1][0],
    MemInfoData->DimmDFE[XMP_PROFILE_3][0][1][0],
    MemInfoData->DimmDFE[USER_PROFILE_4][0][1][0],
    MemInfoData->DimmDFE[USER_PROFILE_5][0][1][0]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_C0D1_DIMM_TAP2_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmDFE[DEFAULT_SPD_PROFILE][0][1][1],
    MemInfoData->DimmDFE[CUSTOM_PROFILE][0][1][1],
    MemInfoData->DimmDFE[XMP_PROFILE_1][0][1][1],
    MemInfoData->DimmDFE[XMP_PROFILE_2][0][1][1],
    MemInfoData->DimmDFE[XMP_PROFILE_3][0][1][1],
    MemInfoData->DimmDFE[USER_PROFILE_4][0][1][1],
    MemInfoData->DimmDFE[USER_PROFILE_5][0][1][1]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_C1D0_DIMM_TAP1_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmDFE[DEFAULT_SPD_PROFILE][1][0][0],
    MemInfoData->DimmDFE[CUSTOM_PROFILE][1][0][0],
    MemInfoData->DimmDFE[XMP_PROFILE_1][1][0][0],
    MemInfoData->DimmDFE[XMP_PROFILE_2][1][0][0],
    MemInfoData->DimmDFE[XMP_PROFILE_3][1][0][0],
    MemInfoData->DimmDFE[USER_PROFILE_4][1][0][0],
    MemInfoData->DimmDFE[USER_PROFILE_5][1][0][0]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_C1D0_DIMM_TAP2_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmDFE[DEFAULT_SPD_PROFILE][1][0][1],
    MemInfoData->DimmDFE[CUSTOM_PROFILE][1][0][1],
    MemInfoData->DimmDFE[XMP_PROFILE_1][1][0][1],
    MemInfoData->DimmDFE[XMP_PROFILE_2][1][0][1],
    MemInfoData->DimmDFE[XMP_PROFILE_3][1][0][1],
    MemInfoData->DimmDFE[USER_PROFILE_4][1][0][1],
    MemInfoData->DimmDFE[USER_PROFILE_5][1][0][1]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_C1D1_DIMM_TAP1_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmDFE[DEFAULT_SPD_PROFILE][1][1][0],
    MemInfoData->DimmDFE[CUSTOM_PROFILE][1][1][0],
    MemInfoData->DimmDFE[XMP_PROFILE_1][1][1][0],
    MemInfoData->DimmDFE[XMP_PROFILE_2][1][1][0],
    MemInfoData->DimmDFE[XMP_PROFILE_3][1][1][0],
    MemInfoData->DimmDFE[USER_PROFILE_4][1][1][0],
    MemInfoData->DimmDFE[USER_PROFILE_5][1][1][0]
    );

  InitString (
    HiiHandle,
    STRING_TOKEN (STR_MEMORY_C1D1_DIMM_TAP2_DEFAULT_VALUE),
    Timing5cString,
    MemInfoData->DimmDFE[DEFAULT_SPD_PROFILE][1][1][1],
    MemInfoData->DimmDFE[CUSTOM_PROFILE][1][1][1],
    MemInfoData->DimmDFE[XMP_PROFILE_1][1][1][1],
    MemInfoData->DimmDFE[XMP_PROFILE_2][1][1][1],
    MemInfoData->DimmDFE[XMP_PROFILE_3][1][1][1],
    MemInfoData->DimmDFE[USER_PROFILE_4][1][1][1],
    MemInfoData->DimmDFE[USER_PROFILE_5][1][1][1]
    );
*/
  InitMaxOcRatioLimitStrings (HiiHandle, Class);

  VariableSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  &VariableAttr,
                  &VariableSize,
                  &SetupData
                  );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (SA_SETUP);
  Status = gRT->GetVariable (
                  L"SaSetup",
                  &gSaSetupVariableGuid,
                  &VariableAttr,
                  &VariableSize,
                  &SaSetup
                  );
  ASSERT_EFI_ERROR (Status);
  //
  // Store the Factory configured values if IsOcDefaultsInitalized == 0.
  // IsOcDefaultsInitalized will be 0 :
  //    1. During first boot
  //    2. If User goes into BIOS setup and selects: Restore Defaults
  //
  if (SetupData.IsOcDefaultsInitalized == 0) {
    //
    // Loading the default values from the MemInfoHobProtocol, using the currently running profile
    //
    MemoryTiming         = &MemInfoData->Timing[MemInfoData->Profile];
    if (!EFI_ERROR (Status)) {
      SaSetup.tCL          = (UINT8) MemoryTiming->tCL;
      SaSetup.tCWL         = (UINT8) MemoryTiming->tCWL;
      SaSetup.tFAW         = MemoryTiming->tFAW;
      SaSetup.tRAS         = MemoryTiming->tRAS;
      SaSetup.tRCDtRP      = (UINT8) MemoryTiming->tRCDtRP;
      SaSetup.tREFI        = MemoryTiming->tREFI;
      SaSetup.tRFC         = MemoryTiming->tRFC;
      SaSetup.tRRD         = (UINT8) MemoryTiming->tRRD;
      SaSetup.tRTP         = (UINT8) MemoryTiming->tRTP;
      SaSetup.tWR          = (UINT8) MemoryTiming->tWR;
      SaSetup.tWTR         = (UINT8) MemoryTiming->tWTR;
      SaSetup.tRFCpb       = (UINT8) MemoryTiming->tRFCpb;
      SaSetup.tRFC2        = (UINT8) MemoryTiming->tRFC2;
      SaSetup.tRFC4        = (UINT8) MemoryTiming->tRFC4;
      SaSetup.tRRD_L       = (UINT8) MemoryTiming->tRRD_L;
      SaSetup.tRRD_S       = (UINT8) MemoryTiming->tRRD_S;
      SaSetup.tWTR_L       = (UINT8) MemoryTiming->tWTR_L;
      SaSetup.tCCD_L       = (UINT8) MemoryTiming->tCCD_L;
      SaSetup.tCCD_L_WR    = (UINT8) MemoryTiming->tCCD_L_WR;
      SaSetup.tWTR_S       = (UINT8) MemoryTiming->tWTR_S;
      SaSetup.NModeSupport = (UINT8) MemoryTiming->NMode;
      SaSetup.DdrRatio     = MemInfoData->Ratio;
      SaSetup.DdrRefClk    = MemInfoData->RefClk;

      Status = gRT->SetVariable (
                      L"SaSetup",
                      &gSaSetupVariableGuid,
                      VariableAttr,
                      sizeof (SA_SETUP),
                      &SaSetup
                      );
      ASSERT_EFI_ERROR (Status);
    }
  }

#endif
}

/**
  This funciton uses the tCK value to choose a clock ratio based on reference clock.

  @param[in]      tCKvalue       - The tCK value to be converted to frequency ratio.
  @param[in]      DdrType        - Memory Type, 1 is Ddr5, 2 is Lpddr5
  @param[in,out]  Ratio          - Used as return value for chosen Ratio.

  @retval Nothing
**/
VOID
ConvertClock2FreqRatio (
  IN     UINT32               tCKvalue,
  IN     UINT8                DdrType,
  IN OUT UINT16               *Ratio
  )
{
  UINT32  RefClkValue;
  UINT32  Factor;
  UINT64  Value;
  UINT32  BClkValue;

  /*
   * @Todo:
   *  Need update to get the right BClkFrequency
   *  Temporarily, set BClkFrequency to 0
   */

  BClkValue     = 100000000UL;
  Factor        = BClkValue / 100000UL;
  RefClkValue   = 333333333UL;

  Value = 10000000000000000000ULL;
  Value = DivU64x32(Value, Factor);
  Value = MultU64x64(Value, (DdrType == MRC_DDR_TYPE_LPDDR5) ? 8 : 2);
  Value = DivU64x32(Value, RefClkValue);
  Value = ((tCKvalue == 0) ? 0 : DivU64x32 (Value + (tCKvalue >> 1), tCKvalue));
  *Ratio = (UINT16) Value;

  return;
}

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
  )
{
  CPU_SETUP               *CpuSetup;
  UINT8                   LfmRatio;
  UINT8                   HfmRatio;
  UINTN                   VarSize;
  EFI_STATUS              Status;
  PCH_SETUP               *PchSetup;
  EFI_STRING              RequestString;
  EFI_STRING              RequestString2;
  EFI_STRING              RequestString3;
  EFI_STRING              RequestStringOcWdt;
  UINT8                   Index;
  OC_SETUP                *OcSetup;

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  Status = EFI_SUCCESS;
  RequestString  = NULL;
  RequestString2 = NULL;
  RequestString3 = NULL;
  RequestStringOcWdt = NULL;

  VarSize = sizeof (CPU_SETUP);
  CpuSetup = AllocatePool (VarSize);
  ASSERT (CpuSetup != NULL);
  if (CpuSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  // GetBrowserData by VarStore Name (CpuSetup)
  if (!HiiGetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *) CpuSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  GetBusRatio (&HfmRatio, &LfmRatio);

  VarSize = sizeof(PCH_SETUP);
  PchSetup = AllocatePool(VarSize);
  ASSERT(PchSetup != NULL);
  if (PchSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // GetBrowserData by VarStore Name (PchSetup)
  if (!HiiGetBrowserData(&gPchSetupVariableGuid, L"PchSetup", VarSize, (UINT8 *)PchSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

VarSize = sizeof(OC_SETUP);
  OcSetup = AllocatePool(VarSize);
  ASSERT(OcSetup != NULL);
  if (OcSetup == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  // GetBrowserData by VarStore Name (OcSetup)
  if (!HiiGetBrowserData(&gOcSetupVariableGuid, L"OcSetup", VarSize, (UINT8 *)OcSetup)) {
    Status = EFI_NOT_FOUND;
  }
  ASSERT_EFI_ERROR (Status);

  switch (KeyValue) {
    case KEY_CPU_D2D_RATIO:
      if ((OcSetup->CpuD2dRatio < 15) || (OcSetup->CpuD2dRatio > 40)) {
        OcSetup->CpuD2dRatio = 0;
      }
      RequestString = HiiConstructRequestString(RequestString, OFFSET_OF(OC_SETUP, CpuD2dRatio), sizeof (OcSetup->CpuD2dRatio));
      break;

    case  KEY_TjMaxOffset:
      if ((OcSetup->TjMaxOffset < 10) || (OcSetup->TjMaxOffset > 63)) {
        OcSetup->TjMaxOffset = 0;
      }
      RequestString = HiiConstructRequestString(RequestString, OFFSET_OF(OC_SETUP, TjMaxOffset), sizeof (OcSetup->TjMaxOffset));
      break;

    case KEY_CORE_MAX_OC_RATIO_LIMIT:
      //
      // Ensure that max oc ratio limit does not go below the LFM ratio
      //
      if ((OcSetup->MaxOcRatio[MAILBOX_OC_DOMAIN_ID_IA_CORE] < LfmRatio) && (OcSetup->MaxOcRatio[MAILBOX_OC_DOMAIN_ID_IA_CORE] != 0)) {
        OcSetup->MaxOcRatio[MAILBOX_OC_DOMAIN_ID_IA_CORE] = LfmRatio;
      }
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (OC_SETUP, MaxOcRatio[MAILBOX_OC_DOMAIN_ID_IA_CORE]), sizeof (OcSetup->MaxOcRatio[MAILBOX_OC_DOMAIN_ID_IA_CORE]));
      break;

    case KEY_RING_MIN_RATIO_LIMIT:
      //
      // Ensure that min ring ratio limit does not go below the LFM ratio
      //
      if ((CpuSetup->MinRingRatioLimit < LfmRatio) && (CpuSetup->MinRingRatioLimit != 0)) {
        CpuSetup->MinRingRatioLimit = LfmRatio;
      }
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, MinRingRatioLimit), sizeof (CpuSetup->MinRingRatioLimit));
      break;

    case KEY_RING_MAX_RATIO_LIMIT:
      //
      // Ensure that max ring ratio limit does not go below the LFM ratio
      //
      if ((CpuSetup->MaxRingRatioLimit < LfmRatio) && (CpuSetup->MaxRingRatioLimit != 0)) {
        CpuSetup->MaxRingRatioLimit = LfmRatio;
      }
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (CPU_SETUP, MaxRingRatioLimit), sizeof (CpuSetup->MaxRingRatioLimit));
      break;

    case KEY_RING_MAX_OC_RATIO_LIMIT:
      //
      // Ensure that max ring OC ratio limit does not go below the LFM ratio
      //
      if ((OcSetup->MaxOcRatio[MAILBOX_OC_DOMAIN_ID_RING] < LfmRatio) && (OcSetup->MaxOcRatio[MAILBOX_OC_DOMAIN_ID_RING] != 0)) {
        OcSetup->MaxOcRatio[MAILBOX_OC_DOMAIN_ID_RING] = LfmRatio;
      }
      RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (OC_SETUP, MaxOcRatio[MAILBOX_OC_DOMAIN_ID_RING]), sizeof (OcSetup->MaxOcRatio[MAILBOX_OC_DOMAIN_ID_RING]));
      break;

    case KEY_OC_ENABLE_DEPENDENCY:
      OcSetup->IaIccMax = mIaIccMax;
      OcSetup->GtIccMax = mIaIccMax;
      PchSetup->WdtEnable = (UINT8)OcSetup->OverclockingSupport;
      RequestString = HiiConstructRequestString(RequestString, OFFSET_OF(OC_SETUP, IaIccMax), sizeof (OcSetup->IaIccMax));
      RequestString2 = HiiConstructRequestString(RequestString2, OFFSET_OF(OC_SETUP, GtIccMax), sizeof (OcSetup->GtIccMax));
      RequestStringOcWdt = HiiConstructRequestString(RequestStringOcWdt, OFFSET_OF(PCH_SETUP, WdtEnable), sizeof(PchSetup->WdtEnable));
      break;

    case KEY_PERCORE_DISABLE_DEPENDENCY:
      VarSize = sizeof (CPU_SETUP);
      if (OcSetup->PerCoreDisableConfiguration == 1) {
        CpuSetup->ActiveCoreCount = 0xFF;
        CpuSetup->ActiveSmallCoreCount = 0xFF;
        RequestString  = HiiConstructRequestString(RequestString, OFFSET_OF(CPU_SETUP, ActiveCoreCount), sizeof (CpuSetup->ActiveCoreCount));
        RequestString2 = HiiConstructRequestString(RequestString2, OFFSET_OF(CPU_SETUP, ActiveSmallCoreCount), sizeof (CpuSetup->ActiveSmallCoreCount));
      } else {
        for (Index = 0; Index < MAX_OC_CORES; ++Index) {
          OcSetup->PerCoreDisable[Index] = 0;
        }
        RequestString3 = HiiConstructRequestString(RequestString3, OFFSET_OF(OC_SETUP, PerCoreDisable), (sizeof (OcSetup->PerCoreDisable[0])*40));
        if (RequestString3 != NULL) {
          if (!HiiSetBrowserData (&gOcSetupVariableGuid, L"OcSetup", VarSize, (UINT8 *)OcSetup, RequestString3)) {
            Status = EFI_NOT_FOUND;
          }
          ASSERT_EFI_ERROR (Status);
        }
      }
      break;

    default:
      ASSERT(FALSE);
  }

  if (RequestString != NULL) {
    VarSize = sizeof (CPU_SETUP);
    if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *)CpuSetup, RequestString)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    VarSize = sizeof (OC_SETUP);
    if (!HiiSetBrowserData (&gOcSetupVariableGuid, L"OcSetup", VarSize, (UINT8 *)OcSetup, RequestString)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (RequestString);
  }

  if (RequestString2 != NULL) {
    if (!HiiSetBrowserData (&gCpuSetupVariableGuid, L"CpuSetup", VarSize, (UINT8 *)CpuSetup, RequestString2)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    VarSize = sizeof (OC_SETUP);
    if (!HiiSetBrowserData (&gOcSetupVariableGuid, L"OcSetup", VarSize, (UINT8 *)OcSetup, RequestString2)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR (Status);
    FreePool (RequestString2);
  }

  if (RequestString3 != NULL) {
    FreePool (RequestString3);
  }

  if (RequestStringOcWdt != NULL) {
    if (!HiiSetBrowserData(&gPchSetupVariableGuid, L"PchSetup", VarSize, (UINT8 *)PchSetup, RequestStringOcWdt)) {
      Status = EFI_NOT_FOUND;
    }
    ASSERT_EFI_ERROR(Status);
    FreePool(RequestStringOcWdt);
  }

  FreePool(CpuSetup);
  FreePool(PchSetup);
  FreePool(OcSetup);

  return EFI_SUCCESS;
}

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
  )
{
  EFI_STATUS                      Status;
  SA_SETUP                        SaSetup;
  UINTN                           VariableSize;
  EFI_STRING                      RequestString;
  MEMORY_TIMING                   *MemoryTiming;
  UINT8                           MemoryProfile;
  MEM_INFO_PROTOCOL               *MemInfoHobProtocol;
  MEMORY_INFO_DATA                *MemInfoData;

  RequestString = NULL;

  if (Action != EFI_BROWSER_ACTION_CHANGING && Action != EFI_BROWSER_ACTION_CHANGED) {
    return EFI_UNSUPPORTED;
  }

  // Get the Memory Info HOB Protocol if it exists.
  Status = gBS->LocateProtocol (&gMemInfoProtocolGuid, NULL, (VOID **) &MemInfoHobProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  VariableSize = sizeof (SA_SETUP);
  if (!HiiGetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VariableSize, (UINT8 *) &SaSetup)) {
    return EFI_NOT_FOUND;
  }

  MemoryProfile = SaSetup.SpdProfileSelected;

  // If the MemoryProfile is changed to Custom, do display updated timing values
  if (MemoryProfile == CUSTOM_PROFILE) {
    return EFI_UNSUPPORTED;
  }

  MemInfoData  = &MemInfoHobProtocol->MemInfoData;
  MemoryTiming = &MemInfoData->Timing[MemoryProfile];

  SaSetup.tCL           = (UINT8) MemoryTiming->tCL;
  SaSetup.tCWL          = (UINT8) MemoryTiming->tCWL;
  SaSetup.tFAW          = MemoryTiming->tFAW;
  SaSetup.tRAS          = MemoryTiming->tRAS;
  SaSetup.tRCDtRP       = (UINT8) MemoryTiming->tRCDtRP;
  SaSetup.tREFI         = MemoryTiming->tREFI;
  SaSetup.tRFC          = MemoryTiming->tRFC;
  SaSetup.tRRD          = (UINT8) MemoryTiming->tRRD;
  SaSetup.tRTP          = (UINT8) MemoryTiming->tRTP;
  SaSetup.tWR           = (UINT8) MemoryTiming->tWR;
  SaSetup.tWTR          = (UINT8) MemoryTiming->tWTR;
  SaSetup.tRFCpb        = (UINT16) MemoryTiming->tRFCpb;
  SaSetup.tRFC2         = (UINT16) MemoryTiming->tRFC2;
  SaSetup.tRFC4         = (UINT16) MemoryTiming->tRFC4;
  SaSetup.tRRD_L        = (UINT8) MemoryTiming->tRRD_L;
  SaSetup.tRRD_S        = (UINT8) MemoryTiming->tRRD_S;
  SaSetup.tWTR_L        = (UINT8) MemoryTiming->tWTR_L;
  SaSetup.tCCD_L        = (UINT8) MemoryTiming->tCCD_L;
  SaSetup.tCCD_L_WR     = (UINT8) MemoryTiming->tCCD_L_WR;
  SaSetup.tWTR_S        = (UINT8) MemoryTiming->tWTR_S;
  SaSetup.NModeSupport  = (UINT8) MemoryTiming->NMode;
  SaSetup.MemoryVddVoltage    = MemInfoData->VddVoltage[MemoryProfile];
  SaSetup.MemoryVddqVoltage   = MemInfoData->VddqVoltage[MemoryProfile];
  SaSetup.MemoryVppVoltage    = MemInfoData->VppVoltage[MemoryProfile];
  ConvertClock2FreqRatio(MemoryTiming->tCK, MemInfoData->DdrType, &SaSetup.DdrRatio);

  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tCL), sizeof (SaSetup.tCL));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tCWL), sizeof (SaSetup.tCWL));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tFAW), sizeof (SaSetup.tFAW));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRAS), sizeof (SaSetup.tRAS));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRCDtRP), sizeof (SaSetup.tRCDtRP));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tREFI), sizeof (SaSetup.tREFI));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRFC), sizeof (SaSetup.tRFC));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRRD), sizeof (SaSetup.tRRD));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRTP), sizeof (SaSetup.tRTP));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tWR), sizeof (SaSetup.tWR));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tWTR), sizeof (SaSetup.tWTR));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRFCpb), sizeof (SaSetup.tRFCpb));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRFC2), sizeof (SaSetup.tRFC2));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRFC4), sizeof (SaSetup.tRFC4));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRRD_L), sizeof (SaSetup.tRRD_L));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tRRD_S), sizeof (SaSetup.tRRD_S));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tWTR_L), sizeof (SaSetup.tWTR_L));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tCCD_L), sizeof (SaSetup.tCCD_L));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tCCD_L_WR), sizeof (SaSetup.tCCD_L_WR));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, tWTR_S), sizeof (SaSetup.tWTR_S));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, NModeSupport), sizeof (SaSetup.NModeSupport));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, MemoryVddVoltage), sizeof (SaSetup.MemoryVddVoltage));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, MemoryVddqVoltage), sizeof (SaSetup.MemoryVddqVoltage));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, MemoryVppVoltage), sizeof (SaSetup.MemoryVppVoltage));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, DdrRefClk), sizeof (SaSetup.DdrRefClk));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, DdrRatio), sizeof (SaSetup.DdrRatio));
  RequestString = HiiConstructRequestString (RequestString, OFFSET_OF (SA_SETUP, DdrOddRatioMode), sizeof (SaSetup.DdrOddRatioMode));

  if (RequestString != NULL) {
    if (!HiiSetBrowserData (&gSaSetupVariableGuid, L"SaSetup", VariableSize, (UINT8 *) &SaSetup, RequestString)) {
      return EFI_NOT_FOUND;
    }
    FreePool (RequestString);
  }

  return EFI_SUCCESS;
}
