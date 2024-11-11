/** @file
  By passing in a SPD data structure and platform support values, an output
  structure is populated with DIMM configuration information.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#include "BlueMrcSpdProcessingPrivate.h"  // to cross check private function prototypes
#include "BlueMrcSpdProcessingApi.h" // to cross check implementation against prototypes
#include "MrcSpdProcessingDefs.h"  // for MRC_SPD5_SDRAM_DENSITY_4Gb
#include "CBlueMrcCommonTypes.h"
#include "MrcDdr5.h"
#include "MrcLpddr5.h"
#include "MrcChipApi.h"
#include "MrcPmaApiCrossProj.h"
#include "MrcSagv.h"
#if defined(FULL_HEADLESS) && !defined(NEW_STUB)
#include "StubMrcUcManagement.h" // for MrcStubWriteMrcData()
#endif

#ifdef MRC_DEBUG_PRINT
const char  UnknownString[]    = "unknown";
const char  Ddr5String[]       = "DDR5";
const char  RdimmString[]      = "RDIMM";
const char  UdimmString[]      = "UDIMM";
const char  SodimmString[]     = "SO-DIMM";
const char  Sodimm72String[]   = "72 bit SO-DIMM";
const char  CudimmString[]     = "CUDIMM";
const char  CsodimmString[]    = "CSODIMM";
const char  StdString[]        = "Standard";
const char  CustomString[]     = "Custom";
const char  Xmp1String[]       = "XMP1";
const char  Xmp2String[]       = "XMP2";
const char  Xmp3String[]       = "XMP3";
const char  User4String[]      = "USER4";
const char  User5String[]      = "USER5";
const char  XmpNameString[]    = "  Profile%d Name:";
const char  XpString[]         = "  XMP profile %u is %sabled and recommended channel config: %u DIMM per channel\n";
const char  ErrorString[]      = "ERROR: Unsupported ";
const char  SpdValString[]     = "SPD value: ";
const char  IsSupString[]      = " is supported";
const char  NotSupString[]     = " is not supported";
const char  TimeBaseString[]   = "Timebase (MTB/FTB)";
const char  tAAString[]        = "CAS Latency Time (tAAmin)";
const char  tCKString[]        = "SDRAM Cycle Time (tCKmin)";
const char  tWRString[]        = "Write recovery time (tWRmin)";
const char  tRCDString[]       = "RAS# to CAS# delay time (tRCDmin)";
const char  tRRDString[]       = "Row active to row active delay time (tRRDmin)";
const char  tRPString[]        = "Row precharge delay time (tRPmin)";
const char  Lpddr5String[]     = "LPDDR5";
const char  Lpddr5XString[]     = "LPDDR5X";
const char  LpDimmString[]     = "LP-DIMM";
const char  MemoryDownString[] = "Memory Down";
const char  Ddr5CammString[]   = "DDR5 CAMM";
const char  Lpddr5CammString[] = "LPDDR5 CAMM";
const char  tRPabString[]      = "Row precharge delay time for all banks (tRPab)";
const char  tRASString[]       = "Active to precharge delay time (tRASmin)";
const char  tRCString[]        = "Active to active/refresh delay time (tRCmin)";
const char  tRFCString[]       = "Refresh recovery delay time (tRFCmin)";
const char  tRFCpbString[]     = "Per Bank refresh recovery delay time (tRFCpb)";
const char  tWTRString[]       = "Internal write to read command delay time (tWTRmin)";
const char  tRTPString[]       = "Internal read to precharge delay time (tRTPmin)";
const char  tFAWString[]       = "Active to active/refresh delay time (tFAWmin)";
const char  tREFIString[]      = "Average Periodic Refresh Interval (tREFImin)";
const char  tCWLString[]       = "CAS Write Latency (tCWLmin)";
const char  NmodeString[]      = "Command rate mode (Nmode)";
const char  VoltageString[]    = "Module voltage (mVolts)";
const char  VddString[]        = "VDD";
const char  VddqString[]       = "VDDQ";
const char  VppString[]        = "VPP";
const char  BestCaseString[]   = "Best case value for profiles 0-";
const char  ProfileString[]    = "Profile";
const char  HeaderString[]     = "Profile Controller Channel Dimm Value";
const char  tRRDSString[]      = "Row active to row active delay time (tRRD_Smin)";
const char  tRRDLString[]      = "Row active to row active delay time (tRRD_Lmin)";
const char  tRFC2String[]      = "Refresh recovery delay time (tRFC2min)";
const char  tRFC4String[]      = "Refresh recovery delay time (tRFC4min)";
const char  tWTRLString[]      = "Internal write to read command delay time (tWTR_L)";
const char  tWTRSString[]      = "Internal write to read command delay time (tWTR_S)";
const char  tCCDLString[]      = "CAS to CAS delay for same bank group (tCCD_L)";
const char  tCCDLWRString[]    = "Minimum Write-to-Write delay for same bank group (tCCD_L_WR)";
const char  ChPerSdramPkgStr[] = "Channels per SDRAM Package";
const char  RrcString[][3]     = {
                                     " A", " B", " C", " D", " E", " F", " G", " H", " J", " K",
                                     " L", " M", " N", " P", " R", " T", " U", " V", " W", " Y",
                                     "AA", "AB", "AC", "AD", "AE", "AF", "AG", "AH", "AJ", "AK",
                                     "AL", "ZZ", "AM", "AN", "AP", "AR", "AT", "AU", "AV", "AW",
                                     "AY", "BA", "BB", "BC", "BD", "BE", "BF", "BG", "BH", "BJ",
                                     "BK", "BL", "BM", "BN", "BP", "BR", "BT", "BU", "BV", "BW",
                                     "BY", "CA", "CB", "ZZ"};
#endif // MRC_DEBUG_PRINT

                                   // Ratio
                                   // 33.33
const TRangeTable FreqTable[] = { //Gear 1   Gear 2       Gear 4
  { 0xFFFFFFFF,        fInvalid }, //--------------------  ---------
  { MRC_DDR_1067_TCK_MIN, f1067 }, //  34      14
  { MRC_DDR_1134_TCK_MIN, f1133 }, //  36      18
  { MRC_DDR_1200_TCK_MIN, f1200 }, //  38      19
  { MRC_DDR_1267_TCK_MIN, f1267 }, //  40      20
  { MRC_DDR_1334_TCK_MIN, f1333 }, //  42      21
  { MRC_DDR_1400_TCK_MIN, f1400 }, //  44      22
  { MRC_DDR_1534_TCK_MIN, f1533 }, //  46      23
  { MRC_DDR_1600_TCK_MIN, f1600 }, //  48      24 (POR MIN)
  { MRC_DDR_1667_TCK_MIN, f1667 }, //  50      25
  { MRC_DDR_1733_TCK_MIN, f1733 }, //  52      26
  { MRC_DDR_1800_TCK_MIN, f1800 }, //  54      27
  { MRC_DDR_1867_TCK_MIN, f1867 }, //  56      28
  { MRC_DDR_1933_TCK_MIN, f1933 }, //  58      29
  { MRC_DDR_2000_TCK_MIN, f2000 }, //  60      30
  { MRC_DDR_2067_TCK_MIN, f2067 }, //  62      31
  { MRC_DDR_2133_TCK_MIN, f2133 }, //  64      32           16
  { MRC_DDR_2200_TCK_MIN, f2200 }, //  66      33
  { MRC_DDR_2267_TCK_MIN, f2267 }, //  68      34           17
  { MRC_DDR_2333_TCK_MIN, f2333 }, //  70      35
  { MRC_DDR_2400_TCK_MIN, f2400 }, //  72      36           18 (POR MIN)
  { MRC_DDR_2467_TCK_MIN, f2467 }, //  74      37
  { MRC_DDR_2533_TCK_MIN, f2533 }, //  76      38           19
  { MRC_DDR_2600_TCK_MIN, f2600 }, //  78      39
  { MRC_DDR_2667_TCK_MIN, f2667 }, //  80      40           20
  { MRC_DDR_2733_TCK_MIN, f2733 }, //  82      41
  { MRC_DDR_2800_TCK_MIN, f2800 }, //  84      42           21
  { MRC_DDR_2867_TCK_MIN, f2867 }, //  86      43
  { MRC_DDR_2933_TCK_MIN, f2933 }, //  88      44           22
  { MRC_DDR_3000_TCK_MIN, f3000 }, //  90      45
  { MRC_DDR_3067_TCK_MIN, f3067 }, //  92      46           23
  { MRC_DDR_3133_TCK_MIN, f3133 }, //  94      47
  { MRC_DDR_3200_TCK_MIN, f3200 }, //  96      48 (POR MAX) 24
  { MRC_DDR_3267_TCK_MIN, f3267 }, //  98      49
  { MRC_DDR_3333_TCK_MIN, f3333 }, //  100     50           25
  { MRC_DDR_3400_TCK_MIN, f3400 }, //  102     51
  { MRC_DDR_3467_TCK_MIN, f3467 }, //  104     52           26
  { MRC_DDR_3533_TCK_MIN, f3533 }, //  106     53
  { MRC_DDR_3600_TCK_MIN, f3600 }, //  108     54           27
  { MRC_DDR_3667_TCK_MIN, f3667 }, //  110     55
  { MRC_DDR_3733_TCK_MIN, f3733 }, //  112     56           28
  { MRC_DDR_3800_TCK_MIN, f3800 }, //  114     57
  { MRC_DDR_3867_TCK_MIN, f3867 }, //  116     58           29
  { MRC_DDR_3933_TCK_MIN, f3933 }, //  118     59
  { MRC_DDR_4000_TCK_MIN, f4000 }, //  120     60           30
  { MRC_DDR_4067_TCK_MIN, f4067 }, //  122     61
  { MRC_DDR_4133_TCK_MIN, f4133 }, //  124     62           31
  { MRC_DDR_4200_TCK_MIN, f4200 }, //  126     63
  { MRC_DDR_4267_TCK_MIN, f4267 }, //  128     64           32
  { MRC_DDR_4333_TCK_MIN, f4333 }, //  130     65
  { MRC_DDR_4400_TCK_MIN, f4400 }, //  132     66           33
  { MRC_DDR_4467_TCK_MIN, f4467 }, //  134     67
  { MRC_DDR_4533_TCK_MIN, f4533 }, //  136     68           34
  { MRC_DDR_4600_TCK_MIN, f4600 }, //  138     69
  { MRC_DDR_4667_TCK_MIN, f4667 }, //  140     70           35
  { MRC_DDR_4733_TCK_MIN, f4733 }, //  142     71
  { MRC_DDR_4800_TCK_MIN, f4800 }, //  144     72           36
  { MRC_DDR_4867_TCK_MIN, f4867 }, //  146     73
  { MRC_DDR_4933_TCK_MIN, f4933 }, //  148     74           37
  { MRC_DDR_5000_TCK_MIN, f5000 }, //  150     75
  { MRC_DDR_5067_TCK_MIN, f5067 }, //  152     76           38
  { MRC_DDR_5133_TCK_MIN, f5133 }, //  154     77
  { MRC_DDR_5200_TCK_MIN, f5200 }, //  156     78           39
  { MRC_DDR_5267_TCK_MIN, f5267 }, //  158     79
  { MRC_DDR_5333_TCK_MIN, f5333 }, //  160     80           40
  { MRC_DDR_5400_TCK_MIN, f5400 }, //  162     81
  { MRC_DDR_5467_TCK_MIN, f5467 }, //  164     82           41
  { MRC_DDR_5533_TCK_MIN, f5533 }, //  166     83
  { MRC_DDR_5600_TCK_MIN, f5600 }, //  168     84           42
  { MRC_DDR_5667_TCK_MIN, f5667 }, //  170     85
  { MRC_DDR_5733_TCK_MIN, f5733 }, //  172     86           43
  { MRC_DDR_5800_TCK_MIN, f5800 }, //  174     87
  { MRC_DDR_5867_TCK_MIN, f5867 }, //  176     88           44
  { MRC_DDR_5933_TCK_MIN, f5933 }, //  178     89
  { MRC_DDR_6000_TCK_MIN, f6000 }, //  180     90           45
  { MRC_DDR_6067_TCK_MIN, f6067 }, //  182     91
  { MRC_DDR_6133_TCK_MIN, f6133 }, //  184     92           46
  { MRC_DDR_6200_TCK_MIN, f6200 }, //  186     93
  { MRC_DDR_6267_TCK_MIN, f6267 }, //  188     94           47
  { MRC_DDR_6333_TCK_MIN, f6333 }, //  190     95
  { MRC_DDR_6400_TCK_MIN, f6400 }, //  192     96           48
  { MRC_DDR_6533_TCK_MIN, f6533 }, //  196     98           49
  { MRC_DDR_6667_TCK_MIN, f6667 }, //  200     100          50
  { MRC_DDR_6800_TCK_MIN, f6800 }, //  204     102          51
  { MRC_DDR_6933_TCK_MIN, f6933 }, //  208     104          52
  { MRC_DDR_7067_TCK_MIN, f7067 }, //  212     106          53
  { MRC_DDR_7200_TCK_MIN, f7200 }, //  216     108          54
  { MRC_DDR_7333_TCK_MIN, f7333 }, //  220     110          55
  { MRC_DDR_7467_TCK_MIN, f7467 }, //  224     112          56
  { MRC_DDR_7600_TCK_MIN, f7600 }, //  228     114          57
  { MRC_DDR_7733_TCK_MIN, f7733 }, //  232     116          58
  { MRC_DDR_7867_TCK_MIN, f7867 }, //  236     118          59
  { MRC_DDR_8000_TCK_MIN, f8000 }, //  240     120          60
  { MRC_DDR_8133_TCK_MIN, f8133 }, //  244                  61
  { MRC_DDR_8267_TCK_MIN, f8267 }, //  248                  62
  { MRC_DDR_8400_TCK_MIN, f8400 }, //  252                  63
  { MRC_DDR_8533_TCK_MIN, f8533 }, //  256                  64 (POR MAX)
  { MRC_DDR_8667_TCK_MIN, f8667 }, //  260                  65
  { MRC_DDR_8800_TCK_MIN, f8800 }, //  264                  66
  { MRC_DDR_8933_TCK_MIN, f8933 }, //  268                  67
  { MRC_DDR_9067_TCK_MIN, f9067 }, //  272                  68
  { MRC_DDR_9200_TCK_MIN, f9200 }, //  276                  69
  { MRC_DDR_9333_TCK_MIN, f9333 }, //  280                  70
  { MRC_DDR_9467_TCK_MIN, f9467 }, //  284                  71
  { MRC_DDR_9600_TCK_MIN, f9600 }, //  288                  72
  { MRC_DDR_9733_TCK_MIN, f9733 }, //  292                  73
  { MRC_DDR_9867_TCK_MIN, f9867 }, //  296                  74
  { MRC_DDR_10000_TCK_MIN, f10000 }, // 300                 75
  { MRC_DDR_10133_TCK_MIN, f10133 }, // 304                 76
  { MRC_DDR_10267_TCK_MIN, f10267 }, // 308                 77
  { MRC_DDR_10400_TCK_MIN, f10400 }, // 312                 78
  { MRC_DDR_10533_TCK_MIN, f10533 }, // 316                 79
  { MRC_DDR_10667_TCK_MIN, f10667 }, // 320                 80
  { MRC_DDR_10800_TCK_MIN, f10800 }, // 324                 81
  { MRC_DDR_10933_TCK_MIN, f10933 }, // 328                 82
  { MRC_DDR_11067_TCK_MIN, f11067 }, // 332                 83
  { MRC_DDR_11200_TCK_MIN, f11200 }, // 336                 84
  { MRC_DDR_11333_TCK_MIN, f11333 }, // 340                 85
  { MRC_DDR_11467_TCK_MIN, f11467 }, // 344                 86
  { MRC_DDR_11600_TCK_MIN, f11600 }, // 348                 87
  { MRC_DDR_11733_TCK_MIN, f11733 }, // 352                 88
  { MRC_DDR_11867_TCK_MIN, f11867 }, // 356                 89
  { MRC_DDR_12000_TCK_MIN, f12000 }, // 360                 90
  { MRC_DDR_12133_TCK_MIN, f12133 }, // 364                 91
  { MRC_DDR_12267_TCK_MIN, f12267 }, // 368                 92
  { MRC_DDR_12400_TCK_MIN, f12400 }, // 372                 93
  { MRC_DDR_12533_TCK_MIN, f12533 }, // 376                 94
  { MRC_DDR_12667_TCK_MIN, f12667 }, // 380                 95
  { MRC_DDR_12800_TCK_MIN, f12800 }, // 384                 96
  { MRC_DDR_12933_TCK_MIN, f12933 }, // 388                 97
  { MRC_DDR_13067_TCK_MIN, f13067 }, // 392                 98
  { MRC_DDR_13200_TCK_MIN, f13200 }, // 396                 99
  { MRC_DDR_13333_TCK_MIN, f13333 }, // 400                 100
  { MRC_DDR_13467_TCK_MIN, f13467 }, // 404                 101
  { MRC_DDR_13600_TCK_MIN, f13600 }, // 408                 102
  { MRC_DDR_13733_TCK_MIN, f13733 }, // 412                 103
  { MRC_DDR_13867_TCK_MIN, f13867 }, // 416                 104
  { MRC_DDR_14000_TCK_MIN, f14000 }, // 420                 105
  { MRC_DDR_14133_TCK_MIN, f14133 }, // 424                 106
  { MRC_DDR_14267_TCK_MIN, f14267 }, // 428                 107
  { MRC_DDR_14400_TCK_MIN, f14400 }, // 432                 108
  { MRC_DDR_14533_TCK_MIN, f14533 }, // 436                 109
  { MRC_DDR_14667_TCK_MIN, f14667 }, // 440                 110
  { MRC_DDR_14800_TCK_MIN, f14800 }, // 444                 111
  { MRC_DDR_14933_TCK_MIN, f14933 }, // 448                 112
  { MRC_DDR_15067_TCK_MIN, f15067 }, // 452                 113
  { 0,                  fNoInit }
};

/**
    Calculate the memory clock value from the current memory frequency.

    @param[in, out] MrcData     - Pointer to MrcData data structure.
    @param[in]      Frequency   - Memory frequency to convert.

    @retval Returns the tCK value in [fs] for the given frequency.
**/
UINT32
ConvertFreq2Clock (
  IN OUT MrcParameters *const MrcData,
  IN     const MrcFrequency   Frequency
  )
{
  MrcOutput *Outputs;
  UINT32    tCKminActual;
  UINT32    Index;

  Outputs = &MrcData->Outputs;
  tCKminActual = MRC_DDR_533_TCK_MIN;

  for (Index = 0; Index < ARRAY_COUNT (FreqTable) - 1; Index++) {
    if (Frequency == FreqTable[Index].Frequency) {
      tCKminActual = FreqTable[Index].tCK;
      break;
    }
    if ((Frequency > FreqTable[Index].Frequency) && (Frequency < FreqTable[Index + 1].Frequency)) {
      tCKminActual = FreqTable[Index].tCK;
      break;
    }
  }
#ifdef MRC_DEBUG_PRINT
  if (Index >= (ARRAY_COUNT (FreqTable) - 1)) {
    MRC_DEBUG_MSG (&Outputs->Debug, MSG_LEVEL_ERROR, "Could not find the matching frequency %u\n", Frequency);
  }
#endif

  if (Outputs->IsLpddr5) {
    // tCK is 4:1 of the data rate.
    tCKminActual *= 4;
  }

  return tCKminActual;
}

/**
  Calculate the memory frequency from the memory clock value.

    @param[in, out] MrcData     - Pointer to MrcData data structure.
    @param[in]      tCKmin      - The tCKmin value in [fs] to convert.
    @param[out]     tCKminIndex - Pointer to the chosen table index.

    @retval Returns the frequency that matches the given tCK.
**/
UINT32
ConvertClock2Freq (
  IN OUT MrcParameters *const  MrcData,
  IN     const UINT32          tCKmin,
  OUT    INT32         *const  tCKminIndex
  )
{
  MrcFrequency  Frequency;
  UINT32        Index;
  UINT32        tCKminPs;
  UINT32        FreqPs;
  UINT32        NextFreqPs;

  //
  // Convert tCK value to the nearest frequency value.
  // Then find slowest valid frequency for the given reference clock.
  // Round to the [ps] resolution, because SPD FineTimeBase is 1ps.
  //
  Frequency = fNoInit;
  if (MrcData->Outputs.IsLpddr5) {
    tCKminPs = tCKmin / 4;
  } else {
    tCKminPs = tCKmin;
  }
  tCKminPs    = UDIVIDEROUND (tCKminPs, 1000);
  for (Index = 0; Index < ARRAY_COUNT (FreqTable) - 1; Index++) {
    FreqPs      = UDIVIDEROUND (FreqTable[Index].tCK, 1000);
    NextFreqPs  = UDIVIDEROUND (FreqTable[Index + 1].tCK, 1000);
    if ((tCKminPs <= FreqPs) && (tCKminPs > NextFreqPs)) {
      Frequency = FreqTable[Index].Frequency;
      break;
    }
  }

  if (tCKminIndex != NULL) {
    *tCKminIndex = Index;
  }
  return Frequency;
}

/**
  Determine if the DIMM slot is filled.
  If a valid DRAM device type and valid module package are found then a DIMM is present.

    @param[in] MrcData - Pointer to MrcData data structure.
    @param[in] Spd     - Pointer to Spd data structure.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
MrcDimmSts
DimmPresence (
  IN MrcDebug      *const Debug,
  IN const MrcSpd  *const Spd
  )
{
  UINT8         DramType;
  UINT8         ModuleType;

  //
  // The following code will more closely identify memory instead of just searching for non-zero data.
  //

  //
  // Check for valid DRAM device type and valid module package
  //
  DramType = Spd->Ddr5.Base.DramDeviceType.Bits.Type;
  ModuleType = Spd->Ddr5.Base.ModuleType.Bits.ModuleType;
  switch (DramType) {
    case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
      switch (ModuleType) {
        case UDimmMemoryPackage:
        case SoDimmMemoryPackage:
        case CammMemoryPackage:
        case CuDimmMemoryPackage:
        case CsoDimmMemoryPackage:
          return DIMM_PRESENT;

        case RDimmMemoryPackage:
          MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "  ERROR: RDIMM is not supported !\n");

        default:
          break;
      }
      break;

    case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
      switch (ModuleType) {
        case LpDimmMemoryPackage:
        case NonDimmMemoryPackage:
        case CammMemoryPackage:
          return DIMM_PRESENT;

        default:
          break;
      }
      break;

    default:
      break;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "  No DIMM detected in slot\n");
  return DIMM_NOT_PRESENT;
}

/**
   Determine if MemoryProfile is XMP

   @param[in] Profile

   @retval TRUE Profile is  XMP PROFILE 1-3 or USER_PROFILE4/5.
   @retval FALSE Profile is not XMP PROFILE 1-3 and not USER_PROFILE4/5.
**/
static
BOOLEAN
IsXmp (
  IN MrcProfile Profile
  )
{
  if ((Profile == XMP_PROFILE1) ||
      (Profile == XMP_PROFILE2) ||
      (Profile == XMP_PROFILE3)) {
    return TRUE;
  }
  /*
   * USER_PROFILE4/5 are similar to XMP_PROFILE
   */
  if ((Profile == USER_PROFILE4) ||
      (Profile == USER_PROFILE5)) {
    return TRUE;
  }

  return FALSE;
}

