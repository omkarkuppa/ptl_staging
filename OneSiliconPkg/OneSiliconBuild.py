## @file
#
#  Usage:
#  -T------ [Required] Product Name in product folder (e.g. PantherLake)
#           | OneSiliconPkg
#           | | Product
#           | | | PantherLake
#           | | | ...
#           | | | ...
#           \ \ \ <Product Name>
#  -b------ [Optional] Target type (e.g. RELEASE or DEBUG)
#  -t------ [Optional] Tool chain name (e.g. VS2017, VS2019)
#
#  Ex:
#    Step1. Get RC code base
#    Step2. python Intel/OneSiliconPkg/OneSiliconBuild.py -T PantherLake
#
#  Note:
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
# @par Specification
##

import os
import subprocess
import argparse
import sys
import shutil
import multiprocessing

__prog__        = 'OneSiliconBuild.py'
__version__     = '%s Version %s' % (__prog__, '0.2')
__description__ = 'The script is used for RC build.\n'

def get_args():
    #
    # Prepare argument parser
    #
    parser = argparse.ArgumentParser()
    parser.add_argument (
             '-V',
             '-v',
             '--Version',
             action = 'version',
             version = __version__
             )

    #
    # Tool Chain setting
    #
    if (sys.platform == 'win32'):
        choices = {'VS2015', 'VS2015x86', 'VS2017', 'VS2019', 'CLANGPDB'}
    elif (sys.platform == 'linux'):
        choices = {'GCC', 'CLANGPDB'}
    elif (sys.platform == 'darwin'):
        choices = {'XCODE5'}
    else:
        print ('Currently, %s is unsupported' % (sys.platform) )
        exit(1)
    parser.add_argument (
             '-t',
             '--tagname',
             dest = 'ToolChain',
             help = ('Specify for your tool chain, Ex: VS2017 or VS2019'),
             choices=choices
             )

    parser.add_argument(
             '-T',
             '--Product',
             required = True,
             dest = 'Product',
             help = 'Choose your product name')

    parser.add_argument(
             '-b',
             '--buildtarget',
             dest = 'Target',
             help = 'Choose your Target type, Ex: RELEASE or DEBUG',
             default = "RELEASE",
             choices={'RELEASE', 'DEBUG'}
             )

    args = parser.parse_args()
    return args

