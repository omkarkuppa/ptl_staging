## @file
#  Script to generate the signed BiosGuard Update Package (BGUP).
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
#  @par Specification
#
##

import os
import argparse
from ctypes import *

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to generate the BiosGuard Update Package(BGUP).\n'

#
# Default value
#
DEFAULT_EC_SVN_VALUE        : int = 0xFFFF_FFFF
DEFAULT_BGUP_OUTPUT_FILENAME: str = 'update_package.BIOS_Guard'

#
# Signing method
#
TYPE_SIGN_SELF_TOOL   : str       = 'Self'
TYPE_SIGN_OPENSSL_TOOL: str       = 'OpenSSL'
TYPE_SIGN_LIST        : List[str] = [
    TYPE_SIGN_SELF_TOOL,
    TYPE_SIGN_OPENSSL_TOOL,
    ]

DEFAULT_MODULES_FILE_PREFIX: str = 'Modulus='
DEFAULT_RSA_EXPONENT       : int = 65537

# HASH Algorithm
HASH_SHA256: str = 'sha256'
HASH_SHA384: str = 'sha384'

#
# Signing algorithm
#
# PKCS1 1.5 digital signature [SHA256, RSA2048]
ALG_RSA2048_SHA256_1_5_STR  : str = 'RSA2048_SHA256_1_5'
# PKCS1 v2.1 EMSA/PSS digital signature [SHA256, RSA2048]
ALG_RSA2048_SHA256_V_2_1_STR: str = 'RSA2048_SHA256_v_2_1'
# PKCS1 1.5 digital signature [SHA256, RSA3072]
ALG_RSA3072_SHA256_1_5_STR  : str = 'RSA3072_SHA256_1_5'
# PKCS1 v2.1 EMSA/PSS digital signature [SHA256, RSA3072]
ALG_RSA3072_SHA256_V_2_1_STR: str = 'RSA3072_SHA256_v_2_1'
# PKCS1 1.5 digital signature [SHA384, RSA3072]
ALG_RSA3072_SHA384_1_5_STR  : str = 'RSA3072_SHA384_1_5'
# PKCS1 v2.1 EMSA/PSS digital signature [SHA384, RSA3072]
ALG_RSA3072_SHA384_V_2_1_STR: str = 'RSA3072_SHA384_v_2_1'

#
# BgupC data
#
BGUPC_VERSION_KEY : str  = 'BgupC'
ALG_CODE_NAME_KEY : str  = 'Algorithm'
HASH_ALGORITHM_KEY: str  = 'Hash'
BGUPC_HEADER_DATA : dict = {
    ALG_RSA2048_SHA256_1_5_STR  : {
        BGUPC_VERSION_KEY : BGUPC_VERSION_1,
        ALG_CODE_NAME_KEY : BGUPC_RSA2048_SHA256_PKCS1_1P5,
        HASH_ALGORITHM_KEY: HASH_SHA256,
        },
    ALG_RSA2048_SHA256_V_2_1_STR: {
        BGUPC_VERSION_KEY : BGUPC_VERSION_1,
        ALG_CODE_NAME_KEY : BGUPC_RSA2048_SHA256_PKCS1_V2P1,
        HASH_ALGORITHM_KEY: HASH_SHA256,},
    ALG_RSA3072_SHA256_1_5_STR  : {
        BGUPC_VERSION_KEY : BGUPC_VERSION_2,
        ALG_CODE_NAME_KEY : BGUPC_RSA3072_SHA256_PKCS1_1P5,
        HASH_ALGORITHM_KEY: HASH_SHA256,
        },
    ALG_RSA3072_SHA256_V_2_1_STR: {
        BGUPC_VERSION_KEY : BGUPC_VERSION_2,
        ALG_CODE_NAME_KEY : BGUPC_RSA3072_SHA256_PKCS1_V2P1,
        HASH_ALGORITHM_KEY: HASH_SHA256,
        },
    ALG_RSA3072_SHA384_1_5_STR  : {
        BGUPC_VERSION_KEY : BGUPC_VERSION_3,
        ALG_CODE_NAME_KEY : BGUPC_RSA3072_SHA384_PKCS1_1P5,
        HASH_ALGORITHM_KEY: HASH_SHA384,
        },
    ALG_RSA3072_SHA384_V_2_1_STR: {
        BGUPC_VERSION_KEY : BGUPC_VERSION_3,
        ALG_CODE_NAME_KEY : BGUPC_RSA3072_SHA384_PKCS1_V2P1,
        HASH_ALGORITHM_KEY: HASH_SHA384,
        },
    }

