/** @file
  IP Common Services Interface (CSI)

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.
**/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IP_CSI_REG_H_
#define IP_CSI_REG_H_

  #include "IpWrapper.h"
  #include "IpWrapperDfx.h"
  #include "IpCsiRegDeprecated.h"

// IMPORTANT:
// - all CSI and Wrapper types must be defined in IpCsi.h.  Function declarations will continue
//   to be defined in the appropriate H files
// - please see comments in IpCsi.h for further explanation

// ****************** Easy Register Macros **************************

/**
  Concatenation pre-processor macros
  NOTE: never use internal CAT().  Always use XCAT()
  @param[in]      X 1st param
  @param[in]      X 2nd param

  @retval         XCAT(X,Y) returns XY
**/
#define CAT(X, Y)   X##Y
#define XCAT(X, Y)  CAT (X, Y)

/**
  string pre-processor macros
  NOTE: never use internal _XSTR().  Always use XSTR()
  @param[in]      X 1st param

  @retval         XSTR(X) returns "X"
**/
#define _XSTR(X)  #X
#define XSTR(X)   _XSTR (X)

/**
  The following parameters are consistently used in these macros and defined here once:

  @param[in]  PINST:       the IP's instance pointer
  @param[in]  REG:         the shortened register name. The shortened name is used with appended suffixes to extract information from the register H files.
                            - Example: "REG1" expands to "REG1_REG" to find the register offset in the H file
  @param[in]  DATA/DATAx:  data to be written to the reg/field
  @param[in]  DATA1HILO:   data written to FLD1HILO.  Treated like DATA/DATAx
  @param[in]  FLD/FLDx:    field to be read/written.  The name is used to directly access the member of REG_STRUCT structure
  @param[in]  FLD1HILO:    field that is split into FLD1HILO_lo/FLD1HILO_hi when spans 32bit boundary.  Treated like FLD/FLDx
  @param[in]  FLAGS:       OR'ed flags in [IP_WR_REG_FLAGS].  Macros automatically determine "IpWrRegFlagSize*" and logically OR with other passed FLAGS
  @param[in]  CINDEX:      this is the index into array of [IP_WR_REG_CNTXT] when needed by some registers.
  @param[in]  REGTEMPLATE: this is equivalent to REG, but REGTEMPLATE is used as a template for registers at REGOFFSET
  @param[in]  REGOFFSET:   this is the calculated offsets instead of offsets defined by looking up REG in the H file.
**/

#define REG_CNTXT(REG)  XCAT (REG, _ENDPOINT)
#define REG_CINDEX(REG, CINDEX) \
  XCAT (REG_CNTXT (REG), List)  \
  [CINDEX] // register context with index
#define REG_OFFSET(REG)  XCAT (REG, _REG)
#define REG_STRUCT(REG)  XCAT (REG, _STRUCT)
#define REG_BITS(REG)    XCAT (REG, _WIDTH)

/**
  Polling Register read. This will continue to read the REG.FLD until
  one of the following criteria is met:
    - (REG.FLD == DATA)
    - MAXRETRY is exceeded

  @param[in]      PINST instance
  @param[in]      REG the shortened register name. The shortened name is used with appended suffixes
                  to extract information from the register H files.
  @param[in]      FLD the field name from struct in reg H file
  @param[in]      DATA data to wait for in REG.FLD
  @param[in]      MAXRETRY # of retry before timeout. 0 loops forever
  @param[in]      USECDELAY usec delay per retry. May be 0
  @param[out]     PCSISTS Good=IpCsiStsSuccess, timeout=IpCsiStsErrorTimeout. Optional (ie may be NULL)

  @retval         none
**/
#define REG_POLL_FLD_US(PINST, REG, FLD, DATA, MAXRETRY, USECDELAY, PCSISTS)                                   \
  do {                                                                                                         \
    REG_STRUCT (REG)                                                                                           \
    _Mask;                                                                                                     \
    _Mask.Data     = 0;                                                                                        \
    _Mask.Data     = ~_Mask.Data;                                                                              \
    _Mask.Bits.FLD = 0;                                                                                        \
    _Mask.Data     = ~_Mask.Data;                                                                              \
    REG_STRUCT (REG)                                                                                           \
    _Match;                                                                                                    \
    _Match.Data     = 0;                                                                                       \
    _Match.Bits.FLD = DATA;                                                                                    \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA); \
    IpWrRegPollUs (                                                                                            \
      (PINST)->REG_CNTXT (REG),                                                                                \
      REG_OFFSET (REG),                                                                                        \
      REG_BITS (REG),                                                                                          \
      _Mask.Data,                                                                                              \
      _Match.Data,                                                                                             \
      MAXRETRY,                                                                                                \
      (PINST)->TimeCntxt,                                                                                      \
      USECDELAY,                                                                                               \
      PCSISTS);                                                                                                \
  } while (0)

/**
  Polling Register read. This will continue to read the REG.FLD until
  one of the following criteria is met:
      - (REG.FLD == DATA1)
      - (REG.FLD == DATA2)
      - MAXRETRY is exceeded

  @param[in]      PINST instance
  @param[in]      REG the shortened register name. The shortened name is used with appended suffixes
                  to extract information from the register H files.
  @param[in]      FLD the field name from struct in reg H file
  @param[in]      DATA1 data to wait for in REG.FLD
  @param[in]      DATA2 data to wait for in REG.FLD
  @param[in]      MAXRETRY # of retry before timeout. 0 loops forever
  @param[in]      USECDELAY usec delay per retry. May be 0
  @param[out]     PCSISTS Good=IpCsiStsSuccess, timeout=IpCsiStsErrorTimeout. Optional (ie may be NULL)

  @retval         none
**/
#define REG_POLL_FLD_2VAL_US(PINST, REG, FLD, DATA1, DATA2, MAXRETRY, USECDELAY, PCSISTS)                       \
  do {                                                                                                          \
    REG_STRUCT (REG)                                                                                            \
    _Mask;                                                                                                      \
    _Mask.Data     = 0;                                                                                         \
    _Mask.Data     = ~_Mask.Data;                                                                               \
    _Mask.Bits.FLD = 0;                                                                                         \
    _Mask.Data     = ~_Mask.Data;                                                                               \
    REG_STRUCT (REG)                                                                                            \
    _Match1;                                                                                                    \
    _Match1.Data     = 0;                                                                                       \
    _Match1.Bits.FLD = DATA1;                                                                                   \
    REG_STRUCT (REG)                                                                                            \
    _Match2;                                                                                                    \
    _Match2.Data     = 0;                                                                                       \
    _Match2.Bits.FLD = DATA2;                                                                                   \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA1); \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA2); \
    IpWrRegPoll2ValUs (                                                                                         \
      (PINST)->REG_CNTXT (REG),                                                                                 \
      REG_OFFSET (REG),                                                                                         \
      REG_BITS (REG),                                                                                           \
      _Mask.Data,                                                                                               \
      _Match1.Data,                                                                                             \
      _Match2.Data,                                                                                             \
      MAXRETRY,                                                                                                 \
      (PINST)->TimeCntxt,                                                                                       \
      USECDELAY,                                                                                                \
      PCSISTS);                                                                                                 \
  } while (0)

