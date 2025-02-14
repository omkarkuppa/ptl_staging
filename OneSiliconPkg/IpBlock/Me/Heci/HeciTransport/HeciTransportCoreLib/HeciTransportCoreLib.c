/** @file
  HECI Transport protocol driver core operations for use in PEI, DXE and SMM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <PiPei.h>
#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HeciUtilsLib.h>
#include <Library/HeciTraceLib.h>
#include <Library/HeciAccessCoreLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/HeciRegs.h>
#include "HeciTransportCore.h"
#include <Protocol/HeciAccessProtocol.h>
#include <Library/PreSiliconEnvDetectLib.h>

#define HECI_TRANSPORT_NUM_DEBUG   "[HECI Transport (%d)]"

/**
  Function gets initialization state flag

  Initialization state flag should be set to true after interface
  is successfully initialized

  @param[in]  This    Pointer to HECI Transport device context structure

  @retval TRUE        Heci interface is initialized
  @retval FALSE       Heci interface is not initialized
**/
BOOLEAN
EFIAPI
GetInitializationState (
  IN HECI_TRANSPORT_PRIVATE   *This
  )
{
  if (This == NULL) {
    return FALSE;
  }

  return This->IsInitialized;
}

/**
  Get HECI Device Information Record

  @param[in]      This              Pointer to HECI Transport device context structure
  @param[in, out] HeciDeviceInfo    Pointer to HECI device information structure

  @retval EFI_SUCCESS               HeciDeviceInfo has been updated and keeps correct HECI device information
  @retval EFI_INVALID_PARAMETER     Wrong parameters passed
**/
EFI_STATUS
EFIAPI
GetHeciDeviceInfo (
  IN      HECI_TRANSPORT_PRIVATE     *This,
  IN OUT  HECI_DEVICE_INFO           *HeciDeviceInfo
  )
{
  if (This == NULL || HeciDeviceInfo == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciDeviceInfo->SecurityEngine = This->SecurityEngine;
  HeciDeviceInfo->HeciSegment    = This->PciSegment;
  HeciDeviceInfo->HeciBus        = This->PciBus;
  HeciDeviceInfo->HeciDevice     = This->PciDevice;
  HeciDeviceInfo->HeciFunction   = This->PciFunction;

  return EFI_SUCCESS;
}

/**
  Return Host Firmware status
  Corresponds to HECI HPS (part of) section 4.2.1

  @param[in]      HeciTransport     HECI Transport pointer through which the message will be sent
  @param[in,out]  FirmwareStatus    Pointer to returned firmware status

  @retval EFI_SUCCESS               Returned valid firmware status
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid
  @retval EFI_DEVICE_ERROR          Error occurred
**/
EFI_STATUS
EFIAPI
GetHeciFirmwareStatus (
  IN      HECI_TRANSPORT_PRIVATE    *HeciTransport,
  IN OUT  UINT32                    *FirmwareStatus
  )
{
  EFI_STATUS    Status;
  HECI_ACCESS   *HeciAccess;
  UINTN         HeciNumber;

  if (FirmwareStatus == NULL || HeciTransport == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *FirmwareStatus = MAX_UINT32;

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return EFI_DEVICE_ERROR;
  }

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_INFO, " %a ()\n", __FUNCTION__));

  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   R_ME_CFG_HFS,
                                   1,
                                   FirmwareStatus
                                   );

  return Status;
}

/**
  Return number of filled slots in HECI circular buffer.
  Corresponds to HECI HPS (part of) section 4.2.1

  @param[in]  Offset      Offset to Host/Me Control Status Register

  @retval FilledSlots     Number of filled slots in circular buffer
  @retval MAX_UINT32      Circular buffer overflow has occurred
**/
UINT32
HeciTransportGetFilledSlots (
  IN UINT32 ControlStatusRegister
  )
{
  UINT8   FilledSlots;

  //
  // Calculation documented in HECI HPS 0.68 section 4.2.1
  //
  FilledSlots = (INT8) GET_CB_WRITE_POINTER (ControlStatusRegister);
  FilledSlots -= (INT8) GET_CB_READ_POINTER (ControlStatusRegister);

  return (FilledSlots > (UINT8) GET_CB_DEPTH (ControlStatusRegister) ? MAX_UINT32 : FilledSlots);
}

/**
  Checks if HECI Interrupt Delivery Mode (HIDM) register is locked
  (This means HECI is initialized)

  @param[in]  HeciAccess    HECI Access pointer through which the message will be sent

  @retval TRUE              HIDM register is locked
  @retval FALSE             HIDM register is not locked or wrong parameter has been passed
**/
BOOLEAN
IsHidmRegisterLocked (
  IN  HECI_ACCESS   *HeciAccess
  )
{
  EFI_STATUS    Status;
  UINT8         Hidm;

  if (HeciAccess == NULL) {
    return FALSE;
  }

  Hidm = MAX_UINT8;
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint8,
                                   R_ME_CFG_HIDM,
                                   1,
                                   &Hidm
                                   );
  if (EFI_ERROR (Status) || (Hidm == MAX_UINT8)) {
    return FALSE;
  }

  return (BOOLEAN) (Hidm & B_ME_CFG_HIDM_L);
}

/**
  Check if HECI timeouts expired
  Add one timeout tick.

  @retval TRUE             Timeout is expired
  @retval FALSE            Timeout is not expired
**/
BOOLEAN
HeciTransportIsHeciTimeout (
  IN  UINT32    *Timeout
  )
{
  if ((Timeout == NULL) || (*Timeout == 0)) {
    return TRUE;
  }

  if (*Timeout < HECI_TIMEOUT_TICK) {
    *Timeout = 0;
  } else {
    *Timeout -= HECI_TIMEOUT_TICK;
  }

  MicroSecondDelay (HECI_TIMEOUT_TICK);

  return FALSE;
}

