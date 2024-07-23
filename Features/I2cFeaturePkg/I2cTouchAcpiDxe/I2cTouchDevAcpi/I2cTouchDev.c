/**@file

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

#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <I2cDevices.h>
#include <I2cCommon.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

/**
  Function used to fill I2c Buffer with I2C Rtd3 Structure.
  @param[in] Count            Maximum Pch I2C ports.
  @param[in] I2C_ACPI_DEV     I2C ACPI device structure.

**/
EFI_STATUS
EFIAPI
I2CBoardData (
  IN      UINT8                    Count,
  IN      I2C_ACPI_DEV             *I2cControllerInfo,
  IN      UINT8                     Rtd3Support,
  IN      I2C_DEVICES              *I2cTPTD
  );

I2C_DEVICES_CONFIG_PROTOCOL I2CDevicesBoardConfigData = {
  I2CBoardData
};

/**
  Fetch for I2c Devices Buffer and patch the Board Data accordingly.

  @param[in] I2cDevTable              I2c Device Table
  @param[in] I2cTPTD        I2c Devices Data Structure
**/
VOID
PatchI2cDevicesBuffer(
  EFI_ACPI_DESCRIPTION_HEADER    *I2cDevTable,
  I2C_DEVICES                    *I2cTPTD
)
{
  UINT8                           *I2CStartPointer;
  UINT8                           *I2cEndPointer;
  UINT32                          *Signature;
  UINT8                           *Operation;
  UINT8                           *DataPtr;
  DEBUG ((DEBUG_INFO, "Entering PatchI2cDevicesBuffer\n"));

  I2CStartPointer = (UINT8 *) I2cDevTable + sizeof (EFI_ACPI_DESCRIPTION_HEADER);
  //I2c EndPointer - sizeof (signature)-size(opcode)-size_of_least(package) which is -4-1-1
  I2cEndPointer   = (UINT8 *) I2cDevTable + I2cDevTable->Length - 6;

  DEBUG ((DEBUG_INFO, "I2cDevTable=%x\n",I2cDevTable));

  for (; I2CStartPointer < I2cEndPointer; I2CStartPointer++) {
    Signature = (UINT32 *) (I2CStartPointer + 1);
    //DEBUG((DEBUG_INFO,"Entering the for loop after Signature check for TableID\n"));
    switch(*Signature){
        case(SIGNATURE_32 ('T','P','T','D')):
        //DEBUG((DEBUG_INFO,"Signature check for buffer TPTD sucess\n"));
        if (*I2CStartPointer == AML_NAME_OP){
            DEBUG ((DEBUG_INFO, "1I2CStartPointer =%x \n",I2CStartPointer));
            Operation = I2CStartPointer + 8;
            if(*Operation == AML_BYTE_PREFIX){                                         
                DataPtr = I2CStartPointer + 9;
                CopyMem (DataPtr, &I2cTPTD->TouchPanelNvsVar.TouchPanelType ,sizeof (UINT8));
            }
            Operation = I2CStartPointer + 10;
            if(*Operation == AML_BYTE_PREFIX){
                DataPtr = I2CStartPointer + 11;
                CopyMem (DataPtr, &I2cTPTD->TouchPanelNvsVar.TouchPanelIrqMode,sizeof (UINT8));
            }
            Operation = I2CStartPointer + 12;
            if(*Operation == AML_BYTE_PREFIX){
                DataPtr = I2CStartPointer + 13;
                CopyMem (DataPtr, &I2cTPTD->TouchPanelNvsVar.TouchPanelBusAddress ,sizeof (UINT8));
            }
            Operation = I2CStartPointer + 14;
            if(*Operation == AML_BYTE_PREFIX){
                DataPtr = I2CStartPointer + 15;
                CopyMem (DataPtr, &I2cTPTD->TouchPanelNvsVar.TouchPanelSpeed,sizeof (UINT8));
            }
            Operation = I2CStartPointer + 16;
            if(*Operation == AML_BYTE_PREFIX){
                DataPtr = I2CStartPointer + 17;
                CopyMem (DataPtr, &I2cTPTD->TouchPadNvsVar.TouchPadType,sizeof (UINT8));
            }
            Operation = I2CStartPointer + 18;
            if(*Operation == AML_BYTE_PREFIX){
                DataPtr = I2CStartPointer + 19;
                CopyMem (DataPtr, &I2cTPTD->TouchPadNvsVar.TouchPadIrqMode ,sizeof (UINT8));
            }
            Operation = I2CStartPointer + 20;
            if(*Operation == AML_BYTE_PREFIX){
                DataPtr = I2CStartPointer + 21;
                CopyMem (DataPtr, &I2cTPTD->TouchPadNvsVar.TouchPadBusAddress ,sizeof (UINT8));
            }
            Operation = I2CStartPointer + 22;
            if(*Operation == AML_BYTE_PREFIX){
                DataPtr = I2CStartPointer + 23;
                CopyMem (DataPtr, &I2cTPTD->TouchPadNvsVar.TouchPadSpeed ,sizeof (UINT8));
            }            
        }
        break;
        case(SIGNATURE_32 ('T','D','P','H')):
        //DEBUG((DEBUG_INFO,"Signature check for buffer TDPH sucess\n"));
        if (*I2CStartPointer == AML_NAME_OP){
            DEBUG ((DEBUG_INFO, "2I2CStartPointer =%x \n",I2CStartPointer));            
            Operation = I2CStartPointer + 8;
            if(*Operation == AML_WORD_PREFIX){
                DataPtr = I2CStartPointer + 9;
                CopyMem (DataPtr, &I2cTPTD->TouchPanelNvsVar.TouchPanelHidAddress ,sizeof (UINT16));
            }
            Operation = I2CStartPointer + 11;
            if(*Operation == AML_WORD_PREFIX){
                DataPtr = I2CStartPointer + 12;
                CopyMem (DataPtr, &I2cTPTD->TouchPadNvsVar.TouchPadHidAddress,sizeof (UINT16));
            }
        }
        break;
        case(SIGNATURE_32 ('T','P','D','I')):
        //DEBUG((DEBUG_INFO,"Signature check for buffer TPDI sucess\n"));
        if (*I2CStartPointer == AML_NAME_OP){
            DEBUG ((DEBUG_INFO, "3I2CStartPointer =%x \n",I2CStartPointer));           
            Operation = I2CStartPointer + 8;
            if(*Operation == AML_DWORD_PREFIX){
                DataPtr = I2CStartPointer + 9;
                CopyMem (DataPtr, &I2cTPTD->TouchPadNvsVar.TouchpadIrqGpio ,sizeof (UINT32));
            }
            Operation = I2CStartPointer + 13 ;
            if(*Operation == AML_DWORD_PREFIX){
                DataPtr = I2CStartPointer + 14;
                CopyMem (DataPtr, &I2cTPTD->TouchPanelNvsVar.TouchpanelIrqGpio,sizeof (UINT32));
            }
            Operation = I2CStartPointer + 18;
            if(*Operation == AML_DWORD_PREFIX){ 
                DataPtr = I2CStartPointer + 19;
                CopyMem (DataPtr, &I2cTPTD->TouchPanelNvsVar.Touchpanel0IrqGpio,sizeof (UINT32));
            }
        }
        break;

    }

  }
DEBUG((DEBUG_INFO,"I2cdevices patch completed\n"));
}