/**
  Polling Register read. This will continue to read the REG.FLD until
  one of the following criteria is met:
    - (REG.FLD == DATA)
    - MAXRETRY is exceeded

  @param[in]      PINST instance
  @param[in]      REG the shortened register name. The shortened name is used with appended suffixes
                  to extract information from the register H files.
  @param[in]      FLD the field name from struct in reg H file
  @param[in]      DATA data to wait for in REG.FLD
  @param[in]      MAXRETRY # of retry before timeout. 0 loops forever
  @param[in]      NSECDELAY nanosec delay per retry. May be 0
  @param[out]     PCSISTS Good=IpCsiStsSuccess, timeout=IpCsiStsErrorTimeout. Optional (ie may be NULL)

  @retval         none
**/
#define REG_POLL_FLD_NS(PINST, REG, FLD, DATA, MAXRETRY, NSECDELAY, PCSISTS)                                   \
  do {                                                                                                         \
    REG_STRUCT (REG)                                                                                           \
    _Mask;                                                                                                     \
    _Mask.Data     = 0;                                                                                        \
    _Mask.Data     = ~_Mask.Data;                                                                              \
    _Mask.Bits.FLD = 0;                                                                                        \
    _Mask.Data     = ~_Mask.Data;                                                                              \
    REG_STRUCT (REG)                                                                                           \
    _Match;                                                                                                    \
    _Match.Data     = 0;                                                                                       \
    _Match.Bits.FLD = DATA;                                                                                    \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA); \
    IpWrRegPollNs (                                                                                            \
      (PINST)->REG_CNTXT (REG),                                                                                \
      REG_OFFSET (REG),                                                                                        \
      REG_BITS (REG),                                                                                          \
      _Mask.Data,                                                                                              \
      _Match.Data,                                                                                             \
      MAXRETRY,                                                                                                \
      (PINST)->TimeCntxt,                                                                                      \
      NSECDELAY,                                                                                               \
      PCSISTS);                                                                                                \
  } while (0)

/**
  Polling Register read. This will continue to read the REG.FLD until
  one of the following criteria is met:
    - (REG.FLD == DATA1)
    - (REG.FLD == DATA2)
    - MAXRETRY is exceeded

  @param[in]      PINST instance
  @param[in]      REG the shortened register name. The shortened name is used with appended suffixes
                  to extract information from the register H files.
  @param[in]      FLD the field name from struct in reg H file
  @param[in]      DATA1 data to wait for in REG.FLD
  @param[in]      DATA2 data to wait for in REG.FLD
  @param[in]      MAXRETRY # of retry before timeout. 0 loops forever
  @param[in]      NSECDELAY nanosec delay per retry. May be 0
  @param[out]     PCSISTS Good=IpCsiStsSuccess, timeout=IpCsiStsErrorTimeout. Optional (ie may be NULL)

  @retval         none
**/
#define REG_POLL_FLD_2VAL_NS(PINST, REG, FLD, DATA1, DATA2, MAXRETRY, NSECDELAY, PCSISTS)                       \
  do {                                                                                                          \
    REG_STRUCT (REG)                                                                                            \
    _Mask;                                                                                                      \
    _Mask.Data     = 0;                                                                                         \
    _Mask.Data     = ~_Mask.Data;                                                                               \
    _Mask.Bits.FLD = 0;                                                                                         \
    _Mask.Data     = ~_Mask.Data;                                                                               \
    REG_STRUCT (REG)                                                                                            \
    _Match1;                                                                                                    \
    _Match1.Data     = 0;                                                                                       \
    _Match1.Bits.FLD = DATA1;                                                                                   \
    REG_STRUCT (REG)                                                                                            \
    _Match2;                                                                                                    \
    _Match2.Data     = 0;                                                                                       \
    _Match2.Bits.FLD = DATA2;                                                                                   \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA1); \
    _REG_DFX_FLD (PINST, FLAGS, (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD, DATA2); \
    IpWrRegPoll2ValNs (                                                                                         \
      (PINST)->REG_CNTXT (REG),                                                                                 \
      REG_OFFSET (REG),                                                                                         \
      REG_BITS (REG),                                                                                           \
      _Mask.Data,                                                                                               \
      _Match1.Data,                                                                                             \
      _Match2.Data,                                                                                             \
      MAXRETRY,                                                                                                 \
      (PINST)->TimeCntxt,                                                                                       \
      NSECDELAY,                                                                                                \
      PCSISTS);                                                                                                 \
  } while (0)

/**
REG_READ()
-  derive all of the required register information from REG
-  read REG from derived [IP_WR_REG_CNTXT]

  @param[in]      PINST instance
  @param[in]      REG the shortened register name. The shortened name is used with appended suffixes
                  to extract information from the register H files.

  @retval         data read
**/
#define REG_READ(PINST, REG) \
  REG_READ_FLAG (PINST, 0, REG)

/**
  REG_WRITE()
  -  derive all of the required register information from REG
  -  write REG from derived [IP_WR_REG_CNTXT]

  @param[in]      PINST instance
  @param[in]      REG the shortened register name. The shortened name is used with appended suffixes
                  to extract information from the register H files.
  @param[in]      DATA data to write to REG

  @retval         data written
**/
#define REG_WRITE(PINST, REG, DATA) \
  REG_WRITE_FLAG (PINST, 0, REG, DATA)

/**
  REG_READ_FLD()
  -  derive all of the required register information from REG
  -  read REG.FLD from derived [IP_WR_REG_CNTXT]

  @param[in]      PINST instance
  @param[in]      REG the shortened register name. The shortened name is used with appended suffixes
                  to extract information from the register H files.
  @param[in]      FLD the field name from struct in reg H file

  @retval         read REG.FLD
**/
#define REG_READ_FLD(PINST, REG, FLD) \
  REG_READ_FLD_FLAG (PINST, 0, REG, FLD)

/**
  REG_RW()
  -  derive all of the required register information from REG
  -  read REG from derived [IP_WR_REG_CNTXT] and then write the data back to REG

  @param[in]      PINST instance
  @param[in]      REG the shortened register name. The shortened name is used with appended suffixes
                  to extract information from the register H files.

  @retval         data written
**/
#define REG_RW(PINST, REG) \
  REG_RW_FLAG (PINST, 0, REG)

