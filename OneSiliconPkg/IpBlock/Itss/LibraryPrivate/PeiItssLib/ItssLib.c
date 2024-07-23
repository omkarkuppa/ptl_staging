/** @file
  PEI ITSS Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include <Library/PeiItssLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <Register/ItssRegs.h>
#include <ItssHandle.h>

//
// ITSS Message Decoder Control
//
typedef union {
  struct {
    UINT32        IrqDden          :1;  ///< IRQN Default Decoder Enable [0:0] RW
    UINT32        IntDden          :1;  ///< INTAD Default Decoder Enable [1:1] RW
    UINT32        NmiDden          :1;  ///< NMI Default Decoder Enable [2:2] RW
    UINT32        Rsvd2            :5;  ///< Reserved [7:3] RO
    UINT32        IntdDPir         :4;  ///< INTA-D Default Decoder PIR Mapping [11:8] RW
    UINT32        Rsvd1            :20; ///< Reserved [31:12] RO
  } Fields;
  UINT32 Data32;
} PCH_ITSS_PCR_MSGDC;

/**
  Retrieve ITSS Interrupt Config policy

  @param[in]  SiPolicy  Policy

  @retval InterruptConfig
**/
STATIC
PCH_INTERRUPT_CONFIG*
ItssGetIntConfigPolicy (
  IN CONST SI_POLICY_PPI  *SiPolicy
  )
{
  EFI_STATUS                   Status;
  PCH_INTERRUPT_CONFIG         *InterruptConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);

  return InterruptConfig;
}

/**
  Write to mask registers of controller and target 8259 PICs.
**/
VOID
ItssMask8259Interrupts (
  VOID
  )
{
  IoWrite8 (R_ITSS_IO_8259_MASK_INITIATOR, 0xFF);
  IoWrite8 (R_ITSS_IO_8259_MASK_TARGET, 0xFF);
}

/**
  Configure Message Decoder Control register
    INTDDEN set to 1: Enable interrupts from unknown sources
    INTDDPIR set to 1101: Shift to use PIR13

  @param[in] ItssHandle          Pointer to Itss handle

**/
VOID
ItssMsgDecoderConfig (
  IN ITSS_HANDLE  *ItssHandle
  )
{
  PCH_ITSS_PCR_MSGDC MsgDecoderCntl;

  // 8259 cannot recieve interrupts from SPE or SPF without the Message Decoder Control being configured
  //
  MsgDecoderCntl.Data32 = ItssHandle->PciPcrAccess->Read32 (ItssHandle->PciPcrAccess, R_ITSS_PCR_MSGDC);
  MsgDecoderCntl.Fields.IntDden = 1;
  MsgDecoderCntl.Fields.IntdDPir = V_ITSS_PCR_MSGDC_PIR13;
  ItssHandle->PciPcrAccess->Write32 (ItssHandle->PciPcrAccess, R_ITSS_PCR_MSGDC, MsgDecoderCntl.Data32);
}

/**
  This function will configure ITSS.PIRx registers for INTx->PIRy routing
  based on devices interrupt pin (INTx) and chosen IRQ for it.

  @param[in] DevIntConfigEntry       Table with interrupt assignment
  @param[in] DevIntConfigEntrySize   Number of entries in interrupt assignment table
**/
VOID
UpdatePirxRegs (
  IN ITSS_HANDLE                     *ItssHandle,
  IN PCH_DEVICE_INTERRUPT_CONFIG     *DevIntConfigEntry,
  IN UINT8                           DevIntConfigEntrySize
  )
{
  UINT32                    PirxIndex;
  UINT8                     DevIndex;
  UINT16                    Data16;
  CONST PCH_PIRX_REGISTER   *PirRegData;
  UINT32                    PirRegDataSize;
  CONST PCH_DEV_INT_INFO    *PchDeviceIntInfo;
  UINT32                    PchDeviceIntInfoSize;
  UINT32                    PchDeviceIntInfoIndex;
  BOOLEAN                   UsesPirq;

  PchDeviceIntInfo = ItssHandle->ItssPrivateConfig->PchDevIntInfoData;
  PchDeviceIntInfoSize = ItssHandle->ItssPrivateConfig->PchDevIntInfoSize;

  if (PchDeviceIntInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "%a failed to Get Interrupt Info data \n", __FUNCTION__));
    ASSERT (PchDeviceIntInfo != NULL);
    return;
  }

  PirRegData = ItssHandle->ItssPrivateConfig->ItssPirxRegData;
  PirRegDataSize = ItssHandle->ItssPrivateConfig->ItssPirxRegDataSize;

  for (PirxIndex = 0; PirxIndex < PirRegDataSize; PirxIndex++) {
    //
    // Default value for each PIRx register is 3210h
    //
    Data16 = 0x3210;
    for (DevIndex = 0; DevIndex < DevIntConfigEntrySize; DevIndex++) {
      if (DevIntConfigEntry[DevIndex].Device == PirRegData[PirxIndex].Device) {

        UsesPirq = TRUE;
        //
        // Check if device is under INTx->PIRQy mapping
        //
        for (PchDeviceIntInfoIndex = 0; PchDeviceIntInfoIndex < PchDeviceIntInfoSize; PchDeviceIntInfoIndex++) {
          if ((DevIntConfigEntry[DevIndex].Device == PchDeviceIntInfo[PchDeviceIntInfoIndex].Device) &&
              (DevIntConfigEntry[DevIndex].Function == PchDeviceIntInfo[PchDeviceIntInfoIndex].Function)) {
            if (PchDeviceIntInfo[PchDeviceIntInfoIndex].UsesPirq == USES_PIRQ_NO) {
              UsesPirq = FALSE;
            }
            break;
          }
        }

        if (UsesPirq) {
          Data16 &= ~(UINT16) (0xF << (4*(DevIntConfigEntry[DevIndex].IntX - PchIntA)));
          Data16 |= (UINT16) ((DevIntConfigEntry[DevIndex].Irq - 16) << (4*(DevIntConfigEntry[DevIndex].IntX - PchIntA)));
        }
      }
    }

    ItssHandle->PciPcrAccess->Write16 (ItssHandle->PciPcrAccess, R_ITSS_PCR_PIR0 + PirRegData[PirxIndex].PirIndex * 0x2, Data16);
  }
}