/**
   Determine if MemoryProfile is XMP and if DIMM doesn't support XMP

   @param[in] MrcData - Pointer to MrcData data structure.
   @param[in] Profile

   @retval TRUE on XMP PROFILE 1-5 and DIMM doesn't support the corresponding XMP profile.
   @retval FALSE XMP profile is not 1-5 or DIMM does support the corresponding XMP profile.
**/
BOOLEAN
NeedIgnoreXmp (
  IN MrcParameters *const MrcData,
  IN MrcProfile           Profile
  )
{
  MrcOutput *Outputs;

  Outputs = &MrcData->Outputs;

  // The function works for DDR5
  if (((Profile == XMP_PROFILE1) && ((Outputs->XmpProfileEnable & XMP_PROFILE1_SUPPORT_MASK) == 0)) ||
      ((Profile == XMP_PROFILE2) && ((Outputs->XmpProfileEnable & XMP_PROFILE2_SUPPORT_MASK) == 0)) ||
      ((Profile == XMP_PROFILE3) && ((Outputs->XmpProfileEnable & XMP_PROFILE3_SUPPORT_MASK) == 0))) {
    return TRUE;
  }

  if (((Profile == USER_PROFILE4) && ((Outputs->XmpProfileEnable & XMP_USER_PROFILE4_SUPPORT_MASK) == 0)) ||
      ((Profile == USER_PROFILE5) && ((Outputs->XmpProfileEnable & XMP_USER_PROFILE5_SUPPORT_MASK) == 0))) {
    return TRUE;
  }

  return FALSE;
}

/**
   Determine if MemoryProfile is XMP and if DIMM supports the corresponding profile

   @param[in] DimmOut
   @param[in] Profile

   @retval TRUE on XMP PROFILE 1-5 and DIMM has the corresponding support.
   @retval FALSE Other profile or DIMM lacks the corresponding support.
**/
static
BOOLEAN
XmpSupport (
  IN MrcDimmOut *const DimmOut,
  IN MrcProfile        Profile
  )
{
  UINT8 XmpSupport;

  XmpSupport = DimmOut->XmpSupport;
  if (((Profile == XMP_PROFILE1) && ((XmpSupport & XMP_PROFILE1_SUPPORT_MASK) != 0)) ||
      ((Profile == XMP_PROFILE2) && ((XmpSupport & XMP_PROFILE2_SUPPORT_MASK) != 0)) ||
      ((Profile == XMP_PROFILE3) && ((XmpSupport & XMP_PROFILE3_SUPPORT_MASK) != 0))) {
    return TRUE;
  }
  if (((Profile == USER_PROFILE4) && ((XmpSupport & XMP_USER_PROFILE4_SUPPORT_MASK) != 0)) ||
      ((Profile == USER_PROFILE5) && ((XmpSupport & XMP_USER_PROFILE5_SUPPORT_MASK) != 0))) {
    return TRUE;
  }

  return FALSE;
}

/**
   Determine if XMP Header and Profiles' CRC are correct

   @param[in, out] MrcData - Pointer to MrcData data structure.
   @param[in]      Spd
   @param[in]      DimmOut

   @retval TRUE CRC is correct.
   @retval FALSE CRC is not correct.
**/
static
BOOLEAN
Xmp3VerifyDimmCrc (
  IN OUT MrcParameters *const MrcData,
  IN     const MrcSpd  *const Spd,
  IN OUT MrcDimmOut    *const DimmOut
  )
{
  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0  *Header5;
  MrcDebug            *Debug;
  UINT16              Crc;
  MrcProfile          Profile;
  UINT32              Index;

  Debug = &MrcData->Outputs.Debug;
  Header5 = &Spd->Ddr5.EndUser.Xmp.Header;

  // Verify that our computed CRC is over the correct range or fail to compile the source
  //MRC_DEBUG_ASSERT ((sizeof(SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0) - sizeof(UINT16)) == SPD_XMP3_GLOBAL_SECTION_SIZE,
  //  "SPD Extreme Memory Profile Header 3.0 must be the same size as SPD_XMP3_GLOBAL_SECTION_SIZE");
  GetDimmCrc ((UINT8 *) (&Header5->XmpId), SPD_XMP3_GLOBAL_SECTION_SIZE, &Crc);
  if (Crc != Header5->Crc) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "SPD XMP Header CRC error, Calculated CRC:0x%x, Header CRC:0x%x\n", Crc, Header5->Crc);
    // Don't return currently. Later on, we might change codes for strict checking
    // return FALSE;
  }

  // Verify XMP_PROFILE1/2/3
  for (Profile = 0; Profile < (XMP_PROFILE3 - XMP_PROFILE1 + 1); Profile++) {
    GetDimmCrc ((UINT8 *) (&Spd->Ddr5.EndUser.Xmp.Data[Profile]), SPD_XMP3_PROFILE_SIZE, &Crc);
    if (Crc != Spd->Ddr5.EndUser.Xmp.Data[Profile].Crc) {
      MRC_DEBUG_MSG (Debug,
          MSG_LEVEL_WARNING,
          "SPD XMP Profile%u CRC error, Calculated CRC:0x%x, Profile CRC:0x%x\n",
          Profile + 1,
          Crc,
          Spd->Ddr5.EndUser.Xmp.Data[Profile].Crc);
      // Don't return currently. Later on, we might change codes for strict checking
      // return FALSE;
    }
  }

  // Verify USER_PROFILE4/5
  for (Profile = USER_PROFILE4; Profile <= USER_PROFILE5; Profile++) {
    Index = Profile - XMP_PROFILE1;
    GetDimmCrc ((UINT8 *) (&Spd->Ddr5.EndUser.Xmp.Data[Index]), SPD_XMP3_PROFILE_SIZE, &Crc);
    if ((Crc != Spd->Ddr5.EndUser.Xmp.Data[Index].Crc) &&
        (Spd->Ddr5.EndUser.Xmp.Data[Index].tCKAVGmin.Data != 0))  {
      // When tCKAVGmin.Data != 0, assume this USER_PROFILE is enabled
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "SPD User Profile%d CRC error", Profile - XMP_PROFILE1 + 1);
    }
  }
  return TRUE;
}


/**
  Determine if the DIMM is valid and supported.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE.
**/
BOOLEAN
ValidDimm (
  IN OUT MrcParameters *const MrcData,
  IN     const MrcSpd  *const Spd,
  IN OUT MrcDimmOut    *const DimmOut
  )
{
  MrcOutput   *Outputs;
  BOOLEAN     IsLpddr;
  BOOLEAN     DimmValid;
  UINT8       DeviceType;
  MrcProfile  MemProfile;
  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0  *Header5;
  UINT16              XmpId;
  SPD_REVISION_STRUCT XmpRevision;
#ifdef MRC_DEBUG_PRINT
  static const UINT16 BytesUsedConst[] = {0, 128, 176, 256};
  MrcDebug            *Debug;
  const char          *DramTypeString;
  const char          *ModuleTypeString;
  const char          *ProfileStr;
  SPD_REVISION_STRUCT Revision;
  UINT16              BytesUsed;
  UINT16              BytesTotal;
  UINT16              CrcCoverage;
  UINT16              Byte;

  Debug = &MrcData->Outputs.Debug;
  ModuleTypeString = UnknownString;
#endif // MRC_DEBUG_PRINT

  Outputs    = &MrcData->Outputs;
  DimmValid  = TRUE;
  IsLpddr    = FALSE;
  MemProfile = MrcData->Inputs.ExtInputs.Ptr->MemoryProfile;
  DimmOut->XmpSupport = 0;

  //
  // The dram device type is at the same SPD offset for all types.
  //
  DeviceType = Spd->Ddr5.Base.DramDeviceType.Bits.Type;
  Header5 = NULL;
  XmpId = 0;
  XmpRevision.Data = 0;
  switch (DeviceType) {
    case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
      DimmOut->DdrType    = MRC_DDR_TYPE_DDR5;
      DimmOut->ModuleType = Spd->Ddr5.Base.ModuleType.Bits.ModuleType;
      Outputs->Vdd2Mv     = 1100;
      Header5    = &Spd->Ddr5.EndUser.Xmp.Header;
      XmpId = Header5->XmpId;
      if ((XMP_ID_STRING != Header5->XmpId) ||
          ((MemProfile == XMP_PROFILE1) && (Header5->XmpOrg.Bits.ProfileEnable1 == 0)) ||
          ((MemProfile == XMP_PROFILE2) && (Header5->XmpOrg.Bits.ProfileEnable2 == 0)) ||
          ((MemProfile == XMP_PROFILE3) && (Header5->XmpOrg.Bits.ProfileEnable3 == 0))) {
        if (IsXmp (MemProfile)) {
          DimmValid = FALSE;
        }
      }
      if (XMP_ID_STRING == XmpId) {
        XmpRevision = Header5->XmpRevision;
        if (!Xmp3VerifyDimmCrc (MrcData, Spd, DimmOut)) {
          break;
        }
        if (0x30 == XmpRevision.Data) {
          DimmOut->XmpRevision = XmpRevision.Data;
        }

        DimmOut->IsPmicSupport10MVStep = (Header5->PmicCapabilities.Bits.OCCapable == 1);
        if (DimmOut->IsPmicSupport10MVStep && (Header5->PmicCapabilities.Bits.DefaultStepSize == PMIC_10MVSTEP)) {
          DimmOut->PmicDefaultStepSize = 10;
        } else{
          DimmOut->PmicDefaultStepSize = 5;
        }

        if (Header5->XmpOrg.Bits.ProfileEnable1 != 0) {
          DimmOut->XmpSupport |= XMP_PROFILE1_SUPPORT_MASK;
          Outputs->XmpProfileEnable |= XMP_PROFILE1_SUPPORT_MASK;
          DimmOut->XmpProfile1Config = Header5->XmpConf.Bits.ProfileConfig1;
          /*
           * Save AdvancedMemoryOCFeat for later detailed
           * checking when BIOS enables specific feature
           */
          DimmOut->XmpOverClockFeat[0] = Spd->Ddr5.EndUser.Xmp.Data[0].AdvancedMemoryOCFeat.Data;
        }
        if (Header5->XmpOrg.Bits.ProfileEnable2 != 0) {
          DimmOut->XmpSupport |= XMP_PROFILE2_SUPPORT_MASK;
          Outputs->XmpProfileEnable |= XMP_PROFILE2_SUPPORT_MASK;
          DimmOut->XmpProfile2Config = Header5->XmpConf.Bits.ProfileConfig2;
          DimmOut->XmpOverClockFeat[1] = Spd->Ddr5.EndUser.Xmp.Data[1].AdvancedMemoryOCFeat.Data;
        }
        if (Header5->XmpOrg.Bits.ProfileEnable3 != 0) {
          DimmOut->XmpSupport |= XMP_PROFILE3_SUPPORT_MASK;
          Outputs->XmpProfileEnable |= XMP_PROFILE3_SUPPORT_MASK;
          DimmOut->XmpProfile3Config = Header5->XmpConf.Bits.ProfileConfig3;
          DimmOut->XmpOverClockFeat[2] = Spd->Ddr5.EndUser.Xmp.Data[2].AdvancedMemoryOCFeat.Data;
        }

        if (DimmOut->XmpSupport & XMP_PROFILE1_SUPPORT_MASK) {
          /*
           * XMP3.0 has no Enable bit for USER_PROFILE4/5
           * We assume:
           * 1) Only when XMP_PROFILE1 is enabled, USER_PROFILE4/5 can be enabled.
           * 2) Check (tCKAVGmin != 0) as a temporary solution
           */
          if (Spd->Ddr5.EndUser.Xmp.Data[USER_PROFILE4 - XMP_PROFILE1].tCKAVGmin.Data != 0) {
            DimmOut->XmpSupport |= XMP_USER_PROFILE4_SUPPORT_MASK;
            Outputs->XmpProfileEnable |= XMP_USER_PROFILE4_SUPPORT_MASK;
          }
          if (Spd->Ddr5.EndUser.Xmp.Data[USER_PROFILE5 - XMP_PROFILE1].tCKAVGmin.Data != 0) {
            DimmOut->XmpSupport |= XMP_USER_PROFILE5_SUPPORT_MASK;
            Outputs->XmpProfileEnable |= XMP_USER_PROFILE5_SUPPORT_MASK;
          }
        }
      }
      break;

    case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
      DimmOut->DdrType    = MRC_DDR_TYPE_LPDDR5;
      DimmOut->ModuleType = Spd->Lpddr.Base.ModuleType.Bits.ModuleType;
      IsLpddr             = TRUE;
      Outputs->Vdd2Mv     = 1050;

      if (DeviceType == MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER) {
        // Set LpX flag to handle small differences between LPDDR5 and LPDDR5X
        Outputs->LpX = TRUE;
      }
      break;

    default:
      DimmOut->DdrType    = MRC_DDR_TYPE_UNKNOWN;
      DimmOut->ModuleType = 0;
      DimmValid           = FALSE;
      break;
  }

  if (DimmValid) {
    switch (DimmOut->ModuleType) {
#if (SUPPORT_RDIMM == SUPPORT)
      case RDimmMemoryPackage:
#ifdef MRC_DEBUG_PRINT
        ModuleTypeString = RdimmString;
#endif // MRC_DEBUG_PRINT
        break;
#endif

#if (SUPPORT_UDIMM == SUPPORT)
      case UDimmMemoryPackage:
#ifdef MRC_DEBUG_PRINT
        ModuleTypeString = UdimmString;
#endif // MRC_DEBUG_PRINT
        break;
#endif

#if (SUPPORT_SODIMM == SUPPORT)
      case SoDimmMemoryPackage:
        Outputs->IsSoDimm = TRUE;
#ifdef MRC_DEBUG_PRINT
        ModuleTypeString = SodimmString;
#endif // MRC_DEBUG_PRINT
        break;
#endif // SUPPORT_SODIMM

      case LpDimmMemoryPackage:
        if (IsLpddr) {
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = LpDimmString;
#endif // MRC_DEBUG_PRINT
        } else {
          DimmValid = FALSE;
        }
        break;

      case NonDimmMemoryPackage:
        if (IsLpddr) {
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = MemoryDownString;
#endif // MRC_DEBUG_PRINT
        } else {
          DimmValid = FALSE;
        }
        break;
      case CammMemoryPackage:
        if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = Ddr5CammString;
#endif // MRC_DEBUG_PRINT
        } else if (IsLpddr) {
          Outputs->IsLP5Camm2 = TRUE;
#ifdef MRC_DEBUG_PRINT
          ModuleTypeString = Lpddr5CammString;
#endif // MRC_DEBUG_PRINT
        } else {
          DimmValid = FALSE;
        }
        break;

#if (SUPPORT_UDIMM == SUPPORT)
      case CuDimmMemoryPackage:
#ifdef MRC_DEBUG_PRINT
        ModuleTypeString = CudimmString;
#endif // MRC_DEBUG_PRINT
        break;
#endif // SUPPORT_UDIMM

#if (SUPPORT_SODIMM == SUPPORT)
      case CsoDimmMemoryPackage:
      Outputs->IsSoDimm = TRUE;
#ifdef MRC_DEBUG_PRINT
        ModuleTypeString = CsodimmString;
#endif // MRC_DEBUG_PRINT
        break;
#endif // SUPPORT_SODIMM

      default:
        DimmValid = FALSE;
        break;
    }
  }

#ifdef MRC_DEBUG_PRINT
  switch (MemProfile) {
      case STD_PROFILE:
      default:
        ProfileStr = StdString;
        break;
      case CUSTOM_PROFILE1:
        ProfileStr = CustomString;
        break;
      case USER_PROFILE4:
        ProfileStr = User4String;
        break;
      case USER_PROFILE5:
        ProfileStr = User5String;
        break;
      case XMP_PROFILE1:
        ProfileStr = Xmp1String;
        break;
      case XMP_PROFILE2:
        ProfileStr = Xmp2String;
        break;
      case XMP_PROFILE3:
        ProfileStr = Xmp3String;
        break;
  }

  switch (DeviceType) {
    case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
      DramTypeString = Ddr5String;
      BytesTotal     = 256 * Spd->Ddr5.Base.Description.Bits.BytesTotal;
      BytesUsed      = 0; // DDR5 SPD Reserved Field
      CrcCoverage    = 125;
      Revision.Data  = Spd->Ddr5.Base.Revision.Data;
      break;

    case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
    case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
      DramTypeString = (DeviceType == MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER) ? Lpddr5XString : Lpddr5String;
      BytesTotal     = 256 * Spd->Lpddr.Base.Description.Bits.BytesTotal;
      BytesUsed      = BytesUsedConst[1] * Spd->Lpddr.Base.Description.Bits.BytesUsed;
      CrcCoverage    = 125;
      Revision.Data  = Spd->Lpddr.Base.Revision.Data;
      break;

    default:
      DramTypeString = UnknownString;
      BytesTotal     = 0;
      BytesUsed      = 0;
      CrcCoverage    = 0;
      Revision.Data  = 0;
      break;
  }

  if (DimmValid) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  %s %s detected, Rev: %u.%u, Size: %u used/%u total, CRC coverage: 0 - %u\n",
      DramTypeString,
      ModuleTypeString,
      Revision.Bits.Major,
      Revision.Bits.Minor,
      BytesUsed,
      BytesTotal,
      CrcCoverage
      );
  } else {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "  %s %s detected, SPD Dram type %Xh, module type %Xh\n",
      DramTypeString,
      ModuleTypeString,
      DeviceType,
      DimmOut->ModuleType
      );
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  DIMM profile %s selected\n", ProfileStr);
  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "  XMP String: %Xh\n",
    XmpId
  );
  if (DimmOut->XmpSupport) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  XMP Revision: %u.%u\n",
      XmpRevision.Bits.Major,
      XmpRevision.Bits.Minor
    );
    if (!IsLpddr) {
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        XpString,
        1,
        Header5->XmpOrg.Bits.ProfileEnable1 ? "en" : "dis",
        Header5->XmpConf.Bits.ProfileConfig1 + 1
      );
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        XpString,
        2,
        Header5->XmpOrg.Bits.ProfileEnable2 ? "en" : "dis",
        Header5->XmpConf.Bits.ProfileConfig2 + 1
      );
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_NOTE,
        XpString,
        3,
        Header5->XmpOrg.Bits.ProfileEnable3 ? "en" : "dis",
        Header5->XmpConf.Bits.ProfileConfig3 + 1
      );

      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, XmpNameString, 1);
      for (Byte = 0; Byte < 16; Byte++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %02X", Header5->Profile1Name[Byte]);
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, XmpNameString, 2);
      for (Byte = 0; Byte < 16; Byte++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %02X", Header5->Profile2Name[Byte]);
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, XmpNameString, 3);
      for (Byte = 0; Byte < 16; Byte++) {
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %02X", Header5->Profile3Name[Byte]);
      }
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
    }
  }
  #endif // MRC_DEBUG_PRINT

  return DimmValid;
}

