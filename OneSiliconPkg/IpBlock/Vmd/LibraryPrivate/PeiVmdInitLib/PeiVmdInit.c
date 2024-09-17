/** @file
  The PEI VMD Init Library in Si init

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

#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PchPcieRpLib.h>
#include <Library/PeiVmdInitLib.h>
#include <VmdInfoHob.h>
#include <Register/VmdRegs.h>
#include <Library/VmdInfoLib.h>
#include <Library/PeiVmdInitFruLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Defines/HostBridgeDefines.h>

EFI_PEI_PPI_DESCRIPTOR  gVmdInitDonePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gVmdInitDonePpiGuid,
  NULL
};

/**
  This function compares VMD OS variable input with current platform info and map the matching RP under VMD.
  @param[in]  *VmdInfoHob       pointer to VMD info Hob to store the information of detected devices.
  @param[in]  EFI_VMD_OS_DATA   pointer to VmdVariable

**/
VOID
MapVmdOsVariable (
    VMD_INFO_HOB               *VmdInfoHob,
    EFI_VMD_OS_DATA            *VmdVariablePtr
  )
{
  UINT16       Index;
  UINT8       HobIndex;

  DEBUG ((DEBUG_INFO, "Dumping Vmd OS Variable details\n"));
  DEBUG ((DEBUG_INFO, "VMD Request Entry Count is %d\n", VmdVariablePtr->BYTE0_1.VREC));
  for (Index = 0; Index < VmdVariablePtr->BYTE0_1.VREC; ++Index) {
    DEBUG ((DEBUG_INFO, "Storage device Rp B/D/F and RpIndex is %d/%d/%d and %d\n", VmdVariablePtr->DevDetails[Index].RpBus,
        VmdVariablePtr->DevDetails[Index].RpDevice, VmdVariablePtr->DevDetails[Index].RpFunction, VmdVariablePtr->DevDetails[Index].RpNumber));
  }

  // Assigning devices as per user selection in OS GUI by comparing either Rp Index or Rp function.
  for (Index = 0; Index < VmdVariablePtr->BYTE0_1.VREC; ++Index) {
    for (HobIndex = 0; HobIndex < VMD_MAX_DEVICES; ++HobIndex) {
      if ((VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].DeviceDetected == 1) && (VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].PortEn != 1)) {
        if ((VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpBus == VmdVariablePtr->DevDetails[Index].RpBus) &&
           (VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpDev == VmdVariablePtr->DevDetails[Index].RpDevice) &&
           ((VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpFunc == VmdVariablePtr->DevDetails[Index].RpFunction) ||
           (VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpIndex == VmdVariablePtr->DevDetails[Index].RpNumber))) {
          VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].PortEn = 1;
          DEBUG ((DEBUG_INFO, "Mapping device under VMD as per user selection in OS GUI for B/D/F %d/%d/%d \n",
          VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpBus,VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpDev,VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpFunc));
          break;
        }
      }
    }
  }
  return;
}

