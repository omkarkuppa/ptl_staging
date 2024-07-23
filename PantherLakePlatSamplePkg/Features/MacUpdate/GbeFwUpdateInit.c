/** @file
      Gbe fw Update Initialization driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "GbeFwUpdateDefines.h"
#include <Guid/EventGroup.h>

GLOBAL_REMOVE_IF_UNREFERENCED EFI_EVENT            EndOfDxeEvent;

const struct SUPPORTED_IDS SupportedDeviceId[] = {
  {0x156F}, {0x1570}, {0x15B7}, {0x15B8}, {0x15B9}, {0x15D7}, {0x15D8},
  {0x15E3}, {0x15D6}, {0x15BD}, {0x15BE}, {0x15BB}, {0x15BC}, {0x15DF},
  {0x15E0}, {0x15E1}, {0x15E2}, {0x0D4E}, {0x0D4F}, {0x0D4C}, {0x0D4D},
  {0x0D53}, {0x0D55}, {0x15FB}, {0x15FC}, {0x15F9}, {0x15FA}, {0x15F4},
  {0x15F5}, {0x1A1E}, {0x1A1F}, {0x1A1C}, {0x1A1D}, {0x550A}, {0x550B},
  {0x550C}, {0x550D}, {0x0DC7}, {0x550F}, {0x550E}, {0x57B3}, {0x57B4},
  {0}
};

/**
  Create I2C request packet based on operations size.

  @param[in] Operations         Count of Operations that you want to executed

  @retval Packet                Pointer to EFI_I2C_REQUEST_PACKET
**/
EFI_I2C_REQUEST_PACKET*
PssMonzaNewRequestPacket (
  IN UINTN Operations
  )
{
  EFI_I2C_REQUEST_PACKET *NewPacket;

  if (Operations == 0) {
    return NULL;
  }
  NewPacket = AllocateZeroPool (sizeof (EFI_I2C_REQUEST_PACKET) + (Operations - 1) * sizeof (EFI_I2C_OPERATION));

  if (NewPacket != NULL) {
    NewPacket->OperationCount = Operations;
  }

  return NewPacket;
}

/**
  Delete I2C request packet.

  @param[in] Packet             I2C request packet that you want to delete.

**/
VOID
PssMonzaDeleteRequestPacket (
  IN EFI_I2C_REQUEST_PACKET *Packet
  )
{
  if (Packet != NULL) {
    FreePool (Packet);
  }
}

/**
  Read data from PSS chip.

  @param[in]   I2cIoProtocol           Pointer of EFI_I2C_IO_PROTOCOL.
  @param[out]  Buffer                  Return read data from PSS chip
  @param[in]   Address                 Address located in PSS chip
  @param[in]   Size                    Data Size

  @retval  EFI_DEVICE_ERROR            Fail to QueueRequest I2C device
  @retval  EFI_INVALID_PARAMETER       Buffer is NULL
  @retval  EFI_OUT_OF_RESOURCES        Don't have enough to create I2C request packet.
  @retval  EFI_SUCCESS                 Read data from PSS chip successfully
**/
EFI_STATUS
EFIAPI
PssMonzaReadData (
  IN  EFI_I2C_IO_PROTOCOL *I2cIoProtocol,
  OUT UINT8               *Buffer,
  IN  UINT32              Address,
  IN  UINTN               Size
  )
{
  UINT8                  WriteBuffer[2];
  EFI_I2C_REQUEST_PACKET *RequestPacket;
  EFI_STATUS             Status;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  RequestPacket = NULL;

  if (Size == 0) {
    Status = EFI_SUCCESS;
    goto End;
  }

  if (Buffer == NULL) {
    DEBUG ((DEBUG_INFO, "I2C check input Buffer Failed, Status = %r\n", EFI_INVALID_PARAMETER));
    Status = EFI_INVALID_PARAMETER;
    goto End;
  }

  WriteBuffer[0] = (UINT8) (Address >> 8);
  WriteBuffer[1] = (UINT8) (Address & 0xFF);

  RequestPacket = PssMonzaNewRequestPacket (2);
  if (RequestPacket == NULL) {
    DEBUG ((DEBUG_INFO, "I2C PssMonzaNewRequestPacket Failed, Status = %r\n", EFI_OUT_OF_RESOURCES));
    Status = EFI_OUT_OF_RESOURCES;
    goto End;
  }

  RequestPacket->Operation[0].Flags         = 0;
  RequestPacket->Operation[0].LengthInBytes = sizeof (WriteBuffer);
  RequestPacket->Operation[0].Buffer        = WriteBuffer;
  RequestPacket->Operation[1].Flags         = I2C_FLAG_READ;
  RequestPacket->Operation[1].LengthInBytes = (UINT32) Size;
  RequestPacket->Operation[1].Buffer        = Buffer;
  Status = I2cIoProtocol->QueueRequest (
                            I2cIoProtocol,
                            0,
                            NULL,
                            RequestPacket,
                            NULL
                            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "I2C QueueRequest Failed, Status = %r\n", Status));
    Status = EFI_DEVICE_ERROR;
    goto End;
  }