/**
  Determine if the DIMM SDRAM device width is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
BOOLEAN
ValidSdramDeviceWidth (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug    *Debug;
  MrcOutput   *Outputs;
  MrcDdrType  DdrType;
  BOOLEAN     IsLpddr;
  UINT8       ChPerSdramPkg;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  DdrType = DimmOut->DdrType;
  IsLpddr = (Outputs->IsLpddr);
  ChPerSdramPkg = MRC_SPD_CH_PER_SDRAM_PKG_RSVD;

  if (MRC_DDR_TYPE_DDR5 == DdrType) {
    DimmOut->SdramWidthIndex = Spd->Ddr5.Base.PrimarySdramIoWidth.Bits.SdramIoWidth;
  } else {
    DimmOut->SdramWidthIndex = Spd->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
    DimmOut->DiePerSdramPackage = Spd->Lpddr.Base.SdramPackageType.Bits.DiePerSdramPkg + 1;
    if (!Outputs->IsLP5Camm2) {
      ChPerSdramPkg = Spd->Lpddr.Base.SdramPackageType.Bits.ChannelsPerPkg;
    }
  }

  switch (DimmOut->SdramWidthIndex) {

#if (SUPPORT_DEVWIDTH_4 == SUPPORT)
    case MRC_SPD_SDRAM_DEVICE_WIDTH_4:
      DimmOut->SdramWidth = 4;
      break;
#endif
#if (SUPPORT_DEVWIDTH_8 == SUPPORT)
    case MRC_SPD_SDRAM_DEVICE_WIDTH_8:
      DimmOut->SdramWidth = 8;
      if (IsLpddr) {
        Outputs->LpByteMode = TRUE;
      }
      break;
#endif
#if (SUPPORT_DEVWIDTH_16 == SUPPORT)
    case MRC_SPD_SDRAM_DEVICE_WIDTH_16:
      DimmOut->SdramWidth = 16;
      break;
#endif
#if (SUPPORT_DEVWIDTH_32 == SUPPORT)
    case MRC_SPD_SDRAM_DEVICE_WIDTH_32:
      DimmOut->SdramWidth = 32;
      break;
#endif

    default:
      DimmOut->SdramWidth = 0;
      MRC_DEBUG_MSG (Debug,
        MSG_LEVEL_ERROR,
        "%sSDRAM device width, %s%Xh\n",
        ErrorString,
        SpdValString,
        DimmOut->SdramWidthIndex
        );
      return FALSE;
  }

  if (IsLpddr && !Outputs->IsLP5Camm2) {
    switch (ChPerSdramPkg) {
      case MRC_SPD_CH_PER_SDRAM_PKG_1:
        ChPerSdramPkg = 1;
        break;

      case MRC_SPD_CH_PER_SDRAM_PKG_2:
        ChPerSdramPkg = 2;
        break;

      case MRC_SPD_CH_PER_SDRAM_PKG_4:
        ChPerSdramPkg = 4;
        break;

      default:
      case MRC_SPD_CH_PER_SDRAM_PKG_RSVD:
        ChPerSdramPkg = 0;
        MRC_DEBUG_MSG (
          Debug,
          MSG_LEVEL_ERROR,
          "%s%s, %s%Xh\n",
          ErrorString,
          ChPerSdramPkgStr,
          SpdValString,
          ChPerSdramPkg
          );
        return FALSE;
    }
    DimmOut->ChannelsPerSdramPackage = ChPerSdramPkg;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Die per SDRAM Package: %u\n", DimmOut->DiePerSdramPackage);
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s: %u\n", ChPerSdramPkgStr, ChPerSdramPkg);
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  SDRAM device width: %u\n", DimmOut->SdramWidth);

  return TRUE;
}

/**
  Determine if the DIMM SDRAM row address size is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE if the row address size is valid, otherwise FALSE and the value is set to zero.
**/
BOOLEAN
ValidRowSize (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  UINT8        RowBits;
  UINT8        RowAddress;
  MrcDebug        *Debug;

  Debug = &MrcData->Outputs.Debug;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    RowAddress = Spd->Ddr5.Base.PrimarySdramAddressing.Bits.RowAddress;
  } else {
    RowAddress = Spd->Lpddr.Base.SdramAddressing.Bits.RowAddress;
  }

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (RowAddress) {
#if (SUPPORT_ROW_16 == SUPPORT)
      case MRC_SPD5_SDRAM_ROW_16:
        DimmOut->RowSize = MRC_BIT16;
        RowBits          = 16;
        break;
#endif
#if (SUPPORT_ROW_17 == SUPPORT)
      case MRC_SPD5_SDRAM_ROW_17:
        DimmOut->RowSize = MRC_BIT17;
        RowBits          = 17;
        break;
#endif
#if (SUPPORT_ROW_18 == SUPPORT)
      case MRC_SPD5_SDRAM_ROW_18:
        DimmOut->RowSize = MRC_BIT18;
        RowBits          = 18;
        break;
#endif
      default:
        DimmOut->RowSize = 0;
        RowBits          = 0;
        break;
    }
  } else {
    switch (RowAddress) {
#if (SUPPORT_ROW_12 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_12:
        DimmOut->RowSize = MRC_BIT12;
        RowBits          = 12;
        break;
#endif
#if (SUPPORT_ROW_13 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_13:
        DimmOut->RowSize = MRC_BIT13;
        RowBits          = 13;
        break;
#endif
#if (SUPPORT_ROW_14 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_14:
        DimmOut->RowSize = MRC_BIT14;
        RowBits          = 14;
        break;
#endif
#if (SUPPORT_ROW_15 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_15:
        DimmOut->RowSize = MRC_BIT15;
        RowBits          = 15;
        break;
#endif
#if (SUPPORT_ROW_16 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_16:
        DimmOut->RowSize = MRC_BIT16;
        RowBits          = 16;
        break;
#endif
#if (SUPPORT_ROW_17 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_17:
        DimmOut->RowSize = MRC_BIT17;
        RowBits          = 17;
        break;
#endif
#if (SUPPORT_ROW_18 == SUPPORT)
      case MRC_SPD_SDRAM_ROW_18:
        DimmOut->RowSize = MRC_BIT18;
        RowBits          = 18;
        break;
#endif
      default:
        DimmOut->RowSize = 0;
        RowBits          = 0;
        break;
    }
  }

  if (DimmOut->RowSize == 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%sSDRAM row size, %s%Xh\n", ErrorString, SpdValString, RowAddress);
    return FALSE;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Row bits: %u\n", RowBits);
  return TRUE;
}

/**
  Determine if the DIMM SDRAM column address size is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE if the column address size is valid, otherwise FALSE and the value is set to zero.
**/
BOOLEAN
ValidColumnSize (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug     *Debug;
  UINT8        ColumnBits;
  UINT8        ColumnAddress;

  Debug    = &MrcData->Outputs.Debug;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    ColumnAddress = Spd->Ddr5.Base.PrimarySdramAddressing.Bits.ColumnAddress;
  } else {
    ColumnAddress = Spd->Lpddr.Base.SdramAddressing.Bits.ColumnAddress;
  }

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (ColumnAddress) {
      case MRC_SPD5_SDRAM_COLUMN_10:
        ColumnBits = 10;
        break;
      case MRC_SPD5_SDRAM_COLUMN_11:
        ColumnBits = 11;
        break;
      default:
        ColumnBits = 0;
        break;
    }
  } else {
    switch (ColumnAddress) {
      case MRC_SPD_SDRAM_COLUMN_9:
        ColumnBits = 9;
        break;
      case MRC_SPD_SDRAM_COLUMN_10:
        ColumnBits = 10;
        break;
      case MRC_SPD_SDRAM_COLUMN_11:
        ColumnBits = 11;
        break;
      case MRC_SPD_SDRAM_COLUMN_12:
        ColumnBits = 12;
        break;
      default:
        ColumnBits = 0;
        break;
    }
  }

  if (ColumnBits == 0) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%sSDRAM column size, %s%Xh\n", ErrorString, SpdValString, ColumnAddress);
    DimmOut->ColumnSize = 0;
    return FALSE;
  }

  DimmOut->ColumnSize = 1 << ColumnBits;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Column bits: %u\n", ColumnBits);
  return TRUE;
}

/**
  Determine if the DIMM SDRAM primary bus width is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
BOOLEAN
ValidPrimaryWidth (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug  *Debug;
  MrcOutput *Outputs;
  UINT8     Width;
  UINT8     NumberOfChannels;
  BOOLEAN   IsLpddr;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  IsLpddr = FALSE;
  NumberOfChannels = 0;

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    Width = Spd->Ddr5.ModuleCommon.ModuleMemoryBusWidth.Bits.PrimaryBusWidth;
  } else if (Outputs->IsLP5Camm2) {
    Width = MRC_SPD_PRIMARY_BUS_WIDTH_16;//Hardcoded
    //translate from SPD Raw values to encoding
    switch (Spd->JedecLpddr5.ModuleCommon.ModuleMemoryBusWidth.Bits.NumOfSubChannelsPerDimm) {
      case 0:
        NumberOfChannels = 1;
        break;

      case 1:
        NumberOfChannels = 2;
        break;

      case 2:
        NumberOfChannels = 4;
        break;

      case 3:
        NumberOfChannels = 8;
        break;

      default:
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "LP CAMM2 NumOfSubchannelsPerDimm %s%d\n", SpdValString, Spd->JedecLpddr5.ModuleCommon.ModuleMemoryBusWidth.Bits.NumOfSubChannelsPerDimm);
        return FALSE;
        break;
    }
    if (Width == MRC_SPD_PRIMARY_BUS_WIDTH_16) {
      Outputs->EnhancedChannelMode = TRUE;
    }
    IsLpddr = TRUE;
  } else {  // LPDDR5
    IsLpddr = TRUE;
    NumberOfChannels = 1;//Hardcoded
    Width = MRC_SPD_PRIMARY_BUS_WIDTH_16;//Hardcoded
    // We will check for mixed width case later in MrcSpdProcessing()
    if (Width == MRC_SPD_PRIMARY_BUS_WIDTH_16) {
      Outputs->EnhancedChannelMode = TRUE;
    }
  }

  switch (Width) {
#if (SUPPORT_PRIWIDTH_8 == SUPPORT)
    case MRC_SPD_PRIMARY_BUS_WIDTH_8:
      DimmOut->PrimaryBusWidth = 8;
      break;
#endif
#if (SUPPORT_PRIWIDTH_16 == SUPPORT)
    case MRC_SPD_PRIMARY_BUS_WIDTH_16:
      DimmOut->PrimaryBusWidth = 16;
      break;
#endif
#if (SUPPORT_PRIWIDTH_32 == SUPPORT)
    case MRC_SPD_PRIMARY_BUS_WIDTH_32:
      DimmOut->PrimaryBusWidth = 32;
      break;
#endif
#if (SUPPORT_PRIWIDTH_64 == SUPPORT)
    case MRC_SPD_PRIMARY_BUS_WIDTH_64:
      DimmOut->PrimaryBusWidth = 64;
      break;
#endif
    default:
      DimmOut->PrimaryBusWidth = 0;
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%sSDRAM primary bus width, %s%Xh\n", ErrorString, SpdValString, Width);
      return FALSE;
      break;
  }
  if (IsLpddr) {
    DimmOut->NumLpSysChannel = NumberOfChannels;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Number of channels: %u\n", NumberOfChannels);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Primary bus width: %u\n", DimmOut->PrimaryBusWidth);
  return TRUE;
}

/**
  Converts the input DDR5 SPD Byte 4 Bits 4:0 Density value to a DensityIndex
  value that can be used to index into the SdramCapacityTable array.

    @param[in] Dddr5Density - The DDR5 SDRAM Density Per Die per the DDR5 SPD spec

    @return The density as an index of SdramCapacityTable or 0xFF if the input is undefined.
**/
UINT8
Ddr5SpdToDensityIndex (
  IN UINT8 Ddr5Density
  )
{
  UINT8 DensityIndex;
  switch (Ddr5Density) {
    case MRC_SPD5_SDRAM_DENSITY_4Gb:
      DensityIndex = MrcDensity4Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_8Gb:
      DensityIndex = MrcDensity8Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_12Gb:
      DensityIndex = MrcDensity12Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_16Gb:
      DensityIndex = MrcDensity16Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_24Gb:
      DensityIndex = MrcDensity24Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_32Gb:
      DensityIndex = MrcDensity32Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_48Gb:
      DensityIndex = MrcDensity48Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_64Gb:
      DensityIndex = MrcDensity64Gb;
      break;
    case MRC_SPD5_SDRAM_DENSITY_NONE:
    default:
      DensityIndex = 0xFF;
      break;
  }
  return DensityIndex;
}

/**
  Checks if current DRAM density index is valid for current technology/project. The index check follows the 
  SdramCapacityTable defined by JEDEC spec.

    @param[in] MrcDimmOut - Pointer to structure containing DIMM information.

    @return TRUE if valid density for DDR technology, otherwise FALSE
**/
BOOLEAN
ValidDramDensity (
  IN MrcDimmOut     *const DimmOut
)
{
  BOOLEAN Valid;

  Valid = FALSE;

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (DimmOut->DensityIndex) {
      case MrcDensity16Gb:
      case MrcDensity24Gb:
      case MrcDensity32Gb:
        Valid = TRUE;
        break;
      default:
        break;
    }
  } else {
    switch (DimmOut->DensityIndex) {
      case MrcDensity8Gb:
      case MrcDensity12Gb:
      case MrcDensity16Gb:
      case MrcDensity24Gb:
      case MrcDensity32Gb:
        Valid = TRUE;
        break;
      default:
        break;
    }
  }

  return Valid;
}

/**
  Determines if the number of Bank are valid.
  Determines if the number of Bank Groups are valid.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE.
**/
BOOLEAN
ValidBank (
  IN OUT MrcParameters  *const MrcData,
  IN const MrcSpd       *const Spd,
  IN OUT MrcDimmOut     *const DimmOut
  )
{
  MrcDebug     *Debug;
  UINT8        BankAddress;
  UINT8        BankGroup;
  UINT8        ValidCheck;
  UINT8        ValidCheckStep;
  UINT8        SpdDensity; 

  Debug       = &MrcData->Outputs.Debug;
  ValidCheck  = TRUE;

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    SpdDensity            = Spd->Ddr5.Base.PrimarySdramDensityAndPackage.Bits.Density;
    DimmOut->DensityIndex = Ddr5SpdToDensityIndex (SpdDensity);
    BankAddress           = Spd->Ddr5.Base.PrimarySdramBankGroups.Bits.BanksPerBankGroup;
    BankGroup             = Spd->Ddr5.Base.PrimarySdramBankGroups.Bits.BankGroups;
  } else {
    SpdDensity            = Spd->Lpddr.Base.SdramDensityAndBanks.Bits.Density;
    DimmOut->DensityIndex = SpdDensity;
    BankAddress           = Spd->Lpddr.Base.SdramDensityAndBanks.Bits.BankAddress;
    BankGroup             = Spd->Lpddr.Base.SdramDensityAndBanks.Bits.BankGroup;
  }

  if (!ValidDramDensity (DimmOut)) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "%sDRAM Density, %s%Xh, Index: %u\n",
      ErrorString,
      SpdValString,
      SpdDensity,
      DimmOut->DensityIndex
      );
    ValidCheck = FALSE;
  }

  // Get the DRAM density in Gbit
  // Less than 1Gb density is not POR, smaller values would be invalid and reported as 0
  if (DimmOut->DensityIndex < ARRAY_COUNT (SdramCapacityTable)) {
    DimmOut->DeviceDensity = (UINT8) ((8 * SdramCapacityTable[DimmOut->DensityIndex]) / 1024);
  }

  ValidCheckStep = TRUE;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (BankAddress) {
      case MRC_SPD_DDR5_SDRAM_BANK_1:
      case MRC_SPD_DDR5_SDRAM_BANK_2:
      case MRC_SPD_DDR5_SDRAM_BANK_4:
        DimmOut->Banks = MRC_BIT0 << BankAddress;
        break;
      default:
        ValidCheckStep = FALSE;
    }
  } else {
    switch (BankAddress) {
#if (SUPPORT_BANK_4 == SUPPORT)
      case MRC_SPD_LPDDR_SDRAM_BANK_4:
#endif
#if (SUPPORT_BANK_8 == SUPPORT)
      case MRC_SPD_LPDDR_SDRAM_BANK_8:
#endif
#if ((SUPPORT_BANK_4 == SUPPORT) || (SUPPORT_BANK_8 == SUPPORT))
        DimmOut->Banks = MRC_BIT2 << BankAddress;
        break;
#endif

      default:
        ValidCheckStep = FALSE;
    }
  }

  if (ValidCheckStep == FALSE) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "%sSDRAM number of banks, %s%Xh\n",
      ErrorString,
      SpdValString,
      BankAddress
      );
      ValidCheck = FALSE;
  }

  ValidCheckStep = TRUE;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    switch (BankGroup) {
      case MRC_SPD_DDR5_SDRAM_BANK_GROUPS_1:
      case MRC_SPD_DDR5_SDRAM_BANK_GROUPS_2:
      case MRC_SPD_DDR5_SDRAM_BANK_GROUPS_4:
      case MRC_SPD_DDR5_SDRAM_BANK_GROUPS_8:
        DimmOut->BankGroups = MRC_BIT0 << BankGroup;
        break;
      default:
        ValidCheckStep = FALSE;
    }
  } else {
    DimmOut->BankGroups = MRC_BIT0 << BankGroup;
  }

  if (ValidCheckStep == FALSE) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "%sSDRAM number of bank groups, %s%Xh\n",
      ErrorString,
      SpdValString,
      BankGroup
      );
    ValidCheck = FALSE;
  }

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    (TRUE == ValidCheck) ? "  %u Banks in %u groups\n" : "",
    DimmOut->Banks,
    DimmOut->BankGroups
    );

  if (DimmOut->DensityIndex < ARRAY_COUNT (SdramCapacityTable)) {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  SDRAM Capacity: %u Mb\n",
      SdramCapacityTable[DimmOut->DensityIndex] * 8
      );
  } else {
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_NOTE,
      "  ERROR: SDRAM Capacity is reserved value: 0x%02X\n",
      DimmOut->DensityIndex
      );
  }

  return ValidCheck;
}

/**
  Determine if the number of ranks in the DIMM is valid and return the value.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
BOOLEAN
GetRankCount (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug     *Debug;
  MrcOutput    *Outputs;
  UINT8        RankCount;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    RankCount = Spd->Ddr5.ModuleCommon.ModuleOrganization.Bits.RankCount;
  } else if (Outputs->IsLP5Camm2) {
    RankCount = Spd->JedecLpddr5.ModuleCommon.ModuleOrganization.Bits.NumOfPackageRanksperSubChannel;
  } else {
    RankCount = Spd->Lpddr.Base.ModuleOrganization.Bits.RankCount;
  }

  DimmOut->RankInDimm = RankCount + 1;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Ranks: %u\n", DimmOut->RankInDimm);

  if (MrcData->Inputs.ExtInputs.Ptr->ForceSingleRank) {
    DimmOut->RankInDimm = 1;
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Forcing to Single Rank because ForceSingleRank is 1\n" );
  }

  if (DimmOut->RankInDimm > MAX_RANK_IN_DIMM) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%snumber of ranks, %s%Xh\n", ErrorString, SpdValString, RankCount);
    DimmOut->RankInDimm = 0;
    return FALSE;
  }

  return TRUE;
}

/**
  Transcode SPD Raw Value to Actual Value for SdramDensityPerDie. Used by JEDEC Spec LP CAMM (CAMM2). Units are in Gigabytes.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in, out] SdramDensityPerDie - Die Density from the SPD data.

    @retval Size of each subchannel in MBytes.
**/
UINT8
TranscodeJedecSpecLPCammSdramDensityPerDie (
  IN OUT MrcParameters* const MrcData,
  IN UINT8 const SdramDensityPerDie
  ) 
{
  MrcDebug   *Debug;
  Debug = &MrcData->Outputs.Debug;
  switch (SdramDensityPerDie) {
    case 2:  
      return 1;

    case 3:
      return 2;

    case 4:
      return 4;

    case 5:
      return 8;

    case 6:
      return 16;

    case 7:
      return 32;

    case 8:
      return 12;

    case 9:
      return 24;

    case 10:
      return 3;

    case 11:
      return 6;

    default:
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "Invalid LP CAMM2 SdramDensityPerDie %s%d\n", SpdValString, SdramDensityPerDie);
      return MRC_UINT8_MAX;
  }
}

/**
  Calculate the size of JEDEC Spec Compliant CAMM. Size is module size per sub-channel in MegaBytes

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Size of each subchannel in MBytes.
**/
UINT32
GetJedecSpecLPCammSize (
  IN OUT MrcParameters* const MrcData,
  IN OUT MrcDimmOut* const DimmOut
  ) 
{
  UINT16     SdramDensityPerDie;
  UINT32     TotalSizeMBytes;

  TotalSizeMBytes = 0;

  SdramDensityPerDie = TranscodeJedecSpecLPCammSdramDensityPerDie (MrcData, DimmOut->DensityIndex);
  SdramDensityPerDie *= 1024;//Scale to not lose precision on non power of two values for SdramDensityPerDie.
 
  //The following is straight from the 1.0 JEDEC LP CAMM2 Spec for total size:
  //  DimmSize (GB) = NumOfSubchannelsPerDimm * (PrimaryBusWidthPerSubCh / SdramDieDataWidth) * (SdramDensityPerDie / 8) * PackageRanksPerSubCh
  TotalSizeMBytes = (DimmOut->PrimaryBusWidth / DimmOut->SdramWidth) * (SdramDensityPerDie / 8) * DimmOut->RankInDimm;
  
  return TotalSizeMBytes;
}

/**
  Calculate the size of the DIMM, in MBytes.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE on valid value, otherwise FALSE and the value is set to zero.
**/
BOOLEAN
GetDimmSize (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MRC_EXT_INPUTS_TYPE* ExtInputs;
  MrcInput   *Inputs;
  MrcOutput  *Outputs;
  MrcDebug   *Debug;
  UINT32     DimmSize;
  UINT8      DensityIndex;
  BOOLEAN    IsLpddr;

  Inputs = &MrcData->Inputs;
  ExtInputs = Inputs->ExtInputs.Ptr;

  Outputs      = &MrcData->Outputs;
  Debug        = &Outputs->Debug;
  DensityIndex = DimmOut->DensityIndex;
  IsLpddr      = (Outputs->IsLpddr);

  if ((DimmOut->SdramWidth > 0) && (DensityIndex < ARRAY_COUNT (SdramCapacityTable))) {
    if (Outputs->IsLP5Camm2) {
      DimmSize = GetJedecSpecLPCammSize (MrcData, DimmOut);
    } else if (IsLpddr) {
      DimmSize = (SdramCapacityTable[DensityIndex] * DimmOut->DiePerSdramPackage * DimmOut->PrimaryBusWidth * DimmOut->NumLpSysChannel) / (DimmOut->SdramWidth * DimmOut->ChannelsPerSdramPackage);
      if (ExtInputs->ForceSingleRank && (Spd->Lpddr.Base.ModuleOrganization.Bits.RankCount == 1)) {
        DimmSize = DimmSize / 2;
      }
    } else {
      DimmSize = (((SdramCapacityTable[DensityIndex] * DimmOut->PrimaryBusWidth) / DimmOut->SdramWidth) * DimmOut->RankInDimm);
    }
    if ((DimmSize >= DIMMSIZEMIN) && (DimmSize <= DIMMSIZEMAX)) {
      if (DimmSize != 0) {
        DimmOut->DimmCapacity = DimmSize;
        MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  DIMM size: %u MByte\n", DimmSize);
        return TRUE;
      }
    }
    MRC_DEBUG_MSG (
      Debug,
      MSG_LEVEL_ERROR,
      "%sDIMM size: %u MB, valid range %u - %u MB ",
      ErrorString,
      DimmSize,
      DIMMSIZEMIN,
      DIMMSIZEMAX
      );
  }

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_ERROR,
    "SDRAM capacity %s%Xh\n",
    SpdValString,
    DensityIndex
    );
  DimmOut->DimmCapacity = 0;
  return FALSE;
}

/**
  Obtain ECC support Status for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
BOOLEAN
ValidEccSupport (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
#if (SUPPORT_ECC == SUPPORT)
  UINT8        BusWidthExtension;
#endif // SUPPORT_ECC
  MrcOutput       *Outputs;
  MrcDebug        *Debug;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;

#if (SUPPORT_ECC == SUPPORT)
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    BusWidthExtension = Spd->Ddr5.ModuleCommon.ModuleMemoryBusWidth.Bits.BusWidthExtension;
  } else if (Outputs->IsLP5Camm2) {
    BusWidthExtension = Spd->JedecLpddr5.ModuleCommon.ModuleMemoryBusWidth.Bits.BusWidthExtensionPerSubChannel;
  } else {
    BusWidthExtension = Spd->Lpddr.Base.ModuleMemoryBusWidth.Bits.BusWidthExtension;
  }
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType &&
    ((MRC_SPD5_BUS_WIDTH_EXTENSION_8 == BusWidthExtension) || (MRC_SPD5_BUS_WIDTH_EXTENSION_4 == BusWidthExtension))) {
    DimmOut->EccSupport = TRUE;
  } else if (MRC_SPD_BUS_WIDTH_EXTENSION_8 == BusWidthExtension) {
    DimmOut->EccSupport = TRUE;
  } else
#endif // SUPPORT_ECC
  {
    DimmOut->EccSupport = FALSE;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  ECC is %ssupported\n", (DimmOut->EccSupport == FALSE) ? "not " : "");
  return TRUE;
}

/**
  Obtain CKD support Status for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE Function completed.  This would be a void function but
    it appears in a table of functions that must return a BOOLEAN.
**/
static
BOOLEAN
ValidCkdSupport (
  IN OUT MrcParameters* const MrcData,
  IN const MrcSpd* const Spd,
  IN OUT MrcDimmOut* const DimmOut
)
{
  MrcDebug     *Debug;
  UINT8        DeviceInstalled;

  Debug = &MrcData->Outputs.Debug;

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
    DeviceInstalled = Spd->Ddr5.ModuleCommon.ModuleSpecific.Unbuffered.DeviceInfoRegister.DeviceType.Bits.DevicesInstalled;
    if (MRC_SPD5_DEVICE_INSTALLED == DeviceInstalled) {
      DimmOut->IsCkdSupport = TRUE;
    }  else {
      DimmOut->IsCkdSupport = FALSE;
    }
  } else {
    DimmOut->IsCkdSupport = FALSE;
  }

  if ((DimmOut->IsCkdSupport == TRUE) && MrcData->Inputs.ExtInputs.Ptr->ForceCkdBypass) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  CKD is supported but set to bypass\n");
    DimmOut->IsCkdSupport = FALSE;
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  CKD is %ssupported\n", (DimmOut->IsCkdSupport == FALSE) ? "not " : "");
  }
  return TRUE;
}

