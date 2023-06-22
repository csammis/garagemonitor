#pragma once

#include <Arduino.h>

namespace GarageMonitor {
namespace Networking {

void Networking_setup();
int GetRSSI();
void Networking_teardown();
String GetMAC();

}  // namespace Networking
}  // namespace GarageMonitor