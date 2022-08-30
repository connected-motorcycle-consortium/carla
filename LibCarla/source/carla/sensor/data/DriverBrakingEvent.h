#pragma once

#include "carla/sensor/s11n/DriverTestSerializer.h"
#include "carla/sensor/SensorData.h"

namespace carla {
namespace sensor {
namespace data {

  class DriverBrakingEvent : public SensorData {
  public:

    using Super = SensorData;

    using Serializer = s11n::DriverTestSerializer;

    friend Serializer;

    float _reactionTime;
    float _brakingRatio;


    explicit DriverBrakingEvent(const RawData &&data)
      : Super(data),
       _reactionTime(Serializer::DeserializeRawData(data).reactiontime),
       _brakingRatio(Serializer::DeserializeRawData(data).brakingratio){}
       

 

    float GetReactionTime() const {
      return _reactionTime;
    }


    float GetBrakingRatio() const {
      return _brakingRatio;
    }

  

 


  };

} // namespace data
} // namespace sensor
} // namespace carla
