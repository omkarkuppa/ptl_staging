/** @file
  HDAUDIO policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _HDAUDIO_CONFIG_H_
#define _HDAUDIO_CONFIG_H_

#include <ConfigBlock.h>
#include <Library/PchLimits.h>

#define HDAUDIO_PREMEM_CONFIG_REVISION 5
#define HDAUDIO_CONFIG_REVISION 1
#define HDAUDIO_DXE_CONFIG_REVISION 2

extern EFI_GUID gHdAudioPreMemConfigGuid;
extern EFI_GUID gHdAudioConfigGuid;
extern EFI_GUID gHdAudioDxeConfigGuid;

#pragma pack (push,1)

///
/// The PCH_HDAUDIO_CONFIG block describes the expected configuration of the Intel HD Audio feature.
///

#define HDAUDIO_VERB_TABLE_VIDDID(Vid,Did)                      (UINT32)((UINT16)Vid | ((UINT16)Did << 16))
#define HDAUDIO_VERB_TABLE_RID_SDI_SIZE(Rid,Sdi,VerbTableSize)  (UINT32)((UINT8)Rid | ((UINT8)Sdi << 8) | ((UINT16)VerbTableSize << 16))
#define HDAUDIO_VERB_TABLE_CMD_SIZE(VerbTable)                  ((sizeof (VerbTable) - sizeof (HDA_VERB_TABLE_HEADER)) / (sizeof (UINT32)))

///
/// Use this macro to create HDAUDIO_VERB_TABLE and populate size automatically
///
#define HDAUDIO_VERB_TABLE_INIT(Vid,Did,Rid,Sdi,...) \
{ \
  { Vid, Did, Rid, Sdi, (sizeof((UINT32[]){__VA_ARGS__})/sizeof(UINT32)) }, \
  (UINT32*)(UINT32[]){ __VA_ARGS__ } \
}

/**
  Azalia verb table header
  Every verb table should contain this defined header and followed by azalia verb commands.
**/
typedef struct {
  UINT16  VendorId;             ///< Codec Vendor ID
  UINT16  DeviceId;             ///< Codec Device ID
  UINT8   RevisionId;           ///< Revision ID of the codec. 0xFF matches any revision.
  UINT8   SdiNum;               ///< SDI number, 0xFF matches any SDI.
  UINT16  DataDwords;           ///< Number of data DWORDs following the header.
} HDA_VERB_TABLE_HEADER;

#ifdef _MSC_VER
//
// Disable "zero-sized array in struct/union" extension warning.
// Used for neater verb table definitions.
//
#pragma warning (push)
#pragma warning (disable: 4200)
#endif
typedef struct  {
  HDA_VERB_TABLE_HEADER  Header;
  UINT32 *Data;
} HDAUDIO_VERB_TABLE;
#ifdef _MSC_VER
#pragma warning (pop)
#endif

typedef struct {
  UINT32   ClkA;            ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_DMIC*_CLKA_*
  UINT32   ClkB;            ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_DMIC*_CLKB_*
  UINT32   Data;            ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_DMIC*_DATA_*
} HDA_DMIC_PIN_MUX;

typedef struct {
  UINT32   Sclk;            ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_I2S*_SCLK*
  UINT32   Sfmr;            ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_I2S*_SFMR*
  UINT32   Txd;             ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_I2S*_TXD*
  UINT32   Rxd;             ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_I2S*_RXD*
} HDA_SSP_PIN_MUX;

typedef struct {
  UINT32   Clk;             ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_DMIC*_CLKA_*
  UINT32   Data0;           ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_SNDW*_DATA0_*
  UINT32   Data1;           ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_SNDW*_DATA0_*
  UINT32   Data2;           ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_SNDW*_DATA0_*
  UINT32   Data3;           ///<  Pin mux configuration. Refer to GPIOV2_*_MUXING_SNDW*_DATA0_*
} HDA_SNDW_MULTILANE_PIN_MUX;

