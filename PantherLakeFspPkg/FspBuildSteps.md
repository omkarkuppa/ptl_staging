Intel is a trademark or registered trademark of Intel Corporation or its
subsidiaries in the United States and other countries.
*Other names and brands may be claimed as the property of others.
Copyright (c) 2016 - 2021, Intel Corporation. All rights reserved.

# FSP build steps for building PantherLake FSP binary

In order to build xxxLake FSP binary, following packages needs to be
downloaded along with the FSP SDK open source packages
* PantherLakeFspPkg --> contains all the PantherLake FSP code
* OneSiliconPkg --> contains all the PantherLake Silicon initialization code

Using BuildFsp.cmd (or BuildFsp.sh) is recommended.
Please refer to the usage notes by command - "BuildFsp.cmd /h"

## FSP SDK open source packages and hash versions
SDK open source packages may need some customization to build this version of FSP,
please contact Intel to get the github link for the EDK2 packages used.