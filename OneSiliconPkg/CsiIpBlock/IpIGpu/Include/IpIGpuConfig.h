/** @file
  Header file for IGPU IP Config Block

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

@par Specification Reference:
**/

#ifndef _IP_IGPU_CONFIG_H_
#define _IP_IGPU_CONFIG_H_

#include <IpCsi.h>

#pragma pack(push, 1)

//
// DDI defines
//
typedef enum {
  IpIGpuDdiDisable       = 0x00,
  IpIGpuDdiDdcEnable     = 0x01,
} IGPU_DDI_DDC_VAL;

typedef enum {
  IpIGpuDdiHpdDisable  = 0x00,
  IpIGpuDdiHpdEnable   = 0x01,
} IGPU_DDI_HPD_VAL;

typedef enum {
  IpIGpuDdiPortDisabled   = 0x00,
  IpIGpuDdiPortEdp        = 0x01,
  IpIGpuDdiPortEdpOverTcp = 0x02,
} IGPU_DDI_PORT_SETTINGS;

//
// Data from MRC Start
// OneSiliconPkg\Include\MemInfoHob.h
//

#define HOB_MAX_SAGV_POINTS 4
//
// DIMM timings
//
typedef struct {
  UINT32 tCK;       ///< Memory cycle time, in femtoseconds.
  UINT16 NMode;     ///< Number of tCK cycles for the channel DIMM's command rate mode.
  UINT16 tCL;       ///< Number of tCK cycles for the channel DIMM's CAS latency.
  UINT16 tCWL;      ///< Number of tCK cycles for the channel DIMM's minimum CAS write latency time.
  UINT16 tFAW;      ///< Number of tCK cycles for the channel DIMM's minimum four activate window delay time.
  UINT16 tRAS;      ///< Number of tCK cycles for the channel DIMM's minimum active to precharge delay time.
  UINT16 tRCDtRP;   ///< Number of tCK cycles for the channel DIMM's minimum RAS# to CAS# delay time and Row Precharge delay time.
  UINT32 tREFI;     ///< Number of tCK cycles for the channel DIMM's minimum Average Periodic Refresh Interval.
  UINT16 tRFC;      ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFCpb;    ///< Number of tCK cycles for the channel DIMM's minimum per bank refresh recovery delay time.
  UINT16 tRFC2;     ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRFC4;     ///< Number of tCK cycles for the channel DIMM's minimum refresh recovery delay time.
  UINT16 tRPab;     ///< Number of tCK cycles for the channel DIMM's minimum row precharge delay time for all banks.
  UINT16 tRRD;      ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time.
  UINT16 tRRD_L;    ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for same bank groups.
  UINT16 tRRD_S;    ///< Number of tCK cycles for the channel DIMM's minimum row active to row active delay time for different bank groups.
  UINT16 tRTP;      ///< Number of tCK cycles for the channel DIMM's minimum internal read to precharge command delay time.
  UINT16 tWR;       ///< Number of tCK cycles for the channel DIMM's minimum write recovery time.
  UINT16 tWTR;      ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time.
  UINT16 tWTR_L;    ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for same bank groups.
  UINT16 tWTR_S;    ///< Number of tCK cycles for the channel DIMM's minimum internal write to read command delay time for different bank groups.
  UINT16 tCCD_L;    ///< Number of tCK cycles for the channel DIMM's minimum CAS-to-CAS delay for same bank group.
  UINT16 tCCD_L_WR; ///< Number of tCK cycles for the channel DIMM's minimum Write-to-Write delay for same bank group.
  UINT8  Resv[2];   ///< Reserved.
} IP_MRC_CH_TIMING;

typedef struct {
  UINT16 tRDPRE;     ///< Read CAS to Precharge cmd delay
} IP_MRC_IP_TIMING;

/// This data structure contains per-SaGv timing values that are considered output by the MRC.
typedef struct {
  UINT32           DataRate;    ///< The memory rate for the current SaGv Point in units of MT/s
  IP_MRC_CH_TIMING JedecTiming; ///< Timings used for this entry's corresponding SaGv Point - derived from JEDEC SPD spec
  IP_MRC_IP_TIMING IpTiming;    ///< Timings used for this entry's corresponding SaGv Point - IP specific
  UINT16           MaxMemoryBandwidth; ///< Maximum theoretical bandwidth in GB/s supported by GV
} IP_HOB_SAGV_TIMING_OUT;