/**
  REG_RMW_FLD#()
  -  derive all of the required register information from REG
  -  read REG from derived [IP_WR_REG_CNTXT]
  -  modify specified fields FLD1-FLDx with DATA1-DATAx.
  -  preserve non-specified fields
  -  write new register value to REG

  @param[in] PINST: the IP's instance pointer
  @param[in] REG: the shortened register name. The shortened name is used with appended suffixes to extract information from the register H files.
                  Example: "REG1" expands to "REG1_REG" to find the register offset in the H file
  @param[in] DATA/DATAx: data to be written to the reg/field
  @param[in] FLD/FLDx: field to be read/written
  @param[in] FLAGS: OR'ed flags in [IP_WR_REG_FLAGS].  Macros automatically determine "IpWrRegFlagSize*" and logically OR with other passed FLAGS
  @param[in] CINDEX: this is the index into array of [IP_WR_REG_CNTXT] when needed by some registers.
  @param[in] REGTEMPLATE: this is equivalent to REG, but REGTEMPLATE is used as a template for registers at REGOFFSET
  @param[in] REGOFFSET: this is the calculated offsets instead of offsets defined by looking up REG in the H file.

  @retval         none
**/
#define REG_RMW_FLD1(PINST, REG, FLD1, DATA1) \
  REG_RMW_FLD1_FLAG (PINST, 0, REG, FLD1, DATA1)

#define REG_RMW_FLD2(PINST, REG, FLD1, DATA1, FLD2, DATA2) \
  REG_RMW_FLD2_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2)

#define REG_RMW_FLD3(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  REG_RMW_FLD3_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_RMW_FLD4_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_RMW_FLD5_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_RMW_FLD6(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_RMW_FLD6_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_RMW_FLD7_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_RMW_FLD8(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  REG_RMW_FLD8_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

/**
  REG_WRITE_FLD#_ZERO()
  -  derive all of the required register information from REG
  -  set specified fields FLD1-FLDx with DATA1-DATAx.
  -  set non-specified fields to ZERO
  -  write new register value to REG in derived [IP_WR_REG_CNTXT]

  @param[in] PINST: the IP's instance pointer
  @param[in] REG: the shortened register name. The shortened name is used with appended suffixes to extract information from the register H files.
                  Example: "REG1" expands to "REG1_REG" to find the register offset in the H file
  @param[in] DATA/DATAx: data to be written to the reg/field
  @param[in] FLD/FLDx: field to be read/written
  @param[in] FLAGS: OR'ed flags in [IP_WR_REG_FLAGS].  Macros automatically determine "IpWrRegFlagSize*" and logically OR with other passed FLAGS
  @param[in] CINDEX: this is the index into array of [IP_WR_REG_CNTXT] when needed by some registers.
  @param[in] REGTEMPLATE: this is equivalent to REG, but REGTEMPLATE is used as a template for registers at REGOFFSET
  @param[in] REGOFFSET: this is the calculated offsets instead of offsets defined by looking up REG in the H file.

  @retval         none
**/
#define REG_WRITE_FLD1_ZERO(PINST, REG, FLD1, DATA1) \
  REG_WRITE_FLD1_ZERO_FLAG (PINST, 0, REG, FLD1, DATA1)

#define REG_WRITE_FLD2_ZERO(PINST, REG, FLD1, DATA1, FLD2, DATA2) \
  REG_WRITE_FLD2_ZERO_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2)

#define REG_WRITE_FLD3_ZERO(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  REG_WRITE_FLD3_ZERO_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_WRITE_FLD4_ZERO(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_WRITE_FLD4_ZERO_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_WRITE_FLD5_ZERO(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_WRITE_FLD5_ZERO_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_WRITE_FLD6_ZERO(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_WRITE_FLD6_ZERO_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_WRITE_FLD7_ZERO(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_WRITE_FLD7_ZERO_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_WRITE_FLD8_ZERO(PINST, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  REG_WRITE_FLD8_ZERO_FLAG (PINST, 0, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

/**
  REG_*_FLAG()
  -  these are same as macros without the "_FLAG" suffix, except for the FLAGS param

  @param[in] PINST: the IP's instance pointer
  @param[in] REG: the shortened register name. The shortened name is used with appended suffixes to extract information from the register H files.
                  Example: "REG1" expands to "REG1_REG" to find the register offset in the H file
  @param[in] DATA/DATAx: data to be written to the reg/field
  @param[in] FLD/FLDx: field to be read/written
  @param[in] FLAGS: OR'ed flags in [IP_WR_REG_FLAGS].  Macros automatically determine "IpWrRegFlagSize*" and logically OR with other passed FLAGS
  @param[in] CINDEX: this is the index into array of [IP_WR_REG_CNTXT] when needed by some registers.
  @param[in] REGTEMPLATE: this is equivalent to REG, but REGTEMPLATE is used as a template for registers at REGOFFSET
  @param[in] REGOFFSET: this is the calculated offsets instead of offsets defined by looking up REG in the H file.

  @retval         same as macros without the "_FLAG" suffix
**/
#define REG_READ_FLAG(PINST, FLAGS, REG) \
  REG_READ_RAW (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG))

#define REG_WRITE_FLAG(PINST, FLAGS, REG, DATA) \
  REG_WRITE_RAW (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), DATA)

#define REG_READ_FLD_FLAG(PINST, FLAGS, REG, FLD) \
  REG_READ_FLD_RAW (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD)

#define REG_RW_FLAG(PINST, FLAGS, REG) \
  REG_WRITE_FLAG (PINST, FLAGS, REG, REG_READ_FLAG (PINST, FLAGS, REG))

// REG_RMW_FLD*_FLAG()
#define REG_RMW_FLD1_FLAG(PINST, FLAGS, REG, FLD1, DATA1) \
  _REG_IMW_RAW1 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_FLAG (PINST, FLAGS, REG), FLD1, DATA1)

#define REG_RMW_FLD2_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_FLAG (PINST, FLAGS, REG), FLD1, DATA1, FLD2, DATA2)

#define REG_RMW_FLD3_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_FLAG (PINST, FLAGS, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_FLAG (PINST, FLAGS, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_FLAG (PINST, FLAGS, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_RMW_FLD6_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_FLAG (PINST, FLAGS, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_FLAG (PINST, FLAGS, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_RMW_FLD8_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_FLAG (PINST, FLAGS, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

// REG_WRITE_FLD*_ZERO_FLAG()
#define REG_WRITE_FLD1_ZERO_FLAG(PINST, FLAGS, REG, FLD1, DATA1) \
  _REG_IMW_RAW1 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1)

#define REG_WRITE_FLD2_ZERO_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2)

#define REG_WRITE_FLD3_ZERO_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_WRITE_FLD4_ZERO_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_WRITE_FLD5_ZERO_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_WRITE_FLD6_ZERO_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_WRITE_FLD7_ZERO_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_WRITE_FLD8_ZERO_FLAG(PINST, FLAGS, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

