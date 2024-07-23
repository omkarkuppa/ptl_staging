# [Tool Introduction]

BsssGen tool is used to generate BSSS(BIOS Specific Segment Structure) binary to support FBM Signing.
BSSS binary is inserted into BPM when calling BpmGen tool to generate BPM,
or patched into BIOS of 4G - 4K address in NO ACM case.

## [How to Use]

## Usage

1. To generate BSSS binary(to be inserted into BPM), run following command:
    python BsssGen.py -Fd CLIENTBIOS.fd -FspmLP 0x1 -o bsis.bin -d bsis.bin
2. To generate FSPE binary(to be patched into BIOS of 4G - 4K address in NO ACM case), run following command:
    python BsssGen.py -Fd CLIENTBIOS.fd -FspmLP 0x1 -p fspe.bin

## Options

-Fd input the directory of CLIENTBIOS.fd
-FspmLP FSP load policy:
    0: FSP-O skips loading FSP-M
    1: FSP-O loads FSP-M
-o designate the output of FBM binary
-d designate the FBM binary to be parsed
-p designate the output of FSPE binary

## [Maintainers]

Wang, Fan <fan.wang@intel.com>
Zhu, Chen <chen.zhu@intel.com>