/// This data structure contains SAGV config values that are considered output by the MRC.
typedef struct {
  UINT32                 NumSaGvPointsEnabled; ///< Count of the total number of SAGV Points enabled.
  UINT32                 SaGvPointMask;        ///< Bit mask where each bit indicates an enabled SAGV point.
  IP_HOB_SAGV_TIMING_OUT SaGvTiming[HOB_MAX_SAGV_POINTS];
} IP_HOB_SAGV_INFO;

typedef struct {
  UINT8               MemoryType;             ///< DDR type: DDR3, DDR4, or LPDDR3
  UINT32              NumPopulatedChannels;   ///< Total number of memory channels populated
  IP_HOB_SAGV_INFO    SagvConfigInfo;         ///< This data structure contains SAGV config values that are considered output by the MRC.
} MEMORY_INFO_DATA;

//
// Data from MRC End
//

/**
  This structure configures the Native GPIOs for DDI port per VBT settings.
  these settings must match VBT's settings.
**/
typedef struct {
  UINT8            DdiPortAConfig; /// The Configuration of DDI port A, DdiPortDisabled - No LFP is connected on DdiPortA, <b>DdiPortEdp - Set DdiPortA to eDP</b>
  UINT8            DdiPortBConfig; /// The Configuration of DDI port B, DdiPortDisabled - No LFP is connected on DdiPortB, <b>DdiPortEdp - Set DdiPortB to eDP</b>
  UINT8            DdiPortAHpd;    /// The HPD setting of DDI Port A, <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8            DdiPortBHpd;    /// The HPD setting of DDI Port B, <b>DdiHpdDisable - Disable HPD</b>, <b>DdiHpdEnable - Enable HPD
  UINT8            DdiPortCHpd;    /// The HPD setting of DDI Port C, <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8            DdiPort1Hpd;    /// The HPD setting of DDI Port 1, <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8            DdiPort2Hpd;    /// The HPD setting of DDI Port 2, <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8            DdiPort3Hpd;    /// The HPD setting of DDI Port 3, <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8            DdiPort4Hpd;    /// The HPD setting of DDI Port 4, <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8            DdiPortADdc;    /// The DDC setting of DDI Port A, <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8            DdiPortBDdc;    /// The DDC setting of DDI Port B, <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8            DdiPortCDdc;    /// The DDC setting of DDI Port C, <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8            DdiPort1Ddc;    /// The DDC setting of DDI Port 1, <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8            DdiPort2Ddc;    /// The DDC setting of DDI Port 2, <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8            DdiPort3Ddc;    /// The DDC setting of DDI Port 3, <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8            DdiPort4Ddc;    /// The DDC setting of DDI Port 4, <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
} IGPU_DDI_CONFIGURATION;

typedef struct {
  UINT64             GraphicsConfigPtr;    ///< Address of the Graphics Configuration Table
  UINT64             BltBufferAddress;     ///< Address of Blt buffer for PEIM Logo use
  UINT32             LogoPixelHeight;      ///< Address of LogoPixelHeight for PEIM Logo use
  UINT32             LogoPixelWidth;       ///< Address of LogoPixelWidth for PEIM Logo use
  UINT32             VerticalResolution;   ///< Address of VerticalResolution for PEIM Logo use
  UINT32             HorizontalResolution; ///< Address of HorizontalResolution for PEIM Logo use
  UINT8              PeiGraphicsPeimInit;  ///< This policy is used to enable/disable Intel IGPU PEIM.<b>0- Disable</b>, 1- Enable
  UINT8              SkipFspGop;           ///< This policy is used to skip PEIM GOP in FSP.<b>0- Use FSP provided GOP driver</b>, 1- Skip FSP provided GOP driver
  UINT8              Rsvd[2];
} IGPU_PEI_DISPLAY_CONFIG;

typedef struct {
  UINT8            ConfigureMedia;       ///< Configure Media for use. TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  UINT8            RC1pMediaFreqEnable;  ///< This policy is used to enable/disable Media RC1p Frequency. <b>0- Disable</b>, 1- Enable
  UINT8            Rsvd[2];
} IGPU_PEI_MEDIA_CONFIG;

typedef struct {
  UINT8            ConfigureGT;          ///< Configure GT for use. TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  UINT8            RC1pGtFreqEnable;     ///< This policy is used to enable/disable GT RC1p Frequency. <b>0- Disable</b>, 1- Enable
  UINT8            Rsvd[6];
} IGPU_PEI_GT_CONFIG;


