## @file
#  Generate the self-signed X.509 certificate chain for capsule signing.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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

import argparse
from collections import OrderedDict
from typing import Any, Callable, Dict, MutableMapping

from CapsuleCommon import *

MAJ_VER: int = 1
MIN_VER: int = 0

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (c) 2024 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to generate self-signed X.509 certificate chain.\n'

WORKSPACE: str = os.path.dirname (os.path.abspath (__file__))

#
# Path information.
#
CONFIG_DIR         : str = JoinPath (WORKSPACE, 'Config')
OUTPUT_DIR         : str = JoinPath (WORKSPACE, 'Output')
KEY_CONFIG_PATH    : str = JoinPath (CONFIG_DIR, 'KeyConfig.toml')
OPENSSL_CONFIG_PATH: str = JoinPath (CONFIG_DIR, 'Openssl.cfg')

OPENSSL_WORKSPACE_PATH   : str = JoinPath (WORKSPACE, 'WorkspaceCA')
OPENSSL_NEW_CERT_DIR_PATH: str = JoinPath (OPENSSL_WORKSPACE_PATH, 'NewCerts')
OPENSSL_DATABASE_PATH    : str = JoinPath (OPENSSL_WORKSPACE_PATH, 'Index.txt')
OPENSSL_SERIAL_PATH      : str = JoinPath (OPENSSL_WORKSPACE_PATH, 'Serial')

#
# Content information.
#
OPENSSL_DATABASE_CONTENT_DEFAULT: str = ''
OPENSSL_SERIAL_CONTENT_DEFAULT  : str = '01 \n'

#
# Environment variables.
#
ENV_OPENSSL_CONF_KEY: str = 'OPENSSL_CONF'

ENV_VAR_CONFIG: Dict[str, str]       = os.environ.copy ()
ENV_VAR_CONFIG[ENV_OPENSSL_CONF_KEY] = OPENSSL_CONFIG_PATH

#
# Definition of file extensions.
#
EXT_RSA_KEY_FILE: str = 'key'
EXT_CRT_FILE    : str = 'crt'
EXT_CSR_FILE    : str = 'csr'
EXT_CER_FILE    : str = 'cer'
EXT_PFX_FILE    : str = 'pfx'
EXT_PUBLIC_FILE : str = 'pub.pem'
EXT_PRIVATE_FILE: str = 'pem'

#
# Definition of Key configuration.
#
SECTION_ROOT_CA        : str = 'RootCA'
SECTION_INTERMEDIATE_CA: str = 'IntermediateCA'
SECTION_USER_CERT      : str = 'UserCert'

KEY_EXTENSIONS            : str = 'Extensions'
KEY_KEY_NAME              : str = 'KeyName'
KEY_KEY_LENGTH            : str = 'KeyLength'
KEY_HASH_ALGORITHM        : str = 'HashAlgorithm'
KEY_VALIDITY              : str = 'Validity'
KEY_PASSPHRASE            : str = 'Passphrase'
KEY_COUNTRY_NAME          : str = 'CountryName'
KEY_STATE_OR_PROVINCE_NAME: str = 'StateOrProvinceName'
KEY_LOCAL_NAME            : str = 'LocalName'
KEY_ORGANIZATION_NAME     : str = 'Organization'
KEY_ORGANIZATION_UNIT_NAME: str = 'OrganizationalUnitName'
KEY_COMMON_NAME           : str = 'CommonName'

#
# Exit code of this script.
#
EXIT_SUCCESS: int = 0x00