/**
  HD Audio Link Policies
**/
typedef struct {
  UINT32  Mode                            :  2;  ///< Microphone Privacy Mode: <b>0: No Microphone Privacy Support</b>; 1: HW Managed Microphone Privacy; 2: FW Managed Microphone Privacy; 3: Force Microphone Mute.
  UINT32  Deglitch                        :  1;  ///< Microphone Privacy Deglitch <b>0: Disable</b>; 1: Enable.
  UINT32  HwModeSoundWire0                :  1;  ///< Microphone Privacy applied for SoundWire Link number 0 in HW Mode: <b>0: Disable</b>; 1: Enable.
  UINT32  HwModeSoundWire1                :  1;  ///< Microphone Privacy applied for SoundWire Link number 1 in HW Mode: <b>0: Disable</b>; 1: Enable.
  UINT32  HwModeSoundWire2                :  1;  ///< Microphone Privacy applied for SoundWire Link number 2 in HW Mode: <b>0: Disable</b>; 1: Enable.
  UINT32  HwModeSoundWire3                :  1;  ///< Microphone Privacy applied for SoundWire Link number 3 in HW Mode: <b>0: Disable</b>; 1: Enable.
  UINT32  HwModeSoundWire4                :  1;  ///< Microphone Privacy applied for SoundWire Link number 4 0i0n HW Mode: <b>0: Disable</b>; 1: Enable.
  UINT32  HwModeDmic                      :  1;  ///< Microphone Privacy applied for DMIC in HW Mode: <b>0: Disable</b>; 1: Enable.
  UINT32  RsvdBits0                       : 23;
  UINT32  Timeout;                               ///< Indicates the time-out duration to wait before forcing the actual microphone privacy DMA data zeroing. Unit in number of RTC clocks.
} HDA_MIC_PRIVACY;

/**
  HD Audio Link Policies
**/
typedef struct {
  UINT32  Enable    :  1;  ///< HDA interface enable. When enabled related pins will be switched to native mode: <b>0: Disable</b>; 1: Enable.
  UINT32  RsvdBits0 : 31;
  UINT8   SdiEnable[PCH_MAX_HDA_SDI];  ///< HDA SDI signal enable. When enabled related SDI pins will be switched to appropriate native mode: <b>0: Disable</b>; 1: Enable
  UINT8   Reserved[(4 - (PCH_MAX_HDA_SDI % 4)) % 4];  ///< Padding for SDI enable table.
} HDA_LINK_HDA;

/**
  HD Audio DMIC Interface Policies
**/
typedef struct {
  UINT32             Enable          :  1;  ///< HDA DMIC interface enable. When enabled related pins will be switched to native mode: <b>0: Disable</b>; 1: Enable.
  UINT32             DmicClockSelect :  2;  ///< DMIC link clock select: <b>0: Both</b>, 1: ClkA, 2: ClkB; default is "Both"
  UINT32             RsvdBits0       : 29;
  HDA_DMIC_PIN_MUX   PinMux;          ///< Pin mux configuration.
} HDA_LINK_DMIC;

/**
  HD Audio SSP Interface Policies
**/
typedef struct {
  UINT32             Enable    :  1;  ///< HDA SSP interface enable. When enabled related pins will be switched to native mode: <b>0: Disable</b>; 1: Enable.
  UINT32             RsvdBits0 : 31;
  HDA_SSP_PIN_MUX    PinMux;          ///< Pin mux configuration.
} HDA_LINK_SSP;

/**
  HD Audio SNDW Interface Policies
**/
typedef struct {
  UINT32             Enable           :  1;  ///< HDA SNDW interface enable. When enabled related pins will be switched to native mode: <b>0: Disable</b>; 1: Enable.
  UINT32             RsvdBits0        : 31;
} HDA_LINK_SNDW;

/**
  HD Audio SNDW Multilane Policies
**/
typedef struct {
  UINT32                      Enable        :  2;  ///< HDA SNDW Multilane enablement: <b>0: Disable</b>, 1: Two lines enabled, 2: Three lines enabled, 3: Four Lines enabled
  UINT32                      SndwInterface :  3;  ///< HDA SNDW Interface assigment to a Multiline: <b>0: Sndw Interface 0</b>, 1: Sndw Interface 1, 2: Sndw Interface 2, etc.
  UINT32                      RsvdBits0     : 27;  ///< Reserved bits 0
  HDA_SNDW_MULTILANE_PIN_MUX  PinMux;              ///< Pin mux configuration.
} HDA_SNDW_MULTILANE;

