/** @file
  Initializes the ACE.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PeiHdaLib.h>
#include <Library/PeiAceInitLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Ppi/SiPolicy.h>
#include <Register/AceRegs.h>
#include <IntelRcStatusCode.h>
#include <PcieRegs.h>
#include <HdaCtrl.h>
#include <Hda.h>

#define ACE_MAX_LOOP_TIME_MS        2000
#define ACE_WAIT_PERIOD_MS          5

#define TC_USB_CHANNEL_NUMBER       3

#define ACE_HDA_SDI_0_HDALINK       0
#define ACE_HDA_SDI_1_HDALINK       1
#define ACE_HDA_SDI_2_IDISPLINK     2
#define ACE_MAX_SDI_NUMBER          3
#define ACE_MAX_SDI_MASK            ((1 << ACE_MAX_SDI_NUMBER) - 1)

#define GET_PARAMETER_VERB_ID       0xF00
#define GET_BCLK_FREQUENCY_VERB_ID  0xF37

// [31:28] - CodecAddress
// [27:20] - NodeId
// [19:0]  - Verb Payload
#define CODEC_COMMAND_DATA(CodecAddress, NodeId, VerbPayload) \
          ((CodecAddress << 28) | (NodeId << 20) | VerbPayload)
#define CODEC_VENDOR_ID(AzaliaSdiNum) \
          CODEC_COMMAND_DATA (AzaliaSdiNum, 0x0, ((GET_PARAMETER_VERB_ID << 8) | 0x00))
#define CODEC_REVISION_ID(AzaliaSdiNum) \
          CODEC_COMMAND_DATA (AzaliaSdiNum, 0x0, ((GET_PARAMETER_VERB_ID << 8) | 0x02))
#define BCLK_FREQ_SUPPORT(CodecSdiNum) \
          CODEC_COMMAND_DATA (CodecSdiNum, 0x0, ((GET_PARAMETER_VERB_ID << 8) | 0x16))
#define BCLK_FREQUENCY(CodecSdiNum) \
          CODEC_COMMAND_DATA (CodecSdiNum, 0x0, ((GET_BCLK_FREQUENCY_VERB_ID << 8) | 0x00))

#define MIC_PRIVACY_MASK_SNDW0   0
#define MIC_PRIVACY_MASK_SNDW1   1
#define MIC_PRIVACY_MASK_SNDW2   2
#define MIC_PRIVACY_MASK_SNDW3   3
#define MIC_PRIVACY_MASK_SNDW4   4
#define MIC_PRIVACY_MASK_DMIC    7

/*
  Function Programm Microphone Privacy Setting in HW mode.
  @param[in] HdaController   Pointer to Hda Controller
  @param[in] HdaConfig       Pointer to Hda Policy
*/
VOID
AceConfigureMicPrivacy (
  IN HDA_CONTROLLER            *HdaController,
  IN HDAUDIO_CONFIG            *HdaConfig
  )
{
  UINT8  HwModeLinkMask;

  HwModeLinkMask = 0;

  if (HdaConfig->MicPrivacy.Mode != HdaNoMicrophonePrivacySupport) {
    if ((HdaConfig->MicPrivacy.Mode == HdaHwManagedMicrophonePrivacy) || (HdaConfig->MicPrivacy.Mode == HdaFwManagedMicrophonePrivacy)) {
      //
      // Program the time-out duration to wait before forcing the actual microphone privacy DMA data zeroing.
      // Unit in number of RTC clocks.
      // Valid and static when DDZE = 10. For DDZE = 0x or 11 case, time-out is not necessary as it will not be enabled or force mic disable statically.
      // Locked when DDZPL = 1.
      //
      MmioAndThenOr32 (
        HdaController->DspMmioAddress + R_ACE_MEM_DFMICPVCP,
        (UINT32) (~B_ACE_MEM_DFMICPVCP_DDZWT),
        (UINT32) ((HdaConfig->MicPrivacy.Timeout * 32) & B_ACE_MEM_DFMICPVCP_DDZWT) // timeout (ms) * RTC clock (32kHz)
        );
      DEBUG ((DEBUG_INFO, "Microphone privacy timeout value: %d.\n", MmioRead32 (HdaController->DspMmioAddress + R_ACE_MEM_DFMICPVCP) & B_ACE_MEM_DFMICPVCP_DDZWT));
    }

    if ((HdaConfig->MicPrivacy.Deglitch == TRUE)) {
      //
      // De-glitcher enable for privacy signaling GPIO input running on resume clock domain.
      // Locked when DDZPL = 1.
      //
      MmioOr32 (HdaController->DspMmioAddress + R_ACE_MEM_DFMICPVCP, (UINT32) B_ACE_MEM_DFMICPVCP_DGE);
      DEBUG ((DEBUG_INFO, "Microphone privacy: De-glitcher enabled.\n"));
    }

    if (HdaConfig->MicPrivacy.Mode == HdaHwManagedMicrophonePrivacy || HdaConfig->MicPrivacy.Mode == HdaForceMicrophoneMute) {
      //
      // Select 1 or more audio link to apply the microphone privacy DMA Data Zeroing. 1 bit per audio link.
      // [6:0]: SoundWire link segment
      // [7]: DMIC
      // Valid and static when DDZE = 1x.
      // Locked when DDZPL = 1.
      //
      HwModeLinkMask = (((!!HdaConfig->MicPrivacy.HwModeSoundWire0) << MIC_PRIVACY_MASK_SNDW0) |
                        ((!!HdaConfig->MicPrivacy.HwModeSoundWire1) << MIC_PRIVACY_MASK_SNDW1) |
                        ((!!HdaConfig->MicPrivacy.HwModeSoundWire2) << MIC_PRIVACY_MASK_SNDW2) |
                        ((!!HdaConfig->MicPrivacy.HwModeSoundWire3) << MIC_PRIVACY_MASK_SNDW3) |
                        ((!!HdaConfig->MicPrivacy.HwModeSoundWire4) << MIC_PRIVACY_MASK_SNDW4) |
                        ((!!HdaConfig->MicPrivacy.HwModeDmic) << MIC_PRIVACY_MASK_DMIC));

      DEBUG ((DEBUG_INFO, "Microphone privacy link mask: %d.\n", HwModeLinkMask));
      MmioAndThenOr32 (
        HdaController->DspMmioAddress + R_ACE_MEM_DFMICPVCP,
        (UINT32) (~B_ACE_MEM_DFMICPVCP_DDZLS),
        (UINT32) ((HwModeLinkMask << N_ACE_MEM_DFMICPVCP_DDZLS) & B_ACE_MEM_DFMICPVCP_DDZLS)
        );
    }

    //
    // Indicates the policy setting for HW to force the microphone privacy DMA data zeroing.
    // 00: Disabled (Default)
    // 10: Enabled (Microphone disable dynamically depending on privacy signaling input)
    // 11: Enabled (Force microphone disable statically)
    // Locked when DDZPL = 1.
    //
    if ((HdaConfig->MicPrivacy.Mode == HdaHwManagedMicrophonePrivacy) || (HdaConfig->MicPrivacy.Mode == HdaFwManagedMicrophonePrivacy)) {
      //
      // Microphone disable dynamically depending on privacy signaling input.
      //
      MmioAndThenOr32 (HdaController->DspMmioAddress + R_ACE_MEM_DFMICPVCP, (UINT32) ~B_ACE_MEM_DFMICPVCP_DDZE, (UINT32) (2 << N_ACE_MEM_DFMICPVCP_DDZE));
    } else if (HdaConfig->MicPrivacy.Mode == HdaForceMicrophoneMute) {
      //
      // Force microphone disable statically.
      //
      MmioAndThenOr32 (HdaController->DspMmioAddress + R_ACE_MEM_DFMICPVCP, (UINT32) ~B_ACE_MEM_DFMICPVCP_DDZE, (UINT32) (3 << N_ACE_MEM_DFMICPVCP_DDZE));
    }
  }

  if (HdaConfig->MicPrivacy.Mode != HdaFwManagedMicrophonePrivacy) {
    //
    // When set to 1, it locks the privacy DMA data zeroing policy setting.
    //
    MmioOr32 (HdaController->DspMmioAddress + R_ACE_MEM_DFMICPVCP, (UINT32) B_ACE_MEM_DFMICPVCP_DDZPL);
    DEBUG ((DEBUG_INFO, "Microphone privacy policy locked.\n"));
  }
}

/**
  Check if DSP is disbaled or not by fuses.

  @param[in] HdaController        Pointer to Hda Controller structure

  @retval TRUE                    Audio DSP is not present
  @retval FALSE                   Audio DSP is present
**/
BOOLEAN
AceIsDspFusedOut (
  HDA_CONTROLLER               *HdaController
  )
{
  if (HdaController == NULL) {
    return TRUE;
  }

  if (PciSegmentRead16 (HdaController->PciCfgBaseAddr + R_ACE_CFG_VID) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "HDA not present!\n"));
    return TRUE;
  }

  DEBUG ((DEBUG_INFO, "R_ACE_MEM_FUSVAL: 0x%x\n", MmioRead32 (HdaController->HdaMmioAddress + R_ACE_MEM_FUSVAL)));

  if (MmioRead32 (HdaController->HdaMmioAddress + R_ACE_MEM_FUSVAL) & B_ACE_MEM_FUSVAL_DSPSD) {
    return TRUE;
  }

  return FALSE;
}