/**
  Checks if ME is ready for communication over the HECI interface.

  @param[in]  HeciTransport   HECI Transport pointer through which the message will be sent
  @param[in]  Timeout         Timeout value

  @retval TRUE                ME is ready
  @retval FALSE               ME is not ready
**/
BOOLEAN
HeciTransportIsMeReady (
  IN  HECI_TRANSPORT_PRIVATE    *HeciTransport,
  IN  UINT32                    *Timeout
  )
{
  EFI_STATUS    Status;
  HECI_ACCESS   *HeciAccess;
  UINT32        ControlStatusRegister;
  UINT16        MeVendorId;
  UINTN         HeciNumber;

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return FALSE;
  }

  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    return FALSE;
  }

  do {
    ///
    /// Check for reset first and then for ME Ready
    ///
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_CSE_CSR_HA,
                                     1,
                                     &ControlStatusRegister
                                     );
    if (EFI_ERROR (Status)) {
      return FALSE;
    }

    if (GET_RESET (ControlStatusRegister) == 1) {
      return FALSE;
    }
    if (GET_READY (ControlStatusRegister) == 1) {
      return TRUE;
    }
  } while (!HeciTransportIsHeciTimeout (Timeout));

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_ERROR, " ERROR: Timeout while waiting for ME_RDY bit\n"));

  return FALSE;
}

/**
  Public functions.
**/

/**
  Reset HECI queue.

  This function triggers HECI queue reset. If timeout is greater than zero it
  also waits until ME is ready to receive messages.

  @param[in]      HeciTransport   HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left

  @retval EFI_SUCCESS             Reset HECI queue succeeded
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_TIMEOUT             Timeout occurs during operation
**/
EFI_STATUS
EFIAPI
HeciReset (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout
  )
{
  EFI_STATUS         Status;
  HECI_ACCESS        *HeciAccess;
  UINT32             ResetTimeout;
  UINT32             ControlStatusRegister;
  UINT32             MeControlStatusRegister;
  UINT16             MeVendorId;
  UINTN              HeciNumber;

  if (HeciTransport == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return EFI_DEVICE_ERROR;
  }

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_INFO, " %a ()\n", __FUNCTION__));

  if (!IsHidmRegisterLocked (HeciAccess)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: HECI device not initialized\n"));
    return EFI_DEVICE_ERROR;
  }

  //
  // Check for HECI availability on PCI
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: Reset failed (Device Disabled)\n"));
    return EFI_DEVICE_ERROR;
  }

  ResetTimeout = HeciTransport->ResetTimeout;
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_H_CSR,
                                   1,
                                   &ControlStatusRegister
                                   );
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_CSE_CSR_HA,
                                   1,
                                   &MeControlStatusRegister
                                   );

  DEBUG ((DEBUG_WARN, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_WARN, " WARNING: Resetting HECI interface (CSR: 0x%08x, CSR_HA: 0x%08x)\n", ControlStatusRegister, MeControlStatusRegister));

  if (GET_RESET (ControlStatusRegister) == 0) {
    SET_RESET (ControlStatusRegister, 1);
    SET_INT_GENERATE (ControlStatusRegister, 1);
    Status = HeciAccess->PciIo.Mem.Write (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
  }

  //
  // Wait for H_RDY cleared to make sure that the reset started.
  //
  while (TRUE) {
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
    if (GET_READY (ControlStatusRegister) == 0) {
      break;
    }
    if (HeciTransportIsHeciTimeout (&ResetTimeout)) {
      Status = HeciAccess->PciIo.Mem.Read (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_ME_MEM_CSE_CSR_HA,
                                       1,
                                       &MeControlStatusRegister
                                       );

      DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
      DEBUG ((DEBUG_ERROR, " ERROR: Reset failed (Timeout) (CSR: 0x%08x, CSR_HA: 0x%08x)\n", ControlStatusRegister, MeControlStatusRegister));

      Status = EFI_TIMEOUT;
      break;
    }
  }
  if (!EFI_ERROR (Status)) {
    //
    // Wait for ME to perform reset and signal it is ready.
    //
    while (TRUE) {
      Status = HeciAccess->PciIo.Mem.Read (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_ME_MEM_CSE_CSR_HA,
                                       1,
                                       &MeControlStatusRegister
                                       );
      if (GET_READY (MeControlStatusRegister)) {
        break;
      }
      if (HeciTransportIsHeciTimeout (&ResetTimeout)) {
        Status = HeciAccess->PciIo.Mem.Read (
                                         &(HeciAccess->PciIo),
                                         EfiPciIoWidthUint32,
                                         0,
                                         R_ME_MEM_H_CSR,
                                         1,
                                         &ControlStatusRegister
                                         );

        DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
        DEBUG ((DEBUG_ERROR, " ERROR: Reset failed (Timeout) (CSR: 0x%08x, CSR_HA: 0x%08x)\n", ControlStatusRegister, MeControlStatusRegister));

        Status = EFI_TIMEOUT;
        break;
      }
    }
    if (!EFI_ERROR (Status)) {
      //
      // ME side is ready, signal host side is ready too.
      //
      Status = HeciAccess->PciIo.Mem.Read (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_ME_MEM_H_CSR,
                                       1,
                                       &ControlStatusRegister
                                       );
      SET_RESET (ControlStatusRegister, 0);
      SET_READY (ControlStatusRegister, 1);
      SET_INT_GENERATE (ControlStatusRegister, 1);
      Status = HeciAccess->PciIo.Mem.Write (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_ME_MEM_H_CSR,
                                       1,
                                       &ControlStatusRegister
                                       );
    }
  }

  if (Timeout != NULL) {
    ResetTimeout = HeciTransport->ResetTimeout - ResetTimeout;
    if (*Timeout > ResetTimeout) {
      *Timeout -= ResetTimeout;
    } else {
      *Timeout = 0;
    }
  }

  return Status;
}