/**
  This structure contains the policies which are related to HD Audio device (cAVS).

  <b>Revision 1:</b>
  - Inital version.

**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;          ///< Config Block Header
  UINT32  Pme                   :  1;    ///< Azalia wake-on-ring, <b>0: Disable</b>; 1: Enable
  UINT32  CodecSxWakeCapability :  1;    ///< Capability to detect wake initiated by a codec in Sx (eg by modem codec), <b>0: Disable</b>; 1: Enable
  UINT32  HdAudioLinkFrequency  :  4;    ///< HDA-Link frequency (PCH_HDAUDIO_LINK_FREQUENCY enum): <b>2: 24MHz</b>, 1: 12MHz, 0: 6MHz
  UINT32  RsvdBits0             : 26;    ///< Reserved bits 0
  /**
    Number of the verb table entry defined in VerbTablePtr.
    Each entry points to a verb table which contains HDAUDIO_VERB_TABLE structure and verb command blocks.
  **/
  UINT8   VerbTableEntryNum;
  UINT8   RsvdBytes184[7];                         ///< Reserved bytes, align to multiple 4
  /**
    Pointer to a verb table array.
    This pointer points to 32bits address, and is only eligible and consumed in post mem phase.
    Each entry points to a verb table which contains HDAUDIO_VERB_TABLE structure and verb command blocks.
    The prototype of this is:
    HDAUDIO_VERB_TABLE **VerbTablePtr;
  **/
  UINTN           VerbTablePtr;
  /**
    HdAudio Microphone Privacy Settings
  **/
  HDA_MIC_PRIVACY MicPrivacy;
} HDAUDIO_CONFIG;

typedef struct {
  UINT32   HdaDiscBtOffEnabled : 1;    ///< Discrete BT Offload Enabled
  UINT32   HdaDiscBtOffSspLink : 3;    ///< Discrete BT Offload Ssp Link
  UINT32   Reserved            : 28;   ///< Reserved bits
} HDAUDIO_DISC_BT_OFFLOAD;

/**
  This structure contains the premem policies which are related to HD Audio device (cAVS).

  <b>Revision 1:</b>
  - Inital version.
  <b>Revision 2:</b>
  - Add DmicClockSelect
  <b>Revision 3:</b>
  - Add AudioFpgaSocVer
  <b>Revision 4:</b>
  - Remove AudioFpgaSocVer
  <b>Revision 5:</b>
  - Remove VcType
  <b>Revision 6:</b>
  - Add HdaDiscBtOffload
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;          ///< Config Block Header
  UINT32  Enable                :  1;    ///< Intel HD Audio (Azalia) enablement: 0: Disable, <b>1: Enable</b>
  UINT32  DspEnable             :  1;    ///< DSP enablement: 0: Disable; <b>1: Enable</b>
  /**
    Universal Audio Architecture compliance for DSP enabled system:
    <b>0: Not-UAA Compliant (Intel SST driver supported only)</b>,
       1: UAA Compliant (HDA Inbox driver or SST driver supported)
  **/
  UINT32  DspUaaCompliance      :  1;
  UINT32  IDispLinkFrequency    :  4;    ///< iDisp-Link frequency (PCH_HDAUDIO_LINK_FREQUENCY enum): <b>4: 96MHz</b>, 3: 48MHz
  UINT32  IDispLinkTmode        :  3;    ///< iDisp-Link T-Mode (PCH_HDAUDIO_IDISP_TMODE enum): 0: 2T, 1: 1T, 2: 4T, <b>3: 8T</b>, 4: 16T
  UINT32  IDispCodecDisconnect  :  1;    ///< iDisplay Audio Codec disconnection, <b>0: Not disconnected, enumerable</b>; 1: Disconnected SDI, not enumerable
  UINT32  PowerGatingSupported  :  1;    ///< Power Gating supported: <b>0: Not supported</b>, 1: Supported
  UINT32  SoundWireClockSelect  :  1;    ///< SoundWire clock source select: 0: XTAL, 1: Audio PLL
  UINT32  RsvdBits              : 19;    ///< Reserved bits 0

  /**
    Audio Link Mode configuration bitmask.
    Allows to configure enablement of the following interfaces: HDA-Link, DMIC, SSP, SoundWire.
  **/
  HDA_LINK_HDA          AudioLinkHda;    ///< HDA-Link enablement: 0: Disable; <b>1: Enable</b>.
  /**
    DMIC link enablement: 0: Disable; <b>1: Enable</b>.
    DMIC0  LKF: Muxed with SNDW2/SNDW4.
  **/
  HDA_LINK_DMIC         AudioLinkDmic [PCH_MAX_HDA_DMIC_LINK_NUM];
  /**
    I2S/SSP link enablement: <b>0: Disable</b>; 1: Enable.
    SSP0/1 LKF: Muxed with HDA.
    @note Since the I2S/SSP2 pin set contains pads which are also used for CNVi purpose, enabling AudioLinkSsp2
    is exclusive with CNVi is present.
  **/
  HDA_LINK_SSP          AudioLinkSsp  [PCH_MAX_HDA_SSP_LINK_NUM];
  /**
    SoundWire link enablement: <b>0: Disable</b>; 1: Enable.
    SNDW2  LKF: Muxed with DMIC0/DMIC1.
    SNDW3  LKF: Muxed with DMIC1.
    SNDW4  LKF: Muxed with DMIC0.
  **/
  HDA_LINK_SNDW         AudioLinkSndw [PCH_MAX_HDA_SNDW_LINK_NUM];
  /**
    Multilane SoundWire enablement.
    The index of Sndw interface to be used depends on used platform.
   **/
  HDA_SNDW_MULTILANE    SndwMultilane[PCH_MAX_HDA_SNDW_MULTILANE_NUM];

  UINT16  ResetWaitTimer;                   ///< <b>(Test)</b> The delay timer after Azalia reset, the value is number of microseconds. Default is <b>600</b>.
  UINT8   PowerClockGating;                 ///< Power/Clock Gating (PG/CG): 0: POR (Enable), 1: Enable, 2: Disable
  UINT8   LowFreqLinkClkSrc;                ///< Low Frequency Link Clock Source (LFLCS): 0: POR (Enable), 1: Enable (XTAL), 2: Disable (Audio PLL)
  UINT32  SubSystemIds;                     ///< Value for SID and SVID. If its set to 0 then default value is used.
  /**
      Discrete BT HCI Audio Offload Support
   **/
  HDAUDIO_DISC_BT_OFFLOAD HdaDiscBtOffload;

} HDAUDIO_PREMEM_CONFIG;

