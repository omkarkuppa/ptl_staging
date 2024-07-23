/** @file
  PCH SPI Runtime Driver implements the SPI Host Controller Compatibility Interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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

/**
  @note:  If the operating system contains a driver that accesses the SPI
          controller, then it is not possible to guarantee that the OS SPI
          driver does not attempt to access the SPI controller on a different
          thread at the same time this Runtime DXE code attempts to access the
          same controller. Therefore, if one uses this Runtime DXE
          implementation on an OS that performs direct SPI access, one must
          ensure the OS implements a semaphore that guarantees mutual
          exclusion between direct SPI access and the UEFI runtime services.

          For this reason and other security reasons, on general purpose
          computing systems the SMM implementation of this SPI driver is
          recommended over the Runtime DXE implementation.
**/

#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Guid/EventGroup.h>
#include <Library/PciSegmentLib.h>
#include <Protocol/Spi.h>
#include <Library/SpiCommonLib.h>
#include <Defines/PchReservedResources.h>
#include <IndustryStandard/Pci30.h>
#include <Register/SpiRegs.h>

///
/// Global variables
///
GLOBAL_REMOVE_IF_UNREFERENCED SPI_INSTANCE                  *mSpiInstance;
///
/// PchSpiBar0PhysicalAddr keeps the reserved MMIO range assigned to SPI from PEI.
/// It won't be updated no matter the SPI MMIO is reallocated by BIOS PCI enum.
/// And it's used to override the SPI BAR0 register in runtime environment,
///
GLOBAL_REMOVE_IF_UNREFERENCED UINT64                        mPchSpiBar0PhysicalAddr;
///
/// PchSpiBar0VirtualAddr keeps the virtual address of PchSpiBar0PhysicalAddr.
/// And it's used to provide the SPI BAR0 virtual address mapping to PchSpiBar0PhysicalAddr
/// in runtime environment. Bit address could be over 4G in 64bit OS.
///
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                         mPchSpiBar0VirtualAddr;
//
// mPchSpiBar0SavedPhysicalAddr keeps the MMIO range assigned to SPI by PCI enumeration.
// This is used to restore the original value for SPI BAR0 after finishing
// commands to the SPI controller.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT64                        mPchSpiBar0SavedPhysicalAddr;
//
// mPchSpiBar0RefCount stores the reference count for SPI BAR0.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32                        mPchSpiBar0RefCount;
//
// mPchSpiSavedPciCmdReg stores the PCI command register state at the start of the
// SPI transaction. This is used to restore the original PCI command register
// state after finishing commands to the SPI controller.
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                         mPchSpiSavedPciCmdReg;
//
// mSavedInterruptState stores the original processor interrupt state at the start of
// the SPI transaction. This is used to restore the original interrupt state after
// finishing commands to the SPI controller.
//
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                       mSavedInterruptState;
GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN                       mRuntimeFlag;

//
// Function implementations
//

/**
  Fixup internal data pointers so that the services can be called in virtual mode.

  @param[in] Event                The event registered.
  @param[in] Context              Event context. Not used in this event handler.

**/
VOID
EFIAPI
PchSpiVirtualAddressChangeEvent (
  IN EFI_EVENT        Event,
  IN VOID             *Context
  )
{
  mRuntimeFlag = TRUE;

  gRT->ConvertPointer (0, (VOID *) &mPchSpiBar0VirtualAddr);
  gRT->ConvertPointer (0, (VOID *) &(mSpiInstance->SpiProtocol.FlashRead));
  gRT->ConvertPointer (0, (VOID *) &(mSpiInstance->SpiProtocol.FlashWrite));
  gRT->ConvertPointer (0, (VOID *) &(mSpiInstance->SpiProtocol.FlashErase));
  gRT->ConvertPointer (0, (VOID *) &(mSpiInstance->SpiProtocol.FlashReadSfdp));
  gRT->ConvertPointer (0, (VOID *) &(mSpiInstance->SpiProtocol.FlashReadJedecId));
  gRT->ConvertPointer (0, (VOID *) &(mSpiInstance->SpiProtocol.FlashWriteStatus));
  gRT->ConvertPointer (0, (VOID *) &(mSpiInstance->SpiProtocol.FlashReadStatus));
  gRT->ConvertPointer (0, (VOID *) &(mSpiInstance->SpiProtocol.ReadPchSoftStrap));
  gRT->ConvertPointer (0, (VOID *) &mSpiInstance);
}