/**
  Initialize HECI interface.

  This function initializes host side of HECI interface. If timeout is
  greater than zero it also waits until ME is ready to receive messages.

  @param[in]      HeciTransport   HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left

  @retval EFI_SUCCESS             Initialization succeeded
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_DEVICE_ERROR        Device not present or HECI MBar can't be initialized
**/
EFI_STATUS
EFIAPI
HeciInit (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout
  )
{
  EFI_STATUS    Status;
  HECI_ACCESS   *HeciAccess;
  UINT32        InitTimeout;
  UINT32        ControlStatusRegister;
  UINT32        MeControlStatusRegister;
  UINT16        MeVendorId;
  UINT16        HeciDeviceId;
  UINTN         InterruptMode;
  UINTN         HeciNumber;
  UINTN         HeciSegment;
  UINTN         HeciBus;
  UINTN         HeciDevice;
  UINTN         HeciFunction;

  if (HeciTransport == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_INFO, " %a () Start\n", __FUNCTION__));

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return EFI_DEVICE_ERROR;
  }

  if (Timeout != NULL) {
    InitTimeout = *Timeout;
  } else {
    InitTimeout = 0;
  }

  HeciAccess->PciIo.GetLocation (
                      &(HeciAccess->PciIo),
                      &HeciSegment,
                      &HeciBus,
                      &HeciDevice,
                      &HeciFunction
                      );

  HeciAccess->PciIo.Pci.Read (
                          &(HeciAccess->PciIo),
                          EfiPciIoWidthUint16,
                          PCI_VENDOR_ID_OFFSET,
                          1,
                          &MeVendorId
                          );
  HeciAccess->PciIo.Pci.Read (
                          &(HeciAccess->PciIo),
                          EfiPciIoWidthUint16,
                          PCI_DEVICE_ID_OFFSET,
                          1,
                          &HeciDeviceId
                          );

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((
    DEBUG_INFO,
    " HECI SBDF: [%d:%d:%d:%d] VendorId: 0x%02x, DeviceId: 0x%02x\n",
    HeciSegment,
    HeciBus,
    HeciDevice,
    HeciFunction,
    MeVendorId,
    HeciDeviceId
    ));

  if (MeVendorId == 0xFFFF) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: Init failed (Device Disabled)\n"));
    Status = EFI_DEVICE_ERROR;
  } else {
    HeciAccess->PciIo.Pci.Read (
                            &(HeciAccess->PciIo),
                            EfiPciIoWidthUint8,
                            R_ME_CFG_HIDM,
                            1,
                            &InterruptMode
                            );
    if (InterruptMode & B_ME_CFG_HIDM_L) {
      DEBUG ((DEBUG_INFO, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
      DEBUG ((DEBUG_INFO, " HECI already initialized, skipping\n"));
      HeciTransport->IsInitialized = TRUE;
      return EFI_SUCCESS;
    }

    //
    // Make sure HECI interrupts are disabled before configuring delivery mode.
    //
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
    SET_INT_ENABLE (ControlStatusRegister, 0);
    Status = HeciAccess->PciIo.Mem.Write (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );

    //
    // Set HECI interrupt delivery mode and lock.
    //
    Status = HeciAccess->GetHeciInterruptMode (HeciAccess, &InterruptMode);
    InterruptMode |= B_ME_CFG_HIDM_L;
    HeciAccess->PciIo.Pci.Write (
                            &(HeciAccess->PciIo),
                            EfiPciIoWidthUint8,
                            R_ME_CFG_HIDM,
                            1,
                            &InterruptMode
                            );

    //
    // If HECI was in use reset it to clear queues.
    //
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_CSE_CSR_HA,
                                     1,
                                     &MeControlStatusRegister
                                     );
    if (!EFI_ERROR (Status) && (GET_READY (MeControlStatusRegister) == 1)) {
      if (GET_READY (ControlStatusRegister) == 0) {
        SET_INT_GENERATE (ControlStatusRegister, 1);
        SET_READY (ControlStatusRegister, 1);
        SET_RESET (ControlStatusRegister, 0);

        Status = HeciAccess->PciIo.Mem.Write (
                                         &(HeciAccess->PciIo),
                                         EfiPciIoWidthUint32,
                                         0,
                                         R_ME_MEM_H_CSR,
                                         1,
                                         &ControlStatusRegister
                                         );
      }
    } else {
      if (InitTimeout > 0) {
        Status = HeciReset (HeciTransport, &InitTimeout);
      }
    }
  }

  if (Timeout != NULL) {
    *Timeout = InitTimeout;
  }

  if (!EFI_ERROR (Status)) {
    HeciTransport->IsInitialized = TRUE;
  }

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_INFO, " %a () - End\n", __FUNCTION__));

  return Status;
}

