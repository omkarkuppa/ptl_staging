/** @file
  Policy definition for Internal Graphics Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _GRAPHICS_CONFIG_H_
#define _GRAPHICS_CONFIG_H_

#include <ConfigBlock.h>

#pragma pack(push, 1)

#define IGPU_PEI_PREMEM_CONFIG_REVISION 2
#define IGPU_PEI_CONFIG_REVISION        1
#define IGPU_DXE_CONFIG_REVISION        1

#define DDI_DEVICE_NUMBER   4
#define MAX_BCLM_ENTRIES    30

typedef enum {
  DISPLAY_AUTO  = 0x00,
  DISPLAY_IGD   = 0x01,
  DISPLAY_HG    = 0x02,
  DISPLAY_PCI   = 0x03,
} DISPLAY_DEVICE;

//
// DDI defines
//
typedef enum {
  DdiDisable       = 0x00,
  DdiDdcEnable     = 0x01,
} DDI_DDC_TBT_VAL;

typedef enum {
  DdiHpdDisable  = 0x00,
  DdiHpdEnable   = 0x01,
} DDI_HPD_VAL;

typedef enum {
  DdiPortDisabled   = 0x00,
  DdiPortEdp        = 0x01,
} DDI_PORT_SETTINGS;

/**
  This structure configures the Native GPIOs for DDI port per VBT settings.
**/
typedef struct {
  UINT8 DdiPortAConfig; /// The Configuration of DDI port A, this settings must match VBT's settings. DdiPortDisabled - No LFP is connected on DdiPortA, <b>DdiPortEdp - Set DdiPortA to eDP</b>
  UINT8 DdiPortBConfig; /// The Configuration of DDI port B, this settings must match VBT's settings. DdiPortDisabled - No LFP is connected on DdiPortB, <b>DdiPortEdp - Set DdiPortB to eDP</b>
  UINT8 DdiPortAHpd;    /// The HPD setting of DDI Port A, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPortBHpd;    /// The HPD setting of DDI Port B, this settings must match VBT's settings. DdiHpdDisable - Disable HPD, <b>DdiHpdEnable - Enable HPD</b>
  UINT8 DdiPortCHpd;    /// The HPD setting of DDI Port C, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPort1Hpd;    /// The HPD setting of DDI Port 1, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPort2Hpd;    /// The HPD setting of DDI Port 2, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPort3Hpd;    /// The HPD setting of DDI Port 3, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPort4Hpd;    /// The HPD setting of DDI Port 4, this settings must match VBT's settings. <b>DdiHpdDisable - Disable HPD</b>, DdiHpdEnable - Enable HPD
  UINT8 DdiPortADdc;    /// The DDC setting of DDI Port A, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPortBDdc;    /// The DDC setting of DDI Port B, this settings must match VBT's settings. DdiDisable - Disable DDC, <b>DdiDdcEnable - Enable DDC </b>
  UINT8 DdiPortCDdc;    /// The DDC setting of DDI Port C, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPort1Ddc;    /// The DDC setting of DDI Port 1, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPort2Ddc;    /// The DDC setting of DDI Port 2, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPort3Ddc;    /// The DDC setting of DDI Port 3, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
  UINT8 DdiPort4Ddc;    /// The DDC setting of DDI Port 4, this settings must match VBT's settings. <b>DdiDisable - Disable DDC</b>, DdiDdcEnable - Enable DDC
} DDI_CONFIGURATION;

/**
  This Configuration block is to configure GT related PreMem data/variables.\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added Oem T12 Delay Override.

**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Offset 0-27 Config Block Header
  /**
    Offset 28
    Selection of the primary display device: 0=IGPU, 1=PEG, 2=PCIe Graphics on PCH, <b>3=AUTO</b>, 4=Switchable Graphics\n
    When AUTO mode selected, the priority of display devices is: PCIe Graphics on PCH > PEG > IGPU
  **/
  UINT8                PrimaryDisplay;
  /**
    Offset 29
    Intel IGPU Support. It controls enabling/disabling IGPU device.
    When AUTO mode selected, IGPU will be turned off when external graphics detected.
    If FALSE, all other polices can be ignored.
    <b>2 = AUTO</b>;
    0 = FALSE;
    1 = TRUE.
  **/
  UINT8                InternalGraphics;
  /**
    Offset 30
    Pre-allocated memory for IGPU\n
    0   = 0MB,1 or 247 = 32MB,\n
    2   = 64MB,    3   = 128MB,\n
    240 = 4MB,     241 = 8MB,\n
    242 = 12MB,    243 = 16MB,\n
    244 = 20MB,    245 = 24MB,\n
    246 = 28MB,    248 = 36MB,\n
    249 = 40MB,    250 = 44MB,\n
    251 = 48MB,    252 = 52MB,\n
    253 = 56MB,<b> 254 = 60MB</b>,\n
    <b>Note: enlarging pre-allocated memory for IGPU may need to reduce MmioSize because of 4GB boundary limitation</b>
  **/
  UINT16               IgdDvmt50PreAlloc;   ///< Offset 30 >
  UINT8                PanelPowerEnable;    ///< Offset 32 :<b>(Test)</b> Control for enabling/disabling VDD force bit (Required only for early enabling of eDP panel): 0=FALSE, <b>1=TRUE</b>
  UINT8                MemoryBandwidthCompression; ///< Offset 33 This policy is used to enable/disable Memory Bandwidth Compression <b>0- Disable</b>, 1- Enable
  UINT16               DeltaT12PowerCycleDelay; ///< Offset 34 Power Cycle Delay required for eDP as per VESA standard.0 - 0 ms, <b>0xFFFF - Auto calculate to max 500 ms<\b>
  UINT64               GttMmAdr;            ///< Offset 36 Temp Address of Graphics GTTMMADR: Default is <b>0xAF000000</b>
  UINT64               LMemBar;             ///< Offset 44 Temp Address of Graphics LMEMBAR: Default is <b>0xB0000000</b>
  DDI_CONFIGURATION    DdiConfiguration;    ///< Offset 52 DDI configuration, need to match with VBT settings.
  UINT8                OemT12DelayOverride; ///< Offset 68 :Oem T12 Delay Override <b> Disable<\b>,Enable-Enable T12 Time.
  UINT8                IGpuGsm2Size;        ///< Offset 68 {0:2GB, 1:4GB, 2:6GB, 3:8GB, 4:10GB, 5:12GB, 6:14GB, 7:16GB, 8:18GB, 9:20GB, 10:22GB, 11:24GB, 12:26GB, 13:28GB, 14:30GB, 15:32GB, 0xFF/Other Value:No Allocation}
  UINT8                Rsvd0[6];            /// Reserved for 8 bytes alignment
  UINT32               Rsvd1[8];            /// Reserved for 32 bytes alignment
} IGPU_PEI_PREMEM_CONFIG;