class _KeyConfig (object):
    #
    # List of items need to check for input key within dict.
    #
    KEY_CONFIG_MANDATORY: List[str] = [
        KEY_KEY_NAME,
        KEY_KEY_LENGTH,
        KEY_HASH_ALGORITHM,
        KEY_VALIDITY,
        KEY_PASSPHRASE,
        KEY_COUNTRY_NAME,
        KEY_STATE_OR_PROVINCE_NAME,
        KEY_LOCAL_NAME,
        KEY_ORGANIZATION_NAME,
        KEY_ORGANIZATION_UNIT_NAME,
        KEY_COMMON_NAME,
        ]

    def __init__ (self, **Kwargs: Dict[str, Any]) -> None:
        """ Class used to represent the signing key configuration.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of signing key configuration.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__Extensions         : str = self.__Kwargs[KEY_EXTENSIONS]
        self.__KeyName            : str = self.__Kwargs[KEY_KEY_NAME]
        self.__KeyLength          : str = self.__Kwargs[KEY_KEY_LENGTH]
        self.__HashAlgorithm      : str = self.__Kwargs[KEY_HASH_ALGORITHM]
        self.__Validity           : str = self.__Kwargs[KEY_VALIDITY]
        self.__Passphrase         : str = self.__Kwargs[KEY_PASSPHRASE]
        self.__CountryName        : str = self.__Kwargs[KEY_COUNTRY_NAME]
        self.__StateOrProvinceName: str = self.__Kwargs[KEY_STATE_OR_PROVINCE_NAME]
        self.__LocalName          : str = self.__Kwargs[KEY_LOCAL_NAME]
        self.__OrgName            : str = self.__Kwargs[KEY_ORGANIZATION_NAME]
        self.__OrgUnitName        : str = self.__Kwargs[KEY_ORGANIZATION_UNIT_NAME]
        self.__CommonName         : str = self.__Kwargs[KEY_COMMON_NAME]

        self.__Subject: OpenSSLSubject = self.__GetSubject ()

    @property
    def Extensions (self) -> str:
        """ Return extension name within the OpenSSL configuration file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Extension name within the OpenSSL configuration file.
        """
        return self.__Extensions

    @property
    def KeyName (self) -> str:
        """ Return unique name of this signing key.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Unique name of this signing key.
        """
        return self.__KeyName

    @property
    def KeyLength (self) -> str:
        """ Return length of the signing key in bit.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Length of the signing key in bit.
        """
        return self.__KeyLength

    @property
    def HashAlgorithm (self) -> str:
        """ Return hash algorithm of this signing key signature.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Hash algorithm of this signing key signature.
                (Shall be one of OPENSSL_HASH_ALGORITHM_STR_LIST.)
        """
        Value: str = None

        _, Value = SearchValueInList (
                     InputList   = OPENSSL_HASH_ALGORITHM_STR_LIST,
                     TargetValue = self.__HashAlgorithm,
                     )

        return Value

    @property
    def Validity (self) -> str:
        """ Return length of the validity of this signing key.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Length of the validity of this signing key.
        """
        return self.__Validity

    @property
    def Passphrase (self) -> str:
        """ Return password of this signing key.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Password of this signing key.
        """
        return self.__Passphrase

    @property
    def Subject (self) -> OpenSSLSubject:
        """ Return subject name of this signing key.

        Args:
            None.

        Raises:
            None.

        Returns:
            OpenSSLSubject:
                Subject name of this signing key.
        """
        return self.__Subject

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                Signing key configuration member is not str type.
            ValueError:
                (1) Signing key configuration member is missing.
                (2) Length of signing key is not in
                    OPENSSL_KEY_LENGTH_STR_LIST.
                (3) Hash algorithm of signing key is not in
                    OPENSSL_HASH_ALGORITHM_STR_LIST.

        Returns:
            None.
        """
        Member : str  = None
        Value  : str  = None
        IsFound: bool = None

        #
        # Check all the mandatory members exist and are string type.
        #
        for Member in _KeyConfig.KEY_CONFIG_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

            if not isinstance (self.__Kwargs[Member], str):
                raise TypeError (
                        f'Member {Member} not a str type.'
                        )

        if self.__Kwargs[KEY_KEY_LENGTH] not in OPENSSL_KEY_LENGTH_STR_LIST:
            raise ValueError (
                    f'Invalid key length - [{self.__Kwargs[KEY_KEY_LENGTH]}]',
                    )

        #
        # Check if hash algorithm is valid.
        #
        IsFound, Value = SearchValueInList (
                           InputList   = OPENSSL_HASH_ALGORITHM_STR_LIST,
                           TargetValue = self.__Kwargs[KEY_HASH_ALGORITHM],
                           )
        if not IsFound:
            raise ValueError (
                    f'Invalid hash algorithm - [{self.__Kwargs[KEY_HASH_ALGORITHM]}]',
                    )
        self.__HashAlgorithm = Value

    def __GetSubject (self) -> OpenSSLSubject:
        """ Initial the OpenSSL subject object.

        Args:
            None.

        Raises:
            None.

        Returns:
            OpenSSLSubject:
                Subject name of this signing key.
        """
        Subject: OpenSSLSubject = None

        Subject = OpenSSLSubject (
                    CommonName           = self.__CommonName,
                    CountryName          = self.__CountryName,
                    StateOrProvinceName  = self.__StateOrProvinceName,
                    LocalName            = self.__LocalName,
                    OrganizationName     = self.__OrgName,
                    OrganizationUnitName = self.__OrgUnitName,
                    )

        return Subject

