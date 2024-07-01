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
#ifndef __KUIPER__CGRA__H__
#define __KUIPER__CGRA__H__

#include <queue>
#include "mem/port.hh"
#include "mem/request.hh"
#include "mem/packet.hh"

#include "sim/clocked_object.hh"
#include "params/KuiperCgra.hh"

namespace gem5
{
  class KuiperCgra : public SimObject
  {
  public:
    KuiperCgra(const KuiperCgraParams &param);

  public:
    bool Load0(Addr &addr, Addr *buf, std::uint32_t len = 128);
    bool Load1(Addr &addr, Addr *buf, std::uint32_t len = 128);
    bool Store(Addr &addr, Addr *buf, std::uint32_t len = 128);

  private:
    void Step();
    void SendPacket();

    bool HandleResponse(PacketPtr pkt);
    PacketPtr Package(Addr &addr, Addr *buf, Request::Flags flag, gem5::MemCmd cmd);

    /**
     * Event function to execute on an event trigger
     */
    void ProcessEvent();

  private:
    /**
     * Port on the CGRA core-side that receives responses.
     * Mostly just forwards requests to the cache (owner)
     */
    class CachePort : public RequestPort
    {
    private:
      /// The object that owns this object (KuiperCgra)
      KuiperCgra *owner;

      /// If we tried to send a packet and it was blocked, store it here
      // PacketPtr blockedPacket;

      /// Since this is a vector port, need to know what number this one is
      int id;

    public:
      /**
       * Constructor. Just calls the superclass constructor.
       */
      CachePort(const std::string &name, int id, KuiperCgra *owner) : RequestPort(name),
                                                              owner(owner),
                                                              id(id)
      {
      }

      // /**
      //  * Send a packet across this port. This is called by the owner and
      //  * all of the flow control is hanled in this function.
      //  * This is a convenience function for the KuiperCgra to send pkts.
      //  *
      //  * @param packet to send.
      //  */
      // void sendPacket(PacketPtr pkt);

    protected:
      /**
       * Receive a timing response from the response port.
       */
      bool recvTimingResp(PacketPtr pkt) override;

      /**
       * Called by the response port if sendTimingReq was called on this
       * request port (causing recvTimingReq to be called on the response
       * port) and was unsuccesful.
       */
      void recvReqRetry() override;

      /**
       * Called to receive an address range change from the peer response
       * port. The default implementation ignores the change and does
       * nothing. Override this function in a derived class if the owner
       * needs to be aware of the address ranges, e.g. in an
       * interconnect component like a bus.
       */
      void recvRangeChange() override;
    };

  private:
    /* index 0: loadport0 ; index 1: loadport1 ; index 2: store port */
    CachePort load0_port;
    CachePort load1_port;
    CachePort store_port;

    /// Request data length which is fix to 1024 bit
    const std::uint32_t mReqDataLen = 128;

    /// Request ID
    RequestorID mReqID;

    /// For tracking the miss latency
    Tick missTime;

    /// Latency between calling the event (in ticks)
    const Tick mLatency;
    
    /// An event that wraps the above function
    EventFunctionWrapper mEvent;


  public:
    /// For request block queue
    std::array<std::queue<PacketPtr>, 3> mBlockPktArray;

    /// Cache statistics
  protected:
    struct KuiperCgraCacheStats : public statistics::Group
    {
        KuiperCgraCacheStats(statistics::Group *parent);
        statistics::Scalar hits;
        statistics::Scalar misses;
        statistics::Histogram missLatency;
        statistics::Formula hitRatio;
    } stats;

  public:
    void startup() override;

    /**
     * Get a port with a given name and index. This is used at
     * binding time and returns a reference to a protocol-agnostic
     * port.
     *
     * @param if_name Port name
     * @param idx Index in the case of a VectorPort
     *
     * @return A reference to the given port
     */
    Port &getPort(const std::string &if_name,
                  PortID idx=InvalidPortID) override;
  };
}


#endif /* __KUIPER__CGRA__H__ */
