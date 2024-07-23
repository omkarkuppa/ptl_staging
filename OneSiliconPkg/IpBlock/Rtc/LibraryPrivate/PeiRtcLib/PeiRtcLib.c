/** @file
  Initializes PCH RTC Device in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Ppi/SiPolicy.h>
#include <RtcConfig.h>
#include <Register/RtcRegs.h>
#include <Register/TcoRegs.h>
#include <RtcHandle.h>

/**
  Configura RTC when power failure case.
**/
VOID
RtcPowerFailureConfiguration (
  VOID
  )
{
  EFI_STATUS              Status;
  UINT16                  TcoBase;
  ///
  /// PCH BIOS Spec Power Failure Considerations
  /// RTC_PWR_STS bit, GEN_PMCON_B (PMC PCI offset A4h[2])
  /// When the RTC_PWR_STS bit is set, it indicates that the RTCRST# signal went low.
  /// Software should clear this bit. For example, changing the RTC battery sets this bit.
  /// System BIOS should reset CMOS to default values if the RTC_PWR_STS bit is set.
  /// The System BIOS should execute the sequence below if the RTC_PWR_STS bit is set
  /// before memory initialization. This will ensure that the RTC state machine has been
  /// initialized.
  /// 1. If the RTC_PWR_STS bit is set which indicates a new coin-cell battery insertion or a
  ///    battery failure, steps 2 through 5 should be executed.
  /// 2. Set RTC Register 0Ah[6:4] to 110b or 111b
  /// 3. Set RTC Register 0Bh[7].
  /// 4. Set RTC Register 0Ah[6:4] to 010b
  /// 5. Clear RTC Register 0Bh[7].
  ///

  ///
  /// 2. Set RTC Register 0Ah[6:4] to 110b or 111b
  ///
  IoWrite8 (R_RTC_IO_INDEX_ALT, (UINT8) R_RTC_IO_REGISTER_A);
  IoAndThenOr8 (
    R_RTC_IO_TARGET_ALT,
    (UINT8) ~(BIT6 | BIT5 | BIT4),
    (UINT8) (BIT6 | BIT5)
    );
  ///
  /// 3. Set RTC Register 0Bh[7].
  ///
  IoWrite8 (R_RTC_IO_INDEX_ALT, (UINT8) R_RTC_IO_REGISTER_B);
  IoOr8 (R_RTC_IO_TARGET_ALT, (UINT8) B_RTC_IO_REGISTER_B_RTC_SET);
  ///
  /// 4. Set RTC Register 0Ah[6:4] to 010b
  ///
  IoWrite8 (R_RTC_IO_INDEX_ALT, (UINT8) R_RTC_IO_REGISTER_A);
  IoAndThenOr8 (
    R_RTC_IO_TARGET_ALT,
    (UINT8) ~(BIT6 | BIT5 | BIT4),
    (UINT8) (BIT5)
    );
  ///
  /// 5. Clear RTC Register 0Bh[7].
  ///
  IoWrite8 (R_RTC_IO_INDEX_ALT, (UINT8) R_RTC_IO_REGISTER_B);
  IoAnd8 (R_RTC_IO_TARGET_ALT, (UINT8) ~B_RTC_IO_REGISTER_B_RTC_SET);

  ///
  /// The INTRUDER# input is overloaded as a pin-strap in ICP,
  /// there might be false INTRUDER event status set on RTC reset.
  /// BIOS must clear INTRUDER status if RTC_PWR_STS is set.
  ///
  Status = PchTcoBaseGet (&TcoBase);
  ASSERT_EFI_ERROR (Status);
  if (!EFI_ERROR (Status)) {
    IoWrite16 (TcoBase + R_TCO_IO_TSTS2, B_TCO_IO_TSTS2_INTRD_DET);
  }
}

