/** @file
  CPU SBI access library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/CpuSbiAccessLib.h>
#include <Library/TimerLib.h>
#include <Library/CpuRegbarAccessLib.h>
#include <Register/IomRegs.h>
#include <Register/P2sbRegs.h>
#include <Register/CpuGenRegs.h>

/**
  Execute CPU SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the CpuSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal IOM and hide IOM if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the IOM before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
EFI_STATUS
EFIAPI
CpuSbiExecution (
  IN     CPU_SB_DEVICE_PID              Pid,
  IN     UINT64                         Offset,
  IN     CPU_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN     UINT16                         Fid,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  )
{
  //
  // Check address valid
  //
  if (((UINT32) Offset & 0x3) != 0) {
    //
    // Warning message for the address not DWORD alignment.
    //
    DEBUG ((DEBUG_INFO, "CpuSbiExecution: Address is not DWORD aligned.\n"));
  }

  return CpuSbiExecutionEx ( Pid,
           Offset,
           Opcode,
           Posted,
           0x000F,
           0x0000,
           Fid,
           Data32,
           Response
           );
}

/**
  Full function for executing CPU SBI message
  Take care of that there is no lock protection when using SBI programming in both POST time and SMI.
  It will clash with POST time SBI programming when SMI happen.
  Programmer MUST do the save and restore opration while using the CpuSbiExecution inside SMI
  to prevent from racing condition.
  This function will reveal SA and hide SA if it's originally hidden. If more than one SBI access
  needed, it's better to unhide the SA before calling and hide it back after done.

  When the return value is "EFI_SUCCESS", the "Response" do not need to be checked as it would have been
  SBI_SUCCESS. If the return value is "EFI_DEVICE_ERROR", then this would provide additional information
  when needed.

  @param[in] Pid                        Port ID of the SBI message
  @param[in] Offset                     Offset of the SBI message
  @param[in] Opcode                     Opcode
  @param[in] Posted                     Posted message
  @param[in] Fbe                        First byte enable
  @param[in] Bar                        Bar
  @param[in] Fid                        Function ID
  @param[in, out] Data32                Read/Write data
  @param[out] Response                  Response

  @retval EFI_SUCCESS                   Successfully completed.
  @retval EFI_DEVICE_ERROR              Transaction fail
  @retval EFI_INVALID_PARAMETER         Invalid parameter
  @retval EFI_TIMEOUT                   Timeout while waiting for response
**/
EFI_STATUS
EFIAPI
CpuSbiExecutionEx (
  IN     CPU_SB_DEVICE_PID              Pid,
  IN     UINT64                         Offset,
  IN     CPU_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN     UINT16                         Fbe,
  IN     UINT16                         Bar,
  IN     UINT16                         Fid,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  )
{
  UINTN                                 Timeout;
  UINT16                                SbiStat;
  BOOLEAN                               IomReady;
  //
  // Check opcode valid
  //
  switch (Opcode) {
    case MemoryRead:
    case MemoryWrite:
    case PciConfigRead:
    case PciConfigWrite:
    case PrivateControlRead:
    case PrivateControlWrite:
    case GpioLockUnlock:
      break;
    default:
      return EFI_INVALID_PARAMETER;
      break;
  }

  IomReady = ((CpuRegbarRead32 (SOC_DIE, CPU_SB_PID_IOM, 0, R_IOM_PCR_IOM_TYPEC_STATUS_1)) & B_IOM_PCR_IOM_TYPEC_STATUS_1_IOM_READY) ? TRUE : FALSE;

  if (!IomReady) {
    //
    // Try again after 1ms if failed
    //
    DEBUG ((DEBUG_INFO, "IOM is not ready. Trying again ..\n"));
    MicroSecondDelay (1000);
    IomReady = ((CpuRegbarRead32 (SOC_DIE, CPU_SB_PID_IOM, 0, R_IOM_PCR_IOM_TYPEC_STATUS_1)) & B_IOM_PCR_IOM_TYPEC_STATUS_1_IOM_READY) ? TRUE : FALSE;
  }

  DEBUG((DEBUG_INFO, "IOMReady bit = %d\n", (int)IomReady));

  if (!IomReady) {
    ASSERT (IomReady);
    return EFI_DEVICE_ERROR;
  }
  ///
  /// BWG Section 2.2.1
  /// 1. Poll IOM offset D8h[0] = 0b
  /// Make sure the previous opeartion is completed.
  ///
  Timeout = 0xFFFFFFF;
  while (Timeout > 0) {
    SbiStat = CpuRegbarRead16 (SOC_DIE, CPU_SB_PID_IOM, 0, R_P2SB_CFG_SBISTAT);
    if ((SbiStat & B_P2SB_CFG_SBISTAT_INITRDY) == 0) {
      break;
    }
    Timeout--;
  }
  if (Timeout == 0) {
    return EFI_TIMEOUT;
  }
  //
  // Initial Response status
  //
  *Response = SBI_INVALID_RESPONSE;
  SbiStat   = 0;
  ///
  /// 2. Write IOM offset D0h[31:0] with Address and Destination Port ID
  ///
  CpuRegbarWrite32 (SOC_DIE, CPU_SB_PID_IOM, 0, R_P2SB_CFG_SBIADDR, (UINT32) (((Pid << N_P2SB_CFG_SBIADDR_DESTID) & B_P2SB_CFG_SBIADDR_DESTID) | (Offset & B_P2SB_CFG_SBIADDR_OFFSET)));
  ///
  /// 3. Write IOM offset DCh[31:0] with extended address, which is expected to be 0.
  ///
  CpuRegbarWrite32 (SOC_DIE, CPU_SB_PID_IOM, 0, R_P2SB_CFG_SBIEXTADDR, (UINT32) RShiftU64(Offset, 16));
  ///
  /// 5. Set IOM PCI offset D8h[15:8] = 00000110b for read
  ///    Set IOM PCI offset D8h[15:8] = 00000111b for write
  //
  // Set SBISTAT[15:8] to the opcode passed in
  // Set SBISTAT[7] to the posted passed in
  //
  CpuRegbarAndThenOr16 (
    0,
    CPU_SB_PID_IOM,
    0,
    R_P2SB_CFG_SBISTAT,
    (UINT16) ~(B_P2SB_CFG_SBISTAT_OPCODE | B_P2SB_CFG_SBISTAT_POSTED),
    (UINT16) (((Opcode << N_P2SB_CFG_SBISTAT_OPCODE) & B_P2SB_CFG_SBISTAT_OPCODE) | (Posted << N_P2SB_CFG_SBISTAT_POSTED))
    );
  ///
  /// 6. Write IOM offset DAh[15:0] = F000h
  ///
  //
  // Set RID[15:0] = Fbe << 12 | Bar << 8 | Fid
  //
  CpuRegbarWrite16 (
    0,
    CPU_SB_PID_IOM,
    0,
    R_P2SB_CFG_SBIRID,
    (UINT16) (((Fbe << N_P2SB_CFG_SBIRID_FBE) & B_P2SB_CFG_SBIRID_FBE) |
              ((Bar << N_P2SB_CFG_SBIRID_BAR) & B_P2SB_CFG_SBIRID_BAR) |
              ((Fid << N_P2SB_CFG_SBIRID_FID) & B_P2SB_CFG_SBIRID_FID))
    );

  switch (Opcode) {
    case MemoryWrite:
    case PciConfigWrite:
    case PrivateControlWrite:
    case GpioLockUnlock:
      ///
      /// 4. Write IOM offset D4h[31:0] with the intended data accordingly
      ///
      CpuRegbarWrite32 (SOC_DIE, CPU_SB_PID_IOM, 0, R_P2SB_CFG_SBIDATA, *Data32);
      break;
    default:
      ///
      /// 4. Write IOM offset D4h[31:0] with dummy data such as 0,
      /// because all D0-DFh register range must be touched.
      /// for a successful SBI transaction.
      ///
      CpuRegbarWrite32 (SOC_DIE, CPU_SB_PID_IOM, 0, R_P2SB_CFG_SBIDATA, 0);
      break;
  }
  ///
  /// 7. Set IOM offset D8h[0] = 1b, Poll IOM offset D8h[0] = 0b
  ///
  //
  // Set SBISTAT[0] = 1b, trigger the SBI operation
  //
  CpuRegbarOr32 (SOC_DIE, CPU_SB_PID_IOM, 0, R_P2SB_CFG_SBISTAT, B_P2SB_CFG_SBISTAT_INITRDY);
  //
  // Poll SBISTAT[0] = 0b, Polling for Busy bit
  //
  Timeout = 0xFFFFFFF;
  while (Timeout > 0) {
    SbiStat = CpuRegbarRead16 (SOC_DIE, CPU_SB_PID_IOM, 0, R_P2SB_CFG_SBISTAT);
    if ((SbiStat & B_P2SB_CFG_SBISTAT_INITRDY) == 0) {
      break;
    }
    Timeout--;
  }
  if (Timeout == 0) {
    //
    // If timeout, it's fatal error.
    //
    return EFI_TIMEOUT;
  } else {
    ///
    /// 8. Check if IOM offset D8h[2:1] = 00b for successful transaction
    ///
    *Response = (UINT8) ((SbiStat & B_P2SB_CFG_SBISTAT_RESPONSE) >> N_P2SB_CFG_SBISTAT_RESPONSE);
    if (*Response == SBI_SUCCESSFUL) {
      switch (Opcode) {
        case MemoryRead:
        case PciConfigRead:
        case PrivateControlRead:
          ///
          /// 9. Read IOM offset D4h[31:0] for SBI data
          ///
          *Data32 = CpuRegbarRead32 (SOC_DIE, CPU_SB_PID_IOM, 0, R_P2SB_CFG_SBIDATA);
          break;
        default:
          break;
      }
      return EFI_SUCCESS;
    } else {
      return EFI_DEVICE_ERROR;
    }
  }
}