/**
  Obtain address mirroring Status for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
BOOLEAN
GetAddressMirror (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  // DDR5 address mirroring is automatic via the MIR pin strap, so it's transparent to MC/MRC
  // There is no address mirroring indication in DDR5 SPD
  DimmOut->AddressMirrored = FALSE;
  MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "  DIMM has no address mirroring\n");
  return TRUE;
}

/**
  Obtain thermal and refresh support for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
BOOLEAN
GetThermalRefreshSupport (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug        *Debug;
  const MrcInput  *Inputs;
  MrcOutput       *Outputs;

  Inputs = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug  = &Outputs->Debug;

  DimmOut->PartialSelfRefresh    = 0;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType || Outputs->IsLP5Camm2) {
    DimmOut->OnDieThermalSensor = Spd->Ddr5.ModuleCommon.DeviceInfoThermalSensor.DeviceType.Bits.DevicesInstalledTS0 |
                                  Spd->Ddr5.ModuleCommon.DeviceInfoThermalSensor.DeviceType.Bits.DevicesInstalledTS1;
  } else {
    DimmOut->OnDieThermalSensor = Spd->Lpddr.Base.ModuleThermalSensor.Bits.ThermalSensorPresence;
  }
  DimmOut->AutoSelfRefresh       = 0;
  DimmOut->ExtendedTemperRefresh = 0;
  DimmOut->ExtendedTemperRange   = 0;

  DimmOut->SelfRefreshTemp = ((!DimmOut->AutoSelfRefresh) && (DimmOut->ExtendedTemperRange) && (Inputs->ExtInputs.Ptr->ExtTemperatureSupport)) ? TRUE : FALSE;

  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  Partial Array Self Refresh%s\n",
    DimmOut->PartialSelfRefresh ? IsSupString : NotSupString);
  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  On-Die Thermal Sensor Readout%s\n",
    DimmOut->OnDieThermalSensor ? IsSupString : NotSupString);
  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  Auto Self Refresh%s\n",
    DimmOut->AutoSelfRefresh ? IsSupString : NotSupString);
  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  Extended Temperature Refresh Rate%s\n",
    DimmOut->ExtendedTemperRefresh ? IsSupString : NotSupString);
  MRC_DEBUG_MSG (Debug,
    MSG_LEVEL_NOTE,
    "  Extended Temperature Range%s\n",
    DimmOut->ExtendedTemperRange ? IsSupString : NotSupString);
  return TRUE;
}

/**
  Obtain Pseudo TRR support for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
BOOLEAN
GetpTRRsupport (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcDebug        *Debug;
  MrcOutput       *Outputs;
  UINT32          tMAC;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType || Outputs->IsLP5Camm2) {
    tMAC = MRC_TMAC_UNTESTED;
  } else {
    tMAC = Spd->Lpddr.Base.pTRRsupport.Bits.tMACencoding;
  }

  switch (tMAC) {
    case MRC_TMAC_UNTESTED:
      DimmOut->tMAC = MRC_TMAC_UNTESTED;
      break;
    case MRC_TMAC_200K:
      DimmOut->tMAC = 2;
      break;
    case MRC_TMAC_300K:
      DimmOut->tMAC = 3;
      break;
    case MRC_TMAC_400K:
      DimmOut->tMAC = 4;
      break;
    case MRC_TMAC_500K:
      DimmOut->tMAC = 5;
      break;
    case MRC_TMAC_600K:
      DimmOut->tMAC = 6;
      break;
    case MRC_TMAC_700K:
      DimmOut->tMAC = 7;
      break;
    case MRC_TMAC_UNLIMITED:
      DimmOut->tMAC = MRC_TMAC_UNLIMITED;
      break;
    default:
      DimmOut->tMAC = 0;
      MRC_DEBUG_MSG (
        Debug,
        MSG_LEVEL_ERROR,
        "%stMAC value, %s%Xh\n",
        ErrorString,
        SpdValString,
        tMAC
        );
      return FALSE;
  }

  if (DimmOut->tMAC == MRC_TMAC_UNTESTED) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  tMAC is untested\n");
  } else if (DimmOut->tMAC == MRC_TMAC_UNLIMITED) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  tMAC is unlimited\n");
  } else {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  tMAC: %d00K\n", DimmOut->tMAC);
  }

  return TRUE;
}

/**
  Obtain which JEDEC reference design raw card was used as the basis for the DIMM assembly.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval Returns TRUE.
**/
BOOLEAN
GetReferenceRawCardSupport (
  IN OUT MrcParameters   *const MrcData,
  IN const MrcSpd        *const Spd,
  IN OUT MrcDimmOut      *const DimmOut
  )
{
  MrcOutput *Outputs;
  MrcDebug  *Debug;

  Outputs = &MrcData->Outputs;
  Debug = &Outputs->Debug;
  if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType || Outputs->IsLP5Camm2) {
    DimmOut->ReferenceRawCard = Spd->Ddr5.ModuleCommon.ReferenceRawCardUsed.Bits.Card;
  } else {
    DimmOut->ReferenceRawCard = (Spd->Lpddr.Module.LpDimm.ReferenceRawCardUsed.Bits.Extension << MRC_SPD_REF_RAW_CARD_SIZE) |
    Spd->Lpddr.Module.LpDimm.ReferenceRawCardUsed.Bits.Card;
  }

  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "  Reference raw card: %u '%s'\n",
    DimmOut->ReferenceRawCard,
    (DimmOut->ReferenceRawCard < (sizeof (RrcString) / sizeof (RrcString[0]))) ?
    RrcString[DimmOut->ReferenceRawCard] : UnknownString
  );
  return TRUE;
}

/**
  Calculate the CRC16 of the provided SPD data. CRC16 formula is the same
    one that is used for calculating the CRC16 stored at SPD bytes 126-127.
    This can be used to detect DIMM change.

    @param[in]  Buffer - Pointer to the start of the data.
    @param[in]  Size   - Amount of data in the buffer, in bytes.
    @param[out] Crc    - Pointer to location to write the calculated CRC16 value.

    @retval Returns TRUE.
**/
BOOLEAN
GetDimmCrc (
  IN  const UINT8 *const Buffer,
  IN  const UINT32       Size,
  OUT UINT16 *const      Crc
  )
{
  const UINT8 *Data;
  UINT32      Value;
  UINT32      Byte;
  UINT8       Bit;

  Data  = Buffer;
  Value = CRC_SEED;
  for (Byte = 0; Byte < Size; Byte++) {
    Value ^= (UINT32) *Data++ << 8;
    for (Bit = 0; Bit < 8; Bit++) {
      Value = (Value & MRC_BIT15) ? (Value << 1) ^ CRC_XOR_MASK : Value << 1;
    }
  }

  *Crc = (UINT16) Value;
  return TRUE;
}

/**
  Calculate the medium and fine timebases, using integer math.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if medium timebase is valid, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmTimeBase (
  IN OUT MrcParameters  *const MrcData
  )
{
  MrcDebug                  *Debug;
  MrcOutput                 *Outputs;
  MrcControllerOut          *ControllerOut;
  MrcChannelOut             *ChannelOut;
  MrcDimmOut                *DimmOut;
  MrcTimeBase               *TimeBase;
  MrcProfile                Profile;
  UINT8                     Controller;
  UINT8                     Channel;
  UINT8                     Dimm;
  BOOLEAN                   IsPrinted;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  //
  // // DDR5 does not use MTB/FTB timebase
  // LP5 uses a fixed setting: MTB = 125ps, FTB = 1ps
  // Set these values on all DIMMs / profiles, to reduce code size in this routine
  //
  IsPrinted = FALSE;
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          // DDR5 does not use timebase
          TimeBase = &ChannelOut->TimeBase[Dimm][Profile];
          if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
            TimeBase->Mtb = 0;
            TimeBase->Ftb = 0;
          } else {
            TimeBase->Mtb = 125000;
            TimeBase->Ftb = 1000;
          }
          if (!IsPrinted) {
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s: %u %u\n", TimeBaseString, TimeBase->Mtb, TimeBase->Ftb);
            IsPrinted = TRUE;
          }
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  return TRUE;
}

/**
  Calculate the SDRAM minimum cycle time (tCKmin) that this DIMM supports.
    Then use the lookup table to obtain the frequency closest to the clock multiple.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if the DIMM frequency is supported, otherwise FALSE and the frequency is set to fInvalid.
**/
BOOLEAN
GetChannelDimmtCK (
  IN OUT MrcParameters  *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  INT32                 tCKminMtb;
  INT32                 tCKminFine;
  INT32                 tCKminIndex;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                DimmCalculated;
  UINT32                MemoryClockMax;
  MrcClockRatio         Ratio;
  const MRC_EXT_INPUTS_TYPE   *ExtInputs;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_3_0  *Data3;
  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s (fs)\n", tCKString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = fNoInit;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          DimmCalculated = 0;
          Calculated = 0;
          if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
            tCKminMtb  = 0;
            tCKminFine = 0;
          } else {
            tCKminMtb   = Spd->Lpddr.Base.tCKmin.Bits.tCKmin;
            tCKminFine  = Spd->Lpddr.Base.tCKminFine.Bits.tCKminFine;
          }
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (XmpSupport (DimmOut, Profile)) {
                if (ExtInputs->MemoryProfile != Profile) {
                  MemoryClockMax = ConvertFreq2Clock(MrcData, MAX_FREQ_OC);
                } else {
                  MemoryClockMax = Outputs->MemoryClockMax;
                }
                Data3 = &Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1];
                DimmCalculated = Data3->tCKAVGmin.Bits.tCKmin * 1000; // Convert Picoseconds to Femtoseconds
                Calculated = MAX (MemoryClockMax, DimmCalculated);
              }
              break;
            case CUSTOM_PROFILE1:
              if (ExtInputs->Ratio > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Ratio = ExtInputs->Ratio;
                  Ratio -= Outputs->GearMode ? (Ratio % 4) : (Ratio % 2);
                  DimmCalculated = MrcRatioToClock (MrcData, Ratio);
                  Calculated = DimmCalculated;
                  Calculated = MAX (Outputs->MemoryClockMax, Calculated);
                  break;
                } else {
                  // In AUTO mode, so no break.
                }
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                DimmCalculated = Spd->Ddr5.Base.tCKmin.Bits.tCKmin * 1000; // Convert Picoseconds to Femtoseconds
              } else {
                DimmCalculated = (MediumTimebase * tCKminMtb) + (FineTimebase * tCKminFine);
              }
              Calculated = MAX (Outputs->MemoryClockMax, DimmCalculated);
              break;
          } //switch
          Actual[Profile] = MAX (Actual[Profile], Calculated);

          // tCK is at the 4:1 value in the SPD for LPDDR5 which is what is expected.
          // Find the closest tCK in the table
          ConvertClock2Freq (MrcData, Actual[Profile], &tCKminIndex);
          Actual[Profile] = FreqTable[tCKminIndex].tCK;
          if (Outputs->IsLpddr5) {
            Actual[Profile] *= 4;
          }
          if (Profile == ExtInputs->MemoryProfile) {
            DimmOut->Speed = ConvertClock2Freq (MrcData, DimmCalculated, NULL);
          }

          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
    Outputs->MemFrequency[Profile] = ConvertClock2Freq (MrcData, Actual[Profile], NULL);
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tCK = Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  Outputs->MemoryClock = Actual[ExtInputs->MemoryProfile];

  return TRUE;
}

/**
  Determine if the calculated CAS Latency is within the supported CAS Latency Mask

    @param[in] MrcData     - Pointer to MrcData data structure.
    @param[in] Profile     - the Profile for checking
    @param[in] tAA         - the calculated CAS Latency in tCKs.
    @param[in] DdrType     - the SDRAM type number.
    @param[in] tCLMask     - the supported CAS Latency mask.
    @param[in] tCLLimitMin - the minimum supported CAS Latency
    @param[in] SdramWidth  - SDRAM width (8 or 16)

    @retval TRUE if the CAS latency has been found in supported mask, otherwise FALSE.
**/
BOOLEAN
ValidtCL (
  IN MrcParameters    *const MrcData,
  IN MrcProfile       Profile,
  IN const UINT32     tAA,
  IN const MrcDdrType DdrType,
  IN const UINT64     tCLMask,
  IN const UINT32     tCLLimitMin,
  IN const UINT8      SdramWidth
  )
{
  const MRC_FUNCTION *MrcCall;
  BOOLEAN            Valid;

  Valid           = FALSE;
  MrcCall         = MrcData->Inputs.Call.Func;

  switch (DdrType) {
    case MRC_DDR_TYPE_DDR5:
      // DDR5 Only supports even values of tCL(tAA)
      if (((tAA % 2) != 0) || (tAA < tCLLimitMin)) {
        Valid = FALSE;
      } else {
        Valid = (MrcCall->MrcRightShift64 (tCLMask, (tAA - tCLLimitMin)/2) & 1);

        if (!Valid && (Profile != STD_PROFILE)) {
          // Some XMP memory reports weird CAS Latency mask.
          // Lower tCL can pass, but bigger tCL can't.
          // Below codes work around it.
          if ((MrcCall->MrcLeftShift64 (1UL, tAA - tCLLimitMin) > tCLMask) && (tCLMask > 0)) {
            Valid = TRUE;
          }
        }
      }
      break;
    default:
      if (tAA >= tCLLimitMin) {
        Valid = (MrcCall->MrcRightShift64 (tCLMask, (tAA - tCLLimitMin)) & 1);
      }
      break;

    // Ignoring Valid tCL mask as it doesn't make sense in terms of LPDDR as tCL is binned by the frequency requested
    // and all parts must be of the same timing.
    case MRC_DDR_TYPE_LPDDR5:
      Valid = TRUE;
      break;
  }
  return Valid;
}

/**
  Calculate CAS Latency
    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in] CasLatencies - Pointer to SPD5_CAS_LATENCIES_SUPPORTED_STRUCT data structure.

    @retval UINT64 CasMask
**/
static
UINT64
CalcDdr5CasLatencies (
  IN OUT MrcParameters  * const MrcData,
  IN const SPD5_CAS_LATENCIES_SUPPORTED_STRUCT *CasLatencies
  )
{
  MRC_FUNCTION          *MrcCall;
  UINT64                CasMask;

  MrcCall        = MrcData->Inputs.Call.Func;

  // Lower 32 bits
  CasMask     = ((UINT32) CasLatencies->Data8[0]) |
    ((UINT32) CasLatencies->Data8[1] << 8) |
    ((UINT32) CasLatencies->Data8[2] << 16) |
    ((UINT32) CasLatencies->Data8[3] << 24);
  // Upper 32 bits
  CasMask    |= MrcCall->MrcLeftShift64 ((UINT64) CasLatencies->Data8[4], 32);

  return CasMask;
}
/**
  Calculate the Minimum CAS Latency Time (tAAmin) for the given DIMMs.
      Step 1: Determine the common set of supported CAS Latency values for all modules
              on the memory channel using the CAS Latencies Supported in SPD.
      Step 2: Determine tAAmin(all) which is the largest tAAmin value for all modules on the memory channel.
      Step 3: Determine tCK(all) which is the largest tCKmin value for all
              the modules on the memory channel (Done in function GetChannelDimmtCK).
      Step 4: For a proposed tCK value between tCKmin and tCKmax, determine the desired CAS Latency.
              If tCKproposed is not a standard JEDEC value then tCKproposed must be adjusted to the
              next lower standard tCK value for calculating CLdesired.
      Step 5: Chose an actual CAS Latency that is greater than or equal to CLdesired and is
              supported by all modules on the memory channel as determined in step 1. If no such value exists,
              choose a higher tCKproposed value and repeat steps 4 and 5 until a solution is found.
      Step 6: Once the calculation of CLactual is completed, the BIOS must also verify that this CAS
              Latency value does not exceed tAAmax, which is 20 ns for all DDR3 speed grades.
              If not, choose a lower CL value and repeat steps 5 and 6 until a solution is found.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if the CAS latency has been calculated, otherwise FALSE and the returned value is set to zero.
**/
BOOLEAN
GetChannelDimmtAA (
  IN OUT MrcParameters  *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  BOOLEAN               Found[MAX_PROFILE];
  BOOLEAN               CustomProfile;
  BOOLEAN               tCLOverride;
  BOOLEAN               Status;
  INT32                 MediumTimeBase;
  INT32                 FineTimeBase;
  INT32                 tCKminIndex;
  INT32                 tCKminIndexSave;
  INT32                 TimingFTB;
  UINT32                TimingMTB;
  UINT32                tCKmin;
  UINT64                CommonCasMask[MAX_PROFILE];
  UINT64                CasMask;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                tCLLimitMin;
  UINT32                tCLLimitMax;
  UINT32                tAAmax;
  UINT32                Index;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT8                 SdramWidth;
  MRC_EXT_INPUTS_TYPE  *ExtInputs;

  Inputs         = &MrcData->Inputs;
  Outputs        = &MrcData->Outputs;
  ExtInputs      = Inputs->ExtInputs.Ptr;
  Debug          = &Outputs->Debug;
  tCKmin         = 0;
  Calculated     = 0;
  Status         = FALSE;
  tCLOverride    = FALSE;
  MediumTimeBase = 0;
  FineTimeBase   = 0;
  TimingMTB      = 0;
  TimingFTB      = 0;
  SdramWidth     = 0;

  // Set Outputs->IsDvfscEnabled based on Inputs->IsDvfscEnabled
  Outputs->IsDvfscEnabled = FALSE;

  if (Outputs->IsLpddr5) {
    Outputs->Frequency = ConvertClock2Freq (MrcData, Outputs->MemoryClock, NULL);
    if ((ExtInputs->DvfscEnabled && (Outputs->Frequency <= f3200)) && ((Outputs->SaGvPoint == 0) || !MrcIsSaGvEnabled (MrcData))) {
      Outputs->IsDvfscEnabled = TRUE;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "IsDvfscEnabled: %u\n", Outputs->IsDvfscEnabled);
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s tCL Mask\n", tAAString, HeaderString);

  tAAmax = MRC_UINT32_MAX; // tAA_max is not defined for LPDDR5 or DDR5
  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      tCLLimitMin = 20;
      tCLLimitMax = 98;
      break;

    case MRC_DDR_TYPE_LPDDR5:
    default:
    // For LPDDR5
      tCLLimitMin = 3;
      tCLLimitMax = 40;
      break;
  }
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    CustomProfile          = (Profile == CUSTOM_PROFILE1) && (ExtInputs->MemoryProfile == CUSTOM_PROFILE1);
    CommonCasMask[Profile] = MRC_UINT64_MAX;
    Actual[Profile]        = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          MediumTimeBase = ChannelOut->TimeBase[Dimm][Profile].Mtb;
          FineTimeBase   = ChannelOut->TimeBase[Dimm][Profile].Ftb;
          CasMask        = 0;
          SdramWidth     = DimmOut->SdramWidth;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Index = Profile - XMP_PROFILE1;
              if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
                CasMask     = CalcDdr5CasLatencies(MrcData, &(Spd->Ddr5.EndUser.Xmp.Data[Index].CasLatencies));
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Index].tAAmin.Bits.tAAmin, tCKmin);
                if ((ExtInputs->OCSafeMode) && ((Calculated + 2) < Actual[STD_PROFILE])) {
                  /*
                   * Sometimes, XMP profile might provide inappropriate tCL value.
                   * Add a checking to make sure tCL shouldn't be much smaller
                   * than STD_PROFILE's.
                   */
                  Calculated = Actual[STD_PROFILE] - 2;
                }
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tCL > 0) {
                CasMask         = MRC_UINT64_MAX;
                Calculated      = DimmIn->Timing.tCL;
                tCLOverride     = TRUE;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                CasMask    = CalcDdr5CasLatencies(MrcData, &(Spd->Ddr5.Base.CasLatencies));
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tAAmin.Bits.tAAmin, tCKmin);
              } else {
                TimingMTB   = Spd->Lpddr.Base.tAAmin.Bits.tAAmin;
                TimingFTB   = Spd->Lpddr.Base.tAAminFine.Bits.tAAminFine;
                CasMask     = Spd->Lpddr.Base.CasLatencies.Data & MRC_SPD_LPDDR_CL_SUPPORTED_MASK;
                if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) {
                  Calculated  = GetLpddr5tCL (tCKmin, SdramWidth, (BOOLEAN) ExtInputs->TrainingEnables2.DBI, Outputs->IsDvfscEnabled);
                }
              }
              break;
          } //end switch

          if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
            if (Calculated < 22) {
              Calculated = 22;     // 22 is the smallest valid CL in DDR5
            }

            if ((Profile == CUSTOM_PROFILE1) && ((Calculated % 2) != 0)) {
              Calculated++;        // JEDEC spec only allows even numbers
            }
          }

          Actual[Profile] = MAX (Actual[Profile], Calculated);

          CommonCasMask[Profile] &= CasMask;
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u %8llXh\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated,
            CasMask
            );
        } //for Dimm
      } //for Channel
    } //for Controller

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Profile %u common set of supported CAS Latency values = %llXh\n", Profile, CommonCasMask[Profile]);

    if ((Profile >= XMP_PROFILE1) && (tCKmin == 0)) {
      continue;
    }

    Found[Profile] = FALSE;
    ConvertClock2Freq (MrcData, tCKmin, &tCKminIndex);

    do {
      for (; !Found[Profile] && (Actual[Profile] <= tCLLimitMax); Actual[Profile]++) {
        if (CustomProfile) {
          Found[Profile] = TRUE;
        } else if ((Actual[Profile] * tCKmin) <= tAAmax) {
          Found[Profile] = ValidtCL (
                            MrcData,
                            Profile,
                            Actual[Profile],
                            Outputs->DdrType,
                            CommonCasMask[Profile],
                            tCLLimitMin,
                            SdramWidth
                            );
        } // if
        if (Found[Profile]) {
          if (Profile == ExtInputs->MemoryProfile) {
            Outputs->MemoryClock = tCKmin;
            Status = TRUE;
          }
          Outputs->Timing[Profile].tCL = (UINT16) Actual[Profile];
          Outputs->Timing[Profile].tCK = tCKmin;
          break;
        } //if
      } //for Actual[Profile]
      if (!Found[Profile]) {
        if (CustomProfile && ((ExtInputs->Ratio > 0) || (tCLOverride == TRUE))) {
          break;
        } else {
          tCKminIndexSave = tCKminIndex;
          while (--tCKminIndex > 0) {
            tCKmin = FreqTable[tCKminIndex].tCK;
            if (Outputs->IsLpddr5) {
              tCKmin *= 4;
            }
            ConvertClock2Freq (MrcData, tCKmin, &tCKminIndex);
            Actual[Profile] = (tCKmin == 0) ? 0 : ((MediumTimeBase * TimingMTB) + (FineTimeBase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
            MRC_DEBUG_MSG (Debug,
              MSG_LEVEL_WARNING,
              "Warning: The memory frequency is being downgraded on profile %u, from %u to %u and the new tCL is %u\n",
              Profile,
              FreqTable[tCKminIndexSave].Frequency,
              FreqTable[tCKminIndex].Frequency,
              Actual[Profile]);
            break;
          }
        }
      }
    } while (!Found[Profile] && (tCKminIndex > 0));
  } //for Profile

  Outputs->Frequency = ConvertClock2Freq (MrcData, Outputs->MemoryClock, NULL);
  Outputs->HighFrequency = Outputs->Frequency;

#ifdef MRC_DEBUG_PRINT
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Found[Profile] ? Actual[Profile] : 0);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n  Memory clock = %ufs\n  Memory Frequency = %u\n", Outputs->MemoryClock, Outputs->Frequency);
#endif

  MrcUpdateDdr5MintCL (MrcData);

  return (Status);
}

