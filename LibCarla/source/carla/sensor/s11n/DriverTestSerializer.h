#pragma once

#include "carla/Buffer.h"
#include "carla/Memory.h"
#include "carla/sensor/RawData.h"

#include <cstdint>
#include <cstring>

namespace carla {
namespace sensor {

  class SensorData;

namespace s11n {

  class DriverTestSerializer 
  {
    public:
      struct Data{
        float reactiontime;
        float brakingratio;
        MSGPACK_DEFINE_ARRAY(reactiontime, brakingratio)
      };
    
      template <typename SensorT>
      static Buffer Serialize(
        const SensorT &sensor,
        const float reactionTime,
        const float brakingRatio);
    
      static Data DeserializeRawData(const RawData& message) {
        return MsgPack::UnPack<Data>(message.begin(), message.size());
      }

      static SharedPtr<SensorData> Deserialize(RawData &&data);

  };
  template <typename SensorT>
  inline Buffer DriverTestSerializer::Serialize(
      const SensorT &sensor,
      const float reactionTime,
      const float brakingRatio) {
      return MsgPack::Pack(Data{reactionTime,brakingRatio});
  }

} // namespace s11n
} // namespace sensor
} // namespace carla