/*
 * Copyright (c) 2017 Jason Lowe-Power
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

#ifndef __KUIPER_NPU_L1_CACHE_HH__
#define __KUIPER_NPU_L1_CACHE_HH__

#include <unordered_map>

#include "base/statistics.hh"
#include "mem/port.hh"
#include "mem/request.hh"
#include "mem/packet.hh"
#include "mem/cache/cache.hh"
#include "params/CgraL1Cache.hh"
#include "sim/clocked_object.hh"

// #include "../npu_3dram/npu_3dram.hh"

namespace gem5
{
  /**
   * A CGRA cache object.
   */
  class CgraL1Cache : public Cache
  {
  private:


  protected:
    void recvTimingReq(PacketPtr pkt) override;

  private:
    void Process3dRamRequest(PacketPtr pkt);

    /// Cache statistics
  protected:
    struct CgraL1CacheStats : public statistics::Group
    {
      CgraL1CacheStats(statistics::Group *parent);
      statistics::Scalar hits;
      statistics::Scalar misses;
      statistics::Histogram missLatency;
      statistics::Formula hitRatio;
    } stats;

  public:
    /** constructor
     */
    CgraL1Cache(const CgraL1CacheParams &params);
  };

} // namespace gem5

#endif // __KUIPER_NPU_L1_CACHE_HH__
