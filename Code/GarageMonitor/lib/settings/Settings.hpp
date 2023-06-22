#pragma once

#include <Arduino.h>
#include <IPAddress.h>

namespace GarageMonitor {
namespace Settings {

//! The home network's SSID and password
String SSID();
String Password();

//! The server where the MQTT broker is listening
IPAddress MqttServer();
//! The port of the MQTT broker
uint16_t MqttPort();

//! MQTT broker username
String MqttUsername();
//! MQTT broker password
String MqttPassword();

}  // namespace Settings
}  // namespace GarageMonitor