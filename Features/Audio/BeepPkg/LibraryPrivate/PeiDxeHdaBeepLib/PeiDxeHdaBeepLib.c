/** @file
  HD-A Beep Common Library for both PEI and DXE phase.

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

#include <Base.h>
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiDxeHdaBeepLib.h>
#include <Library/PeiDxeAceCommonLib.h>
#include <Register/AceRegs.h>

#define ACE_MAX_LOOP_TIME_MS        2000
#define ACE_WAIT_PERIOD_MS          5
#define ACE_RESET_WAIT_TIMER        600 // Must be at least 521us (25 frames)

#define ACE_HDA_SDI_2_IDISPLINK     2
#define ACE_MAX_SDI_NUMBER          3
#define ACE_MAX_SDI_MASK            ((1 << ACE_MAX_SDI_NUMBER) - 1)

#define HDA_BEEP_SDI0_ENABLED       0x1
#define HDA_BEEP_SDI1_ENABLED       0x2
#define HDA_BEEP_BOTH_SDI_ENABLED   0x3

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
  IN      UINTN           HdaBar,
  IN OUT  UINT32          *CodecCommandData,
  IN      BOOLEAN         ReadBack
  )
{
  EFI_STATUS              Status;
  UINT32                  LoopTime;

  Status = StatusPolling16 (HdaBar + R_ACE_MEM_ICS, (UINT16) B_ACE_MEM_ICS_ICB, (UINT16) 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "ICB bit is not zero before SendCodecCommand!\n"));
    return EFI_DEVICE_ERROR;
  }

  MmioWrite32 (HdaBar + R_ACE_MEM_IC, *CodecCommandData);
  DEBUG ((DEBUG_INFO, "IC value: 0x%x.\n", *CodecCommandData));
  MmioOr16 (HdaBar + R_ACE_MEM_ICS, (UINT16) ((B_ACE_MEM_ICS_IRV | B_ACE_MEM_ICS_ICB)));

  Status = StatusPolling16 (HdaBar + R_ACE_MEM_ICS, (UINT16) B_ACE_MEM_ICS_ICB, (UINT16) 0);
  if (EFI_ERROR (Status)) {
    MmioAnd16 (HdaBar + R_ACE_MEM_ICS, (UINT16) ~(B_ACE_MEM_ICS_ICB));
    return Status;
  }

  if (ReadBack == TRUE) {
    for (LoopTime = 0; LoopTime < ACE_MAX_LOOP_TIME_MS; LoopTime++) {
      if ((MmioRead16 (HdaBar + R_ACE_MEM_ICS) & B_ACE_MEM_ICS_IRV) != 0) {
        *CodecCommandData = MmioRead32 (HdaBar + R_ACE_MEM_IR);
        DEBUG ((DEBUG_INFO, "IR value: 0x%x.\n", *CodecCommandData));
        MmioAnd16 (HdaBar + R_ACE_MEM_ICS, (UINT16) ~(B_ACE_MEM_ICS_IRV));
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
  IN  UINTN                    HdaBar,
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
  Status = AceSendHdaCodecCommand (HdaBar, CodecVendorId, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "SDI#%d: Reading the Codec Vendor ID/Device ID fail!\n", AzaliaSdiNum));
    return Status;
  }

  ///
  /// Read the Revision ID from the attached codec (ParameterId = 0x02)
  ///
  *CodecRevisionId = CODEC_REVISION_ID (AzaliaSdiNum);
  Status = AceSendHdaCodecCommand (HdaBar, CodecRevisionId, TRUE);
  if (EFI_ERROR (Status)) {
    *CodecRevisionId = 0xFF;
    DEBUG ((DEBUG_WARN, "SDI#%d: Reading the Codec Revision ID fail! Assume Codec Revision ID: 0x%x.\n", AzaliaSdiNum, *CodecRevisionId));
  } else {
    *CodecRevisionId = (*CodecRevisionId >> 8) & 0xFF;
  }

  return EFI_SUCCESS;
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
  IN UINTN   HdaBar
  )
{
  UINT32     LoopTime;
  UINT8      AzaliaSdiMask;
  UINT8      TmpPollingReg;

  AzaliaSdiMask = 0;

  for (LoopTime = 0; LoopTime < ACE_MAX_LOOP_TIME_MS; LoopTime++) {
    TmpPollingReg = (UINT8) (MmioRead8 (HdaBar + R_ACE_MEM_WAKESTS) & ACE_MAX_SDI_MASK);
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
  IN  UINTN       HdaBar,
  IN  UINT8       CodecSdiNum,
  OUT UINT32      *BclkFrequencySupportMask
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
  Status = AceSendHdaCodecCommand (HdaBar, &BclkFreqSupport, TRUE);
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
  IN  UINTN                     HdaBar,
  IN  UINT8                     CodecSdiNum,
  OUT HDAUDIO_LINK_FREQUENCY    *CurrentBclkFrequency
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
  Status   = AceSendHdaCodecCommand (HdaBar, &BclkFreq, TRUE);
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
  IN  UINTN                    HdaBar,
  IN  HDAUDIO_LINK_FREQUENCY   RequestedBclkFrequency
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
  MmioAnd32 (HdaBar + R_ACE_MEM_HDALCTL, ~((UINT32) B_ACE_MEM_HDALCTL_SPA));

  Data32 = MmioRead32 (HdaBar + R_ACE_MEM_HDALCTL);
  DEBUG ((DEBUG_ERROR, "HDALCTL after setting SPA bit to 0 : 0x%08x\n", Data32));

  //
  // This value changes to the value set by SPA when the power of the link has reached that state. Software sets SPA, then monitors CPA to know when the link has changed state.
  //
  Status = StatusPolling (HdaBar + R_ACE_MEM_HDALCTL, (UINT32) B_ACE_MEM_HDALCTL_CPA, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Turn off the link (SPA = 0) failed! CPA polling Status = %r\n", Status));
  }

  //
  // Indicates the frequency that software wishes the link to run at.
  // Changing this value to a value not supported by Link Capabilities shall result in indeterminate results.  The possible encodings are:
  // 000: 6 MHz, 001: 12 MHz, 010: 24 MHz, 011: 48 MHz, 100: 96 MHz, 101: 192 MHz, 110 - 111: Reserved
  // The value of SCF must persist across a controller reset.
  //
  MmioAndThenOr32 (
    HdaBar + R_ACE_MEM_HDALCTL,
    ~((UINT32) (B_ACE_MEM_HDALCTL_SCF)),
    (UINT32) (RequestedBclkFrequency << N_ACE_MEM_HDALCTL_SCF)
    );

  Data32 = MmioRead32 (HdaBar + R_ACE_MEM_HDALCTL);
  DEBUG ((DEBUG_INFO, "HDACTL = 0x%08x\n", Data32));

  // Controller must be out of reset (CRSTB = 1) to set the following registers
  MmioAndThenOr32 (
    HdaBar + R_ACE_MEM_HDAHWI1,
    (UINT32) ~(B_ACE_MEM_HDAHWI1_INPAY | B_ACE_MEM_HDAHWI1_OUTPAY),
    (UINT32) ((InputPayloadWords << N_ACE_MEM_HDAHWI1_INPAY) | (OutputPayloadWords << N_ACE_MEM_HDAHWI1_OUTPAY))
    );

  DEBUG ((DEBUG_INFO, "HDAHWI1 set with value = 0x%08x\n", (UINT32) ((InputPayloadWords << N_ACE_MEM_HDAHWI1_INPAY) | (OutputPayloadWords << N_ACE_MEM_HDAHWI1_OUTPAY))));
  Data32 = MmioRead32 (HdaBar  + R_ACE_MEM_HDAHWI1);
  DEBUG ((DEBUG_INFO, "OUTPAY (Output Payload Capability) = 0x%04x\n", ((Data32 & B_ACE_MEM_HDAHWI1_OUTPAY) >> N_ACE_MEM_HDAHWI1_OUTPAY)));
  DEBUG ((DEBUG_INFO, "INPAY (Input Payload Capability) = 0x%04x\n", ((Data32 & B_ACE_MEM_HDAHWI1_INPAY) >> N_ACE_MEM_HDAHWI1_INPAY)));

  //
  // Software sets this bit to '1' to turn the link on (provided CRSTB = 1), and clears it to '0' when it wishes to turn the link off.
  // When CPA matches the value of this bit, the achieved power state has been reached.  Software is expected to wait for CPA to match SPA before it can program SPA again.
  // Any deviation may result in undefined behavior.
  //
  MmioOr32 (HdaBar + R_ACE_MEM_HDALCTL, (UINT32) B_ACE_MEM_HDALCTL_SPA);

  //
  // This value changes to the value set by SPA when the power of the link has reached that state. Software sets SPA, then monitors CPA to know when the link has changed state.
  //
  Status = StatusPolling (HdaBar + R_ACE_MEM_HDALCTL, (UINT32) B_ACE_MEM_HDALCTL_CPA, (UINT32) B_ACE_MEM_HDALCTL_CPA);
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
  IN  UINTN   HdaBar,
  IN  UINT8   CodecSdiNum
  )
{
  EFI_STATUS                  Status;
  HDAUDIO_LINK_FREQUENCY      RequestedFreq;
  HDAUDIO_LINK_FREQUENCY      CurrentFreq;
  UINT32                      FreqSupportMask;

  DEBUG ((DEBUG_INFO, "%a () - Start. [SDI: %d]\n", __FUNCTION__, CodecSdiNum));

  RequestedFreq = HdaLinkFreq24MHz;

  // Set inpay default to 1c and outpay
  MmioAndThenOr32 (
    HdaBar + R_ACE_MEM_HDAHWI1,
    (UINT32) ~(B_ACE_MEM_HDAHWI1_INPAY | B_ACE_MEM_HDAHWI1_OUTPAY),
    (UINT32) ((0x1c << N_ACE_MEM_HDAHWI1_INPAY) | (0x3c << N_ACE_MEM_HDAHWI1_OUTPAY))
    );

  // Set outpay default
  Status = AceGetHdaCodecFrequencySwitchingSupport (HdaBar, CodecSdiNum, &FreqSupportMask);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = AceGetHdaCodecCurrentBclkFrequency (HdaBar, CodecSdiNum, &CurrentFreq);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (CurrentFreq == RequestedFreq) {
    DEBUG ((DEBUG_INFO, "Requested BCLK clock frequency matches current frequency (ID = %d) - Exit\n", CurrentFreq));
    return EFI_SUCCESS;
  }

  if ((FreqSupportMask >> RequestedFreq) & BIT0) {
    DEBUG ((DEBUG_INFO, "Requested Frequency ID = %d - Supported!\n", RequestedFreq));
    Status = AceSetHdaCodecBclkFrequency (HdaBar, RequestedFreq);
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }

    // Delay needed after switching frequency (link reset), before sending verb to codec.
    MicroSecondDelay (ACE_RESET_WAIT_TIMER);

    Status = AceGetHdaCodecCurrentBclkFrequency (HdaBar, CodecSdiNum, &CurrentFreq);
    if ((Status == EFI_NOT_FOUND) || (CurrentFreq != RequestedFreq)) {
      DEBUG ((DEBUG_WARN, "Frequency switching failed - return to default!\n"));
      AceSetHdaCodecBclkFrequency (HdaBar, HdaLinkFreq24MHz);
      return EFI_DEVICE_ERROR;
    }
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
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
AceSendHdaCodecVerbTable (
  IN  UINTN                  HdaBar,
  IN  HDAUDIO_VERB_TABLE     *VerbTable,
  IN  UINT8                  AzaliaSdiNum,
  OUT UINT32                 CodecVendorId,
  OUT UINT32                 CodecRevisionId
  )
{
  EFI_STATUS                 Status;
  UINT32                     CodecCmdData;
  UINT32                     Index;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a () - Start. [SDI: %d]\n", __FUNCTION__, AzaliaSdiNum));

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
    Status = AceSendHdaCodecCommand (HdaBar, &CodecCmdData, FALSE);
    if (EFI_ERROR (Status)) {
      ///
      /// Skip the Azalia verb table loading when find the verb table content is not
      /// properly matched with the HDA hardware, though IDs match.
      ///
      DEBUG ((DEBUG_WARN, "Error loading verb table for Azalia Codec of 0x%08X\n", CodecVendorId));
      break;
    }

    DEBUG ((DEBUG_VERBOSE, "[%03d] 0x%08X\n", Index, CodecCmdData));
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r\n", __FUNCTION__, Status));
  return Status;
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
  IN HDA_BEEP_CONTEXT   *HdaBeepContext,
  IN UINTN              HdaBar
  )
{
  EFI_STATUS            Status;
  UINT8                 AzaliaSdiMask;
  UINT8                 AzaliaSdiNum;
  UINT32                CodecVendorId;
  UINT32                CodecRevisionId;
  HDAUDIO_VERB_TABLE    *VerbTable;

  Status = EFI_SUCCESS;
  VerbTable = HdaBeepContext->HdaCodecVerbTables->BeepEnable;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  ///
  /// Detect active HDA codecs
  ///
  AzaliaSdiMask = AceDetectHdaCodecs (HdaBar);

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

      MmioOr16 (HdaBar + R_ACE_MEM_WAKEEN, (UINT16) (1 << AzaliaSdiNum));

      Status = AceGetHdaCodecId (HdaBar, AzaliaSdiNum, &CodecVendorId, &CodecRevisionId);
      if (EFI_ERROR(Status)) {
        continue;
      }

      DEBUG ((DEBUG_INFO, "SDI#%d: Detected HD-Audio Codec 0x%08X rev 0x%02X\n", AzaliaSdiNum, CodecVendorId, CodecRevisionId));
      ///
      /// Link static frequency switching
      ///
      AceConfigureHdaLinkFrequency (HdaBar, AzaliaSdiNum);


      ///
      /// Check if verb table for beep matches the codec on SDI line
      ///
      if ((VerbTable != NULL) &&
          ((((UINT32) (VerbTable->Header.VendorId) << 16) | VerbTable->Header.DeviceId) == CodecVendorId) &&
          ((VerbTable->Header.RevisionId == 0xFF) || (VerbTable->Header.RevisionId == CodecRevisionId)) &&
          ((VerbTable->Header.SdiNum == 0xFF) || VerbTable->Header.SdiNum == AzaliaSdiNum)) {
        DEBUG ((
          DEBUG_INFO,
          "Found Verb Table for VendorID 0x%04X, DeviceId 0x%04X,"
          " RevisionID 0x%02X (SDI:%X, size: %d dwords)\n",
          VerbTable->Header.VendorId,
          VerbTable->Header.DeviceId,
          VerbTable->Header.RevisionId,
          VerbTable->Header.SdiNum,
          VerbTable->Header.DataDwords
          ));
      } else {
        DEBUG ((DEBUG_INFO, "No Verb Table is found that matches the codec on the SDI#%d line.\n", AzaliaSdiNum));
        continue;
      }

      ///
      /// Send Verb Table if required table exist based on Codec Vendor ID and Codec Revision ID
      ///
      Status = AceSendHdaCodecVerbTable (HdaBar, VerbTable, AzaliaSdiNum, CodecVendorId, CodecRevisionId);
      if (EFI_ERROR(Status)) {
        continue;
      }

      HdaBeepContext->HdaCodecInfo.IsCodecInitialized  = TRUE;
      HdaBeepContext->HdaCodecInfo.SdiNum              = AzaliaSdiNum;
      HdaBeepContext->HdaCodecInfo.VendorId            = CodecVendorId;
      HdaBeepContext->HdaCodecInfo.RevisionId          = CodecRevisionId;
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
  This function turns on sine generator located in HDA codec

  @param[in] This            Pointer to Beep PPI/Protocol.
  @param[in] Frequency       Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude       Amplitude value for tone generator. Refer to codec specification.

  @retval EFI_SUCCESS        The function completed successfully.
  @retval EFI_NOT_READY      The codec is not ready to receive verbs for beep
**/
EFI_STATUS
EFIAPI
HdaBeepOn (
  IN CONST BEEP              *This,
  IN UINTN                   Frequency,
  IN INTN                    Amplitude
  )
{
  EFI_STATUS                 Status;
  UINTN                      HdaBar;
  BOOLEAN                    IsTemporaryBar;
  HDA_BEEP_CONTEXT           *HdaBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  HdaBeepContext = HDA_BEEP_CONTEXT_FROM_HDA_BEEP_PPI_PROTOCOL (This);

  Status = EnableHdaMmioAccess (HdaBeepContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  if (!HdaBeepContext->HdaCodecInfo.IsCodecInitialized) {
    DEBUG ((DEBUG_ERROR, "The targeted codec has not been initialized, aborting.\n"));
    return EFI_NOT_READY;
  }

  Status = AceSendHdaCodecVerbTable (
             HdaBar,
             HdaBeepContext->HdaCodecVerbTables->BeepOn,
             HdaBeepContext->HdaCodecInfo.SdiNum,
             HdaBeepContext->HdaCodecInfo.VendorId,
             HdaBeepContext->HdaCodecInfo.RevisionId
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Couldn't send the verbs to start beeping on the codec, aborting.\n"));
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return EFI_SUCCESS;
}

/**
  This function turns off sine generator located in HDA codec

  @param[in] This               Pointer to Beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
EFI_STATUS
EFIAPI
HdaBeepOff (
  IN CONST BEEP                 *This
  )
{
  EFI_STATUS                    Status;
  UINTN                         HdaBar;
  BOOLEAN                       IsTemporaryBar;
  HDA_BEEP_CONTEXT              *HdaBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  HdaBeepContext = HDA_BEEP_CONTEXT_FROM_HDA_BEEP_PPI_PROTOCOL (This);

  Status = EnableHdaMmioAccess (HdaBeepContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  if (!HdaBeepContext->HdaCodecInfo.IsCodecInitialized) {
    DEBUG ((DEBUG_ERROR, "The targeted codec has not been initialized, aborting.\n"));
    return EFI_NOT_READY;
  }

  Status = AceSendHdaCodecVerbTable (
             HdaBar,
             HdaBeepContext->HdaCodecVerbTables->BeepOff,
             HdaBeepContext->HdaCodecInfo.SdiNum,
             HdaBeepContext->HdaCodecInfo.VendorId,
             HdaBeepContext->HdaCodecInfo.RevisionId
             );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Couldn't send the verbs to stop beeping on the codec, aborting.\n"));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return EFI_SUCCESS;
}

/**
  This function turns on sine generator located in HDA codec

  @param[in] This            Pointer to Beep PPI/Protocol.
  @param[in] Frequency       Frequency value for tone generator. Refer to codec specification.
  @param[in] Amplitude       Amplitude value for tone generator. Refer to codec specification.
  @param[in] NumOfBeeps      Number of beeps.
  @param[in] BeepDuration    Length of beep sound in miliseconds.
  @param[in] PauseDuration   Duration of pause between beeps in miliseconds.

  @retval EFI_SUCCESS        The function completed successfully.
  @retval EFI_DEVICE_ERROR   Not able to enable DSP memory space.

**/
EFI_STATUS
EFIAPI
HdaBeep (
  IN CONST BEEP              *This,
  IN UINTN                   Frequency,
  IN INTN                    Amplitude,
  IN UINTN                   NumOfBeeps,
  IN UINTN                   BeepDuration,
  IN UINTN                   PauseDuration
  )
{
  EFI_STATUS                 Status;
  UINTN                      Index;
  UINTN                      HdaBar;
  BOOLEAN                    IsTemporaryBar;
  HDA_BEEP_CONTEXT           *HdaBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  Status = EFI_SUCCESS;

  HdaBeepContext = HDA_BEEP_CONTEXT_FROM_HDA_BEEP_PPI_PROTOCOL (This);

  Status = EnableHdaMmioAccess (HdaBeepContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  if (!HdaBeepContext->HdaCodecInfo.IsCodecInitialized) {
    DEBUG ((DEBUG_ERROR, "The targeted codec has not been initialized, aborting.\n"));
    return EFI_NOT_READY;
  }

  for (Index = 0; Index < NumOfBeeps; Index++) {
    Status = AceSendHdaCodecVerbTable (
               HdaBar,
               HdaBeepContext->HdaCodecVerbTables->BeepOn,
               HdaBeepContext->HdaCodecInfo.SdiNum,
               HdaBeepContext->HdaCodecInfo.VendorId,
               HdaBeepContext->HdaCodecInfo.RevisionId
               );
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Couldn't send the verbs to start beeping on the codec, aborting.\n"));
    }

    MicroSecondDelay (BeepDuration * 1000);

    Status = AceSendHdaCodecVerbTable (
               HdaBar,
               HdaBeepContext->HdaCodecVerbTables->BeepOff,
               HdaBeepContext->HdaCodecInfo.SdiNum,
               HdaBeepContext->HdaCodecInfo.VendorId,
               HdaBeepContext->HdaCodecInfo.RevisionId
               );
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "Couldn't send the verbs to stop beeping on the codec, aborting.\n"));
      return EFI_DEVICE_ERROR;
    }

    MicroSecondDelay (PauseDuration * 1000);
  }

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return EFI_SUCCESS;
}