/**
  Send the command to the codec via the Immediate Command mechanism.

  @param[in]      HdaController       Pointer to Hda device structure
  @param[in, out] CodecCommandData    Pointer to the Codec Command to be sent to the codec
  @param[in]      ReadBack            Whether to get the response received from the codec

  @retval EFI_DEVICE_ERROR            Device status error, operation failed
  @retval EFI_SUCCESS                 The function completed successfully
  @retval EFI_TIMEOUT                 Polling bits timed out
**/
EFI_STATUS
AceSendHdaCodecCommand (
  IN      HDA_CONTROLLER  *HdaController,
  IN OUT  UINT32          *CodecCommandData,
  IN      BOOLEAN         ReadBack
  )
{
  EFI_STATUS          Status;
  UINT32              LoopTime;

  Status = StatusPolling16 (HdaController->HdaMmioAddress + R_ACE_MEM_ICS, (UINT16) B_ACE_MEM_ICS_ICB, (UINT16) 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ICB bit is not zero before SendCodecCommand!\n"));
    return EFI_DEVICE_ERROR;
  }

  MmioWrite32 (HdaController->HdaMmioAddress + R_ACE_MEM_IC, *CodecCommandData);
  DEBUG ((DEBUG_INFO, "IC value: 0x%x.\n", *CodecCommandData));
  MmioOr16 ((HdaController->HdaMmioAddress + R_ACE_MEM_ICS), (UINT16) ((B_ACE_MEM_ICS_IRV | B_ACE_MEM_ICS_ICB)));

  Status = StatusPolling16 (HdaController->HdaMmioAddress + R_ACE_MEM_ICS, (UINT16) B_ACE_MEM_ICS_ICB, (UINT16) 0);
  if (EFI_ERROR (Status)) {
    MmioAnd16 ((HdaController->HdaMmioAddress + R_ACE_MEM_ICS), (UINT16) ~(B_ACE_MEM_ICS_ICB));
    return Status;
  }

  if (ReadBack == TRUE) {
    for (LoopTime = 0; LoopTime < ACE_MAX_LOOP_TIME_MS; LoopTime++) {
      if ((MmioRead16 (HdaController->HdaMmioAddress + R_ACE_MEM_ICS) & B_ACE_MEM_ICS_IRV) != 0) {
        *CodecCommandData = MmioRead32 (HdaController->HdaMmioAddress + R_ACE_MEM_IR);
        DEBUG ((DEBUG_INFO, "IR value: 0x%x.\n", *CodecCommandData));
        MmioAnd16 (HdaController->HdaMmioAddress + R_ACE_MEM_ICS,(UINT16) ~(B_ACE_MEM_ICS_IRV));
        break;
      } else {
        MicroSecondDelay (ACE_WAIT_PERIOD_MS);
      }
    }

    if (LoopTime >= ACE_MAX_LOOP_TIME_MS) {
      DEBUG ((DEBUG_WARN, "SendCodecCommand: ReadBack timeout!\n"));
      return EFI_DEVICE_ERROR;
    }
  }

  return EFI_SUCCESS;
}

/**
  For each codec, a predefined codec verb table should be programmed.

  @param[in]  HdaController         Pointer to Hda device structure
  @param[in]  AzaliaSdiNum          Azalia SDI Line Number
  @param[out] CodecVendorId         Codec Vendor Id
  @param[out] CodecRevisionId       Codec Revision Id

  @retval EFI_SUCCESS               The function completed successfully
  @retval EFI_DEVICE_ERROR          Device status error, operation failed
  @retval EFI_TIMEOUT               Polling bits timed out
**/
EFI_STATUS
AceGetHdaCodecId (
  IN  HDA_CONTROLLER           *HdaController,
  IN  UINT8                    AzaliaSdiNum,
  OUT UINT32*                  CodecVendorId,
  OUT UINT32*                  CodecRevisionId
  )
{
  EFI_STATUS  Status;

  ///
  /// Verb:  31~28   27  26~20                   19~0
  ///          CAd    1    NID  Verb Command and data
  ///        0/1/2
  ///
  /// Read the Vendor ID/Device ID pair from the attached codec (ParameterId = 0x00)
  ///
  *CodecVendorId = CODEC_VENDOR_ID (AzaliaSdiNum);
  Status = AceSendHdaCodecCommand (HdaController, CodecVendorId, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "SDI#%d: Reading the Codec Vendor ID/Device ID fail!\n", AzaliaSdiNum));
    return Status;
  }

  ///
  /// Read the Revision ID from the attached codec (ParameterId = 0x02)
  ///
  *CodecRevisionId = CODEC_REVISION_ID (AzaliaSdiNum);
  Status = AceSendHdaCodecCommand (HdaController, CodecRevisionId, TRUE);
  if (EFI_ERROR (Status)) {
    *CodecRevisionId = 0xFF;
    DEBUG ((DEBUG_WARN, "SDI#%d: Reading the Codec Revision ID fail! Assume Codec Revision ID: 0x%x.\n", AzaliaSdiNum, *CodecRevisionId));
  } else {
    *CodecRevisionId = (*CodecRevisionId >> 8) & 0xFF;
  }

  return EFI_SUCCESS;
}

/**
  Locates HD Audio codec verb table.

  @param[in] HdaConfig            Hda Configuration
  @param[in] VendorDeviceId       Codec vendor ID  and device ID
  @param[in] RevisionId           Codec revision ID
  @param[in] SdiNum               SDI channel number

  @retval HDAUDIO_VERB_TABLE      Pointer to the HDA Verb Table
  @retval NULL                    Table not found.
**/
HDAUDIO_VERB_TABLE*
AceLocateVerbTable (
  IN  HDAUDIO_CONFIG          *HdaConfig,
  IN  UINT32                  VendorDeviceId,
  IN  UINT8                   RevisionId,
  IN  UINT8                   SdiNum
  )
{
  HDAUDIO_VERB_TABLE         *VerbTable;
  UINTN                      Num;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if (VendorDeviceId == 0) {
    DEBUG ((DEBUG_INFO, "Vendor Device Id == 0!.\n"));
    return NULL;
  }

  VerbTable = NULL;

  for (Num = 0; Num < HdaConfig->VerbTableEntryNum; ++Num) {
    VerbTable = ((HDAUDIO_VERB_TABLE **)(UINTN) (HdaConfig->VerbTablePtr)) [Num];
    if ((VerbTable != NULL) &&
        ((((UINT32) (VerbTable->Header.VendorId) << 16) | VerbTable->Header.DeviceId) == VendorDeviceId) &&
        ((VerbTable->Header.RevisionId == 0xFF) || (VerbTable->Header.RevisionId == RevisionId)) &&
        ((VerbTable->Header.SdiNum == 0xFF) || VerbTable->Header.SdiNum == SdiNum)) {
      break;
    }
  }

  if (Num >= HdaConfig->VerbTableEntryNum) {
    VerbTable = NULL;
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));

  return VerbTable;
}