/**
  Calculate the tCWL value for DDR5.

    @param[in] tCL - current tCL in DCLKs.

    @retval DDR5 tCWL in DCLK units
**/
UINT32
GetDdr5tCWL (
  IN const UINT32 tCL
  )
{
  UINT32 tCWL;

  tCWL = tCL - 2;
  return tCWL;
}

/**
  Calculate the tCWL value for LPDDR5.

  JEDEC Spec x8/x16 WL values:
    Lower Clk   Upper Clk      SetA   SetB
    Freq Limit  Freq Limit     WL     WL
    --------------------------------------
    10            67           2      2
    67            133          2      3
    133           200          3      4
    200           267          4      5
    267           344          4      7
    344           400          5      8
    400           467          6      9
    467           533          6      11
    533           600          7      12
    600           688          8      14
    688           750          9      15
    750           800          9      16

  @param[in] tCK   - The memory DCLK in femtoseconds.
  @param[in] WlSet - 0: Set A, 1: Set B

@retval LpDDR5 tCWL Value
**/
UINT32
GetLpddr5tCWL (
  IN UINT32 tCKmin,
  IN UINT8  WlSet
  )
{
  UINT32 tCWL;
  UINT32 tCKNorm;

  tCKNorm = tCKmin / 4;
  //
  // Using WL Set B values from table 4.6.2 of LPDDR5 JEDEC Spec.
  //
  if (tCKNorm >= MRC_DDR_533_TCK_MIN) {
    tCWL = 2;
  } else if (tCKNorm >= MRC_DDR_1067_TCK_MIN) {
    tCWL = 3;
  } else if (tCKNorm >= MRC_DDR_1600_TCK_MIN) {
    tCWL = 4;
  } else if (tCKNorm >= MRC_DDR_2133_TCK_MIN) {
    tCWL = 5;
  } else if (tCKNorm >= MRC_DDR_2750_TCK_MIN) {
    tCWL = 7;
  } else if (tCKNorm >= MRC_DDR_3200_TCK_MIN) {
    tCWL = 8;
  } else if (tCKNorm >= MRC_DDR_3733_TCK_MIN) {
    tCWL = 9;
  } else if (tCKNorm >= MRC_DDR_4267_TCK_MIN) {
    tCWL = 11;
  } else if (tCKNorm >= MRC_DDR_4800_TCK_MIN) {
    tCWL = 12;
  } else if (tCKNorm >= MRC_DDR_5500_TCK_MIN) {
    tCWL = 14;
  } else if (tCKNorm >= MRC_DDR_6000_TCK_MIN) {
    tCWL = 15;
  } else if (tCKNorm >= MRC_DDR_6400_TCK_MIN) {
    tCWL = 16;
  } else if (tCKNorm >= MRC_DDR_7500_TCK_MIN) {
    tCWL = 19;
  } else if (tCKNorm >= MRC_DDR_8533_TCK_MIN) {
    tCWL = 22;
  } else {
    tCWL = 24; // MRC_DDR_9600_TCK_MIN
  }
  return tCWL;
}

/**
  Calculate the minimum tCWL timing value for the given memory frequency.
    We calculate timings for all profiles so that this information can be passed out of MRC.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtCWL (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                tCL;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tCWLString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    tCL            = Outputs->Timing[Profile].tCL;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              /*FALLTHROUGH*/
            case CUSTOM_PROFILE1:
              if ((CUSTOM_PROFILE1 == Profile ) && (DimmIn->Timing.tCWL > 0)) {
                Calculated = DimmIn->Timing.tCWL;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) {
                Calculated = GetLpddr5tCWL (tCKmin, 1); // We always use Set B (1)
              } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated = GetDdr5tCWL (tCL);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tCWL = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the DRAM Page Size in units of Bytes

    @param[in, out] ColumnSize - The SDRAM Column Size in bits
    @param[in, out] SdramWidth - The SRAM IO width

    @return The DRAM page size in units of Bytes
**/
UINT16
CalculatePageSize (
  IN UINT16 ColumnSize,
  IN UINT8  SdramWidth
  )
{
  UINT16 ColumnSizeInBytes;
  // Calculate the total Column bit count and convert to Bytes
  ColumnSizeInBytes = (UINT16)(ColumnSize / 8);
  // Calculate the page size
  return ColumnSizeInBytes * SdramWidth;
}