/**
  Write one packet to HECI queue.

  This function puts one packet to HECI queue. If timeout is greater than
  zero the function may wait for space in the queue. This function handles
  only packets shorter than HECI queue length.
  Fragmentation of large messages into packets can be done using HeciSend().

  @param[in]      HeciTransport   HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left
  @param[in]      PacketHeader    Header of the packet to send
  @param[in]      PacketBody      Body of the packet to send of length specified in PacketHeader.Fields.Length

  @retval EFI_SUCCESS             Write one packet to HECI queue succeeded
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_TIMEOUT             Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE     Packet is bigger than queue length
**/
EFI_STATUS
EFIAPI
HeciPacketSend (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout,
  IN     HECI_PACKET_HEADER       PacketHeader,
  IN     UINT32                   *PacketBody
  )
{
  EFI_STATUS         Status;
  HECI_ACCESS        *HeciAccess;
  UINT32             Index;
  UINT32             LengthInDwords;
  UINT32             HeciCsrHost;
  UINT16             MeVendorId;
  UINTN              HeciNumber;

  if (PacketBody == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return EFI_DEVICE_ERROR;
  }

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_VERBOSE, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_VERBOSE, " %a ()\n", __FUNCTION__));

  HeciPktTrace (HeciTransport, "Send pkt: ", PacketHeader, (UINT8 *) PacketBody, PacketHeader.Fields.Length);

  //
  // Check for HECI availability on PCI
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: Send pkt failed (Device Disabled)\n"));
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Compute message length in DWORDS
  ///
  LengthInDwords = ((PacketHeader.Fields.Length + 3) / 4);

  ///
  /// Wait until there is sufficient room in the circular buffer
  /// Must have room for message and message header
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_H_CSR,
                                   1,
                                   &HeciCsrHost
                                   );

  while ((LengthInDwords + 1) > (GET_CB_DEPTH (HeciCsrHost) - HeciTransportGetFilledSlots (HeciCsrHost))) {
    if (HeciTransportIsHeciTimeout (Timeout)) {
      DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
      DEBUG ((DEBUG_ERROR, " ERROR: Timeout while waiting for enough space in the CB"));
      return EFI_TIMEOUT;
    }

    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_H_CSR,
                                     1,
                                     &HeciCsrHost
                                     );
  }

  ///
  /// Write Message Header
  ///
  Status = HeciAccess->PciIo.Mem.Write (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_H_CB_WW,
                                   1,
                                   &PacketHeader.Data
                                   );

  ///
  /// Write Message Body
  ///
  for (Index = 0; Index < LengthInDwords; Index++) {
    Status = HeciAccess->PciIo.Mem.Write (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_H_CB_WW,
                                     1,
                                     &PacketBody[Index]
                                     );
  }
  ///
  /// Set Interrupt Generate bit
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_H_CSR,
                                   1,
                                   &HeciCsrHost
                                   );
  SET_INT_GENERATE (HeciCsrHost, 1);
  Status = HeciAccess->PciIo.Mem.Write (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_H_CSR,
                                   1,
                                   &HeciCsrHost
                                   );

  ///
  /// Test if ME Ready bit is set to 1.
  /// If set to 0 a fatal error occurred during the transmission of this message.
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_CSE_CSR_HA,
                                   1,
                                   &HeciCsrHost
                                   );
  if (!EFI_ERROR (Status) && (GET_READY (HeciCsrHost) == 0)) {
    return EFI_NOT_READY;
  }

  return EFI_SUCCESS;
}

