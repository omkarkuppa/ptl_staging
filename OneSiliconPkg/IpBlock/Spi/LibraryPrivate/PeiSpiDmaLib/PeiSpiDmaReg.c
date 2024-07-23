/** @file
  SPI DMA register routines

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <Register/SpiRegs.h>
#include <Library/PchPciBdfLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/Spi.h>

#define DELAY_STEP_USEC                        100               // 0.1 ms
#define MAX_WAIT_FOR_DMA_WRITE_PTR_RESET_USEC  1000 * 1000 * 1   // 1 second
#define MAX_WAIT_FOR_DMA_TO_FINISH_USEC        1000 * 1000 * 1   // 1 second

/**
  Checks SPI DMA related registers for information whether DMA operation is underway

  @param[in]  SpiPciCfg     SPI PCI config base address

  @retval     BOOLEAN       TRUE if DMA is happening right now else returns FALSE
**/
STATIC
BOOLEAN
IsDmaOperationUnderway (
  UINT64  SpiPciCfg
  )
{
  UINT32 DmaControl;
  UINT32 DmaStatus;

  DmaControl = PciSegmentRead32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL);
  DmaStatus = PciSegmentRead32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_STS);

  if (((DmaControl & B_SPI_CFG_BIOS_RD_DMA_CNTRL_SPI_BIOS_RD_DMA_START) != 0) &&
      ((DmaStatus & B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_CPL) == 0)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Helper function for reseting DMA write pointer which is used by SPI to access
  correct offset in buffer when copying memory from flash

  @param[in]  SpiPciCfg     SPI PCI config base address

  @retval     EFI_SUCCESS   Either reset pointer correctly or it was at 0 already
              EFI_TIMEOUT   Timed out on waiting for pointer to reset to 0
**/
STATIC
EFI_STATUS
ResetDmaWritePointer (
  UINT64 SpiPciCfg
  )
{
  UINT32 Data32;
  UINT32 Timeout;

  Data32 = PciSegmentRead32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_STS);
  if ((Data32 & B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_WRPTR) == 0) {
    DEBUG ((DEBUG_INFO, "%a SPI DMA write pointer is already at 0\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "%a:%d: resetting DMA write pointer and polling \n", __FUNCTION__, __LINE__));
  PciSegmentOr32 (
    SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL,
    B_SPI_CFG_BIOS_RD_DMA_CNTRL_SPI_BIOS_RD_DMA_WRPTR_RST
    );

  Timeout = MAX_WAIT_FOR_DMA_WRITE_PTR_RESET_USEC;

  while (Timeout > 0) {
    Data32 = PciSegmentRead32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL);
    if ((Data32 & B_SPI_CFG_BIOS_RD_DMA_CNTRL_SPI_BIOS_RD_DMA_WRPTR_RST) == 0) {
      return EFI_SUCCESS;
    }
    MicroSecondDelay (DELAY_STEP_USEC);
    Timeout -= DELAY_STEP_USEC;
  }

  DEBUG ((DEBUG_ERROR, "%a operation timed out\n", __FUNCTION__));
  return EFI_TIMEOUT;
}

/**
  Clears DMA related status bits from SPI controller

  @param[in]  SpiPciCfg     SPI PCI config base address
**/
STATIC
VOID
ClearStatusBits (
  UINT64 SpiPciCfg
  )
{
  UINT32 StatusBits;

  StatusBits = (
    B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_ERR |
    B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_SAF_CE |
    B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_SAF_LE |
    B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_SAF_DLE |
    B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_AEL
    );

  PciSegmentOr32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_STS, StatusBits);
}

/**
  Requests SPI to start DMA copy to a given buffer

  @param[in]  BufferBaseAddress   Destination buffer address
  @param[in]  BufferSize          Size of destination buffer
  @param[in]  BiosRegionOffset    Offset from top of BIOS region in 1kB intervals
  @param[in]  FlashRegion         Which BIOS region to read from (only primary
                                  and secondary are supported).

  @retval   EFI_INVALID_PARAMETER   Invalid BIOS region chosen
  @retval   EFI_ACCESS_DENIED       SPI controller not available or DMA operation
                                    already ongoing
  @retval   EFI_TIMEOUT             Timed out on waiting on DMA pointer reset or
                                    DMA operation timed out
  @retval   EFI_DEVICE_ERROR        One of status bits reported an error
  @retval   EFI_SUCESS              DMA completed successfully
**/
EFI_STATUS
StartSpiDma (
  IN  VOID                *BufferBaseAddress,
  IN  UINTN               BufferSize,
  IN  UINT32              BiosRegionOffset,
  IN  FLASH_REGION_TYPE   FlashRegion
  )
{
  UINT32      BufferAddressHi;
  UINT32      BufferAddressLo;
  UINT64      SpiPciCfg;
  UINT32      Data32;
  UINT16      OriginalCmd;
  EFI_STATUS  Status;
  UINT32      Timeout;

  DEBUG ((DEBUG_INFO, "%a %d entry \n", __FUNCTION__, __LINE__));
  DEBUG ((DEBUG_INFO, "%a %d BufferBaseAddress %08X \n", __FUNCTION__, __LINE__, (UINTN)BufferBaseAddress));
  DEBUG ((DEBUG_INFO, "%a %d BufferSize %08X \n", __FUNCTION__, __LINE__, BufferSize ));
  DEBUG ((DEBUG_INFO, "%a %d BiosRegionOffset %08X \n", __FUNCTION__, __LINE__, BiosRegionOffset));

  if ((FlashRegion != FlashRegionBios) && (FlashRegion != FlashRegionSecondaryBios)) {
    DEBUG ((DEBUG_ERROR, "%a only reads from BIOS region are supported\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  SpiPciCfg = SpiPciCfgBase ();
  if (PciSegmentRead32 (SpiPciCfg) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "%a: SPI is not available, aborting...\n", __FUNCTION__));
    return EFI_ACCESS_DENIED;
  }

  // Check DMA Start bit and CPL bit
  if (IsDmaOperationUnderway (SpiPciCfg)) {
    DEBUG ((DEBUG_ERROR, "%a DMA operation is still ongoing, aborting...\n", __FUNCTION__));
    return EFI_ACCESS_DENIED;
  }

  // Reset Write Pointer
  Status = ResetDmaWritePointer (SpiPciCfg);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a timeout when reseting DMA pointer \n", __FUNCTION__));
    return EFI_TIMEOUT;
  }

  // Clear status bits
  ClearStatusBits (SpiPciCfg);

  // Program buffer address
  BufferAddressHi = (UINT32) (RShiftU64 (((UINT64)(UINTN)BufferBaseAddress), 32));
  BufferAddressLo = (UINT32) (UINTN)(BufferBaseAddress);
  PciSegmentWrite32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_BA_HI, BufferAddressHi);
  PciSegmentWrite32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_BA, BufferAddressLo);

  PciSegmentWrite32 (
    SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_BASE_OFFSET,
    ((BiosRegionOffset - 1) << N_SPI_CFG_BIOS_RD_DMA_BASE_OFFSET_BIOS_RD_DMA_BASE_OFFSET)
    );

  PciSegmentAndThenOr32 (
    SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL,
    (UINT32)~(B_SPI_CFG_BIOS_RD_DMA_CNTRL_BIOS_RD_DMA_SIZE),
    (UINT32) ((BufferSize-1) << N_SPI_CFG_BIOS_RD_DMA_CNTRL_BIOS_RD_DMA_SIZE)
    );

  PciSegmentAndThenOr32 (
    SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL,
    (UINT32)~(B_SPI_CFG_BIOS_RD_DMA_CNTRL_RD_DMA_REGION),
    (UINT32) (FlashRegion << N_SPI_CFG_BIOS_RD_DMA_CNTRL_RD_DMA_REGION)
    );

  OriginalCmd = (UINT16) PciSegmentRead16 (SpiPciCfg + PCI_COMMAND_OFFSET);
  PciSegmentOr32 (SpiPciCfg + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_BUS_MASTER);

  // Start the DMA operation
  PciSegmentAndThenOr32 (
    SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL,
    (UINT32)~B_SPI_CFG_BIOS_RD_DMA_CNTRL_SPI_BIOS_RD_DMA_START,
    B_SPI_CFG_BIOS_RD_DMA_CNTRL_SPI_BIOS_RD_DMA_START
    );

  // Poll CPL
  Timeout = MAX_WAIT_FOR_DMA_TO_FINISH_USEC;
  while (Timeout > 0) {
    Data32 = PciSegmentRead32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_STS);
    if ((Data32 & B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_CPL) != 0) {
      break;
    }
    MicroSecondDelay (DELAY_STEP_USEC);
    Timeout -= DELAY_STEP_USEC;
  }

  if (Timeout == 0) {
    DEBUG ((DEBUG_ERROR, "%a DMA operation timed out\n", __FUNCTION__));
    return EFI_TIMEOUT;
  }

  PciSegmentAndThenOr32 (
    SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_STS,
    (UINT32)~B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_CPL,
    B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_CPL
    );

  // Restore original PCI_CMD
  PciSegmentWrite16 (SpiPciCfg + PCI_COMMAND_OFFSET, OriginalCmd);

  // Read DMA_STS and check for errors
  Data32 = PciSegmentRead32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_STS);
  if ((Data32 & (
         B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_ERR |
         B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_SAF_CE  |
         B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_SAF_LE  |
         B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_SAF_DLE |
         B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_AEL
         )) != 0) {
    DEBUG ((
      DEBUG_ERROR,
      "%a one or more errors occured during DMA operation: %08X\n",
      __FUNCTION__, Data32
      ));
    return EFI_DEVICE_ERROR;
  }

  return EFI_SUCCESS;
}