/**
  For each codec a predefined codec verb table should be programmed.

  @param[in]  HdaController       Pointer to Hda device structure
  @param[in]  HdaConfig           Pointer to Hda Configuration
  @param[in]  AzaliaSdiNum        Azalia SDI Line Number
  @param[out] CodecVendorId       Codec Vendor Id
  @param[out] CodecRevisionId     Codec Revision Id

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_UNSUPPORTED         Verb table for codec does not exist
**/
EFI_STATUS
AceMatchAndSendHdaCodecVerbTable (
  IN  HDA_CONTROLLER         *HdaController,
  IN  HDAUDIO_CONFIG         *HdaConfig,
  IN  UINT8                  AzaliaSdiNum,
  OUT UINT32                 CodecVendorId,
  OUT UINT32                 CodecRevisionId
  )
{
  EFI_STATUS                 Status;
  HDAUDIO_VERB_TABLE         *VerbTable;
  UINT32                     CodecCmdData;
  UINT32                     Index;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a () - Start. [SDI: %d]\n", __FUNCTION__, AzaliaSdiNum));

  ///
  /// Locate Verb Table and initialize detected codec
  ///
  VerbTable = AceLocateVerbTable (HdaConfig, CodecVendorId, (UINT8) CodecRevisionId, AzaliaSdiNum);

  if (VerbTable == NULL) {
    DEBUG ((DEBUG_WARN, "SDI#%d: No matching HD-Audio codec verb table found for codec (0x%08X).\n", AzaliaSdiNum, CodecVendorId));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO,
          "Found Verb Table for VendorID 0x%04X, DeviceId 0x%04X,"
          " RevisionID 0x%02X (SDI:%X, size: %d dwords)\n",
          VerbTable->Header.VendorId,
          VerbTable->Header.DeviceId,
          VerbTable->Header.RevisionId,
          VerbTable->Header.SdiNum,
          VerbTable->Header.DataDwords));

  ///
  /// Send the entire list of verbs in the matching verb table one by one to the codec
  ///
  DEBUG ((DEBUG_VERBOSE, "Sending verbs to codec:\n"));
  for (Index = 0; Index < VerbTable->Header.DataDwords; ++Index) {
    CodecCmdData  = VerbTable->Data[Index];
    ASSERT ((CodecCmdData >> 28) == 0);
    ///
    /// Program CAd Field per the SDI number got during codec detection
    ///
    CodecCmdData  |= (UINT32) (AzaliaSdiNum << 28);
    Status = AceSendHdaCodecCommand (HdaController, &CodecCmdData, FALSE);
    if (EFI_ERROR (Status)) {
      ///
      /// Skip the Azalia verb table loading when find the verb table content is not
      /// properly matched with the HDA hardware, though IDs match.
      ///
      DEBUG ((DEBUG_WARN,"Error loading verb table for Azalia Codec of 0x%08X\n", CodecVendorId));
      break;
    }

    DEBUG ((DEBUG_VERBOSE, "[%03d] 0x%08X\n", Index, CodecCmdData));
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Checks if connected codec supports statically switchable BCLK clock frequency.

  @param[in]  HdaController               Pointer to Hda device structure
  @param[in]  CodecSdiNum                 SDI number to which codec is connected
  @param[out] BclkFrequencySupportMask    Bit mask of supported frequencies

  @retval EFI_NO_RESPONSE                 Verb response has not been received from codec
  @retval EFI_UNSUPPORTED                 Codec does not support frequency switching
  @retval EFI_SUCCESS                     Codec supports frequency switching, the function completed successfully
**/
EFI_STATUS
AceGetHdaCodecFrequencySwitchingSupport (
  IN  HDA_CONTROLLER      *HdaController,
  IN  UINT8               CodecSdiNum,
  OUT UINT32              *BclkFrequencySupportMask
  )
{
  EFI_STATUS      Status;
  UINT32          BclkFreqSupport;

  *BclkFrequencySupportMask = 0;

  DEBUG ((DEBUG_INFO, "%a () - Start. [SDI: %d]\n", __FUNCTION__, CodecSdiNum));

  ///
  /// Read Statically Switchable BCLK Frequency capabilities from the attached codec (VerbId = F00h, ParameterId = 16h)
  ///
  BclkFreqSupport = BCLK_FREQ_SUPPORT (CodecSdiNum);
  Status = AceSendHdaCodecCommand (HdaController, &BclkFreqSupport, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Reading the Codec BCLK Frequency Switching support failed!\n"));
    return EFI_NO_RESPONSE;
  }

  DEBUG ((DEBUG_INFO, "SDI#%d Verb F00h ParamtererId = 16h response = 0x%08x\n", CodecSdiNum, BclkFreqSupport));

  ///
  /// If the codec returns value of all 0s, it implies that it does not support static frequency switching.
  /// HDAudio-Link (SDIN 0/1) : BIT2 (24MHz) must be always set if supported
  ///
  if (BclkFreqSupport == 0 || ((BclkFreqSupport & BIT2) == 0)) {
    DEBUG ((DEBUG_INFO, "Codec Static Frequency Switching not supported!\n"));
    return EFI_UNSUPPORTED;
  }

  *BclkFrequencySupportMask = (BclkFreqSupport & 0x1F); // 1Fh = BITS[4:0] - 96/48/24/12/6 MHz
  DEBUG ((DEBUG_INFO, "Codec Static Frequency Switching supported - FrequencySupportMask = 0x%08x\n", *BclkFrequencySupportMask));

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Get codec's current BCLK clock frequency.

  @param[in]  HdaController             Pointer to Hda device structure
  @param[in]  CodecSdiNum               SDI number to which codec is connected
  @param[out] CurrentBclkFrequency      Current BCLK frequency of codec

  @retval EFI_NO_RESPONSE               Verb response has not been received from codec
  @retval EFI_NOT_FOUND                 Could not match current codec frequency with any supported frequency
  @retval EFI_SUCCESS                   Supported frequency found, the function completed successfully
**/
EFI_STATUS
AceGetHdaCodecCurrentBclkFrequency (
  IN  HDA_CONTROLLER                  *HdaController,
  IN  UINT8                           CodecSdiNum,
  OUT HDAUDIO_LINK_FREQUENCY          *CurrentBclkFrequency
  )
{
  EFI_STATUS      Status;
  UINT32          BclkFreq;
  UINT32          FreqIndex;

  *CurrentBclkFrequency = HdaLinkFreqInvalid;

  DEBUG ((DEBUG_INFO, "%a () - Start. [SDI: %d]\n", __FUNCTION__, CodecSdiNum));

  ///
  /// Read BCLK Freq from the attached codec
  ///
  BclkFreq = BCLK_FREQUENCY (CodecSdiNum);
  Status   = AceSendHdaCodecCommand (HdaController, &BclkFreq, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Error: Reading the Codec current BCLK Frequency fail!\n"));
    return EFI_NO_RESPONSE;
  }

  DEBUG ((DEBUG_INFO, "SDI#%d Verb F37h (GET_CCF) response = 0x%08x\n", CodecSdiNum, BclkFreq));

  for (FreqIndex = 0, Status = EFI_NOT_FOUND; FreqIndex < HdaLinkFreqInvalid; FreqIndex++) {
    if ((BclkFreq >> FreqIndex) & BIT0) {
      *CurrentBclkFrequency = FreqIndex;
      DEBUG ((DEBUG_INFO, "SDI#%d Codec BCLK frequency = %d\n", CodecSdiNum, *CurrentBclkFrequency));
      Status = EFI_SUCCESS;
      break;
    }
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Sets link clock frequency.
  Controller must be out of reset to set Output/Input payload registers.
  Link must be turned off before setting Link Control register, then turned on again.

  @param[in] HdaController              Pointer to Hda device structure
  @param[in] RequestedBclkFrequency     Clock frequency to be set

  @retval EFI_SUCCESS                   Requested frequency set, the function completed successfully
  @retval EFI_TIMEOUT                   Polling CPA bit timed out
**/
EFI_STATUS
AceSetHdaCodecBclkFrequency (
  IN  HDA_CONTROLLER                *HdaController,
  IN  HDAUDIO_LINK_FREQUENCY        RequestedBclkFrequency
  )
{
  EFI_STATUS  Status;
  UINT32      Data32;
  UINT32      OutputPayloadWords;
  UINT32      InputPayloadWords;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  ///
  /// HDAudio-Link specific steps
  ///
  DEBUG ((DEBUG_INFO, "HDA-Link Static BCLK Frequency Switch ID = %d - ", RequestedBclkFrequency));

  // For HDA-Link change OUTPAY and INPAY
  switch (RequestedBclkFrequency) {
    case HdaLinkFreq12MHz:
      DEBUG ((DEBUG_INFO, "12MHz\n"));
      // Output: 12MHz -> 500 bits/frame - 40 bits (command & control) = 460 bits -> 28 words of payload
      // Input : 12MHz -> 250 bits/frame - 36 bits (response) = 214 bits -> 13 words of payload
      OutputPayloadWords = 0x1C; // 28 words of payload
      InputPayloadWords  = 0x0D; // 13 words of payload
      break;
    case HdaLinkFreq6MHz:
      DEBUG ((DEBUG_INFO, "6MHz\n"));
      // Output: 6MHz -> 250 bits/frame - 40 bits (command & control) = 210 bits -> 13 words of payload
      // Input : 6MHz -> 125 bits/frame - 36 bits (response) = 89 bits -> 5 words of payload
      OutputPayloadWords = 0x0D; // 13 words of payload
      InputPayloadWords  = 0x05; // 5 words of payload
      break;
    case HdaLinkFreq24MHz:
    default:
      DEBUG ((DEBUG_INFO, "Default 24MHz\n"));
      // Default for 24MHz
      OutputPayloadWords = 0x3C; // 60 words of payload
      InputPayloadWords  = 0x1C; // Due to HW bug, this needs to be reduced by 1 word to 0x1C (28 words of payload)
      break;
  }

  //
  // Software sets this bit to '1' to turn the link on (provided CRSTB = 1), and clears it to '0' when it wishes to turn the link off.
  // When CPA matches the value of this bit, the achieved power state has been reached.  Software is expected to wait for CPA to match SPA before it can program SPA again.
  // Any deviation may result in undefined behavior.
  //
  MmioAnd32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HDALCTL), ~((UINT32) B_ACE_MEM_HDALCTL_SPA));

  Data32 = MmioRead32 (HdaController->HdaMmioAddress + R_ACE_MEM_HDALCTL);
  DEBUG ((DEBUG_ERROR, "HDALCTL after setting SPA bit to 0 : 0x%08x\n", Data32));

  //
  // This value changes to the value set by SPA when the power of the link has reached that state. Software sets SPA, then monitors CPA to know when the link has changed state.
  //
  Status = StatusPolling (HdaController->HdaMmioAddress + R_ACE_MEM_HDALCTL, (UINT32) B_ACE_MEM_HDALCTL_CPA, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Turn off the link (SPA = 0) failed! CPA polling Status = %r\n", Status));
  }

  //
  // Indicates the frequency that software wishes the link to run at.
  // Changing this value to a value not supported by Link Capabilities shall result in indeterminate results.  The possible encodings are:
  // 000: 6 MHz, 001: 12 MHz, 010: 24 MHz, 011: 48 MHz, 100: 96 MHz, 101: 192 MHz, 110 - 111: Reserved
  // The value of SCF must persist across a controller reset.
  //
  MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HDALCTL),
    ~((UINT32) (B_ACE_MEM_HDALCTL_SCF)),
      (UINT32) (RequestedBclkFrequency << N_ACE_MEM_HDALCTL_SCF));

  Data32 = MmioRead32 (HdaController->HdaMmioAddress + R_ACE_MEM_HDALCTL);
  DEBUG ((DEBUG_INFO, "HDACTL = 0x%08x\n", Data32));

  // Controller must be out of reset (CRSTB = 1) to set the following registers
  MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HDAHWI1),
          (UINT32) ~(B_ACE_MEM_HDAHWI1_INPAY | B_ACE_MEM_HDAHWI1_OUTPAY),
          (UINT32) ((InputPayloadWords << N_ACE_MEM_HDAHWI1_INPAY) | (OutputPayloadWords << N_ACE_MEM_HDAHWI1_OUTPAY)));

  DEBUG ((DEBUG_INFO, "HDAHWI1 set with value = 0x%08x\n", (UINT32) ((InputPayloadWords << N_ACE_MEM_HDAHWI1_INPAY) | (OutputPayloadWords << N_ACE_MEM_HDAHWI1_OUTPAY))));
  Data32 = MmioRead32 (HdaController->HdaMmioAddress  + R_ACE_MEM_HDAHWI1);
  DEBUG ((DEBUG_INFO, "OUTPAY (Output Payload Capability) = 0x%04x\n", ((Data32 & B_ACE_MEM_HDAHWI1_OUTPAY) >> N_ACE_MEM_HDAHWI1_OUTPAY)));
  DEBUG ((DEBUG_INFO, "INPAY (Input Payload Capability) = 0x%04x\n", ((Data32 & B_ACE_MEM_HDAHWI1_INPAY) >> N_ACE_MEM_HDAHWI1_INPAY)));

  //
  // Software sets this bit to '1' to turn the link on (provided CRSTB = 1), and clears it to '0' when it wishes to turn the link off.
  // When CPA matches the value of this bit, the achieved power state has been reached.  Software is expected to wait for CPA to match SPA before it can program SPA again.
  // Any deviation may result in undefined behavior.
  //
  MmioOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HDALCTL), (UINT32) B_ACE_MEM_HDALCTL_SPA);

  //
  // This value changes to the value set by SPA when the power of the link has reached that state. Software sets SPA, then monitors CPA to know when the link has changed state.
  //
  Status = StatusPolling (HdaController->HdaMmioAddress + R_ACE_MEM_HDALCTL, (UINT32) B_ACE_MEM_HDALCTL_CPA, (UINT32) B_ACE_MEM_HDALCTL_CPA);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Turn on the link (SPA = 1) failed! CPA polling Status = %r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Configures static link frequency for HDA links.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure
  @param[in] CodecSdiNum          SDI number to which codec is connected

  @retval EFI_NO_RESPONSE         Verb response has not been received from codec
  @retval EFI_UNSUPPORTED         Codec does not support frequency switching
  @retval EFI_NOT_FOUND           Codec current frequency does not match with any supported frequency
  @retval EFI_DEVICE_ERROR        Codec supports requested frequency, but the switching failed (default frequency set)
  @retval EFI_SUCCESS             Requested frequency successfully set (or correct frequency is already configured)