/**
  Read one packet from HECI queue.

  This function reads one packet from HECI queue. If timeout is greater than
  zero the function may wait for the packet. Size of packet buffer is given
  in bytes in (*PacketLen) on input. On exit (*PacketLen) provides the number
  of bytes written to the packet body buffer. If buffer is too short the packet
  is truncated. This function does not reassemble fragmented HECI messages.
  To receive whole message reassembled use HeciReceive().

  @param[in]      HeciTransport   HECI Transport pointer from which the message should be received
  @param[in,out]  Timeout         On input timeout in us, on exit time left
  @param[out]     PacketHeader    Buffer for the header of received packet
  @param[out]     PacketBody      Buffer for the body of received packet
  @param[in,out]  PacketLen       On input buffer size, on exit packet body size in bytes

  @retval EFI_SUCCESS             Read one packet from HECI queue succeeded
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_TIMEOUT             Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE     Message exceeds buffer size
**/
EFI_STATUS
HeciPacketReceive (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout,
     OUT HECI_PACKET_HEADER       *PacketHeader,
     OUT UINT32                   *PacketBody,
  IN OUT UINT32                   *PacketLen
  )
{
  EFI_STATUS    Status;
  HECI_ACCESS   *HeciAccess;
  UINT32        FilledSlots;
  UINT32        Index;
  UINT32        LengthInDwords;
  UINT32        TempBuffer;
  UINT32        BufferLength;
  UINT32        BufferCopySize;
  UINT32        ControlStatusRegister;
  UINT16        MeVendorId;
  UINTN         HeciNumber;

  if (PacketHeader == NULL || PacketBody == NULL || PacketLen == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return EFI_DEVICE_ERROR;
  }

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_VERBOSE, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_VERBOSE, " %a ()\n", __FUNCTION__));

  //
  // Check for HECI availability on PCI
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: Receive failed (Device Disabled)\n"));
    return EFI_DEVICE_ERROR;
  }

  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_CSE_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );

  FilledSlots = HeciTransportGetFilledSlots (ControlStatusRegister);

  ///
  /// For BLOCKING read, wait until data appears in the CB or timeout occurs
  ///
  if ((Timeout != NULL) && (*Timeout != 0)) {
    while (FilledSlots == 0) {
      if (HeciTransportIsHeciTimeout (Timeout)) {
        return EFI_TIMEOUT;
      }
      Status = HeciAccess->PciIo.Mem.Read (
                                       &(HeciAccess->PciIo),
                                       EfiPciIoWidthUint32,
                                       0,
                                       R_ME_MEM_CSE_CSR_HA,
                                       1,
                                       &ControlStatusRegister
                                       );
      FilledSlots = HeciTransportGetFilledSlots (ControlStatusRegister);
    }
  }

  BufferLength = *PacketLen;
  *PacketLen   = 0;

  ///
  /// Check for empty and overflowed CB
  ///
  if (FilledSlots == 0) {
    return EFI_NO_RESPONSE;
  } else if (FilledSlots == MAX_UINT32) {  // Check for HECI Circular buffer overflow
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Read the HECI Message header
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_CSE_CB_RW,
                                   1,
                                   &PacketHeader->Data
                                   );

  ///
  /// Compute required message length in DWORDS
  ///
  LengthInDwords = ((PacketHeader->Fields.Length + 3) / 4);

  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_CSE_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );

  if (LengthInDwords > HeciTransportGetFilledSlots (ControlStatusRegister)) {
    ///
    /// Make sure that the message does not overflow the circular buffer.
    ///
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Wait until whole message appears in circular buffer.
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_CSE_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );

  while (HeciTransportGetFilledSlots (ControlStatusRegister) < LengthInDwords) {
    if (HeciTransportIsHeciTimeout (Timeout)) {
      return EFI_TIMEOUT;
    }

    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_CSE_CSR_HA,
                                     1,
                                     &ControlStatusRegister
                                     );
  }

  ///
  /// Check if buffer can hold the message
  ///
  if ((PacketHeader->Fields.Length) <= BufferLength) {
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_BUFFER_TOO_SMALL;
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: Receive buffer is too small (Required: %d, Current: %d)\n", PacketHeader->Fields.Length, BufferLength));
    return EFI_BUFFER_TOO_SMALL;
  }

  ///
  /// Copy as much bytes as there is space left in the message buffer.
  /// Excessive bytes will be dismissed.
  ///
  for (Index = 0; Index < LengthInDwords; Index++) {
    Status = HeciAccess->PciIo.Mem.Read (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_CSE_CB_RW,
                                     1,
                                     &TempBuffer
                                     );

    BufferCopySize = MIN (sizeof (TempBuffer), BufferLength);
    CopyMem (&PacketBody[Index], &TempBuffer, BufferCopySize);

    BufferLength -= BufferCopySize;
    *PacketLen += BufferCopySize;
  }

  *PacketLen = PacketHeader->Fields.Length;

  ///
  /// Read R_ME_MEM_CSE_CSR_HA.
  /// If the ME_RDY bit is 0, then an ME reset occurred during the transaction and the message
  /// should be discarded as bad data may have been retrieved from the host's circular buffer
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_CSE_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );
  if (GET_READY (ControlStatusRegister) == 0) {
    *PacketLen = 0;
    return EFI_NOT_READY;
  }

  ///
  /// Set Interrupt Generate bit and clear Interrupt Status bit if there are no more messages in the buffer.
  ///
  if (HeciTransportGetFilledSlots (ControlStatusRegister) == 0) {
    SET_INT_GENERATE (ControlStatusRegister, 1);
    SET_INT_STATUS (ControlStatusRegister, 1);
    Status = HeciAccess->PciIo.Mem.Write (
                                     &(HeciAccess->PciIo),
                                     EfiPciIoWidthUint32,
                                     0,
                                     R_ME_MEM_H_CSR,
                                     1,
                                     &ControlStatusRegister
                                     );
  }

  HeciPktTrace (HeciTransport, "Got pkt: ", *PacketHeader, (UINT8 *) PacketBody, *PacketLen);

  return Status;
}