def set_environ(ProductName, ToolChain = None, Target = None):
    #
    # prepare environ. var.
    #
    print ("Setting Environment Variables")
    os.environ['WORKSPACE']              = os.getcwd()
    os.environ['WORKSPACE_CORE']         = os.path.join( os.environ['WORKSPACE'], u'Edk2')
    os.environ['WORKSPACE_COMMON']       = os.path.join( os.environ['WORKSPACE'], u'Intel')
    os.environ['WORKSPACE_SILICON']      = os.path.join( os.environ['WORKSPACE'], u'Intel')
    os.environ['WORKSPACE_PLATFORM']     = os.path.join( os.environ['WORKSPACE'], u'Intel')
    os.environ['WORKSPACE_BINARIES']     = os.path.join( os.environ['WORKSPACE'], u'Binaries')
    os.environ['WORKSPACE_CORE_SILICON'] = os.path.join( os.environ['WORKSPACE'], u'Edk2Platforms', u'Silicon', u'Intel')
    os.environ['EDK_TOOLS_PATH']         = os.path.join( os.environ['WORKSPACE_CORE'], u'BaseTools')
    os.environ['WORKSPACE_TOOLS_PATH']   = os.path.join( os.environ['WORKSPACE_CORE'], u'BaseTools')
    os.environ['TARGET_PLATFORM']        = ProductName
    if ToolChain != None:
      os.environ['TOOL_CHAIN_TAG']       = ToolChain
    if Target != None:
      os.environ['TARGET']               = Target
    else:
      os.environ['TARGET']               = 'RELEASE'
    os.environ['PACKAGES_PATH']          = str(os.environ['WORKSPACE']) + os.pathsep
    os.environ['PACKAGES_PATH']         += str(os.environ['WORKSPACE_CORE_SILICON']) + os.pathsep
    os.environ['PACKAGES_PATH']         += str(os.environ['WORKSPACE_CORE']) + os.pathsep
    os.environ['PACKAGES_PATH']         += str(os.environ['WORKSPACE_COMMON']) + os.pathsep
    os.environ['PACKAGES_PATH']         += str(os.environ['WORKSPACE_SILICON']) + os.pathsep
    if os.path.exists( os.environ['WORKSPACE_BINARIES']):
        os.environ['PACKAGES_PATH']     += str(os.environ['WORKSPACE_BINARIES']) + os.pathsep
    os.environ['CONF_PATH']              = os.path.join( os.environ['WORKSPACE'], u'Conf')
    if not os.path.exists( os.environ['CONF_PATH']):
        os.mkdir( str( os.path.join( os.environ['WORKSPACE'], u'Conf')))
    if sys.platform == 'win32':
        os.environ['EDK_TOOLS_BIN'] = os.path.join( os.environ['EDK_TOOLS_PATH'], u'Bin', u'Win32')
    #
    # print environ. var.
    #
    print("System Platform        = " + sys.platform)
    print("WORKSPACE              = " + os.environ['WORKSPACE'])
    print("WORKSPACE_CORE         = " + os.environ['WORKSPACE_CORE'])
    print("WORKSPACE_COMMON       = " + os.environ['WORKSPACE_COMMON'])
    print("WORKSPACE_SILICON      = " + os.environ['WORKSPACE_SILICON'])
    print("WORKSPACE_CORE_SILICON = " + os.environ['WORKSPACE_CORE_SILICON'])
    if os.path.exists( os.environ['WORKSPACE_BINARIES']):
        print("WORKSPACE_BINARIES     = " + os.environ['WORKSPACE_BINARIES'])
    print("EDK_TOOLS_PATH         = " + os.environ['EDK_TOOLS_PATH'])
    print("PACKAGES_PATH          = " + os.environ['PACKAGES_PATH'])
    print("CONF_PATH              = " + os.environ['CONF_PATH'])
    print("TARGET_PLATFORM        = " + os.environ['TARGET_PLATFORM'])

def call_make_edk2basetools():
    cmd = [ 'make', '-C', 'Edk2/BaseTools']
    print ("cmd : "+str(cmd))
    subprocess.check_call(cmd, stdout=sys.stdout, stderr=sys.stderr)