**/
EFI_STATUS
AceConfigureHdaLinkFrequency (
  IN  HDA_CONTROLLER          *HdaController,
  IN  HDAUDIO_CONFIG          *HdaConfig,
  IN  HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig,
  IN  UINT8                   CodecSdiNum
  )
{
  EFI_STATUS                  Status;
  HDAUDIO_LINK_FREQUENCY      RequestedFreq;
  HDAUDIO_LINK_FREQUENCY      CurrentFreq;
  UINT32                      FreqSupportMask;

  DEBUG ((DEBUG_INFO, "%a () - Start. [SDI: %d]\n", __FUNCTION__, CodecSdiNum));

  RequestedFreq = HdaConfig->HdAudioLinkFrequency;

  // Set inpay default to 1c and outpay
  MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HDAHWI1),
    (UINT32) ~(B_ACE_MEM_HDAHWI1_INPAY | B_ACE_MEM_HDAHWI1_OUTPAY),
    (UINT32) ((0x1c << N_ACE_MEM_HDAHWI1_INPAY) | (0x3c << N_ACE_MEM_HDAHWI1_OUTPAY))
    );

  // Set outpay default
  Status = AceGetHdaCodecFrequencySwitchingSupport (HdaController, CodecSdiNum, &FreqSupportMask);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = AceGetHdaCodecCurrentBclkFrequency (HdaController, CodecSdiNum, &CurrentFreq);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (CurrentFreq == RequestedFreq) {
    DEBUG ((DEBUG_INFO, "Requested BCLK clock frequency matches current frequency (ID = %d) - Exit\n", CurrentFreq));
    return EFI_SUCCESS;
  }

  if ((FreqSupportMask >> RequestedFreq) & BIT0) {
    DEBUG ((DEBUG_INFO, "Requested Frequency ID = %d - Supported!\n", RequestedFreq));
    Status = AceSetHdaCodecBclkFrequency (HdaController, RequestedFreq);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    // Delay needed after switching frequency (link reset), before sending verb to codec.
    MicroSecondDelay (HdaPreMemConfig->ResetWaitTimer);

    Status = AceGetHdaCodecCurrentBclkFrequency (HdaController, CodecSdiNum, &CurrentFreq);
    if ((Status == EFI_NOT_FOUND) || (CurrentFreq != RequestedFreq)) {
      DEBUG ((DEBUG_WARN, "Frequency switching failed - return to default!\n"));
      AceSetHdaCodecBclkFrequency (HdaController, HdaLinkFreq24MHz);
      return EFI_DEVICE_ERROR;
    }
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Detect active HDA codec links
    BIT0(001) -- SDI0
    BIT1(010) -- SDI1
    BIT2(100) -- SDI2

  @param[in] HdaController          Pointer to Hda device structure

  @retval UINT8                     Azalia SDI active codec mask
**/
UINT8
AceDetectHdaCodecs (
  IN  HDA_CONTROLLER        *HdaController
  )
{
  UINT32     LoopTime;
  UINT8      AzaliaSdiMask;
  UINT8      TmpPollingReg;

  AzaliaSdiMask = 0;

  for (LoopTime = 0; LoopTime < ACE_MAX_LOOP_TIME_MS; LoopTime++) {
    TmpPollingReg = (UINT8) (MmioRead8 (HdaController->HdaMmioAddress + R_ACE_MEM_WAKESTS) & ACE_MAX_SDI_MASK);
    if (TmpPollingReg != 0 && (TmpPollingReg == AzaliaSdiMask)) {
      break;
    } else {
      AzaliaSdiMask = TmpPollingReg;
    }
    MicroSecondDelay (ACE_WAIT_PERIOD_MS);
  }

  return AzaliaSdiMask;
}

/**
  For each codec, a predefined codec verb table should be programmed.
  The list contains 32-bit verbs to be sent to the corresponding codec.
  If it is not programmed, the codec uses the default verb table, which may or may not
  correspond to the platform jack information.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_DEVICE_ERROR        Device status error, operation failed
  @retval EFI_NOT_FOUND           HDA Link codec has not been discovered
**/
EFI_STATUS
AceDetectAndInitializeHdaCodecs (
  IN HDA_CONTROLLER         *HdaController,
  IN HDAUDIO_CONFIG         *HdaConfig,
  IN HDAUDIO_PREMEM_CONFIG  *HdaPreMemConfig
  )
{
  EFI_STATUS            Status;
  UINT8                 AzaliaSdiMask;
  UINT8                 AzaliaSdiNum;
  UINT32                CodecVendorId;
  UINT32                CodecRevisionId;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_ENDOFPEI_INIT_HDA_CODECS);

  if ((!HdaPreMemConfig->AudioLinkHda.SdiEnable[0] && !HdaPreMemConfig->AudioLinkHda.SdiEnable[1]) ||
       !HdaPreMemConfig->AudioLinkHda.Enable) {
    DEBUG ((DEBUG_INFO, "%a () - End. Function disabled through policy.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  ///
  /// Detect active HDA codecs
  ///
  AzaliaSdiMask = AceDetectHdaCodecs (HdaController);

  ///
  /// Detecting if any codec is connected
  ///
  if (AzaliaSdiMask == 0) {
    DEBUG ((DEBUG_INFO, "Azalia SDI mask == 0.\n"));
    DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  for (AzaliaSdiNum = 0; AzaliaSdiNum < ACE_MAX_SDI_NUMBER; AzaliaSdiNum++) {
    if ((AzaliaSdiMask >> AzaliaSdiNum) & 0x1) {
      if (AzaliaSdiNum == ACE_HDA_SDI_2_IDISPLINK) {
        DEBUG ((DEBUG_INFO, "Skip HDA codecs initialization for SDI#%d, it is not required for iDisp codec.\n", AzaliaSdiNum));
        continue;
      }

      ///
      /// SDIx has HD-Audio device
      ///
      DEBUG ((DEBUG_INFO, "SDI#%d has HD-Audio device.\n", AzaliaSdiNum));

      ///
      /// PME Enable for each existing codec, these bits are in the resume well
      ///
      if (HdaConfig->Pme != FALSE) {
        MmioOr16 ((HdaController->HdaMmioAddress + R_ACE_MEM_WAKEEN), (UINT16) (1 << AzaliaSdiNum));
      }

      Status = AceGetHdaCodecId (HdaController, AzaliaSdiNum, &CodecVendorId, &CodecRevisionId);
      if (EFI_ERROR(Status)) {
        continue;
      }

      DEBUG ((DEBUG_INFO, "SDI#%d: Detected HD-Audio Codec 0x%08X rev 0x%02X\n", AzaliaSdiNum, CodecVendorId, CodecRevisionId));
      ///
      /// Link static frequency switching
      ///
      AceConfigureHdaLinkFrequency (HdaController, HdaConfig, HdaPreMemConfig, AzaliaSdiNum);
      ///
      /// Send Verb Table if required table exist based on Codec Vendor ID and Codec Revision ID
      ///
      Status = AceMatchAndSendHdaCodecVerbTable (HdaController, HdaConfig, AzaliaSdiNum, CodecVendorId, CodecRevisionId);
      if (EFI_ERROR(Status)) {
        continue;
      }
    } else {
      ///
      /// SDIx has no HD-Audio device
      ///
      DEBUG ((DEBUG_INFO, "SDI#%d has no HD-Audio device.\n", AzaliaSdiNum));
    }
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Checks HD Audio Controller reset state.

  @param[in] HdaController      Pointer to Hda device structure

  @retval BOOLEAN               TRUE if controller is in reset, FALSE otherwise
**/
BOOLEAN
AceIsControllerReset (
  IN  HDA_CONTROLLER            *HdaController
  )
{
  ///
  /// Read RESET# bit - GCTL.CRSTB (offset 08h, BIT0)
  /// TRUE:  controller in reset state (CRSTB = 0b)
  /// FALSE: controller out of reset state (CRSTB = 1b)
  ///
  return (MmioRead32 (HdaController->HdaMmioAddress + R_ACE_MEM_GCTL) & B_ACE_MEM_GCTL_CRSTB) ? FALSE : TRUE;
}

/**
  Sets HD Audio Controller Reset state.
  In reset all state machines, memory mapped registers and links are driven to reset values.

  @param[in] HdaHandle              Pointer to Hda controller configuration structure
  @param[in] AssertResetSignal      TRUE to enter (assert) reset, FALSE to exit (de-assert) reset

  @retval EFI_SUCCESS               Controller changed reset state successfully
  @retval EFI_TIMEOUT               Polling RESET# bit timed out
**/
EFI_STATUS
AceSetControllerReset (
  IN  HDA_CONTROLLER          *HdaController,
  IN  HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig,
  IN  BOOLEAN                 AssertResetSignal
  )
{
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_ENDOFPEI_CTRL_RESET_ACE);

  if (AssertResetSignal) {
    ///
    /// Enter reset state: clear RESET# bit - GCTL.CRSTB (offset 08h, BIT0) = 0b
    ///
    MmioAnd32 ((HdaController->HdaMmioAddress + R_ACE_MEM_GCTL), (UINT32) ~(B_ACE_MEM_GCTL_CRSTB));
  } else {
    ///
    /// Exit reset state: set RESET# bit - GCTL.CRSTB (offset 08h, BIT0) = 1b
    ///
    MmioOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_GCTL), (UINT32) B_ACE_MEM_GCTL_CRSTB);
  }

  ///
  /// For some combo card that will need this delay because each codec has different latency to come out from RESET.
  /// This delay can make sure all codecs will be recognized by BIOS after RESET sequence.
  /// Additional delay might be required to allow codec coming out of reset prior to subsequent operations,
  /// please contact your codec vendor for detail. When clearing this bit and setting it afterward,
  /// BIOS must ensure that minimum link timing requirements (minimum RESET# assertion time, etc.) are met.
  ///
  MicroSecondDelay (HdaPreMemConfig->ResetWaitTimer);

  ///
  /// Poll RESET# bit until it reads back as 1 if RESET# de-asserted or 0 if asserted
  ///
  Status = StatusPolling (HdaController->HdaMmioAddress + R_ACE_MEM_GCTL,  B_ACE_MEM_GCTL_CRSTB, (BOOLEAN) !AssertResetSignal);

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Configures HDA compliance mode (UAA, non-UAA mode) by setting PI, SCC, BCC values.

  @param[in] HdaController      Pointer to Hda device structure
  @param[in] HdaPreMemConfig    Pointer to Hda PreMem Configuration

  @retval VOID
**/
VOID
AceConfigureComplianceMode (
  IN  HDA_CONTROLLER                *HdaController,
  IN  HDAUDIO_PREMEM_CONFIG         *HdaPreMemConfig
  )
{
  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  ///
  /// 0: Select soft strap value. 1: Select BIOS initialized register value. (PI/SCC/BCC)
  ///
  MmioOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HFPCICFGHWIS), B_ACE_MEM_HFPCICFGHWIS_PI | B_ACE_MEM_HFPCICFGHWIS_SCC | B_ACE_MEM_HFPCICFGHWIS_BCC);

  if ((HdaPreMemConfig->DspEnable == TRUE) && !AceIsDspFusedOut (HdaController)) {
    if (HdaPreMemConfig->DspUaaCompliance == FALSE) {
      DEBUG ((DEBUG_INFO, "Select Non-UAA mode.\n"));
      //
      // Set Sub Class Code to 01 for Audio DSP enabled (SST only support)
      //
      MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HFPCICFGHWI0),
      (UINT32) ~(B_ACE_MEM_HFPCICFGHWI0_PI | B_ACE_MEM_HFPCICFGHWI0_SCC | B_ACE_MEM_HFPCICFGHWI0_BCC),
      (UINT32) ((V_ACE_MEM_HFPCICFGHWI0_PI_NON_UAA << N_ACE_MEM_HFPCICFGHWI0_PI) | (V_ACE_MEM_HFPCICFGHWI0_SCC_NON_UAA << N_ACE_MEM_HFPCICFGHWI0_SCC) | (V_ACE_MEM_HFPCICFGHWI0_BCC_NON_UAA << N_ACE_MEM_HFPCICFGHWI0_BCC)));
    } else {
      DEBUG ((DEBUG_INFO, "ACE: Select UAA mode.\n"));
      //
      // Set Programming Interface to 01 for Audio DSP enabled (HDA Inbox and SST support)
      //
      MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HFPCICFGHWI0),
      (UINT32) ~(B_ACE_MEM_HFPCICFGHWI0_PI | B_ACE_MEM_HFPCICFGHWI0_SCC | B_ACE_MEM_HFPCICFGHWI0_BCC),
      (UINT32) ((V_ACE_MEM_HFPCICFGHWI0_PI_UAA << N_ACE_MEM_HFPCICFGHWI0_PI) | (V_ACE_MEM_HFPCICFGHWI0_SCC_UAA << N_ACE_MEM_HFPCICFGHWI0_SCC) | (V_ACE_MEM_HFPCICFGHWI0_BCC_UAA << N_ACE_MEM_HFPCICFGHWI0_BCC)));
    }
  } else {
    DEBUG ((DEBUG_INFO, "Select Non-UAA mode. DSP Disabled.\n"));
    //
    // Set Programming Interface to 01 for Audio DSP disabled (HDA Inbox)
    //
    MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HFPCICFGHWI0),
     (UINT32) ~(B_ACE_MEM_HFPCICFGHWI0_PI | B_ACE_MEM_HFPCICFGHWI0_SCC | B_ACE_MEM_HFPCICFGHWI0_BCC),
     (UINT32) ((V_ACE_MEM_HFPCICFGHWI0_PI_DSP_DIS << N_ACE_MEM_HFPCICFGHWI0_PI) | (V_ACE_MEM_HFPCICFGHWI0_SCC_DSP_DIS << N_ACE_MEM_HFPCICFGHWI0_SCC) | (V_ACE_MEM_HFPCICFGHWI0_BCC_DSP_DIS << N_ACE_MEM_HFPCICFGHWI0_BCC)));
  }

  DEBUG ((DEBUG_INFO, "Audio Class Code: %x.%x.%x.\n",
          PciSegmentRead8 ((UINTN) HdaController->PciCfgBaseAddr + R_ACE_CFG_BCC),
          PciSegmentRead8 ((UINTN) HdaController->PciCfgBaseAddr + R_ACE_CFG_SCC),
          PciSegmentRead8 ((UINTN) HdaController->PciCfgBaseAddr + R_ACE_CFG_PI)));

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
  return;
}