/**
  REG_*_CINDEX()
  -  these are same as macros without the "_CINDEX" suffix, except for the CINDEX param

  @param[in] PINST: the IP's instance pointer
  @param[in] REG: the shortened register name. The shortened name is used with appended suffixes to extract information from the register H files.
                  Example: "REG1" expands to "REG1_REG" to find the register offset in the H file
  @param[in] DATA/DATAx: data to be written to the reg/field
  @param[in] FLD/FLDx: field to be read/written
  @param[in] FLAGS: OR'ed flags in [IP_WR_REG_FLAGS].  Macros automatically determine "IpWrRegFlagSize*" and logically OR with other passed FLAGS
  @param[in] CINDEX: this is the index into array of [IP_WR_REG_CNTXT] when needed by some registers.
  @param[in] REGTEMPLATE: this is equivalent to REG, but REGTEMPLATE is used as a template for registers at REGOFFSET
  @param[in] REGOFFSET: this is the calculated offsets instead of offsets defined by looking up REG in the H file.

  @retval         same as macros without the "_CINDEX" suffix
**/
#define REG_READ_CINDEX(PINST, CINDEX, REG) \
  REG_READ_CINDEX_FLAG (PINST, 0, CINDEX, REG)

#define REG_WRITE_CINDEX(PINST, CINDEX, REG, DATA) \
  REG_WRITE_CINDEX_FLAG (PINST, 0, CINDEX, REG, DATA)

#define REG_READ_FLD_CINDEX(PINST, CINDEX, REG, FLD) \
  REG_READ_FLD_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD)

#define REG_RW_CINDEX(PINST, CINDEX, REG) \
  REG_RW_CINDEX_FLAG (PINST, 0, CINDEX, REG)

// REG_RMW_FLD*_CINDEX
#define REG_RMW_FLD1_CINDEX(PINST, CINDEX, REG, FLD1, DATA1) \
  REG_RMW_FLD1_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1)

#define REG_RMW_FLD2_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2) \
  REG_RMW_FLD2_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1, FLD2, DATA2)

#define REG_RMW_FLD3_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  REG_RMW_FLD3_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_RMW_FLD4_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_RMW_FLD5_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_RMW_FLD6_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_RMW_FLD6_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_RMW_FLD7_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_RMW_FLD8_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  REG_RMW_FLD8_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

// REG_WRITE_FLD*_ZERO_CINDEX
#define REG_WRITE_FLD1_ZERO_CINDEX(PINST, CINDEX, REG, FLD1, DATA1) \
  REG_WRITE_FLD1_ZERO_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1)

#define REG_WRITE_FLD2_ZERO_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2) \
  REG_WRITE_FLD2_ZERO_CINDEX_FLAG (PINST, 0, CINDEX, REG, FLD1, DATA1, FLD2, DATA2)

#define REG_WRITE_FLD3_ZERO_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  REG_WRITE_FLD3_ZERO_CINDEX_FLAG (PINST, 0, CINDEX, REG,  FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_WRITE_FLD4_ZERO_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_WRITE_FLD4_ZERO_CINDEX_FLAG (PINST, 0, CINDEX, REG,  FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_WRITE_FLD5_ZERO_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_WRITE_FLD5_ZERO_CINDEX_FLAG (PINST, 0, CINDEX, REG,  FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_WRITE_FLD6_ZERO_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_WRITE_FLD6_ZERO_CINDEX_FLAG (PINST, 0, CINDEX, REG,  FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_WRITE_FLD7_ZERO_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_WRITE_FLD7_ZERO_CINDEX_FLAG (PINST, 0, CINDEX, REG,  FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_WRITE_FLD8_ZERO_CINDEX(PINST, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  REG_WRITE_FLD8_ZERO_CINDEX_FLAG (PINST, 0, CINDEX, REG,  FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

/**
  REG_*_CINDEX_FLAG()
  -  these are same as macros without the "_CINDEX_FLAG" suffix, except for the FLAGS param

  @param[in] PINST: the IP's instance pointer
  @param[in] REG: the shortened register name. The shortened name is used with appended suffixes to extract information from the register H files.
                  Example: "REG1" expands to "REG1_REG" to find the register offset in the H file
  @param[in] DATA/DATAx: data to be written to the reg/field
  @param[in] FLD/FLDx: field to be read/written
  @param[in] FLAGS: OR'ed flags in [IP_WR_REG_FLAGS].  Macros automatically determine "IpWrRegFlagSize*" and logically OR with other passed FLAGS
  @param[in] CINDEX: this is the index into array of [IP_WR_REG_CNTXT] when needed by some registers.
  @param[in] REGTEMPLATE: this is equivalent to REG, but REGTEMPLATE is used as a template for registers at REGOFFSET
  @param[in] REGOFFSET: this is the calculated offsets instead of offsets defined by looking up REG in the H file.

  @retval         same as macros without the "_CINDEX_FLAG" suffix
**/
#define REG_READ_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG) \
  REG_READ_RAW (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG))

#define REG_WRITE_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, DATA) \
  REG_WRITE_RAW (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), DATA)

#define REG_READ_FLD_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD) \
  REG_READ_FLD_RAW (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD)

#define REG_RW_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG) \
  REG_WRITE_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG, REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG))

// REG_RMW_FLD*_CINDEX_FLAG
#define REG_RMW_FLD1_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1) \
  _REG_IMW_RAW1 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), FLD1, DATA1)

#define REG_RMW_FLD2_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), FLD1, DATA1, FLD2, DATA2)

#define REG_RMW_FLD3_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_RMW_FLD6_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_RMW_FLD8_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

// REG_WRITE_FLD*_ZERO_CINDEX_FLAG
#define REG_WRITE_FLD1_ZERO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1) \
  _REG_IMW_RAW1 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1)

#define REG_WRITE_FLD2_ZERO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2)

#define REG_WRITE_FLD3_ZERO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_WRITE_FLD4_ZERO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_WRITE_FLD5_ZERO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_WRITE_FLD6_ZERO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_WRITE_FLD7_ZERO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_WRITE_FLD8_ZERO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

/**
  REG_*_OFFSET()
  - these are same as macros without the "_OFFSET" suffix, except for the REGOFFSET param
  - this is used when same reg is duplicated many times at different offets (eg per lane config)

  @param[in] PINST: the IP's instance pointer
  @param[in] REG: the shortened register name. The shortened name is used with appended suffixes to extract information from the register H files.
                  Example: "REG1" expands to "REG1_REG" to find the register offset in the H file
  @param[in] DATA/DATAx: data to be written to the reg/field
  @param[in] FLD/FLDx: field to be read/written
  @param[in] FLAGS: OR'ed flags in [IP_WR_REG_FLAGS].  Macros automatically determine "IpWrRegFlagSize*" and logically OR with other passed FLAGS
  @param[in] CINDEX: this is the index into array of [IP_WR_REG_CNTXT] when needed by some registers.
  @param[in] REGTEMPLATE: this is equivalent to REG, but REGTEMPLATE is used as a template for registers at REGOFFSET
  @param[in] REGOFFSET: this is the calculated offsets instead of offsets defined by looking up REG in the H file.

  @retval         same as macros without the "_OFFSET" suffix
**/
#define REG_READ_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET) \
  REG_READ_RAW (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE))

