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

#ifndef __NPU_3DRAM__H__ 
#define __NPU_3DRAM__H__

#include <cassert>
#include "base/types.hh"
#include "params/CgraDram.hh"
#include "sim/clocked_object.hh"

namespace gem5
{
  class CgraDram: public ClockedObject
  {
  public:
    CgraDram(const CgraDramParams &params);

  private:
    Cycles CgraDramLatency(std::uint64_t &addr, std::uint32_t &len, std::uint8_t &buf);

  private:
    Cycles mMinCycles;
    std::uint32_t mBurstBase;
    std::uint32_t mBurstThreshold;
    std::uint32_t mAlign;
    std::uint32_t mCachLine;
    const std::uint32_t mCgraDramCap = 0x4000000;
    std::shared_ptr<std::uint8_t> mCgraDramPtr
  };

} // namespace gem5

#endif /* __NPU_3DRAM__H__ */ 
