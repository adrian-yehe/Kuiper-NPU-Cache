#ifndef UTILS_PORT__H__
#define UTILS_PORT__H__

#include <string>
#include <memory>
#include <queue>
#include "mem/port.hh"
#include "sim/sim_object.hh"

namespace gem5 
{
    namespace kuiper
    {
        template <typename OwnerType, typename MesgType>
        class SlaveRespPort : public ResponsePort
        {
        public:
            /**
             * @brief Construct: Just calls the superclass constructor.
             *
             */
            SlaveRespPort(const std::string &name, SimObject *sim, OwnerType *owner):
            ResponsePort(name, sim),
            mOwner(owner)
           
            {};

        public:
            /**
             * Send a packet across this port. This is called by the owner and
             * all of the flow control is hanled in this function.
             *
             * @param packet to send.
             */
            void sendPacket(PacketPtr pkt);

            /**
             * Receive a timing response from the response port.
             */
            bool recvTimingReq(PacketPtr pkt) override
            {
                return true;
            };

            /**
             * Called by the request port if sendTimingResp was called on this
             * response port (causing recvTimingResp to be called on the request
             * port) and was unsuccesful.
             */
            void recvRespRetry() override {};

        private:
            OwnerType *mOwner;
            std::queue<std::shared_ptr<MesgType>> mMesgQueue;
        };

        template <typename OwnerType, typename MesgType>
        class MasterReqPort : public RequestPort
        {
        public:
            /**
             * @brief Construct: Just calls the superclass constructor.
             *
             */
            MasterReqPort(const std::string &name, SimObject *sim, OwnerType *owner):
            RequestPort(name, sim),
            mOwner(owner)
            {};

        public:
            /**
             * Receive a timing response from the response port.
             */
            bool recvTimingResp(PacketPtr pkt) override 
            {
                return true;
            };

            /**
             * Called by the response port if sendTimingReq was called on this
             * request port (causing recvTimingReq to be called on the responder
             * port) and was unsuccesful.
             */
            void recvReqRetry() override{};

            /**
             * Send a packet across this port. This is called by the owner and
             * all of the flow control is hanled in this function.
             *
             * @param packet to send.
             */
            void sendPacket(PacketPtr pkt) {};

        private:
            OwnerType *mOwner;
            std::queue<std::shared_ptr<MesgType>> mMesgQueue;
        };
    }
}

#endif /* UTILS_PORT__H__*/