class _KeyInfo (object):
    def __init__ (
        self,
        KeyConfig: _KeyConfig,
        ) -> None:
        """ Class to represent the signing key information.

        Args:
            KeyConfig (_KeyConfig):
                Configuration of the signing key.

        Raises:
            None.

        Returns:
            None.
        """
        self.__KeyConfig: _KeyConfig = KeyConfig

        self.__PreCheck ()

    @property
    def KeyConfig (self) -> _KeyConfig:
        """ Return the key configuration object.

        Args:
            None.

        Raises:
            None.

        Returns:
            _KeyConfig:
                Key configuration object.
        """
        return self.__KeyConfig

    @property
    def KeyPairPath (self) -> str:
        """ Return path to the key-pair file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the key-pair file.
        """
        return JoinPath (
                 OUTPUT_DIR,
                 f'{self.__KeyConfig.KeyName}.{EXT_RSA_KEY_FILE}',
                 )

    @property
    def CsrPath (self) -> str:
        """ Return path to the certificate request file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the certificate request file.
        """
        return JoinPath (
                 OUTPUT_DIR,
                 f'{self.__KeyConfig.KeyName}.{EXT_CSR_FILE}',
                 )

    @property
    def CrtPath (self) -> str:
        """ Return path to the certificate file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the certificate file.
        """
        return JoinPath (
                 OUTPUT_DIR,
                 f'{self.__KeyConfig.KeyName}.{EXT_CRT_FILE}',
                 )

    @property
    def CerPath (self) -> str:
        """ Return path to the X.509 formatted file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the X.509 formatted file.
        """
        return JoinPath (
                 OUTPUT_DIR,
                 f'{self.__KeyConfig.KeyName}.{EXT_CER_FILE}',
                 )

    @property
    def PfxPath (self) -> str:
        """ Return path to the PKCS#12 formatted file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the PKCS#12 formatted file.
        """
        return JoinPath (
                 OUTPUT_DIR,
                 f'{self.__KeyConfig.KeyName}.{EXT_PFX_FILE}',
                 )

    @property
    def PublicKeyPath (self) -> str:
        """ Return path to the public key.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the public key.
        """
        return JoinPath (
                 OUTPUT_DIR,
                 f'{self.__KeyConfig.KeyName}.{EXT_PUBLIC_FILE}',
                 )

    @property
    def PrivateKeyPath (self) -> str:
        """ Return path to the private key.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path to the private key. (Without password protection.)
        """
        return JoinPath (
                 OUTPUT_DIR,
                 f'{self.__KeyConfig.KeyName}.{EXT_PRIVATE_FILE}',
                 )

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            None.

        Raises:
            TypeError:
                KeyConfig is not _KeyConfig type.
        """
        if not isinstance (self.__KeyConfig, _KeyConfig):
            raise TypeError (f'KeyConfig shall be _KeyConfig type.')

class _OpenSSLHandler (object):
    def __init__ (
        self,
        SignToolPath: Union[str, os.PathLike],
        SignKeyInfo : _KeyInfo,
        CA          : object = None,
        ) -> None:
        """ Class to provide the APIs for OpenSSL program.

        Args:
            SignToolPath (Union[str, os.PathLike]):
                Path to the OpenSSL program.
            SignKeyInfo (_KeyInfo):
                Information of signing key.
            CA (_CA, optional):
                Object of certificate authority (CA).
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__SignToolPath: str      = AbsPath (SignToolPath)
        self.__SignKeyInfo : _KeyInfo = SignKeyInfo
        self.__CA          : _CA      = CA

        self.__PreCheck ()
        self.__EnvInitial ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) SignKeyInfo is not _KeyInfo type.
                (2) CA is not _CA type.

        Returns:
            None.
        """
        if not isinstance (self.__SignKeyInfo, _KeyInfo):
            raise TypeError (f'SignKeyInfo shall be _KeyInfo type.')
        elif not isinstance (self.__CA, (type (None), _CA)):
            raise TypeError (f'CA shall be _CA type.')

        #
        # Check the input file exist.
        #
        CheckPathList = [
            self.__SignToolPath,
            ]
        CheckFileListExist (CheckPathList)

    def __EnvInitial (self) -> None:
        """ Initial the workspace for OpenSSL environment.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        DatabaseFile: File = None
        SerialFile  : File = None

        #
        # Touch the needed directories.
        #
        TouchDir (OPENSSL_WORKSPACE_PATH)
        TouchDir (OPENSSL_NEW_CERT_DIR_PATH)
        TouchDir (OUTPUT_DIR)

        #
        # Generate the Database file.
        #
        if IsPathExist (OPENSSL_DATABASE_PATH):
            if not IsFile (OPENSSL_DATABASE_PATH):
                ErrorException (f'{OPENSSL_DATABASE_PATH} is not a file.')
        else:
            DatabaseFile = File (FileContent = OPENSSL_DATABASE_CONTENT_DEFAULT)
            DatabaseFile.Save (OPENSSL_DATABASE_PATH)

        #
        # Generate the Serial file.
        #
        if IsPathExist (OPENSSL_SERIAL_PATH):
            if not IsFile (OPENSSL_SERIAL_PATH):
                ErrorException (f'{OPENSSL_SERIAL_PATH} is not a file.')
        else:
            SerialFile = File (FileContent = OPENSSL_SERIAL_CONTENT_DEFAULT)
            SerialFile.Save (OPENSSL_SERIAL_PATH)

    def GenRsaKey (self) -> int:
        """ API to create RSA key-pair file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Exit code of OpenSSL program.
        """
        CmdList: List[str]     = list ()
        GenRsa : OpenSSLGenRsa = None

        GenRsa = OpenSSLGenRsa (
                   KeyLength  = self.__SignKeyInfo.KeyConfig.KeyLength,
                   Cipher     = OPENSSL_CIPHER_AES_256_STR,
                   Passphrase = self.__SignKeyInfo.KeyConfig.Passphrase,
                   OutputPath = self.__SignKeyInfo.KeyPairPath,
                   )

        CmdList.append (f'{self.__SignToolPath}')
        CmdList.extend (GenRsa.CmdList)

        DEBUG (DEBUG_TRACE, f'Command execute: {CmdList}')

        ExitCode, _, _ = ExecWinExeCmd (
                           CmdList,
                           Env = ENV_VAR_CONFIG,
                           Cwd = WORKSPACE,
                           )

        return ExitCode

    def GenSelfSignedCert (self) -> int:
        """ API to create self-signed certificate file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Exit code of OpenSSL program.
        """
        CmdList: List[str]            = list ()
        Req    : OpenSSLReqSelfSigned = None

        Req = OpenSSLReqSelfSigned (
                KeyPairPath   = self.__SignKeyInfo.KeyPairPath,
                Passphrase    = self.__SignKeyInfo.KeyConfig.Passphrase,
                Extensions    = self.__SignKeyInfo.KeyConfig.Extensions,
                Subject       = self.__SignKeyInfo.KeyConfig.Subject,
                HashAlgorithm = self.__SignKeyInfo.KeyConfig.HashAlgorithm,
                Validity      = self.__SignKeyInfo.KeyConfig.Validity,
                OutputPath    = self.__SignKeyInfo.CrtPath,
                )

        CmdList.append (f'{self.__SignToolPath}')
        CmdList.extend (Req.CmdList)

        ExitCode, _, _ = ExecWinExeCmd (
                           CmdList,
                           Env = ENV_VAR_CONFIG,
                           Cwd = WORKSPACE,
                           )

        return ExitCode

    def GenCsr (self) -> int:
        """ API to create certificate request file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Exit code of OpenSSL program.
        """
        CmdList: List[str]     = list ()
        Req    : OpenSSLReqCsr = None

        Req = OpenSSLReqCsr (
                KeyPairPath   = self.__SignKeyInfo.KeyPairPath,
                Passphrase    = self.__SignKeyInfo.KeyConfig.Passphrase,
                Subject       = self.__SignKeyInfo.KeyConfig.Subject,
                HashAlgorithm = self.__SignKeyInfo.KeyConfig.HashAlgorithm,
                OutputPath    = self.__SignKeyInfo.CsrPath,
                )

        CmdList.append (f'{self.__SignToolPath}')
        CmdList.extend (Req.Command)

        ExitCode, _, _ = ExecWinExeCmd (
                           CmdList,
                           Env = ENV_VAR_CONFIG,
                           Cwd = WORKSPACE,
                           )

        return ExitCode

    def GenCaSignedCert (self) -> int:
        """ API to create CA signed certificate file.

        Args:
            None.

        Raises:
            Exception:
                API is not supported when CA is None.

        Returns:
            int:
                Exit code of OpenSSL program.
        """
        CmdList: List[str]          = list ()
        Ca     : OpenSSLCaCsrHandle = None

        if self.__CA is None:
            raise Exception (f'Failed to create CA signed certificate when CA is None.')

        Ca = OpenSSLCaCsrHandle (
                Extensions      = self.__SignKeyInfo.KeyConfig.Extensions,
                CsrPath         = self.__SignKeyInfo.CsrPath,
                CaKeyPairPath   = self.__CA.SignKeyInfo.KeyPairPath,
                CaKeyCrtPath    = self.__CA.SignKeyInfo.CrtPath,
                CaKeyPassphrase = self.__CA.SignKeyInfo.KeyConfig.Passphrase,
                HashAlgorithm   = self.__SignKeyInfo.KeyConfig.HashAlgorithm,
                Validity        = self.__SignKeyInfo.KeyConfig.Validity,
                OutputPath      = self.__SignKeyInfo.CrtPath,
                )

        CmdList.append (f'{self.__SignToolPath}')
        CmdList.extend (Ca.Command)

        ExitCode, _, _ = ExecWinExeCmd (
                           CmdList,
                           Env = ENV_VAR_CONFIG,
                           Cwd = WORKSPACE,
                           )

        return ExitCode

    def PemToDer (self) -> int:
        """ API to convert PEM formatted file into DER formatted file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Exit code of OpenSSL program.
        """
        CmdList  : List[str]                 = list ()
        Converter: OpenSSLX509KeyFormConvert = None

        Converter = OpenSSLX509KeyFormConvert (
                      InputPath  = self.__SignKeyInfo.CrtPath,
                      InputForm  = OPENSSL_KEY_FORM_PEM_STR,
                      OutputPath = self.__SignKeyInfo.CerPath,
                      OutputForm = OPENSSL_KEY_FORM_DER_STR,
                      )

        CmdList.append (f'{self.__SignToolPath}')
        CmdList.extend (Converter.CmdList)

        ExitCode, _, _ = ExecWinExeCmd (
                           CmdList,
                           Env = ENV_VAR_CONFIG,
                           Cwd = WORKSPACE,
                           )

        return ExitCode

    def DerToPem (self) -> int:
        """ API to convert DER formatted file into PEM formatted file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Exit code of OpenSSL program.
        """
        CmdList  : List[str]                 = list ()
        Converter: OpenSSLX509KeyFormConvert = None

        Converter = OpenSSLX509KeyFormConvert (
                      InputPath  = self.__SignKeyInfo.CerPath,
                      InputForm  = OPENSSL_KEY_FORM_DER_STR,
                      OutputPath = self.__SignKeyInfo.PublicKeyPath,
                      OutputForm = OPENSSL_KEY_FORM_PEM_STR,
                      )

        CmdList.append (f'{self.__SignToolPath}')
        CmdList.extend (Converter.CmdList)

        ExitCode, _, _ = ExecWinExeCmd (
                           CmdList,
                           Env = ENV_VAR_CONFIG,
                           Cwd = WORKSPACE,
                           )

        return ExitCode

    def GenPfxFile (self) -> int:
        """ API to create PKCS#12 formatted file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Exit code of OpenSSL program.
        """
        CmdList  : List[str]                 = list ()
        Pkcs12Pfx: OpenSSLPkcs12ToPfx = None

        Pkcs12Pfx = OpenSSLPkcs12ToPfx (
                      KeyPairPath   = self.__SignKeyInfo.KeyPairPath,
                      KeyPassphrase = self.__SignKeyInfo.KeyConfig.Passphrase,
                      CrtPath       = self.__SignKeyInfo.CrtPath,
                      PfxPassphrase = self.__SignKeyInfo.KeyConfig.Passphrase,
                      OutputPath    = self.__SignKeyInfo.PfxPath,
                      )

        CmdList.append (f'{self.__SignToolPath}')
        CmdList.extend (Pkcs12Pfx.Command)

        ExitCode, _, _ = ExecWinExeCmd (
                           CmdList,
                           Env = ENV_VAR_CONFIG,
                           Cwd = WORKSPACE,
                           )

        return ExitCode

    def GetPrivateKey (self) -> int:
        """ API to retrieve the private key file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Exit code of OpenSSL program.
        """
        CmdList        : List[str]                    = list ()
        PfxToPrivateKey: OpenSSLPkcs12ParsePrivateKey = None

        PfxToPrivateKey = OpenSSLPkcs12ParsePrivateKey (
                            PfxPath    = self.__SignKeyInfo.PfxPath,
                            Passphrase = self.__SignKeyInfo.KeyConfig.Passphrase,
                            OutputPath = self.__SignKeyInfo.PrivateKeyPath,
                            )

        CmdList.append (f'{self.__SignToolPath}')
        CmdList.extend (PfxToPrivateKey.Command)

        ExitCode, _, _ = ExecWinExeCmd (
                           CmdList,
                           Env = ENV_VAR_CONFIG,
                           Cwd = WORKSPACE,
                           )

        return ExitCode