End:
  PssMonzaDeleteRequestPacket (RequestPacket);
  DEBUG ((DEBUG_INFO, "%a End, Status = %r\n", __FUNCTION__, Status));
  return Status;
}

/**
  Check PSS Monza chip whether exists.

  @param[in] This                     Pointer of EFI_I2C_IO_PROTOCOL.

  @retval  TRUE                       Detect PSS Monza chip successfully.
  @retval  FALSE                      Unable to detect PSS Monza chip.
**/
BOOLEAN
EFIAPI
IsPssMonzaExist (
  IN EFI_I2C_IO_PROTOCOL *I2cIoProtocol
  )
{
  EFI_STATUS    Status;
  UINT8         PssData[4];
  BOOLEAN       IsChipDetect;

  IsChipDetect = FALSE;
  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  //
  // Read the chip's Class ID from the TID bank, it should be 0xE2 (Gen2)
  // Check whether the PSS IC is Monza X-8K
  //
  ZeroMem (PssData, sizeof (PssData));
  Status = PssMonzaReadData (
             I2cIoProtocol,
             &PssData[0],
             PSS_BASE_ADDR_CLASSID,
             sizeof (PssData)
             );
  DEBUG ((
    DEBUG_INFO,
    "Get PSS Class ID: %2X-%2X-%2X-%2X\n",
    PssData[0],
    PssData[1],
    PssData[2],
    PssData[3]
    ));

  if (!EFI_ERROR (Status)) {
    if ((PssData[0] == PSS_CHIP_CLASS_ID) &&
        ((PssData[2] & 0x0F) == PSS_CHIP_TID_MODEL_HIGH) &&
        (PssData[3] == PSS_CHIP_TID_MODEL_LOW)) {
      DEBUG ((DEBUG_INFO, "PSS (Monza X-8K) detected\n"));
      IsChipDetect = TRUE;
    }
  }
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));

  return IsChipDetect;
}
/**
  Cheks whether the device ID is supported by the driver

  @param[in]    DeviceId          The Device ID of the evaluted handle
  @retval       TRUE              If the device ID is supported
  @retval       FALSE             otherwise.
**/
BOOLEAN
CheckSupportedDeviceId (
  IN UINT16 DeviceId
  )
{
  struct SUPPORTED_IDS* Id;
  for (Id = (struct SUPPORTED_IDS*)SupportedDeviceId; Id->DeviceId; Id++) {
    if (DeviceId == Id->DeviceId)
    return TRUE;
  }
  return FALSE;
}

