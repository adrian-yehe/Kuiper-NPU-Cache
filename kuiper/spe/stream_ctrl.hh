/**
 * @file stream_ctrl.hh
 * @author adrian.yan (yanyehe@tsingmicro.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-01
 * 
 * ************************************************************************
 * 2024 tm@tsingmicro.com Licensed under the Apache License, Version 2.0
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.tsingmicro.com/
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an  BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ************************************************************************
 * 
 */
#ifndef __STREAM_CTRL_H__
#define __STREAM_CTRL_H__

#include "sim/sim_object.hh"
#include "params/StreamCtrl.hh"
#include "mem/port.hh"

#include "stream_ctrl_if.hh"

/**
 * @brief 
 * 
 */
namespace gem5 
{
  namespace kuiper 
  {
    class StreamCtrlInputIf;
    class StreamCtrlOutputIf;

    class StreamCtrl : public SimObject
    {
    public:
        StreamCtrl(const StreamCtrlParams &p);
        virtual ~StreamCtrl(){};

    public:
        void startup() override;
    
    private:
        void processEvent();

    private:
        const Tick mLatency;
        std::uint32_t mTimeLeft;
        EventFunctionWrapper mEvent;

    private:

       StreamCtrlInputIf *mInstInputIf = nullptr; 
       StreamCtrlOutputIf *mDteInstOutputIf = nullptr;
       StreamCtrlOutputIf *mSsuInstOutputIf = nullptr;
    };
  }
};

#endif /* __STREAM_CTRL_H__ */