#define REG_WRITE_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, DATA) \
  REG_WRITE_RAW (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, DATA)

#define REG_READ_FLD_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD) \
  REG_READ_FLD_RAW (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), FLD)

#define REG_RW_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET) \
  REG_WRITE_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET, REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET))

// REG_RMW_FLD*_OFFSET
#define REG_RMW_FLD1_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1) \
  _REG_IMW_RAW1 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET), FLD1, DATA1)

#define REG_RMW_FLD2_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2)

#define REG_RMW_FLD3_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_RMW_FLD6_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_RMW_FLD8_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_OFFSET (PINST, FLAGS, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

// REG_WRITE_FLD*_ZERO_OFFSET
#define REG_WRITE_FLD1_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1) \
  _REG_IMW_RAW1 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1)

#define REG_WRITE_FLD2_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2)

#define REG_WRITE_FLD3_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_WRITE_FLD4_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_WRITE_FLD5_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_WRITE_FLD6_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_WRITE_FLD7_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_WRITE_FLD8_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

/**
  REG_*_OFFPARAM()
  - these are same as macros without the "_OFFPARAM" suffix, except for the OFFSET_PARAMS param
  - this is used when same reg is duplicated many times at different offets (eg per lane config)

  @param[in] OFFSET_PARAMS is the '()' enclose parameter passed to user supplied function "_IpCsiRegOffParamFunc_REGTEMPLATE".
                  Example: REG_READ_OFFPARAM (pInst, 0, IPREG_PORTx_LANEy, (x, y))
                    - this will call _IpCsiRegOffParamFunc_IPREG_PORTx_LANEy(x,y) to calculate the offset.
  @param[in] PINST: the IP's instance pointer
  @param[in] REG: the shortened register name. The shortened name is used with appended suffixes to extract information from the register H files.
                  Example: "REG1" expands to "REG1_REG" to find the register offset in the H file
  @param[in] DATA/DATAx: data to be written to the reg/field
  @param[in] FLD/FLDx: field to be read/written
  @param[in] FLAGS: OR'ed flags in [IP_WR_REG_FLAGS].  Macros automatically determine "IpWrRegFlagSize*" and logically OR with other passed FLAGS
  @param[in] CINDEX: this is the index into array of [IP_WR_REG_CNTXT] when needed by some registers.
  @param[in] REGTEMPLATE: this is equivalent to REG, but REGTEMPLATE is used as a template for registers at REGOFFSET
  @param[in] REGOFFSET: this is the calculated offsets instead of offsets defined by looking up REG in the H file.

  @retval         same as macros without the "_OFFPARAM" suffix
**/

#define REG_READ_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS) \
  REG_READ_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS)

#define REG_WRITE_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, DATA) \
  REG_WRITE_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, DATA)

#define REG_READ_FLD_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD) \
  REG_READ_FLD_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD)

#define REG_RW_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS) \
  REG_RW_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS)

// REG_RMW_FLD*_OFFPARAM
#define REG_RMW_FLD1_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1) \
  REG_RMW_FLD1_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1)

#define REG_RMW_FLD2_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2) \
  REG_RMW_FLD2_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2)

#define REG_RMW_FLD3_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  REG_RMW_FLD3_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_RMW_FLD4_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_RMW_FLD5_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_RMW_FLD6_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_RMW_FLD6_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_RMW_FLD7_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_RMW_FLD8_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  REG_RMW_FLD8_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

// REG_WRITE_FLD*_ZERO_OFFSET
#define REG_WRITE_FLD1_ZERO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1) \
  REG_WRITE_FLD1_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1)

#define REG_WRITE_FLD2_ZERO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2) \
  REG_WRITE_FLD2_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2)

#define REG_WRITE_FLD3_ZERO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  REG_WRITE_FLD3_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_WRITE_FLD4_ZERO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_WRITE_FLD4_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_WRITE_FLD5_ZERO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_WRITE_FLD5_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_WRITE_FLD6_ZERO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_WRITE_FLD6_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_WRITE_FLD7_ZERO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_WRITE_FLD7_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_WRITE_FLD8_ZERO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  REG_WRITE_FLD8_ZERO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

/**
  REG_*_CO()
  -  these are same as macros without the "_CO" suffix, except for the CINDEX and REGOFFSET params

  @param[in]      REGOFFSET this is the calculated offsets instead of offsets defined by looking up REG in the H file.
  @param[in]      CINDEX this is the index into array of IP_WR_REG_CNTXT when needed by some registers.
  @param[in]      OtherParams... see the base macro without suffix for other params

  @retval         same as macros without the "_CO" suffix
**/
#define REG_READ_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET) \
  REG_READ_RAW (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE))

#define REG_WRITE_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, DATA) \
  REG_WRITE_RAW (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, DATA)

#define REG_READ_FLD_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD) \
  REG_READ_FLD_RAW (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), FLD)

#define REG_RW_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET) \
  REG_WRITE_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET))

// REG_RMW_FLD*_CO
#define REG_RMW_FLD1_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1) \
  _REG_IMW_RAW1 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), FLD1, DATA1)

#define REG_RMW_FLD2_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2)

#define REG_RMW_FLD3_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_RMW_FLD6_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_RMW_FLD8_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

// REG_WRITE_FLD*_ZERO_CO
#define REG_WRITE_FLD1_ZERO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1) \
  _REG_IMW_RAW1 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1)

#define REG_WRITE_FLD2_ZERO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2)

#define REG_WRITE_FLD3_ZERO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3)

#define REG_WRITE_FLD4_ZERO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_WRITE_FLD5_ZERO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5)

#define REG_WRITE_FLD6_ZERO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6)

#define REG_WRITE_FLD7_ZERO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_WRITE_FLD8_ZERO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), 0, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8)

// REG_READ_RAW returns read of correct UINT width based on REG
#define REG_READ_RAW(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT) \
   (XCAT(UINT, REG_BITS (REG)))(_REG_READ_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT))

#define REG_WRITE_RAW(PINST, FLAGS, REGCNTXT, REG, OFFSET, DATA) \
  _REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, DATA)

#define _REG_IMW_RAW1(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, INITDATA, FLD1, DATA1) \
  do {                                                                                       \
    REGSTRUCT _RegRmwTempStruct;                                                             \
    _RegRmwTempStruct.Data      = INITDATA;                                                  \
    _RegRmwTempStruct.Bits.FLD1 = DATA1;                                                     \
    REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, _RegRmwTempStruct.Data);             \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD1, DATA1);              \
  } while (0)

