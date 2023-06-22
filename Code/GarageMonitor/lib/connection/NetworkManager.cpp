#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>
#include "NetworkManager.hpp"
#include "Settings.hpp"

#define USE_ESP_WIFIMANAGER_NTP false
#include <ESP_WiFiManager.h>

WiFiClient* client = nullptr;

//! Connect in STA mode to the network that was configured via the portal
static void connectWithStoredCredentials() {
  //  GarageMonitor::Debug::Print("Connecting to network with stored credentials");

  ESP8266WiFiMulti connector;
  connector.addAP(GarageMonitor::Settings::SSID().c_str(), GarageMonitor::Settings::Password().c_str());
  auto connectionAttempts = 0;
  auto status = connector.run();

  do {
    // GarageMonitor::Status::WifiConnectionInProgress();
    status = WiFi.status();
  } while (connectionAttempts++ < 20 && status != WL_CONNECTED);

  if (status == WL_CONNECTED) {
    // GarageMonitor::Debug::Print("Connected to network");
  } else {
    // GarageMonitor::Debug::Print("Could not connect after 20 attempts");
    ESP.restart();
  }
}

//! Stop the networking processes cleanly
void GarageMonitor::Networking::Networking_teardown() { WiFi.disconnect(true); }

// Call from setup() to initialize the WiFi connection components
void GarageMonitor::Networking::Networking_setup() { connectWithStoredCredentials(); }

int GarageMonitor::Networking::GetRSSI() { return WiFi.RSSI(); }

String GarageMonitor::Networking::GetMAC() { return WiFi.macAddress(); }