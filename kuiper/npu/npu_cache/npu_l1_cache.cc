/*
 * Copyright (c) 2024 adrian.yan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "npu_l1_cache.hh"

#include "base/compiler.hh"
#include "base/random.hh"
#include "debug/CgraL1Cache.hh"
#include "sim/system.hh"

namespace gem5
{
    CgraL1Cache::CgraL1Cache(const CgraL1CacheParams &params) : Cache(params),
                                                                stats(this)
    {
    }

    void CgraL1Cache::recvTimingReq(PacketPtr pkt)
    {
        DPRINTF(CgraL1Cache, "Recive timing request from L0 cache\r\n");
        
        // assert(pkt->needsResponse());

        pkt->makeResponse();
        Cache::cpuSidePort.sendTimingResp(pkt); 
    }

    void CgraL1Cache::Process3dRamRequest(PacketPtr pkt)
    {


    }

    CgraL1Cache::CgraL1CacheStats::CgraL1CacheStats(statistics::Group *parent)
        : statistics::Group(parent),
          ADD_STAT(hits, statistics::units::Count::get(), "Number of hits"),
          ADD_STAT(misses, statistics::units::Count::get(), "Number of misses"),
          ADD_STAT(missLatency, statistics::units::Tick::get(),
                   "Ticks for misses to the cache"),
          ADD_STAT(hitRatio, statistics::units::Ratio::get(),
                   "The ratio of hits to the total accesses to the cache",
                   hits / (hits + misses))
    {
        missLatency.init(16); // number of buckets
    }

} // namespace gem5