#
# Script exit code
#
EXIT_SUCCESS: int = 0x00

class BgslBinGenerator (object):
    def __init__ (
        self,
        Bgsl2BinPath: Union[str, os.PathLike],
        BgslPath    : Union[str, os.PathLike],
        OutputPath  : Union[str, os.PathLike],
        ) -> None:
        """ Class to compile the BGSL script into binary file.

        Args:
            Bgsl2BinPath (Union[str, os.PathLike]):
                The path of Bgsl2Bin tool as the compiler.
            BgslPath (Union[str, os.PathLike]):
                The path of BGSL script.
            OutputPath (Union[str, os.PathLike]):
                The path of output BGSL binary file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BgslPath    : str = AbsPath (BgslPath)
        self.__Bgsl2BinPath: str = AbsPath (Bgsl2BinPath)
        self.__OutputPath  : str = AbsPath (OutputPath)
        self.__OutputDir   : str = os.path.dirname (self.__OutputPath)

        self.__PreCheck ()

    @property
    def Filename (self) -> str:
        """ Filename of this compiled BGSL script binary file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The string of filename.
        """
        return os.path.basename (self.__OutputPath)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__BgslPath,
            self.__Bgsl2BinPath,
            ]
        CheckFileListExist (CheckPathList)

    def Compile (self) -> int:
        """ Compile the BGSL script into binary file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exit code of Bgsl2Bin program.
        """
        TouchDir (self.__OutputDir)

        Cmd: List[str] = [
            f'{self.__Bgsl2BinPath}',
            f'{self.__BgslPath}',
            f'{self.__OutputPath}',
            ]

        ExitCode, _, _ = ExecWinExeCmd (Cmd)

        return ExitCode

class BgupWithDataGenerator (object):
    def __init__ (
        self,
        BgpbPath   : Union[str, os.PathLike],
        BgslBinPath: Union[str, os.PathLike],
        OutputPath : Union[str, os.PathLike],
        PlatformId : str,
        BiosSvn    : int,
        DataPath   : Union[str, os.PathLike] = None,
        EcSvn      : int                     = DEFAULT_EC_SVN_VALUE,
        UseSfam    : bool                    = True,
        UseFtu     : bool                    = True,
        ) -> None:
        """ Class to call the BGPB program to generate BGUP with Data.

        +--------------------+
        |     BGUP Header    |
        +--------------------+
        |        BGSL        |
        +--------------------+
        |     Data Buffer    |
        +--------------------+

        Args:
            BgpbPath (Union[str, os.PathLike]):
                The path of BGPB program.
            BgslBinPath (Union[str, os.PathLike]):
                The path of binary file compiled from BGSL.
            OutputPath (Union[str, os.PathLike]):
                The path of the output BGUP with data.
            PlatformId (str):
                The platform ID of this BGUP. (Maximum 16 characters)
            BiosSvn (int):
                The BIOS SVN. (Should be UINT32)
            DataPath (Union[str, os.PathLike], optional):
                The path of data buffer for this BGUP.
                Defaults to None.
            EcSvn (int, optional):
                THE EC SVN. (Should be UINT32)
                Defaults to DEFAULT_EC_SVN_VALUE.
            UseSfam (bool, optional):
                True  - Need to update the SFAM covered range.
                False - No need to update the SFAM covered range.
                Defaults to True.
            UseFtu (bool, optional):
                True  - Script use the Fault Tolerant Update (FTU).
                False - Script not use the Fault Tolerant Update (FTU).
                Defaults to True.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BgpbPath   : str = AbsPath (BgpbPath)
        self.__BgslBinPath: str = AbsPath (BgslBinPath)
        self.__DataPath   : str = AbsPath (DataPath)
        self.__OutputPath : str = AbsPath (OutputPath)
        self.__OutputDir  : str = os.path.dirname (self.__OutputPath)

        self.__PlatformId: str = PlatformId
        self.__BiosSvn   : int = ToInt (BiosSvn)
        self.__EcSvn     : int = ToInt (EcSvn)
        self.__UseSfam   : str = str (ToBool (UseSfam))
        self.__UseFtu    : str = str (ToBool (UseFtu))

        self.__PreCheck ()

    @property
    def Filename (self) -> str:
        """ Filename of this BGUP with data file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The string of filename.
        """
        return os.path.basename (self.__OutputPath)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            None

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__BgpbPath,
            self.__BgslBinPath,
            ]
        CheckFileListExist (CheckPathList)

        CheckPathList = [
            self.__DataPath,
            ]
        CheckFileListExist (CheckPathList, IsBypassEmpty = True)

        if not IsUint32 (self.__BiosSvn):
            raise ValueError (f'BIOS SVN should be UINT32: {self.__BiosSvn}')

        if not IsUint32 (self.__EcSvn):
            raise ValueError (f'EC SVN should be UINT32: {self.__EcSvn}')

    def Build (self) -> int:
        """ Function to call the BGPB program.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exit code of the BGPB.
        """
        TouchDir (self.__OutputDir)
        SourcePath: str = \
            JoinPath (self.__OutputDir, DEFAULT_BGUP_OUTPUT_FILENAME)

        Cmd: List[str] = [
            f'{self.__BgpbPath}',
            '-bios_svn',    f'{self.__BiosSvn}',
            '-ec_svn',      f'{self.__EcSvn}',
            '-use_sfam',    f'{self.__UseSfam}',
            '-use_ftu',     f'{self.__UseFtu}',
            '-platform_id', f'{self.__PlatformId}',
            '-script',      f'{self.__BgslBinPath}',
            ]

        if self.__DataPath:
            Cmd.append ('-data')
            Cmd.append (f'{self.__DataPath}')

        ExitCode, _, _, = ExecWinExeCmd (Cmd, Cwd = self.__OutputDir)

        if ExitCode != EXIT_SUCCESS:
            ErrorException (f'Failed to execute BGPB program.')

        CopyFile (SourcePath, self.__OutputPath)
        RemoveFile (SourcePath)

        return ExitCode