/**
  This funcion gets all the system handles from system map,
  and searches for a supported device and vendor Ids.

  @param[in]         HandleCount           A pointer to the number of handles that were got from LocateHandleBuffer.
  @param[in]         Handles               A pointer to the buffer to return the PciIo protocol handles.
  @param[out]        Adapter               A pointer to the Adapter instance.
  @param[out]        PciConfigHeader       A pointer to the pci config header to be filled in case of success.

  @retval       EFI_SUCCESS                If a supported PCI device was found.
  @retval       EFI_NOT_FOUND              If none of the PCI devices are supported by the capsule.
**/
STATIC
EFI_STATUS
FindGbeHandle (
  IN UINTN* HandleCount,
  IN EFI_HANDLE* Handles,
  OUT ADAPTER_INFO* Adapter,
  OUT PCI_CONFIG_HEADER* PciConfigHeader
  )
{
  EFI_STATUS Status;
  for (UINT32 i = 0; i < *HandleCount; i++) {
    Status = gBS->HandleProtocol (
             Handles[i],
             &gEfiPciIoProtocolGuid,
             (VOID *)&Adapter->PciIo
             );
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO,"HandleProtocol of gEfiPciIoProtocolGuid failed, Status=%r\n", Status));
      continue;
    }
    Status = Adapter->PciIo->Pci.Read(
             Adapter->PciIo,
             EfiPciIoWidthUint32,
             0,
             MAX_PCI_CONFIG_LEN,
             Adapter->PciConfig
             );
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO,"Pci.Read failed, Status=%r\n", Status));
      return Status;
    }
    PciConfigHeader = (PCI_CONFIG_HEADER*)Adapter->PciConfig;
    Adapter->Hw.VendorId = PciConfigHeader->VendorID;
    Adapter->Hw.DeviceId = PciConfigHeader->DeviceID;
    Adapter->Hw.HwAddr = (UINT8*)(UINTN)PciConfigHeader->BaseAddressReg_0;

    if (Adapter->Hw.VendorId != 0x8086)
    continue;
    if (CheckSupportedDeviceId (Adapter->Hw.DeviceId)) {
      DEBUG ((DEBUG_INFO,"Found supported device ID: 0x%x\n", Adapter->Hw.DeviceId));
      DEBUG ((DEBUG_INFO, "Adapter->Hw.HwAddr %x\n ",Adapter->Hw.HwAddr));
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
  This funcion initializes the Hw and Adapter structs.

  @param[in]      HandleCount           A pointer to the number of handles that were got from LocateHandleBuffer.
  @param[in]      Handles               A pointer to the buffer to return the PciIo protocol handles.
  @param[out]     Adapter               A pointer to the Adapter instance.

  @retval         EFI_SUCCESS           If the structs were initialized successfully.
**/
STATIC
EFI_STATUS
InitAdapterHandle (
  IN UINTN*         HandleCount,
  IN EFI_HANDLE*    Handles,
  OUT ADAPTER_INFO* Adapter
  )
{
  EFI_STATUS   Status;
  PCI_CONFIG_HEADER   PciConfigHeader;
  UINT64   Attributes;
  Status = FindGbeHandle (HandleCount, Handles, Adapter, &PciConfigHeader);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO,"No GBE Supported device was found\n"));
    return Status;
  }
  Adapter->Hw.Back = Adapter;
  Adapter->Hw.FlashAddr = Adapter->Hw.HwAddr + GBE_FW_UPDATE_FLASH_BASE_ADDR;
  // The NVM is divided into 2 Banks, which means that the size of each Bank is
  // half of the NVM size

  Adapter->Hw.FlashBankSize = (GetNVMSize (&Adapter->Hw)) / 2;
  // Adjust to word Count
  Adapter->Hw.FlashBankSize /= sizeof(UINT16);
  // Get the PCI Command options that are supported by this controller
  Status = Adapter->PciIo->Attributes (
           Adapter->PciIo,
           EfiPciIoAttributeOperationSupported,
           0,
           &Attributes
           );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO,"Attributes get failed, Status=%r\n", Status));
    return Status;
  }
  // Set the PCI Command options to enable device memory mapped IO,
  // port IO, and bus mastering.

  Status = Adapter->PciIo->Attributes (
           Adapter->PciIo,
           EfiPciIoAttributeOperationEnable,
           Attributes & (EFI_PCI_DEVICE_ENABLE | EFI_PCI_IO_ATTRIBUTE_DUAL_ADDRESS_CYCLE),
           NULL
           );
  return Status;
}

