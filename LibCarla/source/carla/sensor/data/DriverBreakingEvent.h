#pragma once

#include "carla/sensor/s11n/DriverTestSerializer.h"
#include "carla/sensor/SensorData.h"

namespace carla {
namespace sensor {
namespace data {

  class DriverBreakingEvent : public SensorData {
  public:

    using Super = SensorData;

    using Serializer = s11n::DriverTestSerializer;

    friend Serializer;

    float _reactionTime;
    float _breakingRatio;


    explicit DriverBreakingEvent(const RawData &&data)
      : Super(data),
       _reactionTime(Serializer::DeserializeRawData(data).reactiontime),
       _breakingRatio(Serializer::DeserializeRawData(data).breakingratio){}
       

 

    float GetReactionTime() const {
      return _reactionTime;
    }


    float GetBreakingRatio() const {
      return _breakingRatio;
    }

  

 


  };

} // namespace data
} // namespace sensor
} // namespace carla
