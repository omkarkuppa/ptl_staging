#!/usr/bin/env python3
#
## @file
# MaxConcurrentThreads.py
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
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
##
import os
import sys
import subprocess
import traceback

def GetMaxConcurrentThreads():
    #
    # When running in a container, it is possible for CPU utilization quotas to be
    # imposed. This prevents a single container from using all available CPU time.
    #
    # One cannot blindly assume that the number of logical CPU cores that exist on
    # the system is equal to the number of concurrent threads one is allowed to
    # execute on at a time.
    #
    # The fix is to check if a CPU utilization quota has been imposed. If there is
    # a quota, then limit the concurrent thread count to fit inside of the quota.
    # If there is no quota, then all available CPU threads can be used.
    #
    if sys.platform == "linux":
        # Check if a quota exists using the cgroups v2 method
        if os.path.isfile('/sys/fs/cgroup/cpu.max'):
            with open('/sys/fs/cgroup/cpu.max', 'r') as fh:
                data = fh.read().split()
                if data[0] == 'max':
                    # no quota
                    return os.cpu_count()
                quota = int(data[0])
                period = int(data[1])
                cpu_quota = quota // period
                if quota % period > 0:
                    cpu_quota += 1
                return cpu_quota
        # Check if a quota exists using the cgroups v1 method
        elif os.path.isfile('/sys/fs/cgroup/cpu/cpu.cfs_quota_us'):
            quota = -1
            period = 0
            with open ('/sys/fs/cgroup/cpu/cpu.cfs_quota_us', 'r') as fh:
                quota = int(fh.read())
            with open ('/sys/fs/cgroup/cpu/cpu.cfs_period_us', 'r') as fh:
                period = int(fh.read())
            if quota == -1:
                # no quota
                return os.cpu_count()
            cpu_quota = quota // period
            if quota % period > 0:
                cpu_quota += 1
            return cpu_quota
        else:
            # no cgroup, no quota
            return os.cpu_count()
    elif sys.platform == "win32":
        #
        # The Python Launcher for Windows creates its own Job Object and assigns
        # that Job Object to the Python interpreter's process when it creates
        # the child process. This Job Object is nested inside the parent Job
        # Object created by the container runtime. The problem with this is the
        # parent Job Object contains the CPU Rate Control information, whereas
        # the child Job Object does not.
        #
        # Therefore, in order to read the quota data a child process must be
        # created to "breakaway" from the Job Object created by the Python
        # Launcher for Windows, moving up the hierarchy to the first parent Job
        # Object that does not permit breakaway, which should be the Job Object
        # created by the container runtime.
        #
        modulename = os.path.splitext(os.path.basename(__file__))[0]
        result = subprocess.run([sys.executable,
                                 '-c', 'import {0}; import sys; sys.exit({0}._GetCpuQuotaWin32ChildProcess())'.format(modulename)
                                 ],
                                 universal_newlines=True,
                                 stdout=subprocess.PIPE,
                                 cwd=os.path.abspath(os.path.dirname(__file__)))
        if result.returncode:
            raise EnvironmentError('Determining CPU quota failed.')
        return int(result.stdout)
    else:
        # Quota check not implemented on platforms other than Windows & Linux
        return os.cpu_count()

def _GetCpuQuotaWin32ChildProcess():
    try:
        print(_GetCpuQuotaWin32())
        return 0
    except:
        traceback.print_exc()
        return 1

def _GetCpuQuotaWin32():
    # Definitions for making the QueryInformationJobObject() Win32 API call
    from ctypes import windll, POINTER, byref, GetLastError, Structure, Union, WinError
    from ctypes import c_void_p, c_ushort, c_int, c_ulong, sizeof

    class _JOBOBJECT_CPU_RATE_MIN_MAX(Structure):
        _fields_ = [('MinRate', c_ushort),
                    ('MaxRate', c_ushort)]
    class _JOBOBJECT_CPU_RATE_UNION(Union):
        _fields_ = [('CpuRate', c_ulong),
                    ('Weight', c_ulong),
                    ('MinMax', _JOBOBJECT_CPU_RATE_MIN_MAX)]
    class _JOBOBJECT_CPU_RATE_CONTROL_INFORMATION(Structure):
        _fields_ = [('ControlFlags', c_ulong),
                    ('Union', _JOBOBJECT_CPU_RATE_UNION)]
    JobObjectCpuRateControlInformation = 15
    ERROR_ACCESS_DENIED = 5
    JOB_OBJECT_CPU_RATE_CONTROL_ENABLE = 0x1
    JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP = 0x4
    JOB_OBJECT_CPU_RATE_CONTROL_MIN_MAX_RATE = 0x10
    WIN32_MAX_CPU_RATE = 10000
    kernel32 = windll.kernel32
    QueryInformationJobObject = kernel32.QueryInformationJobObject
    QueryInformationJobObject.argtypes = [c_void_p, c_int, POINTER(_JOBOBJECT_CPU_RATE_CONTROL_INFORMATION), c_ulong, POINTER(c_ulong)]
    QueryInformationJobObject.restype = c_int

    # Call QueryInformationJobObject() to retrieve CPU Quota information
    cpuRateInfo = _JOBOBJECT_CPU_RATE_CONTROL_INFORMATION()
    returnedBytes = c_ulong(0)
    if QueryInformationJobObject(c_void_p(0), JobObjectCpuRateControlInformation, byref(cpuRateInfo), sizeof(cpuRateInfo), byref(returnedBytes)) == 0:
        status = GetLastError()
        if status == ERROR_ACCESS_DENIED:
            # no quota
            return os.cpu_count()
        else:
            raise WinError(status)
    if returnedBytes.value < sizeof(cpuRateInfo):
        raise ValueError('returned size from QueryInformationJobObject() is too small')
    if (cpuRateInfo.ControlFlags & JOB_OBJECT_CPU_RATE_CONTROL_ENABLE) != 0:
        quota = WIN32_MAX_CPU_RATE
        if (cpuRateInfo.ControlFlags & JOB_OBJECT_CPU_RATE_CONTROL_HARD_CAP) != 0:
            quota = cpuRateInfo.Union.CpuRate
        elif (cpuRateInfo.ControlFlags & JOB_OBJECT_CPU_RATE_CONTROL_MIN_MAX_RATE) != 0:
            quota = cpuRateInfo.Union.MinMax.MaxRate
        cpu_count = (quota * os.cpu_count()) // WIN32_MAX_CPU_RATE
        if (quota * os.cpu_count()) % WIN32_MAX_CPU_RATE > 0:
            cpu_count += 1
        return cpu_count
    else:
        # no quota
        return os.cpu_count()

if __name__ == '__main__':
    try:
        print(GetMaxConcurrentThreads())
    except:
        traceback.print_exc()
        sys.exit(1)
    sys.exit(0)
