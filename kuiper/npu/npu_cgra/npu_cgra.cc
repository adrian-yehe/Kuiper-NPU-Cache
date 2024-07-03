/*
 * Copyright (c) 2024-06-26 adrian.yan 
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
#include <cstdint>
#include <map>
#include "mem/port.hh"
#include "debug/KuiperCgra.hh"
#include "sim/sim_object.hh"
#include "params/KuiperCgra.hh"
#include "npu_cgra.hh"

namespace gem5
{
    KuiperCgra::KuiperCgra(const KuiperCgraParams &params) : SimObject(params),
                                                             load0_port(params.name + ".load0_port", 0, this),
                                                             load1_port(params.name + ".load1_port", 1, this),
                                                             store_port(params.name + ".store_port", 2, this),
                                                             mReqID(0),
                                                             mLatency(params.latency),
                                                             mEvent([this]
                                                                    { ProcessEvent(); }, name() + ".event"),
                                                             stats(this)

    {
    }

    PacketPtr KuiperCgra::Package(Addr &addr, std::uint8_t *buf, Request::Flags flag, gem5::MemCmd cmd)
    {
        // Create a new request-packet pair
        RequestPtr req = std::make_shared<Request>(
            addr, mReqDataLen,  flag, 0/* mReqID++ */);

        PacketPtr pkt = new Packet(req, cmd, this->mReqDataLen);

        pkt->dataDynamic(buf);
        return pkt;
    }

    bool KuiperCgra::CachePort::recvTimingResp(PacketPtr pkt)
    {
        return owner->HandleResponse(pkt);
    }

    void KuiperCgra::CachePort::recvReqRetry()
    {
        if(0 == owner->mBlockPktArray[id].size())
            assert(0);

        auto pkt = owner->mBlockPktArray[id].back(); 
        sendTimingReq(pkt);
        owner->mBlockPktArray[id].pop();
    }

    void KuiperCgra::CachePort::recvRangeChange()
    {

    }

    bool KuiperCgra::Load0(Addr &addr, std::uint8_t *buf, std::uint32_t len)
    {
        assert(!buf || !addr);
        auto ptr = Package(addr, buf,0 /* Request::Flags::LDCQ */, MemCmd::ReadReq);
        auto ret = this->load0_port.sendTimingReq(ptr);

        if(false == ret)
        {
            DPRINTF(KuiperCgra, "Load0 port send request to L0 fail!!!!!!");
            this->mBlockPktArray[0].push(ptr);
            return ret;
        }

        return ret;
    }

    bool KuiperCgra::Load1(Addr &addr,  std::uint8_t *buf, std::uint32_t len)
    {
        assert(!buf || !addr);
        auto ptr = Package(addr, buf, 0/* Request::Flags::LDCQ */, MemCmd::ReadReq);
        auto ret = this->load1_port.sendTimingReq(ptr);

        if(false == ret)
        {
            DPRINTF(KuiperCgra, "Load1 port send request to L0 fail!!!!!!");
            this->mBlockPktArray[1].push(ptr);
            return ret;
        }
        return ret;
    }

    bool KuiperCgra::Store(Addr &addr, std::uint8_t *buf, std::uint32_t len)
    {
        assert(!buf || !addr);
        auto ptr = Package(addr, buf, 0 /* Request::Flags::STCQ */, MemCmd::WriteReq);
        auto ret = this->store_port.sendTimingReq(ptr);

        if(false == ret)
        {
            DPRINTF(KuiperCgra, " Store port send request to L0 fail!!!!!!");
            this->mBlockPktArray[2].push(ptr);
            return ret;
        }
        return ret;
    }

    void KuiperCgra::ProcessEvent()
    {
        // if(0 == this->mWrite)
        //    Write();
        // else if( 2 <= this->mWrite && 0 == mRead )
        //     Read0();

        if( 0 == this->mRead )
            Read0();

        DPRINTF(KuiperCgra, "CGRA core step\r\n");

        schedule(mEvent, curTick() +  mLatency);
    }

    bool KuiperCgra::HandleResponse(PacketPtr pkt)
    {
        this->mWrite++;
        DPRINTF(KuiperCgra, "Recive response form L0;Addr: %#x\r\n", reinterpret_cast<std::uint64_t>(pkt->getConstPtr<uint8_t>()));

        DDUMP(KuiperCgra, pkt->getConstPtr<uint8_t>(), this->mReqDataLen + 1024);
        return true;
    }

    void KuiperCgra::startup(void)
    {
        schedule(mEvent, mLatency);
    }

    Port &
    KuiperCgra::getPort(const std::string &if_name, PortID idx)
    {
        // This is the name from the Python SimObject declaration in SimpleCache.py
        if (if_name == "store_port")
        {
            panic_if(idx != InvalidPortID,
                     "Mem side of simple cache not a vector port");
            return store_port;
        }
        else if (if_name == "load0_port")
        {
            // We should have already created all of the ports in the constructor
            return load0_port;
        }
        else if (if_name == "load1_port")
        {
            // We should have already created all of the ports in the constructor
            return load1_port;
        }
        else
        {
            // pass it along to our super class
            return SimObject::getPort(if_name, idx);
        }
    }

    void KuiperCgra::Step(void)
    {
        auto *buf = new std::uint8_t[1024] {0xAA};
        memset(buf, 0xAA, 1024);
        Addr addr = 1024;
        DDUMP(KuiperCgra, buf, this->mReqDataLen);

        auto store_pkt = Package(addr, buf, 0, MemCmd::WriteReq);

        store_port.sendTimingReq(store_pkt);

        DPRINTF(KuiperCgra, "load0 port send requeset to l0\r\n");
    };

    void KuiperCgra::Read0(void)
    {
        auto *buf = new std::uint8_t[1024]{0xAA};
        Addr addr = 0x400000;
        memset(buf, 0xAA, 1024);

        DPRINTF(KuiperCgra, "Read0 addr %#x\n", reinterpret_cast<std::uint64_t>(buf));
        DDUMP(KuiperCgra, buf, this->mReqDataLen + 1024);

        auto pkt = Package(addr, buf, 0, MemCmd::ReadReq);

        load0_port.sendTimingReq(pkt);
        this->mRead++;

        DPRINTF(KuiperCgra, "load0 port send requeset to l0\r\n");
    };

    void KuiperCgra::Write(void)
    {
        auto *buf = new std::uint8_t[1024]{0xBB};
        Addr addr = 1024;
        memset(buf, 0xBB, 1024);
        DPRINTF(KuiperCgra, "Write addr %#x\n", reinterpret_cast<std::uint64_t>(buf));
        DDUMP(KuiperCgra, buf, this->mReqDataLen);

        auto pkt = Package(addr, buf, 0, MemCmd::WriteReq);

        store_port.sendTimingReq(pkt);

        this->mWrite++;
        DPRINTF(KuiperCgra, "Store port send requeset to l0\r\n");
    };

    KuiperCgra::KuiperCgraCacheStats::KuiperCgraCacheStats(statistics::Group *parent)
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

};