#define _REG_IMW_RAW2(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, INITDATA, FLD1, DATA1, FLD2, DATA2) \
  do {                                                                                                    \
    REGSTRUCT _RegRmwTempStruct;                                                                          \
    _RegRmwTempStruct.Data      = INITDATA;                                                               \
    _RegRmwTempStruct.Bits.FLD1 = DATA1;                                                                  \
    _RegRmwTempStruct.Bits.FLD2 = DATA2;                                                                  \
    REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, _RegRmwTempStruct.Data);                          \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD1, DATA1);                           \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD2, DATA2);                           \
  } while (0)

#define _REG_IMW_RAW3(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, INITDATA, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3) \
  do {                                                                                                                 \
    REGSTRUCT _RegRmwTempStruct;                                                                                       \
    _RegRmwTempStruct.Data      = INITDATA;                                                                            \
    _RegRmwTempStruct.Bits.FLD1 = DATA1;                                                                               \
    _RegRmwTempStruct.Bits.FLD2 = DATA2;                                                                               \
    _RegRmwTempStruct.Bits.FLD3 = DATA3;                                                                               \
    REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, _RegRmwTempStruct.Data);                                       \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD1, DATA1);                                        \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD2, DATA2);                                        \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD3, DATA3);                                        \
  } while (0)

#define _REG_IMW_RAW4(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, INITDATA, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  do {                                                                                                                              \
    REGSTRUCT _RegRmwTempStruct;                                                                                                    \
    _RegRmwTempStruct.Data      = INITDATA;                                                                                         \
    _RegRmwTempStruct.Bits.FLD1 = DATA1;                                                                                            \
    _RegRmwTempStruct.Bits.FLD2 = DATA2;                                                                                            \
    _RegRmwTempStruct.Bits.FLD3 = DATA3;                                                                                            \
    _RegRmwTempStruct.Bits.FLD4 = DATA4;                                                                                            \
    REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, _RegRmwTempStruct.Data);                                                    \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD1, DATA1);                                                     \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD2, DATA2);                                                     \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD3, DATA3);                                                     \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD4, DATA4);                                                     \
  } while (0)

#define _REG_IMW_RAW5(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, INITDATA, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  do {                                                                                                                                           \
    REGSTRUCT _RegRmwTempStruct;                                                                                                                 \
    _RegRmwTempStruct.Data      = INITDATA;                                                                                                      \
    _RegRmwTempStruct.Bits.FLD1 = DATA1;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD2 = DATA2;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD3 = DATA3;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD4 = DATA4;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD5 = DATA5;                                                                                                         \
    REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, _RegRmwTempStruct.Data);                                                                 \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD1, DATA1);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD2, DATA2);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD3, DATA3);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD4, DATA4);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD5, DATA5);                                                                  \
  } while (0)

#define _REG_IMW_RAW6(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, INITDATA, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  do {                                                                                                                                           \
    REGSTRUCT _RegRmwTempStruct;                                                                                                                 \
    _RegRmwTempStruct.Data      = INITDATA;                                                                                                      \
    _RegRmwTempStruct.Bits.FLD1 = DATA1;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD2 = DATA2;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD3 = DATA3;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD4 = DATA4;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD5 = DATA5;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD6 = DATA6;                                                                                                         \
    REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, _RegRmwTempStruct.Data);                                                                 \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD1, DATA1);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD2, DATA2);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD3, DATA3);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD4, DATA4);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD5, DATA5);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD6, DATA6);                                                                  \
  } while (0)

#define _REG_IMW_RAW7(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, INITDATA, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  do {                                                                                                                                           \
    REGSTRUCT _RegRmwTempStruct;                                                                                                                 \
    _RegRmwTempStruct.Data      = INITDATA;                                                                                                      \
    _RegRmwTempStruct.Bits.FLD1 = DATA1;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD2 = DATA2;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD3 = DATA3;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD4 = DATA4;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD5 = DATA5;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD6 = DATA6;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD7 = DATA7;                                                                                                         \
    REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, _RegRmwTempStruct.Data);                                                                 \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD1, DATA1);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD2, DATA2);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD3, DATA3);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD4, DATA4);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD5, DATA5);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD6, DATA6);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD7, DATA7);                                                                  \
  } while (0)

#define _REG_IMW_RAW8(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, INITDATA, FLD1, DATA1, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7, FLD8, DATA8) \
  do {                                                                                                                                           \
    REGSTRUCT _RegRmwTempStruct;                                                                                                                 \
    _RegRmwTempStruct.Data      = INITDATA;                                                                                                      \
    _RegRmwTempStruct.Bits.FLD1 = DATA1;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD2 = DATA2;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD3 = DATA3;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD4 = DATA4;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD5 = DATA5;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD6 = DATA6;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD7 = DATA7;                                                                                                         \
    _RegRmwTempStruct.Bits.FLD8 = DATA8;                                                                                                         \
    REG_WRITE_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, _RegRmwTempStruct.Data);                                                                 \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD1, DATA1);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD2, DATA2);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD3, DATA3);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD4, DATA4);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD5, DATA5);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD6, DATA6);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD7, DATA7);                                                                  \
    _REG_DFX_FLD (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD8, DATA8);                                                                  \
  } while (0)

#define _REG_FLDHI_BITS(REG, FLD)  XCAT(XCAT(XCAT(REG, _), FLD), _hi_SIZE)
#define _REG_FLDLO_BITS(REG, FLD)  XCAT(XCAT(XCAT(REG, _), FLD), _lo_SIZE)

#define _REG_FLD_DATA_TO_LO(REG, FLDHILO, DATAHILO) \
  (((UINT32)(DATAHILO) << (32-_REG_FLDLO_BITS(REG,FLDHILO))) >> (32-_REG_FLDLO_BITS(REG,FLDHILO))) // NOTE: 32bit shifts

#define _REG_FLD_DATA_TO_HI(REG, FLDHILO, DATAHILO) \
  (UINT32)(IpWrRShiftU64(DATAHILO, _REG_FLDLO_BITS(REG,FLDHILO)))

#define _REG_FLD_DATA_TO_HILO(REG, FLDHILO, DATAHILO) \
  IpWrRShiftU64( IpWrLShiftU64((DATAHILO) , (32-_REG_FLDHI_BITS(REG,FLDHILO))), (64 - (_REG_FLDHI_BITS(REG,FLDHILO) + _REG_FLDLO_BITS(REG,FLDHILO))))

#define REG_READ_FLD_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1HILO) \
  REG_READ_FLD_HILO_RAW (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CNTXT (REGTEMPLATE), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), FLD1HILO)