/**
  Calculate the minimum tFAW timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtFAW (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *SpdIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT16                PageSize;
  MRC_LP5_BANKORG       Lp5BGOrg;
  UINT32                Index;
  UINT32                tFAWNs;
  UINT32                tFAWNCk;

  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0 *Header5;
  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tFAWString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          SpdIn   = &DimmIn->Spd.Data;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Header5 = &SpdIn->Ddr5.EndUser.Xmp.Header;
              if ((Header5->XmpSpecRevision.Data >= 0x12) && Inputs->IsXMP3Revision12Supported) {
                Index = Profile - XMP_PROFILE1;
                Calculated = PicoSecondsToClocks (SpdIn->Ddr5.EndUser.Xmp.Data[Index].tFAWmin.Bits.tFAWmin, tCKmin);
                Calculated = MAX(Calculated, SpdIn->Ddr5.EndUser.Xmp.Data[Index].tFAWmin.Bits.LowerClockLimit);
                break;
              }
            case CUSTOM_PROFILE1:
              if ((Profile == CUSTOM_PROFILE1) && (DimmIn->Timing.tFAW > 0)) {
                Calculated = DimmIn->Timing.tFAW;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                PageSize = CalculatePageSize(
                            DimmOut->ColumnSize,
                            DimmOut->SdramWidth
                            );
                if (PageSize < 2048) {
                  tFAWNCk = 32;
                  if (Outputs->Frequency <= f3200) {
                    tFAWNs = MRC_DDR5_TFAW_1K_3200_MIN;
                  } else if (Outputs->Frequency <= f3600) {
                    tFAWNs = MRC_DDR5_TFAW_1K_3600_MIN;
                  } else if (Outputs->Frequency <= f4000) {
                    tFAWNs = MRC_DDR5_TFAW_1K_4000_MIN;
                  } else if (Outputs->Frequency <= f4400) {
                    tFAWNs = MRC_DDR5_TFAW_1K_4400_MIN;
                  } else if (Outputs->Frequency <= f4800) {
                    tFAWNs = MRC_DDR5_TFAW_1K_4800_MIN;
                  } else if (Outputs->Frequency <= f5200) {
                    tFAWNs = MRC_DDR5_TFAW_1K_5200_MIN;
                  } else if (Outputs->Frequency <= f5600) {
                    tFAWNs = MRC_DDR5_TFAW_1K_5600_MIN;
                  } else if (Outputs->Frequency <= f6000) {
                    tFAWNs = MRC_DDR5_TFAW_1K_6000_MIN;
                  } else if (Outputs->Frequency <= f6400) {
                    tFAWNs = MRC_DDR5_TFAW_1K_6400_MIN;
                  } else if (Outputs->Frequency <= f6800) {
                    tFAWNs = MRC_DDR5_TFAW_1K_6800_MIN;
                  } else if (Outputs->Frequency <= f7200) {
                    tFAWNs = MRC_DDR5_TFAW_1K_7200_MIN;
                  } else if (Outputs->Frequency <= f7600) {
                    tFAWNs = MRC_DDR5_TFAW_1K_7600_MIN;
                  } else if (Outputs->Frequency <= f8000) {
                    tFAWNs = MRC_DDR5_TFAW_1K_8000_MIN;
                  } else if (Outputs->Frequency <= f8400) {
                    tFAWNs = MRC_DDR5_TFAW_1K_8400_MIN;
                  } else {
                    tFAWNs = MRC_DDR5_TFAW_1K_8800_MIN;
                  }
                } else {
                  tFAWNCk = 40;
                  if (Outputs->Frequency <= f3200) {
                    tFAWNs = MRC_DDR5_TFAW_2K_3200_MIN;
                  } else if (Outputs->Frequency <= f3600) {
                    tFAWNs = MRC_DDR5_TFAW_2K_3600_MIN;
                  } else if (Outputs->Frequency <= f4000) {
                    tFAWNs = MRC_DDR5_TFAW_2K_4000_MIN;
                  } else if (Outputs->Frequency <= f4400) {
                    tFAWNs = MRC_DDR5_TFAW_2K_4400_MIN;
                  } else if (Outputs->Frequency <= f4800) {
                    tFAWNs = MRC_DDR5_TFAW_2K_4800_MIN;
                  } else if (Outputs->Frequency <= f5200) {
                    tFAWNs = MRC_DDR5_TFAW_2K_5200_MIN;
                  } else if (Outputs->Frequency <= f5600) {
                    tFAWNs = MRC_DDR5_TFAW_2K_5600_MIN;
                  } else if (Outputs->Frequency <= f6000) {
                    tFAWNs = MRC_DDR5_TFAW_2K_6000_MIN;
                  } else if (Outputs->Frequency <= f6400) {
                    tFAWNs = MRC_DDR5_TFAW_2K_6400_MIN;
                  } else if (Outputs->Frequency <= f6800) {
                    tFAWNs = MRC_DDR5_TFAW_2K_6800_MIN;
                  } else if (Outputs->Frequency <= f7200) {
                    tFAWNs = MRC_DDR5_TFAW_2K_7200_MIN;
                  } else if (Outputs->Frequency <= f7600) {
                    tFAWNs = MRC_DDR5_TFAW_2K_7600_MIN;
                  } else if (Outputs->Frequency <= f8000) {
                    tFAWNs = MRC_DDR5_TFAW_2K_8000_MIN;
                  } else if (Outputs->Frequency <= f8400) {
                    tFAWNs = MRC_DDR5_TFAW_2K_8400_MIN;
                  } else {
                    tFAWNs = MRC_DDR5_TFAW_2K_8800_MIN;
                  }
                }
                Calculated = PicoSecondsToClocks (tFAWNs, tCKmin);
                Calculated = MAX (tFAWNCk, Calculated);
              } else if (tCKmin > 0) {
                Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
                if ((DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) && Lp5BGOrg != MrcLp58Bank) {
                    if (Outputs->LpX) {
                      Calculated = DIVIDECEIL ((15000000 - (tCKmin / 100)), tCKmin); // 15ns
                    } else {
                      Calculated = DIVIDECEIL ((20000000 - (tCKmin / 100)), tCKmin); // 20ns
                    }
                } else {
                  Calculated = DIVIDECEIL ((40000000 - (tCKmin / 100)), tCKmin); // 40ns
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
            );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tFAW = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRAS timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRAS (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRASString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tRASmin.Bits.tRASmin, tCKmin);
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRAS > 0) {
                Calculated = DimmIn->Timing.tRAS;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRASmin.Bits.tRASmin, tCKmin);
                } else {
                  // LPDDR5
                  Calculated = DIVIDECEIL ((42000000 - (tCKmin / 100)), tCKmin); // 42ns
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tRAS = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRC timing value for the given memory frequency.
  SKL MC doesn't have a register for tRC, it uses (tRAS + tRP).
  Print the tRC values for each profile and issue a warning if tRC > (tRAS + tRP).

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRC (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRCString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tRCmin.Bits.tRCmin, tCKmin);
              }
              break;
            case CUSTOM_PROFILE1:
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRCmin.Bits.tRCmin, tCKmin);
                } else {
                  TimingMTB = (UINT32) (Spd->Lpddr.Base.tRPpb.Bits.tRPpb);
                  TimingFTB = Spd->Lpddr.Base.tRPpbFine.Bits.tRPpbFine;
                  Calculated = DIVIDECEIL (((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) + 42000000 - (tCKmin / 100)), tCKmin); // tRC = tRAS + tRPpb (tRAS is 42ns)
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    if (Outputs->Timing[Profile].tRAS + Outputs->Timing[Profile].tRCDtRP < (UINT16) Actual[Profile]) {
      MRC_DEBUG_MSG (Debug, MSG_LEVEL_WARNING, "\nWARNING: tRC is bigger than tRAS+tRP !\n");
    }

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRCD timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRCD (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRCDString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tRCDmin.Bits.tRCDmin, tCKmin);
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRCDtRP > 0) {
                Calculated = DimmIn->Timing.tRCDtRP;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRCDmin.Bits.tRCDmin, tCKmin);
                } else {
                  if (!Outputs->IsDvfscEnabled) {
                    Calculated = DIVIDECEIL ((18000000 - (tCKmin / 100)), tCKmin); // 18ns
                  } else {
                    Calculated = DIVIDECEIL ((19000000 - (tCKmin / 100)), tCKmin); // 19ns
                  }
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tRCDtRP = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tREFI timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtREFI (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                TimingMTB;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tREFIString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;

          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              /*FALLTHROUGH*/
            case CUSTOM_PROFILE1:
              if ((CUSTOM_PROFILE1 == Profile) && (DimmIn->Timing.tREFI > 0)) {
                Calculated = DimmIn->Timing.tREFI;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                switch (DimmOut->DdrType) {
                  default:
                  case MRC_DDR_TYPE_DDR5:
                    // Only supports "Fine Granularity" refresh, so
                    // we use the tREFI2 parameter from the DDR5 spec
                    TimingMTB = TREFIMIN_DDR5;
                    break;
                  case MRC_DDR_TYPE_LPDDR5:
                    TimingMTB = TREFIMIN_LPDDR;
                    break;
                }
                // Scaled formula factor to 1000 to better accuracy values
                Calculated = (((TimingMTB * 1000) + (tCKmin - 1000)) / tCKmin);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tREFI = Actual[Profile];
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  This function returns the tRFC - All banks.
  LPDDR5 timing is based on tRFCab

  @param[in] DeviceCapacity - DRAM Device Capacity

  @retval tRFC - tRFC timing based on Device Capacity
**/
UINT32
MrcGetLpddr5Trfcab (
  IN UINT32   DeviceCapacity
  )
{
  UINT32  tRFC;

  switch (DeviceCapacity) {
    case 8192:
      tRFC = MRC_LP5_tRFCAB_8GB_NS;
      break;
    case 12288:
      tRFC = MRC_LP5_tRFCAB_12GB_NS;
      break;
    case 16384:
      tRFC = MRC_LP5_tRFCAB_16GB_NS;
      break;
    case 24576:
      tRFC = MRC_LP5_tRFCAB_24GB_NS;
      break;
    case 32768:
      tRFC = MRC_LP5_tRFCAB_32GB_NS;
      break;
    default:
      tRFC = MRC_LP5_tRFCAB_8GB_NS;
      break;
  }
  return tRFC;
}

/**
  This function returns the tRFC - per bank
  LPDDR5 timing is based on tRFCpb

  @param[in] DeviceCapacity - DRAM Device Capacity

  @retval tRFCpb - tRFCpb timing based on Device Capacity
**/
UINT32
MrcGetLpddr5Trfcpb (
  IN UINT32   DeviceCapacity
  )
{
  UINT32  tRFCpb;

  switch (DeviceCapacity) {
    case 8192:
      tRFCpb = MRC_LP5_tRFCPB_8GB_NS;
      break;
    case 12288:
      tRFCpb = MRC_LP5_tRFCPB_12GB_NS;
      break;
    case 16384:
      tRFCpb = MRC_LP5_tRFCPB_16GB_NS;
      break;
    case 24576:
      tRFCpb = MRC_LP5_tRFCPB_24GB_NS;
      break;
    case 32768:
      tRFCpb = MRC_LP5_tRFCPB_32GB_NS;
      break;
    default:
      tRFCpb = MRC_LP5_tRFCPB_8GB_NS;
      break;
  }
  return tRFCpb;
}

/**
    Calculate the minimum tRFC timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRFC (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                DeviceCapacity;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRFCString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;

          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tRFC1min.Bits.tRFC1min * 1000, tCKmin);
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRFC > 0) {
                Calculated = DimmIn->Timing.tRFC;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  // DDR5 SPD tRFC1min is in units of nanoseconds
                  Calculated = PicoSecondsToClocks (Spd->Ddr5.Base.tRFC1min.Bits.tRFC1min * 1000, tCKmin);
                } else {
                  TimingMTB = Spd->Lpddr.Base.tRFCab.Bits.tRFCab;
                  if (Outputs->IsDvfscEnabled) {
                    DeviceCapacity = SdramCapacityTable[DimmOut->DensityIndex] * 8;
                    TimingMTB = MrcGetLpddr5Trfcab (DeviceCapacity) * NS_TO_MTB_FACTOR;
                  }
                  Calculated = ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tRFC = (UINT16) Actual[Profile];
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRFCpb timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRFCpb (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                TimingMTB;
  INT32                 MediumTimebase;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                DeviceCapacity;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRFCpbString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tRFCsbmin.Bits.tRFCsbmin * 1000, tCKmin);
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRFCpb > 0) {
                Calculated = DimmIn->Timing.tRFCpb;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_LPDDR5 == DimmOut->DdrType) {
                  TimingMTB = Spd->Lpddr.Base.tRFCpb.Bits.tRFCpb;
                  if (Outputs->IsDvfscEnabled) {
                    DeviceCapacity = SdramCapacityTable[DimmOut->DensityIndex] * 8;
                    TimingMTB = MrcGetLpddr5Trfcpb (DeviceCapacity) * NS_TO_MTB_FACTOR;
                  }
                  Calculated = ((MediumTimebase * TimingMTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  // DDR5 SPD tRFCsbmin is in units of nanoseconds
                  Calculated = PicoSecondsToClocks (Spd->Ddr5.Base.tRFCsbmin.Bits.tRFCsbmin * 1000, tCKmin);
                } else {
                  // This timing does not exist.
                  Calculated = 0;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tRFCpb = (UINT16) Actual[Profile];
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRFC2 timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRFC2 (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRFC2String, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // DDR5 SPD tRFC2min is in units of nanoseconds
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tRFC2min.Bits.tRFC2min * 1000, tCKmin);
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRFC2 > 0) {
                Calculated = DimmIn->Timing.tRFC2;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // DDR5 SPD tRFC2min is in units of nanoseconds
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRFC2min.Bits.tRFC2min * 1000, tCKmin);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tRFC2 = (UINT16) Actual[Profile];
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRFC4 timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRFC4 (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRFC4String, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRFC4 > 0) {
                Calculated = DimmIn->Timing.tRFC4;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tRFC4 = (UINT16) Actual[Profile];
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRP timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRP (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRPString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd            = &DimmIn->Spd.Data;
          Calculated     = 0;
          TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
          MediumTimebase = TimeBase->Mtb;
          FineTimebase   = TimeBase->Ftb;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                continue;
              }
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated  = PicoSecondsToClocks (Spd->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tRPmin.Bits.tRPmin, tCKmin);
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRCDtRP > 0) {
                Calculated = DimmIn->Timing.tRCDtRP;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                  Calculated  = PicoSecondsToClocks (Spd->Ddr5.Base.tRPmin.Bits.tRPmin, tCKmin);
                } else {
                  TimingMTB = Spd->Lpddr.Base.tRPpb.Bits.tRPpb;
                  TimingFTB = Spd->Lpddr.Base.tRPpbFine.Bits.tRPpbFine;
                  Calculated = ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          //
          // Take MAX of tRCD and tRP if they are different in SPD.
          // This assumes that GetChannelDimmtRCD() was already called.
          //
          Actual[Profile] = MAX (Actual[Profile], Outputs->Timing[Profile].tRCDtRP);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tRCDtRP = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRPab timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRPab (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *Spd;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcTimeBase           *TimeBase;
  MrcProfile            Profile;
  BOOLEAN               Flag;
  BOOLEAN               IsLpddr;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  INT32                 MediumTimebase;
  INT32                 FineTimebase;
  UINT32                TimingMTB;
  INT32                 TimingFTB;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Flag    = FALSE;
  IsLpddr = (Outputs->IsLpddr);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if ((DIMM_PRESENT == DimmOut->Status) && IsLpddr) {
            Spd            = &DimmIn->Spd.Data;
            Calculated     = 0;
            TimeBase       = &ChannelOut->TimeBase[Dimm][Profile];
            MediumTimebase = TimeBase->Mtb;
            FineTimebase   = TimeBase->Ftb;
            if (tCKmin > 0) {
              TimingMTB  = Spd->Lpddr.Base.tRPab.Bits.tRPab;
              TimingFTB  = Spd->Lpddr.Base.tRPabFine.Bits.tRPabFine;
              Calculated = ((MediumTimebase * TimingMTB) + (FineTimebase * TimingFTB) - (tCKmin / 100) + (tCKmin - 1)) / tCKmin;
            }

            if (Calculated >= TRPABMINPOSSIBLE) {
              Actual[Profile] = MAX (Actual[Profile], Calculated);
            }
            if (!Flag) {
              Flag = TRUE;
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRPabString, HeaderString);
              MRC_DEBUG_MSG (
                Debug,
                MSG_LEVEL_NOTE,
                "  %7u %10u %8u %5u %6u\n",
                Profile,
                Controller,
                Channel,
                Dimm,
                Calculated
                );
            } //Flag
          } //DimmOut->Status
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  if (Flag) {
    //
    // Set the best case timing for all controllers/channels/dimms, for each profile.
    //
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
    for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
      if (NeedIgnoreXmp (MrcData, Profile)) {
        continue;
      }
      Outputs->Timing[Profile].tRPab = (UINT16) Actual[Profile];

      MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");
  }

  return TRUE;

}

/**
  Calculate the minimum tRRD timing value for the given memory frequency.
    MRC should not set tRRD below 4nCK for all frequencies.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE
**/
BOOLEAN
GetChannelDimmtRRD (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                MinCk;
  BOOLEAN               IsLpddr;
  MRC_LP5_BANKORG       Lp5BGOrg;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;
  IsLpddr  = (Outputs->IsLpddr);

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRRDString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRRD > 0) {
                Calculated = DimmIn->Timing.tRRD;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (!IsLpddr) {
                  Calculated = 0; // tRRD isn't used for DDR5
                } else {
                  // LPDDR5
                  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
                  if (Lp5BGOrg != MrcLp58Bank) {
                    if (Outputs->LpX) {
                      Calculated = DIVIDECEIL ((3750000 - (tCKmin / 100)), tCKmin); // 3.75ns
                    } else {
                      Calculated = DIVIDECEIL ((5000000 - (tCKmin / 100)), tCKmin); // 5ns
                    }
                  } else {
                    Calculated = DIVIDECEIL ((10000000 - (tCKmin / 100)), tCKmin); // 10ns
                  }
                }
              }
              if (IsLpddr) {
                // LPDDR5
                MinCk = 2;
                Calculated = MAX (Calculated, MinCk);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tRRD = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRRD same bank group timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRRD_L (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *SpdIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;
  UINT32                tRDRD_LNs;

  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0 *Header5;
  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRRDLString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          SpdIn   = &DimmIn->Spd.Data;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
             Header5 = &SpdIn->Ddr5.EndUser.Xmp.Header;
             if ((Header5->XmpSpecRevision.Data >= 0x12) && Inputs->IsXMP3Revision12Supported) {
               Index = Profile - XMP_PROFILE1;
               Calculated = PicoSecondsToClocks (SpdIn->Ddr5.EndUser.Xmp.Data[Index].tRRD_Lmin.Bits.tRRD_Lmin, tCKmin);
               Calculated = MAX(Calculated, SpdIn->Ddr5.EndUser.Xmp.Data[Index].tRRD_Lmin.Bits.LowerClockLimit);
               break;
             }
            case CUSTOM_PROFILE1:
              if ((CUSTOM_PROFILE1 == Profile) && (DimmIn->Timing.tRRD_L > 0)) {
                Calculated = DimmIn->Timing.tRRD_L;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                if (Outputs->Frequency <= f6400) {
                  // DDR5 tRRD_L = MAX (8nCK, 5ns)
                  tRDRD_LNs = MRC_DDR5_TRRD_L_6400_MIN;
                } else if (Outputs->Frequency <= f6800) {
                  tRDRD_LNs = MRC_DDR5_TRRD_L_6800_MIN;
                } else if (Outputs->Frequency <= f7200) {
                  tRDRD_LNs = MRC_DDR5_TRRD_L_7200_MIN;
                } else if (Outputs->Frequency <= f7600) {
                  tRDRD_LNs = MRC_DDR5_TRRD_L_7600_MIN;
                } else if (Outputs->Frequency <= f8400) {
                  tRDRD_LNs = MRC_DDR5_TRRD_L_8000_8400_MIN;
                } else {
                  tRDRD_LNs = MRC_DDR5_TRRD_L_8800_MIN;
                }
                Calculated = PicoSecondsToClocks (tRDRD_LNs, tCKmin);
                Calculated = MAX (8, Calculated);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tRRD_L = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tRRD different bank group timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRRD_S (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRRDSString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              break;
              /*FALLTHROUGH*/
            case CUSTOM_PROFILE1:
              if ((CUSTOM_PROFILE1 == Profile) && (DimmIn->Timing.tRRD_S > 0)) {
                Calculated = DimmIn->Timing.tRRD_S;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // DDR5 tRRD_S = 8nCK
                Calculated  = 8;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tRRD_S = (UINT16) Actual[Profile];
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tRTP value for DDR5.
  JEDEC Spec: tRTP = 7.5ns

    @param[in] tCK - the memory DCLK in femtoseconds.

    @retval DDR5 tRTP in DCLK units
**/
UINT32
GetDdr5tRTP (
  IN const UINT32 tCK
  )
{
  UINT32 tRTP;

  // Delay in tck of the interal read command to pre charge command
  tRTP = (tCK == 0) ? 0 : PicoSecondsToClocks (7500, tCK); // 7.5ns
  if ((tRTP == 13) || (tRTP == 16) || (tRTP == 19) || (tRTP == 22) || (tRTP == 25) || (tRTP == 28) || (tRTP == 31)) {
    // 13, 16, 19, 22, 25, 28, 31 are not valid values according to the JEDEC Specification
    // Use the next valid value
    tRTP++;
  }

  // Must be in the range JEDEC allows
  tRTP = RANGE(tRTP, TRTPMINPOSSIBLE, TRTPMAXPOSSIBLE);

  return tRTP;
}

/**
  Calculate the tWR value for LPDDR5
  JEDEC Spec: Ch 9.3 Core AC Timing Parameters for LPDDR5X
        DVFSC       DVFSC
        Disabled    Enabled
  --------------------------
  x16 |  34ns        41ns
  x8  |  36ns        43ns

  @param[in] tCK              - the memory tCK in femtoseconds.
  @param[in] SdramWidthIndex  - 1 for X8 and 2 for X16 SDRAM width
  @param[in] IsDvfscEnabled   - TRUE if DVFSC is enabled

  @retval tWR in tCK units
**/
UINT32
GetLpddr5tWR (
  IN const UINT32     tCK,
  IN UINT8            SdramWidthIndex,
  IN BOOLEAN          IsDvfscEnabled
  )
{
  UINT32  tWR;

  tWR = 0;
  if (tCK > 0) {
    if (!IsDvfscEnabled) {
      tWR = DIVIDECEIL ((((SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 36000000 : 34000000) - (tCK / 100)), tCK);
      if (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) {
        if (tWR <= 3) {
          tWR = 3;
        } else if (tWR <= 5) {
          tWR = 5;
        } else if (tWR <= 8) {
          tWR = 8;
        } else if (tWR <= 10) {
          tWR = 10;
        } else if (tWR <= 13) {
          tWR = 13;
        } else if (tWR <= 15) {
          tWR = 15;
        } else if (tWR <= 17) {
          tWR = 17;
        } else if (tWR <= 20) {
          tWR = 20;
        } else if (tWR <= 22) {
          tWR = 22;
        } else if (tWR <= 25) {
          tWR = 25;
        } else if (tWR <= 28) {
          tWR = 28;
        } else if (tWR <= 29) {
          tWR = 29;
        } else if (tWR <= 34) {
          tWR = 34;
        } else if (tWR <= 39) {
          tWR = 39;
        } else {
          tWR = 44;
        }
      } else {
        if (tWR <= 3) {
          tWR = 3;
        } else if (tWR <= 5) {
          tWR = 5;
        } else if (tWR <= 7) {
          tWR = 7;
        } else if (tWR <= 10) {
          tWR = 10;
        } else if (tWR <= 12) {
          tWR = 12;
        } else if (tWR <= 14) {
          tWR = 14;
        } else if (tWR <= 16) {
          tWR = 16;
        } else if (tWR <= 19) {
          tWR = 19;
        } else if (tWR <= 21) {
          tWR = 21;
        } else if (tWR <= 24) {
          tWR = 24;
        } else if (tWR <= 26) {
          tWR = 26;
        } else if (tWR <= 28) {
          tWR = 28;
        } else if (tWR <= 32) {
          tWR = 32;
        } else if (tWR <= 37) {
          tWR = 37;
        } else {
          tWR = 41;
        }
      }
    } else {  // Dvfsc enabled
      tWR = DIVIDECEIL ((((SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 43000000 : 41000000) - (tCK / 100)), tCK);
      if (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) {
        if (tWR <= 3) {
          tWR = 3;
        } else if (tWR <= 6) {
          tWR = 6;
        } else if (tWR <= 9) {
          tWR = 9;
        } else if (tWR <= 12) {
          tWR = 12;
        } else if (tWR <= 15) {
          tWR = 15;
        } else {
          tWR = 18;
        }
      } else {
        if (tWR <= 3) {
          tWR = 3;
        } else if (tWR <= 6) {
          tWR = 6;
        } else if (tWR <= 9) {
          tWR = 9;
        } else if (tWR <= 11) {
          tWR = 11;
        } else if (tWR <= 15) {
          tWR = 15;
        } else {
          tWR = 17;
        }
      }
    }
  } // tCK > 0
  return tWR;
}

/**
  Calculate the minimum tRTP timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRTP (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *SpdIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;

  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0 *Header5;
  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tRTPString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          SpdIn   = &DimmIn->Spd.Data;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Header5 = &SpdIn->Ddr5.EndUser.Xmp.Header;
                if ((Header5->XmpSpecRevision.Data >= 0x12) && Inputs->IsXMP3Revision12Supported) {
                  Index = Profile - XMP_PROFILE1;
                  Calculated = PicoSecondsToClocks (SpdIn->Ddr5.EndUser.Xmp.Data[Index].tRTPmin.Bits.tRTPmin, tCKmin);
                  Calculated = MAX(Calculated, SpdIn->Ddr5.EndUser.Xmp.Data[Index].tRTPmin.Bits.LowerClockLimit);
                  if ((Calculated == 13) || (Calculated == 16) || (Calculated == 19) || (Calculated == 22) || (Calculated == 25) || (Calculated == 28) || (Calculated == 31)) {
                    // 13, 16, 19, 22, 25, 28, 31 are not valid values according to the JEDEC Specification
                    // Use the next valid value
                    Calculated++;
                  }
                  // Must be in the range JEDEC allows
                  Calculated = RANGE(Calculated, TRTPMINPOSSIBLE, TRTPMAXPOSSIBLE);
                  break;
                } else {
                  Calculated = GetDdr5tRTP (tCKmin);
                }
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tRTP > 0) {
                Calculated = DimmIn->Timing.tRTP;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
                  Calculated = GetDdr5tRTP (tCKmin);
                } else {
                  Calculated = DIVIDECEIL ((7500000 - (tCKmin / 100)), tCKmin); // 7.5ns
                  if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) {
                    if (Calculated > 2) {
                      Calculated -= 2;
                    } else {
                      Calculated = 0;
                    }
                  }
                }
              }
              break;
          } //switch


          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tRTP = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tWR value for DDR5.

    @param[in] tCK the memory DCLK in femtoseconds.
    @param[in] WRmin SPD tWRmin value.

    @return DDR5 tWR in DCLK units
**/
static
UINT32
GetDdr5tWR (
  IN const UINT32 tCK,
  IN const UINT32 tWRmin
  )
{
  UINT32 tWR;

  tWR = PicoSecondsToClocks (tWRmin, tCK);
  tWR = RANGE (tWR, DDR5_TWRMINPOSSIBLE, DDR5_TWRMAXPOSSIBLE);  // JEDEC limits
  tWR = DIVIDECEIL(tWR - DDR5_TWRMINPOSSIBLE, 6) * 6 + DDR5_TWRMINPOSSIBLE; // Step size is 6 CK
  return tWR;
}

/**
  Calculate the minimum tWR timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtWR (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *SpdIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT8                 SdramWidthIndex;

  Inputs      = &MrcData->Inputs;
  Outputs     = &MrcData->Outputs;
  Debug       = &Outputs->Debug;
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tWRString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          SpdIn = &DimmIn->Spd.Data;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;

          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated = GetDdr5tWR (tCKmin, SpdIn->Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].tWRmin.Bits.tWRmin);
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tWR > 0) {
                Calculated = DimmIn->Timing.tWR;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
                Calculated = GetDdr5tWR (tCKmin, SpdIn->Ddr5.Base.tWRmin.Bits.tWRmin);
                } else {
                  // LPDDR5
                  SdramWidthIndex = SpdIn->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                  Calculated = GetLpddr5tWR (tCKmin, SdramWidthIndex, Outputs->IsDvfscEnabled);
                }
              }
              break;
          } //switch


          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tWR = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum tWTR timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtWTR (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *SpdIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                LpCoreAcTiming;
  UINT8                 SdramWidthIndex;
  BOOLEAN               IsLpddr;
  BOOLEAN               IsLpddr5;
  MRC_LP5_BANKORG       Lp5BGOrg;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  IsLpddr         = Outputs->IsLpddr;
  IsLpddr5        = Outputs->IsLpddr5;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tWTRString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin         = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.tWTR > 0) {
                Calculated = DimmIn->Timing.tWTR;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (tCKmin > 0) {
                if (IsLpddr) {
                  SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                  SdramWidthIndex = SpdIn->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                  Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
                  if (IsLpddr5 && (Lp5BGOrg != MrcLp5BgMode)) {
                    if (!Outputs->IsDvfscEnabled) {
                      // 14ns for LP5 ByteMode, 12ns for LP5 x16
                      LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 14000000 : 12000000;
                    } else {
                      // 21ns for LP5 ByteMode, 19ns for LP5 x16
                      LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 21000000 : 19000000;
                    }
                  } else {  // There are no special values for E-DVFSC for BG mode
                    // 12ns for LP5 ByteMode, 10ns for LP5 x16
                    LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 12000000 : 10000000;
                  }

                  Calculated = DIVIDECEIL ((LpCoreAcTiming - (tCKmin / 100)), tCKmin);
                  //Check max 4nCK for LP5
                  LpCoreAcTiming = 4;
                  Calculated = MAX (Calculated, LpCoreAcTiming);
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tWTR = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tWTR_L timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtWTR_L (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcSpd          *SpdIn;
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT8                 SdramWidthIndex;
  UINT32                LpCoreAcTiming;
  MRC_LP5_BANKORG       Lp5BGOrg;
  UINT32                Index;
  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0 *Header5;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  if (Outputs->IsLpddr5) {
    Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
  } else {
    Lp5BGOrg = 0;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tWTRLString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          SpdIn   = &DimmIn->Spd.Data;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Header5 = &SpdIn->Ddr5.EndUser.Xmp.Header;
              if ((Header5->XmpSpecRevision.Data >= 0x12) && Inputs->IsXMP3Revision12Supported) {
                Index = Profile - XMP_PROFILE1;
                Calculated = PicoSecondsToClocks (SpdIn->Ddr5.EndUser.Xmp.Data[Index].tWTR_Lmin.Bits.tWTR_Lmin, tCKmin);
                Calculated = MAX(Calculated, SpdIn->Ddr5.EndUser.Xmp.Data[Index].tWTR_Lmin.Bits.LowerClockLimit);
                break;
              }
            case CUSTOM_PROFILE1:
              if ((CUSTOM_PROFILE1 == Profile) && (DimmIn->Timing.tWTR_L > 0)) {
                Calculated = DimmIn->Timing.tWTR_L;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // max(16nCK, 10ns)
                Calculated = (tCKmin == 0) ? 0 : (10000000 - (tCKmin / 100) + tCKmin - 1) / tCKmin; // 10ns
                Calculated = MAX(16, Calculated);
              } else if ((Outputs->IsLpddr5) && (Lp5BGOrg != MrcLp58Bank)) {
                SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                SdramWidthIndex = SpdIn->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                // 14ns for x8 LP5, 12ns for LP5 x16
                LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 14000000 : 12000000;
                Calculated = DIVIDECEIL ((LpCoreAcTiming - (tCKmin / 100)), tCKmin);
                //Check max 4nCK for LP5
                LpCoreAcTiming = 4;
                Calculated = MAX (Calculated, LpCoreAcTiming);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tWTR_L = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tWTR_S timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtWTR_S (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcSpd          *SpdIn;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT8                 SdramWidthIndex;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                LpCoreAcTiming;
  MRC_LP5_BANKORG       Lp5BGOrg;
  UINT32                Index;

  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0 *Header5;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tWTRSString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          SpdIn   = &DimmIn->Spd.Data;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Header5 = &SpdIn->Ddr5.EndUser.Xmp.Header;
              if ((Header5->XmpSpecRevision.Data >= 0x12) && Inputs->IsXMP3Revision12Supported) {
                Index = Profile - XMP_PROFILE1;
                Calculated = PicoSecondsToClocks (SpdIn->Ddr5.EndUser.Xmp.Data[Index].tWTR_Smin.Bits.tWTR_Smin, tCKmin);
                Calculated = MAX(Calculated, SpdIn->Ddr5.EndUser.Xmp.Data[Index].tWTR_Smin.Bits.LowerClockLimit);
                break;
              }
              /*FALLTHROUGH*/
            case CUSTOM_PROFILE1:
              if ((CUSTOM_PROFILE1 == Profile) && (DimmIn->Timing.tWTR_S > 0)) {
                Calculated = DimmIn->Timing.tWTR_S;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // Max(4nCK, 2.5ns)
                Calculated = (tCKmin == 0) ? 0 : (2500000 - (tCKmin / 100) + tCKmin - 1) / tCKmin; // 2.5ns
                Calculated = MAX(4, Calculated);
              } else if (DimmOut->DdrType == MRC_DDR_TYPE_LPDDR5) {
                Lp5BGOrg = MrcGetBankBgOrg (MrcData, Outputs->Frequency);
                if (Lp5BGOrg == MrcLp5BgMode) {
                  SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
                  SdramWidthIndex = SpdIn->Lpddr.Base.ModuleOrganization.Bits.SdramDeviceWidth;
                  // 8.25ns for LP5 ByteMode and LP5, 6.25ns for LP5 x16
                  LpCoreAcTiming = (SdramWidthIndex == MRC_SPD_SDRAM_DEVICE_WIDTH_8) ? 8250000 : 6250000;
                  Calculated = DIVIDECEIL((LpCoreAcTiming - (tCKmin / 100)), tCKmin);
                  //Check max 4nCK for LP5
                  LpCoreAcTiming = 4;
                  Calculated = MAX(Calculated, LpCoreAcTiming);
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case timing for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }

    Outputs->Timing[Profile].tWTR_S = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tCCD_L timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtCCD_L (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *SpdIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;

  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0 *Header5;
  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tCCDLString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          SpdIn   = &DimmIn->Spd.Data;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Header5 = &SpdIn->Ddr5.EndUser.Xmp.Header;
              if ((Header5->XmpSpecRevision.Data >= 0x12) && Inputs->IsXMP3Revision12Supported) {
                Index = Profile - XMP_PROFILE1;
                Calculated = PicoSecondsToClocks (SpdIn->Ddr5.EndUser.Xmp.Data[Index].tCCD_Lmin.Bits.tCCD_Lmin, tCKmin);
                Calculated = MAX(Calculated, SpdIn->Ddr5.EndUser.Xmp.Data[Index].tCCD_Lmin.Bits.LowerClockLimit);
                break;
              }
              /*FALLTHROUGH*/
            case CUSTOM_PROFILE1:
              if ((CUSTOM_PROFILE1 == Profile) && (DimmIn->Timing.tCCD_L > 0)) {
                Calculated = DimmIn->Timing.tCCD_L;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                // DDR5 tCCD_L = MAX (8nCK, 5ns)
                Calculated = PicoSecondsToClocks (5000, tCKmin);
                Calculated = MAX (8, Calculated);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tCCD_L  = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the tCCD_L_WR timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE Function completed.  This would be a void function but
    it appears in a table of functions that must return a BOOLEAN.
**/
static
BOOLEAN
GetChannelDimmtCCD_L_WR (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MrcSpd          *SpdIn;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                tCKmin;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT32                Index;

  const SPD_EXTREME_MEMORY_PROFILE_HEADER_3_0 *Header5;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", tCCDLWRString, HeaderString);

  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp (MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = 0;
    tCKmin = Outputs->Timing[Profile].tCK;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          SpdIn   = &DimmIn->Spd.Data;
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }

          Calculated     = 0;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Header5 = &SpdIn->Ddr5.EndUser.Xmp.Header;
              if ((Header5->XmpSpecRevision.Data >= 0x12) && Inputs->IsXMP3Revision12Supported) {
                Index = Profile - XMP_PROFILE1;
                Calculated = PicoSecondsToClocks (SpdIn->Ddr5.EndUser.Xmp.Data[Index].tCCD_L_WRmin.Bits.tCCD_L_WRmin, tCKmin);
                Calculated = MAX(Calculated, SpdIn->Ddr5.EndUser.Xmp.Data[Index].tCCD_L_WRmin.Bits.LowerClockLimit);
                break;
              }
              /*FALLTHROUGH*/
            case CUSTOM_PROFILE1:
              if ((CUSTOM_PROFILE1 == Profile) && (DimmIn->Timing.tCCD_L_WR > 0)) {
                Calculated = DimmIn->Timing.tCCD_L_WR;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Calculated = PicoSecondsToClocks (20000, tCKmin);
                Calculated = MAX (32, Calculated);
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].tCCD_L_WR  = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the minimum command rate mode value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE Function completed.  This would be a void function but
    it appears in a table of functions that must return a BOOLEAN.
**/
BOOLEAN
GetChannelDimmNmode (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput        *Inputs;
  const MrcControllerIn *ControllerIn;
  const MrcChannelIn    *ChannelIn;
  const MrcDimmIn       *DimmIn;
  const MRC_EXT_INPUTS_TYPE *ExtInputs;
  MrcDebug              *Debug;
  MrcOutput             *Outputs;
  MrcControllerOut      *ControllerOut;
  MrcChannelOut         *ChannelOut;
  MrcDimmOut            *DimmOut;
  MrcProfile            Profile;
  UINT8                 Controller;
  UINT8                 Channel;
  UINT8                 Dimm;
  UINT32                Actual[MAX_PROFILE];
  UINT32                Calculated;
  UINT8                 MinAllowedNMode;

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s:\n  %s\n", NmodeString, HeaderString);

  MinAllowedNMode = (Inputs->IsDdrIoDtHalo && (Outputs->GearMode == 0)) ? CA_1_NMODE : CA_2_NMODE;
  if (Inputs->MinAllowedNModeOvrd != 0) {
    // Only CA_1_NMODE and CA_2_NMODE are allowed.
    MinAllowedNMode = MIN(Inputs->MinAllowedNModeOvrd, CA_2_NMODE);
  }
  //
  // Find the smallest timing value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = (Profile < XMP_PROFILE1) ? MinAllowedNMode : 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (XmpSupport (DimmOut, Profile)) {
                Calculated = MinAllowedNMode;
                if (DimmOut->DdrType == MRC_DDR_TYPE_DDR5) {
                  Calculated = DimmIn->Spd.Data.Ddr5.EndUser.Xmp.Data[Profile - XMP_PROFILE1].SystemCmdRate.Bits.NMode;
                  if (ExtInputs->OCSafeMode) {
                    MinAllowedNMode = CA_2_NMODE;
                  }
                  Calculated = RANGE(Calculated, MinAllowedNMode, 2);
                }
              }
              break;
            case CUSTOM_PROFILE1:
              if (DimmIn->Timing.NMode > 0) {
                Calculated = DimmIn->Timing.NMode;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
            Calculated = CA_2_NMODE;
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %6u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Outputs->Timing[Profile].NMode = (UINT16) Actual[Profile];

    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the VDD voltage value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE Function completed.  This would be a void function but
    it appears in a table of functions that must return a BOOLEAN.
**/
BOOLEAN
GetChannelDimmVdd (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput                        *Inputs;
  MrcDebug                              *Debug;
  MrcOutput                             *Outputs;
  const MrcSpd                          *Spd;
  const MrcControllerIn                 *ControllerIn;
  const MrcChannelIn                    *ChannelIn;
  const MrcDimmIn                       *DimmIn;
  const MRC_EXT_INPUTS_TYPE             *ExtInputs;
  MrcControllerOut                      *ControllerOut;
  MrcChannelOut                         *ChannelOut;
  MrcDimmOut                            *DimmOut;
  MrcProfile                            Profile;
  UINT32                                Actual[MAX_PROFILE];
  UINT32                                Calculated;
  UINT8                                 Controller;
  UINT8                                 Channel;
  UINT8                                 Dimm;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_3_0  *Data3;
  UINT32                                      Index;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s %s:\n  %s\n", VoltageString, VddString, HeaderString);

  //
  // Find the best case voltage value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = (Profile < XMP_PROFILE1) ? VDD_0_50 : 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd = &DimmIn->Spd.Data;
          Calculated = VDD_1_50;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Index = Profile - XMP_PROFILE1;
              if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                Data3       = &Spd->Ddr5.EndUser.Xmp.Data[Index];
                Calculated  = XMP_VDD_INCREMENT * Data3->Vdd.Bits.Decimal;
                Calculated  = MIN (Calculated, XMP_VDD_INTEGER - 1);
                Calculated += (XMP_VDD_INTEGER * Data3->Vdd.Bits.Integer);
                Calculated  = MAX (Calculated, XMP3_VDD_MIN_POSSIBLE);
                Calculated  = MIN (Calculated, XMP3_VDD_MAX_POSSIBLE);
              }
              break;
            case CUSTOM_PROFILE1:
              if (ExtInputs->VddVoltage > 0) {
                Calculated = ExtInputs->VddVoltage;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (MRC_DDR_TYPE_LPDDR5 == DimmOut->DdrType) {
                Calculated = VDD_0_50;
              } else if (MRC_DDR_TYPE_DDR5 == DimmOut->DdrType) {
                if (ExtInputs->VddVoltage > 0) {
                  Calculated = ExtInputs->VddVoltage;
                } else {
                  Calculated = VDD_1_10;
                }
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %4u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case voltage for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          Outputs->VddVoltage[Profile] = (MrcVddSelect) MAX((UINT32)(Outputs->VddVoltage[Profile]), Actual[Profile]);
          if (MRC_DDR_TYPE_DDR5 == Outputs->DdrType) {
            /*
             * Some XMP memory might use 0 as VDD voltage.
             * MRC might use it later on, so use VDD_1_10
             */
            if (Outputs->VddVoltage[Profile] == VDD_INVALID) {
              Outputs->VddVoltage[Profile] = VDD_1_10;
            }
          }
          DimmOut->VddVoltage[Profile] = (MrcVddSelect) Actual[Profile];
        }
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the VDDQ voltage value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE Function completed.  This would be a void function but
    it appears in a table of functions that must return a BOOLEAN.
**/
static
BOOLEAN
GetChannelDimmVddq (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput                        *Inputs;
  const MRC_EXT_INPUTS_TYPE             *ExtInputs;
  const MrcControllerIn                 *ControllerIn;
  const MrcChannelIn                    *ChannelIn;
  const MrcDimmIn                       *DimmIn;
  const MrcSpd                          *Spd;
  MrcDebug                              *Debug;
  MrcOutput                             *Outputs;
  MrcControllerOut                      *ControllerOut;
  MrcChannelOut                         *ChannelOut;
  MrcDimmOut                            *DimmOut;
  MrcProfile                            Profile;
  UINT32                                Actual[MAX_PROFILE];
  UINT32                                Calculated;
  UINT8                                 Controller;
  UINT8                                 Channel;
  UINT8                                 Dimm;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_3_0  *Data3;
  UINT32                                      Index;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;

  if (Outputs->DdrType != MRC_DDR_TYPE_DDR5) {
    return TRUE;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s %s:\n  %s\n", VoltageString, VddqString, HeaderString);

  //
  // Find the best case voltage value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = (Profile < XMP_PROFILE1) ? VDD_0_50 : 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd        = &DimmIn->Spd.Data;
          Calculated = VDD_1_50;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Index = Profile - XMP_PROFILE1;
              Data3       = &Spd->Ddr5.EndUser.Xmp.Data[Index];
              Calculated  = XMP_VDD_INCREMENT * Data3->Vddq.Bits.Decimal;
              Calculated  = MIN (Calculated, XMP_VDD_INTEGER - 1);
              Calculated += (XMP_VDD_INTEGER * Data3->Vddq.Bits.Integer);
              Calculated  = MAX (Calculated, XMP3_VDD_MIN_POSSIBLE);
              Calculated  = MIN (Calculated, XMP3_VDD_MAX_POSSIBLE);
              break;
            case CUSTOM_PROFILE1:
              if (ExtInputs->VddqVoltage > 0) {
                Calculated = ExtInputs->VddqVoltage;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (ExtInputs->VddqVoltage > 0) {
                Calculated = ExtInputs->VddqVoltage;
              } else {
                Calculated = VDD_1_10;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %4u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case voltage for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          Outputs->VddqVoltage[Profile] = (MrcVddSelect) MAX((UINT32)(Outputs->VddqVoltage[Profile]), Actual[Profile]);
          if (MRC_DDR_TYPE_DDR5 == Outputs->DdrType) {
            /*
             * Some XMP memory might use 0 as VDDQ voltage.
             * MRC might use it later on, so use VDD_1_10
             */
            if (Outputs->VddqVoltage[Profile] == VDD_INVALID) {
              Outputs->VddqVoltage[Profile] = VDD_1_10;
            }
          }
          DimmOut->VddqVoltage[Profile] = (MrcVddSelect) Actual[Profile];
        }
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Calculate the VPP voltage value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
static
BOOLEAN
GetChannelDimmVpp (
  IN OUT MrcParameters *const MrcData
  )
{
  const MrcInput                        *Inputs;
  const MRC_EXT_INPUTS_TYPE             *ExtInputs;
  const MrcControllerIn                 *ControllerIn;
  const MrcChannelIn                    *ChannelIn;
  const MrcDimmIn                       *DimmIn;
  const MrcSpd                          *Spd;
  MrcDebug                              *Debug;
  MrcOutput                             *Outputs;
  MrcControllerOut                      *ControllerOut;
  MrcChannelOut                         *ChannelOut;
  MrcDimmOut                            *DimmOut;
  MrcProfile                            Profile;
  UINT32                                Actual[MAX_PROFILE];
  UINT32                                Calculated;
  UINT8                                 Controller;
  UINT8                                 Channel;
  UINT8                                 Dimm;

  const SPD_EXTREME_MEMORY_PROFILE_DATA_3_0  *Data3;
  UINT32                                      Index;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;
  ExtInputs = Inputs->ExtInputs.Ptr;

  if (Outputs->DdrType != MRC_DDR_TYPE_DDR5) {
    return TRUE;
  }

  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  %s %s:\n  %s\n", VoltageString, VppString, HeaderString);

  //
  // Find the best case voltage value for all the given DIMMs, for all the profiles.
  //
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    Actual[Profile] = (Profile < XMP_PROFILE1) ? VDD_0_50 : 0;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn  = &Inputs->Controller[Controller];
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn   = &ControllerIn->Channel[Channel];
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmIn  = &ChannelIn->Dimm[Dimm];
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (DIMM_PRESENT != DimmOut->Status) {
            continue;
          }
          Spd        = &DimmIn->Spd.Data;
          Calculated = VDD_1_50;
          switch (Profile) {
            case XMP_PROFILE1:
            case XMP_PROFILE2:
            case XMP_PROFILE3:
            case USER_PROFILE4:
            case USER_PROFILE5:
              Calculated = 0;
              if (!XmpSupport (DimmOut, Profile)) {
                break;
              }
              Index = Profile - XMP_PROFILE1;
              Data3       = &Spd->Ddr5.EndUser.Xmp.Data[Index];
              Calculated  = XMP_VDD_INCREMENT * Data3->Vpp.Bits.Decimal;
              Calculated  = MIN (Calculated, XMP_VDD_INTEGER - 1);
              Calculated += (XMP_VDD_INTEGER * Data3->Vpp.Bits.Integer);
              Calculated  = MAX (Calculated, XMP3_VDD_MIN_POSSIBLE);
              Calculated  = MIN (Calculated, XMP3_VDD_MAX_POSSIBLE);
              break;
            case CUSTOM_PROFILE1:
              if (ExtInputs->VppVoltage > 0) {
                Calculated = ExtInputs->VppVoltage;
                break;
              } else {
                // In AUTO mode, so no break.
              }
              /*FALLTHROUGH*/
            case STD_PROFILE:
            default:
              if (ExtInputs->VppVoltage > 0) {
                Calculated = ExtInputs->VppVoltage;
              } else {
                Calculated = VDD_1_80;
              }
              break;
          } //switch

          Actual[Profile] = MAX (Actual[Profile], Calculated);
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "  %7u %10u %8u %5u %4u\n",
            Profile,
            Controller,
            Channel,
            Dimm,
            Calculated
          );
        } //Dimm
      } //Channel
    } //Controller
  } //Profile

  //
  // Set the best case voltage for all controllers/channels/dimms, for each profile.
  //
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "    %s%u:", BestCaseString, MAX_PROFILE - 1);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
    if (NeedIgnoreXmp(MrcData, Profile)) {
      continue;
    }
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          Outputs->VppVoltage[Profile] = (MrcVddSelect) MAX((UINT32)(Outputs->VppVoltage[Profile]), Actual[Profile]);
          DimmOut->VppVoltage[Profile] = (MrcVddSelect) Actual[Profile];
        }
      }
    }
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, " %u", Actual[Profile]);
  }
  MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "\n");

  return TRUE;
}

/**
  Analyze the given DIMM SPD data to determine DIMM presence and configuration.

    @param[in, out] MrcData - Pointer to MRC global data structure.
    @param[in] Controller   - Current controller number.
    @param[in] Channel      - Current channel number.
    @param[in] Dimm         - Current DIMM number.

    @retval mrcSuccess if DIMM is present otherwise mrcDimmNotExist.
**/
MrcStatus
SpdDimmRecognition (
  IN OUT MrcParameters *const MrcData,
  IN  const UINT8          Controller,
  IN  UINT8                Channel,
  IN  UINT8                Dimm
  )
{
  static const SpdRecogCallTable CallTable[] = {
    {ValidDimm},
    {ValidSdramDeviceWidth},
    {ValidPrimaryWidth},
    {GetRankCount},
    {ValidBank},
    {GetDimmSize},
    {ValidRowSize},
    {ValidColumnSize},
    {ValidEccSupport},
    {GetAddressMirror},
    {GetThermalRefreshSupport},
    {GetpTRRsupport},
    {GetReferenceRawCardSupport},
    {ValidCkdSupport}
  };
  const MrcSpd *Spd;
  const UINT8  *CrcStart;
  MrcDimmOut   *DimmOut;
  MrcDimmIn    *DimmIn;
  BOOLEAN      Status;
  UINT32       CrcSize;
  UINT8        Index;
  UINT16       Crc;

  DimmIn  = &MrcData->Inputs.Controller[Controller].Channel[Channel].Dimm[Dimm];
  Spd     = &DimmIn->Spd.Data;
  DimmOut = &MrcData->Outputs.Controller[Controller].Channel[Channel].Dimm[Dimm];
  DimmOut->Status = DIMM_NOT_PRESENT;
  if (DIMM_PRESENT == DimmPresence (&MrcData->Outputs.Debug, Spd)) {
    Status = TRUE;
    for (Index = 0; (Status == TRUE) && (Index < ARRAY_COUNT (CallTable)); Index++) {
      Status &= CallTable[Index].mrc_task (MrcData, Spd, DimmOut);
    }
    if (Status == FALSE) {
      DimmOut->Status = DIMM_DISABLED;
      return mrcDimmNotExist;
    }
    DimmOut->Status = DIMM_PRESENT;
    CrcStart = MrcSpdCrcArea (MrcData, Controller, Channel, Dimm, &CrcSize);
    GetDimmCrc ((const UINT8*const) CrcStart, CrcSize, &Crc);
    DimmOut->Crc = Crc;
  } else {
    return mrcDimmNotExist;
  }

  if (DIMM_DISABLED == DimmIn->Status) {
    DimmOut->Status = DIMM_DISABLED;
    DimmOut->Crc    = 0;
  }

  return mrcSuccess;
}

/**
  Calculate the timing of all DIMMs on all channels.

    @param[in, out] MrcData - The MRC "global data".

    @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
MrcStatus
SpdTimingCalculation (
  IN OUT MrcParameters *const MrcData
  )
{
  static const SpdTimeCallTable CallTable[] = {
    {GetChannelDimmTimeBase}, // Note: This must be done first as all other calculations are based on this.
    {GetChannelDimmtCK},      // Note: This must be done second as all other calculations are based on this.
    {GetChannelDimmtAA},
    {GetChannelDimmtCWL},
    {GetChannelDimmtRAS},
    {GetChannelDimmtRCD},
    {GetChannelDimmtRC},      // Note: This must be done after GetChannelDimmtRAS and GetChannelDimmtRCD
    {GetChannelDimmtREFI},
    {GetChannelDimmtRFC},
    {GetChannelDimmtRFCpb},
    {GetChannelDimmtRP},      // Note: This must be done after GetChannelDimmtRCD
    {GetChannelDimmtRPab},    // Note: This must be done after GetChannelDimmtRP
    {GetChannelDimmtFAW},
    {GetChannelDimmtRRD},
    {GetChannelDimmtRTP},
    {GetChannelDimmtWR},
    {GetChannelDimmtWTR},
    {GetChannelDimmtRFC2},
    {GetChannelDimmtRFC4},
    {GetChannelDimmtRRD_L},
    {GetChannelDimmtRRD_S},
    {GetChannelDimmtWTR_L},
    {GetChannelDimmtWTR_S},
    {GetChannelDimmtCCD_L},
    {GetChannelDimmtCCD_L_WR},
    {GetChannelDimmNmode},
    {GetChannelDimmVdd},
    {GetChannelDimmVddq},
    {GetChannelDimmVpp}
  };
  BOOLEAN    Status;
  UINT8      Index;

  //
  // Find the "least common denominator" timing across the DIMMs.
  // tAA must be done first before any other timings are calculated.
  //
  Status = TRUE;
  for (Index = 0; (Status == TRUE) && (Index < ARRAY_COUNT(CallTable)); Index++) {
    Status &= CallTable[Index].mrc_task (MrcData);
    if (Status != TRUE) {
      MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_ERROR, "%s failed step %d", __FUNCTION__, Index);
    }
  }

  return (Status == FALSE) ? mrcDimmNotExist : mrcSuccess;
}

/**
  Determine the starting address and size of the SPD area to generate a CRC.

    @param[in, out] MrcData    - The MRC "global data".
    @param[in]      Controller - Controller index.
    @param[in]      Channel    - Channel index.
    @param[in]      Dimm       - Dimm index.
    @param[out]     CrcSize    - Location to write CRC block size.

    @retval The starting address of the CRC block.
**/
const UINT8 *
MrcSpdCrcArea (
  IN OUT MrcParameters *const MrcData,
  IN     UINT8                Controller,
  IN     UINT8                Channel,
  IN     UINT8                Dimm,
  OUT    UINT32        *const CrcSize
  )
{
  const MrcDimmIn *DimmIn;
  const UINT8     *CrcStart;
  UINT8           DdrType;

  DimmIn   = &MrcData->Inputs.Controller[Controller].Channel[Channel].Dimm[Dimm];

  DdrType = DimmIn->Spd.Data.Ddr5.Base.DramDeviceType.Bits.Type;
  if (MRC_SPD_DDR5_SDRAM_TYPE_NUMBER == DdrType) {
    CrcStart = (void *) &DimmIn->Spd.Data.Ddr5.ManufactureInfo;
    *CrcSize = SPD5_MANUF_SIZE;
  } else if (MrcData->Outputs.IsLP5Camm2) {
    CrcStart = (void*) &DimmIn->Spd.Data.JedecLpddr5.ManufactureInfo;
    *CrcSize = SPDLP_JEDEC_SPEC_MANUF_SIZE;
  } else if ((MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER == DdrType) || (MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER == DdrType)) {
    CrcStart = (void *) &DimmIn->Spd.Data.Lpddr.ManufactureInfo;
    *CrcSize = SPDLP_MANUF_SIZE;
  } else {
    CrcStart = NULL;
    *CrcSize = 0;
  }
  return (CrcStart);
}

/**
  Check that DqsMapCpu2Dram has valid values.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcWrongInputParameter if these tables have invalid values.
**/
MrcStatus
MrcCheckLpddrMapping (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcDebug        *Debug;
  MrcInput        *Inputs;
  MrcChannelIn    *ChannelIn;
  MrcStatus       Status;
  MrcOutput       *Outputs;
  UINT32          Controller;
  UINT8           CpuByte;
  UINT8           DramByte;
  UINT8           Channel;
  UINT8           *DqsMapCpu2Dram;
  UINT8           DqsMap;
  BOOLEAN         DqsMapCpu2DramGood;
  BOOLEAN         IsLpddr;

  Status  = mrcSuccess;
  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  IsLpddr = (Outputs->IsLpddr);
  DqsMapCpu2DramGood = TRUE;

  if (!IsLpddr) {
    return mrcSuccess;
  }

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      if (!MrcChannelExist (MrcData, Controller, Channel)) {
        continue;
      }
      ChannelIn = &Inputs->Controller[Controller].Channel[Channel];
      DqsMapCpu2Dram = ChannelIn->DqsMapCpu2Dram[dDIMM0]; // Lpddr only uses DIMM0 entry
      DqsMap = 0;

      for (CpuByte = 0; CpuByte < MAX_BYTE_IN_LP_CHANNEL; CpuByte++) {
        DramByte = DqsMapCpu2Dram[CpuByte];
        if (DramByte < MAX_BYTE_IN_LP_CHANNEL) {
          DqsMap |= (1 << DramByte);
        }
      } // for CpuByte
      if (DqsMap != ((1 << MAX_BYTE_IN_LP_CHANNEL) - 1)) {
        DqsMapCpu2DramGood = FALSE;
      }
    } // Channel
  } // Controller

  if (!DqsMapCpu2DramGood) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR,"%s input array %s has invalid values !\n", gErrString, "DqsMapCpu2Dram");
    Status = mrcWrongInputParameter;
  }

  return Status;
}

/**
  Check that some MRC input parameters make sense for the current DDR type.

  @param[in] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcWrongInputParameter if parameters have invalid values.
**/
MrcStatus
MrcCheckInputParams (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcStatus             Status;

  Status = MrcCheckLpddrMapping (MrcData);

  return Status;
}

/**
  Find the vendor-specific swizzling configuration

  @param[in]  MrcData   - Pointer to Spd data
  @param[in]  Controller     - Current Controller
  @param[in]  Channel        - Current Channel
  @param[in]  Dimm           - Current Dimm
  @param[in]  DimmMap        - Dimm Bit Map of the dimm
  @param[in][out] MA1Swizzle - MA1_Swizzling
  @param[in][out] MA2Swizzle - MA2_Swizzling
**/
VOID
MrcGetVendorSwizzling (
  IN MrcParameters *const MrcData,
  IN UINT8             Controller,
  IN UINT8             Channel,
  IN UINT8             Dimm,
  IN UINT8             DimmMap,
  IN OUT UINT32        *MA1Swizzle,
  IN OUT UINT32        *MA2Swizzle
  )
{
  MrcInput                          *Inputs;
  MrcOutput                         *Outputs;
  MrcSpd                            *Spd;
  MrcDimmOut                        *DimmOut;
  UINT8 DensityIndex;
  UINT8                             MR6[MRC_MRR_ARRAY_SIZE];
  UINT8                             MR7[MRC_MRR_ARRAY_SIZE];

  Inputs  = &MrcData->Inputs;
  Outputs = &MrcData->Outputs;

  Spd = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
  DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm];

  DensityIndex = DimmOut->DensityIndex;

  // Search is based on Density and DieRev.
  // 2 Dimm must be configured with same value, so we OR 0x3
  switch (DimmOut->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      if ((Spd->Ddr5.ManufactureInfo.DramIdCode.Data == MICRON_DRAM_ID) ||
            (Spd->Ddr5.ManufactureInfo.ModuleId.IdCode.Data == MICRON_DRAM_ID)) {
        // Don't check DramStepping for DDR5, partially because DramStepping is often 0 in SPD
        *MA1Swizzle |= DimmMap;
        *MA2Swizzle |= DimmMap;
      }

      if ((Spd->Ddr5.ManufactureInfo.DramIdCode.Data == SAMSUNG_DRAM_ID) ||
            (Spd->Ddr5.ManufactureInfo.ModuleId.IdCode.Data == SAMSUNG_DRAM_ID)) {
        if (Spd->Ddr5.ManufactureInfo.DramStepping == 0x95) {
          *MA1Swizzle |= DimmMap;
          *MA2Swizzle |= DimmMap;
        }
        if (Spd->Ddr5.ManufactureInfo.DramStepping == 0x45) {
          *MA2Swizzle |= DimmMap;
        }
        if (Spd->Ddr5.ManufactureInfo.DramStepping == 0x4D) {
          *MA2Swizzle |= DimmMap;
        }
      }
      break;


    case MRC_DDR_TYPE_LPDDR5:
      if ((Spd->Lpddr.ManufactureInfo.DramIdCode.Data == MICRON_DRAM_ID) ||
            (Spd->Lpddr.ManufactureInfo.ModuleId.IdCode.Data == MICRON_DRAM_ID)) {
        // Don't check DramStepping for LPDDR, partially because DramStepping is often 0 in SPD
        if (DensityIndex == MRC_SPD_SDRAM_DENSITY_8Gb) {
          *MA1Swizzle |= DimmMap;
          *MA2Swizzle |= DimmMap;
        }
      }

      if ((Spd->Lpddr.ManufactureInfo.DramIdCode.Data == SAMSUNG_DRAM_ID) ||
            (Spd->Lpddr.ManufactureInfo.ModuleId.IdCode.Data == SAMSUNG_DRAM_ID)) {
        // Lp5 always has rank0. thus issue MRR on Rank0,
        // and Rank1 has same MrrResult.
        MrcIssueMrr (MrcData, Controller, Channel, 0, mrMR6, MR6);
        MrcIssueMrr (MrcData, Controller, Channel, 0, mrMR7, MR7);
        // assume all Dram chips have same stepping, check device 0.
        if (MR6[0] == 0x8 && !Outputs->LpX) {
          *MA1Swizzle |= DimmMap;
          *MA2Swizzle |= DimmMap;
        }
        if (MR6[0] == 0xA && MR7[0] == 0x0 && Outputs->LpX) {
          *MA2Swizzle |= DimmMap;
        }
      }
      break;


    default:
      break;
  }
}

