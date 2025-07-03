# Self-Signed X.509 Certificate for Capsule Signing

> This document is based on [Step by step to generate sample self-signed X.509 certificate chain and sign data with PKCS7 structure](https://github.com/tianocore/edk2/blob/master/BaseTools/Source/Python/Pkcs7Sign/Readme.md) to create the sample self-signed 3-layer X.509 certificate chain.

## Revision History

| Revision Number | Description      | Revision Date |
| --------------- | ---------------- | ------------- |
| 1.0             | Initial Release. | Oct 22, 2024  |

## Introduction

- Following the instructions of the capsule signing key under `EDK-II`, it is required to generate the 3-layer X.509 certificate chain.
- Capsule file would be signed with `PKCS#7` format data.
  - Verify the signed data within `Pkcs7Verify ()` function in `CryptoPkg/Library/BaseCryptLib/Pk/CryptPkcs7VerifyCommon.c`.
  - Two flags would be set during the capsule verification:
    - `PARTIAL_CHAIN`: At least one certificate in trusted store.
    - `NO_CHECK_TIME`: Do not check the certificate validity against current time.
- The intension of this document is used to introduce the tool (`CapsuleSigningKeyGenerator.py`) of generating 3-layer X.509 certificate chain automatically with configuration file `KeyConfig.toml` and integrate the root CA certificate into BIOS.
- This tool has the external dependency on `OpenSSL` program.
  - Tested on the version `v3.0.9`.

## Modules

- `Config/Openssl.cfg`:
  - Configuration file for OpenSSL program.
  - No need to modify, unless require to customize the settings.
- `Config/KeyConfig.toml`:
  - Abstract the key config items into configuration file.
  - See the below section for detailed description of each property.
- `CapsuleSigningKeyGenerator.py`:
  - `_KeyConfig`:
    - Class to provide the configuration of signing key.
  - `_KeyInfo`:
    - Class to provide the information of signing key.
  - `_OpenSSLHandler`:
    - Class to provide the APIs for executing commands of OpenSSL program.
  - `_CA`:
    - Class to provide the certificate authority (CA) related operations.

## Configuration

> Signing key could be configured as below properties.

- `Extensions`:
  - Correspond extensions section name mapping into `Openssl.cfg`.
  - No need to modify, unless require to change the extensions information of certificate.
- `KeyName`:
  - Name of this key. (It shall be unique for each section.)
  - Key name would be used for the output filename.
- `KeyLength`:
  - Size of private key in bits.
  - Default to `3072`, it could be changed into `1024`/`2048`/`3072`/`4096` as needed.
- `HashAlgorithm`:

  > Higher size of private key might require longer length of hash supported.

  - Hash algorithm of the signature for signing certificate.
  - Default to `sha384`, it could be changed as needed.

- `Validity`:
  - Number of days to certify the certificate.
  - Default to `18250` (50 years), it could be changed as needed.
- `Passphrase`:
  - Password to protect the private key.
  - Enter the password start with prefix `pass:` directly.
  - It is possible to set the password as environment variable and start with prefix `env:`.
  - It could be changed as needed.
- `CountryName`:
  - Name of the country of the certificate.
  - Default to `US`, it could be changed as needed.
- `StateOrProvinceName`:
  - State of province name of the certificate.
  - Default to `CA`, it could be changed as needed.
- `LocalName`:
  - Local name of the certificate.
  - Default to `SC`, it could be changed as needed.
- `Organization`:
  - Name of organization.
  - Default to `Intel` and it could be changed as needed.
- `OrganizationalUnitName`:
  - Name of unit within the organization.
  - Default to `Client` and it could be changed as needed.
- `CommonName`:
  - Unique name of this certificate.
  - It cannot have duplicated name or the script would fail to complete the key generation.

## Usage

1. Environment.
   - Install the `toml` Python [package]([toml · PyPI](https://pypi.org/project/toml/)) via the command `python -m pip install toml`.
   - If the tool is running under POSIX-based OS, install the `wine`.
2. Add the path of toolkit into `PYTHONPATH`.

   ```shell
   //
   // For Windows-based OS.
   //
   set PYTHONPATH=C:\Path\To\Toolkit\;%PYTHONPATH%

   //
   // For POSIX-based OS.
   //
   export PYTHONPATH="/Path/To/Toolkit/:${PYTHONPATH}"
   ```

3. Modify the key configuration file `Config/KeyConfig.toml` if needed.
4. Generate the signing key for capsule file with below command.

   ```shell
   python CapsuleSigningKeyGenerator.py -t <PATH> -c <PATH>
   ```

   | Command                 | Description                                                        | Mandatory |
   | ----------------------- | ------------------------------------------------------------------ | --------- |
   | `-t`/`-T`/`--sign-tool` | Path to the OpenSSL program built with Windows executable program. | V         |
   | `-c`/`-C`/`--config`    | Path to the tool customized configuration file for signing keys.   | V         |

5. Get the output signing key files under `Output` directory.
   > Note: Keep the generated 3-layer signing key files as needed.
   - `*.key`: RSA key-pair file with `AES-256` encryption on private key.
   - `*.csr`: Certificate request file for CA to sign. (Self-signed root CA would not have this file.)
   - `*.crt`: Certificate file within private key in `PEM` form.
   - `*.cer`: Certificate file without private key in `DER` form.
   - `*.pfx`: PKCS#12 formatted file within private key and got protected by password.
   - `*.pem`: Private key without the passphrase protected in `PEM` form.
   - `*.pub.pem`: Public key of certificate in `PEM` form.

## Integration

> Note: If you have multiple certificates need to integrate, convert all of them into PCD buffer and put each of its PCD value sequence-by-sequence.

- To integrate the root CA public key into BIOS for checking the validity of capsule.
- It is required to convert the certificate buffer into array of PCD (`gFmpDevicePkgTokenSpaceGuid.PcdFmpDevicePkcs7CertBufferXdr`) in` XDR` format.

  - `BinToPcd.py` script is provided under `BaseTools/Scripts/BinToPcd.py`.

  ```shell
  python BinToPcd.py -i <PATH> -p <TOKEN.VALUE> -x -o <PATH>
  ```

  | Command | Description                                                                                              | Mandatory |
  | ------- | -------------------------------------------------------------------------------------------------------- | --------- |
  | `-i`    | Path to the certificate file without private key in DER format. (`*.cer`)                                | V         |
  | `-p`    | Assign the PCD value with specific token. (`gFmpDevicePkgTokenSpaceGuid.PcdFmpDevicePkcs7CertBufferXdr`) | V         |
  | `-x`    | Encode PCD with `XDR` format to indicate the length of the buffer.                                       | V         |
  | `-o`    | Path to the output include file. (`*.inc`)                                                               | V         |

- Include the above `*.inc` file into each FMP instance to assign the PCD value of each module.
