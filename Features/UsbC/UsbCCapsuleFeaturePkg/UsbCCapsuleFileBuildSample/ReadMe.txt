This file describe how to build system firmware Capsules with CapsuleBuild.bat

There are several Capsule targets supported:
1. Standalone Retimer Device Capsule - A Capsule to update Retimer only.
   ESRT entry GUID: 2FE2CBFC-B9AA-4A93-AB5B-40173B581C42

   a. Involved payloads: ITbtRetimerPayload.bin (ITbtRetimerPayload.bin is generated according to ITbtRetimerPayloadConfig.ini)
      To build Integrated Thunderbolt Retimer Capsule, ITbtRetimerPayloadConfig.ini must be a valid config file.

   b. Involved payloads: DTbtRetimerPayload.bin (DTbtRetimerPayload.bin is generated according to DTbtRetimerPayloadConfig.ini)
      To build Discrete Thunderbolt Retimer Capsule, DTbtRetimerPayloadConfig.ini must be a valid config file.

2. Standalone Discrete TBT Device Capsule - A Capsule to update Discrete TBT device only.
   ESRT entry GUID: 86A885EE-D71E-2ED6-0FC1-9D6CCC9677EB
   Involved payloads: DTbtPayload.bin (DTbtPayload.bin is generated according to DTbtPayloadConfig.ini)
   To build Discrete Thunderbolt Capsule, DTbtPayloadConfig.ini must be a valid config file.

See [How to Use] below for further details.

[Pre-Requisites]
1. Windows operating system
2. Having Python3 installed
3. OpenSSL.exe. By default the tool will look for OpenSSL.exe under the path "%WORKSPACE%\Binaries\Tools\OpenSSL".
   You can change the location by setting environment variable OPEN_SSL_SIGN_TOOL_PATH that points to the OpenSSL folder.

4. If you want to test Capsule update in Windows, having WDK/SDK installed to generate Windows update driver.
   The script to generate Windows update driver requires below files:
   a. signtool.exe under
      C:\Program Files (x86)\Windows Kits\10\bin\x64
      or
      C:\Program Files (x86)\Windows Kits\8.1\bin\x64
      or
      C:\Program Files (x86)\Windows Kits\10\bin\*\x64

   b. Inf2Cat.exe under
      C:\Program Files (x86)\Windows Kits\10\bin\x86
      or
      C:\Program Files (x86)\Windows Kits\10\bin\*\x86

[Apply Capsule in UEFI Shell]

1. Put Capsule file and CapsuleApp.efi in USB storage.
2. In EFI Shell, switch file system to the USB storage (e.g. Fs0:)
3. fs0:> CapsuleApp <Capsule file name (e.g. XXXSystemFwxxxx.Cap)>
4. System should restart
5. After reboot, a progress bar which indicates the update is in progress should be observed with Intel logo.

[Apply Capsule update package in Windows]

Pre-Requisites:
        1. Disable Secure Boot in BIOS setup question to allow firmware upgrade.
        2. Please check the system date and time if it is correct or not.
        3. Set "testsigning" on through bcdedit using below admin command prompt:
                "bcdedit /set testsigning on" and reboot the system.
        4. Confirm the changes by typing "bcdedit" in cmd prompt. Below status should be observed:
                  testsigning                       Yes

Installing the Firmware driver package:
        1. Copy XXXSystemFwxxxx directory to SUT
        2. Do the below things to install the certificate file.
                  a. Double click on the security catalog file (cat file) from package.
                  b. Click on View Signature.
                  c. Click on View Certificate followed by clicking on Install Certificate.
                  d. For Store Location, choose "Local Machine". Click next and Manually browse and select "Trusted Root Certification Authorities".
                  e. Click OK and next to install the certificate on the system for one time.
                  f. You should get the popup message "The Import was Successful"
        3. Enter to driver package created. Run "pnputil -i -a XXXSystemFwxxxx.inf" as Administrator. Then restart system.