/**
  This function enables HDA interface.

  @param[in] This               Pointer to Beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
EFI_STATUS
EFIAPI
HdaEnable (
  IN CONST BEEP                 *This
  )
{
  EFI_STATUS                    Status;
  UINTN                         HdaBar;
  BOOLEAN                       IsTemporaryBar;
  HDA_BEEP_CONTEXT              *HdaBeepContext;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  Status = EFI_SUCCESS;

  HdaBeepContext = HDA_BEEP_CONTEXT_FROM_HDA_BEEP_PPI_PROTOCOL (This);

  Status = EnableHdaMmioAccess (HdaBeepContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  if (IsHdaControllerReset (HdaBar)) {
    DEBUG ((DEBUG_INFO, "HDA Controller is in reset state, getting out of reset...\n"));
    Status = AceSetControllerReset (HdaBar, FALSE);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Un-reset High Definition Audio (CRST = 1) Time Out!\n"));
      return EFI_TIMEOUT;
    }
  }

  AceDetectAndInitializeHdaCodecs (HdaBeepContext, HdaBar);

  DEBUG ((DEBUG_INFO, "%a () - End. Status = %r.\n", __FUNCTION__, Status));

  return EFI_SUCCESS;
}

/**
  This function disables HDA interface.

  @param[in] This               Pointer to Beep PPI/Protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_DEVICE_ERROR      Not able to enable DSP memory space.
**/
EFI_STATUS
EFIAPI
HdaDisable (
  IN CONST BEEP                 *This
  )
{
  UINTN                         HdaBar;
  BOOLEAN                       IsTemporaryBar;
  HDA_BEEP_CONTEXT              *HdaBeepContext;
  EFI_STATUS                    Status;

  DEBUG ((DEBUG_INFO, "%a () - Start.\n", __FUNCTION__));

  HdaBeepContext = HDA_BEEP_CONTEXT_FROM_HDA_BEEP_PPI_PROTOCOL (This);

  Status = EnableHdaMmioAccess (HdaBeepContext->PciIo, &HdaBar, &IsTemporaryBar);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "EnableHdaMmioAccess () failed! Status = %r\n", Status));
    return Status;
  }

  ResetDsp (HdaBar);
  DisableHdaDspMmioAccess (HdaBeepContext->PciIo, IsTemporaryBar);

  DEBUG ((DEBUG_INFO, "%a () - End.\n", __FUNCTION__));

  return EFI_SUCCESS;
}