#define REG_READ_FLD_HILO_FLAG(PINST, FLAGS, REG, FLD1HILO) \
  REG_READ_FLD_HILO_RAW (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CNTXT (REG), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLD1HILO)

#define REG_READ_FLD_HILO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1HILO) \
  REG_READ_FLD_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS, FLD1HILO)

#define REG_RMW_FLD1_HILO_FLAG(PINST, FLAGS, REG, FLD1HILO, DATA1HILO) \
  REG_RMW_FLD2_FLAG(PINST, FLAGS, REG,                                 \
  XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO),  \
  XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO) )

#define REG_RMW_FLD2_HILO_FLAG(PINST, FLAGS, REG, FLD1HILO, DATA1HILO, FLD2, DATA2) \
  REG_RMW_FLD3_FLAG(PINST, FLAGS, REG,                                \
  XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
  XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
  FLD2, DATA2 )

#define REG_RMW_FLD3_HILO_FLAG(PINST, FLAGS, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3) \
  REG_RMW_FLD4_FLAG(PINST, FLAGS, REG,                                 \
  XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO),  \
  XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO),  \
  FLD2, DATA2, FLD3, DATA3 )

#define REG_RMW_FLD4_HILO_FLAG(PINST, FLAGS, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_RMW_FLD5_FLAG(PINST, FLAGS, REG,                                 \
  XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO),  \
  XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO),  \
  FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 )

#define REG_RMW_FLD5_HILO_FLAG(PINST, FLAGS, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_RMW_FLD6_FLAG(PINST, FLAGS, REG,                                 \
  XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO),  \
  XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO),  \
  FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5)

#define REG_RMW_FLD6_HILO_FLAG(PINST, FLAGS, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_RMW_FLD7_FLAG(PINST, FLAGS, REG,                                \
  XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
  XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
  FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_HILO_FLAG(PINST, FLAGS, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_RMW_FLD8_FLAG(PINST, FLAGS, REG,                                \
  XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
  XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
  FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_READ_FLD_HILO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLDHILO) \
  REG_READ_FLD_HILO_RAW (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), FLDHILO)

#define REG_READ_FLD_HILO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLDHILO) \
  REG_READ_FLD_HILO_RAW (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), FLDHILO)

#define REG_RMW_FLD1_HILO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1HILO, DATA1HILO) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO))

#define REG_RMW_FLD2_HILO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1HILO, DATA1HILO, FLD2, DATA2) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
    FLD2, DATA2)

#define REG_RMW_FLD3_HILO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
    FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_HILO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_HILO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5)

#define REG_RMW_FLD6_HILO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_HILO_CINDEX_FLAG(PINST, FLAGS, CINDEX, REG, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REG) | (FLAGS), (PINST)->REG_CINDEX (REG, CINDEX), REG, REG_OFFSET (REG), REG_STRUCT (REG), REG_READ_CINDEX_FLAG (PINST, FLAGS, CINDEX, REG), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REG, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REG, FLD1HILO, DATA1HILO), \
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_RMW_FLD1_HILO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO) \
  _REG_IMW_RAW2 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO))

#define REG_RMW_FLD2_HILO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2) \
  _REG_IMW_RAW3 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2)

#define REG_RMW_FLD3_HILO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3) \
  _REG_IMW_RAW4 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_HILO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  _REG_IMW_RAW5 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_HILO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  _REG_IMW_RAW6 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5)

#define REG_RMW_FLD6_HILO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  _REG_IMW_RAW7 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_HILO_CO(PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  _REG_IMW_RAW8 (PINST, REG_BITS (REGTEMPLATE) | (FLAGS), (PINST)->REG_CINDEX (REGTEMPLATE, CINDEX), REGTEMPLATE, REGOFFSET, REG_STRUCT (REGTEMPLATE), REG_READ_CO (PINST, FLAGS, CINDEX, REGTEMPLATE, REGOFFSET), \
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

// REG_RMW_FLD#_HILO_OFFPARAM
#define REG_RMW_FLD1_HILO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1HILO, DATA1HILO) \
  REG_RMW_FLD2_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO))

#define REG_RMW_FLD2_HILO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1HILO, DATA1HILO, FLD2, DATA2) \
  REG_RMW_FLD3_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2)

#define REG_RMW_FLD3_HILO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3) \
  REG_RMW_FLD4_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_HILO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_RMW_FLD5_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_HILO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_RMW_FLD6_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5)

#define REG_RMW_FLD6_HILO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_RMW_FLD7_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_HILO_OFFPARAM(PINST, FLAGS, REGTEMPLATE, OFFSET_PARAMS, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_RMW_FLD8_OFFSET(PINST, FLAGS, REGTEMPLATE, XCAT(_IpCsiRegOffParamFunc_,REGTEMPLATE) OFFSET_PARAMS,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

// REG_RMW_FLD#_HILO_OFFSET
#define REG_RMW_FLD1_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO) \
  REG_RMW_FLD2_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO))

#define REG_RMW_FLD2_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2) \
  REG_RMW_FLD3_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2)

#define REG_RMW_FLD3_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3) \
  REG_RMW_FLD4_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3)

#define REG_RMW_FLD4_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4) \
  REG_RMW_FLD5_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4)

#define REG_RMW_FLD5_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5) \
  REG_RMW_FLD6_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5)

#define REG_RMW_FLD6_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6) \
  REG_RMW_FLD7_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6)

#define REG_RMW_FLD7_HILO_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET, FLD1HILO, DATA1HILO, FLD2, DATA2, FLD3, DATA3, FLD4, DATA4, FLD5, DATA5, FLD6, DATA6, FLD7, DATA7) \
  REG_RMW_FLD8_OFFSET(PINST, FLAGS, REGTEMPLATE, REGOFFSET,\
    XCAT(FLD1HILO, _lo), _REG_FLD_DATA_TO_LO(REGTEMPLATE, FLD1HILO, DATA1HILO), \
    XCAT(FLD1HILO, _hi), _REG_FLD_DATA_TO_HI(REGTEMPLATE, FLD1HILO, DATA1HILO),\
    FLD2, DATA2, FLD3, DATA3, FLD4, DATA4 , FLD5, DATA5, FLD6, DATA6, FLD7, DATA7)

#define REG_READ_FLD_RAW(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD)                                                  \
  (((REGSTRUCT *)&((PINST)->Prefix._RegMacroTemp64))->Data = REG_READ_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT),    \
   _REG_DFX_FLD_V2_INLINE(REG, FLD, ((REGSTRUCT *)&((PINST)->Prefix._RegMacroTemp64))->Bits.FLD)                               \
   ((REGSTRUCT *)&((PINST)->Prefix._RegMacroTemp64))->Bits.FLD)