#ifndef MDEPKG_NDEBUG
/**
  Check interrupt assignment, provide information if detected improper interrupt setting

  @param[in] DevIntConfigEntry        Table with interrupt assignment
  @param[in] DevIntConfigEntrySize    Number of entries in interrupt assignment table
**/
STATIC
VOID
CheckInterruptSettings (
  IN ITSS_HANDLE                      *ItssHandle,
  IN PCH_DEVICE_INTERRUPT_CONFIG      *DevIntConfigEntry,
  IN UINT8                            DevIntConfigEntrySize
  )
{
  DEBUG_CODE_BEGIN ();
  //
  // Algorithm which will parse whole table and check it correctness, to prevent
  // users from trying to set PCH devices interrupt in non supported way.
  // Function is divided into few step, each verifying different cases:
  // Point 1.
  //  - Entry for certain DxFy appears more than once
  //  - Device is outside D18Fx-D31Fx range
  // Point 2.
  //  - InterruptConfig table contains invalid DxFy (does not generate interrupt or does not exist)
  //  - Device which falls under INTx->PIRQy mapping has its IRQ set outside IRQ16-23 range
  //  - Device uses IRQ 0, 2 or 8
  // Point 3.
  //  - Single function device does not use INTA
  // Point 4.
  //  - Within multifunctional devices same interrupt pin (INTx) is routed to different PIRQy (IRQ16-23)
  // Point 4b.
  //  -  Devices under the same INTx->PIRQy mapping register (PIRn) request conflicting routing
  // Point 5.
  //  - Devices which have interrupt pin setting as RWO are not programmed
  //  - Devices which have requirement for interrupt pin configuration are not programmed correctly
  //  - InterruptConfig has InterruptPin setting not matching default value of INTP register when it is RO
  // Point 6.
  //  - PCI Express Root Ports mapping are recommended to be programmed only with one set of values:
  //     D(29/28/27)Fy -> INTP = (y % 4) , IRQ = 16 + (y % 4)
  //    otherwise _PRT methods in ACPI for RootPorts would require additional patching as
  //    PCIe Endpoint Device Interrupt is further subjected to INTx to PIRQy Mapping
  // Point 7.
  //  - Devices which should have unique interrupt share it
  // Point 8.
  //  - GPIO pads which are routed to IOxAPIC cause an IRQ which is already in use by other device or pad
  //
  // Note: this routine is not able to check if eSPI device is using IRQ >=24 and if it collides with other devices or GPIO pads IRQ usage
  //
  CONST PCH_DEV_INT_INFO    *PchDeviceIntInfo;
  UINT32                    PchDeviceIntInfoIndex;
  UINT32                    PchDeviceIntInfoIndex2;
  UINT8                     DevIntConfigIndex;
  UINT8                     DevIntConfigIndex2;
  CONST PCH_PIRX_REGISTER   *PirRegData;
  UINT32                    PirRegDataSize;
  UINT8                     Index;
  UINT32                    PchDeviceIntInfoSize;
  BOOLEAN                   DevExist;
  BOOLEAN                   IrqError;
  BOOLEAN                   SingleFunctionDev;
  UINT32                    CurrentDevice;
  UINT8                     FunctionNumber;
  UINT8                     IntRouting[4];
  UINT32                    PirxIndex;
  UINT32                    PirxSubIndex;
  BOOLEAN                   FirstDeviceAnalyzed;
  UINT32                    MinDevNum;
  UINT32                    MaxDevNum;
  BOOLEAN                   GpioConflict;

  //
  // Value of 1 on selected bit position indicates that IRQ is being used
  // IrqUsage[0] - for IRQ0-31
  // IrqUsage[1] - for IRQ31-63
  // IrqUsage[2] - for IRQ64-95
  // IrqUsage[3] - for IRQ96-127
  //
  UINT32         IrqUsage[4];

  GpioConflict = FALSE;

  ZeroMem (IrqUsage, sizeof (IrqUsage));

  PchDeviceIntInfoSize = ItssHandle->ItssPrivateConfig->PchDevIntInfoSize;
  PchDeviceIntInfo = ItssHandle->ItssPrivateConfig->PchDevIntInfoData;

  MinDevNum = PchDeviceIntInfo[0].Device;
  MaxDevNum = PchDeviceIntInfo[PchDeviceIntInfoSize - 1].Device;

  //
  // 1.Check if for each DxFy there is only one entry and if none of devices is outside the range of supported devices
  //
  for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {
    if ((DevIntConfigEntry[DevIntConfigIndex].Device < MinDevNum) ||
        (DevIntConfigEntry[DevIntConfigIndex].Device > MaxDevNum)) {
      //
      // Report an error if device number is outside 18-31 range
      //
      DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: InterruptConfig has a device (D%dF%d) which is outside D%d-D%d range!\n",
              DevIntConfigEntry[DevIntConfigIndex].Device,
              DevIntConfigEntry[DevIntConfigIndex].Function,
              MinDevNum,
              MaxDevNum));
      ASSERT (FALSE);
      return;
    }

    for (DevIntConfigIndex2 = DevIntConfigIndex + 1; DevIntConfigIndex2 < DevIntConfigEntrySize; DevIntConfigIndex2++) {

      if ((DevIntConfigEntry[DevIntConfigIndex].Device == DevIntConfigEntry[DevIntConfigIndex2].Device) &&
          (DevIntConfigEntry[DevIntConfigIndex].Function == DevIntConfigEntry[DevIntConfigIndex2].Function)) {
        //
        // Report an error if entry appears more than once
        //
        DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: Entry for D%dF%d appears more than once!\n",
                DevIntConfigEntry[DevIntConfigIndex].Device,
                DevIntConfigEntry[DevIntConfigIndex].Function));
        ASSERT (FALSE);
        return;
      }
    }
  }

  //
  // 2.Check if for each DnFx selected IRQ is supported for device. If device uses INTx->PIRQy mapping
  //   must have IRQ  >= 16 and IRQ <= 23. Report an error if DevIntConfigEntry[] consist of
  //   device which does not generate interrupts, is not a valid device or uses IRQ0,2,8
  //
  for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {

    DevExist = FALSE;

    PchDeviceIntInfoIndex = (DevIntConfigEntry[DevIntConfigIndex].Device - MinDevNum)*8 + DevIntConfigEntry[DevIntConfigIndex].Function;
    if (PchDeviceIntInfo[PchDeviceIntInfoIndex].ValidDevice == VALID_DEV_YES) {

      DevExist = TRUE;
      //
      // Check if InterruptConfig consist of entry for device which does not generate interrupt
      //
      if (PchDeviceIntInfo[PchDeviceIntInfoIndex].UsesInterrupts == INTERRUPT_NO) {
        DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: D%dF%d does not generate interrupts!\n",
                DevIntConfigEntry[DevIntConfigIndex].Device,
                DevIntConfigEntry[DevIntConfigIndex].Function));
        ASSERT (FALSE);
        return;
      }

      //
      // Check if for device which uses INTx -> PIRQy mapping has its IRQ set outside 16 to 23 range
      // NOTE: If cAVS/HDa is going to be supported in ACPI mode then this check needs to be rewritten.
      // HDa in PCI mode is under INTx->PIRQy mapping but in ACPI mode can have its IRQ outside 16 to 23 range.
      //
      if (PchDeviceIntInfo[PchDeviceIntInfoIndex].UsesPirq == USES_PIRQ_YES) {
        if (DevIntConfigEntry[DevIntConfigIndex].Irq < 16 || DevIntConfigEntry[DevIntConfigIndex].Irq > 23) {
          DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: D%dF%d has its IRQ outside 16 to 23 range!\n",
                  DevIntConfigEntry[DevIntConfigIndex].Device,
                  DevIntConfigEntry[DevIntConfigIndex].Function));
          ASSERT (FALSE);
          return;
        }
      }

      //
      // Check if IRQ for device is not set to 0,2 or 8
      //
      if ((DevIntConfigEntry[DevIntConfigIndex].Irq == 0) ||
          (DevIntConfigEntry[DevIntConfigIndex].Irq == 2) ||
          (DevIntConfigEntry[DevIntConfigIndex].Irq == 8)) {
        DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: Device D%dF%d must not use IRQ%d!\n",
                DevIntConfigEntry[DevIntConfigIndex].Device,
                DevIntConfigEntry[DevIntConfigIndex].Function,
                DevIntConfigEntry[DevIntConfigIndex].Irq));
        ASSERT (FALSE);
        return;
      }
    }

    //
    // If device from InterruptConfig was not found in mPchDeviceInterruptInfo[] table then
    // it it is not a valid entry.
    //
    if (DevExist == FALSE) {
      DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: D%dF%d is not a valid device!\n",
              DevIntConfigEntry[DevIntConfigIndex].Device,
              DevIntConfigEntry[DevIntConfigIndex].Function));
      ASSERT (FALSE);
      return;
    }
  }

  //
  // 3.Check if single function device is set to use INTA
  //
  // Locate in mPchDeviceInterruptInfo table single function devices, if found one locate it in InterruptConfig
  // and check if it uses INTA.
  //
  for (PchDeviceIntInfoIndex = 0; PchDeviceIntInfoIndex < PchDeviceIntInfoSize; PchDeviceIntInfoIndex+=8) {
    if (PchDeviceIntInfo[PchDeviceIntInfoIndex].ValidDevice == VALID_DEV_NO) {
      //
      // If DxF0 is not a valid device move to next one.
      //
      continue;
    }
    SingleFunctionDev = TRUE;

    for (PchDeviceIntInfoIndex2 = PchDeviceIntInfoIndex + 1; PchDeviceIntInfoIndex2 < PchDeviceIntInfoIndex + 8; PchDeviceIntInfoIndex2++) {
      //
      // Check other functions of this device (DxF1-DxF7)
      //
      if (PchDeviceIntInfo[PchDeviceIntInfoIndex2].ValidDevice == VALID_DEV_YES) {
        //
        // If found another valid device for certain device number then this is not a signle function device.
        //
        SingleFunctionDev = FALSE;
        break;
      }
    }

    if (SingleFunctionDev == TRUE) {
      //
      // Found a single function device, try to locate it InterruptConfig
      //
      for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {

        if (DevIntConfigEntry[DevIntConfigIndex].Device == PchDeviceIntInfo[PchDeviceIntInfoIndex].Device) {
          //
          // Found device in InterruptConfig, check its interrupt pin
          //
          if (DevIntConfigEntry[DevIntConfigIndex].IntX != PchIntA) {
            //
            // If single function device doesn't use INTA report an error
            //
            DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: Single function device %d isn't configured to use INTA!\n",DevIntConfigEntry[DevIntConfigIndex].Device));
            ASSERT (FALSE);
            return;
          }
        }
      }
    }
  }

  //
  // 4.Check if for one device which uses INTx->PIRQy mapping different functions
  //   with the same INTx are not routed to different PIRQs (IRQ16-23)
  //
  CurrentDevice = 0;
  for (PchDeviceIntInfoIndex = 0; PchDeviceIntInfoIndex < PchDeviceIntInfoSize; PchDeviceIntInfoIndex++) {
    if ((PchDeviceIntInfo[PchDeviceIntInfoIndex].Device == CurrentDevice) ||
        (PchDeviceIntInfo[PchDeviceIntInfoIndex].UsesPirq == USES_PIRQ_NO) ||
        (PchDeviceIntInfo[PchDeviceIntInfoIndex].ValidDevice == VALID_DEV_NO)) {
      //
      // Skip to next device if current was already analyzed,
      // it doesn't use INTx->PIRQy mapping or it is not a PCH device
      //
      continue;
    }
    CurrentDevice = PchDeviceIntInfo[PchDeviceIntInfoIndex].Device;
    ZeroMem (IntRouting, sizeof (IntRouting));

    for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {

      if (DevIntConfigEntry[DevIntConfigIndex].Device == CurrentDevice) {

        if ((DevIntConfigEntry[DevIntConfigIndex].IntX >= PchIntA) &&
            (DevIntConfigEntry[DevIntConfigIndex].IntX <= PchIntD)) {
          //
          // Check if INTx is mapped to the same PIRQy for all functions within one device.
          // NOTE: If cAVS/HDa is going to be supported in ACPI mode then this check needs to be rewritten.
          // HDa in PCI mode is under INTx->PIRQy mapping but when in ACPI mode it can have its IRQ set
          // outside 16 to 23 range and doesn't need to meet criteria analyzed here.
          //
          if (IntRouting[DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA] == 0) {
            IntRouting[DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA] = DevIntConfigEntry[DevIntConfigIndex].Irq;
          } else if (IntRouting[DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA] != DevIntConfigEntry[DevIntConfigIndex].Irq) {
            DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: For D%d INT%c is mapped to different IRQs!\n",
                    CurrentDevice,
                    DevIntConfigEntry[DevIntConfigIndex].IntX + 64));
            ASSERT (FALSE);
            return;
          }
        }
      }
    }
  }

  PirRegData = ItssHandle->ItssPrivateConfig->ItssPirxRegData;
  PirRegDataSize = ItssHandle->ItssPrivateConfig->ItssPirxRegDataSize;

  //
  // 4b. Check if devices under the same INTx->PIRQy mapping register (PIRn)
  //     do not request conflicting routing
  //
  for (PirxIndex = 0; PirxIndex < PirRegDataSize - 1; PirxIndex++) {
    //
    // Initialize INTx->PIRQy mapping table
    // 0xFF - mapping unspecified
    //
    SetMem (IntRouting, sizeof (IntRouting), 0xFF);
    FirstDeviceAnalyzed = FALSE;
    //
    // Check if more than one devices is mapped to the same PIRn register
    //
    for (PirxSubIndex = PirxIndex + 1; PirxSubIndex < PirRegDataSize; PirxSubIndex++) {
      if (PirRegData[PirxSubIndex].PirIndex == PirRegData[PirxIndex].PirIndex) {
        //
        // Two devices (Dxx) use the same PIRn register
        //
        // Get first devices INTx->PIRQy mapping
        //
        if (!FirstDeviceAnalyzed) {
          for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {
            if (DevIntConfigEntry[DevIntConfigIndex].Device == PirRegData[PirxIndex].Device) {
              //
              // Check if device is under INTx->PIRQy mapping
              //
              PchDeviceIntInfoIndex = (DevIntConfigEntry[DevIntConfigIndex].Device - MinDevNum)*8 + DevIntConfigEntry[DevIntConfigIndex].Function;
              if (PchDeviceIntInfo[PchDeviceIntInfoIndex].UsesPirq == USES_PIRQ_NO) {
                continue;
              }

              IntRouting[DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA] = DevIntConfigEntry[DevIntConfigIndex].Irq;
            }
          }
          FirstDeviceAnalyzed = TRUE;
        }

        //
        // Analyze another device INTx->PIRQy mapping and check if it conflicts with previous one
        //
        for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {
          if (DevIntConfigEntry[DevIntConfigIndex].Device == PirRegData[PirxSubIndex].Device) {
            //
            // Check if device is under INTx->PIRQy mapping
            //
            PchDeviceIntInfoIndex = (DevIntConfigEntry[DevIntConfigIndex].Device - MinDevNum)*8 + DevIntConfigEntry[DevIntConfigIndex].Function;
            if (PchDeviceIntInfo[PchDeviceIntInfoIndex].UsesPirq == USES_PIRQ_NO) {
              continue;
            }

            //
            // If mapping for certain IntX was not specified by previous device update it
            //
            if (IntRouting[DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA] == 0xFF) {
              IntRouting[DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA] = DevIntConfigEntry[DevIntConfigIndex].Irq;

            //
            // Check if there is no conflicting mapping
            //
            } else if (IntRouting[DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA] != DevIntConfigEntry[DevIntConfigIndex].Irq) {
              DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: For D%d and D%d there is conflicting INT%c to IRQ mapping!\n",
                      PirRegData[PirxIndex].Device,
                      PirRegData[PirxSubIndex].Device,
                      DevIntConfigEntry[DevIntConfigIndex].IntX + 64
                      ));
              ASSERT (FALSE);
              return;
            }
          }
        }
      }
    }
  }

  //
  // 5. Check if devices which have special requirement regarding InterruptPin
  //    are properly configured in InterruptConfig table
  //
  for (PchDeviceIntInfoIndex = 0; PchDeviceIntInfoIndex < PchDeviceIntInfoSize; PchDeviceIntInfoIndex++) {
    if ((PchDeviceIntInfo[PchDeviceIntInfoIndex].IntPinRequirement != INTP_NO_REQ) &&
        (PchDeviceIntInfo[PchDeviceIntInfoIndex].ValidDevice == VALID_DEV_YES)) {
      //
      // Found a device which has special requirement regarding interrupt pin.
      // If devices requires its INTP register programmed then entry for this device must be included
      // in InterruptConfig.
      // If INTP is RO register, then either this device may be omitted in InterruptConfig or
      // if included then InterruptPin value must match the one preset in this register.
      //
      DevExist = FALSE;
      for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {

        if ((DevIntConfigEntry[DevIntConfigIndex].Device == PchDeviceIntInfo[PchDeviceIntInfoIndex].Device) &&
            (DevIntConfigEntry[DevIntConfigIndex].Function == PchDeviceIntInfo[PchDeviceIntInfoIndex].Function)) {

          if (PchDeviceIntInfo[PchDeviceIntInfoIndex].IntPinRequirement == INTP_MUST_SET) {
            //
            // Device  which required its INTP to be set was found, move to next one
            //
            DevExist = TRUE;
            break;
          } else {
            //
            // If device has its INTP in RO register or there are other requirements for it,
            // then InterruptConfig must match expected configuration
            //
            if ((UINT32) (DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA) != (PchDeviceIntInfo[PchDeviceIntInfoIndex].IntPinRequirement - INTP_IS_INTA)) {
              DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: Device D%dF%d can use only INT%c!\n",
                      PchDeviceIntInfo[PchDeviceIntInfoIndex].Device,
                      PchDeviceIntInfo[PchDeviceIntInfoIndex].Function,
                      PchDeviceIntInfo[PchDeviceIntInfoIndex].IntPinRequirement - INTP_IS_INTA + 65));
              ASSERT (FALSE);
              return;
            }
          }
        }
      }
      if ((DevExist == FALSE) &&
          (PchDeviceIntInfo[PchDeviceIntInfoIndex].IntPinRequirement == INTP_MUST_SET)) {
        //
        // If after analyzing all entries in DevIntConfigEntry[] table
        // there is no required device, report an error
        //
        DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: Device D%dF%d is not included in InterruptConfig table!\n",
                PchDeviceIntInfo[PchDeviceIntInfoIndex].Device,
                PchDeviceIntInfo[PchDeviceIntInfoIndex].Function));
        ASSERT (FALSE);
        return;
      }
    }
  }

  //
  // 6. Check if PCI Express Root Ports mapping is programmed in following way
  //    D(27-29)Fy -> INTP = (y % 4) , IRQ = 16 + (y % 4)
  //
  for (PchDeviceIntInfoIndex = 0; PchDeviceIntInfoIndex < PchDeviceIntInfoSize; PchDeviceIntInfoIndex++) {
    if ((PchDeviceIntInfo[PchDeviceIntInfoIndex].IsPcieRp)) {
      for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {
        if ((DevIntConfigEntry[DevIntConfigIndex].Device == PchDeviceIntInfo[PchDeviceIntInfoIndex].Device) &&
            (DevIntConfigEntry[DevIntConfigIndex].Function == PchDeviceIntInfo[PchDeviceIntInfoIndex].Function)) {
          FunctionNumber = DevIntConfigEntry[DevIntConfigIndex].Function;
          if ((DevIntConfigEntry[DevIntConfigIndex].Irq != (16 + (FunctionNumber % 4))) ||
            ((DevIntConfigEntry[DevIntConfigIndex].IntX - PchIntA) != (FunctionNumber % 4))) {

            DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: RootPort Device D%dF%d is not programmed with recommended interrupt settings!\n",
                  DevIntConfigEntry[DevIntConfigIndex].Device,
                  DevIntConfigEntry[DevIntConfigIndex].Function));
            ASSERT (FALSE);
            return;
          }
        }
      }
    }
  }

  //
  // 7. Check if devices which need unique IRQ do not share it with other devices.
  //
  IrqError = FALSE;
  for (DevIntConfigIndex = 0; DevIntConfigIndex < DevIntConfigEntrySize; DevIntConfigIndex++) {
    //
    // Save IRQ number used by this device. This information will be needed for Step 7
    // for GPIO interrupts check.
    //
    IrqUsage[DevIntConfigEntry[DevIntConfigIndex].Irq >> 5]|= 0x1 << (DevIntConfigEntry[DevIntConfigIndex].Irq % 32);

    PchDeviceIntInfoIndex = (DevIntConfigEntry[DevIntConfigIndex].Device - MinDevNum) * 8 + DevIntConfigEntry[DevIntConfigIndex].Function;
    if (PchDeviceIntInfo[PchDeviceIntInfoIndex].IrqUnique == IRQ_UNIQUE_NO) {
      //
      // IRQ can be shared, move to next device
      //
      continue;
    }
    //
    // Device which needs unique IRQ was found, now search for another device which uses the same IRQ
    //
    for (DevIntConfigIndex2 = DevIntConfigIndex + 1; DevIntConfigIndex2 < DevIntConfigEntrySize; DevIntConfigIndex2++) {
      if (DevIntConfigEntry[DevIntConfigIndex].Irq == DevIntConfigEntry[DevIntConfigIndex2].Irq) {
        // If found device with the same IRQ check its number,
        // if IRQ > 23 it must be unique - report an error,
        // If IRQ <= 23 it can be shared but only between devices with different PortID
        //
        if (DevIntConfigEntry[DevIntConfigIndex].Irq > 23 ) {
          IrqError = TRUE;

          if (DevIntConfigEntry[DevIntConfigIndex].Irq == ItssHandle->ItssPrivateConfig->UniqueIrqOverride) {
             IrqError = FALSE;
          }

        } else {
          //
          // Check if device which has the same IRQ (which is <=23) can share interrupt
          //
          PchDeviceIntInfoIndex2 = (DevIntConfigEntry[DevIntConfigIndex2].Device - MinDevNum) * 8 + DevIntConfigEntry[DevIntConfigIndex2].Function;
          if (PchDeviceIntInfo[PchDeviceIntInfoIndex].IrqUnique == PchDeviceIntInfo[PchDeviceIntInfoIndex2].IrqUnique) {
            //
            // If both of these devices are the same type their IRQs must be unique
            //
            IrqError = TRUE;
          }
        }

        //
        // If found an IRQ sharing between devices which require unique interrupt then assert.
        //
        if (IrqError == TRUE) {
          DEBUG ((DEBUG_ERROR, "InterruptConfig ERROR: Device D%dF%d and D%dF%d share interrupt (IRQ%d)!\n",
                  DevIntConfigEntry[DevIntConfigIndex].Device,
                  DevIntConfigEntry[DevIntConfigIndex].Function,
                  DevIntConfigEntry[DevIntConfigIndex2].Device,
                  DevIntConfigEntry[DevIntConfigIndex2].Function,
                  DevIntConfigEntry[DevIntConfigIndex].Irq));
          return;
        }
      }
    }
  }
  //
  // 8. Check for any GPIO pad conflicts
  //
  if (ItssHandle->ItssCallBack->ItssCheckGpioConflicts != NULL) {
    GpioConflict = ItssHandle->ItssCallBack->ItssCheckGpioConflicts (ItssHandle, IrqUsage);
  } else {
    DEBUG ((DEBUG_WARN, "%a : ItssCheckGpioConflicts is NULL\n", __FUNCTION__));
  }

  if (GpioConflict) {
    return;
  }

  //
  // Dump IRQ usage
  //
  DEBUG ((DEBUG_INFO, "------------------ IRQ Usage ------------------\n"));
  DEBUG ((DEBUG_INFO, "  IRQxxx  USED\n"));
  for (Index = 0; Index < 127; Index++) {
    DEBUG ((DEBUG_INFO, "  IRQ%03d  %x\n",Index,((IrqUsage[Index >> 5] >> (Index % 32)) & BIT0)));
  }
  DEBUG_CODE_END ();
}
#endif // MDEPKG_NDEBUG