/**
  Configures SRAM retention mode depending on the clock frequency of the XTAL oscillator clock input.

  @param[in] HdaController      Pointer to Hda device structure

  @retval VOID
**/
VOID
AceConfigureSramRetentionMode (
  IN HDA_CONTROLLER          *HdaController,
  IN OUT UINT32              *XtalValue
  )
{
  UINT32 XtalFrequency;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  XtalFrequency = (MmioRead32 (HdaController->HdaMmioAddress + R_ACE_MEM_HSTRVAL) & B_ACE_MEM_HSTRVAL_XOCFS);
  /// The encoding below specify the clock frequency of the XTAL oscillator clock input.
  ///   00b: 24 MHz
  ///   01b: 38.4 MHz
  ///   10b: 24.576 MHz
  ///   11b: 19.2 MHz
  switch (XtalFrequency) {
    case V_ACE_MEM_HSTRVAL_XOCFS_XTAL_24MHZ:
      *XtalValue = 24000000;
      MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_PTDC),
        (UINT32) ~B_ACE_MEM_PTDC_SRMIW,
        (UINT32) (0x4 << N_ACE_MEM_PTDC_SRMIW));
      DEBUG ((DEBUG_INFO, "SRAM retention mode idle wait - 64 XTAL oscillator clocks\n"));
      break;
    case V_ACE_MEM_HSTRVAL_XOCFS_XTAL_38_4MHZ:
      *XtalValue = 38400000;
      MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_PTDC),
        (UINT32) ~B_ACE_MEM_PTDC_SRMIW,
        (UINT32) (0x4 << N_ACE_MEM_PTDC_SRMIW));
      DEBUG ((DEBUG_INFO, "SRAM retention mode idle wait - 64 XTAL oscillator clocks\n"));
      break;
    case V_ACE_MEM_HSTRVAL_XOCFS_XTAL_19_2MHZ:
      *XtalValue = 19200000;
      MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_PTDC),
        (UINT32) ~B_ACE_MEM_PTDC_SRMIW,
        (UINT32) (0x3 << N_ACE_MEM_PTDC_SRMIW));
      DEBUG ((DEBUG_INFO, "SRAM retention mode idle wait - 32 XTAL oscillator clocks\n"));
      break;
    default:
      *XtalValue = 38400000;
      DEBUG ((DEBUG_WARN, "Invalid XTAL Oscillator Clock Frequency!\n"));
      break;
  }
  DEBUG ((DEBUG_INFO, "Detected XTAL: %d Hz.\n", *XtalValue));

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
  return;
}

/**
  Program HDA and DSP bars for temporary MMIO access.

  @param[in, out] HdaController        Pointer to Hda device structure

  @retval VOID
**/
VOID
AceEnableHdaDspMmioAccess (
  IN OUT  HDA_CONTROLLER        *HdaController
  )
{
  if (HdaController == NULL) {
    return;
  }

  if (PciSegmentRead16 (HdaController->PciCfgBaseAddr + R_ACE_CFG_VID) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "HDA not present!\n"));
    return;
  }
  // Clear MSE
  PciSegmentAnd16 (HdaController->PciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) (~EFI_PCI_COMMAND_MEMORY_SPACE));

  // Set HDA and DSP BARs
  PciSegmentWrite32 (HdaController->PciCfgBaseAddr + R_ACE_CFG_HDALBA, (UINT32) HdaController->HdaMmioAddress);
  PciSegmentWrite32 (HdaController->PciCfgBaseAddr + R_ACE_CFG_HDAUBA, (UINT32) RShiftU64 (HdaController->HdaMmioAddress, 32));
  PciSegmentWrite32 (HdaController->PciCfgBaseAddr + R_ACE_CFG_DSPLBA, (UINT32) HdaController->DspMmioAddress);
  PciSegmentWrite32 (HdaController->PciCfgBaseAddr + R_ACE_CFG_DSPUBA, (UINT32) RShiftU64 (HdaController->DspMmioAddress, 32));

  // Set MSE
  PciSegmentOr16 (HdaController->PciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE);

  DEBUG ((DEBUG_INFO, "HdaPciBase = 0x%llx\n", HdaController->PciCfgBaseAddr));
  DEBUG ((DEBUG_INFO, "HdaMmioAddress = 0x%llx\n", (UINT64) HdaController->HdaMmioAddress));
  DEBUG ((DEBUG_INFO, "DspMmioAddress = 0x%llx\n", (UINT64) HdaController->DspMmioAddress));

  DEBUG((DEBUG_INFO, "VID: %x \n", PciSegmentRead16 (HdaController->PciCfgBaseAddr + R_ACE_CFG_VID)));
  DEBUG((DEBUG_INFO, "DID: %x \n", PciSegmentRead16 (HdaController->PciCfgBaseAddr + R_ACE_CFG_DID)));

  return;
}

/**
  Clear HDA and DSP bars to disable MMIO access.

  @param[in] HdaController           Pointer to Hda device structure

  @retval VOID
**/
VOID
AceDisableHdaDspMmioAccess (
  IN  HDA_CONTROLLER        *HdaController
  )
{
  if (HdaController == NULL) {
    return;
  }

  if (PciSegmentRead16 (HdaController->PciCfgBaseAddr + R_ACE_CFG_VID) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "HDA not present!\n"));
    return;
  }

  // Clear MSE
  PciSegmentAnd16 (HdaController->PciCfgBaseAddr + PCI_COMMAND_OFFSET, (UINT16) (~EFI_PCI_COMMAND_MEMORY_SPACE));

  // Clear HDA and DSP BARs
  PciSegmentWrite32 (HdaController->PciCfgBaseAddr + R_ACE_CFG_HDALBA, 0);
  PciSegmentWrite32 (HdaController->PciCfgBaseAddr + R_ACE_CFG_HDAUBA, 0);
  PciSegmentWrite32 (HdaController->PciCfgBaseAddr + R_ACE_CFG_DSPLBA, 0);
  PciSegmentWrite32 (HdaController->PciCfgBaseAddr + R_ACE_CFG_DSPUBA, 0);

  return;
}