/**
  <b>SPI Runtime DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SPI Runtime DXE module provide a standard way for other modules to use the PCH SPI Interface in DXE/Runtime.

  - @pre
    - If SPI Runtime DXE driver is run before Status Code Runtime Protocol is installed
      and there is the need to use Status code in the driver, it will be necessary
      to add EFI_STATUS_CODE_RUNTIME_PROTOCOL_GUID to the dependency file.

  - @result
    The SPI Runtime DXE driver produces @link _PCH_SPI_PROTOCOL PCH_SPI_PROTOCOL @endlink

  - <b>Integration Check List</b>\n
    - This driver supports Descriptor Mode only.
    - This driver supports Hardware Sequence only.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
  @exception EFI_UNSUPPORTED      The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Do not have enough resources to initialize the driver.
  @retval EFI_DEVICE_ERROR        Device error, driver exits abnormally.
**/
EFI_STATUS
EFIAPI
InstallPchSpi (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS                      Status;
  UINT64                          BaseAddress;
  UINT64                          Length;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR SpiBar0MemorySpaceDescriptor;
  UINT64                          Attributes;
  EFI_EVENT                       AddressChangeEvent;

  DEBUG ((DEBUG_INFO, "InstallPchSpi() Start\n"));

  ///
  /// Allocate Runtime memory for the SPI protocol instance.
  ///
  mSpiInstance = AllocateRuntimeZeroPool (sizeof (SPI_INSTANCE));
  if (mSpiInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ///
  /// Initialize the SPI protocol instance
  ///
  mPchSpiBar0PhysicalAddr       = PCH_SPI_BASE_ADDRESS;
  mPchSpiBar0VirtualAddr        = (UINTN) mPchSpiBar0PhysicalAddr;
  mPchSpiBar0SavedPhysicalAddr  = 0;
  mPchSpiBar0RefCount           = 0;
  mPchSpiSavedPciCmdReg         = 0;
  mSavedInterruptState          = FALSE;
  mRuntimeFlag                  = FALSE;
  Status = SpiProtocolConstructor (mSpiInstance);
  if (EFI_ERROR (Status)) {
    FreePool (mSpiInstance);
    return Status;
  }
  PciSegmentRegisterForRuntimeAccess (mSpiInstance->PchSpiBase);
  ///
  /// Install the PCH_SPI_PROTOCOL interface
  ///
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &(mSpiInstance->Handle),
                  &gPchSpiProtocolGuid,
                  &(mSpiInstance->SpiProtocol),
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    FreePool (mSpiInstance);
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Create Address Change event
  ///
  ///
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  PchSpiVirtualAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &AddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Set SPI space in GCD to be RUNTIME so that the range will be supported in
  /// virtual address mode in EFI aware OS runtime.
  /// It will assert if SPI Memory Space is not allocated
  /// The caller is responsible for the existence and allocation of the SPi Memory Spaces
  ///

  //
  //  SPI MMIO memory space
  //
  BaseAddress = (EFI_PHYSICAL_ADDRESS) mPchSpiBar0PhysicalAddr;
  Length      = 0x1000;

  Status      = gDS->GetMemorySpaceDescriptor (BaseAddress, &SpiBar0MemorySpaceDescriptor);
  ASSERT_EFI_ERROR (Status);

  Attributes  = SpiBar0MemorySpaceDescriptor.Attributes | EFI_MEMORY_RUNTIME;

  Status      = gDS->SetMemorySpaceAttributes (
                       BaseAddress,
                       Length,
                       Attributes
                       );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "InstallPchSpi() End\n"));

  return EFI_SUCCESS;
}

/**
  Acquire PCH SPI MMIO address.
  It is not expected for this BAR0 to change because the SPI device is usually
  hidden from the OS. But if it is ever different from the preallocated address,
  reassign it back. The SpiBar0 could be 64bit in virtual address.

  @param[in] SpiInstance          Pointer to SpiInstance to initialize

  @retval PchSpiBar0              return SPI MMIO address
**/
UINTN
AcquireSpiBar0 (
  IN  SPI_INSTANCE                *SpiInstance
  )
{
  UINT64                          SpiBar0;
  //
  // Save original SPI physical MMIO address
  //
  SpiBar0 = PciSegmentRead32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET) & B_SPI_CFG_BIOS_SPI_BAR0_MEMBAR;
  if (PciSegmentRead32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
    SpiBar0 |= LShiftU64 ((UINT64) PciSegmentRead32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET + 4), 32);
  }

  if (SpiBar0 != mPchSpiBar0PhysicalAddr) {
    //
    // Save interrupt state, PCI command register state,
    // and SPI BAR0 value assigned by PCI enumeration
    //
    mSavedInterruptState          = SaveAndDisableInterrupts ();
    mPchSpiSavedPciCmdReg         = PciSegmentRead8 (SpiInstance->PchSpiBase + PCI_COMMAND_OFFSET);
    mPchSpiBar0SavedPhysicalAddr  = SpiBar0;

    //
    // Temporary disable MSE, and override with SPI reserved MMIO address, then enable MSE.
    //
    PciSegmentAnd8 (SpiInstance->PchSpiBase + PCI_COMMAND_OFFSET, (UINT8) ~EFI_PCI_COMMAND_MEMORY_SPACE);
    PciSegmentWrite32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) mPchSpiBar0PhysicalAddr);
    if (PciSegmentRead32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
      PciSegmentWrite32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (mPchSpiBar0PhysicalAddr, 32));
    }
    PciSegmentOr8 (SpiInstance->PchSpiBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  } else if (mPchSpiBar0RefCount == 0) {
    mPchSpiBar0SavedPhysicalAddr  = 0;
    mPchSpiSavedPciCmdReg         = 0;
    mSavedInterruptState          = FALSE;
  }
  mPchSpiBar0RefCount++;

  if (mRuntimeFlag) {
    return mPchSpiBar0VirtualAddr;
  } else {
    return (UINTN) mPchSpiBar0PhysicalAddr;
  }
}