/**
  Write a message to HECI queue, if necessary perform fragmentation.

  This function puts one message to HECI queue. If the message is longer than
  HECI queue size and timeout is greater than zero the function will split
  the message into several packets and wait for space in the queue to send all
  packets/fragments. Each packet has HECI packet header added.

  @param[in]      HeciTransport   HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left
  @param[in]      HostAddress     Host address
  @param[in]      EngineAddress   Engine address
  @param[in]      MessageBody     Body of the message to send
  @param[in]      MessageLength   Length of the message to send in bytes

  @retval EFI_SUCCESS             Message has been sent successfully
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_TIMEOUT             Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE     Packet is bigger than queue length
**/
EFI_STATUS
EFIAPI
HeciTransportSend (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout,
  IN     UINT8                    HostAddress,
  IN     UINT8                    EngineAddress,
  IN     UINT32                   *MessageBody,
  IN     UINT32                   MessageLength
  )
{
  EFI_STATUS            Status;
  HECI_PACKET_HEADER    PacketHeader;
  HECI_ACCESS           *HeciAccess;
  UINT32                CircularBufferDepth;
  UINT32                SendLength;
  UINT32                BytesLeft;
  UINT32                SendTimeout;
  UINT32                ControlStatusRegister;
  UINT16                MeVendorId;
  UINTN                 HeciNumber;

  if (MessageBody == NULL || HeciTransport == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return EFI_DEVICE_ERROR;
  }

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_VERBOSE, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_VERBOSE, " %a ()\n", __FUNCTION__));

  if (Timeout == NULL) {
    SendTimeout = HeciTransport->SendTimeout;
    Timeout = &SendTimeout;
  }

  if (!IsHidmRegisterLocked (HeciAccess)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: HECI device not initialized!\n"));
    return EFI_DEVICE_ERROR;
  }

  //
  // Check for HECI availability on PCI
  //
  Status = HeciAccess->PciIo.Pci.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint16,
                                   PCI_VENDOR_ID_OFFSET,
                                   1,
                                   &MeVendorId
                                   );
  if (EFI_ERROR (Status) || (MeVendorId == 0xFFFF)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: Send pkt failed (Device Disabled)\n"));
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Make sure that HECI is ready for communication
  ///
  if (!HeciTransportIsMeReady (HeciTransport, Timeout)) {
    ///
    /// Reset HECI if not ready
    ///
    Status = HeciReset (HeciTransport, Timeout);
    if (EFI_ERROR (Status)) {
      return EFI_NOT_READY;
    }
  }

  ///
  /// Grab Circular Buffer length and convert it from Dword to bytes
  ///
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_H_CSR,
                                   1,
                                   &ControlStatusRegister
                                   );
  CircularBufferDepth = 4 * GET_CB_DEPTH (ControlStatusRegister);

  ///
  /// Prepare message header
  ///
  PacketHeader.Data                 = 0;
  PacketHeader.Fields.EngineAddress = EngineAddress;
  PacketHeader.Fields.HostAddress   = HostAddress;

  BytesLeft = MessageLength;
  ///
  /// Break message up into CB-sized packets and loop until completely sent
  ///
  while (BytesLeft) {
    ///
    /// Set the MessageBody Complete bit if this is our last packet in the message.
    /// Needs to be less or equal to CB depth minus one Dword for HECI header.
    ///
    if (BytesLeft <= CircularBufferDepth - sizeof (PacketHeader)) {
      PacketHeader.Fields.MessageComplete = 1;
    }
    ///
    /// Calculate length for MessageBody Header:
    /// It will be the smaller value from circular buffer depth and remaining message length.
    ///
    SendLength = MIN (CircularBufferDepth - sizeof (PacketHeader), BytesLeft);
    PacketHeader.Fields.Length = SendLength;
    ///
    /// Send the current packet
    ///
    Status = HeciPacketSend (HeciTransport, Timeout, PacketHeader, (UINT32 *) ((UINTN) MessageBody + (MessageLength - BytesLeft)));
    if (EFI_ERROR (Status)) {
      Status = HeciReset (HeciTransport, Timeout);
      break;
    }
    ///
    /// Update the length information
    ///
    BytesLeft -= SendLength;
  }

  return Status;
}

/**
  Read a message from HECI queue, if necessary perform reassembly.

  This function reads one message from HECI queue. If the message is
  fragmented and timeout is greater than zero the function will wait for
  subsequent packets (fragments) until full message is completed.

  @param[in]      HeciTransport   HECI Transport pointer from which the message should be received
  @param[in,out]  Timeout         On input timeout in us, on exit time left
  @param[out]     HostAddress     Host address
  @param[out]     EngineAddress   Engine address
  @param[out]     MessageBody     Body of the message to receive
  @param[in,out]  MessageLength   On input buffer size, on exit message body size in bytes

  @retval EFI_SUCCESS             Message has been received successfully
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_TIMEOUT             Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE     Message exceeds buffer size
**/
EFI_STATUS
EFIAPI
HeciTransportReceive (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout,
     OUT UINT8                    *HostAddress,
     OUT UINT8                    *EngineAddress,
     OUT UINT32                   *MessageBody,
  IN OUT UINT32                   *MessageLength
  )
{
  EFI_STATUS            Status;
  HECI_PACKET_HEADER    PacketHeader;
  HECI_ACCESS           *HeciAccess;
  UINT32                PacketLen;
  UINT32                *PacketBody;
  UINT32                ReceiveTimeout;
  UINT32                TotalLength;
  UINTN                 HeciNumber;
  UINT8                 EAddr;
  UINT8                 HAddr;

  if (MessageBody == NULL || MessageLength == NULL || HeciTransport == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    return EFI_DEVICE_ERROR;
  }

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_VERBOSE, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_VERBOSE, " %a ()\n", __FUNCTION__));

  if (Timeout == NULL) {
    ReceiveTimeout = HeciTransport->ReceiveTimeout;
    Timeout = &ReceiveTimeout;
  }

  if (!IsHidmRegisterLocked (HeciAccess)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: HECI device not initialized!\n"));
    return EFI_DEVICE_ERROR;
  }

  PacketHeader.Data = 0;
  TotalLength       = 0;
  HAddr             = 0;
  EAddr             = 0;
  PacketBody        = MessageBody;

  if (!HeciTransportIsMeReady (HeciTransport, Timeout)) {
    //
    // Return as CB will be empty after reset and CSME will not put any data
    //
    HeciReset (HeciTransport, Timeout);
    return EFI_NOT_READY;
  }

  //
  // Read until MessageComplete bit is set
  //
  do {
    if (*MessageLength > TotalLength) {
      PacketLen = *MessageLength - TotalLength;
    } else {
      ///
      /// Buffer is too small to receive data
      /// Reset HECI to clear data in buffer and exit with error
      ///
      HeciReset (HeciTransport, Timeout);
      Status = EFI_BUFFER_TOO_SMALL;
      break;
    }

    Status = HeciPacketReceive (HeciTransport, Timeout, &PacketHeader, PacketBody, &PacketLen);

    ///
    /// If timeout occurred we need to reset the interface to clear the data that could possibly come later.
    /// Also buffer overflow and transaction errors will require a reset.
    /// We need to continue read even if buffer too small to clear the data and signal the buffer size.
    ///
    if (EFI_ERROR (Status) && Status != EFI_BUFFER_TOO_SMALL) {
      if (Status != EFI_NO_RESPONSE) {
        HeciReset (HeciTransport, Timeout);
      }
      *MessageLength = TotalLength;
      break;
    }

    if (PacketBody == MessageBody) { // If it is first fragment/packet
      HAddr = (UINT8) PacketHeader.Fields.HostAddress;
      EAddr = (UINT8) PacketHeader.Fields.EngineAddress;
    } else if (PacketHeader.Fields.HostAddress != HAddr || PacketHeader.Fields.EngineAddress != EAddr) {
      DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
      DEBUG ((DEBUG_ERROR, " ERROR: Got packet %X->%X while waiting for %X->%X, dropping\n", PacketHeader.Fields.EngineAddress, PacketHeader.Fields.HostAddress, EAddr, HAddr));
      PacketLen = 0;
    }

    ///
    /// Track the MessageLength of what we have read so far
    ///
    TotalLength += PacketLen;
    PacketBody = (UINT32 *) ((UINT8 *) PacketBody + PacketLen);
  } while (PacketHeader.Fields.MessageComplete == 0);

  if (!EFI_ERROR (Status)) {
    *MessageLength = TotalLength;

    if (HostAddress != NULL) {
      *HostAddress = HAddr;
    }
    if (EngineAddress != NULL) {
      *EngineAddress = EAddr;
    }
  }

  return Status;
}

