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
#include "mem/port.hh"

namespace gem5
{
  using Latency = std::uint32_t;

  template <typename DRAM_CAP = 0x4000000, typename BANK_CAP = 0x1000000>
  class Dram 
  {
      public:
        Dram() {
          mDramPtr = std::make_unique<std::uint8_t>[mDramCap];
        }
        
      private:
        Latency DramLatency(std::uint64_t &addr, std::uint32_t &len, std::uint8_t &buf) {
          assert(buf);

          std::uint32_t burst = len / this->mAlign;
          if( burst +  mBurstBase > mMinLatency )
            return (burst + mBurstBase);
          else 
            return mMinLatency;
        }

      private:
        const std::uint32_t mMinLatency = 23;
        const std::uint32_t mBurstBase = 16;
        const std::uint32_t mAlign = 256; 
        std::uint32_t mDramCap =  DRAM_CAP;
        std::shared_ptr<std::uint8_t> mDramPtr
  };

} // namespace gem5

#endif /* __NPU_3DRAM__H__ */ 
