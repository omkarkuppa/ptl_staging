/** @file
  This contains code necessary to build the Domain IGpu Library fpr Display.

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

#include <IGpuDisplay.h>
#include <IGpuDataHob.h>

CONST EFI_PEI_PPI_DESCRIPTOR  gIGpuDisplayInitPreMemDonePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gDisplayInitPreMemDonePpiGuid,
  NULL
};

GLOBAL_REMOVE_IF_UNREFERENCED STATIC EFI_PEI_NOTIFY_DESCRIPTOR  mPeiIGpuNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiGraphicsPpiGuid,
  IGpuPeiNotifyCallback
};

EFI_PEI_PPI_DESCRIPTOR  mIGpuFramebufferReadyPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiGraphicsFramebufferReadyPpiGuid,
  NULL
};

/**
  ProgramDisplayNativeGpioInit: Initialize the Display Native Gpio

  @param[in] IGpuPreMemConfig        - IGPU_PEI_PREMEM_CONFIG to access the IGpuConfig related information

**/
VOID
ProgramDisplayNativeGpioInit (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "%a: Begin\n", __FUNCTION__));
  ///
  /// Check if IGPU is supported or enabled.
  ///
  if (FALSE == IGpuIsSupported ()) {
    DEBUG ((DEBUG_INFO, "Exit %a() since IGPU is unsupported or disabled!\n", __FUNCTION__));
    return;
  }

  ///
  /// Return if Display Engine is not present
  ///
  if (FALSE == IGpuIsDisplayPresent ()) {
    DEBUG ((DEBUG_INFO, "Returning from %a since Display Engine is not present.\n", __FUNCTION__));
    return;
  }

  //
  // Enable shared and specific pins for eDP
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPortAConfig == DdiPortEdp) {
    DEBUG ((DEBUG_INFO, "Configure GPIOs for eDP on DDI-A\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_PANEL_AVDD_EN, 0);
    DEBUG ((DEBUG_INFO, "VDD EN Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_PANEL_BKLTEN, 0);
    DEBUG ((DEBUG_INFO, "BKLT EN Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_PANEL_BKLTCTL, 0);
    DEBUG ((DEBUG_INFO, "BKLT CTL Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDSP_HPDALV, 0);
    DEBUG ((DEBUG_INFO, "HPD Status = %r\n", Status));
  }

  if (IGpuPreMemConfig->DdiConfiguration.DdiPortBConfig == DdiPortEdp) {
    DEBUG ((DEBUG_INFO, "Configure GPIOs for eDP on DDI-B \n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_PANEL_AVDD_EN2, 0);
    DEBUG ((DEBUG_INFO, "VDD EN Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_PANEL_BKLTEN2, 0);
    DEBUG ((DEBUG_INFO, "BKLT EN Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_PANEL_BKLTCTL2, 0);
    DEBUG ((DEBUG_INFO, "BKLT CTL Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDSP_HPD_B, 0);
    DEBUG ((DEBUG_INFO, "HPD Status = %r\n", Status));
  }

  ///
  /// Enable DDSP_HPD pins for DP HotPlug
  ///
  //
  // HPD Port A
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPortAHpd == DdiHpdEnable) {
    DEBUG ((DEBUG_INFO, "Configure HPD for DDI-A\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDSP_HPDALV, 0);
    DEBUG ((DEBUG_INFO, "HPD Status = %r\n", Status));
  }

  //
  // HPD Port B
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPortBHpd == DdiHpdEnable) {
    DEBUG ((DEBUG_INFO, "Configure HPD for DDI-B\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDSP_HPD_B, 0);
    DEBUG ((DEBUG_INFO, "HPD Status = %r\n", Status));
  }

  //
  // HPD Port 1
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPort1Hpd == DdiHpdEnable) {
    DEBUG ((DEBUG_INFO, "Configure HPD for DDI-1\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDSP_HPD (1), 0);
    DEBUG ((DEBUG_INFO, "HPD Status = %r\n", Status));
  }

  //
  // HPD Port 2
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPort2Hpd == DdiHpdEnable) {
    DEBUG ((DEBUG_INFO, "Configure HPD for DDI-2\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDSP_HPD (2), 0);
    DEBUG ((DEBUG_INFO, "HPD Status = %r\n", Status));
  }

  //
  // HPD Port 3
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPort3Hpd == DdiHpdEnable) {
    DEBUG ((DEBUG_INFO, "Configure HPD for DDI-3\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDSP_HPD (3), 0);
    DEBUG ((DEBUG_INFO, "HPD Status = %r\n", Status));
  }

  //
  // HPD Port 4
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPort4Hpd == DdiHpdEnable) {
    DEBUG ((DEBUG_INFO, "Configure HPD for DDI-4\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDSP_HPD (4), 0);
    DEBUG ((DEBUG_INFO, "HPD Status = %r\n", Status));
  }

  ///
  /// Enable DDP CTRLCLK and CTRLDATA pins
  ///
  //
  // DDI Port A
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPortADdc == DdiDdcEnable) {
    DEBUG ((DEBUG_INFO, "Configure DDC for DDI-A\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_CLK_A, 0);
    DEBUG ((DEBUG_INFO, "CTRL CLK Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_DATA_A, 0);
    DEBUG ((DEBUG_INFO, "CTRL DATA Status = %r\n", Status));
  }

  //
  // DDI Port B
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPortBDdc == DdiDdcEnable) {
    DEBUG ((DEBUG_INFO, "Configure DDC for DDI-B\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_CLK_B, 0);
    DEBUG ((DEBUG_INFO, "CTRL CLK Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_DATA_B, 0);
    DEBUG ((DEBUG_INFO, "CTRL DATA Status = %r\n", Status));
  }

  //
  // DDI Port 1
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPort1Ddc == DdiDdcEnable) {
    DEBUG ((DEBUG_INFO, "Configure DDC for DDI-1\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_CLK (1), 0);
    DEBUG ((DEBUG_INFO, "CTRL CLK Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_DATA (1), 0);
    DEBUG ((DEBUG_INFO, "CTRL DATA Status = %r\n", Status));
  }

  //
  // DDI Port 2
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPort2Ddc == DdiDdcEnable) {
    DEBUG ((DEBUG_INFO, "Configure DDC for DDI-2\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_CLK (2), 0);
    DEBUG ((DEBUG_INFO, "CTRL CLK Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_DATA (2), 0);
    DEBUG ((DEBUG_INFO, "CTRL DATA Status = %r\n", Status));
  }

  //
  // DDI Port 3
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPort3Ddc == DdiDdcEnable) {
    DEBUG ((DEBUG_INFO, "Configure DDC for DDI-3\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_CLK (3), 0);
    DEBUG ((DEBUG_INFO, "CTRL CLK Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_DATA (3), 0);
    DEBUG ((DEBUG_INFO, "CTRL DATA Status = %r\n", Status));
  }

  //
  // DDI Port 4
  //
  if (IGpuPreMemConfig->DdiConfiguration.DdiPort4Ddc == DdiDdcEnable) {
    DEBUG ((DEBUG_INFO, "Configure DDC for DDI-4\n"));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_CLK (4), 0);
    DEBUG ((DEBUG_INFO, "CTRL CLK Status = %r\n", Status));
    Status = ConfigureDisplayGpio (GPIOV2_SIGNAL_DDP_CTRL_DATA (4), 0);
    DEBUG ((DEBUG_INFO, "CTRL DATA Status = %r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "%a: End\n", __FUNCTION__));
}

/**
  IGpuDisplayInitPreMem: Initialize the Display in PreMem phase

  @param[in] IGpuPreMemConfig        - IGPU_PEI_PREMEM_CONFIG to access the IGpuConfig related information

**/
VOID
IGpuDisplayInitPreMem (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  )
{
  IP_IGPU_INST             *IGpuInst;
  EFI_PREMEM_GRAPHICS_PPI  *GraphicsPreMemPpi;
  EFI_STATUS               Status;
  EFI_BOOT_MODE            BootMode;
  EFI_HOB_GUID_TYPE        *GuidHob;
  CHAR8                    *String;
  UINT64                   StartTime;
  IGPU_DATA_HOB            *IGpuDataHob;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  if (IGpuInst->Signature != IP_IGPU_SIGNATURE) {
    CpuDeadLoop ();
  }

  ///
  /// Get the system Boot mode.
  //
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Check and Enable Panel Power (Vdd Bit)
  ///
  IpIGpuCheckAndForceVddOn (IGpuInst, BootMode);
  ///
  /// Initialize Display Native GPIO's
  ///
  ProgramDisplayNativeGpioInit (IGpuPreMemConfig);

  ///
  /// End of Display Init Pre Mem Signal
  /// Install gIGpuDisplayInitPreMemDonePpi for OEM use.
  ///
  PeiServicesInstallPpi (&gIGpuDisplayInitPreMemDonePpi);

  //
  // VGA Initialization
  //
  if (IpIGpuSupported (IGpuInst) == TRUE) {
    if (IGpuPreMemConfig->VgaInitControl != VGA_DISPLAY_DISABLED) {
      Status = PeiServicesLocatePpi (&gIntelPeiPreMemGraphicsPpiGuid, 0, NULL, (VOID **)&GraphicsPreMemPpi);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Failed to locate Ppi %g\n", &gIntelPeiPreMemGraphicsPpiGuid));
        return;
      }

      //
      // Enable IO Bar on 0:2:0
      //
      IpIGpuEnableIoCmdReg (IGpuInst);

      //
      // Log the uGOP Timings
      //
      StartTime = GetFspCurrentTime ();

      Status = GraphicsPreMemPpi->GraphicsPreMemPpiInit (IGpuPreMemConfig->VbtPtr);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "GraphicsPreMemPpiInit Failed\n"));
        return;
      }

      IGpuDataHob = (IGPU_DATA_HOB *)GetFirstGuidHob (&gIGpuDataHobGuid);
      if (IGpuDataHob != NULL) {
        IGpuDataHob->VgaDisplayConfig = IGpuPreMemConfig->VgaInitControl;
      }

      String = IGpuPreMemConfig->VgaMessage;
      if (IS_VGA_MODE3_ENABLED (IGpuPreMemConfig->VgaInitControl)) {
        GraphicsPreMemPpi->GraphicsPreMemPpiVgaWrite (VGA_TEXT_CENTER, 12, String);
      } else if (IS_VGA_MODE12_ENABLED (IGpuPreMemConfig->VgaInitControl)) {
        if ((IGpuPreMemConfig->Mode12Info.LogoPixelWidth != 0) && (IGpuPreMemConfig->Mode12Info.LogoPixelHeight != 0) && (IGpuPreMemConfig->Mode12Info.VgaMode12ImagePtr != NULL)) {
          VgaMode12DrawImage (IGpuPreMemConfig->Mode12Info.LogoXPosition, IGpuPreMemConfig->Mode12Info.LogoYPosition, IGpuPreMemConfig->Mode12Info.LogoPixelWidth, IGpuPreMemConfig->Mode12Info.LogoPixelHeight, (VOID *)IGpuPreMemConfig->Mode12Info.VgaMode12ImagePtr);
        }
      }

      //
      // Log uGOP timing in FSP
      //
      LogFspPerformanceData (FspuGopPerf, StartTime);
    }
  }

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  Initialize the Display IP in Post Mem phase

**/
VOID
IGpuDisplayInitPostMem (
  VOID
  )
{
  IP_IGPU_INST       *IGpuInst;
  EFI_HOB_GUID_TYPE  *GuidHob;

  DEBUG ((DEBUG_INFO, "%a() Start\n", __FUNCTION__));

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return;
  }

  if (IGpuInst->Signature != IP_IGPU_SIGNATURE) {
    ASSERT (FALSE);
    return;
  }

  ///
  /// Display MemSS Config Init
  ///
  IpIGpuDisplayMemSsInit (IGpuInst);
  //
  // Program Flat CCS Base Address register in Display
  //
  IpIGpuProgramFlatCcsBaseForDisplay (IGpuInst);

  DEBUG ((DEBUG_INFO, "%a() End\n", __FUNCTION__));
}

/**
  IGpuPeiNotifyCallback to call the GOP and fill the display.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
IGpuPeiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS         Status;
  UINT64             GtApertureSize;
  UINT64             GtApertureAdr;
  IP_IGPU_INST       *IGpuInst;
  EFI_HOB_GUID_TYPE  *GuidHob;

  GtApertureAdr = 0;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_PEI_IGPU_NTFYCBK_ENTRY); // PostCode (0xA04)

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  ///
  /// Initialize Gttmmadr and Aperture BAR
  ///
  IpIGpuProgramGttMmAdr (IGpuInst);
  IpIGpuProgramLMemBar (IGpuInst);

  IpIGpuEnableCmdReg (IGpuInst);

  GtApertureAdr  = IpIGpuGetLMemBar (IGpuInst);
  GtApertureSize = IpIGpuGetLMemBarSize (IGpuInst);
  DEBUG ((DEBUG_INFO, "GtApertureAdr = %lx GtApertureSize = %lx \n", GtApertureAdr, GtApertureSize));

  DEBUG ((DEBUG_INFO, "Configure FrameBuffer Memory as Write Combine before PEIM GOP load.\n"));
  Status = MtrrSetMemoryAttributeInMtrrSettings (
                                                 NULL,
                                                 GtApertureAdr,
                                                 GtApertureSize,
                                                 CacheWriteCombining
                                                 );

  DEBUG ((DEBUG_INFO, "IGpuCallPpiAndFillFrameBuffer\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_CALLPPI_AND_FILLFRAMEBUFFER);  // PostCode (0xA05)

  Status = IGpuCallPpiAndFillFrameBuffer (IGpuInst);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to Call Ppi and fill frame buffer:%r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_PEI_IGPU_NTFYCBK_EXIT);  // PostCode (0xA0F)

  return Status;
}

/**
  IGpuCallPpiAndFillFrameBuffer: Call GraphicsInitPeim PeiGraphicsPpi to initalize display and get Mode info.
  Publish GraphicsInfoHob and call FillFrameBufferAndShowLogo.

  @param[in]  IGpuInst       The instance

  @retval    EFI_OUT_OF_RESOURCES - no enough reosurce for completing this task.
  @retval    EFI_STATUS - status code from calling several sub functions.
**/
EFI_STATUS
EFIAPI
IGpuCallPpiAndFillFrameBuffer (
  IP_IGPU_INST  *IGpuInst
  )
{
  EFI_STATUS                         Status;
  EFI_PEI_GRAPHICS_PPI               *IGpuInitPpi;
  EFI_PEI_GRAPHICS_INFO_HOB          *PlatformGraphicsOutput;
  EFI_PEI_GRAPHICS_DEVICE_INFO_HOB   *IGpuDeviceInfoHob;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  *Mode = NULL;

  DEBUG ((DEBUG_INFO, "%a Begin\n", __FUNCTION__));

  Status = EFI_SUCCESS;
  ///
  /// Locate IGpuInit Ppi
  ///
  Status = PeiServicesLocatePpi (&gEfiPeiGraphicsPpiGuid, 0, NULL, (VOID **)&IGpuInitPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Failed to locate Ppi GraphicsPpiInit and GraphicsPpiGetMode. \n"));
    return Status;
  }

  ///
  /// Call PeiGraphicsPpi.GraphicsPpiInit to initilize the display
  ///
  DEBUG ((DEBUG_INFO, "GraphicsPpiInit Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_IGPU_PPI_INIT_START); // PostCode (0xA06)
  Status = IGpuInitPpi->GraphicsPpiInit ((VOID *)IGpuInst->IGpuConfig.PeiDisplayConfig.GraphicsConfigPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "GraphicsPpiInit failed. \n"));
    SendFspErrorInfo (
                      gSaFspErrorTypeCallerId,
                      gSaFspErrorTypePeiGopInit,
                      EfiStatusToUint32 (Status)
                      );
    return Status;
  }

  Mode = AllocateZeroPool (sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
  if (Mode == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Mode->Info = AllocateZeroPool (sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  if (Mode->Info == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Call PeiGraphicsPpi.GraphicsPpiGetMode to get display resolution
  ///
  DEBUG ((DEBUG_INFO, "GraphicsPpiGetMode Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_GRPAHICS_PPIGETMODE_START); // PostCode (0xA07)
  Status = IGpuInitPpi->GraphicsPpiGetMode (Mode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "GraphicsPpiGetMode failed. \n"));
    SendFspErrorInfo (
                      gSaFspErrorTypeCallerId,
                      gSaFspErrorTypePeiGopGetMode,
                      EfiStatusToUint32 (Status)
                      );
    return Status;
  }

  ///
  /// Print Mode information recived from GraphicsPeim
  ///
  DEBUG ((DEBUG_INFO, "MaxMode:0x%x\n", Mode->MaxMode));
  DEBUG ((DEBUG_INFO, "Mode:0x%x\n", Mode->Mode));
  DEBUG ((DEBUG_INFO, "SizeOfInfo:0x%x\n", Mode->SizeOfInfo));
  DEBUG ((DEBUG_INFO, "FrameBufferBase:0x%lx\n", Mode->FrameBufferBase));
  DEBUG ((DEBUG_INFO, "FrameBufferSize:0x%x\n", Mode->FrameBufferSize));
  DEBUG ((DEBUG_INFO, "Version:0x%x\n", Mode->Info->Version));
  DEBUG ((DEBUG_INFO, "HorizontalResolution:0x%d\n", Mode->Info->HorizontalResolution));
  DEBUG ((DEBUG_INFO, "VerticalResolution:0x%x\n", Mode->Info->VerticalResolution));
  DEBUG ((DEBUG_INFO, "PixelFormat:0x%x\n", Mode->Info->PixelFormat));
  DEBUG ((DEBUG_INFO, "PixelsPerScanLine:0x%x\n", Mode->Info->PixelsPerScanLine));

  ///
  /// Publish GraphicsInfoHob and GraphicsDeviceInfoHob to be used by platform code
  ///
  PlatformGraphicsOutput = BuildGuidHob (&gEfiGraphicsInfoHobGuid, sizeof (EFI_PEI_GRAPHICS_INFO_HOB));

  if (PlatformGraphicsOutput == NULL) {
    DEBUG ((DEBUG_WARN, "Failed to build GraphicsInfoHob\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  PlatformGraphicsOutput->GraphicsMode.Version              = Mode->Info->Version;
  PlatformGraphicsOutput->GraphicsMode.HorizontalResolution = Mode->Info->HorizontalResolution;
  PlatformGraphicsOutput->GraphicsMode.VerticalResolution   = Mode->Info->VerticalResolution;
  PlatformGraphicsOutput->GraphicsMode.PixelFormat          = Mode->Info->PixelFormat;
  PlatformGraphicsOutput->GraphicsMode.PixelInformation     = Mode->Info->PixelInformation;
  PlatformGraphicsOutput->GraphicsMode.PixelsPerScanLine    = Mode->Info->PixelsPerScanLine;
  PlatformGraphicsOutput->FrameBufferBase                   = Mode->FrameBufferBase;
  PlatformGraphicsOutput->FrameBufferSize                   = (UINT32)Mode->FrameBufferSize;

  IGpuDeviceInfoHob = BuildGuidHob (&gEfiGraphicsDeviceInfoHobGuid, sizeof (EFI_PEI_GRAPHICS_DEVICE_INFO_HOB));
  if (IGpuDeviceInfoHob == NULL) {
    DEBUG ((DEBUG_WARN, "Failed to build IGpuDeviceInfoHob\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  IGpuDeviceInfoHob->VendorId          = IpIGpuGetVendorId (IGpuInst);
  IGpuDeviceInfoHob->DeviceId          = IpIGpuGetDeviceId (IGpuInst);
  IGpuDeviceInfoHob->SubsystemVendorId = IpIGpuGetSubsystemVendorId (IGpuInst);
  IGpuDeviceInfoHob->SubsystemId       = IpIGpuGetSubsystemId (IGpuInst);
  IGpuDeviceInfoHob->RevisionId        = IpIGpuGetRevisionId (IGpuInst);
  IGpuDeviceInfoHob->BarIndex          = 1; // Second BAR is for framebuffer in IGPU case.

  ///
  /// Install PeiGraphicsFramebufferReadyPpi
  ///
  Status = PeiServicesInstallPpi (&mIGpuFramebufferReadyPpi);
  ASSERT_EFI_ERROR (Status);
  ///
  /// Display Logo if user provides valid Bmp image
  ///
  if (IGpuInst->IGpuConfig.PeiDisplayConfig.BltBufferAddress != 0) {
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_SA_DISPLAY_LOGO); // PostCode (0xA08)
    Status = IGpuFillFrameBufferAndShowLogo (IGpuInst, Mode);
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}

/**
  Fill frame buffer with the image.

  @param[in]  IGpuInst       The instance

  @retval    EFI_STATUS
**/
EFI_STATUS
IGpuFillFrameBufferAndShowLogo (
  IP_IGPU_INST                       *IGpuInst,
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  *Mode
  )
{
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *Blt;
  UINTN                          Height;
  UINTN                          Width;
  EFI_STATUS                     Status;
  UINTN                          LogoDestX;
  UINTN                          LogoDestY;
  UINTN                          SrcY, DstY;
  UINT8                          *SrcAddress;
  UINT8                          *DstAddress;
  UINT32                         BytesPerScanLine;
  UINT32                         VerticalResolution;
  UINT32                         HorizontalResolution;

  DEBUG ((DEBUG_INFO, "%a Begin\n", __FUNCTION__));

  Blt    = NULL;
  Status = EFI_SUCCESS;
  if (Mode == NULL) {
    DEBUG ((DEBUG_INFO, "Returning from %a due to invalid mode\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  //
  // frame buffer with the image
  //
  Width  = IGpuInst->IGpuConfig.PeiDisplayConfig.LogoPixelWidth;
  Height = IGpuInst->IGpuConfig.PeiDisplayConfig.LogoPixelHeight;
  Blt    = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)(UINTN)IGpuInst->IGpuConfig.PeiDisplayConfig.BltBufferAddress;

  if ((IGpuInst->IGpuConfig.PeiDisplayConfig.HorizontalResolution == 0) && (IGpuInst->IGpuConfig.PeiDisplayConfig.VerticalResolution == 0)) {
    DEBUG ((DEBUG_INFO, "Default  value for Resolutions are 0, Overring with default values\n"));
    HorizontalResolution = Mode->Info->HorizontalResolution;
    VerticalResolution   = Mode->Info->VerticalResolution;
  } else {
    HorizontalResolution = IGpuInst->IGpuConfig.PeiDisplayConfig.HorizontalResolution;
    VerticalResolution   = IGpuInst->IGpuConfig.PeiDisplayConfig.VerticalResolution;
  }

  //
  // if Convert Bmp to blt successful Center the logo and fill frame buffer.
  //
  LogoDestX        = (HorizontalResolution - Width) / 2;
  LogoDestY        = (VerticalResolution - Height) / 2;
  BytesPerScanLine = Mode->Info->PixelsPerScanLine * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);

  DEBUG ((DEBUG_INFO, "Logo Width = 0x%x\n", Width));
  DEBUG ((DEBUG_INFO, "Logo Height = 0x%x\n", Height));
  DEBUG ((DEBUG_INFO, "Buffer Blt = 0x%x\n", Blt));
  DEBUG ((DEBUG_INFO, "HorizontalResolution = 0x%x\n", HorizontalResolution));
  DEBUG ((DEBUG_INFO, "VerticalResolution = 0x%x\n", VerticalResolution));
  DEBUG ((DEBUG_INFO, "Logo Dest X = 0x%x\n", LogoDestX));
  DEBUG ((DEBUG_INFO, "Logo Dest Y = 0x%x\n", LogoDestY));
  DEBUG ((DEBUG_INFO, "BytesPerScanLine = 0x%x\n", BytesPerScanLine));

  //
  // Fill framebuffer with the logo line by line
  //
  for (SrcY = 0, DstY = LogoDestY; DstY < (LogoDestY + Height); SrcY++, DstY++) {
    DstAddress = (UINT8 *)(UINTN)(Mode->FrameBufferBase + DstY * BytesPerScanLine + LogoDestX * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    SrcAddress = (UINT8 *)((UINT8 *)Blt + (SrcY * Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)));
    CopyMem (DstAddress, SrcAddress, Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}

/**
  IGpuPeiDisplayInit: Initialize the IGD using GraphicsInitPeim and display logo in PEI.

  @param[in] IGpuPreMemConfig  - IGPU_PEI_PREMEM_CONFIG to access the IGpuPreMemConfig related information
  @param[in] IGpuConfig        - IGPU_PEI_CONFIG to access the IGpuConfig related information

  @retval     EFI_STATUS
**/
EFI_STATUS
IGpuPeiDisplayInit (
  IN   IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig,
  IN   IGPU_PEI_CONFIG         *IGpuConfig
  )
{
  EFI_STATUS     Status;
  IGPU_DATA_HOB  *IGpuDataHob;

  DEBUG ((DEBUG_INFO, "%a Begin\n", __FUNCTION__));
  IGpuDataHob = NULL;

  //
  // Get the HOB for Gfx Data
  //
  IGpuDataHob = (IGPU_DATA_HOB *)GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob == NULL) {
    DEBUG ((DEBUG_ERROR, "IGpu Data Hob not found\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// Return if Graphics not supported or not enabled
  ///
  if (IGpuIsSupported () == FALSE) {
    DEBUG ((DEBUG_INFO, "Returning from %a since IGD is not supported or not enabled\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  ///
  /// Return if Display Engine is not present
  ///
  if (IGpuIsDisplayPresent () == FALSE) {
    DEBUG ((DEBUG_INFO, "Exit %a since Display is not present\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  if (IGpuConfig->PeiDisplayConfig.PeiGraphicsPeimInit == 0) {
    return EFI_SUCCESS;
  }

  if (IGpuConfig->PeiDisplayConfig.GraphicsConfigPtr == NULL) {
    DEBUG ((DEBUG_INFO, "Bad VBT Pointer passed\n"));
    return EFI_SUCCESS;
  }

  ///
  /// Return if Primary Display is  not set IGfx and External Gfx not connected
  ///
  if ((IGpuPreMemConfig->PrimaryDisplay != DISPLAY_IGD) && (IGpuDataHob->PrimaryDisplayDetection == DISPLAY_PCI)) {
    return EFI_SUCCESS;
  }

  ///
  /// Register notify to on PeiGraphicsPpi
  ///
  Status = PeiServicesNotifyPpi (&mPeiIGpuNotifyList);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return Status;
}

/**
  Initialize PAVP feature of Graphics.

  @param[in] IGpuPreMemConfig     - IGPU_PEI_CONFIG to access the IGpuPreMemConfig related information
  @param[in] IGpuConfig           - IGPU_PEI_CONFIG to access the IGpuConfig related information

  @retval     EFI_STATUS
**/
EFI_STATUS
IGpuPavpInit (
  IN  IGPU_PEI_CONFIG         *IGpuConfig,
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  )
{
  EFI_STATUS         Status;
  IP_IGPU_INST       *IGpuInst;
  UINT32             GMSSizeSelector;
  UINT32             GMSSize;
  EFI_HOB_GUID_TYPE  *GuidHob;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  GuidHob = GetFirstGuidHob (&gIGpuInstHobGuid);
  if (GuidHob != NULL) {
    IGpuInst = (IP_IGPU_INST *)GET_GUID_HOB_DATA (GuidHob);
  } else {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  if (IpIGpuIsInstValid (IGpuInst) == FALSE) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  GMSSizeSelector = (UINT32)GetHostBridgeRegisterData (HostBridgeGgcCfgReg, HostBridgeGgcDsmSizeSelector);
  if (GMSSizeSelector < 240 ) {
    GMSSize = (UINT32)GMSSizeSelector * 32;
  } else {
    GMSSize = 4 * (GMSSizeSelector - 239);
  }

  IGpuInst->IGpuPrivateConfig.DsmSize          = GMSSize;
  IGpuInst->IGpuPrivateConfig.GmsBase          = (UINT32)GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmFullData);

  Status = IpIGpuPavpInit (IGpuInst);

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return Status;
}