typedef struct {
  UINT32  AutonomousClockStop            :  1;    ///< SoundWire link autonomous clock stop capability: <b>0: Disable</b>; 1: Enable
  UINT32  DataOnActiveIntervalSelect     :  2;    ///< SoundWire link data on active interval select 0: 3 clock periods; <b>1: 4 clock periods</b>; 2: 5 clock periods; 3: 6 clock periods
  UINT32  DataOnActiveIntervalExtSelect  :  1;    ///< SoundWire link data on active interval extension select 0: Disable; 1: Enable
  UINT32  DataOnActiveIntervalExt2Select :  1;    ///< SoundWire link data on active interval extension 2 select 0: Disable; 1: Enable
  UINT32  DataOnDelaySelect              :  1;    ///< SoundWire link data on delay select 0: 2 clock periods; 1: 3 clock periods
  UINT32  DataOnDelayExtSelect           :  1;    ///< SoundWire link data on delay extension select 0: Disable; 1: Enable
  UINT32  DataOnDelayExt2Select          :  1;    ///< SoundWire link data on delay extension 2 select 0: Disable; 1: Enable
  UINT32  ClockLoopbackDelaySelectSndw   :  4;    ///< SoundWire link clock loopback delay select 0x0: 3 clock periods; 0x1: 0x4 clock periods; ... ; 0xE - 17 clock periods
  UINT32  ClockLoopbackEnableSndw        :  1;    ///< SoundWire link clock loopback enable 0: Disable; 1: Enable
  UINT32  ClockLoopbackSourceSndw        :  1;    ///< SoundWire link clock loopback source 0: External Loopback; 1: Internal Loopback
  UINT32  RsvdBits1                      : 18;    ///< Reserved bits 1
} HDAUDIO_SNDW_CONFIG;

/**
  This structure contains the DXE policies which are related to HD Audio device (cAVS).
  <b>Revision 1:</b>
  - Inital version.
  <b>Revision 2:</b>
  - Add IoControlEnabled
  <b>Revision 3:</b>
  - Remove IoControlEnabled
**/
typedef struct {
  CONFIG_BLOCK_HEADER        Header;          ///< Config Block Header
  /**
    SNDW configuration for exposed via SNDW ACPI tables:
  **/
  HDAUDIO_SNDW_CONFIG        SndwConfig[PCH_MAX_HDA_SNDW_LINK_NUM];
  /**
    Bitmask of supported DSP features:
    [BIT0] - WoV; [BIT1] - BT Sideband; [BIT2] - Codec VAD; [BIT5] - BT Intel HFP; [BIT6] - BT Intel A2DP
    [BIT7] - DSP based speech pre-processing disabled; [BIT8] - 0: Intel WoV, 1: Windows Voice Activation
    [BIT9] - BT Intel Low Energy;[BIT9] - 0 BT Intel Low Energy is not supported , 1: BT Intel Low Energy is supported
    Default is <b>zero</b>.
  **/
  UINT32  DspFeatureMask;
  /**
   *  Discrete BT HCI Audio Offload Support
   **/
  HDAUDIO_DISC_BT_OFFLOAD HdaDiscBtOffload;
} HDAUDIO_DXE_CONFIG;

#pragma pack (pop)

#endif // _HDAUDIO_CONFIG_H_