/**
  Configures interrupts polarity for PCI devices.

  @param[in] PchInterruptConfig    PCH interrupt Configuration
**/
VOID
SetInterruptsPolarityForPchInternalDevices (
  IN ITSS_HANDLE       *ItssHandle,
  IN PCH_INTERRUPT_CONFIG    *InterruptConfig
  )
{
  UINT8   DevIntConfigIndex;
  UINT32  IpcXReg[PCH_MAX_ITSS_IPC_REGS];
  UINTN   IpcXIndex;

  ZeroMem (IpcXReg, sizeof (IpcXReg));

  //
  // PIRQA-H (IRQ16-23)
  // All PCI interrupts polarity have to be set as active low.
  // PCI specification rev. 2.2 December 18,1998 - chapter 2.2.6
  //
  IpcXReg[0] = 0x00FF0000;

  //
  // GPI legacy interrupt polarity can be either ActiveLow or ActiveHigh
  // OS can be informed about polarity by ACPI Interrupt{..} descriptor
  // We set GPI interrupt polarity to ActiveLow to be coherent
  // with PCI devices for simplicity
  //
  ASSERT (InterruptConfig->GpioIrqRoute < PCH_MAX_ITSS_IRQ_NUM);
  IpcXIndex = InterruptConfig->GpioIrqRoute / 32;
  if (IpcXIndex < PCH_MAX_ITSS_IPC_REGS) {
    IpcXReg[IpcXIndex] |= 0x1 << (InterruptConfig->GpioIrqRoute % 32);
  }

  //
  // All PCH internal devices appear as PCI devices.
  // Due to the fact that not all PCH internal devices use IRQs from range IRQ[16-23]
  // Get all IRQ numbers used by these and switch their interrupt to ActiveLow.
  //
  for (DevIntConfigIndex = 0; DevIntConfigIndex < InterruptConfig->NumOfDevIntConfig; DevIntConfigIndex++) {
    IpcXReg[InterruptConfig->DevIntConfig[DevIntConfigIndex].Irq >> 5] |= 0x1 << (InterruptConfig->DevIntConfig[DevIntConfigIndex].Irq % 32);
  }

  //
  // Program ITSS.IPC0-3 registers.
  // Setting only requested IRQs to active low AND NOT altering
  // previous configuration.
  //
  for (IpcXIndex = 0; IpcXIndex < PCH_MAX_ITSS_IPC_REGS; IpcXIndex++) {
    ItssHandle->PciPcrAccess->AndThenOr32 (ItssHandle->PciPcrAccess, R_ITSS_PCR_IPC0 + (UINT16) IpcXIndex * 4, ~0u, IpcXReg[IpcXIndex]);
  }
}