/**
  Convert register space enum into SBI opcode.

  @param[in] RegisterSpace  To which register space is the msg addressed
  @param[in] Write          TRUE if this is a write operation

  @return P2SB_SBI_OPCODE to address given register space
**/
STATIC
P2SB_SBI_OPCODE
CpuSbiOpcodeFromRegisterSpace (
  IN P2SB_REGISTER_SPACE   RegisterSpace,
  IN BOOLEAN               Write
  )
{
  switch (RegisterSpace) {
    case P2SbMemory:
      return Write ? MemoryWrite : MemoryRead;
    case P2SbPciConfig:
      return Write ? PciConfigWrite : PciConfigRead;
    case P2SbPrivateConfig:
    default:
      return Write ? PrivateControlWrite : PrivateControlRead;
  }
}

/**
  Reads an 8-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 8-bit register value specified by Offset
**/
UINT8
CpuSbiRead8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  *Private;

  Private = (P2SB_SIDEBAND_REGISTER_ACCESS*) This;

  switch (Private->AccessMethod) {
    case P2SbMmioAccess:
      return CpuRegbarRead8 (SOC_DIE, Private->P2SbPid, 0, (UINT16)Offset);
    case P2SbMsgAccess:
    default:
      DEBUG ((DEBUG_ERROR, "P2SB: Incorrect access method %d\n", Private->AccessMethod));
      return 0;
  }
}