/**
  The function performs RTC specific programming.

  @param[in]  RtcHandle           Pointer to RTC handle structure

**/
VOID
RtcConfiguration (
  IN  RTC_HANDLE                  *RtcHandle
  )
{
  REGISTER_ACCESS                 *RtcPcrAccess;
  UINT32                          OriginalData;
  UINT32                          TestResult;

  RtcPcrAccess = RtcHandle->RtcPcrAccess;

  ///
  /// Set PCR[RTC] + 3400h[6] = 1b
  ///
  RtcPcrAccess->AndThenOr32 (
    RtcPcrAccess,
    R_RTC_PCR_RC,
    ~0u,
    B_RTC_PCR_RC_HPM_HW_DIS
    );
  ///
  /// Set PCR[RTC] + 3418h[2:0] = 111b
  ///
  RtcPcrAccess->AndThenOr32 (
    RtcPcrAccess,
    R_RTC_PCR_RTCDCG,
    ~0u,
    B_RTC_PCR_RTCDCG_RTCPGCBDCGEN | B_RTC_PCR_RTCDCG_RTCPCICLKDCGEN | B_RTC_PCR_RTCDCG_RTCROSIDEDCGEN
    );
  ///
  /// Set PCR[RTC] + 3F00h[8] = 1b
  ///
  RtcPcrAccess->AndThenOr32 (
    RtcPcrAccess,
    R_RTC_PCR_RTCTM,
    ~0u,
    BIT8
    );
  ///
  /// Set Daylight Saving Override bit in RTC by default
  /// @note make sure to mask TopSwap bit, so it's not accidentally set to 1 because
  /// it'll latch in RTC and will be restored after reset even if TopSwap was desired
  /// to be disabled
  ///

  OriginalData = RtcPcrAccess->Read32 (RtcPcrAccess, R_RTC_PCR_BUC);
  RtcPcrAccess->AndThenOr32 (RtcPcrAccess, R_RTC_PCR_BUC, (UINT32)~(B_RTC_PCR_BUC_TS), 0);
  TestResult = RtcPcrAccess->Read32 (RtcPcrAccess, R_RTC_PCR_BUC);

  if (((OriginalData & TestResult) & B_RTC_PCR_BUC_TS) == B_RTC_PCR_BUC_TS)  {
    //
    // TS is asserted by HW. Mask TopSwap bit, so it's not accidentally set to 1.
    //
    RtcPcrAccess->AndThenOr32 (RtcPcrAccess, R_RTC_PCR_BUC, (UINT32)~(B_RTC_PCR_BUC_TS), B_RTC_PCR_BUC_SDO);
  } else {
    //
    // TS is not asserted by HW, keep the value as is.
    //
    RtcPcrAccess->AndThenOr32 (RtcPcrAccess, R_RTC_PCR_BUC, (UINT32)~0, (OriginalData & B_RTC_PCR_BUC_TS) | B_RTC_PCR_BUC_SDO);
  }

  ///
  /// Remove HOSTIA_POSTBOOT_SAI from Control Policy of Policy Group 1 Registers.
  ///
  RtcPcrAccess->AndThenOr32 (RtcPcrAccess, R_RTC_PCR_SAIPG1_CP_LO, (UINT32)~(BIT0), 0);

}

/**
  The function performs RTC lock setting.

  @param[in]  RtcHandle       Pointer to RTC handle structure
**/
VOID
RtcLock (
  IN  RTC_HANDLE              *RtcHandle
  )
{
  RTC_CONFIG                  *RtcConfig;
  REGISTER_ACCESS             *RtcPcrAccess;
  UINT32                      Data32And;
  UINT32                      Data32Or;

  RtcConfig = RtcHandle->RtcConfig;
  RtcPcrAccess = RtcHandle->RtcPcrAccess;
  ///
  /// PCH BIOS Spec on using RTC RAM
  /// Regardless of BUC.TS being updated or not, BIOS must set RC.BILD bit PCR[RTC] + 3400h[31] before exit
  /// For Data integrity protection, set RTC Memory locks (Upper 128 Byte Lock and
  /// Lower 128 Byte Lock) at PCR[RTC] + 3400h[4] and PCR[RTC] + 3400h[3].
  /// Note once locked bytes 0x38 - 0x3F in each of the Upper and Lower Byte blocks, respectively,
  /// cannot be unlocked until next reset.
  ///
  Data32And = ~0u;
  Data32Or  = 0;
  if (RtcConfig->BiosInterfaceLock == TRUE) {
    Data32Or |= B_RTC_PCR_RC_BILD;
  }
  if (RtcConfig->MemoryLock == TRUE) {
    Data32Or |= (B_RTC_PCR_RC_UL | B_RTC_PCR_RC_LL);
  }
  RtcPcrAccess->AndThenOr32 (
    RtcPcrAccess,
    R_RTC_PCR_RC,
    Data32And,
    Data32Or
    );
}
