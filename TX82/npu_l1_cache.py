# -*- coding: utf-8 -*-
# Copyright (c) 2016 adrian-yan Tsing micro
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

from m5.objects.ClockedObject import ClockedObject
from m5.objects import Cache
from m5.SimObject import SimObject
from m5.params import *
from m5.proxy import *

# class CgraL1Cache(Cache):
#     type = 'CgraL1Cache'
#     cxx_header = "kuiper/npu/npu_cache/npu_l1_cache.hh"
#     cxx_class = "gem5::CgraL1Cache"

#     # Default parameters
#     size = "256kB"
#     assoc = 8
#     tag_latency = 20
#     data_latency = 20
#     response_latency = 20
#     mshrs = 20
#     tgts_per_mshr = 12
    
#     # Vector port. Load0/1 and store data ports connect to this
#     # port which is automatically split out into two ports.
#     # cpu_side = VectorResponsePort("CPU side port, receives requests")
#     # mem_side = RequestPort("Memory side port, sends requests")
    
#     # dram_port = VectorRequestPort(" 3DRAM Bank port, read and write data ")
   
#     # def connectCacheBus(self, bus):
#     #     self.cpu_side = bus.mem_side_ports

#     # def connectMemSideBus(self, bus):
#     #     self.mem_side = bus.cpu_side_ports

#     # def connect3DRam(self, dram): 

# # Add the common scripts to our path
# m5.util.addToPath("../../../")

# Some specific options for caches
# For all options see src/mem/cache/BaseCache.py

import m5
from m5.objects import Cache
from m5.params import *

class CgraL1Cache(Cache):
    """Simple L1 data cache with default values"""

    # Default parameters
    size = "256kB"
    assoc = 8
    tag_latency = 20
    data_latency = 20
    response_latency = 20
    mshrs = 20
    tgts_per_mshr = 12

    def connectCPUSideBus(self, bus):
        self.cpu_side = bus.mem_side_ports

    def connectMemSideBus(self, bus):
        self.mem_side = bus.cpu_side_ports
