/**
 * @file stream_ctrl_if.hh
 * @author adrian.yan (yanyehe@tsingmicro.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-29
 * 
 * 
 */
#ifndef __STREAM_CTRL_IF__H__
#define __STREAM_CTRL_IF__H__

#include <string>
#include <memory>
#include <queue>
#include "mem/port.hh"
#include "sim/sim_object.hh"

#include "stream_ctrl.hh"

namespace gem5 
{
    namespace kuiper
    {
        struct stream_ctrl_in_inst_t
        {

        };
        using StreamInInst = struct stream_ctrl_in_inst_t;

        struct stream_ctrl_out_inst_t
        {

        };
        using StreamOutInst = struct stream_ctrl_out_inst_t;

        class StreamCtrlInputIf : public ResponsePort 
        {
        public:
            StreamCtrlInputIf(const std::string &name, StreamCtrl *stream, gem5::PortID id);


        public:
            /**
             * Send a packet across this port. This is called by the owner and
             * all of the flow control is hanled in this function.
             *
             * @param packet to send.
             */
            bool sendPacket(PacketPtr pkt);

            /**
             * Receive a timing response from the response port.
             */
            bool recvTimingReq(PacketPtr pkt) override;

            /**
             * Called by the request port if sendTimingResp was called on this
             * response port (causing recvTimingResp to be called on the request
             * port) and was unsuccesful.
             */
            void recvRespRetry() override;


        private:
            StreamCtrl *mOwner;
            // std::queue<std::shared_ptr<MesgType>> mMesgQueue;

        };

        class StreamCtrlOutputIf : public RequestPort        
        {
        public:
            StreamCtrlOutputIf(const std::string &name, StreamCtrl *stream);

        public:
            /**
             * Receive a timing response from the response port.
             */
            bool recvTimingResp(PacketPtr pkt) override;

            /**
             * Called by the response port if sendTimingReq was called on this
             * request port (causing recvTimingReq to be called on the responder
             * port) and was unsuccesful.
             */
            void recvReqRetry() override;

            /**
             * Send a packet across this port. This is called by the owner and
             * all of the flow control is hanled in this function.
             *
             * @param packet to send.
             */
            void sendPacket(PacketPtr pkt) {};

        private:
            StreamCtrl *mOwner;
            // std::queue<std::shared_ptr<MesgType>> mMesgQueue;
        };

    }
}

#endif /* __STREAM_CTRL_IF__H__ */


