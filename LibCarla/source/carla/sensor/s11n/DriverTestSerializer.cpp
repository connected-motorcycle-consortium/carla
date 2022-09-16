// Copyright (c) 2022 Connected Motorcycle Consortium (CMC).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include "carla/sensor/s11n/DriverTestSerializer.h"
#include "carla/sensor/data/DriverBrakingEvent.h"

namespace carla {
namespace sensor {
namespace s11n {

  SharedPtr<SensorData> DriverTestSerializer::Deserialize(RawData &&data) {
    
    return SharedPtr<SensorData>(new data::DriverBrakingEvent(std::move(data)));
  }

} // namespace s11n
} // namespace sensor
} // namespace carla