/**
  This function finds the supported device ID, initalizes the
  hardware to firmware update process and initializes ADAPTER_INFO and hardware
  structures.

  @param[in]     ImageHandle               Handle to Image for FMP.
  @param[in]     SystemTable               A pointer to all the system devices

  @return        Status                    The status for the FMP process.
**/
VOID
EFIAPI
GbeFwUpdate (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  ADAPTER_INFO* Adapter;
  EFI_STATUS        Status;
  EFI_HANDLE*       Handles;
  UINTN             HandleCount;
  SETUP_DATA        SystemConfiguration;
  UINTN             VarSize;
  UINT16            Image_Pss[PSS_CHIP_MAC_ADDRESS_LENGTH / 2];
  UINT16            Image[PSS_CHIP_MAC_ADDRESS_LENGTH / 2];
  UINTN             ImageSize;
  UINT16            ReadData_Pss[PSS_CHIP_MAC_ADDRESS_LENGTH / 2];
  UINT16            ReadData[PSS_CHIP_MAC_ADDRESS_LENGTH / 2];
  UINT32            VariableAttributes;
  I2C_PLATFORM_SECURE_STORAGE_PROTOCOL  *I2cPssProtocol;
  EFI_I2C_IO_PROTOCOL           *I2cIoProtocol;
  PSS_MONZA_PRIVATE_CONTEXT     *PssMonzaContext;
  UINT16 length = PSS_CHIP_MAC_ADDRESS_LENGTH / 2;
  UINT32            RAH0[PSS_CHIP_MAC_ADDRESS_LENGTH / 2];
  UINT64            GbeBar;
  UINT32            Value;

  DEBUG ((DEBUG_INFO, "\nGbeFwUpdateEntryPoint\n"));
  SystemConfiguration.GbeFwUpdateEnable = TRUE;
  Status = gBS->LocateProtocol (
                &gI2cPssProtocolGuid,
                NULL,
                (VOID **) &I2cPssProtocol
                );
  DEBUG ((DEBUG_INFO, "Locate I2cPssProtocol - %r\n", Status));
  if (!EFI_ERROR (Status)) {
    gBS->CloseEvent (Event);
    PssMonzaContext = PSS_MONZA_CONTEXT_FROM_PROTOCOL (I2cPssProtocol);

    if (PssMonzaContext->I2cIoProtocol == NULL) {
      Status = EFI_INVALID_PARAMETER;
      DEBUG ((DEBUG_INFO, "%a End, Status = %r\n", __FUNCTION__, Status));
      return;
    }

    I2cIoProtocol = PssMonzaContext->I2cIoProtocol;

    Status = IsPssMonzaExist(I2cIoProtocol);
    if (!EFI_ERROR (Status)) {
      VarSize = sizeof (SETUP_DATA);
      Status = gRT->GetVariable (
          L"Setup",
          &gSetupVariableGuid,
          &VariableAttributes,
          &VarSize,
          &SystemConfiguration);
      if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO, "GetVariable of gSetupVariableguid failed, Status=%r\n", Status));
        return;
      }

      if (SystemConfiguration.GbeFwUpdateEnable == TRUE) {
        Status = I2cPssProtocol->PssGetMacAddress (I2cPssProtocol, Image);
        //
        // Byte swapping
        //
        Status = ByteSwapping_call(Image, length, Image_Pss);

        if (!EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO,"Mac Address from PSS Chip start (Normal order): %x %x %x\n",Image_Pss[0], Image_Pss[1], Image_Pss[2]));
        } else {
          DEBUG ((DEBUG_ERROR,"Reading PSS Data Failed\n"));
          return;
        }
      } else {
        DEBUG ((DEBUG_INFO, "GbeFwUpdateSupport is Disabled\n"));
        return;
      }
    } else {
        DEBUG ((DEBUG_ERROR, "PSS detect Failed : %r\n", Status));
        return;
    }
 }

  Status = gBS->LocateHandleBuffer (
           ByProtocol,
           &gEfiPciIoProtocolGuid,
           NULL,
           &HandleCount,
           &Handles
           );
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "LocateHandleBuffer of gEfiPciIoProtocolGuid failed, Status=%r\n", Status));
    return;
  }
  Adapter = AllocateRuntimeZeroPool(sizeof(ADAPTER_INFO));
  if (!Adapter) {
    Status = EFI_OUT_OF_RESOURCES;
    DEBUG ((DEBUG_INFO, "AllocateRuntimeZeroPool failed, Status=%r\n", Status));
    return;
  }
  Status = InitAdapterHandle (&HandleCount, Handles, Adapter);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Init Adapter Handle failed, Status=%r\n", Status));
    goto free_pool;
  }

  DEBUG ((DEBUG_INFO,"GbeFwUpdateSupport\n"));
  ReadNVM (&Adapter->Hw,
          0x00,//offset 0 has value 8888, offset 1 has 8888 and offset 2 has 8887
          3,
          ReadData
          );
  //
  // Byte swapping
  //
  Status = ByteSwapping_call(ReadData, length, ReadData_Pss);

  DEBUG ((DEBUG_INFO, "SystemConfiguration.GbeFwUpdateEnable %x\n",SystemConfiguration.GbeFwUpdateEnable));
  if (SystemConfiguration.GbeFwUpdateEnable == TRUE) {
    DEBUG ((DEBUG_INFO,"Default Mac from NVM in normal order %x %x %x\n",ReadData_Pss[0], ReadData_Pss[1], ReadData_Pss[2]));
    DEBUG ((DEBUG_INFO,"Default Mac from NVM in reverse order %x %x %x\n",ReadData[0], ReadData[1], ReadData[2]));
    if (Image[0] == ReadData[0] && Image[1] == ReadData[1] && Image[2] == ReadData[2]){
      //check if the data is same as existing data
      DEBUG ((DEBUG_INFO, "The data read from the PSS Chip is same as the data existing in it\n"));
      goto free_pool;
    }

    if (Image[0] == 0 && Image[1] == 0 && Image[2] == 0){
      //check if it is equal to 0
      DEBUG ((DEBUG_INFO, "The data read from PSS Chip is equal to 0\n"));
      goto free_pool;
    }
  }

  //
  // Defining size of Pss data as 3 words
  //
  ImageSize = sizeof (Image_Pss) / sizeof (UINT16);
  DEBUG ((DEBUG_INFO, "Size of the Image %x\n", ImageSize));
  Status = WriteEepromImage (Adapter, Image, ImageSize, TRUE);
  if(Status){
    DEBUG ((DEBUG_INFO,"Flashing EEPROM Failed\n"));
    goto free_pool;
  }
  SystemConfiguration.GbeFwUpdateEnable = FALSE;
  VarSize = sizeof(SETUP_DATA);
  Status = gRT->SetVariable (
           L"Setup",
           &gSetupVariableGuid,
           VariableAttributes,
           VarSize,
           &SystemConfiguration
           );
  DEBUG ((DEBUG_INFO, "SystemConfiguration.GbeFwUpdateEnable %x\n",SystemConfiguration.GbeFwUpdateEnable));

  //
  // Calling UpdateNVM to ensure we are on valid bank i.e bank 1
  // So the data can be updated to GBE region
  //
  Status = UpdateNVM (&Adapter->Hw);
  if (Status) {
    goto free_pool;
  }

  // Updating the RAL0 and RAH0 registers for CSME to get intrrupt and read the MAC address.
  //

  DEBUG((DEBUG_INFO, "Reading the data present in RAL0 and RAH0\n"));

  if (PciSegmentRead32 (GbePciCfgBase ()) != 0xFFFFFFFF) {
    GbeBar = PciSegmentRead32 (GbePciCfgBase () + PCI_BASE_ADDRESSREG_OFFSET) & 0xFFFFFFF0;
    if ((PciSegmentRead32 (GbePciCfgBase () + PCI_BASE_ADDRESSREG_OFFSET) & B_PCI_BAR_MEMORY_TYPE_MASK) == B_PCI_BAR_MEMORY_TYPE_64) {
      GbeBar += LShiftU64 (PciSegmentRead32 (GbePciCfgBase () + (PCI_BASE_ADDRESSREG_OFFSET + 4)), 32);
    }
    DEBUG ((DEBUG_INFO, "GbeBar = 0x%x\n", GbeBar));
    if (GbeBar == 0) {
      DEBUG ((DEBUG_ERROR, "MMIO Bar for GbE device isn't programmed\n"));
      goto free_pool;
    }

    //
    // Write to Gbe register RAL0
    //
    Value = (Image[1] << 16) + Image[0];
    MmioWrite32 (GbeBar + R_GBE_MEM_RAL_0, Value);

    //
    // Write to Gbe register RAH0
    //
    RAH0[0] = MmioRead32 (GbeBar + R_GBE_MEM_RAH_0);
    DEBUG((DEBUG_INFO, "Data present in RAH0 %x\n", RAH0[0]));

    Value = RAH0[0] & 0xFFFF0000;
    Value = Value | Image[2];
    DEBUG ((DEBUG_INFO,"Value %x\n", Value));
    MmioWrite32 (GbeBar + R_GBE_MEM_RAH_0, Value);

    //
    // Read the gbe register RAL0.
    //
    DEBUG ((DEBUG_INFO, "Data present in RAL0 after write to registers %x\n", MmioRead32 (GbeBar + R_GBE_MEM_RAL_0)));

    //
    // Read the gbe register RAH0.
    //
    DEBUG ((DEBUG_INFO, "Data present in RAH0 after write to  registers %x\n", MmioRead32 (GbeBar + R_GBE_MEM_RAH_0)));
  }