typedef struct {
  VOID*        GraphicsConfigPtr;    ///< Address of the Graphics Configuration Table
  VOID*        BltBufferAddress;     ///< Address of Blt buffer for PEIM Logo use
  UINT32       LogoPixelHeight;      ///< Address of LogoPixelHeight for PEIM Logo use
  UINT32       LogoPixelWidth;       ///< Address of LogoPixelWidth for PEIM Logo use
  UINT32       VerticalResolution;   ///< Address of VerticalResolution for PEIM Logo use
  UINT32       HorizontalResolution; ///< Address of HorizontalResolution for PEIM Logo use
  UINT8        PeiGraphicsPeimInit;  ///< This policy is used to enable/disable Intel IGPU PEIM.<b>0- Disable</b>, 1- Enable
  UINT8        SkipFspGop;           ///< This policy is used to skip PEIM GOP in FSP.<b>0- Use FSP provided GOP driver</b>, 1- Skip FSP provided GOP driver
  UINT8        LidStatus;            ///< This Policy is used for LidStatus
  UINT8        Rsvd[1];
  UINT32       VbtSize;              ///< This Policy is used for Vbt Size
} PEI_DISPLAY_CONFIG;

typedef struct {
  UINT8        ConfigureMedia;       ///< Configure Media for use. TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  UINT8        RC1pMediaFreqEnable;  ///< This policy is used to enable/disable Media RC1p Frequency. <b>0- Disable</b>, 1- Enable
  UINT8        Rsvd[2];
} PEI_MEDIA_CONFIG;

typedef struct {
  UINT8       ConfigureGT;          ///< Configure GT for use. TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  UINT8       RC1pGtFreqEnable;     ///< This policy is used to enable/disable GT RC1p Frequency. <b>0- Disable</b>, 1- Enable
  UINT8       Rsvd[6];
} PEI_GT_CONFIG;


/**
  This configuration block is to configure IGD related variables used in PostMem PEI.
  If Intel IGPU Device is not supported, all policies can be ignored.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added MediaStandby
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  PEI_DISPLAY_CONFIG   PeiDisplayConfig;
  PEI_MEDIA_CONFIG     PeiMediaConfig;
  PEI_GT_CONFIG        PeiGtConfig;
  UINT8                RenderStandby;        ///< Offset 28 :<b>(Test)</b> This field is used to enable or disable RC6 (Render Standby): 0=FALSE, <b>1=TRUE</b>
  UINT8                PavpEnable;           ///< Offset 29 :IGD PAVP TRUE/FALSE: 0=FALSE, <b>1=TRUE</b>
  UINT8                MediaStandby;         ///< Offset 30 :This field is used to enable or disable MC6 (Media Standby): 0=FALSE, <b>1=TRUE</b>
  UINT8                Rsvd;
} IGPU_PEI_CONFIG;

/**
  This configuration block is to configure IGD related variables used in DXE.
  If Intel IGPU Device is not supported or disabled, all policies will be ignored.
  The data elements should be initialized by a Platform Module.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Offset 0-27: Config Block Header
  UINT32                Size;                     ///< Offset 28: This field gives the size of the GOP VBT Data buffer
  EFI_PHYSICAL_ADDRESS  VbtAddress;               ///< Offset 32: This field points to the GOP VBT data buffer
  CHAR16                GopVersion[0x10];         ///< Offset 40: This field holds the GOP Driver Version. It is an Output Protocol and updated by the Silicon code
  UINT32                BCM1[MAX_BCLM_ENTRIES];   ///< Offset 72: IGD Backlight Brightness Level Duty cycle Mapping Table for LFP1.
  UINT32                BCM2[MAX_BCLM_ENTRIES];   ///< Offset 192: IGD Backlight Brightness Level Duty cycle Mapping Table for LFP2.
  UINT8                 PlatformConfig;           ///< Offset 312: This field gives the Platform Configuration Information (0=Platform is S0ix Capable for ULT SKUs only, <b>1=Platform is not S0ix Capable</b>, 2=Force Platform is S0ix Capable for All SKUs)
  UINT8                 AlsEnable;                ///< Offset 313: Ambient Light Sensor Enable: <b>0=Disable</b>, 2=Enable
  UINT8                 Reserved[6];              ///< Offset 314 : Reserved for QWORD alignment.
} IGPU_DXE_CONFIG;
#pragma pack(pop)

#endif // _GRAPHICS_CONFIG_H_