/**
  This Configuration block IGPU
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  /**
    Selection of the primary display device: 0=IGPU, 1=PEG, 2=PCIe Graphics on PCH, <b>3=AUTO</b>, 4=Switchable Graphics
    When AUTO mode selected, the priority of display devices is: PCIe Graphics on PCH > PEG > IGPU
  **/
  UINT8                  PrimaryDisplay;
  /**
    Intel IGPU Support. It controls enabling/disabling IGPU device.
    When AUTO mode selected, IGPU will be turned off when external graphics detected.
    If FALSE, all other polices can be ignored.
    <b>2 = AUTO</b>;
    0 = FALSE;
    1 = TRUE.
  **/
  UINT8                  InternalGraphics;
  /**
    Pre-allocated memory for IGPU
    0   = 0MB,1 or 247 = 32MB,
    2   = 64MB,    3   = 128MB,
    240 = 4MB,     241 = 8MB,
    242 = 12MB,    243 = 16MB,
    244 = 20MB,    245 = 24MB,
    246 = 28MB,    248 = 36MB,
    249 = 40MB,    250 = 44MB,
    251 = 48MB,    252 = 52MB,
    253 = 56MB,<b> 254 = 60MB</b>,
    <b>Note: enlarging pre-allocated memory for IGPU may need to reduce MmioSize because of 4GB boundary limitation
  **/
  UINT8                  PanelPowerEnable;           /// Control for enabling/disabling VDD force bit (Required only for early enabling of eDP panel): 0=FALSE, <b>1=TRUE
  UINT8                  MemoryBandwidthCompression; /// This policy is used to enable/disable Memory Bandwidth Compression <b>0- Disable</b>, 1- Enable
  UINT16                 IgdDvmt50PreAlloc;          ///
  UINT16                 DeltaT12PowerCycleDelay;    /// 34 Power Cycle Delay required for eDP as per VESA standard.0 - 0 ms, <b>0xFFFF - Auto calculate to max 500 ms<\b>
  UINT64                 GttMmAdr;                   /// Temp Address of Graphics GTTMMADR: Default is <b>0xAF000000
  UINT64                 LMemBar;                    /// Temp Address of Graphics LMEMBAR: Default is <b>0xB0000000
  IGPU_DDI_CONFIGURATION DdiConfiguration;         /// DDI configuration, need to match with VBT settings.
  UINT8                  OemT12DelayOverride;        /// Oem T12 Delay Override <b> Disable<\b>,Enable-Enable T12 Time.
  UINT8                  TestRsvd0[7];               /// Reserved for 8 bytes alignment
} IP_IGPU_PEI_PREMEM_CONFIG;

/**
  This configuration block is to configure IGD related variables used in PostMem PEI.
  If Intel IGPU Device is not supported, all policies can be ignored.
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  IGPU_PEI_DISPLAY_CONFIG   PeiDisplayConfig;
  IGPU_PEI_MEDIA_CONFIG     PeiMediaConfig;
  IGPU_PEI_GT_CONFIG        PeiGtConfig;
  UINT8                     RenderStandby;        ///< Offset 28 :<b>(Test)</b> This field is used to enable or disable RC6 (Render Standby): 0=FALSE, <b>1=TRUE</b>
  UINT8                     PavpEnable;           ///< Offset 29 :IGD PAVP TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  UINT8                     MediaStandby;         ///< Offset 30 :This field is used to enable or disable MC6 (Media Standby): 0=FALSE, <b>1=TRUE</b>
  UINT8                     Rsvd;
} IP_IGPU_PEI_CONFIG;

/**
  These Members used to store the data during the initialization of IGPU
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  //
  // the Status of Display, Media and GT will be saved during the Graphics Initialization flow.
  //

  //
  // PreMem
  //
  BOOLEAN            IsDisplayPresent;
  BOOLEAN            IsMediaPresent;
  BOOLEAN            IsGtPresent;

  //
  // PostMem
  //
  BOOLEAN            IsIbeccEnabled;
  BOOLEAN            IsIGpuVtdEngineEnabled;
  UINT8              Rsvd[3];
  MEMORY_INFO_DATA   MemoryInfoData;       ///< Memory related data for Display
  UINT32             GmsBase;              // GMS Base Address
  UINT32             DsmSize;              // Size should be in the MB
  UINT64             FlatCcsBaseAddr;      ///< Flat CCS Base Address
  UINT32             FlatCcsMemSize;       ///< Flat CCS Size in MB
  UINT16             GgcBase;
  UINT8              Rsvd1[2];
  UINT64             BdsmBase;
  UINT64             BgsmBase;
  UINT32             CsmeBusNum;
  UINT32             CsmeDevNum;
  UINT32             CsmeFuncNum;
  BOOLEAN            IsEom;
} IGPU_PRIVATE_CONFIG;

#pragma pack(pop)

#endif // _IP_IGPU_CONFIG_H_