/**
  Retrieve interrupt configuration for a given PCH device (DxFy)

  @param[in]  SiPolicy  Policy
  @param[in]  Device    Device number
  @param[in]  Function  Function number
  @param[out] IntPin    Interrupt pin: INTA-INTD (see PCH_INT_PIN for reference)
  @param[out] ApicIrq   APIC IRQ number
**/
VOID
ItssGetDevIntConfig (
  IN  CONST SI_POLICY_PPI  *SiPolicy,
  IN  UINT8                Device,
  IN  UINT8                Function,
  OUT UINT8                *IntPin,
  OUT UINT8                *ApicIrq
  )
{
  PCH_INTERRUPT_CONFIG  *InterruptConfig;
  UINT8                 Index;

  if (SiPolicy == NULL) {
    *IntPin = 0;
    *ApicIrq = 0;
  }

  InterruptConfig = ItssGetIntConfigPolicy (SiPolicy);

  //
  // Loop through whole interrupt table and find matching device
  //
  for (Index = 0; Index < InterruptConfig->NumOfDevIntConfig; Index++) {
    if ((InterruptConfig->DevIntConfig[Index].Device == Device) &&
        (InterruptConfig->DevIntConfig[Index].Function == Function)) {
      *IntPin = InterruptConfig->DevIntConfig[Index].IntX;
      *ApicIrq = InterruptConfig->DevIntConfig[Index].Irq;
      return;
    }
  }

  //
  // Interrupt policy doesn't have interrupt config entry
  // for a device that is being enabled on the platform
  //
  DEBUG ((DEBUG_WARN, "ITSS: Couldn't match interrupt policy for (%d:%d)\n", Device, Function));
}

