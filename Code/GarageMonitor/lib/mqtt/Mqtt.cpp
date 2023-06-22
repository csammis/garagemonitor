#include "Mqtt.hpp"
#include "Settings.hpp"

#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

WiFiClient wifi;

using GarageMonitor::MQTT::MqttClient;

static String LWTTopic = "homeassistant/sensor/garagemonitorD/status";

std::unique_ptr<MqttClient> GarageMonitor::MQTT::MQTT_setup() {
  using namespace GarageMonitor;
  return std::unique_ptr<MqttClient>(new MqttClient(Settings::MqttServer(), Settings::MqttPort()));
}

MqttClient::MqttClient(const IPAddress& server, uint16_t port) : client(wifi) {
  client.setServer(server, port);
  client.setBufferSize(1024);
}

bool MqttClient::Connect() {
  using namespace GarageMonitor;
  if (IsConnected()) {
    return true;
  }
  auto retval = client.connect("GarageMonitor", Settings::MqttUsername().c_str(), Settings::MqttPassword().c_str(),
                               LWTTopic.c_str(), MQTTQOS2, true, "offline");
  if (retval) {
    retval = Publish(LWTTopic, "online", true);
  }
  return retval;
}

void MqttClient::Disconnect() { client.disconnect(); }

bool MqttClient::IsConnected() { return client.connected(); }

const String& MqttClient::LastWillTopic() const { return LWTTopic; }

bool MqttClient::Publish(const String& topic, const String& payload) { return Publish(topic, payload, false); }

bool MqttClient::Publish(const String& topic, const String& payload, bool retainTopic) {
  if (IsConnected()) {
    client.publish(topic.c_str(), payload.c_str(), retainTopic);
    return true;
  }
  return false;
}
