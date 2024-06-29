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
#include "kuiper_cgra.hh"

namespace gem5
{
    KuiperCgra::KuiperCgra(const KuiperCgraParams &params) : ClockedObject(params),
                                                             mLoad0Port(params.name + ".load0_port", 0, this),
                                                             mLoad1Port(params.name + ".load1_port", 1, this),
                                                             mStorePort(params.name + ".store_port", 2, this),
                                                             mReqID(0)
    {
    }

    PacketPtr KuiperCgra::Package(Addr &addr, Addr *buf, Request::Flags flag, gem5::MemCmd cmd)
    {
        // Create a new request-packet pair
        RequestPtr req = std::make_shared<Request>(
            addr, mReqDataLen,  flag, mReqID++);

        PacketPtr pkt = new Packet(req, cmd, this->mReqDataLen);

        pkt->dataDynamic<Addr>(buf);
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

    bool KuiperCgra::Load0(Addr &addr, Addr *buf, std::uint32_t len)
    {
        assert(!buf || !addr);
        auto ptr = Package(addr, buf,0 /* Request::Flags::LDCQ */, MemCmd::ReadReq);
        auto ret = this->mLoad0Port.sendTimingReq(ptr);

        if(false == ret)
        {
            DPRINTF(KuiperCgra, "Load0 port send request to L0 fail!!!!!!");
            this->mBlockPktArray[0].push(ptr);
            return ret;
        }
        return ret;
    }

    bool KuiperCgra::Load1(Addr &addr, Addr *buf, std::uint32_t len)
    {
        assert(!buf || !addr);
        auto ptr = Package(addr, buf, 0/* Request::Flags::LDCQ */, MemCmd::ReadReq);
        auto ret = this->mLoad1Port.sendTimingReq(ptr);

        if(false == ret)
        {
            DPRINTF(KuiperCgra, "Load1 port send request to L0 fail!!!!!!");
            this->mBlockPktArray[1].push(ptr);
            return ret;
        }
        return ret;
    }

    bool KuiperCgra::Store(Addr &addr, Addr *buf, std::uint32_t len)
    {
        assert(!buf || !addr);
        auto ptr = Package(addr, buf, 0 /* Request::Flags::STCQ */, MemCmd::ReadReq);
        auto ret = this->mStorePort.sendTimingReq(ptr);

        if(false == ret)
        {
            DPRINTF(KuiperCgra, " Store port send request to L0 fail!!!!!!");
            this->mBlockPktArray[2].push(ptr);
            return ret;
        }
        return ret;
    }

    bool KuiperCgra::HandleResponse(PacketPtr pkt)
    {
        assert(pkt->isResponse());
        DPRINTF(KuiperCgra, "Recive response form L0;Addr");

        return true;
    }
};