/**
  Retrieve interrupt pin for a given PCH device (DxFy)

  @param[in]  SiPolicy  Policy
  @param[in]  Device    Device number
  @param[in]  Function  Function number

  @retval Interrupt pin: INTA-INTD (see PCH_INT_PIN for reference)
**/
UINT8
ItssGetDevIntPin (
  IN  CONST SI_POLICY_PPI  *SiPolicy,
  IN  UINT8                Device,
  IN  UINT8                Function
  )
{
  UINT8  IntPin;
  UINT8  ApicIrq;

  ItssGetDevIntConfig (SiPolicy, Device, Function, &IntPin, &ApicIrq);

  return IntPin;
}

/**
  Retrieve IRQ for Gpio Services

  @param[in]  SiPolicy  Policy

  @retval GpioIrq
**/
UINT8
ItssGetGpioDevIntConfig (
  IN CONST SI_POLICY_PPI  *SiPolicy
  )
{
  return ItssGetIntConfigPolicy (SiPolicy)->GpioIrqRoute;
}

/**
  Retrieve IRQ for TCO

  @param[in]  SiPolicy  Policy

  @retval TcoIrq     TCO IRQ. If 0 TCO interrupt is not to be enabled
**/
UINT8
ItssGetTcoDevIntConfig (
  IN CONST SI_POLICY_PPI  *SiPolicy
  )
{
  PCH_INTERRUPT_CONFIG  *InterruptConfig;

  InterruptConfig = ItssGetIntConfigPolicy (SiPolicy);

  if (InterruptConfig->TcoIrqEnable) {
    return InterruptConfig->TcoIrqSelect;
  } else {
    return 0;
  }
}