/**
  Sets iDisplay Audio Codec T-Mode

  @param[in] HdaController        Pointer to Hda device structure
  @param[in] IDispTmode           Display Codec T-Mode to be set
**/
VOID
AceSetIDisplayAudioTmode (
  IN      HDA_CONTROLLER          *HdaController,
  IN      HDAUDIO_IDISP_TMODE     RequestedIDispTmode
  )
{
  UINT32                          Data32;

  Data32 = 0;

  if (HdaController == NULL) {
    DEBUG ((DEBUG_INFO, "%a () - End. Invalid parameters.\n", __FUNCTION__));
    return ;
  }

  DEBUG ((DEBUG_INFO, "Set iDisplay Audio T-Mode - "));
  switch (RequestedIDispTmode) {
    case HdaIDispMode1T:
      DEBUG ((DEBUG_INFO, "1T\n"));
      // 1T Mode ETMODE[28:27] = 0 TMODE[12] = 1
      Data32 = (V_ACE_MEM_EM1_ETMODE_1T_2T << N_ACE_MEM_EM1_ETMODE) | B_ACE_MEM_EM1_TMODE;
      break;
    case HdaIDispMode2T:
      DEBUG ((DEBUG_INFO, "2T\n"));
      // 2T Mode ETMODE[28:27] = 0 TMODE[12] = 0
      Data32 = (V_ACE_MEM_EM1_ETMODE_1T_2T << N_ACE_MEM_EM1_ETMODE);
      break;
    case HdaIDispMode4T:
      DEBUG ((DEBUG_INFO, "4T\n"));
      // 4T Mode ETMODE[28:27] = 1 TMODE[12] = 0
      Data32 = (V_ACE_MEM_EM1_ETMODE_4T << N_ACE_MEM_EM1_ETMODE);
      break;
    case HdaIDispMode8T:
      DEBUG ((DEBUG_INFO, "8T\n"));
      // 8T Mode ETMODE[28:27] = 2 TMODE[12] = 0
      Data32 = (V_ACE_MEM_EM1_ETMODE_8T << N_ACE_MEM_EM1_ETMODE);
      break;
    case HdaIDispMode16T:
      DEBUG ((DEBUG_INFO, "16T\n"));
      // 16T Mode ETMODE[28:27] = 3 TMODE[12] = 0
      Data32 = (V_ACE_MEM_EM1_ETMODE_16T << N_ACE_MEM_EM1_ETMODE);
      break;
    default:
      DEBUG ((DEBUG_WARN, "Unsupported iDisplay Audio T-Mode - Exit!\n"));
      Data32 = 0;
  }

  MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_EM1),
    (UINT32) ~(B_ACE_MEM_EM1_ETMODE | B_ACE_MEM_EM1_TMODE),
    (UINT32) Data32);
}

/**
  This function enables IDisplay Codec.

  @param[in] HdaController        Pointer to Hda device structure
  @param[in] HdaCallbacks         Pointer to Hda callbacks structure
  @param[in] HdaPreMemConfig      Hda PreMem Configuration
**/
VOID
AceConfigureDispAudioCodec (
  IN  HDA_CONTROLLER         *HdaController,
  IN  HDA_CALLBACK           *HdaCallbacks,
  IN  HDAUDIO_PREMEM_CONFIG  *HdaPreMemConfig,
  IN  HDA_PRIVATE_CONFIG     *HdaPrivateConfig
  )
{
  EFI_STATUS                 Status;
  UINT32                     Data32;

  Data32 = 0;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if ((HdaController == NULL) || (HdaCallbacks == NULL) || (HdaPreMemConfig == NULL) || (HdaPrivateConfig == NULL)) {
    DEBUG ((DEBUG_INFO, "%a () - End. Invalid parameters.\n", __FUNCTION__));
    return ;
  }

  if ((HdaPreMemConfig->IDispCodecDisconnect == TRUE) || (HdaPrivateConfig->IGfxEnabled == FALSE)) {
    //
    // Disconnect SDI2 signal to prevent iDisplay Audio codec enumeration
    //
    MmioOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_CDCCFG, BIT2);
    DEBUG ((DEBUG_INFO, "IDisplay Audio Codec disabled - SDI2 disconnected\n"));

    DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
    return ;
  }

  if (HdaCallbacks->HdaIsAudioIDispTmodeSupported != NULL) {
    if (!HdaCallbacks->HdaIsAudioIDispTmodeSupported (HdaPreMemConfig->IDispLinkTmode)) {
      DEBUG ((DEBUG_INFO, "IDisp TMode not supported.\n"));
      DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
      return ;
    }
  } else {
    DEBUG ((DEBUG_WARN, "HdaIsAudioIDispTmodeSupported () NOT defined!\n"));
  }

  //
  // Enable Display Power wells and Audio pin buffer control for Display Audio Codec.
  //
  if (HdaCallbacks->HdaInitializeDisplayAudioCodec != NULL) {
    Status = HdaCallbacks->HdaInitializeDisplayAudioCodec ();
    if (EFI_ERROR (Status)) {
      return ;
    }
  } else {
    DEBUG ((DEBUG_WARN, "HdaInitializeDisplayAudioCodec () NOT defined!\n"));
  }

  //
  // Software sets this bit to '1' to turn the link on (provided CRSTB = 1), and clears it to '0' when it wishes to turn the link off.
  // When CPA matches the value of this bit, the achieved power state has been reached.  Software is expected to wait for CPA to match SPA before it can program SPA again.
  // Any deviation may result in undefined behavior.
  //
  MmioAnd32 ((HdaController->HdaMmioAddress + R_ACE_MEM_IDALCTL), ~((UINT32) B_ACE_MEM_IDALCTL_SPA));

  //
  // This value changes to the value set by SPA when the power of the link has reached that state. Software sets SPA, then monitors CPA to know when the link has changed state.
  //
  Status = StatusPolling (HdaController->HdaMmioAddress + R_ACE_MEM_IDALCTL, (UINT32) B_ACE_MEM_IDALCTL_CPA, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Turn off the link (SPA = 0) failed! CPA polling Status = %r\n", Status));
  }

  //
  // Configure Tmode in Hda controller
  //
  AceSetIDisplayAudioTmode (HdaController, HdaPreMemConfig->IDispLinkTmode);

  //
  // Indicates the frequency that software wishes the link to run at.
  // Changing this value to a value not supported by Link Capabilities shall result in indeterminate results.  The possible encodings are:
  // 000: 6 MHz, 001: 12 MHz, 010: 24 MHz, 011: 48 MHz, 100: 96 MHz, 101: 192 MHz, 110 - 111: Reserved
  // The value of SCF must persist across a controller reset.
  //
  MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_IDALCTL),
    (UINT32) ~(B_ACE_MEM_IDALCTL_SCF),
    (UINT32) (HdaPreMemConfig->IDispLinkFrequency << N_ACE_MEM_IDALCTL_SCF));

  Data32 = MmioRead32 (HdaController->HdaMmioAddress + R_ACE_MEM_IDALCTL);
  DEBUG ((DEBUG_INFO, "IDACTL (Link Control) = 0x%08x\n", Data32));

  //
  // Software sets this bit to '1' to turn the link on (provided CRSTB = 1), and clears it to '0' when it wishes to turn the link off.
  // When CPA matches the value of this bit, the achieved power state has been reached.  Software is expected to wait for CPA to match SPA before it can program SPA again.
  // Any deviation may result in undefined behavior.
  //
  MmioOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_IDALCTL), (UINT32) B_ACE_MEM_IDALCTL_SPA);

  //
  // This value changes to the value set by SPA when the power of the link has reached that state. Software sets SPA, then monitors CPA to know when the link has changed state.
  //
  Status = StatusPolling (HdaController->HdaMmioAddress + R_ACE_MEM_IDALCTL, (UINT32) B_ACE_MEM_IDALCTL_CPA, (UINT32) B_ACE_MEM_IDALCTL_CPA);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "Turn on the link (SPA = 1) failed! CPA polling Status = %r\n", Status));
  }
  return ;
}

/**
  Configures Audio Link Mode

  @param[in] HdaCallbacks         Pointer to Hda callbacks structure
  @param[in] HdaPreMemConfig      Pointer to Hda PreMem Configuration
  @param[in] HdaPrivateConfig     Pointer to Hda Private Configuration
  @param[in] HdaConfig            Pointer to Hda Configuration
**/
VOID
AceConfigureAudioLinks (
  IN HDA_CALLBACK            *HdaCallbacks,
  IN HDAUDIO_PREMEM_CONFIG   *HdaPreMemConfig,
  IN HDA_PRIVATE_CONFIG      *HdaPrivateConfig,
  IN HDAUDIO_CONFIG          *HdaConfig
  )
{
  UINT32                     Index;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, PC_ENDOFPEI_INIT_LINKS_ACE);

  ///
  /// Enable GPIOs for Dmic0 and Dmic1
  ///
  for (Index = 0; Index < HdaPrivateConfig->HdaMaxDmicLinkNum; Index++) {
    if (HdaCallbacks->HdaIsAudioInterfaceSupported != NULL) {
      if (HdaPreMemConfig->AudioLinkDmic[Index].Enable && HdaCallbacks->HdaIsAudioInterfaceSupported (HdaDmic, Index)) {
        if (HdaCallbacks->HdaConfigureDmicLinks != NULL) {
          HdaCallbacks->HdaConfigureDmicLinks (HdaPreMemConfig, Index);
        } else {
          DEBUG ((DEBUG_WARN, "HdaConfigureDmicLinks () not defined!\n"));
        }
      }
    }
  }

  ///
  /// Enable GPIOs for Ssps
  ///
  for (Index = 0; Index < HdaPrivateConfig->HdaMaxSspLinkNum; Index++) {
    if (HdaCallbacks->HdaIsAudioInterfaceSupported != NULL) {
      if (HdaPreMemConfig->AudioLinkSsp[Index].Enable && HdaCallbacks->HdaIsAudioInterfaceSupported (HdaSsp, Index)) {
        if (HdaCallbacks->HdaConfigureSspLinks != NULL) {
          HdaCallbacks->HdaConfigureSspLinks (HdaPreMemConfig, Index);
        } else {
          DEBUG ((DEBUG_WARN, "HdaConfigureSspLinks () not defined!\n"));
        }
      }
    }
  }

  ///
  /// Enable GPIO for Ssp Master Clock
  ///
  if (HdaCallbacks->HdaConfigureSspMasterClock != NULL) {
    HdaCallbacks->HdaConfigureSspMasterClock (HdaPreMemConfig, 0);
  } else {
    DEBUG ((DEBUG_WARN, "HdaConfigureSspMasterClock () not defined!\n"));
  }

  ///
  /// Enable GPIO for Sndws
  ///
  for (Index = 0; Index < HdaPrivateConfig->HdaMaxSndwLinkNum; Index++) {
    if (HdaCallbacks->HdaIsAudioInterfaceSupported != NULL) {
      if (HdaPreMemConfig->AudioLinkSndw[Index].Enable && HdaCallbacks->HdaIsAudioInterfaceSupported (HdaSndw, Index)) {
        if (HdaCallbacks->HdaConfigureSndwLinks != NULL) {
          HdaCallbacks->HdaConfigureSndwLinks (Index);
        } else {
          DEBUG ((DEBUG_WARN, "HdaConfigureSndwLinks () not defined!\n"));
        }
      }
    }
  }

  ///
  /// Enable GPIO for Sndw Multilane
  ///
  for (Index = 0; Index < HdaPrivateConfig->HdaMaxSndwMultilaneNum; Index++) {
    if ((HdaCallbacks->HdaIsAudioInterfaceSupported != NULL) && (HdaCallbacks->HdaIsAudioInterfaceSupported (HdaSndwMultilane, Index))) {
      if (HdaPreMemConfig->SndwMultilane[Index].Enable != HdaSndwMultilaneDisabled) {
        if (HdaCallbacks->HdaConfigureSndwMultilane != NULL) {
          HdaCallbacks->HdaConfigureSndwMultilane (HdaPreMemConfig, (UINT8) Index);
        } else {
          DEBUG ((DEBUG_WARN, "HdaConfigureSndwMultilane () not defined!\n"));
        }
      }
    }
  }

  if (HdaConfig->MicPrivacy.Mode != HdaNoMicrophonePrivacySupport) {
    if (HdaCallbacks->HdaConfigureMicrophonePrivacy != NULL) {
      HdaCallbacks->HdaConfigureMicrophonePrivacy ();
    } else {
      DEBUG ((DEBUG_WARN, "HdaConfigureMicrophonePrivacy () not defined!\n"));
    }
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
}

