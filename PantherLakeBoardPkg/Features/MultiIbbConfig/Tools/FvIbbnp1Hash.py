## @file
#  This script is called to auto generate FvIbbnp1 hash
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
import os
import sys
import argparse
import subprocess
import shutil
import stat

from stat import S_IWRITE, S_IREAD, S_IEXEC

#
# Globals for help information
#
__prog__        = sys.argv[0]
__copyright__   = 'Copyright (c) 2024, Intel Corporation. All rights reserved.'
__description__ = '\n'
PYTHON_COMMAND = 'python3'

def CreateDigest (Payload, PayloadDigest, ToolPath, Verbose = False):

    if ToolPath is None:
        ToolPath = ''
    Command = '"{Path}" '.format (Path = os.path.join (ToolPath, 'openssl'))
    Command = Command + 'dgst -binary -sha384 '
    Command = Command + '{input} '.format (input = Payload)
    Command = Command + '> {output}'.format (output = PayloadDigest)

    if Verbose:
        print (Command)
    try:
        Process = subprocess.Popen (Command, stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, shell = True)
        Result = Process.communicate('')
        print(Result[0].decode('utf-8', 'ignore'))
    except:
        raise ValueError ('{FileName}: error: can not generate digest.'.format (FileName = os.path.basename (__file__)))

    if Process.returncode != 0:
        print (Result[1].decode ())
        raise ValueError ('{FileName}: error: generate digest failed.'.format (FileName = os.path.basename (__file__)))

def RemoveReadonly (func, path, _) :
  # Clear the readonly bit and reattempt the removal
  os.chmod (path, stat.S_IWRITE)
  func (path)
  return

def RemoveItem (Item) :
  # Remove Item if exist, will check if Dir or file to select remove method
  if os.path.exists (Item) :
    print ('  remove', Item)
    if os.path.isdir (str (Item)) :
      shutil.rmtree (str (Item), onerror = RemoveReadonly)
    else :
      os.chmod (Item, stat.S_IWRITE)
      os.remove (Item)
  return

def CopyIfSrcExists (src, dst) :
  # remove empty path, and change all separator to os.sep
  src = os.path.normpath (src)
  dst = os.path.normpath (dst)
  # check src exist
  if os.path.exists (src) :
    # check dst exist and remove
    if os.path.exists (dst) :
      print ('file {} exist'.format (dst))
      RemoveItem (dst)
    # check dst parent dir exist and makedirs
    if not os.path.isdir (os.path.dirname (dst)) :
      os.makedirs (os.path.dirname (dst))
    # copy src to dst
    print ('Copy {} \n  to {}\n'.format (src, dst))
    if os.path.isfile (src) :
      shutil.copyfile (src, dst)
    else :
      shutil.copytree (src, dst)
  else :
    print ('Warning : {} not exist'.format(src))
  # dst chmod
  if os.path.exists (dst) :
    os.chmod (dst, S_IREAD|S_IWRITE|S_IEXEC)
  return
# CopyIfSrcExists End

if __name__ == "__main__":

    #
    # Create command line argument parser object
    #
    parser = argparse.ArgumentParser (
                        prog = __prog__,
                        description = __description__ + __copyright__,
                        )

    parser.add_argument ("-t", "--temp", dest = 'TempFolder', type = str, required = True,
                         help = "Path of temp folder to store hash file.")
    parser.add_argument ("--openssl-tool-path", dest = 'OpensslToolPath', type = str,
                         help = "Path to OpenSSL tool. Optional if path to tools are already in PATH.")
    #
    # Parse command line arguments
    #
    args = parser.parse_args ()
    HASH_TMP_FOLDER = args.TempFolder
    BUILD_FV_FOLDER = os.path.dirname (HASH_TMP_FOLDER)

    if not os.path.exists (HASH_TMP_FOLDER) :
        os.mkdir (HASH_TMP_FOLDER)

    if not os.path.exists (args.OpensslToolPath):
        raise Exception ('FvIbbHash.py: error: {Path} does not exist!'.format (Path = args.OpensslToolPath))
    OPENSSL_FOLDER = args.OpensslToolPath

    # Gen FvIbbnp1Digest
    CreateDigest (os.path.join (BUILD_FV_FOLDER, 'FVIBBNP1.Fv'),
                  os.path.join (HASH_TMP_FOLDER, 'FvIbbnp1Digest.bin'),
                  OPENSSL_FOLDER)

    sys.exit (0)
