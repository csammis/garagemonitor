#include "DistanceSensor.hpp"
#include <ArduinoJson.h>
#include <Arduino.h>
#include "NetworkManager.hpp"
#include "HCSR04.h"

using GarageMonitor::Distance::Sensor;

static const String ConfigTopic = "homeassistant/sensor/garagemonitorD/config";
static const String AttributesTopic = "homeassistant/sensor/garagemonitorD/attributes";
static const String StateTopic = "homeassistant/sensor/garagemonitorD/state";

// Temperature probe objects
constexpr int oneWirePin = 4;

Sensor::Sensor(MQTT::MqttClient* mqtt) : mMqtt(mqtt) {}

void Sensor::PublishNewReading() {
  static constexpr uint8_t TriggerPin = 4;
  static constexpr uint8_t EchoPin = 5;

  UltraSonicDistanceSensor probe(TriggerPin, EchoPin);

  StaticJsonDocument<64> stateDoc;
  float sample_sum = 0.0f;
  for (auto i = 0; i < 10; i++) {
    sample_sum += probe.measureDistanceCm();
  }
  stateDoc["distance"] = sample_sum / 10.0f;

  // Gather data related to the pond monitor station
  StaticJsonDocument<128> attributesDoc;
  attributesDoc["rssi"] = Networking::GetRSSI();

  if (mMqtt->Connect()) {
    String state;
    serializeJson(stateDoc, state);
    mMqtt->Publish(StateTopic, state, true);

    String attributes;
    serializeJson(attributesDoc, attributes);
    mMqtt->Publish(AttributesTopic, attributes);
  }
}

//! Publish to the `config` topic for HomeAssitant's MQTT Discovery to pick this sensor up
void Sensor::PublishSensorConfiguration() {
  StaticJsonDocument<400> doc;
  doc["device_class"] = "distance";
  doc["state_class"] = "measurement";
  doc["name"] = "Garage Monitor - Distance";
  doc["object_id"] = "garage_monitor_distance";
  doc["unique_id"] = GarageMonitor::Networking::GetMAC() + "_distance";
  doc["availability_topic"] = mMqtt->LastWillTopic();
  doc["state_topic"] = StateTopic;
  doc["json_attributes_topic"] = AttributesTopic;
  doc["unit_of_measurement"] = "mm";
  doc["value_template"] = "{{ value_json.distance }}";

  String json;
  serializeJson(doc, json);

  if (mMqtt->Connect()) {
    mMqtt->Publish(ConfigTopic, json, true);
  }
}