def set_edk_environ():
    print ("***********************")
    print ("call edksetup")

    if sys.platform == 'win32':
        def GetEnvironmentVariables (Stdout, Marker):
          IsEnvUpdate     = False
          EnvironmentVars = {}
          Output          = ""

          for Line in Stdout.split ("\n"):
            if IsEnvUpdate and len (Line.split ("=")) == 2:
              Key, Value           = Line.split ("=")
              EnvironmentVars[Key] = Value
            elif Marker not in Line:
              Output += Line + "\n"

            if Marker in Line:
              if IsEnvUpdate:
                IsEnvUpdate = False
              else:
                IsEnvUpdate = True

          return (Output, EnvironmentVars)

        def ExecuteScript (
              Command,
              EnvVariables,
              Workspace,
              IsCollectEnv = False,
              Shell = True
              ):

          Marker = '-----env-----'
          Env    = {}
          Kwarg  = {
            "env"               : EnvVariables,
            "universal_newlines": True,
            "shell"             : Shell,
            "cwd"               : Workspace
          }

          if IsCollectEnv:
            Kwarg["stdout"] = subprocess.PIPE
            Kwarg["stderr"] = subprocess.PIPE

          #
          # Collect environment variables
          #
          if IsCollectEnv:
            #
            # Get the binary that prints environment variables based on os
            #
            if os.name == 'nt':
              GetVarCommand = "set"
            else:
              GetVarCommand = "env"
            #
            # Modify the command to print the environment variables
            #
            if isinstance (Command, list):
              Command += ["&&", "echo", Marker,
                          "&&", GetVarCommand,
                          "&&", "echo", Marker]
            else:
              Command += " " + " ".join (["&&", "echo", Marker,
                                          "&&", GetVarCommand,
                                          "&&", "echo", Marker])
          #
          # Execute the command
          #
          Execute         = subprocess.Popen (Command, **Kwarg)
          Stdout, Stderr  = Execute.communicate ()
          Status          = Execute.returncode

          #
          # Wait for process to be done
          #
          Execute.wait()

          #
          # If collect enviroment variables
          #
          if IsCollectEnv:
            #
            # Get the new environment variables
            #
            Stdout, Env = GetEnvironmentVariables (Stdout, Marker)

          return (Stdout, Stderr, Env, Status)


        os.environ['PATH']                   += os.pathsep + os.path.join( os.environ['EDK_TOOLS_PATH'], u'BinWrappers', u'WindowsLike')
        os.environ['PATH']                   += os.pathsep + os.path.join( os.environ['EDK_TOOLS_BIN'])
        os.environ['BASE_TOOLS_PATH']         = os.environ['EDK_TOOLS_PATH']
        os.environ['BASETOOLS_PYTHON_SOURCE'] = os.path.join( os.environ['EDK_TOOLS_PATH'], u'Source', u'Python')
        os.environ['PYTHONPATH']              = os.environ['BASETOOLS_PYTHON_SOURCE']+';'
        os.environ['PYTHON_COMMAND']          = u'py -3'

        EnvConfig = os.environ.copy ()
        cmd = [os.path.join( os.environ['WORKSPACE_CORE'], u'edksetup.bat'), u'Rebuild']
        print ("cmd : "+str(cmd))
        Stdout, _, Result, Status = ExecuteScript ("{0} {1}".format (cmd[0], cmd[1]), EnvConfig, "./", IsCollectEnv=True, Shell=True)
        if Status == 0 and Result is not None and isinstance (Result, dict):
          EnvConfig.update (Result)
        os.environ.update (EnvConfig)
        print (Stdout)

        if not os.environ.get('NASM_PREFIX_EXT'):
            if os.path.exists( os.path.join( os.environ['SystemDrive'], u'nasm', u'nasm.exe')):
                os.environ['NASM_PREFIX_EXT'] = os.path.join( os.environ['SystemDrive'], u'nasm', u'')
        if not os.environ.get('NASM_PREFIX'):
            os.environ['NASM_PREFIX'] = os.environ['NASM_PREFIX_EXT']
        print("NASM_PREFIX              = " + os.environ['NASM_PREFIX'])


        if not os.environ.get('IASL_PREFIX_EXT'):
            if os.path.exists( os.path.join( os.environ['WORKSPACE_PLATFORM'], os.environ['TARGET_PLATFORM'] + u'PlatSamplePkg', u'Tools', u'Iasl')):
                os.environ['IASL_PREFIX_EXT'] = os.path.join( os.environ['WORKSPACE_PLATFORM'], os.environ['TARGET_PLATFORM'] + u'PlatSamplePkg', u'Tools', u'Iasl', u'')
            elif os.path.exists( os.path.join( os.environ['SystemDrive'], u'ASL')):
                os.environ['IASL_PREFIX_EXT'] = os.path.join( os.environ['SystemDrive'], u'ASL', u'')
        if not os.environ.get('IASL_PREFIX'):
            os.environ['IASL_PREFIX'] = os.environ['IASL_PREFIX_EXT']
        print("IASL_PREFIX              = " + os.environ['IASL_PREFIX'])


        if not os.environ.get('TOOL_CHAIN_TAG'):
            if os.environ.get('VS160COMNTOOLS'):
                os.environ['TOOL_CHAIN_TAG'] = u'VS2019'
            elif os.environ.get('VS150COMNTOOLS'):
                os.environ['TOOL_CHAIN_TAG'] = u'VS2017'
            elif os.environ.get('VS140COMNTOOLS') and os.environ['VS140COMNTOOLS'] == os.path.join(os.environ['ProgramFiles'], u'Microsoft Visual Studio 14.0', u'Common7', u'Tools', u''):
                os.environ['TOOL_CHAIN_TAG'] = u'VS2015'
            elif os.environ.get('VS140COMNTOOLS'):
                os.environ['TOOL_CHAIN_TAG'] = u'VS2015x86'
            elif os.environ.get('CLANG_BIN'):
                os.environ['TOOL_CHAIN_TAG'] = u'CLANGPDB'
            else:
                print ("Please check you have installed MSVC compiler above VS2015")
                exit(1)

    elif sys.platform == 'linux':
        call_make_edk2basetools()
        cmd = ['bash', '-c', '. $WORKSPACE_CORE/edksetup.sh BaseTools']
        os.environ['PATH'] += os.pathsep + os.path.join( os.environ['EDK_TOOLS_PATH'], u'BinWrappers', u'PosixLike')
        if not os.environ.get('TOOL_CHAIN_TAG'):
            os.environ['TOOL_CHAIN_TAG'] = u'GCC'
        print ("cmd : "+str(cmd))
        subprocess.check_call (cmd, stdout=sys.stdout, stderr=sys.stderr)

    elif sys.platform == 'darwin':
        call_make_edk2basetools()
        cmd = ['sh', '-c', '. $WORKSPACE_CORE/edksetup.sh BaseTools']
        os.environ['PATH'] += os.pathsep + os.path.join( os.environ['EDK_TOOLS_PATH'], u'BinWrappers', u'PosixLike')
        os.environ['PATH'] += os.pathsep + os.path.join( os.environ['EDK_TOOLS_PATH'], u'Bin', u'Darwin-x86_64')
        if not os.environ.get('TOOL_CHAIN_TAG'):
            os.environ['TOOL_CHAIN_TAG'] = u'XCODE5'
        print ("cmd : "+str(cmd))
        subprocess.check_call(cmd, stdout=sys.stdout, stderr=sys.stderr)
    else:
        print ('Currently, %s is unsupported' % (sys.platform) )
        exit(1)

