/**@file
    Unit test cases of GetPortNum.

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

#include "GetPortNumMock.h"

/**
  Unit test for GetPortNum.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetPortNum_Success (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS               Status;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  UINT8                    *Node;
  UINT8                    DevicePathLen;
  UINT16                   Port;
  UINT16                   PortMultiplier;

  //
  // Mock DevicePath
  //
  ACPI_HID_DEVICE_PATH PciRoot = {  // PciRoot
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    0x0A0341D0,
    0
  };
  PCI_DEVICE_PATH AhciEp = {  // AhciEp
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    0x17,
    0
  };
  SATA_DEVICE_PATH SataNode = {  // Sata
    {
      MESSAGING_DEVICE_PATH,
      MSG_SATA_DP,
      {
        (UINT8)(sizeof (SATA_DEVICE_PATH)),
        (UINT8)((sizeof (SATA_DEVICE_PATH)) >> 8)
      }
    },
    4,
    0xffff,
    0
  };
  EFI_DEVICE_PATH_PROTOCOL EndNode = {  // End
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (sizeof (EFI_DEVICE_PATH_PROTOCOL)),
      (UINT8) ((sizeof (EFI_DEVICE_PATH_PROTOCOL)) >> 8)
    }
  };
  DevicePathLen = sizeof (PciRoot) + sizeof (AhciEp) + sizeof (SataNode) + sizeof (EndNode);
  DevicePath = AllocatePool (DevicePathLen);
  Node = (UINT8*) DevicePath;
  CopyMem (Node, &PciRoot, sizeof (ACPI_HID_DEVICE_PATH));
  Node += sizeof (ACPI_HID_DEVICE_PATH);
  CopyMem (Node, &AhciEp, sizeof (PCI_DEVICE_PATH));
  Node += sizeof (PCI_DEVICE_PATH);
  CopyMem (Node, &SataNode, sizeof (SATA_DEVICE_PATH));
  Node += sizeof (SATA_DEVICE_PATH);
  CopyMem (Node, &EndNode, sizeof (EFI_DEVICE_PATH_PROTOCOL));

  Status = GetPortNum (DevicePath, &Port, &PortMultiplier);
  FreePool (DevicePath);

  UT_ASSERT_EQUAL (Status, EFI_SUCCESS);
  UT_ASSERT_EQUAL (Port, 4);
  UT_ASSERT_EQUAL (PortMultiplier, 0xffff);
  return UNIT_TEST_PASSED;
}

/**
  Unit test for GetPortNum.

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
UnitTestGetPortNum_Fail (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_STATUS               Status;
  EFI_DEVICE_PATH_PROTOCOL *DevicePath;
  UINT8                    *Node;
  UINT8                    DevicePathLen;
  UINT16                   Port;
  UINT16                   PortMultiplier;

  //
  // Mock DevicePath
  //
  ACPI_HID_DEVICE_PATH PciRoot = {  // PciRoot
    {
      ACPI_DEVICE_PATH,
      ACPI_DP,
      {
        (UINT8) (sizeof (ACPI_HID_DEVICE_PATH)),
        (UINT8) ((sizeof (ACPI_HID_DEVICE_PATH)) >> 8)
      }
    },
    0x0A0341D0,
    0
  };
  PCI_DEVICE_PATH PciBridge = {  // PciBridge
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    0x1D,
    0
  };
  PCI_DEVICE_PATH NvmeHcEp = {  // NvmeHcEp
    {
      HARDWARE_DEVICE_PATH,
      HW_PCI_DP,
      {
        (UINT8) (sizeof (PCI_DEVICE_PATH)),
        (UINT8) ((sizeof (PCI_DEVICE_PATH)) >> 8)
      }
    },
    0,
    0
  };
  NVME_NAMESPACE_DEVICE_PATH NvmeNode = {  // Nvme
    {
      MESSAGING_DEVICE_PATH,
      MSG_NVME_NAMESPACE_DP,
      {
        (UINT8)(sizeof (NVME_NAMESPACE_DEVICE_PATH)),
        (UINT8)((sizeof (NVME_NAMESPACE_DEVICE_PATH)) >> 8)
      }
    },
    1,
    0x00253855019EBB2D
  };
  EFI_DEVICE_PATH_PROTOCOL EndNode = {  // End
    END_DEVICE_PATH_TYPE,
    END_ENTIRE_DEVICE_PATH_SUBTYPE,
    {
      (UINT8) (sizeof (EFI_DEVICE_PATH_PROTOCOL)),
      (UINT8) ((sizeof (EFI_DEVICE_PATH_PROTOCOL)) >> 8)
    }
  };
  DevicePathLen = sizeof (PciRoot) + sizeof (PciBridge) + sizeof (NvmeHcEp) + sizeof (NvmeNode)+ sizeof (EndNode);
  DevicePath = AllocatePool (DevicePathLen);
  Node = (UINT8*) DevicePath;
  CopyMem (Node, &PciRoot, sizeof (ACPI_HID_DEVICE_PATH));
  Node += sizeof (ACPI_HID_DEVICE_PATH);
  CopyMem (Node, &PciBridge, sizeof (PCI_DEVICE_PATH));
  Node += sizeof (PCI_DEVICE_PATH);
  CopyMem (Node, &NvmeHcEp, sizeof (PCI_DEVICE_PATH));
  Node += sizeof (PCI_DEVICE_PATH);
  CopyMem (Node, &NvmeNode, sizeof (NVME_NAMESPACE_DEVICE_PATH));
  Node += sizeof (NVME_NAMESPACE_DEVICE_PATH);
  CopyMem (Node, &EndNode, sizeof (EFI_DEVICE_PATH_PROTOCOL));

  Status = GetPortNum (DevicePath, &Port, &PortMultiplier);
  FreePool (DevicePath);

  UT_ASSERT_EQUAL (Status, EFI_NOT_FOUND);
  return UNIT_TEST_PASSED;
}

