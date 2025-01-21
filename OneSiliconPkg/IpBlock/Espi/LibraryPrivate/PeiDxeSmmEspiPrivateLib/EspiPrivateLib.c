/** @file
  This file contains private routines for eSPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/PchInfoLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/S3PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/EspiPrivateLib.h>
#include <Library/PchLimits.h>
#include <Register/EspiRegs.h>
#include <Register/SpiRegs.h>
#include <RegisterAccess.h>

#define CHANNEL_RESET_TIMEOUT  100      ///< Channel reset timeout in us after which to report error
#define DEVICE_CHANNELS_MAX    7        ///< Max number of channels

//
// eSPI device registers
//
#define R_ESPI_DEVICE_GENCAP           0x08          ///< General Capabilities and Configurations
#define B_ESPI_DEVICE_GENCAP_SUPPCHAN  0xFF          ///< Channels supported bit mask
#define R_ESPI_DEVICE_CHACAP_BASE      0x10          ///< Base address from which channel Cap and Conf registers start on device
#define S_ESPI_DEVICE_CHACAP_OFFSET    0x10          ///< Offset for each channel from base
#define B_ESPI_DEVICE_CHACAP_CHEN      BIT0          ///< device Channel enable bit
#define B_ESPI_DEVICE_CHACAP_CHRDY     BIT1          ///< device Channel ready bit

/**
  Checks if given eSPI device is supported

  @param[in]  DeviceId        Id of device to check
  @param[in]  EspiPcrAccess   eSPI Sideband registers access protocol

  @retval TRUE      There's second device
  @retval FALSE     There's no second device
**/
BOOLEAN
IsEspiDeviceSupported (
  IN ESPI_DEVICE_ID  DeviceId,
  IN REGISTER_ACCESS *EspiPcrAccess
  )
{
  UINT32  Data32;

  switch (DeviceId) {
    case 0:
      return TRUE;  // CS#0 is always supported
    case 1:
      Data32 = EspiPcrAccess->Read32 (EspiPcrAccess, R_SPI_PCR_ESPI_SOFTSTRAP_REG0);
      return ((Data32 & B_SPI_PCR_ESPI_SOFTSTRAP_REG0_CS1_EN) != 0);
    case 2:
      Data32 = EspiPcrAccess->Read32 (EspiPcrAccess, R_SPI_PCR_ESPI_SOFTSTRAP_REG_EXT_2);
      return ((Data32 & B_SPI_PCR_ESPI_SOFTSTRAP_REG_EXT_2_CS_EN) != 0);
    case 3:
      Data32 = EspiPcrAccess->Read32 (EspiPcrAccess, R_SPI_PCR_ESPI_SOFTSTRAP_REG_EXT_3);
      return ((Data32 & B_SPI_PCR_ESPI_SOFTSTRAP_REG_EXT_2_CS_EN) != 0);
    default:
      return FALSE;
  }
}

/**
  Checks in device General Capabilities register if it supports channel with requested number

  @param[in]  DeviceId        Id of device to check
  @param[in]  ChannelNumber   Number of channel of which to check
  @param[in]  EspiPcrAccess   eSPI Sideband registers access protocol

  @retval TRUE      Channel with requested number is supported by device
  @retval FALSE     Channel with requested number is not supported by device
**/
BOOLEAN
IsEspiDeviceChannelSupported (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  UINT8           ChannelNumber,
  IN  REGISTER_ACCESS *EspiPcrAccess
  )
{
  EFI_STATUS  Status;
  UINT32      Data32;
  UINT8       SupportedChannels;

  if (!IsEspiDeviceSupported (DeviceId, EspiPcrAccess)) {
    // Device is not supported
    return FALSE;
  }

  Status = PchEspiDeviceGetConfig (DeviceId, R_ESPI_DEVICE_GENCAP, EspiPcrAccess, &Data32);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  SupportedChannels = (UINT8)(Data32 & B_ESPI_DEVICE_GENCAP_SUPPCHAN);

  DEBUG ((DEBUG_INFO, "Device %d supported channels 0x%4X\n", DeviceId, SupportedChannels));

  if ((ChannelNumber > DEVICE_CHANNELS_MAX) || !(SupportedChannels & (BIT0 << ChannelNumber))) {
    // Incorrect channel number was specified. Either exceeded max or device doesn't support that channel.
    return FALSE;
  }

  return TRUE;
}