class CertSignatureGenerator (object):
    class _BgupCertHeader (LittleEndianStructure):
        """ Inner class to represent the BGUP Certificate header.

        Note:
            This is used to represent the C struct.
        """
        _pack_  : int  = 1
        _fields_: list = [
            ('Version',   c_uint16),
            ('Reserved',  c_uint16),
            ('Algorithm', c_uint32),
            ]

    def __init__ (
        self,
        BgupWithDataPath: Union[str, os.PathLike],
        SignType        : str,
        Algorithm       : str,
        SignToolPath    : Union[str, os.PathLike],
        PubKeyPath      : Union[str, os.PathLike],
        PriKeyPath      : Union[str, os.PathLike],
        OutputPath      : Union[str, os.PathLike],
        ) -> None:
        """ Class to generate the certificate and signature.

        +--------------------+
        |     BGUPC Header   |
        +--------------------+
        |     Certificate    |
        +--------------------+
        |      Signature     |
        +--------------------+

        Args:
            BgupWithDataPath (Union[str, os.PathLike]):
                The path of BGUP with data file.
            SignType (str):
                The type of signing operation.
                Should be one of TYPE_SIGN_LIST.
            Algorithm (str):
                The type of signing algorithm.
            SignToolPath (Union[str, os.PathLike]):
                The tool use for to sign the input buffer as digital
                certificate.
            PubKeyPath (Union[str, os.PathLike]):
                The path of public key use for certificate part.
            PriKeyPath (Union[str, os.PathLike]):
                The path of private key use for signing to get the
                signature.
            OutputPath (Union[str, os.PathLike]):
                The path of output to generate the BGUP certificate.

        Raises:
            None.

        Returns:
            None.
        """
        self.__SignType    : str = SignType
        self.__Algorithm   : str = Algorithm
        self.__BgupDataPath: str = AbsPath (BgupWithDataPath)
        self.__SignToolPath: str = AbsPath (SignToolPath)
        self.__PubKeyPath  : str = AbsPath (PubKeyPath)
        self.__PriKeyPath  : str = AbsPath (PriKeyPath)
        self.__OutputPath  : str = AbsPath (OutputPath)
        self.__OutputDir   : str = os.path.dirname (self.__OutputPath)

        self.__PreCheck ()

    @property
    def Filename (self) -> str:
        """ Filename of this certificate with signature file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The string of filename.
        """
        return os.path.basename (self.__OutputPath)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            None

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__BgupDataPath,
            self.__SignToolPath,
            self.__PubKeyPath,
            self.__PriKeyPath,
            ]
        CheckFileListExist (CheckPathList)

        IsFound, Type = SearchValueInList (TYPE_SIGN_LIST, self.__SignType)

        if not IsFound:
            raise ValueError (f'Need input support sign type {TYPE_SIGN_LIST}')

        self.__SignType = Type

    def __GetCertHeader (self) -> bytearray:
        """ Get the BGUP certificate header.

        Note:
            Base on different sign flow, BGUPC header have different
            value.

        Args:
            None.

        Raises:
            ValueError:
                Unsupported sign flow.

        Returns:
            bytearray:
                The buffer of BGUPC header.
        """
        Header: self._BgupCertHeader = self._BgupCertHeader ()

        IsFound, Certificate = SearchKeyInDict (BGUPC_HEADER_DATA, self.__Algorithm)
        if not IsFound:
            raise ValueError (
                    f'Unsupported type of sign algorithm. [{self.__Algorithm}]'
                    )

        Header.Version   = BGUPC_HEADER_DATA[Certificate][BGUPC_VERSION_KEY]
        Header.Reserved  = 0x0000
        Header.Algorithm = BGUPC_HEADER_DATA[Certificate][ALG_CODE_NAME_KEY]

        return bytearray (Header)

    def __GetCertBody (self) -> bytearray:
        """ Convert public key modules and exponent to cert key slot.

        Note:
            (1) Assume RSA private key uses the default exponent(65537)
                to do the generation.
            (2) Sample OpenSSL command to generate public key modulus
                text file:
                openssl rsa -pubin -modulus -noout < publickey.pem > modulus.txt

        Args:
            ModuleFilePath (str):
                The file that include the public key module value.

        Raises:
            ValueError:
                The input file is not assigned prefix.

        Returns:
            bytearray:
                BiosGuard certification body modules and exponent.
        """
        ModulePrefix  : str   = DEFAULT_MODULES_FILE_PREFIX
        ModuleFilePath: str   = self.__OutputPath + '.module'
        Buffer        : bytes = bytearray ()

        #
        # Convert to PublicKeyModule
        #
        Cmd: List[str] = [
            f'{self.__SignToolPath}',
            'rsa', '-pubin', '-inform', 'DER', '-modulus', '-noout', '-in',
            f'{self.__PubKeyPath}', '-out', f'{ModuleFilePath}',
            ]

        ExecWinExeCmd (Cmd)

        ModulesFile   : File  = File (FilePath = ModuleFilePath)
        ModulesContent: str   = ModulesFile.Content[0].strip ()
        if ModulesContent[0:len (ModulePrefix)] != ModulePrefix:
            raise ValueError (f'Please input valid modules file.')
        else:
            ModulesContent = ModulesContent.replace (ModulePrefix, '')

        #
        # Modules buffer (LSB format).
        #
        Buffer = ByteStrToByte (ModulesContent)
        Buffer.reverse ()

        #
        # Default exponent (LSB format).
        #
        Buffer += DEFAULT_RSA_EXPONENT.\
            to_bytes (4, byteorder = BYTE_ORDER_LITTLE)

        return Buffer

    def __GetSignature (self, HashAlgorithm: str) -> bytearray:
        """ Get the signature of the input data buffer.

        Args:
            HashAlgorithm (str):
                The hash algorithm in use for OpenSSL sign operation.

        Raises:
            None.

        Returns:
            bytearray:
                The signature of binary file for input data buffer.
        """
        Basename  : str = os.path.basename (self.__OutputPath)
        ExportPath: str = JoinPath (self.__OutputDir, Basename + '.sig')

        Cmd: List[str] = [
            f'{self.__SignToolPath}',
            'dgst',  f'-{HashAlgorithm}',    '-keyform', 'PEM',
            '-sign', f'{self.__PriKeyPath}', '-binary',
            '-out',  f'{ExportPath}',
            f'{self.__BgupDataPath}',
            ]

        ExecWinExeCmd (Cmd)

        Buffer = ByteBuffer (FilePath = ExportPath)

        return Buffer.Buffer

    def __SignBgupBySelfTool (self) -> bytearray:
        """ The sign operation to sign the data by self tool.

        Args:
            None.

        Raises:
            None.

        Returns:
            bytearray:
                The certificate body and the input buffer signature.
        """
        ExportPath: str        = self.__OutputPath
        Buffer    : ByteBuffer = None

        Cmd: List[str] = [
            f'{self.__SignToolPath}',
            f'{self.__BgupDataPath}',
            f'{ExportPath}',
            '-K',
            f'{self.__PriKeyPath}',
            f'{self.__PubKeyPath}',
            ]

        ExecWinExeCmd (Cmd)

        Buffer = ByteBuffer (FilePath = ExportPath)

        return Buffer.Buffer

    def __SignBgupByOpenSSL (self) -> bytearray:
        """ The sign operation to sign the data by OpenSSL.

        Args:
            None.

        Raises:
            None.

        Returns:
            bytearray:
                The certificate body and the input buffer signature.
        """
        CertBodyBuffer  : bytearray = bytearray ()
        SignatureBuffer : bytearray = bytearray ()
        HashAlgorithm   : str       = str ()

        IsFound, Algorithm = SearchKeyInDict (BGUPC_HEADER_DATA, self.__Algorithm)
        if not IsFound:
            raise ValueError (f'Unsupported signing algorithm [{self.__Algorithm}]')

        HashAlgorithm   = BGUPC_HEADER_DATA[Algorithm][HASH_ALGORITHM_KEY]
        CertBodyBuffer  = self.__GetCertBody ()
        SignatureBuffer = self.__GetSignature (HashAlgorithm)

        return CertBodyBuffer + SignatureBuffer

    def Build (self) -> None:
        """ Build binary file for BGUP certificate and signature.

        Note:
            Binary is made from Cert Header + Cert Body + Signature.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        TouchDir (self.__OutputDir)

        CertSigBuffer    : ByteBuffer = None
        CertHeaderBuffer : bytearray  = self.__GetCertHeader ()
        CertBodySigBuffer: bytearray  = bytearray ()

        if CheckStringMatch (self.__SignType, TYPE_SIGN_SELF_TOOL):
            CertBodySigBuffer = self.__SignBgupBySelfTool ()
        elif CheckStringMatch (self.__SignType, TYPE_SIGN_OPENSSL_TOOL):
            CertBodySigBuffer = self.__SignBgupByOpenSSL ()
        else:
            raise ValueError (f'Unsupported sign type. [{self.__SignType}]')

        CertSigBuffer = \
            ByteBuffer (Buffer = CertHeaderBuffer + CertBodySigBuffer)

        CertSigBuffer.Save (self.__OutputPath)

class BgupGenerator (object):
    def __init__ (
        self,
        BgupWithDataPath : Union[str, os.PathLike],
        OutputPath       : Union[str, os.PathLike],
        ) -> None:
        """ Class to extract the BGUP only based on BGUP header.

        +--------------------+
        |     BGUP Header    |
        +--------------------+
        |        BGSL        |
        +--------------------+

        Args:
            BgupWithDataPath (Union[str, os.PathLike]):
                The path of the binary that BGUP with data.
            OutputPath (Union[str, os.PathLike]):
                The path of the binary to output that BGUP only.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BgupWithDataPath : str        = AbsPath (BgupWithDataPath)
        self.__Buffer           : ByteBuffer = \
            ByteBuffer (FilePath = self.__BgupWithDataPath)

        self.__OutputPath: str = AbsPath (OutputPath)
        self.__OutputDir : str = os.path.dirname (self.__OutputPath)

        self.__PreCheck ()

    @property
    def Filename (self) -> str:
        """ Filename of this BGUP without data file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The string of filename.
        """
        return os.path.basename (self.__OutputPath)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            None

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__BgupWithDataPath,
            ]
        CheckFileListExist (CheckPathList)

    def __GetBgupHeader (self) -> BgupHdr:
        """ Get the BGUP header from input binary file.

        Note:
            This function assume the offset should be on the 0x0.

        Args:
            None.

        Raises:
            None.

        Returns:
            BgupHdr:
                The header of BGUP.
        """
        BgupHeaderParser  : BgupHdrParser = \
            BgupHdrParser (Buffer = self.__Buffer)

        return BgupHeaderParser.Header

    def Build (self) -> None:
        """ Generate the BGUP without data.

        Note:
            This function would parse the BGUP header and extract the
            binary buffer base on the BGUP header size + BGSL size.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        TouchDir (self.__OutputDir)

        BgupHeader : BgupHdr   = self.__GetBgupHeader ()
        HeaderSize : int       = BGUP_HDR_VER2_BYTE_SIZE
        BgslSize   : int       = HexToDec (BgupHeader.BgslSize)
        StartOffset: int       = DEFAULT_BGUP_HDR_OFFSET
        EndOffset  : int       = HeaderSize + BgslSize
        Buffer     : bytearray = self.__Buffer.Buffer[StartOffset:EndOffset]

        WriteBufferFile (
          Buffer     = Buffer,
          TargetPath = self.__OutputPath,
          )

class SignedBgupGenerator (object):
    def __init__ (
        self,
        BgupPath   : Union[str, os.PathLike],
        CertSigPath: Union[str, os.PathLike],
        OutputPath : Union[str, os.PathLike],
        ) -> None:
        """ Class to generate the signed BGUP.

        +--------------------+
        |     BGUP Header    |
        +--------------------+
        |        BGSL        |
        +--------------------+
        |    BGUPC Header    |
        +--------------------+
        |  BGUPC Certificate |
        +--------------------+
        |   Data Signature   |
        +--------------------+

        Args:
            BgupPath (Union[str, os.PathLike]):
                The path of BGUP binary without data.
            CertSigPath (Union[str, os.PathLike]):
                The path of BGUP certificate.
            OutputPath (Union[str, os.PathLike]):
                The path of output binary file for signed BGUP.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BgupPath   : str = BgupPath
        self.__CertSigPath: str = CertSigPath
        self.__OutputPath : str = OutputPath
        self.__OutputDir  : str = os.path.dirname (self.__OutputPath)

        self.__PreCheck ()

    @property
    def Filename (self) -> str:
        """ Filename of this signed BGUP file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The string of filename.
        """
        return os.path.basename (self.__OutputPath)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            None

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__BgupPath,
            self.__CertSigPath,
            ]
        CheckFileListExist (CheckPathList)

    def Build (self) -> None:
        """ Generate the singed BGUP for capsule update.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        TouchDir (self.__OutputDir)

        BgupBuffer   : ByteBuffer = ByteBuffer (FilePath = self.__BgupPath)
        CertSigBuffer: ByteBuffer = ByteBuffer (FilePath = self.__CertSigPath)

        SignedBgupBuffer: ByteBuffer = \
            ByteBuffer (Buffer = BgupBuffer.Buffer + CertSigBuffer.Buffer)
        SignedBgupBuffer.Save (self.__OutputPath)

def Main (Args: argparse.Namespace) -> int:
    """ Main function of the flow to generate the signed BGUP.

    Args:
        Args (argparse.Namespace):
            The argument to parse from the command line.

    Raises:
        None.

    Returns:
        int:
            Should be EXIT_SUCCESS.
            Since each step would raise exception by itself.
    """
    ExportPath: str = AbsPath (Args.OutputPath)

    BaseFileName: str = ()
    ExportFolder: str = AbsPath (os.path.dirname (ExportPath))
    BaseFileName, _   = GetFileNameAndExt (ExportPath)

    #
    # A. Generate the BGSL binary file.
    #
    BgslBinPath : str = JoinPath (ExportFolder, (BaseFileName + '.bgsl.bin'))
    BgslBinGenerator (
      Bgsl2BinPath = AbsPath (Args.Bgsl2BinPath),
      BgslPath     = AbsPath (Args.InputPath),
      OutputPath   = BgslBinPath,
      ).Compile ()

    #
    # B. Generate the Unsigned BGUP package with Data.
    #
    BgupWithDataPath: str = JoinPath (ExportFolder, (BaseFileName + '.bgup.data'))
    BgupWithDataGenerator (
      BgpbPath    = AbsPath (Args.BgpbPath),
      BgslBinPath = BgslBinPath,
      OutputPath  = BgupWithDataPath,
      PlatformId  = Args.PlatformId,
      BiosSvn     = Args.BiosSvn,
      DataPath    = AbsPath (Args.DataPath),
      EcSvn       = DEFAULT_EC_SVN_VALUE,
      UseSfam     = Args.UseSfam,
      UseFtu      = Args.UseFtu,
      ).Build ()

    #
    # C. Generate the certificate for BGUP package with Data.
    #
    CertSignaturePath: str = JoinPath (ExportFolder, (BaseFileName + '.cert.sig'))
    CertSignatureGenerator (
      BgupWithDataPath = BgupWithDataPath,
      SignType         = Args.SignType,
      Algorithm        = Args.Algorithm,
      SignToolPath     = AbsPath (Args.SignToolPath),
      PubKeyPath       = AbsPath (Args.PbkFilePath),
      PriKeyPath       = AbsPath (Args.PvkFilePath),
      OutputPath       = CertSignaturePath,
      ).Build ()

    #
    # D. Generate the BGUP without data
    #
    BgupPath: str = JoinPath (ExportFolder, (BaseFileName + '.bgup'))
    BgupGenerator (
      BgupWithDataPath  = BgupWithDataPath,
      OutputPath        = BgupPath,
      ).Build ()

    #
    # E. Generate Signed BGUP (For Capsule Update)
    #
    SignedBgupGenerator (
      BgupPath    = BgupPath,
      CertSigPath = CertSignaturePath,
      OutputPath  = ExportPath,
      ).Build ()

    return EXIT_SUCCESS

if __name__ == '__main__':
    def GetArgs () -> argparse.Namespace:
        """ Get the user input arguments.

        Args:
            None.

        Raises:
            None.

        Returns:
            Args [argparse.Namespace]:
                The argument object parsing from user input.
        """
        Parser: argparse.ArgumentParser = argparse.\
            ArgumentParser (
              prog            = __prog__,
              description     = __copyright__ + __description__ + __version__,
              formatter_class = argparse.RawTextHelpFormatter,
              )
        Parser.add_argument (
                 '-V',
                 '-v',
                 '--Version',
                 action  = 'version',
                 version = __version__,
                 )
        Parser.add_argument (
                 '-i',
                 '-I',
                 '--input',
                 dest     = 'InputPath',
                 help     = 'Input file for BGSL script.',
                 required = True,
                 )
        Parser.add_argument (
                 '-d',
                 '-D',
                 '--data',
                 dest     = 'DataPath',
                 help     = 'The data used in this BGUP file.',
                 required = False,
                 default  = None,
                 )
        Parser.add_argument (
                 '-o',
                 '-O',
                 '--output',
                 dest     = 'OutputPath',
                 help     = 'Path for BiosGuard Update Package(BGUP).',
                 required = True,
                 )
        #
        # BiosGuardSL2BIN and BiosGuardPB
        #
        Parser.add_argument (
                 '-bgsl2bin',
                 '-BGSL2BIN',
                 '--bgsl2bin',
                 dest     = 'Bgsl2BinPath',
                 help     = 'Path of tool to compile the BGSL to binary.',
                 required = True,
                 )
        Parser.add_argument (
                 '-bgpb',
                 '-BGPB',
                 '--bgpb',
                 dest     = 'BgpbPath',
                 help     = 'Path of tool to generate the unsigned BGUP with data.',
                 required = True,
                 )
        Parser.add_argument (
                 '-p',
                 '-P',
                 '-platform',
                 dest     = 'PlatformId',
                 help     = 'The platform ID for this BGUP.',
                 required = True,
                 )
        Parser.add_argument (
                 '-biossvn',
                 '-BIOSSVN',
                 '--biosvn',
                 dest     = 'BiosSvn',
                 help     = 'The BIOS SVN value should be UINT32.',
                 required = True,
                 )
        Parser.add_argument (
                 '-use_sfam',
                 '-USE_SFAM',
                 '--use-sfam',
                 dest     = 'UseSfam',
                 help     = 'Boolean value [true|false] used for is need Sfam.',
                 default  = 'true',
                 required = False,
                 )
        Parser.add_argument (
                 '-use_ftu',
                 '-USE_FTU',
                 '--use-ftu',
                 dest     = 'UseFtu',
                 help     = 'Boolean value [true|false] used for is needed UseFtu.',
                 default  = 'true',
                 required = False,
                 )
        #
        # Certificate generate
        #
        Parser.add_argument (
                 '-signtype',
                 dest     = 'SignType',
                 help     = f'Method for different signing flow. {TYPE_SIGN_LIST}',
                 required = True,
                 )
        Parser.add_argument (
                 '-algorithm',
                 dest     = 'Algorithm',
                 help     = f'Method for different signing algorithm. {list (BGUPC_HEADER_DATA.keys ())}',
                 required = True,
                 )
        Parser.add_argument (
                 '-signtool',
                 '-SIGNTOOL',
                 '--signtool',
                 dest     = 'SignToolPath',
                 help     = 'The path of tool used for signing the hash value.',
                 required = True,
                 )
        Parser.add_argument (
                 '-pbk',
                 '-PBK',
                 '--public-key',
                 dest     = 'PbkFilePath',
                 help     = 'The public key use for decrypt. (Certificate)',
                 required = True,
                 )
        Parser.add_argument (
                 '-pvk',
                 '-PVK',
                 '--private-key',
                 dest     = 'PvkFilePath',
                 help     = 'The private key use for to sign the hash value.',
                 required = True,
                 )

        return Parser.parse_args ()

    Args: argparse.Namespace = GetArgs ()

    Main (Args)