/**
  Get the DDR Type for the first populated channel and populate the type
  specific parameters in the global data structure.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess
**/
MrcStatus
MrcGetSpdDdrTypeParams (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcDebug                      *Debug;
  const MrcInput                *Inputs;
  const MrcControllerIn         *ControllerIn;
  const MrcChannelIn            *ChannelIn;
  const MrcDimmIn               *DimmIn;
  const MrcSpd                  *SpdIn;
  MrcOutput                     *Outputs;
  UINT8                         Controller;
  UINT8                         Channel;
  UINT8                         MaxChannel;
  UINT8                         Dimm;
  UINT8                         MaxDimm;
  UINT8                         DeviceType;
  UINT8                         RankCount;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;

  Outputs->DdrType   = MRC_DDR_TYPE_UNKNOWN;
  Outputs->MaxDqBits = 0;
  MaxDimm            = MAX_DIMMS_IN_CHANNEL;
  MaxChannel         = MAX_CHANNEL;
  RankCount             = 0;
  Outputs->Is2RankDdr5  = FALSE;

  //
  // Scan Dimms for first valid DdrType
  //
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn = &Inputs->Controller[Controller];
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      ChannelIn = &ControllerIn->Channel[Channel];
      for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
        DimmIn = &ChannelIn->Dimm[Dimm];
        SpdIn = &DimmIn->Spd.Data;
        DeviceType = SpdIn->Ddr5.Base.DramDeviceType.Bits.Type; // Device Type for all DdrTypes is in the same location
        if (DimmPresence (Debug, SpdIn) == DIMM_PRESENT) {
          switch (DeviceType) {
           case MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER:
           case MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER:
             Outputs->DdrType   = MRC_DDR_TYPE_LPDDR5;
             Outputs->IsLpddr   = TRUE;
             Outputs->IsLpddr5  = TRUE;
             Outputs->IsDdr5    = FALSE;
             Outputs->MaxDqBits = 16;
             MaxDimm = 1;
             break;

           case MRC_SPD_DDR5_SDRAM_TYPE_NUMBER:
             Outputs->DdrType   = MRC_DDR_TYPE_DDR5;
             Outputs->IsLpddr   = FALSE;
             Outputs->IsLpddr5  = FALSE;
             Outputs->IsDdr5    = TRUE;
             Outputs->MaxDqBits = 32 + NUM_ECC_BITS;
             MaxDimm = 2;
             MaxChannel = 2;
             if (!Outputs->Is2RankDdr5) {
               RankCount = SpdIn->Ddr5.ModuleCommon.ModuleOrganization.Bits.RankCount;
               RankCount += 1;
               Outputs->Is2RankDdr5 = (RankCount == 2) ? TRUE : FALSE;
             }
             break;
          }
        }
      }
    }
  }

  // Now we know the memory types across the system set MaxChannel based on DDR Type
  Outputs->MaxChannels = MaxChannel;
  // Now we know the memory types across the system set MaxDimm based on DDR Type
  Outputs->MaxDimm = MaxDimm;

  return mrcSuccess;
}

