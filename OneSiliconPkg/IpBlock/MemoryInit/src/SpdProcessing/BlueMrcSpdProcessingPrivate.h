/** @file
  Prototype information for private/internal functions used for
  SPD (Serial Presence Detection) processing.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef BLUE_MRC_SPD_PROCESSING_PRIVATE_H
#define BLUE_MRC_SPD_PROCESSING_PRIVATE_H


#include "CMrcInterface.h" // For MrcParameters


// DO NOT REMOVE: Ensures this header is only included once in the code
PRIVATE_HEADER_DEFENDER(BlueSpdProcessingPrivate)

typedef struct {
  UINT8 Count;
  UINT8 SpdAddress[MAX_DIMMS_IN_SYSTEM];
  BOOLEAN IsSwizzled;
} PhysicalDimmConfig;

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
  );


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
  );



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
  );


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
  );


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
  );


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
  );


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
  );


/**
  Converts the input DDR5 SPD Byte 4 Bits 4:0 Density value to a DensityIndex
  value that can be used to index into the SdramCapacityTable array.

    @param[in] Dddr5Density - The DDR5 SDRAM Density Per Die per the DDR5 SPD spec

    @return The density as an index of SdramCapacityTable or 0xFF if the input is undefined.
**/
UINT8
Ddr5SpdToDensityIndex (
  IN UINT8 Ddr5Density
  );


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
  );


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
  );


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
  );


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
  );


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
  );



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
  );

/**
  Obtain CKD support Status for this DIMM.

    @param[in, out] MrcData - Pointer to MrcData data structure.
    @param[in]      Spd     - Pointer to Spd data structure.
    @param[in, out] DimmOut - Pointer to structure containing DIMM information.

    @retval TRUE Function completed.  This would be a void function but
    it appears in a table of functions that must return a BOOLEAN.
**/
BOOLEAN
ValidCkdSupport (
  IN OUT MrcParameters* const MrcData,
  IN const MrcSpd* const Spd,
  IN OUT MrcDimmOut* const DimmOut
  );

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
  );


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
  );


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
  );


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
  );


/**
  Calculate the medium and fine timebases, using integer math.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if medium timebase is valid, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmTimeBase (
  IN OUT MrcParameters  *const MrcData
  );


/**
  Calculate the SDRAM minimum cycle time (tCKmin) that this DIMM supports.
    Then use the lookup table to obtain the frequency closest to the clock multiple.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if the DIMM frequency is supported, otherwise FALSE and the frequency is set to fInvalid.
**/
BOOLEAN
GetChannelDimmtCK (
  IN OUT MrcParameters  *const MrcData
  );


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
  );


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
  );


/**
  Calculate the tCWL value for DDR5.

    @param[in] tCL - current tCL in DCLKs.

    @retval DDR5 tCWL in DCLK units
**/
UINT32
GetDdr5tCWL (
  IN const UINT32 tCL
  );

