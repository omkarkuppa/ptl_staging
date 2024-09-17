//// @file
//
//  @copyright
//  INTEL CONFIDENTIAL
//  Copyright (C) 2024 Intel Corporation.
//
//  This software and the related documents are Intel copyrighted materials,
//  and your use of them is governed by the express license under which they
//  were provided to you ("License"). Unless the License provides otherwise,
//  you may not use, modify, copy, publish, distribute, disclose or transmit
//  this software or the related documents without Intel's prior written
//  permission.
//
//  This software and the related documents are provided as is, with no
//  express or implied warranties, other than those that are expressly stated
//  in the License.
//
// @par Specification
////

//-----------------------------------------------------------------------------
//
//  Section:     SaMmioRead64
//
//  Description: Read 64 bits from the Memory Mapped I/O space.
//  Use MMX instruction for atomic access, because some MC registers have side effect.
//
//  @param[in] Address - Memory mapped I/O address.
//
//-----------------------------------------------------------------------------
//UINT64
//SaMmioRead64 (
//  IN  UINTN Address
//  )

ASM_GLOBAL ASM_PFX(SaMmioRead64)
ASM_PFX(SaMmioRead64):
   subl    $8, %esp
   movq    %mm0, (%esp)       //Save mm0 on stack
   movl    %ecx, %edx         //edx = Address
   movq    (%edx), %mm0       //mm0 = [Address]
   movq    %mm0, %rax         //Store mm0 to rax
   movq    (%esp), %mm0       //Restore mm0
   emms
   addl    $8, %esp
   ret

//-----------------------------------------------------------------------------
//
//  Section:     SaMmioWrite64
//
//  Description: Write 64 bits to the Memory Mapped I/O space.
//  Use MMX instruction for atomic access, because some MC registers have side effect.
//
//  @param[in] Address - Memory mapped I/O address.
//  @param[in] Value   - The value to write.
//
//-----------------------------------------------------------------------------

//UINT64
//SaMmioWrite64 (
//  IN UINTN Address,
//  IN UINT64 Value
//  )

ASM_GLOBAL ASM_PFX(SaMmioWrite64)
ASM_PFX(SaMmioWrite64):
   subl    $8, %esp
   movq    %mm0, (%esp)       //Save mm0 on Stack
   movq    %rdx, %mm0         //mm0 = Value
   movl    #ecx, %edx         //edx = Address
   movq    %mm0, (%edx)       //[Address] = Value
   movq    (%esp), %mm0       //Restore mm0
   emms
   addl     $8, %esp
   ret

//-----------------------------------------------------------------------------
//  Intel Silicon View Technology check point interface based on IO port reading
//
//  @param CheckPoint        Check point AH value.
//                           AH = 0x10:  End of MRC State
//                           AH = 0x20:  End of DXE State
//                           AH = 0x30:  Ready to boot before INT-19h or UEFI boot
//                           AH = 0x40:  After OS booting, need a timer SMI trigger to implement (TBD);
//
//  @param PortReading       IO port reading address used for breakpoints
//-----------------------------------------------------------------------------

//VOID
//EFIAPI
//IsvtCheckPoint (
//  IN UINT32          CheckPoint,
//  IN UINT32          PortReading
//  )

ASM_GLOBAL ASM_PFX(IsvtCheckPoint)
ASM_PFX(IsvtCheckPoint):
   pushl    %rax

   // Stack layout at this point:
   //-------------
   // PortReading     RDX
   //-------------
   // CheckPoint      RCX
   //-------------
   // EIP             RSP + 8
   //-------------
   // EAX             RSP
   //-------------
   //-------------

   mov      %cl, %ah         // CheckPoint
   in       %dx, %al         // signal debugger

   popl     %rax
   ret