/**
  Retrieve IRQ for PMC SCI

  @param[in]  SiPolicy  Policy

  @retval PmcSciIrq
**/
UINT8
ItssGetPmcSciDevIntConfig (
  IN CONST SI_POLICY_PPI  *SiPolicy
  )
{
  return ItssGetIntConfigPolicy (SiPolicy)->SciIrqSelect;
}

/**
  Configures PCH interrupts

  @param[in] ItssHandle    Itss Handle
**/
VOID
ItssConfigureInterrupts (
  IN ITSS_HANDLE  *ItssHandle
  )
{
  if (ItssHandle->InterruptConfig->NumOfDevIntConfig >= PCH_MAX_DEVICE_INTERRUPT_CONFIG) {
    ASSERT (FALSE);
    return;
  }

#ifndef MDEPKG_NDEBUG
  //
  // Check interrupt settings
  //
  CheckInterruptSettings (
    ItssHandle,
    ItssHandle->InterruptConfig->DevIntConfig,
    ItssHandle->InterruptConfig->NumOfDevIntConfig
    );
#endif //MDEPKG_NDEBUG

  //
  // Set Pirx Interrupt routing register
  //
  SetInterruptsPolarityForPchInternalDevices (ItssHandle, ItssHandle->InterruptConfig);
  UpdatePirxRegs (
    ItssHandle,
    ItssHandle->InterruptConfig->DevIntConfig,
    ItssHandle->InterruptConfig->NumOfDevIntConfig
    );
}

