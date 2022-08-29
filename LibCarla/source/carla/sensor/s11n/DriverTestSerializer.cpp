#include "carla/sensor/s11n/DriverTestSerializer.h"
#include "carla/sensor/data/DriverBreakingEvent.h"

namespace carla {
namespace sensor {
namespace s11n {

  SharedPtr<SensorData> DriverTestSerializer::Deserialize(RawData &&data) {
    
    return SharedPtr<SensorData>(new data::DriverBreakingEvent(std::move(data)));
  }

} // namespace s11n
} // namespace sensor
} // namespace carla
