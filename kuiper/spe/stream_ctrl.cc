/**
 * @file stream_ctrl.cc
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
#include "stream_ctrl.hh"

#include "base/trace.hh"
#include "base/logging.hh"
#include "debug/StreamCtrl.hh"

namespace gem5 
{
  namespace kuiper 
  {
    StreamCtrl::StreamCtrl(const StreamCtrlParams &p):
                SimObject(p),
                mLatency(p.time_to_wait),
                mTimeLeft(p.number_of_fires),
                mEvent([this] {processEvent();}, name())

    {
      DPRINTF(StreamCtrl,"Construct StreamCtrl object;\n");
    }

    void 
    StreamCtrl::startup() 
    {
        schedule(mEvent, curTick() + mLatency);
    }

    void 
    StreamCtrl::processEvent() 
    {
      DPRINTF(StreamCtrl, " Process spe module event\n");
    }
  }
}