/**
  Callback to lock SPI DMA after copying FVs.

  @param PeiServices       -  General purpose services available to every PEIM.
  @param NotifyDescriptor  -  Not used.
  @param Ppi               -  Not used.

  @retval EFI_SUCCESS   - SPI DMA Locked successfully
          EFI_ACCESS_DENIED - SPI Region was not available
**/
EFI_STATUS
LockSpiDmaCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  UINT64   SpiPciCfg;
  UINT32      Data32;

  SpiPciCfg = SpiPciCfgBase ();

  if (PciSegmentRead32 (SpiPciCfg) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "%a: SPI is not available, aborting...\n", __FUNCTION__));
    return EFI_ACCESS_DENIED;
  }


  // Check DMA Start bit and CPL bit if the DMA is still happening
  if (IsDmaOperationUnderway (SpiPciCfg)) {
    DEBUG ((DEBUG_ERROR, "%a DMA hang detected. Will clear start bit before locking SPI DMA\n", __FUNCTION__));

    // Set DMA CPL bit & Clear the DMA Start bit, before locking, if hang is detected
    // WA SW fix for backpressure issue, to ensure that SPI Clock stops and the system
    // can put the SPI IP to sleep for S0iX
    PciSegmentAndThenOr32 (
      SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_STS,
      (UINT32)~B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_CPL,
      B_SPI_CFG_BIOS_RD_DMA_STS_SPI_BIOS_RD_DMA_CPL
      );
    PciSegmentAnd32 (
      SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL,
      (UINT32)~B_SPI_CFG_BIOS_RD_DMA_CNTRL_SPI_BIOS_RD_DMA_START
      );
  }

  Data32 = PciSegmentRead32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL);

  if ((Data32 & B_SPI_CFG_BIOS_RD_DMA_CNTRL_BIOS_DMA_LOCK) == 0) {
    PciSegmentAndThenOr32 (
      SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL,
      (UINT32)~B_SPI_CFG_BIOS_RD_DMA_CNTRL_BIOS_DMA_LOCK,
      B_SPI_CFG_BIOS_RD_DMA_CNTRL_BIOS_DMA_LOCK
      );
  }

  DEBUG ((DEBUG_INFO, "%a: SPI DMA Locked Successfully\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Check if SPI DMA is supported on the Soc

  @retval     TRUE              SPI DMA is supported.
  @retval     FALSE             SPI DMA isn't supported.
**/
BOOLEAN
IsSpiDmaSupported (
  VOID
)
{
  UINT64   SpiPciCfg;
  UINT32      Data32;
  UINT32   DmaRegion;

  SpiPciCfg = SpiPciCfgBase ();

  if (PciSegmentRead32 (SpiPciCfg) == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "%a: SPI is not available, aborting...\n", __FUNCTION__));
    return FALSE;
  }

  // Read DMA_REGION
  Data32 = PciSegmentRead32 (SpiPciCfg + R_SPI_CFG_BIOS_RD_DMA_CNTRL);

  DmaRegion = (Data32 & B_SPI_CFG_BIOS_RD_DMA_CNTRL_RD_DMA_REGION) >> N_SPI_CFG_BIOS_RD_DMA_CNTRL_RD_DMA_REGION;

  if (DmaRegion == FlashRegionBios) {
    DEBUG ((DEBUG_INFO, "%a: SPI DMA region writeable. SPI DMA is supported DMA Region%x \n", __FUNCTION__, DmaRegion));
    return TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "%a: SPI DMA region isn't writeable. SPI DMA not supported DMA Region%x \n", __FUNCTION__, DmaRegion));
    return FALSE;
  }
}