/**
  Send request message to HECI queue, wait for response if needed.

  This function writes one message to HECI queue and - if receive buffer
  was provided and timeout is greater than zero - waits for response message.
  The request can be retried if Retries > 0 (0 means one attempt, no retry),
  but if retries are requested, the ReqMsg and RspMsg pointers must point
  to different buffers. This is required to make sure request is unchanged.
  Size of receive message buffer is given in bytes in (*RspLen) on input.
  On exit (*RspLen) provides the number of bytes written to the message
  buffer. If buffer is too short the message is truncated.

  @param[in]      HeciTransport   HECI Transport pointer through which the message will be sent
  @param[in,out]  Timeout         On input timeout in us, on exit time left
  @param[in]      Retries         The number of retries requested (0 => no retry)
  @param[in]      HostAddress     Host address
  @param[in]      EngineAddress   Engine address
  @param[in]      ReqMsg          Request message
  @param[in]      ReqLen          Request message length in bytes
  @param[out]     RspMsg          Buffer for the response message
  @param[in,out]  RspLen          On input buffer size, on exit message, in bytes
  @param[in,out]  SendStatus      Send operation status
  @param[in,out]  ReceiveStatus   Receive operation status

  @retval EFI_SUCCESS             Message request has been completed successfully
  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_DEVICE_ERROR        HECI MBar can't be initialized
  @retval EFI_UNSUPPORTED         Device not supported for this operation
  @retval EFI_TIMEOUT             Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE     Message is too big
**/
EFI_STATUS
EFIAPI
HeciSendAndReceive (
  IN     HECI_TRANSPORT_PRIVATE   *HeciTransport,
  IN OUT UINT32                   *Timeout,
  IN     UINT8                    Retries,
  IN     UINT8                    HostAddress,
  IN     UINT8                    EngineAddress,
  IN     UINT32                   *ReqMsg,
  IN     UINT32                   ReqLen,
     OUT UINT32                   *RspMsg,
  IN     UINT32                   *RspLen,
  IN OUT EFI_STATUS               *SendStatus,
  IN OUT EFI_STATUS               *ReceiveStatus
  )
{
  EFI_STATUS    Status;
  HECI_ACCESS   *HeciAccess;
  UINT32        RspLenTemp;
  UINT32        SingleRequestTimeout;
  UINT32        ControlStatusRegister;
  UINTN         HeciNumber;
  UINT8         ReceiveHostAddress;
  UINT8         ReceiveEngineAddress;

  if (ReqMsg == NULL || (RspMsg != NULL && RspLen == NULL) || ReqMsg == RspMsg || HeciTransport == NULL || Retries == 0) {
    if (SendStatus != NULL) {
      *SendStatus = EFI_INVALID_PARAMETER;
    }
    if (ReceiveStatus != NULL) {
      *ReceiveStatus = EFI_INVALID_PARAMETER;
    }

    return EFI_INVALID_PARAMETER;
  }

  HeciAccess = GetHeciAccess (HeciTransport);
  if (HeciAccess == NULL) {
    if (SendStatus != NULL) {
      *SendStatus = EFI_DEVICE_ERROR;
    }
    if (ReceiveStatus != NULL) {
      *ReceiveStatus = EFI_DEVICE_ERROR;
    }

    return EFI_DEVICE_ERROR;
  }

  GetHeciNumber (HeciTransport->PciFunction, &HeciNumber);

  DEBUG ((DEBUG_VERBOSE, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
  DEBUG ((DEBUG_VERBOSE, " %a ()\n", __FUNCTION__));

  //
  // If HECI buffer is not empty reset HECI to clean it up
  //
  Status = HeciAccess->PciIo.Mem.Read (
                                   &(HeciAccess->PciIo),
                                   EfiPciIoWidthUint32,
                                   0,
                                   R_ME_MEM_CSE_CSR_HA,
                                   1,
                                   &ControlStatusRegister
                                   );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_ERROR, " ERROR: Can't read HECI interface!\n"));
    return EFI_DEVICE_ERROR;
  }

  if (HeciTransportGetFilledSlots (ControlStatusRegister) > 0) {
    HeciReset (HeciTransport, Timeout);
  }

  if (Timeout != NULL) {
    SingleRequestTimeout = *Timeout;
  }

  RspLenTemp           = 0;
  ReceiveHostAddress   = HostAddress;
  ReceiveEngineAddress = EngineAddress;

  while (Retries > 0)  {
    //
    // Reload timeout if provided
    //
    if (Timeout != NULL) {
      *Timeout = SingleRequestTimeout;
    }

    //
    // Send the request and wait for response if response expected
    //
    Status = HeciTransportSend (HeciTransport, Timeout, HostAddress, EngineAddress, ReqMsg, ReqLen);
    if (SendStatus != NULL) {
      *SendStatus = Status;
    }

    while (!EFI_ERROR (Status) && RspMsg != NULL) {
      RspLenTemp = *RspLen;
      Status = HeciTransportReceive (HeciTransport, Timeout, &ReceiveHostAddress, &ReceiveEngineAddress, RspMsg, &RspLenTemp);
      if (ReceiveStatus != NULL) {
        *ReceiveStatus = Status;
      }

      if (!EFI_ERROR (Status)) {
        if (ReceiveHostAddress != HostAddress || ReceiveEngineAddress != EngineAddress) {
          DEBUG ((DEBUG_WARN, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
          DEBUG ((DEBUG_WARN, " WARNING: Got message %X->%X while waiting for %X->%X, skipping\n", ReceiveEngineAddress, ReceiveHostAddress, EngineAddress, HostAddress));
          Status = EFI_DEVICE_ERROR;
        } else {
          Retries = 0;
          break;
        }
      } else {
        if (Status == EFI_DEVICE_ERROR) {
          DEBUG ((DEBUG_ERROR, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
          DEBUG ((DEBUG_ERROR, " ERROR: HECI request failed because of device error, aborting\n"));
          Retries = 0;
        }
        break;
      }
    }

    if (Retries == 0) {
      break;
    }

    Retries--;
    DEBUG ((DEBUG_WARN, HECI_TRANSPORT_NUM_DEBUG, HeciNumber));
    DEBUG ((DEBUG_WARN, " WARNING: HECI request failed (%r), %d retries left\n", Status, Retries));
  }

  if (RspLen != NULL) {
    *RspLen = RspLenTemp;
  }

  return Status;
}

/**
  Initialize HECI_TRANSPORT protocol functions

  @param[out]  Heci  HECI transport pointer to be initialized
**/
VOID
SetHeciTransportFunctions (
  OUT HECI_TRANSPORT_PRIVATE *Heci
  )
{
  if (Heci == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - NULL pointer passed!\n", __FUNCTION__));
    return;
  }

  Heci->HeciTransport.GetInitializationState  = (HECI_TRANSPORT_GET_INIT_STATE) GetInitializationState;
  Heci->HeciTransport.GetHeciDeviceInfo       = (HECI_TRANSPORT_GET_HECI_DEVICE_INFO) GetHeciDeviceInfo;
  Heci->HeciTransport.GetHeciFirmwareStatus   = (HECI_TRANSPORT_GET_FIRMWARE_STATUS) GetHeciFirmwareStatus;

  Heci->HeciTransport.GetNextHeciTransport    = (HECI_TRANSPORT_GET_NEXT_HECI) GetNextHeciTransport;

  Heci->HeciTransport.HeciInit                = (HECI_TRANSPORT_INIT) HeciInit;
  Heci->HeciTransport.HeciReset               = (HECI_TRANSPORT_RESET) HeciReset;
  Heci->HeciTransport.HeciSend                = (HECI_TRANSPORT_SEND) HeciTransportSend;
  Heci->HeciTransport.HeciReceive             = (HECI_TRANSPORT_RECEIVE) HeciTransportReceive;
  Heci->HeciTransport.HeciSendAndReceive      = (HECI_TRANSPORT_SEND_AND_RECEIVE) HeciSendAndReceive;
}

/**
  Initialize HECI_TRANSPORT protocol instance

  @param[in]  HeciTransport       HECI transport pointer to be initialized
  @param[in]  HeciAccess          Associated HECI access instance

  @retval EFI_INVALID_PARAMETER   Input parameters are invalid
  @retval EFI_SUCCESS             Heci transport initialized
**/
EFI_STATUS
HeciInitializeNewTransportInstance (
  IN OUT HECI_TRANSPORT_PRIVATE   *Heci,
  IN     HECI_ACCESS              *HeciAccess
  )
{
  if (Heci == NULL || HeciAccess == NULL) {
    DEBUG ((DEBUG_ERROR, "%a () - NULL pointer passed!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  SetHeciTransportFunctions (Heci);

  //
  // Before MemoryDiscovered we should use HeciAccessIndex instead of direct pointer to HeciAccess
  //
  Heci->HeciAccess      = HeciAccess;
  Heci->HeciAccessIndex = MAX_INT32;
  Heci->IsInitialized   = FALSE;
  Heci->SendTimeout     = HECI_SEND_TIMEOUT;
  Heci->ReceiveTimeout  = HECI_READ_TIMEOUT;
  Heci->ResetTimeout    = HECI_RESET_TIMEOUT;
  if (IsHfpgaEnvironment ()) {
    Heci->ResetTimeout = MAX_UINT32;
  }
  Heci->SecurityEngine  = ((HECI_ACCESS_PRIVATE *) HeciAccess)->DeviceConfig.SecurityEngine;
  HeciAccess->PciIo.GetLocation (
                      &(HeciAccess->PciIo),
                      &(Heci->PciSegment),
                      &(Heci->PciBus),
                      &(Heci->PciDevice),
                      &(Heci->PciFunction)
                      );

  return EFI_SUCCESS;
}