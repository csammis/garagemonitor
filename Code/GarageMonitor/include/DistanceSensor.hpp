#pragma once

#include "Mqtt.hpp"

namespace GarageMonitor {
namespace Distance {
class Sensor {
 public:
  Sensor(MQTT::MqttClient* mqtt);

  void PublishSensorConfiguration();
  void PublishNewReading();

 private:
  MQTT::MqttClient* mMqtt;
};

}  // namespace Distance
}  // namespace GarageMonitor