class _CA (object):
    def __init__ (
        self,
        SignToolPath: Union[str, os.PathLike],
        SignKeyInfo : _KeyInfo,
        CA          : object = None,
        ) -> None:
        """ Class to represent the certificate authority (CA).

        Note:
            If CA is None, go through the self-signed flow.
            If CA is not None, go through the CA-signed flow.

        Args:
            SignToolPath (Union[str, os.PathLike]):
                Path to the OpenSSL program.
            SignKeyInfo (_KeyInfo):
                Information of signing key.
            CA (object, optional):
                Object of certificate authority (CA).
                Defaults to None.
        """
        self.__SignToolPath: Union[str, os.PathLike] = AbsPath (SignToolPath)
        self.__SignKeyInfo : _KeyInfo                = SignKeyInfo
        self.__CA          : _CA                     = CA

        self.__PreCheck ()

        self.__OpenSSLHandler: _OpenSSLHandler = \
            _OpenSSLHandler (
              SignToolPath = self.__SignToolPath,
              SignKeyInfo  = self.__SignKeyInfo,
              CA           = self.__CA,
              )
        self.__OperationFlow : OrderedDict[str, Callable] = \
            self.__GetOperationFlow ()

        self.__Execute ()

    @property
    def SignKeyInfo (self) -> _KeyInfo:
        """ Return information of signing key.

        Args:
            None.

        Raises:
            None.

        Returns:
            _KeyInfo:
                Information of the signing key.
        """
        return self.__SignKeyInfo

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) SignKeyInfo is not _KeyInfo type.
                (2) CA is not _CA type.

        Returns:
            None.
        """
        if not isinstance (self.__SignKeyInfo, _KeyInfo):
            raise TypeError (f'SignKeyInfo shall be _KeyInfo type.')
        elif not isinstance (self.__CA, (type (None), _CA)):
            raise TypeError (f'CA shall be _CA type.')

        #
        # Check the input file exist.
        #
        CheckPathList = [
            self.__SignToolPath,
            ]
        CheckFileListExist (CheckPathList)

    def __GetOperationFlow (self) -> OrderedDict[str, Callable]:
        """ Return the flow for the specific signing operation.

        Args:
            None.

        Raises:
            None.

        Returns:
            OrderedDict[str, Callable]:
                str:
                    Name of the step.
                Callable:
                    Function to execute for this step.
        """
        SELF_SIGNED_OP: OrderedDict[str, Callable] = OrderedDict ({
            'GenRsaKey'            : self.__OpenSSLHandler.GenRsaKey,
            'GenSelfSignedCert'    : self.__OpenSSLHandler.GenSelfSignedCert,
            'ConvertCrtToCer'      : self.__OpenSSLHandler.PemToDer,
            'ConvertCerToPublicPem': self.__OpenSSLHandler.DerToPem,
            'ConvertKeyPairToPfx'  : self.__OpenSSLHandler.GenPfxFile,
            'ExtractPrivatePem'    : self.__OpenSSLHandler.GetPrivateKey,
            })

        CA_CSR_CERT_OP: OrderedDict[str, Callable] = OrderedDict ({
            'GenRsaKey'            : self.__OpenSSLHandler.GenRsaKey,
            'GenCsr'               : self.__OpenSSLHandler.GenCsr,
            'GenCASignedCrt'       : self.__OpenSSLHandler.GenCaSignedCert,
            'ConvertCrtToCer'      : self.__OpenSSLHandler.PemToDer,
            'ConvertCerToPublicPem': self.__OpenSSLHandler.DerToPem,
            'ConvertKeyPairToPfx'  : self.__OpenSSLHandler.GenPfxFile,
            'ExtractPrivatePem'    : self.__OpenSSLHandler.GetPrivateKey,
            })

        if self.__CA is None:
            return SELF_SIGNED_OP
        else:
            return CA_CSR_CERT_OP

    def __Execute (self) -> None:
        """ Execute the flow to generate the signing key.

        Args:
            None.

        Raises:
            ErrorException:
                Failed to perform the specific step for generating
                the signing key.

        Returns:
            None.
        """
        ExitCode: int = STATUS_SUCCESS

        for OpName, Func in self.__OperationFlow.items ():
            DEBUG (DEBUG_TRACE, f'Perform the step - {OpName}')
            ExitCode = Func ()
            if ExitCode != STATUS_SUCCESS:
                raise ErrorException (f'Failed to execute the step - {OpName}')

def Main (
    OpenSSLToolPath   : Union[str, os.PathLike],
    RootCASignKeyCfg  : _KeyConfig,
    SubCaSignKeyCfg   : _KeyConfig,
    UserCertSignKeyCfg: _KeyConfig,
    ) -> int:
    """ Main function for generating signing keys for capsule file.

    Args:
        OpenSSLToolPath (Union[str, os.PathLike]):
            Path to the OpenSSL program.
        RootCASignKeyCfg (_KeyConfig):
            Signing key configuration for the root CA.
        SubCaSignKeyCfg (_KeyConfig):
            Signing key configuration for the intermediate CA.
        UserCertSignKeyCfg (_KeyConfig):
            Signing key configuration for the user certificate.

    Raises:
        None.

    Returns:
        int:
            Exit code of this function.
    """
    SignToolPath   : str      = AbsPath (OpenSSLToolPath)
    RootCA         : _CA      = None
    SubCA          : _CA      = None
    UserCert       : _CA      = None
    RootCAKeyInfo  : _KeyInfo = _KeyInfo (KeyConfig = RootCASignKeyCfg)
    SubCAKeyInfo   : _KeyInfo = _KeyInfo (KeyConfig = SubCaSignKeyCfg)
    UserCertKeyInfo: _KeyInfo = _KeyInfo (KeyConfig = UserCertSignKeyCfg)

    #
    # Remove the old data if needed.
    #
    RemoveDir (OPENSSL_WORKSPACE_PATH)
    RemoveDir (OPENSSL_NEW_CERT_DIR_PATH)
    RemoveDir (OUTPUT_DIR)

    RootCA  = _CA (
               SignToolPath = SignToolPath,
               SignKeyInfo  = RootCAKeyInfo,
               CA           = None,
               )

    SubCA   = _CA (
               SignToolPath = SignToolPath,
               SignKeyInfo  = SubCAKeyInfo,
               CA           = RootCA,
               )

    UserCert = _CA (
               SignToolPath = SignToolPath,
               SignKeyInfo  = UserCertKeyInfo,
               CA           = SubCA,
               )

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
                 '-v',
                 '-V',
                 '--version',
                 action  = 'version',
                 version = __version__,
                 )
        Parser.add_argument (
                 '-t',
                 '-T',
                 '--sign-tool',
                 dest     = 'SignTool',
                 help     = 'Path to the OpenSSL signing tool program.',
                 required = True,
                 )
        Parser.add_argument (
                 '-c',
                 '-C',
                 '--config',
                 dest     = 'Config',
                 help     = 'Path to the key configuration file.',
                 required = False,
                 default  = KEY_CONFIG_PATH,
                 )

        return Parser.parse_args ()

    SetDebugLevel (DEBUG_TRACE)

    Args         : argparse.Namespace        = GetArgs ()
    ExitCode     : int                       = EXIT_SUCCESS
    KeyConfig    : Config                    = Config (FilePath = Args.Config)
    KeyConfigData: MutableMapping[str, Any]  = KeyConfig.Data

    #
    # Get the signing keys configuration.
    #
    RootCASignKeyCfg  : _KeyConfig = _KeyConfig (**KeyConfigData[SECTION_ROOT_CA])
    SubCASignKeyCfg   : _KeyConfig = _KeyConfig (**KeyConfigData[SECTION_INTERMEDIATE_CA])
    UserCertSignKeyCfg: _KeyConfig = _KeyConfig (**KeyConfigData[SECTION_USER_CERT])

    #
    # Execute the main code flow.
    #
    ExitCode = Main (
                 OpenSSLToolPath    = Args.SignTool,
                 RootCASignKeyCfg   = RootCASignKeyCfg,
                 SubCaSignKeyCfg    = SubCASignKeyCfg,
                 UserCertSignKeyCfg = UserCertSignKeyCfg,
                 )

    sys.exit (ExitCode)