/**
  Function which print I2c Acpi struct value.

  @param[in] Count            Maximum Pch I2C ports.
  @param[in] I2C_ACPI_DEV     I2C ACPI device structure.

**/
VOID
PrintRtd3I2cStructData (
  IN      UINT8                    Count,
  IN      I2C_ACPI_DEV             *I2cControllerInfo
  )
{

  UINT8 I2CIndex;
  DEBUG ((DEBUG_INFO, "integrated board data count =%x\n",Count));
  for (I2CIndex = 0; I2CIndex < Count; I2CIndex++) {
    if (I2CIndex == I2cControllerInfo[I2CIndex].I2cControllerTouchIndex ) {
      DEBUG ((DEBUG_INFO, "I2c Struct I2cControllerEnabled =%x\n",I2cControllerInfo[I2CIndex].I2cControllerEnabled));
      DEBUG ((DEBUG_INFO, "I2c Struct TouchPadnterruptGpio =%x\n",I2cControllerInfo[I2CIndex].TouchPadInfo.TouchPadInterruptGpio));
      DEBUG ((DEBUG_INFO, "I2c Struct TouchPadInterruptGpioPolarity =%x\n",I2cControllerInfo[I2CIndex].TouchPadInfo.TouchPadInterruptGpioPolarity));
      DEBUG ((DEBUG_INFO, "I2c Struct TouchPanelInterruptGpio =%x\n",I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchPanelInterruptGpio));
      DEBUG ((DEBUG_INFO, "I2c Struct TouchPanelInterruptGpioPolarity =%x\n",I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchPanelInterruptGpioPolarity));
      DEBUG ((DEBUG_INFO, "I2c Struct TouchpanelPwrGpio =%x\n",I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchpanelPwrGpio));
      DEBUG ((DEBUG_INFO, "I2c Struct TouchpanelPwrGpioPolarity =%x\n",I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchpanelPwrGpioPolarity));
      DEBUG ((DEBUG_INFO, "I2c Struct TouchPanelResetGpio =%x\n",I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchPanelResetGpio));
      DEBUG ((DEBUG_INFO, "I2c Struct TouchPanelResetGpioPolarity =%x\n",I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchPanelResetGpioPolarity));
      DEBUG ((DEBUG_INFO, "I2c Struct I2cDeviceType =%x\n",I2cControllerInfo[I2CIndex].I2cDeviceType));
    }
  }
}