/**
  Is eSPI enabled in strap.

  @param[in] EspiPcrAccess        eSPI Sideband registers access protocol

  @retval TRUE          Espi is enabled in strap
  @retval FALSE         Espi is disabled in strap
**/
BOOLEAN
IsEspiEnabled (
  IN REGISTER_ACCESS *EspiPcrAccess
  )
{
  return (EspiPcrAccess->Read32 (EspiPcrAccess, R_SPI_PCR_SPI_CONFIGURATION_VAL) & B_SPI_PCR_SPI_CONFIGURATION_VAL_ESPI_EN) != 0;
}

/**
  eSPI helper function to clear device configuration register status

  @param[in] EspiPcrAccess        eSPI Sideband registers access protocol

  @retval EFI_SUCCESS Write to private config space succeed
  @retval others      Read / Write failed
**/
STATIC
VOID
EspiClearScrs (
  IN REGISTER_ACCESS *EspiPcrAccess
  )
{
  EspiPcrAccess->AndThenOr32 (
                   EspiPcrAccess,
                   R_ESPI_PCR_SLV_CFG_REG_CTL,
                   (UINT32) ~0,
                   B_ESPI_PCR_SLV_CFG_REG_CTL_SCRS
                   );
}

/**
  eSPI helper function to poll device configuration register enable for 0
  and to check for device configuration register status

  @param[in] EspiPcrAccess        eSPI Sideband registers access protocol

  @retval EFI_SUCCESS       Enable bit is zero and no error in status bits
  @retval EFI_DEVICE_ERROR  Error in SCRS
  @retval others            Read / Write to private config space failed
**/
STATIC
EFI_STATUS
EspiPollScreAndCheckScrs (
  IN REGISTER_ACCESS *EspiPcrAccess
  )
{
  UINT32  ScrStat;
  UINT32  Counter;

  Counter = 1000;

  do {
    ScrStat = EspiPcrAccess->Read32 (EspiPcrAccess, R_ESPI_PCR_SLV_CFG_REG_CTL);
    if (Counter-- <= 0) {
      break;
    }
  } while ((ScrStat & B_ESPI_PCR_SLV_CFG_REG_CTL_SCRE) != 0);

  ScrStat = (ScrStat & B_ESPI_PCR_SLV_CFG_REG_CTL_SCRS) >> N_ESPI_PCR_SLV_CFG_REG_CTL_SCRS;
  if (ScrStat != V_ESPI_PCR_SLV_CFG_REG_CTL_SCRS_NOERR) {
    DEBUG ((DEBUG_WARN, "eSPI device config register status (error) is %x \n", ScrStat));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

typedef enum {
  EspiDeviceOperationConfigRead,
  EspiDeviceOperationConfigWrite,
  EspiDeviceOperationStatusRead,
  EspiDeviceOperationInBandReset
} ESPI_DEVICE_OPERATION;

/**
  Helper library to do all the operations regards to eSPI device

  @param[in]      DeviceId         eSPI device ID
  @param[in]      DeviceAddress    Device address to be put in R_ESPI_PCR_SLV_CFG_REG_CTL[11:0]
  @param[in]      DeviceOperation  Based on ESPI_DEVICE_OPERATION
  @param[in]      EspiPcrAccess    eSPI Sideband registers access protocol
  @param[in,out]  Data

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER Device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_INVALID_PARAMETER Device configuration register address exceed maximum allowed
  @retval EFI_INVALID_PARAMETER Device configuration register address is not DWord aligned
  @retval EFI_ACCESS_DENIED     eSPI device write to address range 0 to 0x7FF has been locked
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
STATIC
EFI_STATUS
EspiDeviceOperationHelper (
  IN     ESPI_DEVICE_ID        DeviceId,
  IN     UINT32                DeviceAddress,
  IN     ESPI_DEVICE_OPERATION DeviceOperation,
  IN     REGISTER_ACCESS       *EspiPcrAccess,
  IN OUT UINT32                *Data
  )
{
  EFI_STATUS  Status;
  UINT32      Data32;
  UINT16      RegOffset;

  //
  // Check the DeviceId is 0 or 1
  //
  if (DeviceId >= PCH_MAX_ESPI_DEVICES) {
    DEBUG ((DEBUG_WARN, "eSPI device ID of %d or more is not accepted \n", PCH_MAX_ESPI_DEVICES));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if DeviceId 1 is used, it is a PCH_H
  //
  if (!IsEspiDeviceSupported (DeviceId, EspiPcrAccess)) {
    DEBUG ((DEBUG_WARN, "eSPI device ID of %d is not supported.\n", DeviceId));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check the address is not more then 0xFFF
  //
  if (DeviceAddress > B_ESPI_PCR_SLV_CFG_REG_CTL_SCRA) {
    DEBUG ((DEBUG_WARN, "eSPI device address must be less than 0x%x \n", (B_ESPI_PCR_SLV_CFG_REG_CTL_SCRA + 1)));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check the address is DWord aligned
  //
  if ((DeviceAddress & 0x3) != 0) {
    DEBUG ((DEBUG_WARN, "eSPI device address must be DWord aligned \n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if write is allowed
  //
  if ((DeviceOperation == EspiDeviceOperationConfigWrite) &&
      (DeviceAddress <= 0x7FF))
  {
    //
    // If the SLCRR is not set in corresponding device, we will check the lock bit
    //
    if (DeviceId < 2) {
      RegOffset = (UINT16)(R_ESPI_PCR_LNKERR_SLV0 + (DeviceId * S_ESPI_PCR_LNKERR_SLV0));
    } else {
      // Subtracting 2 from DeviceId so the offset is calculated correctly and not increased too much
      RegOffset = (UINT16)(R_ESPI_PCR_LNKERR_SLV_EXT_2 + ((DeviceId - 2) * S_ESPI_PCR_LNKERR_SLV0));
    }

    Data32 = EspiPcrAccess->Read32 (EspiPcrAccess, RegOffset);
    if ((Data32 & B_ESPI_PCR_LNKERR_SLV0_SLCRR) == 0) {
      Data32 = EspiPcrAccess->Read32 (EspiPcrAccess, (UINT16)R_ESPI_PCR_SLV_CFG_REG_CTL);
      if ((Data32 & B_ESPI_PCR_SLV_CFG_REG_CTL_SBLCL) != 0) {
        DEBUG ((DEBUG_WARN, "eSPI device write to address range 0 to 0x7FF has been locked \n"));
        return EFI_ACCESS_DENIED;
      }
    }
  }

  //
  // Input check done, now go through all the processes
  //
  EspiClearScrs (EspiPcrAccess);

  if (DeviceOperation == EspiDeviceOperationConfigWrite) {
    EspiPcrAccess->Write32 (
                     EspiPcrAccess,
                     (UINT16)R_ESPI_PCR_SLV_CFG_REG_DATA,
                     *Data
                     );
  }

  EspiPcrAccess->AndThenOr32 (
                   EspiPcrAccess,
                   (UINT16)R_ESPI_PCR_SLV_CFG_REG_CTL,
                   (UINT32)~(B_ESPI_PCR_SLV_CFG_REG_CTL_SID | B_ESPI_PCR_SLV_CFG_REG_CTL_SCRT | B_ESPI_PCR_SLV_CFG_REG_CTL_SCRA),
                   (B_ESPI_PCR_SLV_CFG_REG_CTL_SCRE | DeviceId << N_ESPI_PCR_SLV_CFG_REG_CTL_SID) |
                   (((UINT32)DeviceOperation) << N_ESPI_PCR_SLV_CFG_REG_CTL_SCRT) |
                   DeviceAddress
                   );

  Status = EspiPollScreAndCheckScrs (EspiPcrAccess);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if ((DeviceOperation == EspiDeviceOperationConfigRead) || (DeviceOperation == EspiDeviceOperationStatusRead)) {
    Data32 = EspiPcrAccess->Read32 (
                              EspiPcrAccess,
                              (UINT16)R_ESPI_PCR_SLV_CFG_REG_DATA
                              );
    if (DeviceOperation == EspiDeviceOperationStatusRead) {
      *Data = Data32 & 0xFFFF;
    } else {
      *Data = Data32;
    }
  }

  return EFI_SUCCESS;
}

/**
  Get configuration from eSPI device

  @param[in]  DeviceId       eSPI device ID
  @param[in]  DeviceAddress  Device Configuration Register Address
  @param[in]  EspiPcrAccess  eSPI Sideband registers access protocol
  @param[out] OutData        Configuration data read

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER device ID is not supported
  @retval EFI_INVALID_PARAMETER device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_INVALID_PARAMETER device configuration register address exceed maximum allowed
  @retval EFI_INVALID_PARAMETER device configuration register address is not DWord aligned
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiDeviceGetConfig (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  UINT32          DeviceAddress,
  IN  REGISTER_ACCESS *EspiPcrAccess,
  OUT UINT32          *OutData
  )
{
  //
  // 1. Clear status from previous transaction by writing 111b to status in SCRS, PCR[eSPI] + 4000h [30:28]
  // 2. Program SLV_CFG_REG_CTL with the right value (Bit[31]=01, Bit [20:19]=<SlvID>, Bit [17:16] = 00b, Bit[11:0] = <addr_xxx>.
  // 3. Poll the SCRE (PCR[eSPI] +4000h [31]) to be set back to 0
  // 4. Check the transaction status in SCRS (bits [30:28])
  // 5. Read SLV_CFG_REG_DATA.
  //
  return EspiDeviceOperationHelper (DeviceId, DeviceAddress, EspiDeviceOperationConfigRead, EspiPcrAccess, OutData);
}

/**
  Set eSPI device configuration

  Note: A Set_Configuration must always be followed by a Get_Configuration in order to ensure
  that the internal state of the eSPI-MC is consistent with the device register settings.

  @param[in]  DeviceId       eSPI device ID
  @param[in]  DeviceAddress  device Configuration Register Address
  @param[in]  EspiPcrAccess  eSPI Sideband registers access protocol
  @param[in]  InData         Configuration data to write

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_INVALID_PARAMETER device configuration register address exceed maximum allowed
  @retval EFI_INVALID_PARAMETER device configuration register address is not DWord aligned
  @retval EFI_ACCESS_DENIED     eSPI device write to address range 0 to 0x7FF has been locked
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiDeviceSetConfig (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  UINT32          DeviceAddress,
  IN  REGISTER_ACCESS *EspiPcrAccess,
  IN  UINT32          InData
  )
{
  EFI_STATUS  Status;
  UINT32      Data32;

  //
  // 1. Clear status from previous transaction by writing 111b to status in SCRS, PCR[eSPI] + 4000h [30:28]
  // 2. Program SLV_CFG_REG_DATA with the write value.
  // 3. Program SLV_CFG_REG_CTL with the right value (Bit[31]=01, Bit [20:19]=<SlvID>, Bit [17:16] = 01b, Bit[11:0] = <addr_xxx>.
  // 4. Poll the SCRE (PCR[eSPI] +4000h [31]) to be set back to 0
  // 5. Check the transaction status in SCRS (bits [30:28])
  //
  Status = EspiDeviceOperationHelper (DeviceId, DeviceAddress, EspiDeviceOperationConfigWrite, EspiPcrAccess, &InData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PchEspiDeviceGetConfig (DeviceId, DeviceAddress, EspiPcrAccess, &Data32);
  return Status;
}

/**
  Get status from eSPI device

  @param[in]  DeviceId       eSPI device ID
  @param[in]  EspiPcrAccess  eSPI Sideband registers access protocol
  @param[out] OutData        Configuration data read

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiDeviceGetStatus (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  REGISTER_ACCESS *EspiPcrAccess,
  OUT UINT16          *OutData
  )
{
  EFI_STATUS  Status;
  UINT32      TempOutData;

  TempOutData = 0;

  //
  // 1. Clear status from previous transaction by writing 111b to status in SCRS, PCR[eSPI] + 4000h [30:28]
  // 2. Program SLV_CFG_REG_CTL with the right value (Bit[31]=01, Bit [20:19]=<SlvID>, Bit [17:16] = 10b, Bit[11:0] = <addr_xxx>.
  // 3. Poll the SCRE (PCR[eSPI] +4000h [31]) to be set back to 0
  // 4. Check the transaction status in SCRS (bits [30:28])
  // 5. Read SLV_CFG_REG_DATA [15:0].
  //
  Status   = EspiDeviceOperationHelper (DeviceId, 0, EspiDeviceOperationStatusRead, EspiPcrAccess, &TempOutData);
  *OutData = (UINT16)TempOutData;

  return Status;
}

/**
  eSPI device in-band reset

  @param[in]  DeviceId          eSPI device ID
  @param[in]  EspiPcrAccess  eSPI Sideband registers access protocol

  @retval EFI_SUCCESS           Operation succeed
  @retval EFI_INVALID_PARAMETER device ID is not supported or DeviceId 1 is used in PCH_LP
  @retval EFI_DEVICE_ERROR      Error in SCRS during polling stage of operation
**/
EFI_STATUS
PchEspiDeviceInBandReset (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  REGISTER_ACCESS *EspiPcrAccess
  )
{
  //
  // 1. Clear status from previous transaction by writing 111b to status in SCRS, PCR[eSPI] + 4000h [30:28]
  // 2. Program SLV_CFG_REG_CTL with the right value (Bit[31]=01, Bit [20:19]=<SlvID>, Bit [17:16] = 11b).
  // 3. Poll the SCRE (PCR[eSPI] +4000h [31]) to be set back to 0
  // 4. Check the transaction status in SCRS (bits [30:28])
  //
  return EspiDeviceOperationHelper (DeviceId, 0, EspiDeviceOperationInBandReset, EspiPcrAccess, NULL);
}

/**
  eSPI device channel reset helper function

  @param[in]  DeviceId          eSPI device ID
  @param[in]  ChannelNumber     Number of channel to reset
  @param[in]  EspiPcrAccess     eSPI Sideband registers access protocol

  @retval     EFI_SUCCESS       Operation succeeded
  @retval     EFI_UNSUPPORTED   device doesn't support that channel or invalid number specified
  @retval     EFI_TIMEOUT       Operation has timeouted
**/
EFI_STATUS
PchEspiDeviceChannelReset (
  IN  ESPI_DEVICE_ID  DeviceId,
  IN  UINT8           ChannelNumber,
  IN  REGISTER_ACCESS *EspiPcrAccess
  )
{
  UINT8       Timeout;
  UINT32      Data32;
  UINT32      DeviceChannelAddress;
  BOOLEAN     DeviceBmeSet;
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "eSPI device %d channel %d reset\n", DeviceId, ChannelNumber));

  Timeout = CHANNEL_RESET_TIMEOUT;
  DeviceBmeSet = FALSE;

  if (!IsEspiDeviceChannelSupported (DeviceId, ChannelNumber, EspiPcrAccess)) {
    // Incorrect channel number was specified. Either exceeded max or device doesn't support that channel.
    DEBUG ((DEBUG_WARN, "Channel %d is not valid channel number for device %d!\n", ChannelNumber, DeviceId));
    return EFI_UNSUPPORTED;
  }

  // Calculating device channel address
  DeviceChannelAddress = R_ESPI_DEVICE_CHACAP_BASE + (S_ESPI_DEVICE_CHACAP_OFFSET * ChannelNumber);

  // If we're resetting Peripheral Channel then we need to disable Bus Mastering first and reenable after reset
  if (ChannelNumber == 0) {
    Status = PchEspiDeviceGetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, &Data32);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if ((Data32 & B_ESPI_DEVICE_BME) != 0) {
      Data32 &= ~(B_ESPI_DEVICE_BME);
      Status  = PchEspiDeviceSetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, Data32);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      DeviceBmeSet = TRUE;
    }
  }

  // Disable channel
  Status = PchEspiDeviceGetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, &Data32);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Data32 &= ~(B_ESPI_DEVICE_CHACAP_CHEN);
  Status  = PchEspiDeviceSetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, Data32);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  // Enable channel
  Status = PchEspiDeviceGetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, &Data32);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Data32 |= B_ESPI_DEVICE_CHACAP_CHEN;
  Status  = PchEspiDeviceSetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, Data32);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Waiting for Channel Ready bit\n"));
  // Wait until channel is ready by polling Channel Ready bit
  while (((Data32 & B_ESPI_DEVICE_CHACAP_CHRDY) == 0) && (Timeout > 0)) {
    Status = PchEspiDeviceGetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, &Data32);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    MicroSecondDelay (1);
    --Timeout;
  }

  if (Timeout == 0) {
    // The waiting for channel to be ready has timed out
    DEBUG ((DEBUG_WARN, "The operation of channel %d reset for device %d has timed out!\n", ChannelNumber, DeviceId));
    return EFI_TIMEOUT;
  }

  if ((ChannelNumber == 0) && DeviceBmeSet) {
    Status = PchEspiDeviceGetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, &Data32);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Data32 |= B_ESPI_DEVICE_BME;
    Status  = PchEspiDeviceSetConfig (DeviceId, DeviceChannelAddress, EspiPcrAccess, Data32);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  Disable EISS (Enable InSMM.STS)
