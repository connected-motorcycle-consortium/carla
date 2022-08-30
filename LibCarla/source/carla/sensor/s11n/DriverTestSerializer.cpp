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