/**
  Writes an 8-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] Value   Value to write to register

  @return The 8-bit register value written to register
**/
UINT8
CpuSbiWrite8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            Value
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  *Private;

  Private = (P2SB_SIDEBAND_REGISTER_ACCESS*) This;
  switch (Private->AccessMethod) {
    case P2SbMmioAccess:
      CpuRegbarWrite8 (SOC_DIE, Private->P2SbPid, 0, (UINT16)Offset, Value);
      //
      // Readback to flush the write cycle and ensure ordering with primary
      //
      return CpuRegbarRead8 (SOC_DIE, Private->P2SbPid, 0, (UINT16)Offset);
    case P2SbMsgAccess:
    default:
      DEBUG ((DEBUG_ERROR, "IOM: Incorrect access method %d\n", Private->AccessMethod));
      return 0;
  }
}

/**
  Performs an 8-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 8-bit register value written to register
**/
UINT8
CpuSbiOr8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            OrData
  )
{
  UINT8  Value;

  Value = CpuSbiRead8 (This, Offset);
  Value |= OrData;
  return CpuSbiWrite8 (This, Offset, Value);
}

/**
  Performs an 8-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 8-bit register value written to register
**/
UINT8
CpuSbiAnd8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            AndData
  )
{
  UINT8  Value;

  Value = CpuSbiRead8 (This, Offset);
  Value &= AndData;
  return CpuSbiWrite8 (This, Offset, Value);
}

/**
  Performs an 8-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 8-bit register value written to register
**/
UINT8
CpuSbiAndThenOr8 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT8            AndData,
  IN UINT8            OrData
  )
{
  UINT8  Value;

  Value = CpuSbiRead8 (This, Offset);
  Value &= AndData;
  Value |= OrData;
  return CpuSbiWrite8 (This, Offset, Value);
}