/**
  Calculate the minimum tCWL timing value for the given memory frequency.
    We calculate timings for all profiles so that this information can be passed out of MRC.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtCWL (
  IN OUT MrcParameters *const MrcData
  );


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
  );


/**
  Calculate the minimum tFAW timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtFAW (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRAS timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRAS (
  IN OUT MrcParameters *const MrcData
  );


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
  );


/**
  Calculate the minimum tRCD timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRCD (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tREFI timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtREFI (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRFC timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRFC (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRFCpb timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRFCpb (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRFC2 timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRFC2 (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRFC4 timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRFC4 (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRP timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRP (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRPab timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRPab (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRRD timing value for the given memory frequency.
    MRC should not set tRRD below 4nCK for all frequencies.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE
**/
BOOLEAN
GetChannelDimmtRRD (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRRD same bank group timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRRD_L (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tRRD different bank group timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRRD_S (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the tRTP value for DDR5.
  JEDEC Spec: tRTP = 7.5ns

    @param[in] tCK - the memory DCLK in femtoseconds.

    @retval DDR5 tRTP in DCLK units
**/
UINT32
GetDdr5tRTP (
  IN const UINT32 tCK
  );


/**
  Calculate the tWR value for LPDDR5
  JEDEC Spec: 36 ns.

    @param[in] tCK              - the memory DCLK in femtoseconds.
    @param[in] SdramWidthIndex  - 1 for X8 and 2 for X16 SDRAM width

    @retval tWR in DCLK units
**/
UINT32
GetLpddr5tWR (
  IN const UINT32     tCK,
  IN UINT8            SdramWidthIndex,
  IN BOOLEAN          IsDvfscEnabled
  );


/**
  Calculate the minimum tRTP timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtRTP (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tWR timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtWR (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the minimum tWTR timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtWTR (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the tWTR_L timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtWTR_L (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the tWTR_S timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtWTR_S (
  IN OUT MrcParameters *const MrcData
  );



/**
  Calculate the tCCD_L timing value for the given memory frequency.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtCCD_L (
  IN OUT MrcParameters *const MrcData
  );

/**
  Calculate the tCCD_L_WR timing value for the given memory frequency.

  @param[in, out] MrcData - Pointer to MrcData data structure.

  @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmtCCD_L_WR (
  IN OUT MrcParameters *const MrcData
  );

/**
  Calculate the minimum command rate mode value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmNmode (
  IN OUT MrcParameters *const MrcData
  );


/**
  Calculate the VDD voltage value for the given channel.

    @param[in, out] MrcData - Pointer to MrcData data structure.

    @retval TRUE if there are DIMMs present, otherwise FALSE.
**/
BOOLEAN
GetChannelDimmVdd (
  IN OUT MrcParameters *const MrcData
  );


/**
  Analyze the given DIMM SPD data to determine DIMM presence and configuration.

    @param[in, out] MrcData - Pointer to MRC global data structure.
    @param[in] Controller   - Current controller number.
    @param[in] Channel      - Current channel number.
    @param[in] Dimm         - Current DIMM number.

    @retval mrcSuccess if DIMM is present, mrcUnsupportedTechnology if DIMM technology is not supported
            otherwise mrcDimmNotExist.
**/
MrcStatus
SpdDimmRecognition (
  IN OUT MrcParameters *const MrcData,
  IN  const UINT8          Controller,
  IN  UINT8                Channel,
  IN  UINT8                Dimm
  );


/**
  Calculate the timing of all DIMMs on all channels.

    @param[in, out] MrcData - The MRC "global data".

    @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
MrcStatus
SpdTimingCalculation (
  IN OUT MrcParameters *const MrcData
  );


/**
  Check that DqsMapCpu2Dram has valid values.

  @param[in, out] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcWrongInputParameter if these tables have invalid values.
**/
MrcStatus
MrcCheckLpddrMapping (
  IN OUT MrcParameters *const MrcData
  );


/**
  Check that some MRC input parameters make sense for the current DDR type.

  @param[in] MrcData - The MRC "global data".

  @retval mrcSuccess on success, mrcWrongInputParameter if parameters have invalid values.
**/
MrcStatus
MrcCheckInputParams (
  IN OUT MrcParameters *const MrcData
  );

/**
  Process the SPD information for all DIMMs with no platform frequency limits
  and populate the MaxDimmFreq host struct field.

    @param[in, out] MrcData - The MRC "global data".

    @retval mrcSuccess on success, mrcDimmNotExist if no DIMMs found.
**/
MrcStatus
MrcSpdProcessingMaxFreqCheck (
  IN OUT MrcParameters *const MrcData
  );

/**
  This function get physcial DIMM count in MemSS mounted in the first slot on channel and
  checks if DIMMs are swizzled.

  @param[in] MrcData is a pointer to MrcData data structure.

  @returns Physical DIMM0 configuration in the system.
**/
PhysicalDimmConfig
MrcGetPhysicalDimmConfig (
  IN MrcParameters *const MrcData
  );

/**
  This function sets MptuChannelMap to the default one without swizzling .

  @param[in] MrcData is a pointer to MrcData data structure.

  @returns None.
**/
VOID
MrcSetDefaultChannelToMptuSwizzleMap (
  IN MrcParameters *const MrcData
  );

/**
 * This function sets MptuChannelMap for DDR5.

  @param[in] MrcData is a pointer to MrcData data structure.

  @returns None.
**/
VOID
MrcSetDdr5ChannelToMptuSwizzleMap (
  IN MrcParameters *const MrcData
  );

  /*
  Print MptuChannelMap in the system.

  @param[in] MrcData is a pointer to MrcData data structure.
**/
VOID
PrintSystemToMptuChannelMap (
  IN MrcParameters *const MrcData
  );

  /**
  This function sets ValidMptuChBitMask inside MrcOutput structure based on
  MptuChannelMap.

  @param[in] MrcData is a pointer to MrcData data structure.

  @returns None.
**/
VOID
SetValidMptuChBitMasks (
  IN MrcParameters *const MrcData
  );

#endif // BLUE_MRC_SPD_PROCESSING_PRIVATE_H
