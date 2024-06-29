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
from m5.SimObject import SimObject
from m5.params import *
from m5.proxy import *

class NpuCache(ClockedObject):
    type = 'NpuCache'
    cxx_header = "kuiper/npu/npu_cache/npu_cache.hh"
    cxx_class = "gem5::NpuCache"

    # Vector port example. Both the instruction and data ports connect to this
    # port which is automatically split out into two ports.
    cpu_side = VectorResponsePort("CPU side port, receives requests")
    mem_side = RequestPort("Memory side port, sends requests")

    # load_port0 = ResponsePort(" Load port0, read data from L0 cache to cgra executor")
    # load_port1 = ResponsePort(" Load port1, read data from L0 cache to cgra executor")
    # store_port = ResponsePort(" Store port ,store data to L0 cache or 3dram") 

    # read_port = RequestPort("Read data from DDR , sends requests")
    # write_port = RequestPort("Write data to DDR , sends requests")
    
    latency = Param.Cycles(1, "Cycles taken on a hit or to resolve a miss")

    size = Param.MemorySize("16kB", "The size of the cache")

    system = Param.System(Parent.any, "The system this cache is part of")

