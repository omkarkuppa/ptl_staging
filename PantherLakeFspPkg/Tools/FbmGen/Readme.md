# [Tool Introduction]

FbmGen tool is used to generate FBM binary according to FSP.fd to support FBM Signing,
and extract Fbm.bin from IFWI/BIOS image.

## [How to Use]

## Usage

1. To generate FBM binary, run following command:
    python GenFbm.py -c fbmConfig.txt -o dirPath -f FSP.fd --ltsign-tool-path ltsign.exe --openssl-tool-path openssl.exe
2. To extract the FBM binary from IFWI/BIOS image, run following command:
    python GenFbm.py -i ifwi.bin

## Options

-f/--fspFile        input the directory of FSP.fd
-c/--configFile     input the fbm platform related config file
-o/--outputDir      designate the output directory of FBM binary
-i/--imageDump      designate BIOS/IFWI image path to extract Fbm.bin
--ltsign-tool-path  designate path to LT Sign tool
--openssl-tool-path designate Path to OpenSSL tool

## [Maintainers]

Wang, Fan <fan.wang@intel.com>
Zhu, Chen <chen.zhu@intel.com>