/**
  Initialize IOAPIC according to IoApicConfig policy of the PCH
  Policy PPI

  @param[in] ItssHandle          Itss Handle

**/
VOID
ItssInitIoApic (
  IN ITSS_HANDLE  *ItssHandle
  )
{
  EFI_STATUS            Status;
  UINT32                IoApicId;
  UINT32                Data32;
  UINT32                Data32And;
  EFI_BOOT_MODE         BootMode;

  //
  // Configure ITSS power management settings
  //
  if (!ItssHandle->ItssPrivateConfig->LegacyIoLowLatency) {
    //
    // Set PCR[ITSS] + 3300h[4:0] = 11011b
    // Set PCR[ITSS] + 3300h[2] = 1 in POST time only if 8254 timer is not used.
    //
    Data32And = ~0u;
    Data32 = (B_ITSS_PCR_ITSSPRC_PGCBDCGE | B_ITSS_PCR_ITSSPRC_HPETDCGE | B_ITSS_PCR_ITSSPRC_SBDCGE | B_ITSS_PCR_ITSSPRC_PCIDCGE);
    if (ItssHandle->IoApicConfig->Enable8254ClockGating) {
      Data32 |= B_ITSS_PCR_ITSSPRC_CGE8254;
    } else {
      //
      // During S3 resume path, if Enable8254ClockGating is disabled and
      // Enable8254ClockGatingOnS3 is enabled, RC will set 8254CGE with BOOT_SAI
      // to avoid of SMI requirement of setting this.
      //
      Status = PeiServicesGetBootMode (&BootMode);
      ASSERT_EFI_ERROR (Status);
      if ((BootMode == BOOT_ON_S3_RESUME) &&
          ItssHandle->IoApicConfig->Enable8254ClockGatingOnS3) {
        Data32 |= B_ITSS_PCR_ITSSPRC_CGE8254;
      }
    }

    ItssHandle->PciPcrAccess->AndThenOr32 (
      ItssHandle->PciPcrAccess,
      R_ITSS_PCR_ITSSPRC,
      Data32And,
      Data32
      );
  }

  //
  // Program APIC Range Select bits that define address bits 19:12 for the IOxAPIC range.
  // This value must not be changed unless the IOxAPIC Enable bit is cleared.
  //

  if (ItssHandle->ItssCallBack->ItssP2SbConfigureIoApic != NULL) {
    ItssHandle->ItssCallBack->ItssP2SbConfigureIoApic (ItssHandle);
  } else {
    DEBUG ((DEBUG_WARN, "%a : ItssP2SbConfigureIoApic is NULL\n", __FUNCTION__));
  }

  //
  // Get current IO APIC ID
  //
  MmioWrite8 ((ItssHandle->ItssPrivateConfig->IoApicAddress + R_ITSS_APIC_MEM_IDX), 0);
  IoApicId = MmioRead32 ((ItssHandle->ItssPrivateConfig->IoApicAddress + R_ITSS_APIC_MEM_WDW)) >> 24;

  //
  // IO APIC ID is at APIC Identification Register [27:24]
  //
  if ((ItssHandle->IoApicConfig->IoApicId != IoApicId) && (ItssHandle->IoApicConfig->IoApicId < 0x10)) {
    //
    // Program APIC ID
    //
    MmioWrite8 ((ItssHandle->ItssPrivateConfig->IoApicAddress + R_ITSS_APIC_MEM_IDX), 0);
    MmioWrite32 ((ItssHandle->ItssPrivateConfig->IoApicAddress + R_ITSS_APIC_MEM_WDW), (UINT32) (ItssHandle->IoApicConfig->IoApicId << 24));
  }

  if (ItssHandle->IoApicConfig->IoApicEntry24_119 == FALSE) {
    //
    // Program IOAPIC maximal entry to 24 in PCR[ITSS] + 31FCh[9]
    // and in IOAPIC Version Register
    //
    ItssHandle->PciPcrAccess->AndThenOr16 (ItssHandle->PciPcrAccess, R_ITSS_PCR_GIC, 0xFFFF, (UINT16) B_ITSS_PCR_GIC_MAX_IRQ_24);
    MmioWrite8 ((ItssHandle->ItssPrivateConfig->IoApicAddress + R_ITSS_APIC_MEM_IDX), 1);
    MmioWrite32 ((ItssHandle->ItssPrivateConfig->IoApicAddress + R_ITSS_APIC_MEM_WDW), 0x170020);
  } else {
    //
    // Program IOAPIC maximal entry to 119 in PCR[ITSS] + 31FCh[9]
    // and in IOAPIC Version Register
    //
    ItssHandle->PciPcrAccess->AndThenOr16 (ItssHandle->PciPcrAccess, R_ITSS_PCR_GIC, (UINT16) (~B_ITSS_PCR_GIC_MAX_IRQ_24), 0x0);
    MmioWrite8 ((ItssHandle->ItssPrivateConfig->IoApicAddress + R_ITSS_APIC_MEM_IDX), 1);
    MmioWrite32 ((ItssHandle->ItssPrivateConfig->IoApicAddress + R_ITSS_APIC_MEM_WDW), 0x770020);
  }

  //
  // Build the resource descriptor hob for IOAPIC address resource.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT    |
     EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
     EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    ItssHandle->ItssPrivateConfig->IoApicAddress,
    0x1000
    );
  BuildMemoryAllocationHob (
    ItssHandle->ItssPrivateConfig->IoApicAddress,
    0x1000,
    EfiMemoryMappedIO
    );
}