def OneSiliconBuild(ProductName, ToolChain = None, Target = None):
    set_environ(ProductName, ToolChain, Target)
    set_edk_environ()

    print ("***********************")
    print ("call build")
    if not os.environ.get('rc-dsc'):
        print("rc-dsc not found, set to default")
        os.environ['rc-dsc'] = os.path.join( u'Product', os.environ['TARGET_PLATFORM'], os.environ['TARGET_PLATFORM'] + u'SiliconPkg.dsc')
    print("rc-dsc=" + os.environ['rc-dsc'])

    if sys.platform == 'win32':
        cmd = [
                u'build',
                '-b', os.environ['TARGET'],
                '-t', os.environ.get('TOOL_CHAIN_TAG'),
                '-p', os.path.join( os.environ['WORKSPACE_SILICON'], u'OneSiliconPkg', os.environ['rc-dsc']),
                '-a', 'X64', '-n', '{0}'.format(multiprocessing.cpu_count())
              ]
    elif sys.platform == 'linux':
        cmd = [
                u'build',
                '-b', os.environ['TARGET'],
                '-t', os.environ.get('TOOL_CHAIN_TAG'),
                '-p', os.path.join( os.environ['WORKSPACE_SILICON'], u'OneSiliconPkg', os.environ['rc-dsc']),
                '-a', 'X64', '-n', '{0}'.format(multiprocessing.cpu_count())
              ]

    elif sys.platform == 'darwin':
        cmd = [
                os.path.join( os.environ['EDK_TOOLS_PATH'], u'Bin', u'CYGWIN_NT-5.1-i686', u'build'),
                '-b', os.environ['TARGET'],
                '-t', os.environ.get('TOOL_CHAIN_TAG'),
                '-p', os.path.join( os.environ['WORKSPACE_SILICON'], u'OneSiliconPkg', os.environ['rc-dsc']),
                '-a', 'X64', '-n', '{0}'.format(multiprocessing.cpu_count())
              ]
    # call build command
    print ("cmd : " + str(cmd))
    result = subprocess.Popen (' '.join (cmd), shell=True, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr, universal_newlines=True)

    # process stdout, stderr, returncode
    outs, errs = result.communicate()

    # forward stdout to output
    print(str(outs))

    # output return code
    return result.returncode

if __name__ == '__main__' :
    opts = get_args()
    exit (OneSiliconBuild(opts.Product, opts.ToolChain, opts.Target))
