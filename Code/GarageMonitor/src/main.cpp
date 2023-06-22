#include <Arduino.h>
#include "NetworkManager.hpp"
#include "Mqtt.hpp"
#include "DistanceSensor.hpp"

std::unique_ptr<GarageMonitor::MQTT::MqttClient> mqtt;
std::unique_ptr<GarageMonitor::Distance::Sensor> sensor;

void setup() {
  pinMode(D0, WAKEUP_PULLUP);
  Serial.begin(74880);

  Serial.write("connecting\n");
  GarageMonitor::Networking::Networking_setup();
  mqtt = GarageMonitor::MQTT::MQTT_setup();
  sensor = std::make_unique<GarageMonitor::Distance::Sensor>(mqtt.get());
  sensor->PublishSensorConfiguration();

  // Apparently my no-name several-year-old ESP8266 modules have an issue with
  // not properly resetting after wakeup from deep sleep. Might have something
  // to do with D0 (GPIO16) not dragging RST low long enough to reset the chip.
  //
  //
  //
  // but this module is going to be plugged into the wall sitting in my garage
  // so a big healthy *shrug* is my response.
}

void loop() {
  Serial.write("measuring\n");
  sensor->PublishNewReading();
  delay(5 * 1000);
}
