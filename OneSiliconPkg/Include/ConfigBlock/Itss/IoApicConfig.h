/** @file
  IoApic policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _IOAPIC_CONFIG_H_
#define _IOAPIC_CONFIG_H_

#define PCH_IOAPIC_CONFIG_REVISION 1
extern EFI_GUID gIoApicConfigGuid;

#pragma pack (push,1)

/**
  The PCH_IOAPIC_CONFIG block describes the expected configuration of the PCH
  IO APIC, it's optional and PCH code would ignore it if the BdfValid bit is
  not TRUE. Bus:device:function fields will be programmed to the register
  P2SB IBDF(P2SB PCI offset R6Ch-6Dh), it's using for the following purpose:
  As the Requester ID when initiating Interrupt Messages to the processor.
  As the Completer ID when responding to the reads targeting the IOxAPI's
  Memory-Mapped I/O registers.
  This field defaults to Bus 0: Device 31: Function 0 after reset. BIOS can
  program this field to provide a unique Bus:Device:Function number for the
  internal IOxAPIC.
  The address resource range of IOAPIC must be reserved in E820 and ACPI as
  system resource.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  UINT32  IoApicEntry24_119     :  1;   ///< 0: Disable; <b>1: Enable</b> IOAPIC Entry 24-119
  /**
    Enable 8254 Static Clock Gating during early POST time. 0: Disable, <b>1: Enable</b>
    Setting 8254CGE is required to support SLP_S0.
    Enable this if 8254 timer is not used.
    However, set 8254CGE=1 in POST time might fail to boot legacy OS using 8254 timer.
    Make sure it is disabled to support legacy OS using 8254 timer.
    @note:
    For some OS environment that it needs to set 8254CGE in late state it should
    set this policy to FALSE and use ItssSet8254ClockGateState  (TRUE) in SMM later.
    This is also required during S3 resume.
    To avoid SMI requirement in S3 reusme path, it can enable the Enable8254ClockGatingOnS3
    and RC will do 8254 CGE programming in PEI during S3 resume with BOOT_SAI.
  **/
  UINT32  Enable8254ClockGating :  1;
  /**
    Enable 8254 Static Clock Gating on S3 resume path. 0: Disable, <b>1: Enable</b>
    This is only applicable when Enable8254ClockGating is disabled.
    If Enable8254ClockGating is enabled, RC will do the 8254 CGE programming on
    S3 resume path as well.
  **/
  UINT32  Enable8254ClockGatingOnS3 :  1;
  UINT32  RsvdBits1             : 29;   ///< Reserved bits
  UINT8   IoApicId;                     ///< This member determines IOAPIC ID. Default is <b>0x02</b>.
  UINT8   Rsvd0[3];                     ///< Reserved bytes
} PCH_IOAPIC_CONFIG;

#pragma pack (pop)

#endif // _IOAPIC_CONFIG_H_