free_pool:
  FreePool (Adapter);
  FreePool (Handles);
  return;
}

/**
  Entry point for Gbe Fw update Driver.

  @param ImageHandle  -  A handle for the image that is initializing this driver.
  @param SystemTable  -  A pointer to the EFI system table.

  @retval EFI_SUCCESS           -  Driver initialized successfully.
  @retval EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  @retval EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.

**/
EFI_STATUS
EFIAPI
GbeFwUpdateEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
)
{
  VOID                                  *Registration;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  EfiCreateProtocolNotifyEvent (
    &gI2cPssProtocolGuid,
    TPL_CALLBACK,
    GbeFwUpdate,
    NULL,
    &Registration
    );

  return EFI_SUCCESS;
}

/**
  This function is used to swap the bytes.

  @param[in]    MacData              Pointer to the MAC address read from NVM region or PSS chip.
  @param[in]    length               The length of the MAC address.
  @param[out]   Mac_NormalOrder      Pointer to the Mac address to be printed in normal order.

  @return       Status                   The return status for this function.
*/
EFI_STATUS
ByteSwapping_call (
  IN UINT16  *MacData ,
  IN UINT16   length,
  OUT UINT16 *Mac_NormalOrder
  )
{
  UINT16 i = 0;
  EFI_STATUS   Status = EFI_SUCCESS;
  //
  // Byte swapping
  //
  for (i = 0; i < length; i++) {
    Mac_NormalOrder[i] = ((MacData[i] << 8) & 0xff00) | ((MacData[i] >> 8) & 0x00ff);
  }
  return Status;
}