/**
  Configures ACE controller.
  Temporarily enables HDA and DSP MMIO accesses, configures compliance mode and SRAM retention mode.
  Initializes HDA codecs and SNDW IO access.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure

  @retval EFI_SUCCESS             Controller configured successfully
**/
EFI_STATUS
AceConfigureController (
  IN HDA_CONTROLLER        *HdaController,
  IN HDA_CALLBACK          *HdaCallbacks,
  IN HDAUDIO_CONFIG        *HdaConfig,
  IN HDAUDIO_PREMEM_CONFIG *HdaPreMemConfig,
  IN HDA_PRIVATE_CONFIG    *HdaPrivateConfig,
  IN OUT UINT32            *XtalValue,
  IN UINT8                 InterruptPin
  )
{
  EFI_STATUS              Status;
  UINT32                  Data32AndFncfg;
  UINT32                  Data32OrFncfg;

  Data32AndFncfg = (UINT32) ~0;
  Data32OrFncfg  = (UINT32) 0;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  //
  // Bring HDA out of reset so it can do MMIO register programming.
  //
  if (AceIsControllerReset (HdaController)) {
    Status = AceSetControllerReset (HdaController, HdaPreMemConfig, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Un-reset High Definition Audio (CRST = 1) Time Out!\n"));
      return EFI_TIMEOUT;
    }
  }

  if ((HdaPreMemConfig->DspEnable == TRUE) && !AceIsDspFusedOut (HdaController)) {
    //
    // Enable Audio DSP for operation:
    // GPROCEN = 0, ADSPxBA (BAR2) is mapped to the Intel HD Audio memory mapped configuration registers (Default),
    // GPROCEN = 1, ADSPxBA (BAR2) is mapped to the actual Audio DSP memory mapped configuration registers.
    //
    DEBUG ((DEBUG_INFO, "Set PPCTL.GPROCEN = 1 (Global Processing Enable - to enable ADSP for operation)\n"));
    MmioOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_PPCTL), (UINT32) B_ACE_MEM_PPCTL_GPROCEN);
  }

  if (HdaPreMemConfig->SubSystemIds != 0) {
    ///
    /// Enable SID and SVID programming by BIOS. Write to HfPCICFGHWIS.
    /// This register selects the PCI function (host root space) HW initialization value to be sourced from soft strap (HfPCICFGSSVx) or BIOS initialized registers (HfPCICFGHWIx)
    /// 0: Select soft strap value.
    /// 1: Select BIOS initialized register value.
    ///
    MmioOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_HFPCICFGHWIS), B_ACE_MEM_HFPCICFGHWIS_SID | B_ACE_MEM_HFPCICFGHWIS_SVID);
    ///
    /// The SID/SVID registers, enables the operating environment to distinguish one audio subsystem from another.
    /// Software (BIOS) will write the value to this register.
    ///
    MmioWrite32 (HdaController->HdaMmioAddress + R_ACE_MEM_HFPCICFGHWI1, (HdaPreMemConfig->SubSystemIds));
  }

  AceConfigureComplianceMode (HdaController, HdaPreMemConfig);

  /// EM1 register programming.
  ///   Clear EM1 [0:29]
  ///   Set EM1.FIFORDYSEL (FIFO Ready Select) [9:10] = 11b: FIFO Ready will be asserted when FIFO available data is above the L1 exit threshold
  ///   Set EM1.LFLCS (Low Frequencies Link Clock Source) [24] = 1b: Use XTAL oscillator clock for low frequencies link clock.
  ///   Set EM1.ECCWRBKD (SRAM ECC Write Back Disable) [29] = 1b: It disables the SRAM ECC single bit error correction write back.
  MmioAnd32 ((HdaController->HdaMmioAddress + R_ACE_MEM_EM1), (UINT32) ~B_ACE_MEM_EM1_BBRK);

  ///
  /// Configure NodeID field for iDISPLAY Audio Link's SET_CLKOFF command
  ///
  MmioAndThenOr32 ((HdaController->HdaMmioAddress + R_ACE_MEM_EM1),
    (UINT32) ~(B_ACE_MEM_EM1_NODEID),
    (UINT32) HdaPrivateConfig->NodeId
    );

  AceConfigureSramRetentionMode (HdaController, XtalValue);

  /// LTRP register programming.
  /// BIOS to program the L1 exit threshold to 1/4 of FIFO size + apply minimum buffer size for small payload stream,
  /// so that the partial row data of data is less than the L1 exit threshold and able to exit L1 to top up.
  MmioAndThenOr8 ((HdaController->HdaMmioAddress + R_ACE_MEM_LTRP),
    (UINT8) ~(B_ACE_MEM_LTRP_GB),
    (UINT8) V_ACE_MEM_LTRP_GB
    );

  ///
  /// CDCCFG (Codec Configuration) register programming
  ///   This register configures the codec exposure in the Intel HD Audio controller.
  /// SDID (SDIN Disconnect)
  ///   Bits which control which SDI signal(s) can be disconnected.
  ///   A 1 bit in the bit location indicates that the associated SDIN signal is
  ///   disconnected and not able to generate any wake or response.
  ///   These bits are preserved in Sx state.
  ///
  if (HdaCallbacks->HdaIsAudioInterfaceSupported != NULL) {
    if ((HdaPreMemConfig->AudioLinkHda.Enable == FALSE) || !HdaCallbacks->HdaIsAudioInterfaceSupported (HdaLink, 0)) {
      MmioOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_CDCCFG, (BIT0 | BIT1));
      DEBUG ((DEBUG_INFO, "SDI0 and SDI1 in Hda controller disabled\n"));
    } else {
      if (HdaPreMemConfig->AudioLinkHda.SdiEnable[0] == FALSE) {
        MmioOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_CDCCFG, BIT0);
        DEBUG ((DEBUG_INFO, "SDI0 in Hda controller disabled\n"));
      }

      if (HdaPreMemConfig->AudioLinkHda.SdiEnable[1] == FALSE) {
        MmioOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_CDCCFG, BIT1);
        DEBUG ((DEBUG_INFO, "SDI1 in Hda controller disabled\n"));
      }
    }
  }

  ///
  /// Set Buffer Size Minimum Threshold to 128B.
  /// Set EM2/SEM2.BSMT [BIT27:26] to 01b
  ///
  MmioAndThenOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_EM2,
    (UINT32) ~(B_ACE_MEM_EM2_BSMT),
    (UINT32) (V_ACE_MEM_EM2_BSMT << N_ACE_MEM_EM2_BSMT)
    );

  ///
  /// Set Output Stream L1 Exit Threshold 2 to 1/4 of FIFO size
  /// Set EM4L/SEM4L.OSL1EXT2 to 11b
  ///
  MmioAndThenOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_EM4L,
    (UINT32) ~B_ACE_MEM_EM4L_OSL1EXT2,
    (UINT32) (V_ACE_MEM_EM4L_OSL1EXT2 << N_ACE_MEM_EM4L_OSL1EXT2));

  ///
  /// Program the DPIB status write policy to always send update at the end of the HD-A 48 KHz frame period.
  /// Set EM2/SEM2.DUM [BIT23] to 1b
  ///
  MmioOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_EM2, (UINT32) B_ACE_MEM_EM2_DUM);

  ///
  /// Set Input Stream L1 Exit Threshold 2 to 1/4 of FIFO size
  /// Set EM3L/SEM3L.ISL1EXT2 to 11b
  ///
  MmioAndThenOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_EM3L,
    (UINT32) ~B_ACE_MEM_EM3L_ISL1EXT2,
    (UINT32) (V_ACE_MEM_EM3L_ISL1EXT2 << N_ACE_MEM_EM3L_ISL1EXT2));

  ///
  /// When set, this bit overrides the DEVC.NSNPEN bit functionality and behave as though DEVC.NSNPEN = 1.
  /// Force non snoop to be enabled as default is PCI mode where DEVC.NSNPEN = 0.
  /// Setr EM2.FNSNPEN to 1b
  ///
  MmioOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_EM2, B_ACE_MEM_EM2_FNSNPEN);

  if (HdaPrivateConfig->TrafficClassAssignmentUsb < 0xFF) {
    ///
    /// When PSF Deferred Write Buffer is supported, PSF implements TC (traffic class)-based channel mapping for txns from XHCI.
    /// Parameter TCUSBCHM will decide which IOSF Primary interface channel VCusb is being mapped to.
    ///
    MmioAndThenOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_TCA,
      (UINT32) ~(B_ACE_MEM_TCA_TC0 << (TC_USB_CHANNEL_NUMBER * 4)),
      (UINT32) (HdaPrivateConfig->TrafficClassAssignmentUsb << (TC_USB_CHANNEL_NUMBER * 4)));
  }

  ///
  /// GPIOs configuration for Audio Links
  ///
  AceConfigureAudioLinks (HdaCallbacks, HdaPreMemConfig, HdaPrivateConfig, HdaConfig);

  AceConfigureDispAudioCodec (HdaController, HdaCallbacks, HdaPreMemConfig, HdaPrivateConfig);

  if (HdaPreMemConfig->DspEnable == FALSE) {
      if (HdaCallbacks->HdaPsfDisableDspBar != NULL) {
      HdaCallbacks->HdaPsfDisableDspBar ();
    } else {
      DEBUG ((DEBUG_WARN, "HdaPsfDisableDspBar () not defined!\n"));
    }
    /// Set DSPSD (DSP Subsystem Disable)
    /// When set, the DSP subsystem is disabled and all register accesses associated
    /// with DSP subsystem are treated as unsupported requests, and returns the UR
    /// response if it is a non-posted cycle.
    Data32OrFncfg |= (UINT32) B_ACE_MEM_FNCFG_DSPSD;
  }

  ///
  /// FNCFG (Function Configuration) register programming
  ///
  if (HdaPreMemConfig->PowerClockGating != FORCE_DISABLE) {
    // Clear CGD (Clock Gating Disabled)
    //  When cleared, it allows local/dynamic clock gating and trunk clock gating
    //  to take place per the associated enable and idle conditions.
    // Clear PGD (Power Gating Disabled)
    //  When cleared, it allows power gating to take place per their associated
    //  enable and idle conditions.
    Data32AndFncfg &= ~(B_ACE_MEM_FNCFG_CGD | B_ACE_MEM_FNCFG_PGD);
  }

    // Set BCLD (BIOS Configuration Lock Down)
    //  When set, it indicates BIOS configuration is done and ready for operations.
    Data32OrFncfg |= B_ACE_MEM_FNCFG_BCLD;

  MmioAndThenOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_FNCFG, Data32AndFncfg, Data32OrFncfg);

  if ((HdaPreMemConfig->DspEnable == FALSE) || AceIsDspFusedOut(HdaController)) {
      if (HdaCallbacks->HdaPsfDisableDspBar != NULL) {
      HdaCallbacks->HdaPsfDisableDspBar ();
    } else {
      DEBUG ((DEBUG_WARN, "HdaPsfDisableDspBar () not defined!\n"));
    }
  }

  ///
  /// Configure HD Audio interrupt
  ///
  PciSegmentWrite8 (HdaController->PciCfgBaseAddr + PCI_INT_PIN_OFFSET, InterruptPin);

  ///
  /// Configure Microphone Privacy
  ///
  if (HdaPrivateConfig->MicPrivacySupported == TRUE) {
    AceConfigureMicPrivacy (HdaController, HdaConfig);
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Disables ACE controller.

  @param[in] HdaController      Pointer to Hda device structure
  @param[in] HdaCallbacks       Pointer to Hda callbacks structure
  @param[in] HdaPreMemConfig    Pointer to Hda PreMem Configuration

  @retval EFI_SUCCESS             Controller disabled successfully
**/
EFI_STATUS
AceDisableController (
  IN HDA_CONTROLLER         *HdaController,
  IN HDA_CALLBACK           *HdaCallbacks,
  IN HDAUDIO_PREMEM_CONFIG  *HdaPreMemConfig
  )
{
  UINT32  Data32AndFncfg;
  UINT32  Data32OrFncfg;

  Data32AndFncfg = (UINT32) ~0;
  Data32OrFncfg  = (UINT32) 0;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  ///
  /// 1. Set FNCFG (Function Configuration) register
  ///     ACED  (BIT0) - 1b
  ///     DSPSD (BIT2) - 1b
  ///     CGD   (BIT3) - 0b
  ///     BCLD  (BIT4) - 1b
  ///     PGD   (BIT5) - 0b
  ///

  /// Set ACED (ACE IP Disable)
  /// When set, the ACE IP (including DSP subsystem) is disabled and all register
  /// accesses are treated as unsupported requests, and returns the UR response
  /// if it is a non-posted cycle.
  Data32OrFncfg |= (UINT32) B_ACE_MEM_FNCFG_ACED;

  /// Set DSPSD (DSP Subsystem Disable)
  /// When set, the DSP subsystem is disabled and all register accesses associated
  /// with DSP subsystem are treated as unsupported requests, and returns the UR
  /// response if it is a non-posted cycle.
  Data32OrFncfg |= (UINT32) B_ACE_MEM_FNCFG_DSPSD;

  ///
  /// FNCFG (Function Configuration) register programming
  ///
  if (HdaPreMemConfig->PowerClockGating != FORCE_DISABLE) {
    // Clear CGD (Clock Gating Disabled)
    // When cleared, it allows local/dynamic clock gating and trunk clock gating
    // to take place per the associated enable and idle conditions.
    // Clear PGD (Power Gating Disabled)
    // When cleared, it allows power gating to take place per their associated
    // enable and idle conditions.
    Data32AndFncfg &= ~(B_ACE_MEM_FNCFG_CGD | B_ACE_MEM_FNCFG_PGD);
  }

    // Set BCLD (BIOS Configuration Lock Down)
    // When set, it indicates BIOS configuration is done and ready for operations.
    Data32OrFncfg |= B_ACE_MEM_FNCFG_BCLD;

  MmioAndThenOr32 (HdaController->HdaMmioAddress + R_ACE_MEM_FNCFG, Data32AndFncfg, Data32OrFncfg);

  ///
  /// 2. Disable HDA in PSF
  ///
  if (HdaCallbacks->HdaPsfDisableDevice != NULL) {
    HdaCallbacks->HdaPsfDisableDevice ();
  } else {
    DEBUG ((DEBUG_WARN, "HdaPsfDisableDevice () not defined!\n"));
  }

  ///
  /// 3. Arm audio IP function disable on the PMC side
  ///
  if (HdaCallbacks->HdaPmcDisableDevice != NULL) {
    HdaCallbacks->HdaPmcDisableDevice ();
  } else {
    DEBUG ((DEBUG_WARN, "HdaPmcDisableDevice () not defined!\n"));
  }

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Triggers ACE initialization or disables ACE depending on HdaPreMem configuration.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure

  @retval EFI_SUCCESS             Controller disabled successfully
**/
EFI_STATUS
AceConfigureHdAudio (
  IN HDA_HANDLE                *HdaHandle,
  IN OUT UINT32                *XtalValue,
  IN UINT8                     InterruptPin
  )
{
  EFI_STATUS                 Status;
  HDA_PRIVATE_CONFIG         *HdaPrivateConfig;
  HDA_CONTROLLER             *HdaController;
  HDA_CALLBACK               *HdaCallbacks;
  HDAUDIO_PREMEM_CONFIG      *HdaPreMemConfig;
  HDAUDIO_CONFIG             *HdaConfig;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  HdaPreMemConfig  = HdaHandle->HdaPreMemConfig;
  HdaController    = HdaHandle->HdaController;
  HdaCallbacks     = HdaHandle->HdaCallback;
  HdaPrivateConfig = HdaHandle->HdaPrivateConfig;
  HdaConfig        = HdaHandle->HdaConfig;

  if ((HdaController == NULL) || (HdaCallbacks == NULL) || (HdaPreMemConfig == NULL) || (HdaPrivateConfig == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (PciSegmentRead16 (HdaController->PciCfgBaseAddr + R_ACE_CFG_VID) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "HDA not present!\n"));
    return EFI_ABORTED;
  }

  if (HdaPreMemConfig->Enable == TRUE) {
    Status = AceConfigureController (HdaController, HdaCallbacks, HdaConfig, HdaPreMemConfig, HdaPrivateConfig, XtalValue, InterruptPin);
  } else {
    Status = AceDisableController (HdaController, HdaCallbacks, HdaPreMemConfig);
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Initialize the Intel High Definition Audio Codec(s) present in the system.
  For each codec, a predefined codec verb table should be programmed.
  The list contains 32-bit verbs to be sent to the corresponding codec.
  If it is not programmed, the codec uses the default verb table, which may or may not
  correspond to the platform jack information.

  @param[in] HdaHandle            Pointer to Hda controller configuration structure

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_ABORTED             Controller in reset - unable to proceed with initialization
  @retval EFI_INVALID_PARAMETER   Provided VerbTableData is null
**/
EFI_STATUS
AceConfigureHdAudioCodec (
  IN HDA_HANDLE                *HdaHandle
  )
{
  EFI_STATUS                   Status;
  HDA_CONTROLLER               *HdaController;
  HDA_CALLBACK                 *HdaCallbacks;
  HDAUDIO_PREMEM_CONFIG        *HdaPreMemConfig;
  HDAUDIO_CONFIG               *HdaConfig;
  HDA_PRIVATE_CONFIG           *HdaPrivateConfig;

  HdaController    = HdaHandle->HdaController;
  HdaCallbacks     = HdaHandle->HdaCallback;
  HdaPreMemConfig  = HdaHandle->HdaPreMemConfig;
  HdaConfig        = HdaHandle->HdaConfig;
  HdaPrivateConfig = HdaHandle->HdaPrivateConfig;
  Status           = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  if ((HdaController == NULL) || (HdaCallbacks == NULL) || (HdaPreMemConfig == NULL) || (HdaConfig == NULL) || (HdaPrivateConfig == NULL)) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  if (HdaPreMemConfig->Enable == FALSE) {
    DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  if (PciSegmentRead16 (HdaController->PciCfgBaseAddr + R_ACE_CFG_VID) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "HDA not present!\n"));
    return EFI_ABORTED;
  }

  AceDetectAndInitializeHdaCodecs (HdaController, HdaConfig, HdaPreMemConfig);

  if (AceIsControllerReset (HdaController)) {
    DEBUG ((DEBUG_ERROR, "Controller in reset state - initialization aborted!\n"));
    Status = EFI_ABORTED;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
}
