/** @file
  SPS SPI Definition.

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

@par Specification Reference:
**/

#ifndef _SPS_API_H_
#define _SPS_API_H_

/*

  Binary format:

 ============ +-----------------------------------+----------
              |       SMM_ENTRY_POINT_HEADER      |          |
     SMM      |          InfoTableAddress         |----      |<--
  EntryPoint  |-----------------------------------|    |     |   |
              |         Entry Point Code          |    |     |   |
 ============ +-----------------------------------+ <--  ----    |
              | SMM_ENTRY_POINT_INFORMATION_TABLE |              |
              |         SmmEntryPointSize         |--------------
     SMM      |-----------------------------------|
  InfoTable   | SMM_ENTRY_POINT_INFORMATION_ENTRY |
              |-----------------------------------|
              |             ......                |
              |-----------------------------------|
              |     SMM_ENTRY_POINT_INFO_END      |
 ============ +-----------------------------------+
              |        PE COFF Image              |
              |       ---------------             |
     SPS      |        PE COFF Header             |
   Runtime    |        PE COFF Code (EntryPoint)  |
              |        PE COFF Data               |
              |        PE COFF Reloc              |
 ============ +-----------------------------------+

*/

/*

  SPS Runtime: (Supervisor pages)

 SMBASE+8000->+-----------------------------------+    (SMI)
              |        SMM EntryPoint (0)         |-------------
              |-----------------------------------|             |
              |                                   |             |
              |-----------------------------------|             |
              |        SMM EntryPoint (n-1)       |             |
              +-----------------------------------+             |
                                                                |
  4K aligned->+-----------------------------------+             |
              |        SMM Info Table             |             |
              +-----------------------------------+             |
                                                                |
  4K aligned->+-----------------------------------+             |
              |        SPS Runtime PE Image       |             |
              |    (SpsEntryPoint/PeEntryPoint)   |<-----       |
              |                                   |      |      (Exception)
              |      (SpsExceptionEntryPoint)     |--------------------
              +-----------------------------------+      |      |      |
                                                         |      |      |
  4K aligned->+-----------------------------------+      |      |      |
              |        SPS Stack (0)              |      |      |      |
              |-----------------------------------|      |      |      |
              |                                   |      |      |      |
              |-----------------------------------|      |      |      |
              |        SPS Stack (n-1)            |      |      |      |
              +-----------------------------------+      |      |      |
                                                         |      |      |
  4K aligned->+-----------------------------------+      |      |      |
              |        SPS Exception Stack (0)    |      |      |      |
              |-----------------------------------|      |      |      |
              |                                   |      |      |      |
              |-----------------------------------|      |      |      |
              |        SPS Exception Stack (n-1)  |      |      |      |
              +-----------------------------------+      |      |      |
                                                         |      |      |
  BIOS SMM Runtime: (User pages)                         |      |      |
                                                         |      |      |
              +-----------------------------------+      |      |      |
              |        SMM Handler PE Image       |(Init)|      |      |
              |           (PeEntryPoint)          |------       |      |
              |                                   |             |      |
              |          (SmiEntryPoint)          |<------------       |
              |      (SmiExceptionEntryPoint)     |<-------------------
              +-----------------------------------+

              +-----------------------------------+
              |        SMM Stack (0)              |
              |-----------------------------------|
              |                                   |
              |-----------------------------------|
              |        SMM Stack (n-1)            |
              +-----------------------------------+

              +-----------------------------------+
              |        SMM Exception Stack (0)    |
              |-----------------------------------|
              |                                   |
              |-----------------------------------|
              |        SMM Exception Stack (n-1)  |
              +-----------------------------------+

*/

typedef UINT32 SPS_STATUS;

#define SPS_SUCCESS            0x00000000
#define SPS_INVALID_PARAMETER  0x80000002
#define SPS_UNSUPPORTED        0x80000003

typedef struct {
  VOID      *ImageBase;
  UINT32    CodeSegment;
  UINT32    TaskSegment;
  //
  // Assumption:
  //   CodeSegment      = The ring0 long mode segment
  //   DataSegment      = CodeSegment + 0x8
  //   Ring3CodeSegment = CodeSegment + 0x20
  //   Ring3DataSegment = CodeSegment + 0x28
  //
} SPS_BIOS_CONTEXT;

typedef struct {
  UINTN     SpsExceptionEntryPoint[32];
} SPS_KERNEL_CONTEXT;

/*
  This is SPS PE/COFF image entrypoint declared in the standard PE/COFF header.

  This function initializes SPS and returns the SPS context.

  This function is called by BIOS SMM code when BIOS SMM constructs the environment.

  This function is invoked by CALL instruction from BIOS SMM.
  This function uses RET instruction to return to BIOS SMM.
*/
typedef
SPS_STATUS
(EFIAPI *SPS_ENTRY_POINT) (
  IN  SPS_BIOS_CONTEXT        *BiosContext,
  OUT SPS_KERNEL_CONTEXT      **KernelContext
  );

/*
  This is BIOS SMI entry point, declared by SMM_ENTRY_POINT_INFO_USER_MODE_ENTRY_POINT.
  It is invoked by SPS in every SMI.

  This function is executed in ring3 environment.

  This function is invoked by SYSEXIT instruction from SPS.
  This function uses SYSENTER to return to SPS.
*/
typedef
VOID
(EFIAPI *SMI_ENTRY_POINT) (
  VOID
  );

/*
  This is BIOS SMI exception entry point, declared by SMM_ENTRY_POINT_INFO_USER_MODE_EXCEPTION_ENTRY_POINT.

  BIOS SMM code should register SPS exception handler to IDT.
  When the exception happens, the SPS exception handler should be called.
  If the SPS exception handler can process the exception based upon the policy,
  such as MSR read or MSR write, the SPS exception handler returns back to BIOS SMM directly.
  If the SPS exception handler cannot process the exception, such as machine check,
  page fault, or double fault, the SPS exception handler calls BIOS registered
  exception handler to handle the event, such as machine check, or dump the exception
  context and dead loop, such as double fault.

  This function is executed in ring3 environment.

  This function is invoked by SYSEXIT instruction from SPS.
  This function uses SYSENTER to return to SPS.

  Please note: the X64 calling convention uses RCX for InterruptType and RDX for SystemContext.
  At same time, SYSEXIT uses RCX for RIP and RDX for RSP.
  The BIOS SMM cannot use a C code to handle the parameter directly.
  The SPS pushes SystemContext and InterruptType to stack like IA32 calling convention.
  The BIOS SMM should use assembly code to move SystemContext and InterruptType
  from stack to general purpose register then to call C code.
*/
typedef
VOID
(EFIAPI *SMI_EXCEPTION_ENTRY_POINT) (
  IN EFI_EXCEPTION_TYPE  InterruptType,
  IN EFI_SYSTEM_CONTEXT  SystemContext
  );


#endif
