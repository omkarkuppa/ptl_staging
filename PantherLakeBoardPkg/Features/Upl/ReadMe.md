<!-- @file
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
-->


# Universal Payload Wrapper v0.9
##
Spec v0.9: https://universalscalablefirmware.github.io/documentation/2_universal_payload.html

## Term
  | Name | Description                                            |
  |------|--------------------------------------------------------|
  | PPL  | Platform Payload, can't sharable with other bootloader |
  | UPL  | Universal Payload, can sharable with other bootloader  |

## BIOS Boot Flow
  ```
                                                                                           HOBs
  SEC -> PEI -> DXE -> DXE IPL -> UefiPayloadPkg\PayloadLoaderPeim\PayloadLoaderPeim.c ----------> Load UniversalPayload.elf -> Dispatch UPL and PPL Drivers -> Operation System
  ```

## BIOS Build Flow
  ```
  Binary Format
  +-------------------+
  | BIOS.rom          |
  +-------------------+
  | Other Firmware    |
  +-------------------+
  | ...               |  FMMT                                                                                                                                                                        UniversalPayloadBuild.py
  +-------------------+<----------------+-----------------------+  GenFfs    +-----------------------+  Rsa2048Sha256 Sign +-----------------------+  LzmaCompress +----------------------+  GenSec +--------------------------------+
  |                   |                 | EDK2 FFS Header       |<-----------| Rsa2048Sha256 Hash    |<--------------------| UniversalPayload.lzma |<--------------| EDK2 SEC Header      |<--------| UniversalPayload.elf           |
  | RAW Data          |                 +-----------------------+            +-----------------------+                     +-----------------------+               +----------------------+         +--------------------------------+
  |                   |                 | Rsa2048Sha256 Hash    |            | UniversalPayload.lzma |                                                             | UniversalPayload.elf |         | upld_info                      |
  |                   |                 +-----------------------+            +-----------------------+                                                             +----------------------+         +--------------------------------+
  |                   |                 | UniversalPayload.lzma |                                                                                                  | upld_info            |         | upld.uefi_fv                   |
  +-------------------+<----------------+-----------------------+                                                                                                  +----------------------+         +--------------------------------+
  | ...               |                                                                                                                                            | upld.uefi_fv         |         | upld.bds_fv                    |
  +-------------------+                                                                                                                                            +----------------------+         +--------------------------------+
  | Other Firmware    |                                                                                                                                            | upld.bds_fv          |         | upld.network_fv                |
  +-------------------+                                                                                                                                            +----------------------+         +--------------------------------+
                                                                                                                                                                   | upld.network_fv      |
                                                                                                                                                                   +----------------------+
  ```