/**
  Reads a 16-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 16-bit register value specified by Offset
**/
UINT16
CpuSbiRead16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  *Private;

  Private = (P2SB_SIDEBAND_REGISTER_ACCESS*) This;

  switch (Private->AccessMethod) {
    case P2SbMmioAccess:
      return CpuRegbarRead16 (SOC_DIE, Private->P2SbPid, 0, (UINT16)Offset);
    case P2SbMsgAccess:
    default:
      DEBUG ((DEBUG_ERROR, "P2SB: Incorrect access method %d\n", Private->AccessMethod));
      return 0;
  }
}

/**
  Writes a 16-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] Value   Value to write to register

  @return The 16-bit register value written to register
**/
UINT16
CpuSbiWrite16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           Value
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  *Private;

  Private = (P2SB_SIDEBAND_REGISTER_ACCESS*) This;
  switch (Private->AccessMethod) {
    case P2SbMmioAccess:
      CpuRegbarWrite16 (SOC_DIE, Private->P2SbPid, 0, (UINT16)Offset, Value);
      //
      // Readback to flush the write cycle and ensure ordering with primary
      //
      return CpuRegbarRead16 (SOC_DIE, Private->P2SbPid, 0, (UINT16)Offset);
    case P2SbMsgAccess:
    default:
      DEBUG ((DEBUG_ERROR, "IOM: Incorrect access method %d\n", Private->AccessMethod));
      return 0;
  }
}

/**
  Performs a 16-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
UINT16
CpuSbiOr16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           OrData
  )
{
  UINT16  Value;

  Value = CpuSbiRead16 (This, Offset);
  Value |= OrData;
  return CpuSbiWrite16 (This, Offset, Value);
}

/**
  Performs a 16-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 16-bit register value written to register
**/
UINT16
CpuSbiAnd16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           AndData
  )
{
  UINT16  Value;

  Value = CpuSbiRead16 (This, Offset);
  Value &= AndData;
  return CpuSbiWrite16 (This, Offset, Value);
}

/**
  Performs a 16-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 16-bit register value written to register
**/
UINT16
CpuSbiAndThenOr16 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT16           AndData,
  IN UINT16           OrData
  )
{
  UINT16  Value;

  Value = CpuSbiRead16 (This, Offset);
  Value &= AndData;
  Value |= OrData;
  return CpuSbiWrite16 (This, Offset, Value);
}

/**
  Reads a 32-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank

  @return The 32-bit register value specified by Offset
**/
UINT32
CpuSbiRead32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  *Private;
  EFI_STATUS                     Status;
  UINT32                         Value;
  UINT8                          Response;

  Private = (P2SB_SIDEBAND_REGISTER_ACCESS*) This;

  switch (Private->AccessMethod) {
    case P2SbMmioAccess:
      return CpuRegbarRead32 (SOC_DIE, Private->P2SbPid, 0, (UINT16)Offset);
    case P2SbMsgAccess:
      Status = CpuSbiExecutionEx (
                 Private->P2SbPid,
                 Offset,
                 CpuSbiOpcodeFromRegisterSpace (Private->RegisterSpace, FALSE),
                 FALSE,
                 0xF,
                 0,
                 Private->Fid,
                 &Value,
                 &Response
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "IOM: MSG failed %r, %d\n", Status, Response));
        return 0xFFFFFFFF;
      } else {
        return Value;
      }
    default:
      DEBUG ((DEBUG_ERROR, "IOM: Incorrect access method %d\n", Private->AccessMethod));
      return 0xFFFFFFFF;
  }
}

/**
  Writes a 32-bit register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] Value   Value to write to register

  @return The 32-bit register value written to register
**/
UINT32
CpuSbiWrite32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           Value
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS  *Private;
  EFI_STATUS                     Status;
  UINT8                          Response;

  Private = (P2SB_SIDEBAND_REGISTER_ACCESS*) This;
  switch (Private->AccessMethod) {
    case P2SbMmioAccess:
      CpuRegbarWrite32 (SOC_DIE, Private->P2SbPid, 0, Offset, Value);
      //
      // Readback to flush the write cycle and ensure ordering with primary
      //
      return CpuRegbarRead32 (SOC_DIE, Private->P2SbPid, 0, Offset);
    case P2SbMsgAccess:
      Status = CpuSbiExecutionEx (
                 Private->P2SbPid,
                 Offset,
                 CpuSbiOpcodeFromRegisterSpace (Private->RegisterSpace, TRUE),
                 FALSE,
                 0xF,
                 0,
                 Private->Fid,
                 &Value,
                 &Response
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "IOM: MSG failed %r, %d\n", Status, Response));
        return 0xFFFFFFFF;
      } else {
        return Value;
      }
    default:
      DEBUG ((DEBUG_ERROR, "IOM: Incorrect access method %d\n", Private->AccessMethod));
      return 0xFFFFFFFF;
  }
}

