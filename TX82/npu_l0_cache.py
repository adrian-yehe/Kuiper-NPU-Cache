# Copyright (c) 2015 Jason Power
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met: redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer;
# redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution;
# neither the name of the copyright holders nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

""" Caches with options for a simple gem5 configuration script

This file contains L1 I/D and L2 caches to be used in the simple
gem5 configuration script. It uses the SimpleOpts wrapper to set up command
line options from each individual class.
"""

import m5
from m5.objects import Cache
from m5.objects.ClockedObject import ClockedObject
from m5.params import *

# # Add the common scripts to our path
# m5.util.addToPath("../../../")

# Some specific options for caches
# For all options see src/mem/cache/BaseCache.py
class L0CacheBase(Cache):
    """Simple L1 Cache with default values"""
    assoc = 2
    tag_latency = 2
    data_latency = 2
    response_latency = 2
    mshrs = 4
    tgts_per_mshr = 20

    def connectBus(self, bus):
        """Connect this cache to a memory-side bus"""
        self.mem_side = bus.cpu_side_ports

    def connectCgra(self, cpu):
        """Connect this cache's port to a CPU-side port
        This must be defined in a subclass"""
        raise NotImplementedError

class CgraL0Cache(L0CacheBase):
    # Set the default size
    size = "256kB"

    # Vector port. Load0/1 and store data ports connect to this
    # port which is automatically split out into two ports.
    cpu_side = VectorResponsePort("CPU side port, receives requests")
    
    def connectCgra(self, cpu):
        """Connect this cache's port to a CGRA cache port"""
        self.cpu_side = cpu.load0_port
        self.cpu_side = cpu.load1_port
        self.cpu_side = cpu.store_port
