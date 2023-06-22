#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <memory>

namespace GarageMonitor {
namespace MQTT {

class MqttClient {
 public:
  MqttClient(const IPAddress& server, uint16_t port);
  bool Connect();
  bool IsConnected();
  bool Publish(const String& topic, const String& payload);
  bool Publish(const String& topic, const String& payload, bool retainTopic);
  void Disconnect();
  const String& LastWillTopic() const;

 private:
  PubSubClient client;
};

std::unique_ptr<MqttClient> MQTT_setup();

}  // namespace MQTT
}  // namespace GarageMonitor