/**
  Performs a 32-bit or on the register

  @param[in] This    Pointer to REGISTER_ACCESS
  @param[in] Offset  Offset of the register in the register bank
  @param[in] OrData  Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
UINT32
CpuSbiOr32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           OrData
  )
{
  UINT32  Value;

  Value = CpuSbiRead32 (This, Offset);
  Value |= OrData;
  return CpuSbiWrite32 (This, Offset, Value);
}

/**
  Performs a 32-bit and on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed

  @return The 32-bit register value written to register
**/
UINT32
CpuSbiAnd32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData
  )
{
  UINT32  Value;

  Value = CpuSbiRead32 (This, Offset);
  Value &= AndData;
  return CpuSbiWrite32 (This, Offset, Value);
}

/**
  Performs a 32-bit and then or on the register

  @param[in] This     Pointer to REGISTER_ACCESS
  @param[in] Offset   Offset of the register in the register bank
  @param[in] AndData  Data with which register should be AND-ed
  @param[in] OrData   Data with which register should be OR-ed

  @return The 32-bit register value written to register
**/
UINT32
CpuSbiAndThenOr32 (
  IN REGISTER_ACCESS  *This,
  IN UINT32           Offset,
  IN UINT32           AndData,
  IN UINT32           OrData
  )
{
  UINT32  Value;

  Value = CpuSbiRead32 (This, Offset);
  Value &= AndData;
  Value |= OrData;
  return CpuSbiWrite32 (This, Offset, Value);
}

/**
  Builds CPU SBI access.

  @param[in]  Pid                           P2SB PID
  @param[in]  Fid                           P2SB FID
  @param[in]  RegisterSpace                 Register space to access
  @param[in]  AccessMethod                  Access method
  @param[out] CpuSbiRegisterAccess          Structure to initialize

  @retval TRUE   Initialized successfully
  @retval FALSE  Failed to initialize
**/
BOOLEAN
BuildCpuSbiAccess (
  IN  P2SB_PID                       Pid,
  IN  UINT16                         Fid,
  IN  P2SB_REGISTER_SPACE            RegisterSpace,
  IN  P2SB_SIDEBAND_ACCESS_METHOD    AccessMethod,
  OUT P2SB_SIDEBAND_REGISTER_ACCESS  *CpuSbiRegisterAccess
  )
{
  if (CpuSbiRegisterAccess == NULL) {
    return FALSE;
  }
  CpuSbiRegisterAccess->Access.Read8 = CpuSbiRead8;
  CpuSbiRegisterAccess->Access.Write8 = CpuSbiWrite8;
  CpuSbiRegisterAccess->Access.Or8 = CpuSbiOr8;
  CpuSbiRegisterAccess->Access.And8 = CpuSbiAnd8;
  CpuSbiRegisterAccess->Access.AndThenOr8 = CpuSbiAndThenOr8;
  CpuSbiRegisterAccess->Access.Read16 = CpuSbiRead16;
  CpuSbiRegisterAccess->Access.Write16 = CpuSbiWrite16;
  CpuSbiRegisterAccess->Access.Or16 = CpuSbiOr16;
  CpuSbiRegisterAccess->Access.And16 = CpuSbiAnd16;
  CpuSbiRegisterAccess->Access.AndThenOr16 = CpuSbiAndThenOr16;
  CpuSbiRegisterAccess->Access.Read32 = CpuSbiRead32;
  CpuSbiRegisterAccess->Access.Write32 = CpuSbiWrite32;
  CpuSbiRegisterAccess->Access.Or32 = CpuSbiOr32;
  CpuSbiRegisterAccess->Access.And32 = CpuSbiAnd32;
  CpuSbiRegisterAccess->Access.AndThenOr32 = CpuSbiAndThenOr32;

  CpuSbiRegisterAccess->AccessMethod = AccessMethod;
  CpuSbiRegisterAccess->P2SbPid = Pid;
  CpuSbiRegisterAccess->Fid = Fid;
  CpuSbiRegisterAccess->RegisterSpace = RegisterSpace;
  CpuSbiRegisterAccess->PostedWrites = FALSE; // No use case so far to support posted writes on CPU SBI.
  CpuSbiRegisterAccess->P2SbCtrl = NULL; // CPU SBI doesn't support multiple P2SB devices.

  return TRUE;
}