/**
  Fetch for I2c Rtd3 Buffer and patch the Board Data accordingly.

  @param[in] I2cDevTable              I2c Device Table
  @param[in] I2cControllerInfo        I2c Controller Board Data Structure
  @param[in] Count                    Maximum Pch I2C ports.
**/
VOID
PatchRtd3Buffer (
  EFI_ACPI_DESCRIPTION_HEADER    *I2cDevTable,
  I2C_ACPI_DEV                   *I2cControllerInfo,
  UINT8                          Count
  )
{
  UINT8                           *I2CStartPointer;
  UINT8                           *I2cEndPointer;
  UINT32                          *Signature;
  UINT8                           I2CIndex;

  I2CStartPointer = (UINT8 *)I2cDevTable;
  //I2c EndPointer - sizeof (signature)-size(opcode)-size_of_least(package) which is -4-1-1
  I2cEndPointer = I2CStartPointer + I2cDevTable->Length -6;
  for (; I2CStartPointer <= I2cEndPointer; I2CStartPointer++) {
    Signature = (UINT32 *) (I2CStartPointer + 1);
    if ((*I2CStartPointer == AML_NAME_OP) && (*Signature == SIGNATURE_32 ('I','2','B','U'))) {
      I2CStartPointer = I2CStartPointer + I2C_BUFFER_START_LOCATION;
      for (I2CIndex =0; I2CIndex < Count ; I2CIndex ++) {
        if (I2CIndex == I2cControllerInfo[I2CIndex].I2cControllerTouchIndex ) {
          *( (UINT8  *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + I2C_ENABLE_BYTE_OFFSET))) = I2cControllerInfo[I2CIndex].I2cControllerEnabled;
          *( (UINT8  *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + END_POINT_PRESENT_BYTE_OFFSET))) = I2cControllerInfo[I2CIndex].I2cDeviceType;
          *( (UINT32 *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + TOUCH_PAD_INTERRUPT_GPIO_DWORD_OFFSET))) = I2cControllerInfo[I2CIndex].TouchPadInfo.TouchPadInterruptGpio;
          *( (UINT8  *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + TOUCH_PAD_INTERRUPT_POLARITY_BYTE_OFFSET))) = I2cControllerInfo[I2CIndex].TouchPadInfo.TouchPadInterruptGpioPolarity;
          *( (UINT32 *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_INTERRUPT_GPIO_DWORD_OFFSET))) = I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchPanelInterruptGpio;
          *( (UINT8  *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_INTERRUPT_POLARITY_BYTE_OFFSET))) = I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchPanelInterruptGpioPolarity;
          *( (UINT32 *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_POWER_GPIO_DWORD_OFFSET))) = I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchpanelPwrGpio;
          *( (UINT8  *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_POWER_POLARITY_BYTE_OFFSET))) = I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchpanelPwrGpioPolarity;
          *( (UINT32 *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_RESET_GPIO_DWORD_OFFSET))) = I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchPanelResetGpio;
          *( (UINT8  *) (I2CStartPointer  +  ((I2CIndex * I2C_BUFFER_SIZE) + TOUCH_PANEL_RESET_POLARITY_BYTE_OFFSET))) = I2cControllerInfo[I2CIndex].TouchPanelInfo.TouchPanelResetGpioPolarity;
        }
      }
    }
  }
}

/**
  Function used to fill data into I2c Buffer from I2c Rtd3 structure.

  @param[in] Count            Maximum Pch I2C ports.
  @param[in] I2C_ACPI_DEV     I2C ACPI device structure.

**/
EFI_STATUS
EFIAPI
I2CBoardData (
  IN      UINT8                    Count,
  IN      I2C_ACPI_DEV             *I2cControllerInfo,
  IN      UINT8                    Rtd3Support,
  IN      I2C_DEVICES             *I2cTPTD
  )
{

  EFI_ACPI_DESCRIPTION_HEADER   *I2cDevTable;
  TABLE_POINTER_ADDR_PROTOCOL   *TableProtocol;
  EFI_STATUS                     Status;

  DEBUG ((DEBUG_INFO," I2cControllerInfo rtd3 =%x \n",I2cControllerInfo));
  PrintRtd3I2cStructData (Count,I2cControllerInfo);

  Status = gBS->LocateProtocol (&gI2cDeviceProtocolGuid, NULL, (VOID **)&TableProtocol);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  PcdSet64S (PcdI2cDevTableName, SIGNATURE_64 ('I', '2', 'C', '_','D','E','V','T'));
  I2cDevTable = TableProtocol->GetDevTablePointer();
  if(I2cDevTable != NULL) {
    PatchI2cDevicesBuffer(I2cDevTable,I2cTPTD);
    if (Rtd3Support == 1) {
      PatchRtd3Buffer (I2cDevTable,I2cControllerInfo,Count);
    }
  }
  else{
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO," i2crtd3 status return \n"));
  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
I2cDevPatchEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                Status;

  //
  // Install I2c Protocol.
  //
  Status = gBS->InstallProtocolInterface (
          &ImageHandle,
          &gI2cDeviceInfoProtocolGuid,
          EFI_NATIVE_INTERFACE,
          &I2CDevicesBoardConfigData
          );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Status=%x \n",Status));
  }

  return Status;
}