/**
  This function performs basic initialization for VMD in PEI phase (post-mem)
  @param VMD_PEI_PREMEM_CONFIG - Pointer to config data
  @retval     EFI_SUCCESS     Successfully initialized VMD
  @retval     EFI_UNSUPPORTED         VMD unsupported
  @retval     EFI_INVALID_PARAMETER   If VmdPeiConfig is null, return invalid parameter
**/
EFI_STATUS
EFIAPI
VmdInit (
  IN   VMD_PEI_CONFIG  *VmdPeiConfig
  )
{
  CFGBARSZ_IOC_VMD_STRUCT       VmdCfgBarSize;
  MEMBAR1SZ_IOC_VMD_STRUCT      VmdMemBar1Size;
  MEMBAR2SZ_IOC_VMD_STRUCT      VmdMemBar2Size;
  VMASSIGN_IOC_VMD_STRUCT       VmAssign;
  CFGBAR_IOC_VMD_STRUCT         CfgBar;
  MEMBAR1_IOC_VMD_STRUCT        MemBar1;
  MEMBAR2_IOC_VMD_STRUCT        MemBar2;
  VMCONFIG_IOC_VMD_STRUCT       VmConfig;
  VMCAP_IOC_VMD_STRUCT          VmCap;

  FN0_ASSIGN_IOC_VMD_STRUCT     Fn0Assign;
  FN1_ASSIGN_IOC_VMD_STRUCT     Fn1Assign;
  FN2_ASSIGN_IOC_VMD_STRUCT     Fn2Assign;
  FN3_ASSIGN_IOC_VMD_STRUCT     Fn3Assign;
  FN4_ASSIGN_IOC_VMD_STRUCT     Fn4Assign;
  FN5_ASSIGN_IOC_VMD_STRUCT     Fn5Assign;
  FN6_ASSIGN_IOC_VMD_STRUCT     Fn6Assign;
  FN7_ASSIGN_IOC_VMD_STRUCT     Fn7Assign;

  UINT32                        DeviceBaseAddress;
  VMD_INFO_HOB                  *VmdInfoHob;
  UINT8                         Index;
  UINT8                         HobIndex;
  EFI_STATUS                    Status;
  BOOLEAN                       VmdDevenStatus;

  //
  // VMD Initializations if the VMD IP is Supported
  //
  if (IsVmdSupported()!= TRUE) {
    DEBUG ((DEBUG_INFO, "------- Vmd is not supported -----\n"));
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO, "-----------------VmdInit Start ----------------\n"));
  if (VmdPeiConfig == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Create HOB for VMD INFO
  //
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (VMD_INFO_HOB),
             (VOID **) &VmdInfoHob
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize default HOB data
  //
  VmdInfoHob->EfiHobGuidType.Name = gVmdInfoHobGuid;
  DEBUG ((DEBUG_INFO, "VmdInfoHob->EfiHobGuidType.Name: %g\n", &VmdInfoHob->EfiHobGuidType.Name));
  ZeroMem (&(VmdInfoHob->VmdPortInfo), sizeof (VMD_PORT_INFO));

  DEBUG ((DEBUG_INFO, "VmdInfoHob @ %X\n", VmdInfoHob));
  DEBUG ((DEBUG_INFO, "&(VmdInfoHob->VmdPortInfo) @ %X\n", &(VmdInfoHob->VmdPortInfo)));
  DEBUG ((DEBUG_INFO, "VmdHobSize - HobHeaderSize: %X\n", sizeof (VMD_INFO_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "VmdInfoSize: %X\n", sizeof (VMD_PORT_INFO)));

  //
  // Detect the mass storage devices/controller connected to CPU and PCH root ports
  //
  VmdDetectPcieStorageDevices (VmdInfoHob);

  DEBUG ((DEBUG_INFO, "VMD: Dumping port info\n"));
  for (Index = 0; Index < VMD_MAX_DEVICES; ++Index) {
    if (VmdInfoHob->VmdPortInfo.PortInfo[Index].DeviceDetected) {
      DEBUG ((DEBUG_INFO, "RpBus  %d\t", VmdInfoHob->VmdPortInfo.PortInfo[Index].RpBus));
      DEBUG ((DEBUG_INFO, "RpDev  %d\t", VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev));
      DEBUG ((DEBUG_INFO, "RpFunc  %d\t", VmdInfoHob->VmdPortInfo.PortInfo[Index].RpFunc));
      DEBUG ((DEBUG_INFO, "DevId  0x%x\t", VmdInfoHob->VmdPortInfo.PortInfo[Index].DevId));
      DEBUG ((DEBUG_INFO, "RpIndex  %d\n", VmdInfoHob->VmdPortInfo.PortInfo[Index].RpIndex));
    }
  }

  //
  // Initializations only if VMD is enabled
  //
  if (!VmdPeiConfig->VmdEnable) { // if VMD is not enabled by user
    //disable the devEnable bit for VMD device (bit 14) if its enabled
    VmdDevenStatus = UncoreDevEnRead (Vmd, 0);
    if (VmdDevenStatus == TRUE) {
      UncoreDevEnWrite (Vmd, 0, Disable);
    }
    DEBUG ((DEBUG_INFO, "\nVMD: Device has been disabled\n"));
    return EFI_SUCCESS;
  }

  DEBUG((DEBUG_INFO, "VMD: Enabled in the setup \n"));

  // Check the devEnable bit for VMD device (bit 14)
  VmdDevenStatus = UncoreDevEnRead (Vmd, 0);
  if (VmdDevenStatus == TRUE) {
    DEBUG((DEBUG_VERBOSE, "VMD device is already enabled\n"));
  } else {
    DEBUG((DEBUG_VERBOSE, "Enabling VMD device in DevEn \n"));
    UncoreDevEnWrite (Vmd, 0, Enable);
  }

  DeviceBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, VMD_BUS_NUM, VMD_DEV_NUM, VMD_FUN_NUM,0);

  //
  // Reset and then Configure FUN assign registers as per the policy / user selection of Dev and Func
  //
  Fn0Assign.Data = 0;
  PciSegmentWrite32 ( DeviceBaseAddress + FN0_ASSIGN_IOC_VMD_REG, Fn0Assign.Data);
  Fn1Assign.Data = 0;
  PciSegmentWrite32 ( DeviceBaseAddress + FN1_ASSIGN_IOC_VMD_REG, Fn1Assign.Data);
  Fn2Assign.Data = 0;
  PciSegmentWrite32 ( DeviceBaseAddress + FN2_ASSIGN_IOC_VMD_REG, Fn2Assign.Data);
  Fn3Assign.Data = 0;
  PciSegmentWrite32 ( DeviceBaseAddress + FN3_ASSIGN_IOC_VMD_REG, Fn3Assign.Data);
  Fn4Assign.Data = 0;
  PciSegmentWrite32 ( DeviceBaseAddress + FN4_ASSIGN_IOC_VMD_REG, Fn4Assign.Data);
  Fn5Assign.Data = 0;
  PciSegmentWrite32 ( DeviceBaseAddress + FN5_ASSIGN_IOC_VMD_REG, Fn5Assign.Data);
  Fn6Assign.Data = 0;
  PciSegmentWrite32 ( DeviceBaseAddress + FN6_ASSIGN_IOC_VMD_REG, Fn6Assign.Data);
  Fn7Assign.Data = 0;
  PciSegmentWrite32 ( DeviceBaseAddress + FN7_ASSIGN_IOC_VMD_REG, Fn7Assign.Data);

  // If the Global Mapping Policy is enabled then map all the detected devices under VMD
  if (VmdPeiConfig->VmdGlobalMapping) {
    DEBUG ((DEBUG_INFO, "VMD: Global Mapping policy is enabled so mapping all detected storage devices under VMD!!\n"));
    for (Index = 0; Index < VMD_MAX_DEVICES; ++Index) {
      // Check the list of detected devices and map all under VMD
      if (VmdInfoHob->VmdPortInfo.PortInfo[Index].DeviceDetected) {
        VmdInfoHob->VmdPortInfo.PortInfo[Index].PortEn = 1;
      }
    }
  } else { // Map devices based on user selection in BIOS setup and UEFI variable input from OS.
    // Check if platform configuration is same as last boot by comparing config block and hob entries
    // Apply policy from previous boot if matching B/D/F is found in current boot.
    DEBUG ((DEBUG_VERBOSE, "VMD: Check if platfrom configuration is same as last boot by comparing config block and hob entries\n"));
    for (Index = 0; Index < VMD_MAX_DEVICES; ++Index) {
      if (VmdPeiConfig->VmdPortEnable[Index].RpEnable == 1) {
        DEBUG ((DEBUG_INFO, "VMD: As per config policy, storage device Rp %d/%d/%d is mapped under VMD\n", VmdPeiConfig->VmdPortEnable[Index].RpBus,
                VmdPeiConfig->VmdPortEnable[Index].RpDevice, VmdPeiConfig->VmdPortEnable[Index].RpFunction));

        // Check if this B/D/F is still valid for the current boot
        for (HobIndex = 0; HobIndex < VMD_MAX_DEVICES; ++HobIndex) {
          if ((VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].DeviceDetected == 1) && (VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].PortEn != 1)) {
            if ((VmdPeiConfig->VmdPortEnable[Index].RpBus == VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpBus)
              &&(VmdPeiConfig->VmdPortEnable[Index].RpDevice == VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpDev)
              &&(VmdPeiConfig->VmdPortEnable[Index].RpFunction == VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].RpFunc)) {
              DEBUG ((DEBUG_INFO, "VMD: Platfrom configuration is same, applying earlier user selection!!\n"));
              VmdInfoHob->VmdPortInfo.PortInfo[HobIndex].PortEn = 1;
              break;
            }
          }
        }
      }
    }
    if (VmdPeiConfig->VmdVariablePtr != NULL) {
      MapVmdOsVariable(VmdInfoHob, VmdPeiConfig->VmdVariablePtr);
    } else {
      DEBUG((DEBUG_INFO, "VMD: OS Variable is not available \n"));
    }
  }

  for (Index = 0; Index < VMD_MAX_DEVICES; ++Index) {
    if ((VmdInfoHob->VmdPortInfo.PortInfo[Index].DeviceDetected) && (VmdInfoHob->VmdPortInfo.PortInfo[Index].RpBus == 0)) {
      if (VmdInfoHob->VmdPortInfo.PortInfo[Index].PortEn == 1) {
        DEBUG ((DEBUG_INFO, "VMD: Port %d is mapped under VMD\n", Index));
        switch(VmdInfoHob->VmdPortInfo.PortInfo[Index].RpFunc) {
          case 0:
          Fn0Assign.Data |= 1 << VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
          PciSegmentWrite32 ( DeviceBaseAddress + FN0_ASSIGN_IOC_VMD_REG, Fn0Assign.Data);
          break;
          case 1:
          Fn1Assign.Data |= 1 << VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
          PciSegmentWrite32 ( DeviceBaseAddress + FN1_ASSIGN_IOC_VMD_REG, Fn1Assign.Data);
          break;
          case 2:
          Fn2Assign.Data |= 1 << VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
          PciSegmentWrite32 ( DeviceBaseAddress + FN2_ASSIGN_IOC_VMD_REG, Fn2Assign.Data);
          break;
          case 3:
          Fn3Assign.Data |= 1 << VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
          PciSegmentWrite32 ( DeviceBaseAddress + FN3_ASSIGN_IOC_VMD_REG, Fn3Assign.Data);
          break;
          case 4:
          Fn4Assign.Data |= 1 << VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
          PciSegmentWrite32 ( DeviceBaseAddress + FN4_ASSIGN_IOC_VMD_REG, Fn4Assign.Data);
          break;
          case 5:
          Fn5Assign.Data |= 1 << VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
          PciSegmentWrite32 ( DeviceBaseAddress + FN5_ASSIGN_IOC_VMD_REG, Fn5Assign.Data);
          break;
          case 6:
          Fn6Assign.Data |= 1 << VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
          PciSegmentWrite32 ( DeviceBaseAddress + FN6_ASSIGN_IOC_VMD_REG, Fn6Assign.Data);
          break;
          case 7:
          Fn7Assign.Data |= 1 << VmdInfoHob->VmdPortInfo.PortInfo[Index].RpDev;
          PciSegmentWrite32 ( DeviceBaseAddress + FN7_ASSIGN_IOC_VMD_REG, Fn7Assign.Data);
          break;
        }
      }
    }// end of device detected
  }
  // Program VMAssign only when need to assign device and all its function under VMD
  VmAssign.Data = PciSegmentRead32 (DeviceBaseAddress + VMASSIGN_IOC_VMD_REG);
  // Skip VMD configuration if Config space is not valid.
  if (VmAssign.Data == 0xFFFFFFFF) {
    DEBUG((DEBUG_INFO, "VMD %02d not present\n",VmAssign.Data));
    return EFI_UNSUPPORTED;
  }

  // program CfgBarSize
  VmdCfgBarSize.Data  = PciSegmentRead8 (DeviceBaseAddress + CFGBARSZ_IOC_VMD_REG);
  VmdCfgBarSize.Bits.size = VMD_CFG_BAR_SIZE_DEFAULT;
  PciSegmentWrite8 (DeviceBaseAddress + CFGBARSZ_IOC_VMD_REG,VmdCfgBarSize.Data);

  // program MemBarSize 1/2
  VmdMemBar1Size.Data = PciSegmentRead8 (DeviceBaseAddress + MEMBAR1SZ_IOC_VMD_REG);
  VmdMemBar1Size.Bits.size = VMD_MEM_BAR1_SIZE_DEFAULT;
  PciSegmentWrite8 (DeviceBaseAddress + MEMBAR1SZ_IOC_VMD_REG,VmdMemBar1Size.Data);

  VmdMemBar2Size.Data = PciSegmentRead8 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM,
  VMD_BUS_NUM, VMD_DEV_NUM, VMD_FUN_NUM, MEMBAR2SZ_IOC_VMD_REG));
  VmdMemBar2Size.Bits.size = VMD_MEM_BAR2_SIZE_DEFAULT;
  PciSegmentWrite8 (DeviceBaseAddress + MEMBAR2SZ_IOC_VMD_REG,VmdMemBar2Size.Data);

  DEBUG ((DEBUG_INFO, "VMD VMAssign:%02x CfgBarSize:%02d MemBar1Size:%02d MemBar2Size:%02d\n",
  VmAssign.Data, VmdCfgBarSize.Data, VmdMemBar1Size.Data, VmdMemBar2Size.Data));

  DEBUG((DEBUG_INFO, "Fn0Assign.Data = 0x%x \n", Fn0Assign.Data));
  DEBUG((DEBUG_INFO, "Fn1Assign.Data = 0x%x \n", Fn1Assign.Data));
  DEBUG((DEBUG_INFO, "Fn2Assign.Data = 0x%x \n", Fn2Assign.Data));
  DEBUG((DEBUG_INFO, "Fn3Assign.Data = 0x%x \n", Fn3Assign.Data));
  DEBUG((DEBUG_INFO, "Fn4Assign.Data = 0x%x \n", Fn4Assign.Data));
  DEBUG((DEBUG_INFO, "Fn5Assign.Data = 0x%x \n", Fn5Assign.Data));
  DEBUG((DEBUG_INFO, "Fn6Assign.Data = 0x%x \n", Fn6Assign.Data));
  DEBUG((DEBUG_INFO, "Fn7Assign.Data = 0x%x \n", Fn7Assign.Data));

  // Program CfgBar attribute
  CfgBar.Data = PciSegmentRead32 (DeviceBaseAddress + CFGBAR_IOC_VMD_REG);
  //64-bit non-prefetcheble
  CfgBar.Bits.prefetchable = 0;
  CfgBar.Bits.type_f = 2;
  PciSegmentWrite32 (DeviceBaseAddress + CFGBAR_IOC_VMD_REG, CfgBar.Data32[0]);

  // Program MemBar1 attribute
  MemBar1.Data = PciSegmentRead32 (DeviceBaseAddress + MEMBAR1_IOC_VMD_REG);
  //32-bit non-prefetchable
  MemBar1.Bits.prefetchable = 0;
  MemBar1.Bits.type_f = 0;
  PciSegmentWrite32 (DeviceBaseAddress + MEMBAR1_IOC_VMD_REG, MemBar1.Data32[0]);

  // Program MemBar2 attribute
  MemBar2.Data = PciSegmentRead32 (DeviceBaseAddress + MEMBAR2_IOC_VMD_REG);
  // 64-bit non-prefetchable
  MemBar2.Bits.prefetchable = 0;
  MemBar2.Bits.type_f = 2;
  PciSegmentWrite32 (DeviceBaseAddress + MEMBAR2_IOC_VMD_REG, MemBar2.Data32[0]);

  DEBUG((DEBUG_INFO, "VMD VmAssign:0x%02x CfgBar:0x%02x MemBar1:0x%02x MemBar2:0x%02x\n",
  VmAssign.Data, CfgBar.Data, MemBar1.Data, MemBar2.Data));

  // Program VMCAP and VMConfig to apply bus restrictions and select bus numbering
  VmCap.Data = PciSegmentRead32 (DeviceBaseAddress + VMCAP_IOC_VMD_REG);
  VmCap.Bits.bus_restrict_cap = 0x1;
  PciSegmentWrite32 (DeviceBaseAddress + VMCAP_IOC_VMD_REG, VmCap.Data);

  VmConfig.Data = PciSegmentRead32 (DeviceBaseAddress + VMCONFIG_IOC_VMD_REG);
  DEBUG((DEBUG_INFO, "Default Read Value of VmConfig 0x%x\n", VmConfig.Data));

  // Select bus numbers 225 to 255 for VMD
  VmConfig.Bits.bus_restrictions = 0x2;

  // Initialize platform specific configurations
  VmdInitFruApi(VmdInfoHob, &VmConfig);

  DEBUG((DEBUG_INFO, "DidAssign value is 0x%x\n", VmConfig.Bits.did_assign));
  // program VMConfig to lock VMD registers
  VmConfig.Bits.vmdlock = 1;

  DEBUG((DEBUG_INFO, "value to be written to VmConfig 0x%x\n", VmConfig.Data));
  PciSegmentWrite32 (DeviceBaseAddress + VMCONFIG_IOC_VMD_REG, VmConfig.Data);
  VmConfig.Data = PciSegmentRead32 (DeviceBaseAddress + VMCONFIG_IOC_VMD_REG);
  DEBUG((DEBUG_INFO, "Read after write VmConfig 0x%x\n", VmConfig.Data));

  DEBUG((DEBUG_INFO, "Bus Restrictions are applied and Lock Bit is set for VMD \n"));
  DEBUG((DEBUG_INFO, "VMD DID value is 0x%x\n", PciSegmentRead16 (DeviceBaseAddress + PCI_DEVICE_ID_OFFSET)));
  ///
  /// Program temp VMD BARs
  ///
  ProgramVmdTempBars(VmdPeiConfig->VmdCfgBarBase, VmdPeiConfig->VmdMemBar1Base, VmdPeiConfig->VmdMemBar2Base);
  //
  // Install Vmd Init Done PPI to notify PEIM module.
  //
  Status = PeiServicesInstallPpi (&gVmdInitDonePpi);
  DEBUG ((DEBUG_INFO, "VMD: Installed gVmdInitDonePpi\n"));
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ VmdInit End ------------------\n"));
  return EFI_SUCCESS;
}