**/
VOID
EspiDisableEiss (
  VOID
  )
{
  UINT64  EspiPciBaseAddr;

  EspiPciBaseAddr = EspiPciCfgBase ();

  ASSERT ((PciSegmentRead8 (EspiPciBaseAddr + R_ESPI_CFG_ESPI_BC) & B_ESPI_CFG_ESPI_BC_LE) == 0);

  PciSegmentAnd8 (EspiPciBaseAddr + R_ESPI_CFG_ESPI_BC, (UINT8) ~(B_ESPI_CFG_ESPI_BC_EISS));
}

/**
  Returns whether eSPI peripheral channel is available and ready on device $0

  @param[in]  EspiPcrAccess     eSPI Sideband registers access protocol

  @retval     TRUE              eSPI Peripheral channel for device #0 is
                                enabled and ready
  @retval     FALSE             eSPI Peripheral channel for device #0 is either
                                enabled or not ready
**/
BOOLEAN
IsEspiCs0PeripheralChannelAvailable(
  IN  REGISTER_ACCESS   *EspiPcrAccess
  )
{
  EFI_STATUS    Status;
  UINT32        Data32;
  UINT32        ValuesMask;

  Status = PchEspiDeviceGetConfig (0, R_ESPI_DEVICE_CHACAP_BASE, EspiPcrAccess, &Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR, "[%a] Error while reading Channel 0 and Capabilities register. Status: %r",
      __FUNCTION__, Status
      ));
    return FALSE;
  }

  ValuesMask = (B_ESPI_DEVICE_CHACAP_CHRDY | B_ESPI_DEVICE_CHACAP_CHEN);
  if ((Data32 & ValuesMask) == ValuesMask) {
    return TRUE;
  }
  return FALSE;
}