/**
  Release PCH SPI MMIO address. If AcquireSpiBar0() previously overwrote the
  value of BAR0, this function will restore the original value assigned by PCI
  enumeration

  @param[in] SpiInstance          Pointer to SpiInstance to initialize

  @retval None
**/
VOID
ReleaseSpiBar0 (
  IN     SPI_INSTANCE       *SpiInstance
  )
{
  //
  // Reference counting is used here because multiple nested calls to
  // AcquireSpiBar0()/ReleaseSpiBar0() will cause SpiBar0 to be reprogrammed
  // back to the original value before access to the SPI controller is done.
  // Reference counting ensures that the BAR is not restored until after access
  // is complete.
  //
  if (mPchSpiBar0RefCount <= 1) {
    mPchSpiBar0RefCount = 0;
    if (mPchSpiBar0SavedPhysicalAddr != 0) {
      //
      // Temporary disable MSE, restore the original SPI MMIO address, then
      // restore PCI command register state
      //
      PciSegmentAnd8 (SpiInstance->PchSpiBase + PCI_COMMAND_OFFSET, (UINT8)~EFI_PCI_COMMAND_MEMORY_SPACE);
      PciSegmentWrite32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET, (UINT32) mPchSpiBar0SavedPhysicalAddr);
      if (PciSegmentRead32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET) & BIT2) {
        PciSegmentWrite32 (SpiInstance->PchSpiBase + PCI_BASE_ADDRESSREG_OFFSET + 4, (UINT32) RShiftU64 (mPchSpiBar0SavedPhysicalAddr, 32));
      }
      PciSegmentWrite8 (SpiInstance->PchSpiBase + PCI_COMMAND_OFFSET, mPchSpiSavedPciCmdReg);

      //
      // Clear saved state
      //
      mPchSpiBar0SavedPhysicalAddr  = 0;
      mPchSpiSavedPciCmdReg         = 0;

      //
      // Restore interrupt state
      //
      SetInterruptState (mSavedInterruptState);
      mSavedInterruptState  = FALSE;
    }
  } else {
    mPchSpiBar0RefCount--;
  }
}

/**
  This function is a hook for Spi to disable BIOS Write Protect

  @retval EFI_SUCCESS             The protocol instance was properly initialized
  @retval EFI_ACCESS_DENIED       The BIOS Region can only be updated in SMM phase

**/
EFI_STATUS
EFIAPI
DisableBiosWriteProtect (
  VOID
  )
{
  UINT64                          SpiBaseAddress;

  SpiBaseAddress = mSpiInstance->PchSpiBase;
  if ((PciSegmentRead8 (SpiBaseAddress + R_SPI_CFG_BIOS_SPI_BC) & B_SPI_CFG_BIOS_SPI_BC_EISS) != 0) {
    return EFI_ACCESS_DENIED;
  }
  ///
  /// Enable the access to the BIOS space for both read and write cycles
  ///
  PciSegmentOr8 (
    SpiBaseAddress + R_SPI_CFG_BIOS_SPI_BC,
    (UINT8) (B_SPI_CFG_BIOS_SPI_BC_WPD)
    );

  return EFI_SUCCESS;
}

/**
  This function is a hook for Spi to enable BIOS Write Protect
**/
VOID
EFIAPI
EnableBiosWriteProtect (
  VOID
  )
{
  UINT64           SpiBaseAddress;

  SpiBaseAddress = mSpiInstance->PchSpiBase;
  ///
  /// Disable the access to the BIOS space for write cycles
  ///
  PciSegmentAnd8 (
    SpiBaseAddress + R_SPI_CFG_BIOS_SPI_BC,
    (UINT8) (~B_SPI_CFG_BIOS_SPI_BC_WPD)
    );
}

/**
  Check if it's granted to do flash write.

  @retval TRUE    It's secure to do flash write.
  @retval FALSE   It's not secure to do flash write.
**/
BOOLEAN
IsSpiFlashWriteGranted (
  VOID
  )
{
  return TRUE;
}

/**
  Check if a save and restore of the SPI controller state is necessary

  @retval TRUE    It's necessary to save and restore SPI controller state
  @retval FALSE   It's not necessary to save and restore SPI controller state
**/
BOOLEAN
IsSpiControllerSaveRestoreEnabled (
  VOID
  )
{
  return mRuntimeFlag;
}