/**
  Process the SPD information for all DIMMs on all channels.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
MrcStatus
MrcSpdProcessingStatic (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcDebug                      *Debug;
  const MrcInput                *Inputs;
  const MrcControllerIn         *ControllerIn;
  const MrcChannelIn            *ChannelIn;
  const MrcDimmIn               *DimmIn;
  const MrcSpd                  *SpdIn;
  MrcOutput                     *Outputs;
  MrcControllerOut              *ControllerOut;
  MrcChannelOut                 *ChannelOut;
  MrcDimmOut                    *DimmOut;
  MrcStatus                     Status;
  UINT32                        DimmCount;
  UINT8                         Controller;
  UINT8                         Channel;
  UINT8                         MaxChannel;
  UINT8                         Dimm;
  UINT8                         MaxDimm;
  UINT8                         ExpectedWidth;
  const SPD5_MANUFACTURING_DATA *Ddr5ManufactureData;
  MrcSaveData                   *SaveData;
  UINT8                         FirstDimmRankInDimm = 0;
  UINT8                         CurDimmRankInDimm = 0;
  UINT8                         FirstDimmSdramWidth = 0;
  UINT8                         CurDimmSdramWidth = 0;
  BOOLEAN                       IsFirstDimm;

  Inputs   = &MrcData->Inputs;
  Outputs  = &MrcData->Outputs;
  Debug    = &Outputs->Debug;
  SaveData = &MrcData->Save.Data;

  Status             = mrcDimmNotExist;
  MaxDimm            = Outputs->MaxDimm;
  MaxChannel         = Outputs->MaxChannels;

  //
  // Scan thru each DIMM to see if it is a valid DIMM and to get its configuration.
  //
  DimmCount  = 0;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn  = &Inputs->Controller[Controller];
    ControllerOut = &Outputs->Controller[Controller];
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      ChannelIn   = &ControllerIn->Channel[Channel];
      ChannelOut  = &ControllerOut->Channel[Channel];
      ChannelOut->DimmCount = 0;
      for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
        DimmIn  = &ChannelIn->Dimm[Dimm];
        DimmOut = &ChannelOut->Dimm[Dimm];
        if (DimmIn->Status == DIMM_ENABLED || DimmIn->Status == DIMM_DISABLED) {
          MRC_DEBUG_MSG (
            Debug,
            MSG_LEVEL_NOTE,
            "SPD Dimm recognition, %s %u/%u/%u\n",
            CcdString,
            Controller,
            Channel,
            Dimm
            );
          if (mrcSuccess == SpdDimmRecognition (MrcData, Controller, Channel, Dimm)) {
            DimmCount++;
            ChannelOut->DimmCount++;
            if (ChannelOut->DimmCount == 2) {
              Outputs->IsAny2Dpc = TRUE;
            }
            if (Dimm == 1) {
              Outputs->IsDimm1Enabled = TRUE;
            }
            if (Outputs->DdrType == MRC_DDR_TYPE_UNKNOWN) {
              Outputs->DdrType = DimmOut->DdrType;
            } else if (Outputs->DdrType != DimmOut->DdrType) {
              Status = mrcMixedDimmSystem;
            }
            if (Status == mrcMixedDimmSystem) {
              MRC_DEBUG_MSG (
                Debug,
                MSG_LEVEL_ERROR,
                "%s configuration, system contains a mix of memory types\n",
                ErrorString
                );
              return (Status);
            }
          }
        }
      }
    }
  }

  Outputs->SystemDimmCount = DimmCount;

  // Check for mixed PrimaryBusWidth - for LP5 only
  if (Outputs->IsLpddr) {
    ExpectedWidth = MRC_SPD_PRIMARY_BUS_WIDTH_16;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
          if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Status == DIMM_PRESENT) {
            SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
            if (SpdIn->Lpddr.Base.ModuleMemoryBusWidth.Bits.PrimaryBusWidth != ExpectedWidth) {
              Status = mrcMixedDimmSystem;
              MRC_DEBUG_MSG (Debug, MSG_LEVEL_ERROR, "%s Mixed PrimaryBusWidth across DRAMs!\n", gErrString);
              return (Status);
            }
          }
        }
      }
    }
  }

  if (Inputs->IsDdrIoDtHalo) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        ChannelOut = &ControllerOut->Channel[Channel];
        IsFirstDimm = TRUE;
        for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
          DimmOut = &ChannelOut->Dimm[Dimm];
          if (Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Status == DIMM_PRESENT) {
            if (IsFirstDimm) {
              FirstDimmRankInDimm = DimmOut->RankInDimm;
              FirstDimmSdramWidth = DimmOut->SdramWidth;
              IsFirstDimm = FALSE;
              if (Dimm == 1) {
                if (FirstDimmRankInDimm == 1) {
                  Outputs->Is2SPC0R1R = TRUE;
                } else if (FirstDimmRankInDimm == 2) {
                  Outputs->Is2SPC0R2R = TRUE;
                }
              } else if ((ChannelOut->DimmCount == 1) && (Dimm == 0)) {
                if (FirstDimmRankInDimm == 1) {
                  Outputs->Is1DPC1R = TRUE;
                } else if (FirstDimmRankInDimm == 2) {
                  Outputs->Is1DPC2R = TRUE;
                }
              }
            } else {
              CurDimmRankInDimm = DimmOut->RankInDimm;
              CurDimmSdramWidth = DimmOut->SdramWidth;
              if (FirstDimmSdramWidth == CurDimmSdramWidth) {
                if ((FirstDimmRankInDimm == MAX_RANK_IN_DIMM) && (CurDimmRankInDimm == MAX_RANK_IN_DIMM)) {
                  Outputs->Is2DPC2R2R = TRUE;
                } else if ((FirstDimmRankInDimm == (MAX_RANK_IN_DIMM - 1)) && (CurDimmRankInDimm == (MAX_RANK_IN_DIMM - 1))) {
                  Outputs->Is2DPC1R1R = TRUE;
                }
              }
            }
          }
        }
      }
    }
  }
  if (DimmCount > 0) {
    Status = mrcSuccess;
    if (Outputs->IsDdr5) {
      Outputs->BurstLength = 8;
    } else {
      // LPDDR5
      // Burst length is 32.  tCK in 4:1 is 8 UI's per clock
      Outputs->BurstLength = 4;
    }
  }

  if (Outputs->IsDdr5) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerOut = &Outputs->Controller[Controller];
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        ChannelOut  = &ControllerOut->Channel[Channel];
        for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
          DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm];
          if (DimmOut->Status == DIMM_PRESENT) {

            SpdIn = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
            Ddr5ManufactureData = &SpdIn->Ddr5.ManufactureInfo;
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Controller/Channel/Dimm: %u/%u/%u\n", Controller, Channel, Dimm);
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  DramIdCode: 0x%x\n", SpdIn->Ddr5.ManufactureInfo.DramIdCode.Data);
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  ModuleId: 0x%x\n", SpdIn->Ddr5.ManufactureInfo.ModuleId.IdCode.Data);
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  DramStepping: %u\n", SpdIn->Ddr5.ManufactureInfo.DramStepping);
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  DateCode: 0x%x\n", (SpdIn->Ddr5.ManufactureInfo.ModuleId.Date.Year << 8) |
                                                                         SpdIn->Ddr5.ManufactureInfo.ModuleId.Date.Week);
            MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Density: %u\n", SpdIn->Ddr5.Base.PrimarySdramDensityAndPackage.Bits.Density);

            if ((Ddr5ManufactureData->DramIdCode.Data == SKHYNIX_DRAM_ID) ||
                (Ddr5ManufactureData->ModuleId.IdCode.Data == SKHYNIX_DRAM_ID)) {
              SaveData->IsDdr5Hynix = TRUE;
            }
          }
        }
      }
    }
  }

  return Status;
}

/**
  Process the SPD information for all DIMMs with no platform frequency limits
  and populate the MaxDimmFreq host struct field.

    @param[in, out] MrcData - The MRC "global data".

    @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
MrcStatus
MrcSpdProcessingMaxFreqCheck (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcStatus Status;
  MrcOutput *Outputs;

  Outputs = &MrcData->Outputs;

  if ((Outputs->MaxDimmFreq != 0) && !Outputs->IsOCProfile) {
    // Only check if the value isn't already known
    // This should run only on the first GV point
    return mrcSuccess;
  }

  // Outputs->MemoryClockMax is zero at this point, the only frequency limit will be the DIMM SPD timings.
  MRC_DEBUG_ASSERT (Outputs->MemoryClockMax == 0, &MrcData->Outputs.Debug, "MemoryClockMax=%d", Outputs->MemoryClockMax);
  Status = SpdTimingCalculation (MrcData);
  if (Status == mrcSuccess) {
    Outputs->MaxDimmFreq = Outputs->Frequency;
    MRC_DEBUG_MSG (&MrcData->Outputs.Debug, MSG_LEVEL_NOTE, "MaxDimmFreq=%d\n", Outputs->MaxDimmFreq);
  }
  return Status;
}

/**
  Process the SPD information for all DIMMs on all channels.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
MrcStatus
MrcSpdProcessingCalc (
  IN OUT MrcParameters *const MrcData
  )
{
  MrcDebug                      *Debug;
  MrcOutput                     *Outputs;
  MrcControllerOut              *ControllerOut;
  MrcChannelOut                 *ChannelOut;
  MrcDimmOut                    *DimmOut;
  MrcStatus                     Status;
  UINT8                         Controller;
  UINT8                         Channel;
  UINT8                         Rank;
  UINT8                         MaxChannel;
  UINT8                         Dimm;
  UINT8                         MaxDimm;
  UINT8                         ValidRankBitMask;
  BOOLEAN                       Xmp1DPC;
  BOOLEAN                       IsNonCkd;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  Status  = mrcDimmNotExist;

  Outputs->tMAC      = MRC_TMAC_UNLIMITED;
  MaxDimm            = Outputs->MaxDimm;
  MaxChannel         = Outputs->MaxChannels;
  IsNonCkd           = FALSE;

  // Perform an inital timing calculation with no frequency limits to get the
  // max frequency supported by the current DIMM population
  if (MrcSpdProcessingMaxFreqCheck (MrcData) != mrcSuccess) {
    return mrcFail;
  }

  //
  // Get the maximum allowed frequency / refclk
  //
  Status = MrcMcCapabilityPreSpd (MrcData);

  if (Outputs->SystemDimmCount > 0) {
    //
    // Scan thru each channel to see if it is a valid channel and to get its configuration.
    //
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "SPD Dimm timing calculation\n");
    if (mrcSuccess == SpdTimingCalculation (MrcData)) {
      Outputs->EccSupport = TRUE;
      Outputs->IsCkdSupported = FALSE;
      Outputs->ValidMcBitMask = 0;
      Outputs->ValidChBitMask = 0;
      Outputs->ValidRankMask  = 0;
      Outputs->FirstPopController = MAX_CONTROLLER;

      //
      // Count up the number of valid DIMMs.
      //
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        ControllerOut = &Outputs->Controller[Controller];
        ControllerOut->FirstPopCh = MAX_CHANNEL;
        for (Channel = 0; Channel < MaxChannel; Channel++) {
          Xmp1DPC = FALSE;
          ChannelOut = &ControllerOut->Channel[Channel];
          for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
            DimmOut = &ChannelOut->Dimm[Dimm];
            if (DIMM_PRESENT == DimmOut->Status) {
#if (MAX_RANK_IN_CHANNEL > 8)
#error The next switch statement and ValidRankBitMask needs updated to support additional ranks.
#endif
              switch (DimmOut->RankInDimm) {
                case 1:
                  ValidRankBitMask = 1;
                  break;
#if (MAX_RANK_IN_DIMM > 1)

                case 2:
                  ValidRankBitMask = 3;
                  break;
#endif
#if (MAX_RANK_IN_DIMM > 2)

                case 3:
                  ValidRankBitMask = 7;
                  break;
#endif
#if (MAX_RANK_IN_DIMM > 3)

                case 4:
                  ValidRankBitMask = 15;
                  break;
#endif

                default:
                  ValidRankBitMask = 0;
                  break;
              }

              ChannelOut->ValidRankBitMask |= ValidRankBitMask << (Dimm * MAX_RANK_IN_DIMM);
              Outputs->EccSupport  &= DimmOut->EccSupport;
              Outputs->IsCkdSupported |= DimmOut->IsCkdSupport;
              IsNonCkd |= (DimmOut->IsCkdSupport == 0 ? TRUE : FALSE);
              Outputs->tMAC         = MIN (Outputs->tMAC, DimmOut->tMAC);
              if ((XmpSupport (DimmOut, XMP_PROFILE1) && (DimmOut->XmpProfile1Config == 0)) ||
                  (XmpSupport (DimmOut, XMP_PROFILE2) && (DimmOut->XmpProfile2Config == 0)) ||
                  (XmpSupport (DimmOut, XMP_PROFILE3) && (DimmOut->XmpProfile3Config == 0))) {
                Xmp1DPC = TRUE;
              }
            } // DIMM_PRESENT
          } // Dimm

          if ((ChannelOut->DimmCount > 0) && (ChannelOut->ValidRankBitMask > 0)) {
            ControllerOut->ChannelCount++;
            ControllerOut->Channel[Channel].Status = CHANNEL_PRESENT;
            if ((Outputs->XmpProfileEnable != 0) && Xmp1DPC && ((ChannelOut->DimmCount > 1))) {
              MRC_DEBUG_MSG (Debug,
                  MSG_LEVEL_WARNING,
                  "Warning: MC%u C%u: XMP memory config is 1DPC, but is populated with 2DPC\n",
                  Controller,
                  Channel);
              Outputs->XmpConfigWarning = TRUE;
            }
            // Initialize ValidChBitMask and ValidRankMask used during all training steps
            ControllerOut->ValidChBitMask |= (1 << Channel);
            ControllerOut->FirstPopCh = MIN (Channel, ControllerOut->FirstPopCh);
            Outputs->ValidChBitMask |= (ControllerOut->ValidChBitMask << (Controller * Outputs->MaxChannels));
            Outputs->ValidRankMask  |= ChannelOut->ValidRankBitMask;
            MRC_DEBUG_MSG (
              Debug,
              MSG_LEVEL_NOTE,
              "Mc%u.C%u: ValidRankBitMask=0x%x, Overall ValidRankMask=0x%x\n",
              Controller,
              Channel,
              ChannelOut->ValidRankBitMask,
              Outputs->ValidRankMask
            );
          }
        } // Channel

        if (ControllerOut->ChannelCount > 0) {
          ControllerOut->Status = CONTROLLER_PRESENT;
          Status                = mrcSuccess;
          Outputs->ValidMcBitMask |= (1 << Controller);
          Outputs->FirstPopController = MIN ((UINT8) Controller, Outputs->FirstPopController);
        }
        MRC_DEBUG_MSG (
          Debug,
          MSG_LEVEL_NOTE,
          "Mc%u: ValidChBitMask=0x%x\n",
          Controller,
          ControllerOut->ValidChBitMask
          );
      }
    }
  }
  Outputs->McChBitMask = Outputs->ValidChBitMask;
  MRC_DEBUG_MSG (
    Debug,
    MSG_LEVEL_NOTE,
    "ValidMcBitMask=0x%x, Overall ValidChBitMask=0x%x\n",
    Outputs->ValidMcBitMask,
    Outputs->ValidChBitMask
  );

#if defined(FULL_HEADLESS) && !defined(NEW_STUB)
  MrcStubWriteMrcData (MrcData); // sending data early for sync-server.exe
#endif

  // Disable CkdSupport to have CKD DIMM as default bypass mode if system contains mixture of CKD and non CKD DIMMs
  if (Outputs->IsCkdSupported && IsNonCkd) {
    MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "Disabling CKD due to not all DIMMs are CKD supported\n");
    Outputs->IsCkdSupported = FALSE;
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel = 0; Channel < MaxChannel; Channel++) {
        for (Dimm = 0; Dimm < MaxDimm; Dimm++) {
          DimmOut = &Outputs->Controller[Controller].Channel[Channel].Dimm[Dimm];
          if (DIMM_PRESENT == DimmOut->Status) {
            DimmOut->IsCkdSupport = FALSE;
          }
        }
      }
    }
  }

  ValidRankBitMask = 0;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MaxChannel; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        ValidRankBitMask |= MrcRankExist (MrcData, Controller, Channel, Rank);
      }
    }
  }

  Outputs->MaxRanks = MrcCountBitsEqOne (ValidRankBitMask);

  if (Status != mrcSuccess) {
    return Status;
  }

  Status = MrcCheckInputParams (MrcData);

  return Status;
}

/**
  This function calculates the DDR5 timings tCL which takes into account SOC requirements

  @param[in]     MrcData      - Include all MRC global data.
**/
VOID
MrcUpdateDdr5MintCL (
  IN MrcParameters *const MrcData
  )
{
  MrcOutput        *Outputs;
  MrcDebug         *Debug;
  MrcProfile       Profile;
  BOOLEAN          IsGear4;
  BOOLEAN          IsUpdated;

  Outputs = &MrcData->Outputs;
  Debug   = &Outputs->Debug;
  IsGear4 = (Outputs->GearMode == 1);
  IsUpdated = FALSE;

  if (!Outputs->IsDdr5) {
    return;
  }
  // MRC must ensure the smallest RL in DDR5 Gear4 is 28.
  // This RL can occur in 3200 and 3600 JEDEC bins.
  // DDR5-3600 doesn't have 28 in official JEDEC speed bins, hence limit to 30
  if (IsGear4 && ((Outputs->Frequency == f3200) || (Outputs->Frequency == f3600))) {
    for (Profile = STD_PROFILE; Profile < MAX_PROFILE; Profile++) {
      if (NeedIgnoreXmp (MrcData, Profile)) {
        continue;
      }
      if ((Outputs->Frequency == f3200) && (Outputs->Timing[Profile].tCL < 28)) {
        Outputs->Timing[Profile].tCL = 28;     // 28 is the smallest valid CL in DDR5 3200G4.
        IsUpdated = TRUE;
       }
       if ((Outputs->Frequency == f3600) && (Outputs->Timing[Profile].tCL < 30)) {
         Outputs->Timing[Profile].tCL = 30;     // 30 is the smallest valid CL in DDR5 3600G4.
         IsUpdated = TRUE;
       }
       if (IsUpdated) {
         MRC_DEBUG_MSG (Debug, MSG_LEVEL_NOTE, "  Profile:%d New tCL: %u\n", Profile, Outputs->Timing[Profile].tCL);
       }
    }
  }
}