#define REG_READ_FLD_HILO_RAW(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLDHILO)                                               \
  (((REGSTRUCT *)&((PINST)->Prefix._RegMacroTemp64))->Data = REG_READ_RAW (PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT),          \
  _REG_DFX_FLD_V2_INLINE(REG, FLDHILO, _REG_FLD_DATA_TO_HILO(REG, FLDHILO, ((REGSTRUCT *)&((PINST)->Prefix._RegMacroTemp64))->Data)) \
  _REG_FLD_DATA_TO_HILO(REG, FLDHILO, ((REGSTRUCT *)&((PINST)->Prefix._RegMacroTemp64))->Data))

  #ifdef IP_WR_DFX_REG
#define _REG_READ_RAW(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT)            IpWrDfxRegRead (REGCNTXT, OFFSET, FLAGS, #REG)
#define _REG_WRITE_RAW(PINST, FLAGS, REGCNTXT, REG, OFFSET, DATA)                IpWrDfxRegWrite (REGCNTXT, OFFSET, DATA, FLAGS, #REG)
#define _REG_DFX_FLD(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD, DATA)  IpWrDfxFldData (XSTR (REG.FLD), DATA)
#define _REG_DFX_FLD_V2_INLINE(REG, FLD, DATA)                                   IpWrDfxFldData (XSTR (REG.FLD), DATA), // NOTE: comma required here, but NOT in calling code
  #else
#define _REG_READ_RAW(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT)            IpWrRegRead (REGCNTXT, OFFSET, FLAGS)
#define _REG_WRITE_RAW(PINST, FLAGS, REGCNTXT, REG, OFFSET, DATA)                IpWrRegWrite (REGCNTXT, OFFSET, DATA, FLAGS)
#define _REG_DFX_FLD(PINST, FLAGS, REGCNTXT, REG, OFFSET, REGSTRUCT, FLD, DATA)  do{}while(0) // NOOP
#define _REG_DFX_FLD_V2_INLINE(REG, FLD, DATA)                                                // NOTE: do not put expression, comma, etc here or in calling code
  #endif

// DEPRECATED: the following are deprecated and will be removed
  #if 1
#define REG_RMW1  REG_RMW_FLD1
#define REG_RMW2  REG_RMW_FLD2
#define REG_RMW3  REG_RMW_FLD3
#define REG_RMW4  REG_RMW_FLD4
#define REG_RMW5  REG_RMW_FLD5
#define REG_RMW6  REG_RMW_FLD6

#define REG_RMW1_OFFSET  REG_RMW_FLD1_OFFSET
#define REG_RMW2_OFFSET  REG_RMW_FLD2_OFFSET
#define REG_RMW3_OFFSET  REG_RMW_FLD3_OFFSET
#define REG_RMW4_OFFSET  REG_RMW_FLD4_OFFSET
#define REG_RMW5_OFFSET  REG_RMW_FLD5_OFFSET
#define REG_RMW6_OFFSET  REG_RMW_FLD6_OFFSET

#define REG_RMW1_FLAG  REG_RMW_FLD1_FLAG
#define REG_RMW2_FLAG  REG_RMW_FLD2_FLAG
#define REG_RMW3_FLAG  REG_RMW_FLD3_FLAG
#define REG_RMW4_FLAG  REG_RMW_FLD4_FLAG
#define REG_RMW5_FLAG  REG_RMW_FLD5_FLAG
#define REG_RMW6_FLAG  REG_RMW_FLD6_FLAG

#define REG_RMW1_CINDEX  REG_RMW_FLD1_CINDEX
#define REG_RMW2_CINDEX  REG_RMW_FLD2_CINDEX
#define REG_RMW3_CINDEX  REG_RMW_FLD3_CINDEX
#define REG_RMW4_CINDEX  REG_RMW_FLD4_CINDEX
#define REG_RMW5_CINDEX  REG_RMW_FLD5_CINDEX
#define REG_RMW6_CINDEX  REG_RMW_FLD6_CINDEX

#define REG_RMW1_CO  REG_RMW_FLD1_CO
#define REG_RMW2_CO  REG_RMW_FLD2_CO
#define REG_RMW3_CO  REG_RMW_FLD3_CO
#define REG_RMW4_CO  REG_RMW_FLD4_CO
#define REG_RMW5_CO  REG_RMW_FLD5_CO
#define REG_RMW6_CO  REG_RMW_FLD6_CO

#define REG_W1  REG_WRITE_FLD1_ZERO
#define REG_W2  REG_WRITE_FLD2_ZERO
#define REG_W3  REG_WRITE_FLD3_ZERO
#define REG_W4  REG_WRITE_FLD4_ZERO
#define REG_W5  REG_WRITE_FLD5_ZERO
#define REG_W6  REG_WRITE_FLD6_ZERO

#define REG_W1_FLAG  REG_WRITE_FLD1_ZERO_FLAG
#define REG_W2_FLAG  REG_WRITE_FLD2_ZERO_FLAG
#define REG_W3_FLAG  REG_WRITE_FLD3_ZERO_FLAG
#define REG_W4_FLAG  REG_WRITE_FLD4_ZERO_FLAG
#define REG_W5_FLAG  REG_WRITE_FLD5_ZERO_FLAG
#define REG_W6_FLAG  REG_WRITE_FLD6_ZERO_FLAG

#define REG_W1_CINDEX  REG_WRITE_FLD1_ZERO_CINDEX
#define REG_W2_CINDEX  REG_WRITE_FLD2_ZERO_CINDEX
#define REG_W3_CINDEX  REG_WRITE_FLD3_ZERO_CINDEX
#define REG_W4_CINDEX  REG_WRITE_FLD4_ZERO_CINDEX
#define REG_W5_CINDEX  REG_WRITE_FLD5_ZERO_CINDEX
#define REG_W6_CINDEX  REG_WRITE_FLD6_ZERO_CINDEX

#define REG_W1_OFFSET  REG_WRITE_FLD1_ZERO_OFFSET
#define REG_W2_OFFSET  REG_WRITE_FLD2_ZERO_OFFSET
#define REG_W3_OFFSET  REG_WRITE_FLD3_ZERO_OFFSET
#define REG_W4_OFFSET  REG_WRITE_FLD4_ZERO_OFFSET
#define REG_W5_OFFSET  REG_WRITE_FLD5_ZERO_OFFSET
#define REG_W6_OFFSET  REG_WRITE_FLD6_ZERO_OFFSET

#define REG_W1_CO  REG_WRITE_FLD1_ZERO_CO
#define REG_W2_CO  REG_WRITE_FLD2_ZERO_CO
#define REG_W3_CO  REG_WRITE_FLD3_ZERO_CO
#define REG_W4_CO  REG_WRITE_FLD4_ZERO_CO
#define REG_W5_CO  REG_WRITE_FLD5_ZERO_CO
#define REG_W6_CO  REG_WRITE_FLD6_ZERO_CO

  #endif //deprecated

#endif //IP_CSI_REG_H_

#ifdef __cplusplus
}
#endif
